#include "Logger.h"

Logger::Logger(const string &logFileName) {
  fileStream_.open(logFileName, ios::out | ios::app);
  errif(!fileStream_.is_open(), "Error opening log file");
}

void Logger::log(LogLevel level, const char *msg) {

  lock_guard<mutex> lock(mutex_);

  ostringstream logMessage;
  logMessage << getTimestamp() << " ";

  switch (level) {
  case LogLevel::INFO:
    logMessage << "[INFO] ";
    break;
  case LogLevel::WARNING:
    logMessage << "[WARNING] ";
    break;
  case LogLevel::ERROR:
    logMessage << "[ERROR] ";
    break;
  default:
    break;
  }

  char buffer[256];
  snprintf(buffer, sizeof(buffer), "%s", msg);

  logMessage << buffer << endl;

  if (fileStream_.is_open()) {
    fileStream_ << logMessage.str();
    fileStream_.flush();
  }
}

string Logger::getTimestamp() const { return Timestamp::now().toString(); }

Logger::~Logger() {
  if (fileStream_.is_open()) {
    fileStream_.close();
  }
}
