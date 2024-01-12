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
        ":InetAddress",
        ":util",
    ],
)

cc_library(
    name = "Epoll",
    srcs = ["lib/Epoll.cpp"],
    hdrs = ["lib/Epoll.h"],
    deps = [
        ":Channel",
        ":util",
    ],
)

cc_library(
    name = "Channel",
    srcs = ["lib/Channel.cpp"],
    hdrs = ["lib/Channel.h"],
    deps = [
        ":EventLoop"
    ],
)

cc_library(
    name = "Server",
    srcs = ["lib/Server.cpp"],
    hdrs = ["lib/Server.h"],
    deps = [
        ":Channel",
        ":Socket",
    ],
)

cc_library(
    name = "EventLoop",
    srcs = ["lib/EventLoop.cpp"],
    hdrs = ["lib/EventLoop.h"],
    deps = [
        ":Epoll",
        ":Channel"
    ],
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [
        ":EventLoop",
        ":Server",
    ],
)
