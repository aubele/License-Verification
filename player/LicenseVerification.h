#pragma once

#include "LicenseModel.h"

#include "QObject"


/**
 * Class for the necessary steps to verify a license, contains also methods to obtain data 
 * from the model for the view.
 */
class LicenseVerification : public QObject
{
	Q_OBJECT
public:
	/**
	 * A constructor, also initializes the model.
	 */
	LicenseVerification();
	/**
	 * A destructor.
	 */
	~LicenseVerification();

	/**
	* This method controls the whole verification process and calls all necessary methods
	* to guarantee a valid verification. It checks the amount of license- and signature files,
	* the signature, the mac adress and the expirationdate. This method should always get 
	* called right after creating the LicenseVerification object. Throws LicenseExceptions, if
	* the verification fails.
	* @see checkLicenseFileNumber()
	* @see checkSignatureFileNumber()
	* @see getLicenseFilePathFromDirectory()
	* @see getSignatureFilePathFromDirectory()
	* @see verifySignature()
	* @see readDataFromLicenseFile()
	* @see checkMacAdress()
	* @see checkExpirationDate()
	*/
	void processLicense();
	/**
	* Gets called to show a critical messagebox with a custom help button, which shows a 
	* pdf with informations for the license process.
	* @param title The title for the messagebox.
	* @param errorText The text for the messagebox.
	* @see onLicenseHelp()
    */
	void showErrorMessageBox(QString title, QString errorText);

	// All those methods just return the value from the model.
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
	* Checks how many licensefiles are in the designated 'lic' directory. Gets called from
	* processLicense().
	* @see processLicense()
	* @return If there is exactly one file it is true, else it is false.
	*/
	bool checkLicenseFileNumber();
	/**
	* Checks how many signaturefiles are in the designated 'lic' directory.Gets called from
	* processLicense().
	* @see processLicense()
	* @return If there is exactly one file it is true, else it is false.
	*/
	bool checkSignatureFileNumber();
	/**
	* Checks if the signature in the signaturefile is valid with the CryptoPP library. Gets
	* called from processLicense().
	* @param licensePath The filepath to the licensefile.
	* @param signaturePath the filepath to the signaturefile.
	* @see processLicense()
	* @return True if the signature is valid, else false.
	*/
	bool verifySignature(QString licensePath, const char* signaturePath);

	/**
	* Reads the data from the licensefile and sets everything in the model. Gets called from
	* processLicense().
	* @param licensePath The filepath to the licensefile.
	* @see processLicense()
	*/
	void readDataFromLicenseFile(QString licensePath);
	/**
	* Gets the filepath from the licensefile through the 'lic' directory. Gets called from 
	* processLicense().
	* @see processLicense()
	* @return The filepath from the licensefile.
	*/
	const QString getLicenseFilePathFromDirectory();
	/**
	* Gets the filepath from the signaturefile through the 'lic' directory. Gets called from
	* processLicense().
	* @see processLicense()
	* @return The filepath from the signaturefile.
	*/
	const QString getSignatureFilePathFromDirectory();

	/**
	* Checks if the mac adress mentioned in the license file is valid and available on the pc.
	* Gets called from processLicense.
	* @see processLicense()
	* @return True if the mac adress is valid, else false.
	*/
	bool checkMacAdress();
	/**
	* Checks if the expiration date is already exceeded. Gets called from processLicense.
	* @see processLicense()
	* @return True if the date is not exceeded, else false.
	*/
	bool checkExpirationDate();

	/**
	* Private member for the data from the license.
	*/
	LicenseModel* model;

private slots:
	/**
	* Slot for the help button on the critical messagebox for license errors.
	* @see showErrorMessageBox()
	*/
	void onLicenseHelp();
};
