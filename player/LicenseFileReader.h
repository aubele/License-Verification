#pragma once

#include "QString"
#include "QList"
#include "QHash"


/**
* Class to read a license file and extract the data. After creating an object,
* we first need to read a license file with readLicenseFile() and then grab the data 
* with the methods getAllEntriesForHeader() and getSpecificEntryValue().
*/
class LicenseFileReader 
{
public:
	LicenseFileReader();
	~LicenseFileReader();

	/**
	* Read the data from the license file and saves it in licenseFileData.
	*
	* Basic structure of the file:
	* [Header]
	* entry (keyword=value)
	* entry (keyword=value)
	* entry (keyword=value)
	*
	* The header are stored as keys and the entries, consisting of keyword and value, as value 
	* in a QHash.
	* @param filePath The path to the licensefile.
	* @return True if the file exists, false if not
	*/
	bool readLicenseFile(QString filePath);
	
	/**
	* Extracts all the entries for one specific header.
	* @param header The name of the header.
	* @return All the entries for the header.
	*/
	QHash<QString, QString> getAllEntriesForHeader(QString header);
	/**
	* Get a specific value for a headername and a keyword.
	* @param header The name of the header.
	* @param keyword The name of the keyword for the value.
	* @return The specific value.
	*/
	QString getSpecificEntryValue(QString header, QString keyword);

private:
	/**
	* The data structure for the license file.
	*/
	QHash<QString, QHash<QString, QString>> licenseFileData;
};
