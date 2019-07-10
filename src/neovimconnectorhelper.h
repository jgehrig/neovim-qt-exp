#ifndef NEOVIM_QT_CONNECTORHELPER
#define NEOVIM_QT_CONNECTORHELPER

#include "neovimconnector.h"

namespace NeovimQt {

class NeovimConnectorHelper: public QObject
{
	Q_OBJECT
public:
	NeovimConnectorHelper(NeovimConnector *);

public slots:
	void handleMetadata(uint32_t, uint64_t, const QVariant& result);
	void handleMetadataError(uint32_t msgid, uint64_t,
		const QVariant& errobj);
private:
	NeovimConnector *m_c;

};

} // Namespace NeovimQt
#endif
