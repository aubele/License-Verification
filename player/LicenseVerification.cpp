#include "LicenseVerification.h"
#include "LicenseExceptions.h"

#include "QByteArray"
#include "QDirIterator"
#include "QTextStream"
#include "QNetworkInterface"
#include "QPushButton"
#include "QMessageBox"
#include "QDesktopServices"
#include "QUrl"

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

using namespace CryptoPP;


///////////////////////////////////////////////////////////////////////////////////////////
// ### CONSTRUCTOR ### 


LicenseVerification::LicenseVerification()
{
	model = new LicenseModel();
}

LicenseVerification::~LicenseVerification()
{
}


///////////////////////////////////////////////////////////////////////////////////////////
// ### LICENSE VERIFICATION ### 



void LicenseVerification::processLicense()
{
	bool verification = true;
	// First check the amount of license files
	if (checkLicenseFileNumber())
	{
		// Then get the licensepath
		QString licensePath = getLicenseFilePathFromDirectory();
		// Then the amount of signature files
		if (checkSignatureFileNumber())
		{
			// Then get the signature path as const char* for CryptoPP
			QString signaturePath = getSignatureFilePathFromDirectory();
			QByteArray signatureFilePath = signaturePath.toLatin1();
			const char* cSignatureFilePath = signatureFilePath.data();
			// Verify the signature
			//verification = verifySignature(getLicenseFilePathFromDirectory(), cSignatureFilePath);
			if (verification)
			{
				// Read all the data from the license file and set it in the model
				readDataFromLicenseFile(getLicenseFilePathFromDirectory());
				// Check mac
				if (!checkMacAdress())
				{
					throw LicenseMacAdressException("");
				}
				// Check date
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
		else
		{
			throw LicenseSignatureFileNumberException("");
		}
	}
	else
	{
		throw LicenseFileNumberException("");
	}
}

bool LicenseVerification::checkLicenseFileNumber()
{
	// Just iterate through the lic directory
	int counterLicenseFile = 0;
	QDirIterator itLic("lic", QStringList() << "*.lic", QDir::Files | QDir::NoDotAndDotDot | QDir::Readable | QDir::NoSymLinks);
	while (itLic.hasNext())
	{
		counterLicenseFile++;
		itLic.next();
	}
	if (counterLicenseFile == 1)
	{
		return true;
	}
	return false;
}


bool LicenseVerification::checkSignatureFileNumber()
{
	// Just iterate through the lic directory
	int counterSignature = 0;
	QDirIterator itSig("lic", QStringList() << "*.sig", QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
	while (itSig.hasNext())
	{
		counterSignature++;
		itSig.next();
	}
	if (counterSignature == 1)
	{
		return true;
	}
	return false;
}

const QString LicenseVerification::getLicenseFilePathFromDirectory()
{
	// Get path from lic directory
	QDirIterator itLic("lic", QStringList() << "*.lic", QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
	while (itLic.hasNext())
	{
		return itLic.next();
	}
	return QString();
}

const QString LicenseVerification::getSignatureFilePathFromDirectory()
{
	// Get path from lic directory
	QDirIterator itSig("lic", QStringList() << "*.sig", QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
	while (itSig.hasNext())
	{
		return itSig.next();
	}
	return QString();
}

bool LicenseVerification::verifySignature(QString licensePath, const char* signaturePath)
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
		return true;
	}
	catch (SignatureVerificationFilter::SignatureVerificationFailed)
	{
		return false;
	}

	return false;
}

void LicenseVerification::readDataFromLicenseFile(QString licensePath)
{
	// Open file
	QFile file(licensePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);
	while (!in.atEnd())
	{
		// Line after line
		QString line = in.readLine();
		if (line != "" && line != "\n")
		{
			// Split the line in keyword and value
			QStringList split = line.split(" : ");
			if (split.length() == 2)
			{
				QString keyword = split[0];
				QString value = split[1];

				// Not a good style but necessary to get the right data
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

bool LicenseVerification::checkMacAdress()
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

bool LicenseVerification::checkExpirationDate()
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

void LicenseVerification::onLicenseHelp()
{
	// Open the help pdf
	QString path = QDir::currentPath();
	path.append("/LicenseHelp.pdf");
	QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void LicenseVerification::showErrorMessageBox(QString title, QString errorText)
{
	// Show a critical message box
	QPushButton* licenseHelp = new QPushButton("Help");
	connect(licenseHelp, SIGNAL(clicked()), this, SLOT(onLicenseHelp()));
	QMessageBox msgBox(QMessageBox::Critical, title, errorText, QMessageBox::Ok);
	msgBox.setEscapeButton(QMessageBox::Ok);
	msgBox.addButton(licenseHelp, QMessageBox::HelpRole);
	msgBox.exec();
}


///////////////////////////////////////////////////////////////////////////////////////////
// ### MODEL ### 


const QString LicenseVerification::getModelFirstName()
{
	return model->getFirstName();
}

const QString LicenseVerification::getModelLastName()
{
	return model->getLastName();
}

const QString LicenseVerification::getModelCustomerNumber()
{
	return model->getCustomerNumber();
}

const QString LicenseVerification::getModelCompany()
{
	return model->getCompany();
}

const QDate& LicenseVerification::getModelExpirationDate()
{
	return model->getExpirationDate();
}

bool LicenseVerification::getModelFeatureFullScreen()
{
	return model->getFeatureFullScreen();
}

bool LicenseVerification::getModelFeatureSpeed()
{
	return model->getFeatureSpeed();
}

bool LicenseVerification::getModelFeatureColor()
{
	return model->getFeatureColor();
}

bool LicenseVerification::getModelFeatureHistogram()
{
	return model->getFeatureHistogram();
}
