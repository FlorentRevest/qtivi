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

#ifndef MEDIAPLAYERBACKEND_H
#define MEDIAPLAYERBACKEND_H

#include <QtIviMedia/QIviMediaPlayerBackendInterface>

#include <mopidy/eventhandler.h>
#include <mopidy/jsonrpchandler.h>
#include <mopidy/playbackcontroller.h>
#include <mopidy/tracklistcontroller.h>

class MediaPlayerBackend : public QIviMediaPlayerBackendInterface
{
    Q_OBJECT

public:
    MediaPlayerBackend(QSharedPointer<mopidy::JsonRpcHandler> jrhdl, QObject *parent = nullptr);

    void initialize() override;
    void play() override;
    void pause() override;
    void stop() override;
    void seek(qint64 offset) override;
    void next() override;
    void previous() override;
    void setPlayMode(QIviMediaPlayer::PlayMode playMode) override;
    void setPosition(qint64 position) override;
    void setCurrentIndex(int index) override;

    bool canReportCount() override;
    void fetchData(int start, int count) override;

    void insert(int index, const QIviPlayableItem *item) override;
    void remove(int index) override;
    void move(int cur_index, int new_index) override;

signals:
    void playTrack(const QUrl& url);

public Q_SLOTS:
    void updateState();

private Q_SLOTS:
    // Event handler
    void onEvPlaybackStateChanged(mopidy::PlaybackState oldState, mopidy::PlaybackState newState);
    void onEvStreamTitleChanged(const QString &title);

    // PlaybackController
    void onPcCurrentTlTrackReceived(const mopidy::TlTrack &tlTrack);
    void onPcStateReceived(mopidy::PlaybackState state);
    void onPcTimePositionReceived(int tp);
    void onPcStreamTitleReceived(const QString &title);
    void onPcSingleReceived(bool isSingle);
    void onPcRepeatReceived(bool isRepeat);
    void onPcRandomReceived(bool isRandom);

private:
    void changePlayMode();

    QIviMediaPlayer::PlayState m_state;

    mopidy::TracklistController m_trckLstCtrl;
    mopidy::EventHandler m_evtHdl;
    mopidy::PlaybackController m_playbackCtrl;

    bool m_repeatMode, m_randomMode, m_singleMode;
};

#endif // MEDIAPLAYERBACKEND_H
