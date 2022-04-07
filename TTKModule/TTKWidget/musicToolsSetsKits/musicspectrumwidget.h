#ifndef MUSICSPECTRUMWIDGET_H
#define MUSICSPECTRUMWIDGET_H

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

class Spek;
class QAbstractButton;

namespace Ui {
class MusicSpectrumWidget;
}

/*! @brief The class of the music spectrum object.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicSpectrum
{
    enum SpectrumType
    {
        Normal,
        Plus,
        Wave,
        Flow,
        Florid,
        Light
    };

    QString m_module;
    QWidget *m_object;
    SpectrumType m_type;
};
TTK_DECLARE_LIST(MusicSpectrum);

/*! @brief The class of the music spectrum widget all.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSpectrumWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSpectrumWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSpectrumWidget(QWidget *parent = nullptr);
    ~MusicSpectrumWidget();

public Q_SLOTS:
    /*!
     * Tab Index Changed.
     */
    void tabIndexChanged(int index);
    /*!
     * Spectrum Normal Type Changed.
     */
    void spectrumNormalTypeChanged(bool &state, const QString &name);
    /*!
     * Spectrum Plus Type Changed.
     */
    void spectrumPlusTypeChanged(bool &state, const QString &name);
    /*!
     * Spectrum Wave Type Changed.
     */
    void spectrumWaveTypeChanged(bool &state, const QString &name);
    /*!
     * Spectrum Flow Type Changed.
     */
    void spectrumFlowTypeChanged(bool &state, const QString &name);
    /*!
     * Spectrum Florid Type Changed.
     */
    void spectrumFloridTypeChanged(bool &state, const QString &name);
    /*!
     * Override show function.
     */
    void show();
    /*!
     * Local file button clicked.
     */
    void localFileButtonClicked();
    /*!
     * Open file button clicked.
     */
    void openFileButtonClicked();
    /*!
     * Emitted when visual widget is show fullscreen by user.
     */
    void fullscreenByUser(QWidget *widget, bool state);

protected:
    /*!
     * Create spectrum widget.
     */
    void createSpectrumWidget(MusicSpectrum::SpectrumType spectrum, bool &state, const QString &name, QLayout *layout);
    /*!
     * Create flow widget.
     */
    void createFlowWidget(MusicSpectrum::SpectrumType spectrum, bool &state, const QString &name, QLayout *layout);
    /*!
     * Create florid widget.
     */
    void createFloridWidget(MusicSpectrum::SpectrumType spectrum, bool &state, const QString &name, QLayout *layout);
    /*!
     * Create module widget.
     */
    void createModuleWidget(MusicSpectrum::SpectrumType spectrum, bool &state, const QString &name, QLayout *layout, bool florid);
    /*!
     * Create light widget.
     */
    void createLightWidget(MusicSpectrum::SpectrumType spectrum, bool &state, const QString &name, QLayout *layout, const QString &url = QString());
    /*!
     * Adjust widget layout.
     */
    void adjustWidgetLayout(int offset);
    /*!
     * Find spectrum widget index by name.
     */
    int findSpectrumWidget(const QString &name);
    /*!
     * Show message box widget.
     */
    void showMessageBoxWidget();

    Ui::MusicSpectrumWidget *m_ui;
    QLayout *m_spectrumLayout;
    QString m_lastFlowName;
    QString m_lastFloridName;
    MusicSpectrumList m_types;

};

#endif // MUSICSPECTRUMWIDGET_H
