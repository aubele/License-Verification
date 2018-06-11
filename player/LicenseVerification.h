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
	/**
	* Destructor.
	*/
	~LicenseVerification();

	/**
	* This method controls the whole verification process and calls all necessary methods
	* to guarantee a valid verification. It checks the amount of license- and signature files,
	* the validation of the signature, the mac adress and the expirationdate.
	* This method should always get called right after creating the LicenseVerification object.
	* Throws LicenseExceptions, if the verification fails.
	* @see checkLicenseFileNumberObfus()
	* @see checkSignatureFileNumberObfus()
	* @see getLicenseFilePathFromDirectoryObfus()
	* @see verifySignatureOnProcessObfus()
	* @see readDataIntoModelObfus()
	* @see checkMacAdressObfus()
	* @see checkExpirationDateObfus()
	* @see toggleNoLicenseObfus()
	*/
	bool processLicense();
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

	/**
	* Checks, with the CryptoPP library, if the signature in the signaturefile for the licensefile
	* is valid. This method gets called on multiple locations in this software to assure that the 
	* main signature verification was not patched.
	* @see getLicenseFilePathFromDirectory()
	* @see getSignatureFilePathFromDirectory()
	* @return True if the signature is valid, else false.
	*/
	bool verifySignatureObfus();
	/**
	* Same like verifySignatureObfus(), but with a boolean reference so the verification process can
	* get canceled without throwing an exception. Only gets called in processLicense.
	* @param cancel Shows if the verification process failed and no exception should be triggered.
	* @see getLicenseFilePathFromDirectory()
	* @see getSignatureFilePathFromDirectory()
	* @return True if the signature is valid, else false.
	*/
	bool verifySignatureOnProcessObfus(bool& cancel);

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
	int checkLicenseFileNumberObfus();
	/**
	* Checks how many signaturefiles are in the designated 'lic' directory.
	* @return The amount of signaturefiles in the 'lic' direcotry.
	*/
	int checkSignatureFileNumberObfus();

	/**
	* Reads the data from the licensefile through the LicenseFileReader and sets everything
	* in the model.
	* @param licensePath The filepath to the licensefile.
	*/
	void readDataIntoModelObfus();
	/**
	* Gets the filepath from the licensefile through the 'lic' directory.
	* @return The filepath from the licensefile.
	*/
	const QString getLicenseFilePathFromDirectoryObfus();
	/**
	* Gets the filepath from the signaturefile through the 'lic' directory.
	* @return The filepath from the signaturefile.
	*/
	const QString getSignatureFilePathFromDirectoryObfus();

	/**
	* Checks if the mac adress mentioned in the license file is valid and available on the pc.
	* @return True if the mac adress is valid, else false.
	*/
	bool checkMacAdressObfus();
	/**
	* Checks if the expiration date is already exceeded.
	* @return True if the date is not exceeded, else false.
	*/
	bool checkExpirationDateObfus();

	/**
	* Triggers a warning, that no license is activated and sets the boolean isLicensingActive on false.
	* @see showMessageBox()
	*/
	void toggleNoLicenseObfus();

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
	* the view changes, if there is a licensefile.
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