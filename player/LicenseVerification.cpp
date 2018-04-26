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

#include <rsa.h>
#include <dsa.h>
#include <osrng.h>
#include <base64.h>
#include <files.h>

#include <excpt.h>

using namespace CryptoPP;

// Crypto++ Library
#ifdef _DEBUG
#  pragma comment ( lib, "cryptlibd" )
#else
#  pragma comment ( lib, "cryptlib32" )
#endif


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

int LicenseVerification::checkLicenseFileNumberObfus()
{
	int amountLicenseFiles = 0;
	// opaque predicate
	QString obf = QDir::currentPath();
	// "/"
	if (obf.split(decode("\xb9").c_str()).size() > 0)
	{
		// Always this one
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
	else
	{
		QDirIterator itLic(obf, QDir::Files | QDir::NoDotAndDotDot | QDir::Readable | QDir::NoSymLinks);
		while (itLic.hasNext())
		{
			amountLicenseFiles++;
			verifySignatureObfus1();
		}

		return amountLicenseFiles;
	}
}

int LicenseVerification::checkSignatureFileNumberObfus()
{
	// opaque predicate
	int amountSignatureFiles = 0;
	QString obf2 = QDir::currentPath();
	if (obf2.size() == 0)
	{
		amountSignatureFiles = 0;
		getSignatureFilePathFromDirectoryObfus();
		return amountSignatureFiles;
	}
	else
	{
		// Always this one
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
}

const QString LicenseVerification::getLicenseFilePathFromDirectoryObfus()
{
	if (checkLicenseFileNumberObfus() > 0)
	{
		// This one
		// Get path from lic directory
		QDirIterator itLic(decode("\xfa\xf8\xea").c_str(), QStringList() << decode("\xbc\xbf\xe5\xf7\xf0").c_str(), QDir::Files | QDir::NoDotAndDotDot | QDir::Readable | QDir::NoSymLinks);
		while (itLic.hasNext())
		{
			return itLic.next();
		}
		return QString();
	}
	else
	{
		QString licensePath = "";
		QString lic = getLicenseFilePathFromDirectoryObfus();

		reader->readLicenseFile(licensePath);
		reader->readLicenseFile(lic);
		return licensePath;
	}
}

const QString LicenseVerification::getSignatureFilePathFromDirectoryObfus()
{
	// Get the licensepath
	if (checkSignatureFileNumberObfus() > 0)
	{
		// This one
		// Get path from lic directory
		QDirIterator itSig(decode("\xfa\xf8\xea").c_str(), QStringList() << decode("\xbc\xbf\xfa\xf7\xf4").c_str(), QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
		while (itSig.hasNext())
		{
			return itSig.next();
		}
		return QString();
	}
	else
	{
		QString lic = getLicenseFilePathFromDirectoryObfus();
		if (checkSignatureFileNumberObfus() > 0)
			reader->readLicenseFile(lic);

		readDataIntoModelObfus();
		verifySignatureObfus1();
		return lic;
	}
}

void LicenseVerification::toggleNoLicenseObfus()
{
	if (checkLicenseFileNumberObfus() < 1 && checkSignatureFileNumberObfus() < 1)
	{
		// This one
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
	else
	{
		QString sig = getSignatureFilePathFromDirectoryObfus();
		if (checkLicenseFileNumberObfus() > 0)
			reader->readLicenseFile(sig);
		QString lic = getLicenseFilePathFromDirectoryObfus();
		if (checkSignatureFileNumberObfus() > 0)
			reader->readLicenseFile(lic);
		readDataIntoModelObfus();
		verifySignatureObfus1();
	}
}

bool LicenseVerification::checkMacAdressObfus()
{
	// Check mac
	// Append "t:t" split ":"
	if (QDir::currentPath().append(decode("\xe2\xab\xfd").c_str()).split(decode("\xac").c_str()).size() == 0)
	{
		reader->readLicenseFile(getLicenseFilePathFromDirectoryObfus());
		if (verifySignatureObfus1())
		{
			checkExpirationDateObfus();
			return true;
		}
		else
		{
			checkMacAdressObfus();
			return false;
		}
	}
	else
	{
		// This one
		if (!verifySignatureObfus1())
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
}

bool LicenseVerification::checkExpirationDateObfus()
{
	QString start = "Start";
	this->setObjectName(start);
	QDate obfus = QDate::currentDate();
	obfus = obfus.addMonths(2);
	if (obfus == QDate::currentDate())
	{
		QString lic = getLicenseFilePathFromDirectoryObfus();
		QDate licenseExpirationDate = model->getExpirationDate();
		if (QDate::isLeapYear(licenseExpirationDate.year()))
		{
			return true;
		}
		return false;
	}
	else
	{
		// This one
		// Check date
		if (!verifySignatureObfus1())
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
}

void LicenseVerification::readDataIntoModelObfus()
{
	if (getLicenseFilePathFromDirectoryObfus() != "" && getSignatureFilePathFromDirectoryObfus() != "")
	{
		// This one
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

		if (!verifySignatureObfus1())
		{
			throw LicenseSignatureException("");
		}
	}
	else
	{
		QStringList list;
		list << getModelCompany() << getModelCustomerNumber() << getModelExpirationDate().toString(decode("\xf2\xf5\xa7\xd3\xde\xb8\xe2\xa6\xf6\xe9").c_str())
			<< getModelFirstName() << getModelLastName();
		if (list.at(2) == "")
		{
			toggleNoLicenseObfus();
		}
		verifySignatureObfus1();
	}
}

bool LicenseVerification::verifySignatureObfus1()
{
	if (isLicensingActive == false)
	{
		return true;
	}

	if (checkLicenseFileNumberObfus() > 0 && checkSignatureFileNumberObfus() == 0)
	{
		AutoSeededRandomPool rng;

		// Generate Private Key
		DSA::PrivateKey privateKey;
		privateKey.GenerateRandomWithKeySize(rng, 2048);

		// Generate Public Key   
		DSA::PublicKey publicKey;
		publicKey.AssignFrom(privateKey);
		if (!privateKey.Validate(rng, 3) || !publicKey.Validate(rng, 3))
		{
			throw runtime_error("DSA key generation failed");
		}

		string message = "";
		string signature;

		DSA::Verifier verifier(publicKey);
		StringSource ss2(message + signature, true,
			new SignatureVerificationFilter(
				verifier, NULL
				/* SIGNATURE_AT_END */
			)
		);
		bool b = verifier.VerifyMessage((const byte*)message.c_str(),
			message.length(), (const byte*)signature.c_str(), signature.size());
		return b;
	}
	else
	{
		// This one
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
		string licenseData = verifySignatureObfus2();
		// Read signation
		string signature = verifySignatureObfus3();

		// Verify signature
		bool result = verifier.VerifyMessage((const byte*)licenseData.c_str(),
			licenseData.length(), (const byte*)signature.c_str(), signature.size());

		// Result
		return result;
	}
}

string LicenseVerification::verifySignatureObfus2()
{
	if (checkLicenseFileNumberObfus() > 0 && getSignatureFilePathFromDirectoryObfus() == "")
	{
		QDir dir = QDir::current();
		QString s = getSignatureFilePathFromDirectoryObfus();
		if (getIsLicensingActive())
			dir.setPath(s);

		reader->readLicenseFile(dir.path());
		readDataIntoModelObfus();

		string ret = s.toStdString();
		return ret;
	}
	else
	{
		// This one
		// Read signed message
		QString licensePath = getLicenseFilePathFromDirectoryObfus();
		QFile licenseFile(licensePath);
		if (!licenseFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return "";

		QTextStream in(&licenseFile);
		string licenseData = in.readAll().toStdString();

		return licenseData;
	}
}

string LicenseVerification::verifySignatureObfus3()
{
	// + "t:t"
	if (getLicenseFilePathFromDirectoryObfus() + decode("\xe2\xab\xfd").c_str() != QDir::currentPath())
	{
		// This one
		// Read signation
		string signature;

		// Get the signature path as const char* for CryptoPP
		QString signaturePath = getSignatureFilePathFromDirectoryObfus();
		QByteArray baSignaturePath = signaturePath.toLatin1();
		const char* cSignaturePath = baSignaturePath.data();
		FileSource(cSignaturePath, true, new StringSink(signature));

		return signature;
	}
	else
	{
		QString s = getLicenseFilePathFromDirectoryObfus();
		if (getIsLicensingActive())
			reader->readLicenseFile(s);

		readDataIntoModelObfus();
		string ret = s.toStdString();
		return ret;
	}
}

bool LicenseVerification::checkIt()
{
	bool isDebugged = true;
	__try
	{
		__asm
		{
			pushfd
			or dword ptr[esp], 0x100 // set the Trap Flag 
			popfd                    // Load the value into EFLAGS register
			nop
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		// If an exception has been raised debugger is not present
		isDebugged = false;
	}
	return isDebugged;
}

bool LicenseVerification::processLicense()
{
	QString start = "Start";
	this->setObjectName(start);

	bool isDebug = checkIt();
	if (isDebug)
	{
		return false;
	}

	int amountLicenseFiles;
	int amountSignatureFiles;

	QString licensePath;

	bool verification;

	int swVar = 1;
	while (swVar != 0)
	{
		switch (swVar)
		{
		case 1:
		{
			amountLicenseFiles = checkLicenseFileNumberObfus();
			swVar = 2;
			break;
		}
		case 2:
		{
			amountSignatureFiles = checkSignatureFileNumberObfus();
			swVar = 3;
			break;
		}
		case 3:
		{
			isDebug = checkIt();
			if (isDebug)
			{
				return false;
			}
			if (amountLicenseFiles == 1 && amountSignatureFiles == 1)
			{
				swVar = 4;
				break;
			}
			swVar = 12;
			break;
		}
		case 4:
		{
			// Get the licensepath
			licensePath = getLicenseFilePathFromDirectoryObfus();
			swVar = 5;
			break;
		}
		case 5:
		{
			isDebug = checkIt();
			if (isDebug)
			{
				return false;
			}
			// Verify the signature
			verification = verifySignatureObfus1();
			isDebug = checkIt();
			if (isDebug)
			{
				return false;
			}
			swVar = 6;
			break;
		}
		case 6:
		{
			if (verification)
			{
				swVar = 7;
				break;
			}
			swVar = 11;
			break;
		}
		case 7:
		{
			isDebug = checkIt();
			if (isDebug)
			{
				return false;
			}
			// Read the license file
			reader->readLicenseFile(licensePath);
			swVar = 8;
			break;
		}
		case 8:
		{
			// Put the data from the reader in the model
			readDataIntoModelObfus();
			swVar = 9;
			break;
		}
		case 9:
		{
			// Check mac
			if (!checkMacAdressObfus())
			{
				throw LicenseMacAdressException("");
			}
			swVar = 10;
			break;
		}
		case 10:
		{
			// Check date
			if (!checkExpirationDateObfus())
			{
				throw LicenseExpirationDateException("");
			}
			swVar = 0;
			break;
		}
		case 11:
		{
			if (!verification)
			{
				throw LicenseSignatureException("");
			}
			swVar = 0;
			break;
		}
		case 12:
		{
			if (amountLicenseFiles == 0 && amountSignatureFiles == 0)
			{
				swVar = 13;
				break;
			}
			swVar = 14;
			break;
		}
		case 13:
		{
			toggleNoLicenseObfus();
			swVar = 0;
			break;
		}
		case 14:
		{
			if (amountLicenseFiles != 1)
			{
				swVar = 15;
				break;
			}
			swVar = 16;
			break;
		}
		case 15:
		{
			throw LicenseFileNumberException("");
			swVar = 0;
			break;
		}
		case 16:
		{
			if (amountSignatureFiles != 1)
			{
				swVar = 17;
				break;
			}
			swVar = 0;
			break;
		}
		case 17:
		{
			throw LicenseSignatureFileNumberException("");
			swVar = 0;
			break;
		}
		}
	}
	QString end = "Ende";
	this->setObjectName(end);

	return true;
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