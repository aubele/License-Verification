#pragma once
#include <stdexcept>
	
using namespace std;


/**
* Base exception for all exceptions, which can get thrown in the verification process.
* Explanation for the methods is the same for all Exceptions.
*/
struct LicenseException : public runtime_error
{
	/**
	* A Constructor.
	* @param what_arg Custom description, which plays no role, because all exception have default descriptions.
	*/
	LicenseException(const char* what_arg);
	/**
	* Member for the the default description
	* @return The default description
	*/
	const char * what() const throw ();
};

/**
* Exception for a invalid mac adress, which is not available on the current hardware.
*/
struct LicenseMacAdressException : public LicenseException
{
	LicenseMacAdressException(const char* what_arg);
	const char * what() const throw ();
};

/**
* Exception for a invalid expiration date, which is exceeded.
*/
struct LicenseExpirationDateException : public LicenseException
{
	LicenseExpirationDateException(const char* what_arg);
	const char * what() const throw ();
};

/**
* Exception for a invalid signature.
*/
struct LicenseSignatureException : public LicenseException
{
	LicenseSignatureException(const char* what_arg);
	const char * what() const throw ();
};

/**
* Exception if there are more then one licensefiles in the 'lic' directory.
*/
struct LicenseFileNumberException : public LicenseException
{
	LicenseFileNumberException(const char* what_arg);
	const char * what() const throw ();
};

/**
* Exception if there are more then one signaturefiles in the 'lic' directory.
*/
struct LicenseSignatureFileNumberException : public LicenseException
{
	LicenseSignatureFileNumberException(const char* what_arg);
	const char * what() const throw ();
};
