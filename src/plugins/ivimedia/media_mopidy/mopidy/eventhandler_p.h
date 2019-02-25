#ifndef EVENTHANDLER_P_H
#define EVENTHANDLER_P_H

#include "eventhandler.h"

namespace mopidy {

class EventHandlerPrivate : public QObject
{
    Q_OBJECT

public:
    EventHandlerPrivate(EventHandler *parent);
    ~EventHandlerPrivate();

    QSharedPointer<mopidy::JsonRpcHandler> jsRpcHdl;

public slots:
    void onEventReceived(const QString &event, const QJsonObject &params);

private:
    EventHandler *const q_ptr;
    Q_DECLARE_PUBLIC(EventHandler)
};
}

#endif //EVENTHANDLER_P_H
