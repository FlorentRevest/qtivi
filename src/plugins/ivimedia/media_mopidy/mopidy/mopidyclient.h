#ifndef MOPIDY_CLIENT_H
#define MOPIDY_CLIENT_H

#include "jsonrpchandler.h"
#include <QObject>

namespace mopidy {

class MopidyClientPrivate;
class MopidyClient : public QObject
{
    Q_OBJECT

public:
    MopidyClient(QObject *parent = 0);
    ~MopidyClient();

    void connectToServer(const QString &host = "127.0.0.1",
                         quint16 port        = 6680,
                         const QString &path = "/mopidy/ws");
    void disconnectFromServer();

    QSharedPointer<JsonRpcHandler> jsonrpcHandler();

signals:
    // connections signals
    void clientConnected();
    void clientDisconnected();
    void connectionError(int code, const QString &message);

private:
    Q_DECLARE_PRIVATE(MopidyClient)
    QScopedPointer<MopidyClientPrivate> d_ptr;
};

} // mopidy

#endif //MOPIDY_CLIENT_H
