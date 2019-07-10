#pragma once

#include <QObject>
#include <QList>
#include <QPair>

namespace NeovimQt {

/// Representation of a Neovim API function signature
class Function {
	Q_ENUMS(FunctionId)

public:
	Function(const QString& ret, const QString& name, QList<QPair<QString, QString>> params,
		bool can_fail);

	Function(const QString& ret, const QString& name, QList<QString> paramTypes, bool can_fail);

	/// Constructs a function from the Neovim provided RPC message
	Function(const QVariant& function);

	// FIXME Issue#1: This should be removed when the API is regenerated
	static Function fromVariant(const QVariant& function);

	bool operator==(const Function& other);

	bool isValid() const { return m_valid; }

	bool canFailBeforeReturn() const { return m_can_fail; }

	QString getReturnType() const { return m_return_type; }

	QString getName() const { return m_name; }

	QList<QPair<QString, QString>> getParameters() const { return m_parameters; }

private:
	bool m_valid{ false };
	bool m_can_fail{ false };

	QString m_return_type;
	QString m_name;
	QList<QPair<QString, QString>> m_parameters;
};

} // namespace NeovimQt
