#ifndef MUSICDOWNLOADSTATUSLABEL_H
#define MUSICDOWNLOADSTATUSLABEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerysinglethread.h"
#include "musicglobaldefine.h"

class QLabel;
class MusicApplication;

class MUSIC_WIDGET_EXPORT MusicDownloadStatusLabel : public QObject
{
    Q_OBJECT
public:
    explicit MusicDownloadStatusLabel(QWidget *w);
    ~MusicDownloadStatusLabel();

    void setMovieLabel(QLabel *label) { m_movieLabel = label;}
    void musicCheckHasLrcAlready();
    bool checkSettingParameterValue() const;

public Q_SLOTS:
    void musicHaveNoLrcAlready();
    /////////////////////////////////////////////
    ///This is a slot by MusicSongSearchOnlineWidget's signal emit
    void showDownLoadInfoFor(MusicObject::DownLoadType type);
    void showDownLoadInfoFinished(const QString &type);
    void networkConnectionStateChanged(bool state);

protected:
    MusicApplication *m_parentWidget;
    MusicDownLoadQueryThreadAbstract *m_downloadLrcThread;
    QLabel *m_movieLabel;

};

#endif // MUSICDOWNLOADSTATUSLABEL_H
