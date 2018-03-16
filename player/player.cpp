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

#include "player.h"

#include "playlistmodel.h"
#include "histogramwidget.h"

#include <QMediaService>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QAudioProbe>
#include <QMediaMetaData>
#include <QtWidgets>

using namespace std;

Player::Player(LicenseVerification* verification, QWidget *parent)
    : QWidget(parent)
    , videoWidget(0)
    , coverLabel(0)
    , slider(0)
    , colorDialog(0)
	, verification(verification)
{
//! [create-objs]
    player = new QMediaPlayer(this);
    // owned by PlaylistModel
    playlist = new QMediaPlaylist();
    player->setPlaylist(playlist);
//! [create-objs]

    connect(player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(player, SIGNAL(metaDataChanged()), SLOT(metaDataChanged()));
    connect(playlist, SIGNAL(currentIndexChanged(int)), SLOT(playlistPositionChanged(int)));
    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));
    connect(player, SIGNAL(bufferStatusChanged(int)), this, SLOT(bufferingProgress(int)));
    connect(player, SIGNAL(videoAvailableChanged(bool)), this, SLOT(videoAvailableChanged(bool)));
    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(displayErrorMessage()));
    connect(player, &QMediaPlayer::stateChanged, this, &Player::stateChanged);

//! [2]
	// Boolean needed for the key events
    videoWidget = new VideoWidget(verification->getModelFeatureFullScreen(), this);
    player->setVideoOutput(videoWidget);

    playlistModel = new PlaylistModel(this);
    playlistModel->setPlaylist(playlist);
//! [2]

    playlistView = new QListView(this);
    playlistView->setModel(playlistModel);
    playlistView->setCurrentIndex(playlistModel->index(playlist->currentIndex(), 0));

    connect(playlistView, SIGNAL(activated(QModelIndex)), this, SLOT(jump(QModelIndex)));

    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, player->duration() / 1000);

    labelDuration = new QLabel(this);
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));
	
	labelHistogram = new QLabel(this);
	labelHistogram->setText("Histogram:");
	videoHistogram = new HistogramWidget(this);
	audioHistogram = new HistogramWidget(this);
	QHBoxLayout *histogramLayout = new QHBoxLayout;
	histogramLayout->addWidget(labelHistogram);
	histogramLayout->addWidget(videoHistogram, 1);
	histogramLayout->addWidget(audioHistogram, 2);

	videoProbe = new QVideoProbe(this);
	connect(videoProbe, SIGNAL(videoFrameProbed(QVideoFrame)), videoHistogram, SLOT(processFrame(QVideoFrame)));
	videoProbe->setSource(player);

	audioProbe = new QAudioProbe(this);
	connect(audioProbe, SIGNAL(audioBufferProbed(QAudioBuffer)), audioHistogram, SLOT(processBuffer(QAudioBuffer)));
	audioProbe->setSource(player);

    QPushButton *openButton = new QPushButton(tr("Open"), this);

    connect(openButton, SIGNAL(clicked()), this, SLOT(open()));

    controls = new PlayerControls(this);
    controls->setState(player->state());
    controls->setVolume(player->volume());
    controls->setMuted(controls->isMuted());

    connect(controls, SIGNAL(play()), player, SLOT(play()));
    connect(controls, SIGNAL(pause()), player, SLOT(pause()));
    connect(controls, SIGNAL(stop()), player, SLOT(stop()));
    connect(controls, SIGNAL(next()), playlist, SLOT(next()));
    connect(controls, SIGNAL(previous()), this, SLOT(previousClicked()));
    connect(controls, SIGNAL(changeVolume(int)), player, SLOT(setVolume(int)));
    connect(controls, SIGNAL(changeMuting(bool)), player, SLOT(setMuted(bool)));
    connect(controls, SIGNAL(changeRate(qreal)), player, SLOT(setPlaybackRate(qreal)));
    connect(controls, SIGNAL(stop()), videoWidget, SLOT(update()));

    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            controls, SLOT(setState(QMediaPlayer::State)));
    connect(player, SIGNAL(volumeChanged(int)), controls, SLOT(setVolume(int)));
    connect(player, SIGNAL(mutedChanged(bool)), controls, SLOT(setMuted(bool)));

    fullScreenButton = new QPushButton(tr("FullScreen"), this);
    fullScreenButton->setCheckable(true);

    colorButton = new QPushButton(tr("Color Options..."), this);
    colorButton->setEnabled(false);
    connect(colorButton, SIGNAL(clicked()), this, SLOT(showColorDialog()));

	QBoxLayout *displayLayout = new QHBoxLayout;
	displayLayout->addWidget(videoWidget, 2);
	displayLayout->addWidget(playlistView);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(openButton);
    controlLayout->addStretch(1);
    controlLayout->addWidget(controls);
    controlLayout->addStretch(1);
    controlLayout->addWidget(fullScreenButton);
    controlLayout->addWidget(colorButton);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(displayLayout);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(slider);
    hLayout->addWidget(labelDuration);
    layout->addLayout(hLayout);
    layout->addLayout(controlLayout);
	layout->addLayout(histogramLayout);

    setLayout(layout);

    if (!isPlayerAvailable()) {
        QMessageBox::warning(this, tr("Service not available"),
                             tr("The QMediaPlayer object does not have a valid service.\n"\
                                "Please check the media service plugins are installed."));

        controls->setEnabled(false);
        playlistView->setEnabled(false);
        openButton->setEnabled(false);
        colorButton->setEnabled(false);
        fullScreenButton->setEnabled(false);
    }

    metaDataChanged();

	// Added by Fabio Aubele
	// Setup the license widget at top if licensing is active
	if (verification->getIsLicensingActive())
	{
		QBoxLayout *licenseLayout = setUpLicenseInfo();
		layout->insertLayout(0, licenseLayout);
	}
	// Enable or disable the features
	toggleFeatures();
}

