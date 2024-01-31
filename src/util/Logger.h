#pragma once

#include <fstream>
#include <mutex>
#include <string>

#include "Errif.h"
#include "Timestamp.h"

using namespace std;

enum class LogLevel { INFO, WARNING, ERROR };

class Logger {
public:
  explicit Logger(const string &logFileName = "server_log.txt");
  ~Logger();

  template <typename... Args>
  void log(LogLevel level, const char *format, Args... args);

private:
  std::ofstream fileStream_;
  std::mutex mutex_;

  std::string getTimestamp() const;
};


template <typename... Args>
void Logger::log(LogLevel level, const char *format, Args... args) {

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
  }

  char buffer[256];
  snprintf(buffer, sizeof(buffer), format, args...);

  logMessage << buffer << endl;

  if (fileStream_.is_open()) {
    fileStream_ << logMessage.str();
    fileStream_.flush();
  }
}
