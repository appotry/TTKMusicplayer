#ifndef MUSICUSERWINDOW_H
#define MUSICUSERWINDOW_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QStackedWidget>
#include "musicglobaldefine.h"

class MusicUserDialog;
class MusicUserManager;

namespace Ui {
class MusicUserWindow;
}

class MUSIC_USER_EXPORT MusicUserWindow : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MusicUserWindow(QWidget *parent = 0);
    ~MusicUserWindow();

Q_SIGNALS:

public Q_SLOTS:
    void musicUserLogin();
    void userStateChanged(const QString &uid, const QString &icon);
    void musicUserContextLogin();
    void checkToAutoLogin();

protected:
    Ui::MusicUserWindow *ui;
    bool connectDatabase();
    bool disConnectDatabase();

    MusicUserManager *m_userManager;

};

#endif // MUSICUSERWINDOW_H
