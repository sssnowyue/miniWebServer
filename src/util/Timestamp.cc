#include "Timestamp.h"

Timestamp::Timestamp() : microSecondsSinceEpoch_(0) {}

Timestamp::Timestamp(uint64_t microSecondsSinceEpoch)
    : microSecondsSinceEpoch_(microSecondsSinceEpoch) {}

Timestamp Timestamp::now() {
  auto now = system_clock::now();
  auto duration = duration_cast<microseconds>(now.time_since_epoch()).count();
  return Timestamp(duration);
}

std::string Timestamp::toString() const {
  auto timePoint =
      time_point<system_clock>(microseconds(microSecondsSinceEpoch_));
  auto time_t = system_clock::to_time_t(timePoint);
  auto localTm = std::localtime(&time_t);

  std::ostringstream oss;
  oss << std::put_time(localTm, "%Y-%m-%d %H:%M:%S");
  return oss.str();
}

Timestamp::~Timestamp() {}