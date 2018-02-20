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

const QStringList& LicenseModel::getFeatures()
{
	return features;
}

const QString LicenseModel::getKeyWordFeature()
{
	return "features";
}

void LicenseModel::setFeatures(const QStringList& features)
{
	this->features = features;
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
