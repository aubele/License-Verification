#pragma once
#include <stdexcept>
	
using namespace std;


// Base exception
struct LicenseException : public runtime_error
{
	LicenseException(const char* what_arg);
	const char * what() const throw ();
};

struct LicenseMacAdressException : public LicenseException
{
	LicenseMacAdressException(const char* what_arg);
	const char * what() const throw ();
};

struct LicenseExpirationDateException : public LicenseException
{
	LicenseExpirationDateException(const char* what_arg);
	const char * what() const throw ();
};

struct LicenseSignatureException : public LicenseException
{
	LicenseSignatureException(const char* what_arg);
	const char * what() const throw ();
};

struct LicenseFileNumberException : public LicenseException
{
	LicenseFileNumberException(const char* what_arg);
	const char * what() const throw ();
};

struct LicenseSignatureFileNumberException : public LicenseException
{
	LicenseSignatureFileNumberException(const char* what_arg);
	const char * what() const throw ();
};
