/****************************************************************************
**
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "mediaplayerbackend.h"
#include "searchandbrowsebackend.h"

MediaPlayerBackend::MediaPlayerBackend(QSharedPointer<mopidy::JsonRpcHandler> jrhdl, QObject *parent)
    : QIviMediaPlayerBackendInterface(parent)
    , m_state(QIviMediaPlayer::Stopped)
    , m_repeatMode(false)
    , m_singleMode(false)
    , m_randomMode(false)
{
    //    m_timeTimer.setInterval(1000);
    //    connect(&m_timeTimer, &QTimer::timeout, this, &MediaPlugin::onTimeTimeout);

    m_trckLstCtrl.setJsonRpcHandler(jrhdl);
    m_playbackCtrl.setJsonRpcHandler(jrhdl);
    m_evtHdl.setJsonRpcHandler(jrhdl);

    connect(&m_trckLstCtrl, &mopidy::TracklistController::singleReceived, this, &MediaPlayerBackend::onPcSingleReceived);
    connect(&m_trckLstCtrl, &mopidy::TracklistController::repeatReceived, this, &MediaPlayerBackend::onPcRepeatReceived);
    connect(&m_trckLstCtrl, &mopidy::TracklistController::randomReceived, this, &MediaPlayerBackend::onPcRandomReceived);

    connect(&m_evtHdl, &mopidy::EventHandler::playbackStateChanged, this, &MediaPlayerBackend::onEvPlaybackStateChanged);
    connect(&m_evtHdl, &mopidy::EventHandler::streamTitleChanged, this, &MediaPlayerBackend::onEvStreamTitleChanged);

    connect(&m_playbackCtrl, &mopidy::PlaybackController::stateReceived, this, &MediaPlayerBackend::onPcStateReceived);
    connect(&m_playbackCtrl, &mopidy::PlaybackController::timePositionReceived, this, &MediaPlayerBackend::onPcTimePositionReceived);
    connect(&m_playbackCtrl, &mopidy::PlaybackController::currentTlTrackReceived, this, &MediaPlayerBackend::onPcCurrentTlTrackReceived);
    connect(&m_playbackCtrl, &mopidy::PlaybackController::streamTitleReceived, this, &MediaPlayerBackend::onPcStreamTitleReceived);
}

void MediaPlayerBackend::initialize()
{
    emit durationChanged(0);
    emit positionChanged(0);
    emit initializationDone();
}

void MediaPlayerBackend::play()
{
    m_playbackCtrl.play();
}

void MediaPlayerBackend::pause()
{
    m_playbackCtrl.pause();
}

void MediaPlayerBackend::stop()
{
    m_playbackCtrl.stop();
}

void MediaPlayerBackend::seek(qint64 offset)
{
    // TODO: position + offset
    m_playbackCtrl.seek(offset);
}

void MediaPlayerBackend::next()
{
    m_playbackCtrl.next();
}

void MediaPlayerBackend::previous()
{
    m_playbackCtrl.previous();
}

void MediaPlayerBackend::setPlayMode(QIviMediaPlayer::PlayMode playMode)
{
    switch(playMode) {
    case QIviMediaPlayer::Normal:
        m_trckLstCtrl.setSingle(false);
        m_trckLstCtrl.setRepeat(false);
        m_trckLstCtrl.setRandom(false);
        break;
    case QIviMediaPlayer::RepeatTrack:
        m_trckLstCtrl.setSingle(true);
        m_trckLstCtrl.setRepeat(true);
        m_trckLstCtrl.setRandom(false);
        break;
    case QIviMediaPlayer::RepeatAll:
        m_trckLstCtrl.setSingle(false);
        m_trckLstCtrl.setRepeat(true);
        m_trckLstCtrl.setRandom(false);
        break;
    case QIviMediaPlayer::Shuffle:
        m_trckLstCtrl.setSingle(false);
        m_trckLstCtrl.setRepeat(false);
        m_trckLstCtrl.setRandom(true);
        break;
    };
}

void MediaPlayerBackend::setPosition(qint64 position)
{
    m_playbackCtrl.seek(position);
}

bool MediaPlayerBackend::canReportCount()
{
    return true;
}

void MediaPlayerBackend::fetchData(int start, int count)
{
    m_trckLstCtrl.getTracks();
}

void MediaPlayerBackend::insert(int index, const QIviPlayableItem *item)
{
}

void MediaPlayerBackend::remove(int index)
{
}

void MediaPlayerBackend::move(int cur_index, int new_index)
{
}

void MediaPlayerBackend::setCurrentIndex(int index)
{
}

void MediaPlayerBackend::updateState()
{
    m_playbackCtrl.getState();
}

void MediaPlayerBackend::onPcCurrentTlTrackReceived(const mopidy::TlTrack &tlTrack)
{
    // time
    emit durationChanged(tlTrack.track.length);

    // informations
    QStringList lsArtists;
    for (auto artist : tlTrack.track.artists)
        lsArtists << artist.name;
    //m_ui->lbCurrentTrackArtists->setText(lsArtists.join(", "));

    //m_ui->lbCurrentTrackTitle->setText(tlTrack.track.name);

    // request cover
    //m_libraryCtrl.getImages({tlTrack.track.album.uri});
}

void MediaPlayerBackend::onPcStateReceived(mopidy::PlaybackState state)
{
    switch (state)
    {
    case mopidy::PlaybackState::Paused:
        m_state = QIviMediaPlayer::Paused;
        m_playbackCtrl.getCurrentTlTrack();
        //m_timeTimer.stop();
        break;

    case mopidy::PlaybackState::Playing:
        m_state = QIviMediaPlayer::Playing;
        m_playbackCtrl.getCurrentTlTrack();
        m_playbackCtrl.getTimePosition();
        break;

    case mopidy::PlaybackState::Stopped:
        m_state = QIviMediaPlayer::Stopped;
        //m_timeTimer.stop();
        emit positionChanged(0);
        break;
    }

    emit playStateChanged(m_state);
}

void MediaPlayerBackend::onPcTimePositionReceived(int tp)
{
    emit positionChanged(tp);
    //if (!m_timeTimer.isActive())
    //    m_timeTimer.start();
}

void MediaPlayerBackend::onPcStreamTitleReceived(const QString &title)
{
    //m_ui->lbCurrentTrackTitle->setText(title);
}

void MediaPlayerBackend::onEvPlaybackStateChanged(mopidy::PlaybackState oldState, mopidy::PlaybackState newState)
{
    onPcStateReceived(newState);
}

void MediaPlayerBackend::onEvStreamTitleChanged(const QString &title)
{
    //m_ui->lbCurrentTrackTitle->setText(title);
}

void MediaPlayerBackend::changePlayMode()
{
    QIviMediaPlayer::PlayMode playMode = QIviMediaPlayer::Normal;
    if(!m_singleMode && !m_repeatMode && m_randomMode)
        playMode = QIviMediaPlayer::Shuffle;
    else if(m_singleMode && m_repeatMode && !m_randomMode)
        playMode = QIviMediaPlayer::RepeatTrack;
    else if(!m_singleMode && m_repeatMode && !m_randomMode)
        playMode = QIviMediaPlayer::RepeatAll;

    emit playModeChanged(playMode);
}

void MediaPlayerBackend::onPcSingleReceived(bool isSingle)
{
    m_singleMode = isSingle;
    changePlayMode();
}

void MediaPlayerBackend::onPcRepeatReceived(bool isRepeat)
{
    m_repeatMode = isRepeat;
    changePlayMode();
}

void MediaPlayerBackend::onPcRandomReceived(bool isRandom)
{
    m_randomMode = isRandom;
    changePlayMode();
}
