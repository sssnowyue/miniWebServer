#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std::chrono;
class Timestamp {
 public:
  Timestamp();
  explicit Timestamp(uint64_t microSecondsSinceEpoch);
  // Get Current Timestamp(microseconds)
  static Timestamp now();
  // Format Timestamp (microseconds) To YYYY-MM-DD HH:MM:SS
  std::string toString() const;
  ~Timestamp();

 private:
  // Timestamp (microseconds)
  uint64_t microSecondsSinceEpoch_;
};