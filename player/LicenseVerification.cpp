#include "LicenseVerification.h"
#include "LicenseExceptions.h"

#include "QByteArray"
#include "QDirIterator"
#include "QTextStream"
#include "QNetworkInterface"
#include "QPushButton"
#include "QDesktopServices"
#include "QUrl"
#include "QApplication"

// Crypto++ Library
#ifdef _DEBUG
#  pragma comment ( lib, "cryptlib32d" )
#else
#  pragma comment ( lib, "cryptlib32" )
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
	reader = new LicenseFileReader();
	isLicensingActive = true;
}

LicenseVerification::~LicenseVerification()
{
}


///////////////////////////////////////////////////////////////////////////////////////////
// ### LICENSE VERIFICATION ### 


void LicenseVerification::processLicense()
{
	bool verification = false;
	// First check the amount of files
	int amountLicenseFiles = checkLicenseFileNumber();
	int amountSignatureFiles = checkSignatureFileNumber();
	// If there is one of each file, everything is fine
	if (amountLicenseFiles == 1 && amountSignatureFiles == 1)
	{
		// Get the licensepath
		QString licensePath = getLicenseFilePathFromDirectory();
		// Verify the signature
		verification = verifySignature();
		if (verification)
		{
			// Read the license file
			reader->readLicenseFile(licensePath);
			// Put the data from the reader in the model
			readDataIntoModel(licensePath);
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
	// If there are no files, show a warning cause the user probably doesnt use licensing
	else if (amountLicenseFiles == 0 && amountSignatureFiles == 0)
	{
		toggleNoLicense();
	}
	// If there is only one available file or more then one from each type, its probably an
	// error from the user, so we tell him
	else
	{
		if (amountLicenseFiles != 1)
		{
			throw LicenseFileNumberException("");
		}
		else if (amountSignatureFiles != 1)
		{
			throw LicenseSignatureFileNumberException("");
		}
	}
}

int LicenseVerification::checkLicenseFileNumber()
{
	// Just iterate through the lic directory
	int counterLicenseFile = 0;
	QDirIterator itLic("lic", QStringList() << "*.lic", QDir::Files | QDir::NoDotAndDotDot | QDir::Readable | QDir::NoSymLinks);
	while (itLic.hasNext())
	{
		counterLicenseFile++;
		itLic.next();
	}

	return counterLicenseFile;
}


int LicenseVerification::checkSignatureFileNumber()
{
	// Just iterate through the lic directory
	int counterSignature = 0;
	QDirIterator itSig("lic", QStringList() << "*.sig", QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
	while (itSig.hasNext())
	{
		counterSignature++;
		itSig.next();
	}

	return counterSignature;
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

bool LicenseVerification::verifySignature()
{
	RSA::PublicKey pubKey;
	// Decoded string
	string pubKeyValue = "MIGdMA0GCSqGSIb3DQEBAQUAA4GLADCBhwKBgQDORaGFvo/jOfWYVbODeD4A/+RVIRprM0+6NJa5bCAXb4/kmVB/TcZK4LdWGvmUlPf+nSmXxwJJ6jBBAEwj4p24hGnwdyoMrZtqJdqZ13Rb5jv5/JRY+hxMBIZj8lcoZdXq6rvQGNN/fwDx7ZjQl/pzOv2dAw7xZqwHNDRuAB0zcQIBEQ";
	StringSource ss(pubKeyValue, true, new Base64Decoder);
	CryptoPP::ByteQueue bytes;
	// String to bytes
	ss.TransferTo(bytes);
	bytes.MessageEnd();
	// Load public key
	pubKey.Load(bytes);

	RSASSA_PKCS1v15_SHA_Verifier verifier(pubKey);

	// Read signed message
	QString licensePath = getLicenseFilePathFromDirectory();
	QFile licenseFile(licensePath);
	if (!licenseFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	QTextStream in(&licenseFile);
	string licenseData = in.readAll().toStdString();

	// Read signation
	string signature;
	// Get the signature path as const char* for CryptoPP
	QString signaturePath = getSignatureFilePathFromDirectory();
	QByteArray baSignaturePath = signaturePath.toLatin1();
	const char* cSignaturePath = baSignaturePath.data();
	FileSource(cSignaturePath, true, new StringSink(signature));

	// Verify signature
	bool result = verifier.VerifyMessage((const byte*)licenseData.c_str(),
		licenseData.length(), (const byte*)signature.c_str(), signature.size());

	// Result
	return result;
}

void LicenseVerification::readDataIntoModel(QString licensePath)
{
	model->setFirstName(reader->getSpecificEntryValue("Customer", model->getKeyWordFirstName()));
	model->setLastName(reader->getSpecificEntryValue("Customer", model->getKeyWordLastName()));
	model->setEmail(reader->getSpecificEntryValue("Customer", model->getKeyWordEmail()));
	model->setCompany(reader->getSpecificEntryValue("Customer", model->getKeyWordCompany()));
	model->setCustomerNumber(reader->getSpecificEntryValue("Customer", model->getKeyWordCustomerNumber()));

	// Cast to bool
	model->setFeatureFullScreen(reader->getSpecificEntryValue("Product", model->getKeyWordFullScreen()).toInt());
	// Cast to bool
	model->setFeatureSpeed(reader->getSpecificEntryValue("Product", model->getKeyWordSpeed()).toInt());
	// Cast to bool
	model->setFeatureColor(reader->getSpecificEntryValue("Product", model->getKeyWordColor()).toInt());
	// Cast to bool
	model->setFeatureHistogram(reader->getSpecificEntryValue("Product", model->getKeyWordHistogram()).toInt());
	// Cast to int
	model->setDuration(reader->getSpecificEntryValue("Product", model->getKeyWordDuration()).toInt());
	// Cast to QDate
	model->setExpirationDate(QDate::fromString(reader->getSpecificEntryValue("Product", model->getKeyWordExpirationDate()), "dd.MM.yyyy"));

	model->setMac(reader->getSpecificEntryValue("Licensing", model->getKeyWordMac()));

	if (!verifySignature())
	{
		throw LicenseSignatureException("");
	}
}

bool LicenseVerification::checkMacAdress()
{
	if (!verifySignature())
	{
		throw LicenseSignatureException("");
	}

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
	if (!verifySignature())
	{
		throw LicenseSignatureException("");
	}

	QDate licenseExpirationDate = model->getExpirationDate();
	QDate today = QDate::currentDate();
	// If today is earlier then the expiration date, everything is fine
	if (today <= licenseExpirationDate)
	{
		return true;
	}
	return false;
}

void LicenseVerification::toggleNoLicense()
{
	// Show a warning
	showMessageBox("Keine gueltige Lizenz",
		"Sie haben keine aktive Lizenz, deshalb sind keinerlei zusaetzliche Features aktiv."
		"\n"
		"Bitte lesen Sie das beilegende Dokument unter 'Help' zur Lizenzierung durch. Dieses beinhaltet "
		"Informationen, die nuetzlich sein koennten.\n"
		"\n"
		"Wenn Sie noch offene Fragen haben oder eine Lizene erwerben wollen, melden Sie sich bitte beim Support!",
		QMessageBox::Warning);

	// And set the boolean so you can tell, that the user has no licensefile
	isLicensingActive = false;
}

void LicenseVerification::onLicenseHelp()
{
	// Open the help pdf
	QString path = QDir::currentPath();
	path.append("/LicenseHelp.pdf");
	QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void LicenseVerification::showMessageBox(QString title, QString errorText, QMessageBox::Icon iconEnum /*= QMessageBox::Critical*/)
{
	// Show a critical message box
	QPushButton* licenseHelp = new QPushButton("Help");
	connect(licenseHelp, SIGNAL(clicked()), this, SLOT(onLicenseHelp()));
	QMessageBox msgBox(iconEnum, title, errorText, QMessageBox::Ok);
	msgBox.setEscapeButton(QMessageBox::Ok);
	msgBox.addButton(licenseHelp, QMessageBox::HelpRole);
	msgBox.exec();
}

bool LicenseVerification::getIsLicensingActive()
{
	return isLicensingActive;
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
