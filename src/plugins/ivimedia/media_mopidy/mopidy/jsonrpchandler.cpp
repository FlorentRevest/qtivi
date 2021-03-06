#include "jsonrpchandler.h"
#include "jsonrpchandler_p.h"

#include <QJsonDocument>
#include <QMutexLocker>

using namespace mopidy;

JsonRpcHandlerPrivate::JsonRpcHandlerPrivate(JsonRpcHandler *parent)
    : q_ptr(parent),
      m_connected(false)
{
}

JsonRpcHandlerPrivate::~JsonRpcHandlerPrivate()
{
}

bool JsonRpcHandlerPrivate::sendJsonRpcMessage(const QString &method, const QJsonObject &params, const QString &id)
{
    if (!ws)
        return false;

    // create message
    QJsonObject message;
    message["jsonrpc"] = "2.0";
    message["method"]  = method;
    if (!params.isEmpty())
        message["params"] = params;
    if (!id.isEmpty())
        message["id"] = id;

    // send it if connected or queue it for when we are connected
    QJsonDocument jdoc(message);
    QByteArray data = jdoc.toJson(QJsonDocument::Compact);
    if(m_connected) {
        qint64 sent = ws->sendTextMessage(data);
        return (sent == data.size());
    } else {
        m_msgsQueue.enqueue(data);
        return true;
    }
}

QString JsonRpcHandlerPrivate::buildMsgId()
{
    return QString("qtivi_media_mopidy_%1%2")
        .arg(QDateTime::currentMSecsSinceEpoch())
        .arg(requests.size());
}

void JsonRpcHandlerPrivate::onClientConnected()
{
    m_connected = true;

    while (!m_msgsQueue.isEmpty()) {
         QByteArray data = m_msgsQueue.dequeue();
         qDebug() << data;
         ws->sendTextMessage(data);
    }
}

void JsonRpcHandlerPrivate::onClientDisconnected()
{
    m_connected = false;
}

void JsonRpcHandlerPrivate::onWsTextReceived(const QString &message)
{
    Q_Q(JsonRpcHandler);

    QJsonParseError jpe;
    QJsonDocument jdoc = QJsonDocument::fromJson(message.toLocal8Bit(), &jpe);

    if (jpe.error != QJsonParseError::NoError)
    {
        emit q->error(
            JsonRpcHandler::RpcErrorCode::SocketError,
            {{"error", QString("Cannot parse message: %1 : %2").arg(message).arg(jpe.errorString())}});
        return;
    }

    auto wsresponse = jdoc.object();

    // reponse to request or notification ?
    if (wsresponse.contains("id"))
    {
        auto id = wsresponse.value("id").toString();
        if (requests.contains(id))
        {
            auto hdl = requests.take(id);
            hdl(wsresponse.value("result"));
        }
        else
        {
            emit q->error(
                JsonRpcHandler::RpcErrorCode::JsonRpcError,
                {{"error", QString("Unknown id %1").arg(id)}});
        }
    }
    else if (wsresponse.contains("event"))
    {
        auto method = wsresponse.take("event").toString();
        emit q->eventReceived(method, wsresponse);
    }
    else
    {
        qDebug() << "Unknow message:" << wsresponse;
    }
}

JsonRpcHandler::JsonRpcHandler(QObject *parent)
    : QObject(parent), d_ptr(new JsonRpcHandlerPrivate(this))
{
}

JsonRpcHandler::~JsonRpcHandler()
{
}

void JsonRpcHandler::setSocket(QSharedPointer<QWebSocket> ws)
{
    Q_D(JsonRpcHandler);
    d->ws = ws;
    if (d->ws) {
        connect(d->ws.data(), &QWebSocket::textMessageReceived, d, &JsonRpcHandlerPrivate::onWsTextReceived, Qt::QueuedConnection);
        connect(d->ws.data(), &QWebSocket::connected, d, &JsonRpcHandlerPrivate::onClientConnected);
        connect(d->ws.data(), &QWebSocket::disconnected, d, &JsonRpcHandlerPrivate::onClientDisconnected);
    }

}

void JsonRpcHandler::sendNotification(const QString &method, const QJsonObject &params)
{
    Q_D(JsonRpcHandler);
    QMutexLocker lock(&d->mutex);

    if (!d->sendJsonRpcMessage(method, params))
        emit error(RpcErrorCode::SocketError, {});
}

void JsonRpcHandler::sendRequest(const QString &method, JsonRpcHandler::ResponseHandler rhdl, const QJsonObject &params)
{
    Q_D(JsonRpcHandler);
    QMutexLocker lock(&d->mutex);

    auto id = d->buildMsgId();
    if (d->sendJsonRpcMessage(method, params, id) && rhdl)
        d->requests.insert(id, rhdl);
    else
        emit error(RpcErrorCode::SocketError, {});
}
