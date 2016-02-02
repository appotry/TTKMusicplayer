#ifndef MUSICBACKGROUNDREMOTEWIDGET_H
#define MUSICBACKGROUNDREMOTEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QWidget>
#include "musicglobaldefine.h"
#include "musicobject.h"

class QButtonGroup;
class QListWidgetItem;
class MusicBackgroundListWidget;
class MusicDownloadQueueCache;

class MUSIC_WIDGET_EXPORT MusicBackgroundRemoteWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MusicBackgroundRemoteWidget(QWidget *parent = 0);
    ~MusicBackgroundRemoteWidget();

Q_SIGNALS:
    void showCustomSkin(const QString &path);

public Q_SLOTS:
    void buttonClicked(int index);
    void itemUserClicked(QListWidgetItem *item);

protected:
    void initWidget();
    void createButton();
    void createUrls();
    QStringList createPaths();

    MStringLists m_urls;
    MusicBackgroundListWidget *m_listWidget;
    MusicDownloadQueueCache *m_downloadQueue;
    QButtonGroup *m_group;
    int m_currentIndex;

};

#endif // MUSICBACKGROUNDREMOTEWIDGET_H
