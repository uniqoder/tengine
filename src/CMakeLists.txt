add_library(tengine STATIC
    core/engine.cpp
)

add_library(tengine::tengine ALIAS tengine)

target_include_directories(tengine
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(tengine PUBLIC cxx_std_20)

target_compile_definitions(tengine
  PRIVATE TENGINE_BUILDING_ENGINE
)

tengine_set_warnings(tengine)
