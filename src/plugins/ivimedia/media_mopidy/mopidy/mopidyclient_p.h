#ifndef MOPIDY_CLIENT_P_H
#define MOPIDY_CLIENT_P_H

#include <QSharedPointer>
#include <QWebSocket>

#include "jsonrpchandler.h"
#include "mopidyclient.h"

namespace mopidy {

class MopidyClientPrivate : public QObject
{
    Q_OBJECT

public:
    MopidyClientPrivate(MopidyClient *parent);
    ~MopidyClientPrivate();

    QSharedPointer<JsonRpcHandler> jsrpcHdl;
    QSharedPointer<QWebSocket> ws;

public slots:
    void onWsError(QAbstractSocket::SocketError error);

public:
    MopidyClient *const q_ptr;
    Q_DECLARE_PUBLIC(MopidyClient)
};

} // mopidy

#endif //MOPIDY_CLIENT_P_H
