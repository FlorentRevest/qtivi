#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include "basecontroller.h"
#include "models.h"

namespace mopidy {

class PlaybackController : public BaseController
{
    Q_OBJECT

public:
    explicit PlaybackController();
    ~PlaybackController();

    //
    void play();
    void play(const mopidy::TlTrack &tltrack);
    void play(int tlid);
    void next();
    void previous();
    void stop();
    void pause();
    void resume();
    void seek(int timePosition);

    // Current track
    void getCurrentTlTrack();
    void getCurrentTrack();
    void getStreamTitle();
    void getTimePosition();
    void getState();
    void setState(const mopidy::PlaybackState &state);

signals:
    void currentTlTrackReceived(const mopidy::TlTrack &tltrack);
    void currentTrackReceived(const mopidy::Track &track);
    void stateReceived(mopidy::PlaybackState state);
    void streamTitleReceived(const QString &title);
    void timePositionReceived(int tp);
    void seeked(bool ack);
};

} // mopidy

#endif //PLAYBACKCONTROLLER_H
