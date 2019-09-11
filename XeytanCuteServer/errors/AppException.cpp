#include "AppException.h"


AppException::AppException(const char* message): exception(message)
{
}

AppException::~AppException()
{
}
