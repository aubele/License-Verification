#pragma once

#include "QString"
#include "QDate"


/**
* The model for the license-verification and license-generation, contains all
* nessecary data-fields for the license. There is for each field a getKeyWord*()-method 
* to receive a key for filestorage, so it is recognizable in the reading-process.
*/
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

	bool getFeatureFullScreen();
	void setFeatureFullScreen(bool fullScreen);
	const QString getKeyWordFullScreen();

	bool getFeatureSpeed();
	void setFeatureSpeed(bool speed);
	const QString getKeyWordSpeed();

	bool getFeatureColor();
	void setFeatureColor(bool color);
	const QString getKeyWordColor();

	bool getFeatureHistogram();
	void setFeatureHistogram(bool histogram);
	const QString getKeyWordHistogram();

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
	bool featureFullScreen;
	bool featureSpeed;
	bool featureColor;
	bool featureHistogram;
	int duration;
	QDate expirationDate;
	QString customerNumber;
};
