#include "logToFile.h"


logToFile::logToFile()
{
    this->file.open(logFile);

    if (!file.is_open())
    {
        std::cout << "\033[91m" << "[ERROR] Couldn't open the log file." << "\033[0m" << std::endl;
        exit(255);
    }
}

logToFile::~logToFile()
{
    this->file.close();
}

template <typename T>
logToFile& logToFile::operator<<(const T& value)
{
    this->file << value << std::endl;
    return *this;
}