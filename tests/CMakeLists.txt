add_executable(tengine_tests
    core/test_smoke.cpp
)

target_link_libraries(tengine_tests
    PRIVATE
        tengine::tengine
        Catch2::Catch2WithMain
)

tengine_set_warnings(tengine_tests)

list(APPEND CMAKE_MODULE_PATH
    ${Catch2_SOURCE_DIR}/extras
)

include(Catch)

catch_discover_tests(tengine_tests)
