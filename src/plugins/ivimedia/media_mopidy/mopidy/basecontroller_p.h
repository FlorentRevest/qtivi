#ifndef BASECONTROLLER_P_H
#define BASECONTROLLER_P_H

#include "jsonrpchandler.h"

namespace mopidy {

class BaseControllerPrivate
{
public:
    explicit BaseControllerPrivate(const QString &ctrlName);
    ~BaseControllerPrivate();

    QSharedPointer<mopidy::JsonRpcHandler> rpc;
    const QString controllerName;
};

} // mopidy

#endif //ABSTRACTCONTROLLER_P_H
