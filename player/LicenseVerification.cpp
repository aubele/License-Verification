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
#  pragma comment ( lib, "cryptlibd" )
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


std::string decode(const std::string& input) 
{   
	// Use std::string, cause bit operations are easier then
	// choose a power of two length => then compiler can replace "modulo x" by much faster "and (x-1)"   
	const size_t passwordLength = 16;   
	// at least as long as passwordLength, can be longer, too ...   
	static const char password[passwordLength] = "invalid pointer";   
	// out = in XOR NOT(password)   
	std::string result = input;   
	for (size_t i = 0; i < input.length(); i++)     
		result[i] ^= ~password[i % passwordLength];   

	return result; 
}

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
	QDirIterator itLic(decode("\xfa\xf8\xea").c_str(), QStringList() << decode("\xbc\xbf\xe5\xf7\xf0").c_str(), QDir::Files | QDir::NoDotAndDotDot | QDir::Readable | QDir::NoSymLinks);
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
	QDirIterator itSig(decode("\xfa\xf8\xea").c_str(), QStringList() << decode("\xbc\xbf\xfa\xf7\xf4").c_str(), QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
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
	QDirIterator itLic(decode("\xfa\xf8\xea").c_str(), QStringList() << decode("\xbc\xbf\xe5\xf7\xf0").c_str(), QDir::Files | QDir::NoDotAndDotDot | QDir::Readable | QDir::NoSymLinks);
	while (itLic.hasNext())
	{
		return itLic.next();
	}
	return QString();
}

