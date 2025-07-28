include(AddCatch2)

add_executable(unit_tests GbaTests.cpp ArmInstrTests.cpp)

target_link_libraries(unit_tests PRIVATE gba Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(unit_tests)