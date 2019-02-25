#ifndef JSONRPCHANDLER_P_H
#define JSONRPCHANDLER_P_H

#include "jsonrpchandler.h"

#include <QMap>
#include <QMutex>
#include <QQueue>
#include <QByteArray>

namespace mopidy {

class JsonRpcHandlerPrivate : public QObject
{
    Q_OBJECT

public:
    explicit JsonRpcHandlerPrivate(JsonRpcHandler *parent);
    virtual ~JsonRpcHandlerPrivate();

    QSharedPointer<QWebSocket> ws;
    QMap<QString, JsonRpcHandler::ResponseHandler> requests;
    QMutex mutex;

    QString buildMsgId();
    bool sendJsonRpcMessage(const QString &method, const QJsonObject &params = QJsonObject(), const QString &id = QString());

public slots:
    void onWsTextReceived(const QString &message);    
    void onClientConnected();
    void onClientDisconnected();

private:
    JsonRpcHandler *const q_ptr;
    Q_DECLARE_PUBLIC(JsonRpcHandler)
    QQueue<QByteArray> m_msgsQueue;
    bool m_connected;
};

} // mopidy

#endif //JSONRPCHANDLER_P_H