Player::~Player()
{
}

bool Player::isPlayerAvailable() const
{
    return player->isAvailable();
}

void Player::open()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Files"));
    QStringList supportedMimeTypes = player->supportedMimeTypes();
    if (!supportedMimeTypes.isEmpty()) {
        supportedMimeTypes.append("audio/x-m3u"); // MP3 playlists
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    }
    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    if (fileDialog.exec() == QDialog::Accepted)
        addToPlaylist(fileDialog.selectedUrls());
}

static bool isPlaylist(const QUrl &url) // Check for ".m3u" playlists.
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}

void Player::addToPlaylist(const QList<QUrl> urls)
{
    foreach (const QUrl &url, urls) {
        if (isPlaylist(url))
            playlist->load(url);
        else
            playlist->addMedia(url);
    }
}

void Player::durationChanged(qint64 duration)
{
    this->duration = duration/1000;
    slider->setMaximum(duration / 1000);
}

void Player::positionChanged(qint64 progress)
{
    if (!slider->isSliderDown()) {
        slider->setValue(progress / 1000);
    }
    updateDurationInfo(progress / 1000);
}

void Player::metaDataChanged()
{
    if (player->isMetaDataAvailable()) {
        setTrackInfo(QString("%1 - %2")
                .arg(player->metaData(QMediaMetaData::AlbumArtist).toString())
                .arg(player->metaData(QMediaMetaData::Title).toString()));

        if (coverLabel) {
            QUrl url = player->metaData(QMediaMetaData::CoverArtUrlLarge).value<QUrl>();

            coverLabel->setPixmap(!url.isEmpty()
                    ? QPixmap(url.toString())
                    : QPixmap());
        }
    }
}

void Player::previousClicked()
{
    // Go to previous track if we are within the first 5 seconds of playback
    // Otherwise, seek to the beginning.
    if(player->position() <= 5000)
        playlist->previous();
    else
        player->setPosition(0);
}

void Player::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        playlist->setCurrentIndex(index.row());
        player->play();
    }
}

void Player::playlistPositionChanged(int currentItem)
{
    clearHistogram();
    playlistView->setCurrentIndex(playlistModel->index(currentItem, 0));
}

void Player::seek(int seconds)
{
    player->setPosition(seconds * 1000);
}

