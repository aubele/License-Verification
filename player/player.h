/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
// Added methods to injure a correct license verification | by: Fabio Aubele

#ifndef PLAYER_H
#define PLAYER_H

#include "videowidget.h"
#include "playercontrols.h"
#include "LicenseVerification.h"
#include "LicenseExceptions.h"

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QBoxLayout>

QT_BEGIN_NAMESPACE
class QAbstractItemView;
class QLabel;
class QMediaPlayer;
class QModelIndex;
class QPushButton;
class QSlider;
class QVideoProbe;
class QVideoWidget;
class QAudioProbe;
QT_END_NAMESPACE

class PlaylistModel;
class HistogramWidget;

class Player : public QWidget
{
    Q_OBJECT

public:
    Player(LicenseVerification* verification, QWidget *parent = 0);
    ~Player();

    bool isPlayerAvailable() const;

    void addToPlaylist(const QList<QUrl> urls);

	/**
	* Returns a boolean, based on the multiple signature verification calls, to check if the player 
	* object passed the test on construction.
	*/
	bool getPassedTest() const;

signals:
    void fullScreenChanged(bool fullScreen);

private slots:
    void open();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void metaDataChanged();

    void previousClicked();

    void seek(int seconds);
    void jump(const QModelIndex &index);
    void playlistPositionChanged(int);

    void statusChanged(QMediaPlayer::MediaStatus status);
    void stateChanged(QMediaPlayer::State state);
    void bufferingProgress(int progress);
    void videoAvailableChanged(bool available);

    void displayErrorMessage();

    void showColorDialog();

private:

	/**
	* Creates two labels, which show informations about the user and the expiration date.
	* @return Returns the layout with all nessecary gui-elements in it.
	*/
	QBoxLayout* setUpLicenseInfo();

	/**
	* Calls all toggle-feature-functions to activate or deactivate the feature like it is
	* mentioned in the license file.
	* @see toggleFullScreenFeature()
	* @see toggleSpeedFeature()
	* @see toggleColorFeature()
	* @see toggleHistogramFeature()
	*/
	void toggleFeatures();
	/**
	* Toggles the fullscreen-button and sets a tooltip if it get's deactivated.
	* @param enable True to activate, false to deactivate the feature.
	*/
	void toggleFullScreenFeature(bool enable);
	/**
	* Toggles the speed-box and sets a tooltip if it get's deactivated.
	* @param enable True to activate, false to deactivate the feature.
	*/
	void toggleSpeedFeature(bool enable);
	/**
	* Toggles the color-button and sets a tooltip if it get's deactivated.
	* @param enable True to activate, false to deactivate the feature.
	*/
	void toggleColorFeature(bool enable);
	/**
	* Toggles the histogram-widgets.
	* @param enable True to activate, false to deactivate the feature.
	*/
	void toggleHistogramFeature(bool enable);

    void clearHistogram();
    void setTrackInfo(const QString &info);
    void setStatusInfo(const QString &info);
    void handleCursor(QMediaPlayer::MediaStatus status);
    void updateDurationInfo(qint64 currentInfo);

    QMediaPlayer *player;
    QMediaPlaylist *playlist;
	PlayerControls *controls;
    VideoWidget *videoWidget;
    QLabel *coverLabel;
    QSlider *slider;
    QLabel *labelDuration;
    QPushButton *fullScreenButton;
    QPushButton *colorButton;
    QDialog *colorDialog;

    QLabel *labelHistogram;
    HistogramWidget *videoHistogram;
    HistogramWidget *audioHistogram;
    QVideoProbe *videoProbe;
    QAudioProbe *audioProbe;

    PlaylistModel *playlistModel;
    QAbstractItemView *playlistView;
    QString trackInfo;
    QString statusInfo;
    qint64 duration;

	bool passedTest;

	/**
	* Verification object, mainly used here to get all the nessecary data from the model and the
	* additional signature checks.
	*/
	LicenseVerification* verification;
};

#endif // PLAYER_H
