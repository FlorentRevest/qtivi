#include "playbackcontroller.h"
#include "modeltranslator.h"

using namespace mopidy;

PlaybackController::PlaybackController()
    : BaseController("core.playback")
{
}

PlaybackController::~PlaybackController()
{
}

void PlaybackController::play()
{
    sendNotification("play");
}

void PlaybackController::play(const TlTrack &tltrack)
{
    QJsonObject params;

    if (tltrack.tlid >= 0)
        params.insert("tl_track", ModelTranslator::toJson(tltrack));
    else
        params.insert("tl_track", QJsonValue::Null);

    sendNotification("play", params);
}

void PlaybackController::play(int tlid)
{
    sendNotification("play", {{"tlid", tlid}});
}

void PlaybackController::next()
{
    sendNotification("next");
}

void PlaybackController::previous()
{
    sendNotification("previous");
}

void PlaybackController::stop()
{
    sendNotification("stop");
}

void PlaybackController::pause()
{
    sendNotification("pause");
}

void PlaybackController::resume()
{
    sendNotification("resume");
}

void PlaybackController::seek(int timePosition)
{
    auto rhdl = [&](const QJsonValue &v) { emit seeked(v.toBool()); };
    sendRequest("seek", rhdl, {{"time_position", timePosition}});
}

void PlaybackController::getCurrentTlTrack()
{
    auto rhdl = [&](const QJsonValue &v) {
        TlTrack tltrack;
        ModelTranslator::fromJson(v.toObject(), tltrack);
        emit currentTlTrackReceived(tltrack);
    };

    sendRequest("get_current_tl_track", rhdl);
}

void PlaybackController::getCurrentTrack()
{
    auto rhdl = [&](const QJsonValue &v) {
        Track track;
        ModelTranslator::fromJson(v.toObject(), track);
        emit currentTrackReceived(track);
    };

    sendRequest("get_current_track", rhdl);
}

void PlaybackController::getStreamTitle()
{
    auto rhdl = [&](const QJsonValue &v) { emit streamTitleReceived(v.toString()); };
    sendRequest("get_stream_title", rhdl);
}

void PlaybackController::getTimePosition()
{
    auto rhdl = [&](const QJsonValue &v) { emit timePositionReceived(v.toInt()); };
    sendRequest("get_time_position", rhdl);
}

void PlaybackController::getState()
{
    auto rhdl = [&](const QJsonValue &v) { emit stateReceived(ModelTranslator::getState(v.toString())); };
    sendRequest("get_state", rhdl);
}

void PlaybackController::setState(const PlaybackState &state)
{
    sendNotification("set_state", {{"new_state", ModelTranslator::getStateStr(state)}});
}