void Player::statusChanged(QMediaPlayer::MediaStatus status)
{
    handleCursor(status);

    // handle status message
    switch (status) {
    case QMediaPlayer::UnknownMediaStatus:
    case QMediaPlayer::NoMedia:
    case QMediaPlayer::LoadedMedia:
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
        setStatusInfo(QString());
        break;
    case QMediaPlayer::LoadingMedia:
        setStatusInfo(tr("Loading..."));
        break;
    case QMediaPlayer::StalledMedia:
        setStatusInfo(tr("Media Stalled"));
        break;
    case QMediaPlayer::EndOfMedia:
        QApplication::alert(this);
        break;
    case QMediaPlayer::InvalidMedia:
        displayErrorMessage();
        break;
    }
}

void Player::stateChanged(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::StoppedState)
        clearHistogram();
}

void Player::handleCursor(QMediaPlayer::MediaStatus status)
{
#ifndef QT_NO_CURSOR
    if (status == QMediaPlayer::LoadingMedia ||
        status == QMediaPlayer::BufferingMedia ||
        status == QMediaPlayer::StalledMedia)
        setCursor(QCursor(Qt::BusyCursor));
    else
        unsetCursor();
#endif
}

void Player::bufferingProgress(int progress)
{
    setStatusInfo(tr("Buffering %4%").arg(progress));
}

void Player::videoAvailableChanged(bool available)
{
    if (!available) {
        disconnect(fullScreenButton, SIGNAL(clicked(bool)), videoWidget, SLOT(setFullScreen(bool)));
        disconnect(videoWidget, SIGNAL(fullScreenChanged(bool)), fullScreenButton, SLOT(setChecked(bool)));
        videoWidget->setFullScreen(false);
    } else {
        connect(fullScreenButton, SIGNAL(clicked(bool)), videoWidget, SLOT(setFullScreen(bool)));
        connect(videoWidget, SIGNAL(fullScreenChanged(bool)), fullScreenButton, SLOT(setChecked(bool)));

        if (fullScreenButton->isChecked())
            videoWidget->setFullScreen(true);
    }
	// Only reactivate color button if the feature is unlocked according to the license
	if(verification->getModelFeatureColor())
		colorButton->setEnabled(available);
}

void Player::setTrackInfo(const QString &info)
{
    trackInfo = info;
    if (!statusInfo.isEmpty())
        setWindowTitle(QString("%1 | %2").arg(trackInfo).arg(statusInfo));
    else
        setWindowTitle(trackInfo);
}

void Player::setStatusInfo(const QString &info)
{
    statusInfo = info;
    if (!statusInfo.isEmpty())
        setWindowTitle(QString("%1 | %2").arg(trackInfo).arg(statusInfo));
    else
        setWindowTitle(trackInfo);
}

void Player::displayErrorMessage()
{
    setStatusInfo(player->errorString());
}

void Player::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || duration) {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60, (currentInfo*1000)%1000);
        QTime totalTime((duration/3600)%60, (duration/60)%60, duration%60, (duration*1000)%1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    labelDuration->setText(tStr);
}

void Player::showColorDialog()
{
	if (!colorDialog) {
		QSlider *brightnessSlider = new QSlider(Qt::Horizontal);
		brightnessSlider->setRange(-100, 100);
		brightnessSlider->setValue(videoWidget->brightness());
		connect(brightnessSlider, SIGNAL(sliderMoved(int)), videoWidget, SLOT(setBrightness(int)));
		connect(videoWidget, SIGNAL(brightnessChanged(int)), brightnessSlider, SLOT(setValue(int)));

		QSlider *contrastSlider = new QSlider(Qt::Horizontal);
		contrastSlider->setRange(-100, 100);
		contrastSlider->setValue(videoWidget->contrast());
		connect(contrastSlider, SIGNAL(sliderMoved(int)), videoWidget, SLOT(setContrast(int)));
		connect(videoWidget, SIGNAL(contrastChanged(int)), contrastSlider, SLOT(setValue(int)));

		QSlider *hueSlider = new QSlider(Qt::Horizontal);
		hueSlider->setRange(-100, 100);
		hueSlider->setValue(videoWidget->hue());
		connect(hueSlider, SIGNAL(sliderMoved(int)), videoWidget, SLOT(setHue(int)));
		connect(videoWidget, SIGNAL(hueChanged(int)), hueSlider, SLOT(setValue(int)));

		QSlider *saturationSlider = new QSlider(Qt::Horizontal);
		saturationSlider->setRange(-100, 100);
		saturationSlider->setValue(videoWidget->saturation());
		connect(saturationSlider, SIGNAL(sliderMoved(int)), videoWidget, SLOT(setSaturation(int)));
		connect(videoWidget, SIGNAL(saturationChanged(int)), saturationSlider, SLOT(setValue(int)));

		QFormLayout *layout = new QFormLayout;
		layout->addRow(tr("Brightness"), brightnessSlider);
		layout->addRow(tr("Contrast"), contrastSlider);
		layout->addRow(tr("Hue"), hueSlider);
		layout->addRow(tr("Saturation"), saturationSlider);

		QPushButton *button = new QPushButton(tr("Close"));
		layout->addRow(button);

		colorDialog = new QDialog(this);
		colorDialog->setWindowTitle(tr("Color Options"));
		colorDialog->setLayout(layout);

		connect(button, SIGNAL(clicked()), colorDialog, SLOT(close()));
	}
	colorDialog->show();
}

