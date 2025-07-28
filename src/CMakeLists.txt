add_library(gba Gba.cpp GbaHelpers.cpp ArmInstr.cpp)
target_include_directories(gba PUBLIC "${CMAKE_SOURCE_DIR}/include")