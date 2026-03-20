set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.10)
project(TestProject CXX)


find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

include_directories("gtk_componets")
include_directories(".")

set(WIDGET_DIR "gtk_componets")
set(PROJECT_DIR "project")
set(CORE_DIR "core")

set(DEBUG_COMPONETS "${PROJECT_DIR}/debug_scene.cpp")
set(PROJECT_COMPONETS "${PROJECT_DIR}/calendar_scene.cpp" "${PROJECT_DIR}/note_scene.cpp" "${PROJECT_DIR}/persist_data.cpp" "${PROJECT_DIR}/note_container.cpp")
set(WIDGET_COMPONETS  "${WIDGET_DIR}/widget.cpp" "${WIDGET_DIR}/button_widget.cpp" "${WIDGET_DIR}/container.cpp" "${WIDGET_DIR}/grid_container.cpp" "${WIDGET_DIR}/label.cpp" "${WIDGET_DIR}/text_field.cpp" "${WIDGET_DIR}/key_map_label.cpp")
set(CORE_COMPONETS "${CORE_DIR}/app.cpp" "${CORE_DIR}/window.cpp" "${CORE_DIR}/space.cpp" "${CORE_DIR}/util.cpp" "${CORE_DIR}/file_util.cpp" "${CORE_DIR}/sort.cpp" "${CORE_DIR}/time_componet.cpp" "${CORE_DIR}/scene.cpp" "${CORE_DIR}/csv_entry.cpp" "${CORE_DIR}/csv_writer.cpp" "${CORE_DIR}/error.cpp")
set(SIGNAL_COMPONETS "${CORE_DIR}/signal_handler.cpp")


if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    add_definitions(-DNOMINMAX)

    add_compile_options(/TP /GS-) 
endif()

add_executable(Calendar main.cpp ${DEBUG_COMPONETS} ${CORE_COMPONETS} ${SIGNAL_COMPONETS} ${PROJECT_COMPONETS} ${WIDGET_COMPONETS})
target_compile_options(Calendar PRIVATE ${GTK4_CFLAGS})
target_compile_features(Calendar PRIVATE cxx_std_23)
target_link_libraries(Calendar PkgConfig::GTK4)


