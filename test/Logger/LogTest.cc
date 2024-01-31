#include "../../src/util/Logger.h"

int main() {
  Logger logger("test_log.txt");

  logger.log(LogLevel::INFO, "This is an informational message.");
  logger.log(LogLevel::WARNING, "This is a warning message.");
  logger.log(LogLevel::ERROR, "This is an error message with error code: %d", 123);

  return 0;
}
