add_executable(tengine_tests
    core/test_smoke.cpp
    math/test_aabb.cpp
    math/test_intersections.cpp
    math/test_matrix3.cpp
    math/test_matrix4.cpp
    math/test_plane.cpp
    math/test_quaternion.cpp
    math/test_ray.cpp
    math/test_scalar.cpp
    math/test_sphere.cpp
    math/test_transform.cpp
    math/test_vector2.cpp
    math/test_vector3.cpp
    math/test_vector4.cpp
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
