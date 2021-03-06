#include "LicenseExceptions.h"


LicenseException::LicenseException(const char* what_arg) : runtime_error(what_arg)
{
}

const char * LicenseException::what() const throw ()
{
	return "";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LicenseMacAdressException::LicenseMacAdressException(const char* what_arg) : LicenseException(what_arg)
{
}

const char * LicenseMacAdressException::what() const throw ()
{
	//"Ihre Lizenz ist ungueltig, da die in der Lizenz angegebene Hardware-Adresse mit keiner 
	//Ihrer eigenen Hardware-Adressen uebereinstimmt!"
	return "\xdf\xf9\xfb\xfb\xb3\xda\xf2\xa5\xea\xfe\xec\xb1\xe2\xe9\xf9\xdf\xe3\xff\xee\xeb\xf6\xfa\xef\xb6\xe8\xbc\xb6\xf5\xea\xba\xe9\x96\xf3\xb1\xe0\xf0\xb3\xf2\xfe\xad\xaf\xdc\xff\xeb\xee\xf4\xf7\xdf\xf7\xff\xee\xfb\xf4\xf3\xf9\xba\xe1\xf5\xb6\xd9\xea\xe8\xe9\x88\xf7\xe3\xec\xb3\xd2\xf2\xe9\xba\xfc\xe3\xf3\xb1\xe6\xf3\xf9\xdf\xfd\xf4\xe0\xf0\xf6\xe4\xbb\x96\xe7\xe2\xf3\xe3\xab\xff\xe4\x98\xf3\xff\xec\xf0\xb3\xde\xfa\xad\xeb\xe7\xf7\xe3\xee\xb7\xcc\x9b\xe4\xf4\xfa\xed\xf6\xf8\xbb\xaa\xea\xf2\xf3\xe3\xee\xf3\xe3\x8c\xe2\xf8\xe4\xf3\xe7\xb7";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LicenseExpirationDateException::LicenseExpirationDateException(const char* what_arg) : LicenseException(what_arg)
{
}

const char * LicenseExpirationDateException::what() const throw ()
{
	//"Ihre Lizenz ist ungueltig, da das in der Lizenz angegebene Ablaufdatum bereits ueberschritten ist!"
	return "\xdf\xf9\xfb\xfb\xb3\xda\xf2\xa5\xea\xfe\xec\xb1\xe2\xe9\xf9\xdf\xe3\xff\xee\xeb\xf6\xfa\xef\xb6\xe8\xbc\xb6\xf5\xea\xba\xe9\x9e\xe5\xb1\xe0\xf0\xb3\xf2\xfe\xad\xaf\xdc\xff\xeb\xee\xf4\xf7\xdf\xf7\xff\xee\xfb\xf4\xf3\xf9\xba\xe1\xf5\xb6\xd0\xe9\xf6\xec\x8a\xf0\xf5\xe8\xea\xe6\xfb\xbb\xbd\xea\xe2\xf3\xf8\xff\xe9\xad\x8a\xf3\xf3\xec\xec\xe0\xf5\xf3\xad\xe6\xe4\xe2\xf4\xe5\xba\xe4\x8c\xe2\xb0";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LicenseSignatureException::LicenseSignatureException(const char* what_arg) : LicenseException(what_arg)
{
}
	
const char * LicenseSignatureException::what() const throw ()
{
	//"Die Signatur und somit ihre angegebene Lizenz sind ungueltig. Bitte stellen Sie sicher, 
	//dass Sie keine Aenderungen in der Signaturdatei, sowie in der Lizenzdatei durchgefuehrt haben."
	return "\xd2\xf8\xec\xbe\xc0\xff\xfc\xb1\xee\xe4\xe3\xe3\xab\xef\xe3\x9b\xb6\xe2\xe6\xf3\xfa\xe2\xbb\xb6\xe7\xe2\xf3\xb1\xea\xf4\xea\x9a\xf1\xf4\xeb\xfb\xfd\xf3\xbb\x93\xe6\xea\xf3\xff\xf1\xba\xfe\x96\xf8\xf5\xa9\xeb\xfd\xf1\xee\xba\xe3\xe4\xff\xf6\xa5\xba\xcf\x96\xe2\xe5\xec\xbe\xe0\xe2\xfe\xb3\xe3\xf5\xf8\xb1\xd8\xf3\xe8\xdf\xe5\xf8\xea\xf6\xf6\xe4\xb7\xff\xeb\xf1\xe5\xe2\xab\xc9\xe4\x9a\xb6\xfa\xec\xf7\xfd\xf3\xbb\x9e\xea\xfe\xf2\xf4\xf9\xef\xe3\x98\xf3\xff\xa9\xf7\xfd\xb6\xff\xba\xfd\xb0\xc5\xf8\xec\xf4\xec\x8b\xe3\xe3\xed\xff\xe7\xf3\xf2\xf3\xaf\xe3\xf9\xe6\xe2\xff\xad\x96\xf8\xb1\xed\xfb\xe1\xb6\xd7\xb6\xf5\xf5\xf8\xeb\xef\xfb\xf9\x9a\xff\xb1\xed\xeb\xe1\xf5\xf3\xb8\xea\xf6\xe3\xf4\xe3\xe8\xf9\xdf\xfe\xf0\xeb\xfb\xfd\xb8";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LicenseFileNumberException::LicenseFileNumberException(const char* what_arg) : LicenseException(what_arg)
{
}
	
const char * LicenseFileNumberException::what() const throw ()
{
	//"Bitte ueberpruefen Sie, ob sich genau eine Datei mit der Endung '*.lic' in dem 'lic'-Ordner befindet."
	return "\xd4\xf8\xfd\xea\xf6\xb6\xee\xba\xed\xf5\xe4\xe1\xf9\xef\xe8\x99\xf3\xff\xa9\xcd\xfa\xf3\xb7\xff\xe0\xf2\xb6\xe2\xe2\xf9\xe5\xdf\xf1\xf4\xe7\xff\xe6\xb6\xfe\xb6\xe1\xf5\xb6\xd5\xea\xee\xe8\x96\xb6\xfc\xe0\xea\xb3\xf2\xfe\xad\xaf\xd5\xf8\xf5\xfe\xf4\xea\xdf\xb1\xbb\xa7\xf2\xfa\xf5\xbc\xff\xe6\xfe\xb6\xf5\xee\xf7\xad\xd8\xfa\xf8\xea\xb9\xbe\xd9\xe9\xbb\xe1\xf5\xe4\xb1\xe9\xff\xeb\x96\xf8\xf5\xec\xea\xbd";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LicenseSignatureFileNumberException::LicenseSignatureFileNumberException(const char* what_arg) : LicenseException(what_arg)
{
}

const char * LicenseSignatureFileNumberException::what() const throw ()
{
	//"Bitte ueberpruefen Sie, ob sich genau eine Datei mit der Endung '*.sig' in dem 'lic'-Ordner befindet."
	return "\xd4\xf8\xfd\xea\xf6\xb6\xee\xba\xed\xf5\xe4\xe1\xf9\xef\xe8\x99\xf3\xff\xa9\xcd\xfa\xf3\xb7\xff\xe0\xf2\xb6\xe2\xe2\xf9\xe5\xdf\xf1\xf4\xe7\xff\xe6\xb6\xfe\xb6\xe1\xf5\xb6\xd5\xea\xee\xe8\x96\xb6\xfc\xe0\xea\xb3\xf2\xfe\xad\xaf\xd5\xf8\xf5\xfe\xf4\xea\xdf\xb1\xbb\xa7\xed\xfa\xf1\xbc\xff\xe6\xfe\xb6\xf5\xee\xf7\xad\xd8\xfa\xf8\xea\xb9\xbe\xd9\xe9\xbb\xe1\xf5\xe4\xb1\xe9\xff\xeb\x96\xf8\xf5\xec\xea\xbd";
}
