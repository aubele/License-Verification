#include "LicenseVerification.h"
#include "LicenseExceptions.h"
#include "AntiReverseEngineering.h"

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

#include <windows.h>

using namespace CryptoPP;


// Global timestamps for easy access
FILETIME time1,time2,time3,time4,time5,time6;


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
// ### OPERATIONS FOR VERIFICATION ### 


// Obfuscated with Opaque Predicate
int LicenseVerification::checkLicenseFileNumberObfus()
{
	int amountLicenseFiles = 0;
	QString obf = QDir::currentPath();
	// "/"
	if (obf.split(decode("\xb9").c_str()).size() > 0)
	{
		// This one
		// Just iterate through the lic directory
		int counterLicenseFile = 0;
		// "lic" || "*.lic"
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
		// Never this one
		QDirIterator itLic(obf, QDir::Files | QDir::NoDotAndDotDot | QDir::Readable | QDir::NoSymLinks);
		while (itLic.hasNext())
		{
			amountLicenseFiles++;
			verifySignatureObfus();
		}

		return amountLicenseFiles;
	}
}

// Obfuscated with Opaque Predicate
int LicenseVerification::checkSignatureFileNumberObfus()
{
	int amountSignatureFiles = 0;
	QString obf2 = QDir::currentPath();
	if (obf2.size() == 0)
	{
		// Never this one
		amountSignatureFiles = 0;
		getSignatureFilePathFromDirectoryObfus();
		return amountSignatureFiles;
	}
	else
	{
		// This one
		// Just iterate through the lic directory
		int counterSignature = 0;
		// "lic" || "*.sig"
		QDirIterator itSig(decode("\xfa\xf8\xea").c_str(), QStringList() << decode("\xbc\xbf\xfa\xf7\xf4").c_str(), QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
		while (itSig.hasNext())
		{
			counterSignature++;
			itSig.next();
		}

		return counterSignature;
	}
}

// Obfuscated with Opaque Predicate
const QString LicenseVerification::getLicenseFilePathFromDirectoryObfus()
{
	if (checkLicenseFileNumberObfus() > 0)
	{
		// This one
		// Get path from lic directory
		// "lic" || "*.lic"
		QDirIterator itLic(decode("\xfa\xf8\xea").c_str(), QStringList() << decode("\xbc\xbf\xe5\xf7\xf0").c_str(), QDir::Files | QDir::NoDotAndDotDot | QDir::Readable | QDir::NoSymLinks);
		while (itLic.hasNext())
		{
			return itLic.next();
		}
		return QString();
	}
	else
	{
		// Never this one
		QString licensePath = "";
		QString lic = getLicenseFilePathFromDirectoryObfus();

		reader->readLicenseFile(licensePath);
		reader->readLicenseFile(lic);
		return licensePath;
	}
}

// Obfuscated with Opaque Predicate
const QString LicenseVerification::getSignatureFilePathFromDirectoryObfus()
{
	if (checkSignatureFileNumberObfus() > 0)
	{
		// This one
		// Get path from lic directory
		// "lic" || "*.sig"
		QDirIterator itSig(decode("\xfa\xf8\xea").c_str(), QStringList() << decode("\xbc\xbf\xfa\xf7\xf4").c_str(), QDir::Files | QDir::NoDotAndDotDot | QDir::Readable);
		while (itSig.hasNext())
		{
			return itSig.next();
		}
		return QString();
	}
	else
	{
		// Never this one
		QString lic = getLicenseFilePathFromDirectoryObfus();
		if (checkSignatureFileNumberObfus() > 0)
			reader->readLicenseFile(lic);

		readDataIntoModelObfus();
		verifySignatureObfus();
		return lic;
	}
}


// Obfuscated with Opaque Predicate
void LicenseVerification::toggleNoLicenseObfus()
{
	if (checkLicenseFileNumberObfus() < 1 && checkSignatureFileNumberObfus() < 1)
	{
		// This one
		// Build a string that gets displayed in the warning box
		//"Sie haben keine aktive Lizenz, deshalb sind keinerlei zusaetzliche Features aktiv."
		string message1 = decode("\xc5\xf8\xec\xbe\xfb\xf7\xf9\xba\xe1\xb0\xfd\xf4\xe2\xf4\xe8\xdf\xf7\xfa\xfd\xf7\xe5\xf3\xbb\x93\xe6\xea\xf3\xff\xf1\xb6\xad\x9b\xf3\xe2\xe1\xff\xff\xf4\xbb\xac\xe6\xfe\xf2\xb1\xe0\xff\xe4\x91\xf3\xe3\xe5\xfb\xfa\xb6\xe1\xaa\xfc\xf1\xf3\xe5\xf1\xf6\xe4\x9c\xfe\xf4\xa9\xd8\xf6\xf7\xef\xaa\xfd\xf5\xe5\xb1\xea\xf1\xf9\x96\xe0\xbf");
		//"Bitte lesen Sie das beilegende Dokument unter 'Help' zur Lizenzierung durch. Dieses beinhaltet ""Informationen, die nuetzlich sein koennten.
		string message2 = decode("\xd4\xf8\xfd\xea\xf6\xb6\xf7\xba\xfc\xf5\xf8\xb1\xd8\xf3\xe8\xdf\xf2\xf0\xfa\xbe\xf1\xf3\xf2\xb3\xea\xf7\xf3\xff\xef\xff\xad\xbb\xf9\xfa\xfc\xf3\xf6\xf8\xef\xff\xfa\xfe\xe2\xf4\xf9\xba\xaa\xb7\xf3\xfd\xf9\xb9\xb3\xec\xee\xad\xaf\xdc\xff\xeb\xee\xf4\xf7\x96\xf3\xe3\xfc\xf0\xf4\xb6\xff\xaa\xfd\xf3\xfe\xbf\xab\xde\xe4\x9a\xe5\xf4\xfa\xbe\xf1\xf3\xf2\xb1\xe7\xf1\xfa\xe5\xee\xee\xad\xb6\xf8\xf7\xe6\xec\xfe\xf7\xef\xb6\xe0\xfe\xf3\xff\xa7\xba\xe9\x96\xf3\xb1\xe7\xeb\xf6\xe2\xe1\xb3\xe6\xf3\xfe\xb1\xf8\xff\xe4\x91\xb6\xfa\xe6\xfb\xfd\xf8\xef\xba\xe1\xbe");
		//"Wenn Sie noch offene Fragen haben oder eine Lizene erwerben wollen, melden Sie sich bitte beim Support!"
		string message3 = decode("\xc1\xf4\xe7\xf0\xb3\xc5\xf2\xba\xaf\xfe\xf9\xf2\xe3\xba\xe2\x99\xf0\xf4\xe7\xfb\xb3\xd0\xe9\xbe\xe8\xf5\xf8\xb1\xe3\xfb\xef\x9a\xf8\xb1\xe6\xfa\xf6\xe4\xbb\xba\xe6\xfe\xf3\xb1\xc7\xf3\xf7\x9a\xf8\xf4\xa9\xfb\xe1\xe1\xfe\xad\xed\xf5\xf8\xb1\xfc\xf5\xe1\x93\xf3\xff\xa5\xbe\xfe\xf3\xf7\xbb\xea\xfe\xb6\xc2\xe2\xff\xad\x8c\xff\xf2\xe1\xbe\xf1\xff\xef\xab\xea\xb0\xf4\xf4\xe2\xf7\xad\xac\xe3\xe1\xf9\xf1\xe1\xe2\xba");
		// Build the whole string
		string all = message1 + "\n" + message2 + "\n" + "\n" + message3;
		// Show the warning
		// "Keine gueltige Lizenz"
		showMessageBox(decode("\xdd\xf4\xe0\xf0\xf6\xb6\xfc\xaa\xea\xfc\xe2\xf8\xec\xff\xad\xb3\xff\xeb\xec\xf0\xe9").c_str(),
			QString::fromStdString(all),
			QMessageBox::Warning);

		// And set the boolean so you can tell, that the user has no licensefile
		isLicensingActive = false;
	}
	else
	{
		// Never this one
		QString sig = getSignatureFilePathFromDirectoryObfus();
		if (checkLicenseFileNumberObfus() > 0)
			reader->readLicenseFile(sig);
		QString lic = getLicenseFilePathFromDirectoryObfus();
		if (checkSignatureFileNumberObfus() > 0)
			reader->readLicenseFile(lic);
		readDataIntoModelObfus();
		verifySignatureObfus();
	}
}

// Obfuscated with Opaque Predicate
bool LicenseVerification::checkMacAdressObfus()
{
	// Append "t:t" || split ":"
	if (QDir::currentPath().append(decode("\xe2\xab\xfd").c_str()).split(decode("\xac").c_str()).size() == 0)
	{
		// Never this one
		reader->readLicenseFile(getLicenseFilePathFromDirectoryObfus());
		if (verifySignatureObfus())
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
		// Check mac
		if (!verifySignatureObfus())
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

// Obfuscated with Opaque Predicate
bool LicenseVerification::checkExpirationDateObfus()
{
	QDate obfus = QDate::currentDate();
	obfus = obfus.addMonths(2);
	if (obfus == QDate::currentDate())
	{
		// Never this one
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
		if (!verifySignatureObfus())
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

// Obfuscated with Opaque Predicate
void LicenseVerification::readDataIntoModelObfus()
{
	if (getLicenseFilePathFromDirectoryObfus() != "" && getSignatureFilePathFromDirectoryObfus() != "")
	{
		// This one
		// "Customer" for the next 5
		model->setFirstName(reader->getSpecificEntryValue(decode("\xd5\xe4\xfa\xea\xfc\xfb\xfe\xad").c_str(), model->getKeyWordFirstName()));
		model->setLastName(reader->getSpecificEntryValue(decode("\xd5\xe4\xfa\xea\xfc\xfb\xfe\xad").c_str(), model->getKeyWordLastName()));
		model->setEmail(reader->getSpecificEntryValue(decode("\xd5\xe4\xfa\xea\xfc\xfb\xfe\xad").c_str(), model->getKeyWordEmail()));
		model->setCompany(reader->getSpecificEntryValue(decode("\xd5\xe4\xfa\xea\xfc\xfb\xfe\xad").c_str(), model->getKeyWordCompany()));
		model->setCustomerNumber(reader->getSpecificEntryValue(decode("\xd5\xe4\xfa\xea\xfc\xfb\xfe\xad").c_str(), model->getKeyWordCustomerNumber()));
		// Cast to bool
		// "Product" for the next 5
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
		// "Product" || "dd.mm.yyyy"
		model->setExpirationDate(QDate::fromString(reader->getSpecificEntryValue(decode("\xc6\xe3\xe6\xfa\xe6\xf5\xef").c_str(), model->getKeyWordExpirationDate()), decode("\xf2\xf5\xa7\xd3\xde\xb8\xe2\xa6\xf6\xe9").c_str()));
		// "Licensing"
		model->setMac(reader->getSpecificEntryValue(decode("\xda\xf8\xea\xfb\xfd\xe5\xf2\xb1\xe8").c_str(), model->getKeyWordMac()));

		if (!verifySignatureObfus())
		{
			throw LicenseSignatureException("");
		}
	}
	else
	{
		// Never this one
		QStringList list;
		list << getModelCompany() << getModelCustomerNumber() << getModelExpirationDate().toString(decode("\xf2\xf5\xa7\xd3\xde\xb8\xe2\xa6\xf6\xe9").c_str())
			<< getModelFirstName() << getModelLastName();
		if (list.at(2) == "")
		{
			toggleNoLicenseObfus();
		}
		verifySignatureObfus();
	}
}


///////////////////////////////////////////////////////////////////////////////////////////
// ### LICENSE VERIFICATION ### 


// Obfuscated with Opaque Predicate
bool LicenseVerification::verifySignatureOnProcessObfus(bool& cancel)
{
	if (checkLicenseFileNumberObfus() > 0 && checkSignatureFileNumberObfus() == 0)
	{
		// Never this one
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
		if (isLicensingActive == false)
		{
			return true;
		}

		if (checkNtQueryInformationProcess())
		{
			cancel = true;
			return false;
		}
		if (int2DCheck())
		{
			cancel = true;
			return false;
		}

		RSA::PublicKey pubKey;
		// Decoded string
		//"MIGdMA0GCSqGSIb3DQEBAQUAA4GLADCBhwKBgQDORaGFvo/jOfWYVbODeD4A/+RVIRprM0+6NJa5bCAXb4/kmVB/TcZK4LdWGvmUlPf+nSmXxwJJ6jBBAEwj4p24hGnwdyoMrZtqJdqZ13Rb5jv5/JRY+hxMBIZj8lcoZdXq6rvQGNN/fwDx7ZjQl/pzOv2dAw7xZqwHNDRuAB0zcQIBEQ"
		string pubKeyValue = decode("\xdb\xd8\xce\xfa\xde\xd7\xab\x98\xcc\xc3\xe7\xd6\xd8\xd3\xef\xcc\xd2\xc0\xcc\xdc\xd2\xc7\xce\x9e\xce\xa4\xd1\xdd\xca\xde\xce\xbd\xfe\xe6\xc2\xdc\xf4\xc7\xdf\x90\xdd\xf1\xd1\xd7\xfd\xf5\xa2\x95\xd9\xf7\xde\xc7\xc5\xf4\xd4\x9b\xea\xd4\xa2\xd0\xa4\xb1\xdf\xa9\xdf\xc3\xf9\xec\xde\xa6\xb0\xe9\xc1\xda\xf7\xa4\xe9\xd9\xcc\xa7\xf4\xa5\xa6\xf5\xfe\xc0\xd9\xf0\xdb\xf3\xcc\xda\xbf\xd6\xe9\xa8\xd1\xe7\xe4\xcb\xff\xc6\xfd\xf4\xe1\xc3\xfb\xc9\xf3\xed\xc7\xb5\xa0\xfb\xcb\xdc\xd2\xd3\xec\xb5\xbb\xe0\xa4\xa5\xe3\xdd\xe3\x88\xf2\xe8\xe6\xd3\xe1\xcc\xef\xae\xc5\xf4\xe7\xcb\xba\xa9\xdf\x9d\xa3\xfb\xff\xab\xbc\xdc\xc9\x86\xa4\xf8\xee\xdc\xc9\xd3\xd7\x95\xae\xfd\xea\xf1\xc9\xf2\xc3\xae\xb9\xe2\xe0\xc0\xcc\xd4\xc3\xd0\xf0\xe6\xcd\xe6\xa4\xcc\xf1\x8e\xe3\xbf\xe6\xeb\xc4\xec\xbf\x9b\xd7\xe6\xbe\xe6\xc9\xe7\xec\x97\xc1\xd4\xc4\xe4\xca\xd8\xbd\x85\xf5\xc0\xc0\xdc\xd6\xc7").c_str();
		StringSource ss(pubKeyValue, true, new Base64Decoder);
		CryptoPP::ByteQueue bytes;
		// String to bytes
		ss.TransferTo(bytes);
		bytes.MessageEnd();
		// Load public key
		pubKey.Load(bytes);

		SYSTEMTIME systime4;
		GetSystemTime(&systime4);
		SystemTimeToFileTime(&systime4, &time4);

		double timeExecution = (time4.dwLowDateTime - time3.dwLowDateTime) / 10000.0;
		if (timeExecution > 20)
		{
			cancel = true;
			return false;
		}

		if (int2DCheck())
		{
			cancel = true;
			return false;
		}

		RSASSA_PKCS1v15_SHA_Verifier verifier(pubKey);

		// Read signed message
		QString licensePath = getLicenseFilePathFromDirectoryObfus();
		QFile licenseFile(licensePath);
		if (!licenseFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return "";
		QTextStream in(&licenseFile);
		string licenseData = in.readAll().toStdString();

		// Read signation
		string signature;
		// Get the signature path as const char* for CryptoPP
		QString signaturePath = getSignatureFilePathFromDirectoryObfus();
		QByteArray baSignaturePath = signaturePath.toLatin1();
		const char* cSignaturePath = baSignaturePath.data();
		FileSource(cSignaturePath, true, new StringSink(signature));

		SYSTEMTIME systime5;
		GetSystemTime(&systime5);
		SystemTimeToFileTime(&systime5, &time5);

		timeExecution = (time5.dwLowDateTime - time4.dwLowDateTime) / 10000.0;
		if (timeExecution > 20)
		{
			cancel = true;
			return false;
		}

		if (int2DCheck())
		{
			cancel = true;
			return false;
		}

		// Verify signature
		bool result = verifier.VerifyMessage((const byte*)licenseData.c_str(),
			licenseData.length(), (const byte*)signature.c_str(), signature.size());

		SYSTEMTIME systime6;
		GetSystemTime(&systime6);
		SystemTimeToFileTime(&systime6, &time6);

		timeExecution = (time6.dwLowDateTime - time5.dwLowDateTime) / 10000.0;
		if (timeExecution > 20)
		{
			cancel = true;
			return false;
		}
		// Result
		return result;
	}
}

// Obfuscated with Opaque Predicate
bool LicenseVerification::verifySignatureObfus()
{
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
			throw runtime_error(decode("DSA key generation failed").c_str());
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
		if (isLicensingActive == false)
		{
			return true;
		}

		// This one
		RSA::PublicKey pubKey;
		// Decoded string
		//"MIGdMA0GCSqGSIb3DQEBAQUAA4GLADCBhwKBgQDORaGFvo/jOfWYVbODeD4A/+RVIRprM0+6NJa5bCAXb4/kmVB/TcZK4LdWGvmUlPf+nSmXxwJJ6jBBAEwj4p24hGnwdyoMrZtqJdqZ13Rb5jv5/JRY+hxMBIZj8lcoZdXq6rvQGNN/fwDx7ZjQl/pzOv2dAw7xZqwHNDRuAB0zcQIBEQ"
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
		QString licensePath = getLicenseFilePathFromDirectoryObfus();
		QFile licenseFile(licensePath);
		if (!licenseFile.open(QIODevice::ReadOnly | QIODevice::Text))
			return "";

		QTextStream in(&licenseFile);
		string licenseData = in.readAll().toStdString();

		// Read signation
		string signature;
		// Get the signature path as const char* for CryptoPP
		QString signaturePath = getSignatureFilePathFromDirectoryObfus();
		QByteArray baSignaturePath = signaturePath.toLatin1();
		const char* cSignaturePath = baSignaturePath.data();
		FileSource(cSignaturePath, true, new StringSink(signature));

		// Verify signature
		bool result = verifier.VerifyMessage((const byte*)licenseData.c_str(),
			licenseData.length(), (const byte*)signature.c_str(), signature.size());

		// Result
		return result;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////
// ### MAIN PROCESS METHOD ### 


// This method got obfuscated with Control Flow Flattening
bool LicenseVerification::processLicense()
{
	SYSTEMTIME systime1;
	GetSystemTime(&systime1);
	SystemTimeToFileTime(&systime1, &time1);
	
	bool isDebug = checkDebuggerWithTrapFlag();
	if (isDebug)
	{
		return false;
	}

	int amountLicenseFiles;
	int amountSignatureFiles;
	QString licensePath;
	bool verification;

	if (checkNtQueryInformationProcess())
	{
		return false;
	}

	int swVar = 1;
	while (swVar != 0)
	{
		switch (swVar)
		{
		case 1:
		{
			if (int2DCheck())
			{
				return false;
			}
			amountLicenseFiles = checkLicenseFileNumberObfus();
			swVar = 2;
			break;
		}
		case 2:
		{
			if (int2DCheck())
			{
				return false;
			}
			amountSignatureFiles = checkSignatureFileNumberObfus();
			swVar = 3;
			break;
		}
		case 3:
		{
			SYSTEMTIME systime2;
			GetSystemTime(&systime2);
			SystemTimeToFileTime(&systime2, &time2);

			double timeExecution = (time2.dwLowDateTime - time1.dwLowDateTime) / 10000.0;
			if (timeExecution > 15)
			{
				return false;
			}

			isDebug = checkDebuggerWithTrapFlag();
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
			if (int2DCheck())
			{
				return false;
			}
			// Get the licensepath
			licensePath = getLicenseFilePathFromDirectoryObfus();
			swVar = 5;
			break;
		}
		case 5:
		{
			isDebug = checkDebuggerWithTrapFlag();
			if (isDebug)
			{
				return false;
			}

			SYSTEMTIME systime3;
			GetSystemTime(&systime3);
			SystemTimeToFileTime(&systime3, &time3);

			double timeExecution = (time3.dwLowDateTime - time2.dwLowDateTime) / 10000.0;
			if (timeExecution > 15)
			{
				return false;
			}


			if (int2DCheck())
			{
				return false;
			}

			if (checkNtQueryInformationProcess())
			{
				return false;
			}

			// Verify the signature
			bool cancel = false;
			verification = verifySignatureOnProcessObfus(cancel);
			if (cancel)
			{
				return false;
			}
			isDebug = checkDebuggerWithTrapFlag();
			if (isDebug)
			{
				return false;
			}
			swVar = 6;
			break;
		}
		case 6:
		{
			if (int2DCheck())
			{
				return false;
			}
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
			isDebug = checkDebuggerWithTrapFlag();
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

	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////
// ### OTHER OPERATIONS ### 


void LicenseVerification::onLicenseHelp()
{
	// Open the help pdf
	QString path = QDir::currentPath();
	//"/LicenseHelp.pdf"
	path.append(decode("\xb9\xdd\xe0\xfd\xf6\xf8\xe8\xba\xc7\xf5\xfa\xe1\xa5\xea\xe9\x99").c_str());
	QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void LicenseVerification::showMessageBox(QString title, QString errorText, QMessageBox::Icon iconEnum /*= QMessageBox::Critical*/)
{
	// Show a critical message box
	// "Help"
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