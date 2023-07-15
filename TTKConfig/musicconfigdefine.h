#ifndef MUSICCONFIGDEFINE_H
#define MUSICCONFIGDEFINE_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include "musicobject.h"
#include "ttkversion.h"
#include "ttkglobaldefine.h"

//
#define TTK_DOWNLOAD_DIR_FULL       TTK::applicationPath() + TTK_PDIR + DOWNLOAD_DIR
#define TTK_LRC_DIR_FULL            TTK_DOWNLOAD_DIR_FULL + LRC_DIR
#define TTK_MUSIC_DIR_FULL          TTK_DOWNLOAD_DIR_FULL + MUSIC_DIR
#define TTK_MOVIE_DIR_FULL          TTK_DOWNLOAD_DIR_FULL + MOVIE_DIR
//
#define TTK_CACHE_DIR_FULL          CACHE_DIR_FULL
#define TTK_ART_DIR_FULL            ART_DIR_FULL
#define TTK_BACKGROUND_DIR_FULL     BACKGROUND_DIR_FULL
//
#define TTK_COFIG_PATH_FULL         COFIG_PATH_FULL
#define TTK_PLAYLIST_PATH_FULL      PLAYLIST_PATH_FULL
#define TTK_NORMAL_DOWN_PATH_FULL   NORMAL_DOWN_PATH_FULL
#define TTK_CLOUD_DOWN_PATH_FULL    CLOUD_DOWN_PATH_FULL
#define TTK_CLOUD_UP_PATH_FULL      CLOUD_UP_PATH_FULL
#define TTK_SEARCH_PATH_FULL        SEARCH_PATH_FULL
#define TTK_USER_THEME_DIR_FULL     USER_THEME_DIR_FULL
//
#define TTK_THEME_DIR_FULL          TTK::applicationPath() + THEME_DIR
#define TTK_PLUGINS_DIR_FULL        TTK::applicationPath() + PLUGINS_DIR
#define TTK_LANGUAGE_DIR_FULL       TTK::applicationPath() + LANGUAGE_DIR


#ifdef Q_OS_WIN
#  define TTK_SERVICE_FULL          TTK::applicationPath() + SERVICE_EXE_NAME
#else
#  define TTK_ROUTINE_FULL          TTK::applicationPath() + TTK_PDIR + "TTKRoutine.sh"
#  define TTK_MUSICPLAYER_FULL      TTK::applicationPath() + TTK_PDIR + APP_SHL_NAME
#  define TTK_INIT_FULL             TTK::applicationPath() + "TTKInit.sh"
#  define TTK_CONSOLE_FULL          TTK::applicationPath() + "TTKConsole.sh"
#  define TTK_SERVICE_FULL          TTK::applicationPath() + SERVICE_SHL_NAME
#  define TTK_ROUTINECOPY_FULL      TTK::applicationPath() + "TTKRoutineCopy.sh"
#endif

#endif
