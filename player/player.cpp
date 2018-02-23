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

#include "playercontrols.h"
#include "playlistmodel.h"
#include "histogramwidget.h"

#include <QMediaService>
#include <QMediaPlaylist>
#include <QVideoProbe>
#include <QAudioProbe>
#include <QMediaMetaData>
#include <QtWidgets>
#include <QtNetwork>
#include <QMenu>
#include <QMenuBar>

// Crypto++ Library
#ifdef _DEBUG
#  pragma comment ( lib, "cryptlibd" )
#else
#  pragma comment ( lib, "cryptlib" )
#endif

#include <rsa.h>
#include <osrng.h>
#include <base64.h>
#include <files.h>

using namespace std;
using namespace CryptoPP;

Player::Player(QWidget *parent)
    : QWidget(parent)
    , videoWidget(0)
    , coverLabel(0)
    , slider(0)
    , colorDialog(0)
	, licenseMacError(false)
	, licenseExpirationDateError(false)
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
    videoWidget = new VideoWidget(this);
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

    PlayerControls *controls = new PlayerControls(this);
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
}

Player::~Player()
{
}

bool Player::isPlayerAvailable() const
{
    return player->isAvailable();
}

void Player::setLicenseMacError(bool licenseMacError)
{
	this->licenseMacError = licenseMacError;
}

bool Player::getLicenseMacError()
{
	return licenseMacError;
}

void Player::setLicenseExpirationDateError(bool licenseExpirationDateError)
{
	this->licenseExpirationDateError = licenseExpirationDateError;
}

bool Player::getLicenseExpirationDateError()
{
	return licenseExpirationDateError;
}

void Player::showErrorMessageBox(QString title, QString errorText)
{
	QPushButton* licenseHelp = new QPushButton("Help", this);
	connect(licenseHelp, SIGNAL(clicked()), this, SLOT(onLicenseHelp()));
	QMessageBox msgBox(QMessageBox::Critical, title, errorText, QMessageBox::Ok);
	msgBox.addButton(licenseHelp, QMessageBox::HelpRole);
	msgBox.exec();
}

// Reads the license file and checks if everything is ok
// Can throw exceptions
void Player::processLicense()
{
	model = new LicenseModel();
	bool verification = /*false*/ true;
	if (checkLicenseFileNumber())
	{
		if (checkSignatureFileNumber())
		{
			QByteArray signatureFilePath = getSignatureFilePath().toLatin1();
			const char* cSignatureFilePath = signatureFilePath.data();
			//verification = verifySignature(getLicenseFilePath(), cSignatureFilePath);
		}
		else
		{
			throw LicenseSignatureFileNumberException("");
		}
	}
	else
	{
		throw LicenseFileNumberException("");
	}

	if (verification)
	{
		readDataFromLicenseFile(getLicenseFilePath());
		if (!checkMacAdress())
		{
			throw LicenseMacAdressException("");
		}
		if (!checkExpirationDate())
		{
			throw LicenseExpirationDateException("");
		}
	}
	else
	{
		throw LicenseSignatureException("");
	}
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
        disconnect(fullScreenButton, SIGNAL(clicked(bool)),
                    videoWidget, SLOT(setFullScreen(bool)));
        disconnect(videoWidget, SIGNAL(fullScreenChanged(bool)),
                fullScreenButton, SLOT(setChecked(bool)));
        videoWidget->setFullScreen(false);
    } else {
        connect(fullScreenButton, SIGNAL(clicked(bool)),
                videoWidget, SLOT(setFullScreen(bool)));
        connect(videoWidget, SIGNAL(fullScreenChanged(bool)),
                fullScreenButton, SLOT(setChecked(bool)));

        if (fullScreenButton->isChecked())
            videoWidget->setFullScreen(true);
    }
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

