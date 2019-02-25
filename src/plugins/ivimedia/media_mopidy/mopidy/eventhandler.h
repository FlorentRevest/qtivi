#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QObject>
#include "jsonrpchandler.h"
#include "models.h"

namespace mopidy {

class EventHandlerPrivate;
class EventHandler : public QObject
{
    Q_OBJECT

public:
    EventHandler();
    ~EventHandler();

    void setJsonRpcHandler(QSharedPointer<mopidy::JsonRpcHandler> jsRpcHdl);

signals:
    void optionsChanged();
    void playbackStateChanged(mopidy::PlaybackState oldState, mopidy::PlaybackState newState);
    void playlistChanged(const mopidy::Playlist &playlist);
    void playlistDeleted(const QString &uri);
    void playlistsLoaded();
    void seeked(int time_position);
    void trackPlaybackEnded(const mopidy::TlTrack &tl_track, int time_position);
    void trackPlaybackPaused(const mopidy::TlTrack &tl_track, int time_position);
    void trackPlaybackResumed(const mopidy::TlTrack &tl_track, int time_position);
    void trackPlaybackStarted(const mopidy::TlTrack &tl_track);
    void tracklistChanged();
    void streamTitleChanged(const QString &title);

private:
    Q_DECLARE_PRIVATE(EventHandler)
    QScopedPointer<EventHandlerPrivate> d_ptr;
};

} // mopidy

#endif //EVENTHANDLER_H
