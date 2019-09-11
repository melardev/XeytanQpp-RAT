#pragma once
#include <exception>

class AppException : public std::exception
{
public:
    AppException(const char* what);
	~AppException();
};

