#include "LicenseFileReader.h"

#include "QTextStream"


LicenseFileReader::LicenseFileReader()
{
}

LicenseFileReader::~LicenseFileReader()
{
}

bool LicenseFileReader::readLicenseFile(QString filePath)
{
	// Open file
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	QTextStream in(&file);
	
	QString currentHeader;
	while (!in.atEnd())
	{
		// Line after line
		QString line = in.readLine();
		// Found a header
		if (line.startsWith("[") && line.endsWith("]"))
		{
			currentHeader = line;
			// Remove the brackets
			currentHeader.chop(1);
			currentHeader.remove(0, 1);
			QHash<QString, QString> values;
			// Next lines should be entries till an empty line follows
			do
			{
				line = in.readLine();
				if (line.contains("\n") || line != "")
				{
					// Split the entry in keyword and value
					QStringList split = line.split("=");
					if (split.length() == 2)
					{
						QString keyword = split[0];
						QString value = split[1];
						// Save all the entries seperated as keyword and value
						values.insert(keyword, value);
					}
				}
			// Exit when its an empty line
			} while (line.contains("\n") || line != "");
			// Insert the entries for the header
			licenseFileData.insert(currentHeader, values);
		}
	}
	return true;
}

QHash<QString, QString> LicenseFileReader::getAllEntriesForHeader(QString header)
{
	// All entries for the specific header
	return licenseFileData.value(header);
}

QString LicenseFileReader::getSpecificEntryValue(QString header, QString keyword)
{
	// First get all the entries for the specific header
	QHash<QString, QString> entries = licenseFileData.value(header);
	// Then get the specific value for the keyword
	return entries.value(keyword);
}
