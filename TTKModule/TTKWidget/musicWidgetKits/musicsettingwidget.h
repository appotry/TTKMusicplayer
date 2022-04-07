#ifndef MUSICSETTINGWIDGET_H
#define MUSICSETTINGWIDGET_H

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

#include "musicsettingmanager.h"
#include "musicabstractmovedialog.h"
#include "musicabstracttablewidget.h"

class QComboBox;

namespace Ui {
class MusicSettingWidget;
}

/*! @brief The class of the function tool item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicFunctionItem
{
    MusicFunctionItem(const QString &icon, const QString &name)
    {
        m_icon = icon;
        m_name = name;
    }

    QString m_icon;
    QString m_name;
};
TTK_DECLARE_LIST(MusicFunctionItem);

/*! @brief The class of the list table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFunctionTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicFunctionTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFunctionTableWidget(QWidget *parent = nullptr);

    /*!
     * Add table list items by index and icons and paths.
     */
    void addFunctionItems(int index, const MusicFunctionItemList &items);

Q_SIGNALS:
    /*!
     * Current list index changed.
     */
    void currentIndexChanged(int index);

public Q_SLOTS:
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override final;

protected:
    /*!
     * Override the widget event.
     */
    virtual void leaveEvent(QEvent *event) override final;

    int m_listIndex;
};


/*! @brief The class of the setting parameters widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSettingWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSettingWidget)
public:
    enum LrcType
    {
        LrcInterior, /*!< lrc interior type*/
        LrcDesktop   /*!< lrc desktop type*/
    };

    enum ProxyType
    {
        ProxyTest,   /*!< proxy test type*/
        ProxyApply   /*!< proxy apply type*/
    };

    /*!
     * Object contsructor.
     */
    explicit MusicSettingWidget(QWidget *parent = nullptr);
    ~MusicSettingWidget();

    /*!
     * Init controller parameter to widget.
     */
    void initialize();

Q_SIGNALS:
    /*!
     * Parameters setting changed emit.
     */
    void parameterSettingChanged();

public Q_SLOTS:
    /*!
     * Clear all function tables selection.
     */
    void clearFunctionTableSelection();
    /*!
     * Set global hotkey box changed.
     */
    void globalHotkeyBoxChanged(bool state);

    /*!
     * Set clean download cache.
     */
    void downloadCacheClean();
    /*!
     * Set enable or disable download cache.
     */
    void downloadGroupCached(int index);
    /*!
     * Set enable or disable download speed limit.
     */
    void downloadGroupSpeedLimit(int index);
    /*!
     * Set select download dir or lrc dir.
     */
    void downloadDirSelected(int index);

    /*!
     * Version update check changed.
     */
    void rippleVersionUpdateChanged();
    /*!
     * Ripple spectrum color changed.
     */
    void rippleSpectrumColorChanged();
    /*!
     * Ripple spectrum opacity enable changed.
     */
    void rippleSpectrumOpacityEnableClicked(bool state);
    /*!
     * Ripple low power mode enable changed.
     */
    void rippleLowPowerEnableBoxClicked(bool state);

    /*!
     * Plugin manager changed.
     */
    void otherPluginManagerChanged();

    /*!
     * Change to desktop lrc widget.
     */
    void changeDesktopLrcWidget();
    /*!
     * Change to interior lrc widget.
     */
    void changeInteriorLrcWidget();
    /*!
     * Change to download widget.
     */
    void changeDownloadWidget();

    /*!
     * Interior lrc foreground change.
     */
    void interiorLrcFrontgroundChanged();
    /*!
     * Interior lrc background change.
     */
    void interiorLrcBackgroundChanged();
    /*!
     * Default lrc color change by index.
     */
    void defaultLrcColorChanged(int value);
    /*!
     * Interior lrc transparent changed by index.
     */
    void interiorLrcTransChanged(int value);
    /*!
     * Show interior lrc preview.
     */
    void showInteriorLrcDemo();
    /*!
     * Reset interior parameter.
     */
    void resetInteriorParameter();

    /*!
     * Desktop lrc foreground change.
     */
    void desktopFrontgroundChanged();
    /*!
     * Desktop lrc background change.
     */
    void desktopBackgroundChanged();
    /*!
     * Default desktop lrc color change by index.
     */
    void defaultDesktopLrcColorChanged(int value);
    /*!
     * Desktop lrc transparent changed by index.
     */
    void desktopLrcTransChanged(int value);
    /*!
     * Show desktop lrc preview.
     */
    void showDesktopLrcDemo();
    /*!
     * Reset desktop parameter.
     */
    void resetDesktopParameter();

    /*!
     * Set network proxy control enabled or not.
     */
    void setNetworkProxyControl(int enable);
    /*!
     * Test current proxy available.
     */
    void testNetworkProxy();
    /*!
     * Test current proxy available changed.
     */
    void testProxyStateChanged(bool state);
    /*!
     * Test current network connection available.
     */
    void testNetworkConnection();
    /*!
     * Check current network connection available.
     */
    void checkNetworkConnection();
    /*!
     * Test current network connection available changed.
     */
    void testNetworkConnectionStateChanged(const QString &name);

    /*!
     * Music fade in and out changed.
     */
    void musicFadeInAndOutClicked(bool state);

    /*!
     * Save the parameter setting results.
     */
    void saveParameterSettings();
    /*!
     * Override exec function.
     */
    virtual int exec();

private Q_SLOTS:
    /*!
     * Set scroll widget page index.
     */
    void setScrollWidgetPageIndex(int index);
    /*!
     * Scroll widget index changed.
     */
    void scrollWidgetValueChanged(int value);

protected:
    /*!
     * Select function table index.
     */
    void selectFunctionTableIndex(int row, int col);
    /*!
     * Init scroll widget page widget.
     */
    void initScrollWidgetPage();
    /*!
     * Init normal setting stack widget.
     */
    void initNormalSettingWidget();
    /*!
     * Init spectrum setting stack widget.
     */
    void initSpectrumSettingWidget();
    /*!
     * Init other setting stack widget.
     */
    void initOtherSettingWidget();
    /*!
     * Init download stack widget.
     */
    void initDownloadWidget();
    /*!
     * Init desktop lrc stack widget.
     */
    void initDesktopLrcWidget();
    /*!
     * Init interior lrc stack widget.
     */
    void initInteriorLrcWidget();
    /*!
     * Init sound effect stack widget.
     */
    void initSoundEffectWidget();
    /*!
     * Init audio setting stack widget.
     */
    void initAudioSettingWidget();
    /*!
     * Init network stack widget.
     */
    void initNetworkWidget();
    /*!
     * Set lrc color value by type and value type.
     */
    void lcrColorValue(LrcType key, const QString &type, QLabel *obj);
    /*!
     * Set lrc default color by type and index.
     */
    void lrcColorByDefault(LrcType key, int index);
    /*!
     * Set lrc transparent by type and value.
     */
    void lrcTransparentValue(LrcType key, int value) const;
    /*!
     * Set network proxy by type.
     */
    bool setNetworkProxyByType(ProxyType type);

    Ui::MusicSettingWidget *m_ui;

};

#endif // MUSICSETTINGWIDGET_H
