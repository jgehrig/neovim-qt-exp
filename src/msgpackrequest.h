#ifndef NEOVIM_QT_REQUEST
#define NEOVIM_QT_REQUEST

#include <QObject>
#include <QTimer>

namespace NeovimQt {

class MsgpackIODevice;
class MsgpackRequest: public QObject {
	Q_OBJECT
public:
	MsgpackRequest(uint32_t id, MsgpackIODevice *dev, QObject *parent=0);
	void setFunction(uint64_t);
	uint64_t function();
	void setTimeout(int msec);
	/** The identifier for this Msgpack request */
	const uint32_t id;
signals:
	void finished(uint32_t msgid, uint64_t fun, const QVariant& resp);
	void error(uint32_t msgid, uint64_t fun, const QVariant& err);
	void timeout(uint32_t id);
protected slots:
	void requestTimeout();

private:
	MsgpackIODevice *m_dev;
	uint64_t m_function;
	QTimer m_timer;
};
} // Namespace

#endif