const QString LicenseVerification::getSignatureFilePathFromDirectory()
{
	// Get path from lic directory
	QDirIterator itSig(decode("\xfa\xf8\xea").c_str(), QStringList() << decode("\xbc\xbf\xfa\xf7\xf4").c_str(), QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
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
	string pubKeyValue = decode("\xdb\xd8\xce\xfa\xde\xd7\xab\x98\xcc\xc3\xe7\xd6\xd8\xd3\xef\xcc\xd2\xc0\xcc\xdc\xd2\xc7\xce\x9e\xce\xa4\xd1\xdd\xca\xde\xce\xbd\xfe\xe6\xc2\xdc\xf4\xc7\xdf\x90\xdd\xf1\xd1\xd7\xfd\xf5\xa2\x95\xd9\xf7\xde\xc7\xc5\xf4\xd4\x9b\xea\xd4\xa2\xd0\xa4\xb1\xdf\xa9\xdf\xc3\xf9\xec\xde\xa6\xb0\xe9\xc1\xda\xf7\xa4\xe9\xd9\xcc\xa7\xf4\xa5\xa6\xf5\xfe\xc0\xd9\xf0\xdb\xf3\xcc\xda\xbf\xd6\xe9\xa8\xd1\xe7\xe4\xcb\xff\xc6\xfd\xf4\xe1\xc3\xfb\xc9\xf3\xed\xc7\xb5\xa0\xfb\xcb\xdc\xd2\xd3\xec\xb5\xbb\xe0\xa4\xa5\xe3\xdd\xe3\x88\xf2\xe8\xe6\xd3\xe1\xcc\xef\xae\xc5\xf4\xe7\xcb\xba\xa9\xdf\x9d\xa3\xfb\xff\xab\xbc\xdc\xc9\x86\xa4\xf8\xee\xdc\xc9\xd3\xd7\x95\xae\xfd\xea\xf1\xc9\xf2\xc3\xae\xb9\xe2\xe0\xc0\xcc\xd4\xc3\xd0\xf0\xe6\xcd\xe6\xa4\xcc\xf1\x8e\xe3\xbf\xe6\xeb\xc4\xec\xbf\x9b\xd7\xe6\xbe\xe6\xc9\xe7\xec\x97\xc1\xd4\xc4\xe4\xca\xd8\xbd\x85\xf5\xc0\xc0\xdc\xd6\xc7").c_str();
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
	model->setFirstName(reader->getSpecificEntryValue(decode("\xd5\xe4\xfa\xea\xfc\xfb\xfe\xad").c_str(), model->getKeyWordFirstName()));
	model->setLastName(reader->getSpecificEntryValue(decode("\xd5\xe4\xfa\xea\xfc\xfb\xfe\xad").c_str(), model->getKeyWordLastName()));
	model->setEmail(reader->getSpecificEntryValue(decode("\xd5\xe4\xfa\xea\xfc\xfb\xfe\xad").c_str(), model->getKeyWordEmail()));
	model->setCompany(reader->getSpecificEntryValue(decode("\xd5\xe4\xfa\xea\xfc\xfb\xfe\xad").c_str(), model->getKeyWordCompany()));
	model->setCustomerNumber(reader->getSpecificEntryValue(decode("\xd5\xe4\xfa\xea\xfc\xfb\xfe\xad").c_str(), model->getKeyWordCustomerNumber()));

	// Cast to bool
	model->setFeatureFullScreen(reader->getSpecificEntryValue(decode("\xc6\xe3\xe6\xfa\xe6\xf5\xef").c_str(), model->getKeyWordFullScreen()).toInt());
	// Cast to bool
	model->setFeatureSpeed(reader->getSpecificEntryValue(decode("\xc6\xe3\xe6\xfa\xe6\xf5\xef").c_str(), model->getKeyWordSpeed()).toInt());
	// Cast to bool
	model->setFeatureColor(reader->getSpecificEntryValue(decode("\xc6\xe3\xe6\xfa\xe6\xf5\xef").c_str(), model->getKeyWordColor()).toInt());
	// Cast to bool
	model->setFeatureHistogram(reader->getSpecificEntryValue(decode("\xc6\xe3\xe6\xfa\xe6\xf5\xef").c_str(), model->getKeyWordHistogram()).toInt());
	// Cast to int
	model->setDuration(reader->getSpecificEntryValue(decode("\xc6\xe3\xe6\xfa\xe6\xf5\xef").c_str(), model->getKeyWordDuration()).toInt());
	// Cast to QDate
	model->setExpirationDate(QDate::fromString(reader->getSpecificEntryValue(decode("\xc6\xe3\xe6\xfa\xe6\xf5\xef").c_str(), model->getKeyWordExpirationDate()), decode("\xf2\xf5\xa7\xd3\xde\xb8\xe2\xa6\xf6\xe9").c_str()));

	model->setMac(reader->getSpecificEntryValue(decode("\xda\xf8\xea\xfb\xfd\xe5\xf2\xb1\xe8").c_str(), model->getKeyWordMac()));

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
	//"Sie haben keine aktive Lizenz, deshalb sind keinerlei zusaetzliche Features aktiv."
	string message1 = decode("\xc5\xf8\xec\xbe\xfb\xf7\xf9\xba\xe1\xb0\xfd\xf4\xe2\xf4\xe8\xdf\xf7\xfa\xfd\xf7\xe5\xf3\xbb\x93\xe6\xea\xf3\xff\xf1\xb6\xad\x9b\xf3\xe2\xe1\xff\xff\xf4\xbb\xac\xe6\xfe\xf2\xb1\xe0\xff\xe4\x91\xf3\xe3\xe5\xfb\xfa\xb6\xe1\xaa\xfc\xf1\xf3\xe5\xf1\xf6\xe4\x9c\xfe\xf4\xa9\xd8\xf6\xf7\xef\xaa\xfd\xf5\xe5\xb1\xea\xf1\xf9\x96\xe0\xbf");
	//"Bitte lesen Sie das beilegende Dokument unter 'Help' zur Lizenzierung durch. Dieses beinhaltet ""Informationen, die nuetzlich sein koennten.
	string message2 = decode("\xd4\xf8\xfd\xea\xf6\xb6\xf7\xba\xfc\xf5\xf8\xb1\xd8\xf3\xe8\xdf\xf2\xf0\xfa\xbe\xf1\xf3\xf2\xb3\xea\xf7\xf3\xff\xef\xff\xad\xbb\xf9\xfa\xfc\xf3\xf6\xf8\xef\xff\xfa\xfe\xe2\xf4\xf9\xba\xaa\xb7\xf3\xfd\xf9\xb9\xb3\xec\xee\xad\xaf\xdc\xff\xeb\xee\xf4\xf7\x96\xf3\xe3\xfc\xf0\xf4\xb6\xff\xaa\xfd\xf3\xfe\xbf\xab\xde\xe4\x9a\xe5\xf4\xfa\xbe\xf1\xf3\xf2\xb1\xe7\xf1\xfa\xe5\xee\xee\xad\xb6\xf8\xf7\xe6\xec\xfe\xf7\xef\xb6\xe0\xfe\xf3\xff\xa7\xba\xe9\x96\xf3\xb1\xe7\xeb\xf6\xe2\xe1\xb3\xe6\xf3\xfe\xb1\xf8\xff\xe4\x91\xb6\xfa\xe6\xfb\xfd\xf8\xef\xba\xe1\xbe");
	//"Wenn Sie noch offene Fragen haben oder eine Lizene erwerben wollen, melden Sie sich bitte beim Support!"
	string message3 = decode("\xc1\xf4\xe7\xf0\xb3\xc5\xf2\xba\xaf\xfe\xf9\xf2\xe3\xba\xe2\x99\xf0\xf4\xe7\xfb\xb3\xd0\xe9\xbe\xe8\xf5\xf8\xb1\xe3\xfb\xef\x9a\xf8\xb1\xe6\xfa\xf6\xe4\xbb\xba\xe6\xfe\xf3\xb1\xc7\xf3\xf7\x9a\xf8\xf4\xa9\xfb\xe1\xe1\xfe\xad\xed\xf5\xf8\xb1\xfc\xf5\xe1\x93\xf3\xff\xa5\xbe\xfe\xf3\xf7\xbb\xea\xfe\xb6\xc2\xe2\xff\xad\x8c\xff\xf2\xe1\xbe\xf1\xff\xef\xab\xea\xb0\xf4\xf4\xe2\xf7\xad\xac\xe3\xe1\xf9\xf1\xe1\xe2\xba");
	string all = message1 + "\n" + message2 + "\n" + "\n" + message3;

	// Show a warning
	showMessageBox(decode("\xdd\xf4\xe0\xf0\xf6\xb6\xfc\xaa\xea\xfc\xe2\xf8\xec\xff\xad\xb3\xff\xeb\xec\xf0\xe9").c_str(),
		QString::fromStdString(all),
		QMessageBox::Warning);

	// And set the boolean so you can tell, that the user has no licensefile
	isLicensingActive = false;
}

void LicenseVerification::onLicenseHelp()
{
	// Open the help pdf
	QString path = QDir::currentPath();
	path.append(decode("\xb9\xdd\xe0\xfd\xf6\xf8\xe8\xba\xc7\xf5\xfa\xe1\xa5\xea\xe9\x99").c_str());
	QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void LicenseVerification::showMessageBox(QString title, QString errorText, QMessageBox::Icon iconEnum /*= QMessageBox::Critical*/)
{
	// Show a critical message box
	QPushButton* licenseHelp = new QPushButton(decode("\xde\xf4\xe5\xee").c_str());
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