void Player::clearHistogram()
{
	QMetaObject::invokeMethod(videoHistogram, "processFrame", Qt::QueuedConnection, Q_ARG(QVideoFrame, QVideoFrame()));
	QMetaObject::invokeMethod(audioHistogram, "processBuffer", Qt::QueuedConnection, Q_ARG(QAudioBuffer, QAudioBuffer()));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// All the methods below and the function-calls are added by Fabio Aubele, to add a valid license verification

QBoxLayout* Player::setUpLicenseInfo()
{
	QWidget* licenseInfos = new QWidget;

	// Label with user infos
	QLabel* licenseText = new QLabel;
	licenseText->setText("Dieses Programm ist lizenziert fuer " + verification->getModelFirstName() + " " + verification->getModelLastName() +
		", Kundennummer: " + verification->getModelCustomerNumber() + ", Firma: " + verification->getModelCompany());
	licenseText->setAlignment(Qt::AlignLeft);
	licenseText->setFrameShape(QFrame::Panel);
	licenseText->setFrameShadow(QFrame::Sunken);
	licenseText->setLineWidth(1);
	licenseText->setMidLineWidth(3);
	licenseText->setMargin(5);

	// Label for expiration date
	QLabel* licenseDate = new QLabel;
	licenseDate->setText("Gueltig bis " + verification->getModelExpirationDate().toString("dd.MM.yyyy"));
	licenseDate->setAlignment(Qt::AlignRight);
	licenseDate->setFrameShape(QFrame::Panel);
	licenseDate->setFrameShadow(QFrame::Sunken);
	licenseDate->setLineWidth(1);
	licenseDate->setMidLineWidth(3);
	licenseDate->setMargin(5);
	licenseDate->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QBoxLayout *licenseLabelLayout = new QHBoxLayout;
	licenseLabelLayout->setMargin(0);
	licenseLabelLayout->addWidget(licenseText);
	licenseLabelLayout->addWidget(licenseDate);

	licenseInfos->setLayout(licenseLabelLayout);

	QBoxLayout *licenseLayout = new QHBoxLayout;
	licenseLayout->addWidget(licenseInfos);

	return licenseLayout;
}

void Player::toggleFeatures()
{
	toggleFullScreenFeature(verification->getModelFeatureFullScreen());
	toggleSpeedFeature(verification->getModelFeatureSpeed());
	toggleColorFeature(verification->getModelFeatureColor());
	toggleHistogramFeature(verification->getModelFeatureHistogram());
}

void Player::toggleFullScreenFeature(bool enable)
{
	fullScreenButton->setEnabled(enable);
	if (enable == false)
		fullScreenButton->setToolTip("Keine gueltige Lizenz fuer dieses Feature");
	else
		fullScreenButton->setToolTip("");
}

void Player::toggleSpeedFeature(bool enable)
{
	controls->toggleSpeedBox(enable);
}

void Player::toggleColorFeature(bool enable)
{
	colorButton->setEnabled(enable);
	if(enable == false)
		colorButton->setToolTip("Keine gueltige Lizenz fuer dieses Feature");
	else
		colorButton->setToolTip("");
}

void Player::toggleHistogramFeature(bool enable)
{
	labelHistogram->setVisible(enable);
	videoHistogram->setVisible(enable);
	audioHistogram->setVisible(enable);
}
