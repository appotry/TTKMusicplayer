#ifndef MUSICKWQUERYPLAYLISTREQUEST_H
#define MUSICKWQUERYPLAYLISTREQUEST_H

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

#include "musickwqueryinterface.h"
#include "musicqueryplaylistrequest.h"

/*! @brief The class of kuwo query playlist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicKWQueryPlaylistRequest : public MusicQueryPlaylistRequest, private MusicKWQueryInterface
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicKWQueryPlaylistRequest)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicKWQueryPlaylistRequest(QObject *parent = nullptr);

    /*!
     * Start to Search data from name and type.
     */
    virtual void startToSearch(QueryType type, const QString &playlist) override final;
    /*!
     * Start to search data from name and type bt page.
     */
    virtual void startToPage(int offset) override final;
    /*!
     * Start to Search data.
     */
    virtual void startToSearch(const QString &playlist) override final;
    /*!
     * Query playlist info.
     */
    virtual void queryPlaylistInfo(MusicResultsItem &item) override final;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;
    /*!
     * Download details data from net finished.
     */
    void downloadDetailsFinished();
    /*!
     * Download more playlisy details finished.
     */
    void downloadMoreDetailsFinished();

protected:
    /*!
     * Get more playlisy details.
     */
    void morePlaylistDetails(const QString &pid);

    QString m_tags;

};

#endif // MUSICKWQUERYPLAYLISTREQUEST_H
