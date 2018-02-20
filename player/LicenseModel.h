#pragma once

#include "QString"
#include "QStringList"
#include "QDate"



class LicenseModel
{
public:
	LicenseModel();
	~LicenseModel();

	const QString getFirstName();
	const QString getKeyWordFirstName();
	void setFirstName(const QString& firstName);

	const QString getLastName();
	const QString getKeyWordLastName();
	void setLastName(const QString& lastName);

	const QString getEmail();
	const QString getKeyWordEmail();
	void setEmail(const QString& email);

	const QString getCompany();
	const QString getKeyWordCompany();
	void setCompany(const QString& company);

	const QString getMac();
	const QString getKeyWordMac();
	void setMac(const QString& mac);

	const QStringList& getFeatures();
	const QString getKeyWordFeature();
	void setFeatures(const QStringList& firstName);

	int getDuration();
	const QString getKeyWordDuration();
	void setDuration(int duration);

	const QDate& getExpirationDate();
	const QString getKeyWordExpirationDate();
	void setExpirationDate(const QDate& expirationDate);

	const QString getCustomerNumber();
	const QString getKeyWordCustomerNumber();
	void setCustomerNumber(const QString& customerNumber);

private:
	QString firstName;
	QString lastName;
	QString email;
	QString mac;
	QString company;
	QStringList features;
	int duration;
	QDate expirationDate;
	QString customerNumber;

};