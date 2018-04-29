#pragma once

#include "LicenseModel.h"
#include "LicenseFileReader.h"

#include "QObject"
#include "QMessageBox"


std::string decode(const std::string& input);
bool int2DCheck();
bool CheckNtQueryInformationProcess();

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
	* is valid.
	* @see getLicenseFilePathFromDirectory()
	* @see getSignatureFilePathFromDirectory()
	* @return True if the signature is valid, else false.
	*/
	bool verifySignatureObfus();
	/**
	* Same like verifySignatureObfus(), but with a boolean reference so the verification process can
	* get canceled without throwing an exception. Only gets called in processLicense.
	* @param cancel Shows if the verification process failed and no exception should be triggered.
	* @see verifySignatureObfus()
	* @return True if the signature is valid, else false.
	*/
	bool verifySignatureObfusOnProcessObfus(bool& cancel);
	/**
	* Gets the license data from the license file. Only gets called in the verification process
	* from verifySignatureObfus() or verifySignatureObfusOnProcessObfus().
	* @return The license data.
	*/
	std::string verifySignatureGetLicenseDataObfus();
	/**
	* Gets the signature from the signature file. Only gets called in the verification process
	* from verifySignatureObfus() or verifySignatureObfusOnProcessObfus().
	* @return The signature.
	*/
	std::string verifySignatureGetSignatureObfus();

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

	/**
	*	Checks if a debugger is present, with a trapflag.
	* @return True if a debugger is present, else false.
	*/
	bool checkDebuggerWithTrapFlag();

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