#include "LicenseModel.h"
#include "LicenseVerification.h"


LicenseModel::LicenseModel()
{
}

LicenseModel::~LicenseModel()
{
}

////////////////////////////////////////////////////////////////////////////////////

const QString LicenseModel::getFirstName()
{
	return firstName;
}

const QString LicenseModel::getKeyWordFirstName()
{
	return decode("\xf0\xf8\xfb\xed\xe7\xd8\xfa\xb2\xea").c_str();
}

void LicenseModel::setFirstName(const QString& firstName)
{
	this->firstName = firstName;
}

////////////////////////////////////////////////////////////////////////////////////

const QString LicenseModel::getLastName()
{
	return lastName;
}

const QString LicenseModel::getKeyWordLastName()
{
	return decode("\xfa\xf0\xfa\xea\xdd\xf7\xf6\xba").c_str();
}

void LicenseModel::setLastName(const QString& lastName)
{
	this->lastName = lastName;
}

////////////////////////////////////////////////////////////////////////////////////

const QString LicenseModel::getEmail()
{
	return email;
}

const QString LicenseModel::getKeyWordEmail()
{
	return decode("\xf3\xfc\xe8\xf7\xff").c_str();
}

void LicenseModel::setEmail(const QString& email)
{
	this->email = email;
}

////////////////////////////////////////////////////////////////////////////////////

const QString LicenseModel::getCompany()
{
	return company;
}

const QString LicenseModel::getKeyWordCompany()
{
	return decode("\xf5\xfe\xe4\xee\xf2\xf8\xe2").c_str();
}

void LicenseModel::setCompany(const QString& company)
{
	this->company = company;
}

////////////////////////////////////////////////////////////////////////////////////

const QString LicenseModel::getMac()
{
	return mac;
}

const QString LicenseModel::getKeyWordMac()
{
	return decode("\xfb\xf0\xea").c_str();
}

void LicenseModel::setMac(const QString& mac)
{
	this->mac = mac;
}

////////////////////////////////////////////////////////////////////////////////////

bool LicenseModel::getFeatureFullScreen()
{
	return featureFullScreen;
}

const QString LicenseModel::getKeyWordFullScreen()
{
	return decode("\xf0\xe4\xe5\xf2\xc0\xf5\xe9\xba\xea\xfe").c_str();
}

void LicenseModel::setFeatureFullScreen(bool fullScreen)
{
	this->featureFullScreen = fullScreen;
}

////////////////////////////////////////////////////////////////////////////////////

bool LicenseModel::getFeatureSpeed()
{
	return featureSpeed;
}

const QString LicenseModel::getKeyWordSpeed()
{
	return decode("\xe5\xe1\xec\xfb\xf7").c_str();
}

void LicenseModel::setFeatureSpeed(bool speed)
{
	this->featureSpeed = speed;
}

////////////////////////////////////////////////////////////////////////////////////

bool LicenseModel::getFeatureColor()
{
	return featureColor;
}

const QString LicenseModel::getKeyWordColor()
{
	return decode("\xf5\xfe\xe5\xf1\xe1").c_str();
}

void LicenseModel::setFeatureColor(bool color)
{
	this->featureColor = color;
}

////////////////////////////////////////////////////////////////////////////////////

bool LicenseModel::getFeatureHistogram()
{
	return featureHistogram;
}
const QString LicenseModel::getKeyWordHistogram()
{
	return decode("\xfe\xf8\xfa\xea\xfc\xf1\xe9\xbe\xe2").c_str();
}

void LicenseModel::setFeatureHistogram(bool histogram)
{
	this->featureHistogram = histogram;
}

////////////////////////////////////////////////////////////////////////////////////

int LicenseModel::getDuration()
{
	return duration;
}

const QString LicenseModel::getKeyWordDuration()
{
	return decode("\xf2\xe4\xfb\xff\xe7\xff\xf4\xb1").c_str();
}

void LicenseModel::setDuration(int duration)
{
	this->duration = duration;
}

////////////////////////////////////////////////////////////////////////////////////

const QDate& LicenseModel::getExpirationDate()
{
	return expirationDate;
}

const QString LicenseModel::getKeyWordExpirationDate()
{
	return decode("\xf3\xe9\xf9\xf7\xe1\xf7\xef\xb6\xe0\xfe\xd2\xf0\xff\xff").c_str();
}

void LicenseModel::setExpirationDate(const QDate& expirationDate)
{
	this->expirationDate = expirationDate;
}

////////////////////////////////////////////////////////////////////////////////////

const QString LicenseModel::getCustomerNumber()
{
	return customerNumber;
}

const QString LicenseModel::getKeyWordCustomerNumber()
{
	return decode("\xf5\xe4\xfa\xea\xfc\xfb\xfe\xad\xc1\xe5\xfb\xf3\xee\xe8").c_str();
}

void LicenseModel::setCustomerNumber(const QString& customerNumber)
{
	this->customerNumber = customerNumber;
}
