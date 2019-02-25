#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <QObject>
#include "jsonrpchandler.h"

namespace mopidy {

class BaseControllerPrivate;
class BaseController : public QObject
{
    Q_OBJECT

public:
    explicit BaseController(const QString &controllerName);
    virtual ~BaseController();

    void setJsonRpcHandler(QSharedPointer<mopidy::JsonRpcHandler> jsRpcHdl);

protected:
    // shortcut to add full method name
    void sendRequest(const QString &method, mopidy::JsonRpcHandler::ResponseHandler rhdl, const QJsonObject &params = QJsonObject());
    void sendNotification(const QString &method, const QJsonObject &params = QJsonObject());

private:
    Q_DECLARE_PRIVATE(BaseController)
    QScopedPointer<BaseControllerPrivate> d_ptr;
};

} // mopidy

#endif //ABSTRACTCONTROLLER_H
