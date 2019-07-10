#include "function.h"
#include <QMetaMethod>
#include <QStringList>
#include <QDebug>

namespace NeovimQt {

typedef QPair<QString,QString> StringPair;

Function::Function(const QString& ret, const QString& name,
	QList<QPair<QString,QString>> params,bool can_fail) :
	m_valid{ true },
	m_can_fail{ can_fail },
	m_return_type{ ret },
	m_name{ name },
	m_parameters{ params }
{
}

Function::Function(const QString& ret, const QString& name, QList<QString> paramTypes,
	bool can_fail) :
	m_valid(true),
	m_can_fail{ can_fail },
	m_return_type{ ret },
	m_name{ name }
{
	for(const auto& type : paramTypes) {
		m_parameters.append({ type, QString{} });
	}
}

static QList<QPair<QString, QString>> parseParameters(const QVariantList& obj)
{
	QList<QPair<QString,QString>> res;
	for (const auto& val : obj) {

		const QVariantList& params = val.toList();
		if (params.size() % 2 != 0) {
			return {};
		}

		for (int i=0;i<params.size();i+=2) {
			if (!params.at(i).canConvert<QByteArray>() ||
				!params.at(i + 1).canConvert<QByteArray>()) {
				return {};
			}

			res.append({ QString::fromUtf8(params.at(i).toByteArray()),
					QString::fromUtf8(params.at(i + 1).toByteArray()) });
		}
	}
	return res;
}

Function::Function(const QVariant& function)
{
	if (!function.canConvert<QVariantMap>()) {
		qDebug() << "Found unexpected data type when unpacking function" << function;
		return;
	}

	QMapIterator<QString, QVariant> it(function.toMap());
	while (it.hasNext()) {
		it.next();

		if (it.key() == "return_type" && it.value().canConvert<QByteArray>()) {
			m_return_type = QString::fromUtf8(it.value().toByteArray());
			continue;
		}

		if (it.key() == "name" && it.value().canConvert<QByteArray>()) {
			m_name = QString::fromUtf8(it.value().toByteArray());
			continue;
		}

		if (it.key() == "can_fail" && it.value().canConvert<bool>()) {
			m_can_fail = it.value().toBool();
			continue;
		}

		if (it.key() == "parameters" && it.value().canConvert<QVariantList>()) {
			m_parameters = parseParameters(it.value().toList());
			continue;
		}

		// Unused function attributes, still valid
		if (it.key() != "id" ||
			it.key() != "receives_channel_id" ||
			it.key() != "impl_name" ||
			it.key() != "method" ||
			it.key() != "noeval" ||
			it.key() != "deferred" ||
			it.key() == "async" ||
			it.key() == "deprecated_since" ||
			it.key() == "since") {
			continue;
		}

		qDebug() << "Unsupported or malformed function attribute" << it.key() << it.value();
	}

	m_valid = true;
}

/*static*/ Function Function::fromVariant(const QVariant& function)
{
	return { function };
}

bool Function::operator==(const Function& other)
{
	if (m_name != other.m_name) {
		return false;
	}

	if (m_return_type != other.m_return_type) {
		return false;
	}

	if (m_parameters.size() != other.m_parameters.size()) {
		return false;
	}

	for (int i=0; i<m_parameters.size(); i++) {
		if (m_parameters.at(i).first != other.m_parameters.at(i).first) {
			return false;
		}
	}

	return true;
}

} // namespace NeovimQt
