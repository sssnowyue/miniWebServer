#include "../../src/util/Logger.h"

int main() {
  LOG_INFO("This is an information message");
  LOG_WARN("This is a warning message");
  LOG_ERROR("This is an error message with a variable: %d", 42);

  return 0;
}
