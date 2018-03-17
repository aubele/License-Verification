#pragma once

#include "LicenseModel.h"
#include "LicenseFileReader.h"

#include "QObject"
#include "QMessageBox"


/**
 * Class for the necessary steps to verify a license, contains also methods to obtain data 
 * from the model for the view.
 */
class LicenseVerification : public QObject
{
	Q_OBJECT
public:
	/**
	 * Constructor, also initializes the model, the license file reader and a boolean to determine
	 * if licensing is active.
	 */
	LicenseVerification();
	~LicenseVerification();

	/**
	* This method controls the whole verification process and calls all necessary methods
	* to guarantee a valid verification. It checks the amount of license- and signature files,
	* the validation of the signature, the mac adress and the expirationdate. 
	* This method should always get called right after creating the LicenseVerification object. 
	* Throws LicenseExceptions, if the verification fails.
	* @see checkLicenseFileNumber()
	* @see checkSignatureFileNumber()
	* @see getLicenseFilePathFromDirectory()
	* @see verifySignature()
	* @see readDataIntoModel()
	* @see checkMacAdress()
	* @see checkExpirationDate()
	* @see toggleNoLicense()
	*/
	void processLicense();
	/**
	* Gets called to show a messagebox with a custom help button, which shows a 
	* pdf with informations for the license process.
	* @param title The title for the messagebox.
	* @param errorText The text for the messagebox.
	* @param iconEnum The Enum to determine the right icon. Defaul is a critical (error) icon.
	* @see onLicenseHelp()
    */
	void showMessageBox(QString title, QString errorText, QMessageBox::Icon iconEnum = QMessageBox::Critical);

	/**
	* Get the private isLicensingActive-member to determine if licensing is active.
	* @return The isLicensingActive-member
	*/
	bool getIsLicensingActive();

	// All those methods just return values from the model
	const QString getModelFirstName();
	const QString getModelLastName();
	const QString getModelCustomerNumber();
	const QString getModelCompany();
	const QDate& getModelExpirationDate();
	bool getModelFeatureFullScreen();
	bool getModelFeatureSpeed();
	bool getModelFeatureColor();
	bool getModelFeatureHistogram();

private:
	/**
	* Checks how many licensefiles are in the designated 'lic' directory.
	* @return The amount of licensefiles in the 'lic' direcotry.
	*/
	int checkLicenseFileNumber();
	/**
	* Checks how many signaturefiles are in the designated 'lic' directory.
	* @return The amount of signaturefiles in the 'lic' direcotry.
	*/
	int checkSignatureFileNumber();
	/**
	* Checks, with the CryptoPP library, if the signature in the signaturefile for the licensefile 
	* is valid.
	* @param licensePath The filepath to the licensefile.
	* @see getSignatureFilePathFromDirectory()
	* @return True if the signature is valid, else false.
	*/
	bool verifySignature(QString licensePath);

	/**
	* Reads the data from the licensefile through the LicenseFileReader and sets everything
	* in the model.
	* @param licensePath The filepath to the licensefile.
	*/
	void readDataIntoModel(QString licensePath);
	/**
	* Gets the filepath from the licensefile through the 'lic' directory.
	* @return The filepath from the licensefile.
	*/
	const QString getLicenseFilePathFromDirectory();
	/**
	* Gets the filepath from the signaturefile through the 'lic' directory.
	* @return The filepath from the signaturefile.
	*/
	const QString getSignatureFilePathFromDirectory();

	/**
	* Checks if the mac adress mentioned in the license file is valid and available on the pc.
	* @return True if the mac adress is valid, else false.
	*/
	bool checkMacAdress();
	/**
	* Checks if the expiration date is already exceeded.
	* @return True if the date is not exceeded, else false.
	*/
	bool checkExpirationDate();

	/**
	* Triggers a warning, that no license is activated and sets the boolean isLicensingActive on false.
	* @see showMessageBox()
	*/
	void toggleNoLicense();

	/**
	* Private member for the data from the license.
	*/
	LicenseModel* model;
	/**
	* Private member to read the licensefile.
	*/
	LicenseFileReader* reader;
	/**
	* Private member to determine if the licensing is active. This is mainly important since
	* the view gets changed, if there is a licensefile.
	* Accessible from outside through getIsLicensingActive().
	* @see getIsLicensingActive()
	*/
	bool isLicensingActive;

private slots:
	/**
	* Slot for the help button on the messagebox for license errors.
	*/
	void onLicenseHelp();
};
