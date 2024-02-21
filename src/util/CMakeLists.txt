add_library(util SHARED
    Buffer.cc
    Errif.cc
    Logger.cc
    Timestamp.cc
)

target_include_directories(util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
