#include "../../src/util/Logger.h"

int main() {
  Logger logger("test_log.txt");

  logger.log(LogLevel::INFO, "This is an informational message.");
  logger.log(LogLevel::WARNING, "This is a warning message.");

  return 0;
}
