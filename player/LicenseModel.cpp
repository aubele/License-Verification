#include "LicenseModel.h"


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
	return "firstName";
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
	return "lastName";
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
	return "email";
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
	return "company";
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
	return "mac";
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
	return "fullScreen";
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
	return "speed";
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
	return "color";
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
	return "histogram";
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
	return "duration";
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
	return "expirationDate";
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
	return "customerNumber";
}

void LicenseModel::setCustomerNumber(const QString& customerNumber)
{
	this->customerNumber = customerNumber;
}
