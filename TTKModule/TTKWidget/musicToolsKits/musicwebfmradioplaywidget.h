#ifndef MUSICWEBFMRADIOPLAYWIDGET_H
#define MUSICWEBFMRADIOPLAYWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "musicabstractmovewidget.h"

class MusicLrcAnalysis;
class MusicCoreMPlayer;
class MusicFMRadioSongsRequest;

namespace Ui {
class MusicWebFMRadioPlayWidget;
}

/*! @brief The class of the web music radio widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWebFMRadioPlayWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWebFMRadioPlayWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebFMRadioPlayWidget(QWidget *parent = nullptr);
    ~MusicWebFMRadioPlayWidget();

    /*!
     * Update radio song by given id.
     */
    void updateRadioSong(const QString &id);

public Q_SLOTS:
    /*!
     * Media aution play error.
     */
    void mediaAutionPlayError(int code);
    /*!
     * Set radio to play.
     */
    void radioPlay();
    /*!
     * Set radio to previous.
     */
    void radioPrevious();
    /*!
     * Set radio to next.
     */
    void radioNext();
    /*!
     * Set radio volume.
     */
    void radioVolume(int num);
    /*!
     * Radio resource download.
     */
    void radioResourceDownload();
    /*!
     * Query song information finished.
     */
    void querySongInfoFinished();
    /*!
     * Lrc download state changed.
     */
    void lrcDownloadStateChanged();
    /*!
     * Small pic download state changed.
     */
    void picDownloadStateChanged();
    /*!
     * Current position changed.
     */
    void positionChanged(qint64 position);
    /*!
     * Current duration changed.
     */
    void durationChanged(qint64 duration);
    /*!
     * Override show function.
     */
    void show();

protected:
    /*!
     * Override the widget event.
     */
    virtual void closeEvent(QCloseEvent *event) override final;
    /*!
     * Create core module.
     */
    void createCoreModule();

    Ui::MusicWebFMRadioPlayWidget *m_ui;
    bool m_isPlaying;
    QString m_currentIndex;
    MusicLrcAnalysis *m_analysis;
    MusicCoreMPlayer *m_player;
    MusicFMRadioSongsRequest *m_songThread;

};

#endif // MUSICWEBFMRADIOPLAYWIDGET_H
