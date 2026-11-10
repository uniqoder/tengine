add_executable(tengine_example_smoke
    smoke/main.cpp
)

target_link_libraries(tengine_example_smoke
    PRIVATE tengine::tengine
)

tengine_set_warnings(tengine_example_smoke)
