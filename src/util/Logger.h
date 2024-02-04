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
  // Write Log
  void log(LogLevel level, const char *msg);

private:
  ofstream fileStream_;
  mutex mutex_;

  string getTimestamp() const;
};

