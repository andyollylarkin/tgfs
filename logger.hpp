#include <string>

class ILogger
{
public:
	virtual void info(std::string message) = 0;
	virtual void error(std::string message) = 0;
	virtual void debug(std::string message) = 0;
	virtual void warn(std::string message) = 0;
};