void Player::onLicenseHelp()
{
	QString path = QDir::currentPath();
	path.append("/LicenseHelp.pdf");
	QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void Player::clearHistogram()
{
    QMetaObject::invokeMethod(videoHistogram, "processFrame", Qt::QueuedConnection, Q_ARG(QVideoFrame, QVideoFrame()));
    QMetaObject::invokeMethod(audioHistogram, "processBuffer", Qt::QueuedConnection, Q_ARG(QAudioBuffer, QAudioBuffer()));
}


bool Player::checkLicenseFileNumber()
{
	int counterLicenseFile = 0;
	QDirIterator itLic("lic", QStringList() << "*.lic", QDir::Files | QDir::NoDotAndDotDot | QDir::Readable | QDir::NoSymLinks);
	while (itLic.hasNext())
	{
		setLicenseFilePath(itLic.next());
		counterLicenseFile++;
	}
	if (counterLicenseFile == 1)
	{
		return true;
	}
	return false;
}


bool Player::checkSignatureFileNumber()
{
	int counterSignature = 0;
	QDirIterator itSig("lic", QStringList() << "*.dat", QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
	while (itSig.hasNext())
	{
		setSignatureFilePath(itSig.next());
		counterSignature++;
	}
	if (counterSignature == 1)
	{
		return true;
	}
	return false;
}


bool Player::verifySignature(QString licensePath, const char* signaturePath)
{
	// Read public key
	CryptoPP::ByteQueue bytes;
	FileSource file("pubkey.txt", true, new Base64Decoder);
	file.TransferTo(bytes);
	bytes.MessageEnd();
	RSA::PublicKey pubKey;
	pubKey.Load(bytes);

	RSASSA_PKCS1v15_SHA_Verifier verifier(pubKey);

	// Read signed message
	QFile licenseFile(licensePath);
	if (!licenseFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	QTextStream in(&licenseFile);
	string licenseData = in.readAll().toStdString();

	// Read signation
	string signature;
	FileSource(signaturePath, true, new StringSink(signature));

	string combined(licenseData);
	combined.append(signature);

	// Verify signature
	try
	{
		StringSource(combined, true,
			new SignatureVerificationFilter(
				verifier, NULL,
				SignatureVerificationFilter::THROW_EXCEPTION
			)
		);
		qDebug() << "Signature OK" << endl;
		return true;
	}
	catch (SignatureVerificationFilter::SignatureVerificationFailed &err)
	{
		qDebug() << err.what() << endl;
		return false;
	}

	return false;
}

void Player::readDataFromLicenseFile(QString licenseFilePath)
{
	QFile file(licenseFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);
	while (!in.atEnd())
	{
		QString line = in.readLine();
		if (line != "" && line != "\n")
		{
			QStringList split = line.split(" : ");
			if (split.length() == 2)
			{
				QString keyword = split[0];
				QString value = split[1];

				// Not a good style but necessary
				if (keyword == model->getKeyWordFirstName())
				{
					model->setFirstName(value);
				}
				else if (keyword == model->getKeyWordLastName())
				{
					model->setLastName(value);
				}
				else if (keyword == model->getKeyWordEmail())
				{
					model->setEmail(value);
				}
				else if (keyword == model->getKeyWordCompany())
				{
					model->setCompany(value);
				}
				else if (keyword == model->getKeyWordMac())
				{
					model->setMac(value);
				}
				else if (keyword == model->getKeyWordFullScreen())
				{
					// Cast to bool
					model->setFeatureFullScreen(value.toInt());
				}
				else if (keyword == model->getKeyWordSpeed())
				{
					// Cast to bool
					model->setFeatureSpeed(value.toInt());
				}
				else if (keyword == model->getKeyWordColor())
				{
					// Cast to bool
					model->setFeatureColor(value.toInt());
				}
				else if (keyword == model->getKeyWordHistogram())
				{
					// Cast to bool
					model->setFeatureHistogram(value.toInt());
				}
				else if (keyword == model->getKeyWordDuration())
				{
					// Cast to int
					model->setDuration(value.toInt());
				}
				else if (keyword == model->getKeyWordExpirationDate())
				{
					// Cast to QDate
					model->setExpirationDate(QDate::fromString(value, "dd.MM.yyyy"));
				}
				else if (keyword == model->getKeyWordCustomerNumber())
				{
					model->setCustomerNumber(value);
				}
			}
		}
	}
}

void Player::setLicenseFilePath(QString licenseFilePath)
{
	this->licenseFilePath = licenseFilePath;
}

QString Player::getLicenseFilePath()
{
	return licenseFilePath;
}

void Player::setSignatureFilePath(QString signatureFilePath)
{
	this->signatureFilePath = signatureFilePath;
}

QString Player::getSignatureFilePath()
{
	return signatureFilePath;
}

bool Player::checkMacAdress()
{
	QString licenseMac = model->getMac();
	if (!licenseMac.isEmpty()) 
	{
		for (QNetworkInterface netInterface : QNetworkInterface::allInterfaces())
		{
			// Get only the non-loopback mac addresses
			if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
			{
				// If one mac adress is the same like the one mentioned in the license, everything is fine
				if (netInterface.hardwareAddress() == licenseMac)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Player::checkExpirationDate()
{
	QDate licenseExpirationDate = model->getExpirationDate();
	QDate today = QDate::currentDate();
	// If today is earlier then the expiration date, everything is fine
	if (today <= licenseExpirationDate)
	{
		return true;
	}
	return false;
}
