#ifndef MUSICQUERYPLAYLISTREQUEST_H
#define MUSICQUERYPLAYLISTREQUEST_H

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

#include "musicabstractqueryrequest.h"

/*! @brief The class of query playlist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicQueryPlaylistRequest : public MusicAbstractQueryRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicQueryPlaylistRequest)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicQueryPlaylistRequest(QObject *parent = nullptr);

    /*!
     * Start to Search data.
     */
    virtual void startToSearch(const QString &value) = 0;
    /*!
     * Query playlist info.
     */
    virtual void queryPlaylistInfo(MusicResultsItem &item) = 0;

Q_SIGNALS:
    /*!
     * Create the current playlist item.
     */
    void createPlaylistItem(const MusicResultsItem &item);

};

#endif // MUSICQUERYPLAYLISTREQUEST_H
