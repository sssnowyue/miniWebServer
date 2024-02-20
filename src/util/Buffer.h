#pragma once
#include <cstddef>
#include <string>
#include <sys/types.h>
#include <vector>
class Buffer {
public:
  explicit Buffer(size_t initialSize = kInitialSize);

  size_t readableLen() const;
  size_t writeableLen() const;
  size_t emptyLen() const;

  // append data with data_len
  void append(const char *data, size_t data_len);
  // retrieve string of len
  std::string retrieveLenStr(size_t len);
  // retrieve all string
  std::string retrieveAllStr();

  // read data from fd to buffer
  ssize_t readFd(int fd, int *saveErrno);
  // write data into fd from buffer
  ssize_t writeFd(int fd, int *saveErrno);

private:
  static const size_t kPrepend =
      0; // space reserved in advance of the buffer to record the data length
  static const size_t kInitialSize = 1024; // initial size of the buffer

  // Basic Data Structures of Buffer
  std::vector<char> buffer_;
  size_t readerIndex_;
  size_t writerIndex_;

  // get the starting address of buffer
  char *begin();
  const char *begin() const;

  // Expand Buffer before appending data with len if len is more than current
  // writeable space
  void makeSpace(size_t len);
};