cc_library(
  name = "util",
  srcs = ["lib/util.cpp"],
  hdrs = ["lib/util.h"],
)

cc_library(
  name = "InetAddress",
  srcs = ["lib/InetAddress.cpp"],
  hdrs = ["lib/InetAddress.h"],
)

cc_library(
  name = "Socket",
  srcs = ["lib/Socket.cpp"],
  hdrs = ["lib/Socket.h"],
  deps = [
    ":util",
    "InetAddress",
  ]
)

cc_binary(
  name = "main",
  srcs = ["main.cpp"],
  deps = [
    ":util",
    "InetAddress",
    "Socket"
  ],
)