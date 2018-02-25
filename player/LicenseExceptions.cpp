#include "LicenseExceptions.h"


// Just the base class
LicenseException::LicenseException(const char* what_arg) : runtime_error(what_arg)
{
}

const char * LicenseException::what() const throw ()
{
	return "";
}


LicenseMacAdressException::LicenseMacAdressException(const char* what_arg) : LicenseException(what_arg)
{
}

const char * LicenseMacAdressException::what() const throw ()
{
	return "Ihre Lizenz ist ungueltig, da die, in der Lizenz angegebene, Hardware-Adresse "
		"mit keiner Ihrer eigenen Hardware-Adressen uebereinstimmt!\n"
		"\n"
		"Bitte lesen Sie das beilegende Dokument unter 'Help' zur Lizenzierung durch. Diese beinhalten "
		"auch moegliche Fehlerloesungen und Informationen, die nuetzlich sein koennten.\n"
		"\n"
		"Wenn Sie keine passende Loesung fuer das Problem finden koennen, melden Sie "
		"sich bitte beim Support!";
}


LicenseExpirationDateException::LicenseExpirationDateException(const char* what_arg) : LicenseException(what_arg)
{
}

const char * LicenseExpirationDateException::what() const throw ()
{
	return "Ihre Lizenz ist ungueltig, da das, in der Lizenz angegebene, Ablaufdatum bereits "
		"ueberschritten ist!\n"
		"\n"
		"Bitte lesen Sie das beilegende Dokument unter 'Help' zur Lizenzierung durch. Diese beinhalten "
		"auch moegliche Fehlerloesungen und Informationen, die nuetzlich sein koennten.\n"
		"\n"
		"Wenn Sie hierbei keine passende Loesung fuer das Problem finden koennen, melden Sie "
		"sich melden Sie sich bitte beim Support!";
}


LicenseSignatureException::LicenseSignatureException(const char* what_arg) : LicenseException(what_arg)
{
}
	
const char * LicenseSignatureException::what() const throw ()
{
	return	"Die Signatur und dadurch ihre angegebene Lizenz sind ungueltig. Bitte stellen Sie sicher, "
		"dass Sie keine Aenderungen in der Signaturdatei, sowie in der Lizenzdatei durchgefuehrt haben.\n"
		"\n"
		"Bei weiteren Problemen lesen Sie bitte das beilegende Dokument unter 'Help' zur Lizenzierung "
		"durch. Diese beinhalten moegliche Fehlerloesungen und Informationen, die nuetzlich sein koennten.\n"
		"\n"
		"Wenn Sie hierbei keine passende Loesung fuer das Problem finden koennen, melden Sie "
		"sich melden Sie sich bitte beim Support!";
}


LicenseFileNumberException::LicenseFileNumberException(const char* what_arg) : LicenseException(what_arg)
{
}
	
const char * LicenseFileNumberException::what() const throw ()
{
	return	"Bitte ueberpruefen Sie ob sich nur eine Datei mit der Aendung '*.lic' befindet und loeschen "
		"oder verschieben Sie gegebenenfalls die alte Datei aus dem Ordner.\n"
		"\n"
		"Bei weiteren Problemen lesen Sie bitte das beilegende Dokument unter 'Help' zur Lizenzierung "
		"durch. Diese beinhalten moegliche Fehlerloesungen und Informationen, die nuetzlich sein koennten.\n"
		"\n"
		"Wenn Sie hierbei keine passende Loesung fuer das Problem finden koennen, melden Sie "
		"sich melden Sie sich bitte beim Support!";
}


LicenseSignatureFileNumberException::LicenseSignatureFileNumberException(const char* what_arg) : LicenseException(what_arg)
{
}

const char * LicenseSignatureFileNumberException::what() const throw ()
{
	return	"Bitte ueberpruefen Sie ob sich nur eine Datei mit der Aendung '*.dat' befindet und loeschen "
		"oder verschieben Sie gegebenenfalls die alte Datei aus dem Ordner.\n"
		"\n"
		"Bei weiteren Problemen lesen Sie bitte das beilegende Dokument unter 'Help' zur Lizenzierung "
		"durch. Diese beinhalten moegliche Fehlerloesungen und Informationen, die nuetzlich sein koennten.\n"
		"\n"
		"Wenn Sie hierbei keine passende Loesung fuer das Problem finden koennen, melden Sie "
		"sich melden Sie sich bitte beim Support!";
}
