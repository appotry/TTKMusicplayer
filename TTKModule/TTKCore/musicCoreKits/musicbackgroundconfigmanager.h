#ifndef MUSICBACKGROUNDCONFIGMANAGER_H
#define MUSICBACKGROUNDCONFIGMANAGER_H

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

#include <QPixmap>
#include "musicabstractxml.h"

/*! @brief The class of the skin item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicSkinItem
{
    QString m_name;
    int m_useCount;

    MusicSkinItem()
    {
        m_name = MUSIC_AUTHOR_NAME;
        m_useCount = 8888;
    }

    inline bool isValid() const
    {
        return !(m_name.isEmpty() && m_useCount == 0);
    }
};


/*! @brief The class of the skin backgroud image.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicBackgroundImage
{
    QPixmap m_pix;
    MusicSkinItem m_item;

    inline bool isValid() const
    {
        return !m_pix.isNull() && m_item.isValid();
    }
};


/*! @brief The class of the skin XML config manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSkinConfigManager : public MusicAbstractXml
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSkinConfigManager)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSkinConfigManager(QObject *parent = nullptr);

    /*!
     * Read datas from config file.
     */
    void readBuffer(MusicSkinItem &item);
    /*!
     * Write datas into config file.
     */
    void writeBuffer(const MusicSkinItem &item, const QString &path);

};

#endif // MUSICBACKGROUNDCONFIGMANAGER_H
