#include "basecontroller.h"
#include "basecontroller_p.h"

using namespace mopidy;

BaseControllerPrivate::BaseControllerPrivate(const QString &ctrlName)
    : controllerName(ctrlName)
{
}

BaseControllerPrivate::~BaseControllerPrivate()
{
}

BaseController::BaseController(const QString &controllerName)
    : d_ptr(new BaseControllerPrivate(controllerName))
{
}

BaseController::~BaseController()
{
}

void BaseController::setJsonRpcHandler(QSharedPointer<JsonRpcHandler> jsRpcHdl)
{
    Q_D(BaseController);
    d->rpc = jsRpcHdl;
}

void BaseController::sendRequest(const QString &method, JsonRpcHandler::ResponseHandler rhdl, const QJsonObject &params)
{
    Q_D(BaseController);
    if (d->rpc)
        d->rpc->sendRequest(QString("%1.%2").arg(d->controllerName).arg(method), rhdl, params);
}

void BaseController::sendNotification(const QString &method, const QJsonObject &params)
{
    Q_D(BaseController);
    if (d->rpc)
        d->rpc->sendNotification(QString("%1.%2").arg(d->controllerName).arg(method), params);
}
