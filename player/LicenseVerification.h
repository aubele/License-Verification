#pragma once

#include "LicenseModel.h"

#include "QString"
#include "QObject"


class LicenseVerification : public QObject
{
	Q_OBJECT
public:
	LicenseVerification();
	~LicenseVerification();

	void processLicense();
	void showErrorMessageBox(QString title, QString errorText);

	const QString getModelFirstName();
	const QString getModelLastName();
	const QString getModelCustomerNumber();
	const QString getModelCompany();
	const QDate& getModelExpirationDate();

private:
	bool checkLicenseFileNumber();
	bool checkSignatureFileNumber();
	bool verifySignature(QString licensePath, const char* signaturePath);

	void readDataFromLicenseFile(QString licenseFilePath);
	const QString getLicenseFilePath();
	const QString getSignatureFilePath();

	bool checkMacAdress();
	bool checkExpirationDate();

	LicenseModel* model;

	QString licenseFilePath;
	QString signatureFilePath;

private slots:
	void onLicenseHelp();
};
