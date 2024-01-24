#include "../../src/util/Timestamp.h"
#include <iostream>

int main() {
  // Create a Timestamp object for the current time
  Timestamp currentTime = Timestamp::now();

  // Print the current time using toString() method
  std::cout << "Current Time: " << currentTime.toString() << std::endl;

  // Create a Timestamp object with a specific timestamp (for testing)
  Timestamp customTime(1706130525615000); // Replace with your desired timestamp

  // Print the custom time using toString() method
  std::cout << "Past Time: " << customTime.toString() << std::endl;

  return 0;
}
