#ifndef JSONRPCHANDLER_H
#define JSONRPCHANDLER_H

#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QWebSocket>

#include <functional>

namespace mopidy {

class JsonRpcHandlerPrivate;
class JsonRpcHandler : public QObject
{
    Q_OBJECT

public:
    enum class RpcErrorCode
    {
        JsonRpcError,
        SocketError
    };

    // Handler used to process reponse to requests
    typedef std::function<void(const QJsonValue &)> ResponseHandler;

    explicit JsonRpcHandler(QObject *parent = Q_NULLPTR);
    virtual ~JsonRpcHandler();

    void setSocket(QSharedPointer<QWebSocket> ws);

    void sendRequest(const QString &method, ResponseHandler rhdl, const QJsonObject &params = QJsonObject());
    void sendNotification(const QString &method, const QJsonObject &params = QJsonObject());

signals:
    void eventReceived(const QString &event, const QJsonObject &params);
    void error(RpcErrorCode ec, const QJsonObject &jsonRpcErrorObj);

private:
    Q_DECLARE_PRIVATE(JsonRpcHandler)
    QScopedPointer<JsonRpcHandlerPrivate> d_ptr;
};

} // mopidy

#endif //JSONRPCHANDLER_H
