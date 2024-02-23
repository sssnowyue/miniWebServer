#include "Buffer.h"
#include "Logger.h"
#include <algorithm>
#include <string>
#include <sys/uio.h>
#include <unistd.h>
Buffer::Buffer(size_t initialSize)
    : buffer_(kPrepend + kInitialSize), readerIndex_(kPrepend),
      writerIndex_(kPrepend) {}

char *Buffer::begin() { return &*buffer_.begin(); }
const char *Buffer::begin() const { return &*buffer_.begin(); }

size_t Buffer::readableLen() const { return writerIndex_ - readerIndex_; }
size_t Buffer::writeableLen() const { return buffer_.size() - writerIndex_; }
size_t Buffer::emptyLen() const { return readerIndex_; }

void Buffer::append(const char *data, size_t data_len) {
  if (writeableLen() < data_len) {
    makeSpace(data_len);
  }
  std::copy(data, data + data_len, begin() + writerIndex_);
  writerIndex_ += data_len;
}

void Buffer::makeSpace(size_t len) {
  if (emptyLen() + writeableLen() >= len + kPrepend) {
    size_t readableCurLen = readableLen();
    std::copy(begin() + readerIndex_, begin() + writerIndex_,
              begin() + kPrepend);
    readerIndex_ = kPrepend;
    writerIndex_ = readerIndex_ + readableCurLen;
  } else {
    buffer_.resize(writerIndex_ + len);
  }
}

std::string Buffer::retrieveLenStr(size_t len) {
  std::string result;
  if (len < readableLen()) {
    result.assign(begin() + readerIndex_, len);
    readerIndex_ += len;
  } else {
    result.assign(begin() + readerIndex_, readableLen());
    readerIndex_ = kPrepend;
    writerIndex_ = kPrepend;
  }
  return result;
}

std::string Buffer::retrieveAllStr() { return retrieveLenStr(readableLen()); }

ssize_t Buffer::readFd(int fd, int *saveErrno) {
  char extrabuf[65536] = {0}; // 64KB
  struct iovec vec[2];
  const size_t writableLength = writeableLen();
  vec[0].iov_base = begin() + writerIndex_;
  vec[0].iov_len = writableLength;
  vec[1].iov_base = extrabuf;
  vec[1].iov_len = sizeof(extrabuf);

  const int iovcnt = (writableLength < sizeof(extrabuf)) ? 2 : 1;
  const ssize_t n = ::readv(fd, vec, iovcnt);

  if (n < 0) {
    *saveErrno = errno;
  } else {
    if (static_cast<size_t>(n) <= writableLength) {
      writerIndex_ += n;
    } else {
      writerIndex_ = buffer_.size();
      append(extrabuf, n - writableLength);
    }
  }

  return n;
}

ssize_t Buffer::writeFd(int fd, int *saveErrno) {
  ssize_t n = ::write(fd, begin() + readerIndex_, readableLen());
  if (n < 0) {
    *saveErrno = errno;
  } else {
    readerIndex_ += static_cast<size_t>(n);
  }
  return n;
}