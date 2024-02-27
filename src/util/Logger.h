#pragma once

#include <cstdarg>
#include <fstream>
#include <mutex>
#include <string>

#include "Errif.h"
#include "Timestamp.h"

using namespace std;

enum class LogLevel { INFO, WARNING, ERROR };

class Logger {
 public:
  explicit Logger(const string& logFileName = "server_log.txt");
  ~Logger();
  // Write Log
  void log(LogLevel level, const char* msg, ...);

 private:
  ofstream fileStream_;
  mutex mutex_;

  string getTimestamp() const;
};

#define LOG_INFO(msg, ...) Logger().log(LogLevel::INFO, msg, ##__VA_ARGS__)
#define LOG_WARN(msg, ...) Logger().log(LogLevel::WARNING, msg, ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) Logger().log(LogLevel::ERROR, msg, ##__VA_ARGS__)