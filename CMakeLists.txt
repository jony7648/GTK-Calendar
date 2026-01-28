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

set(PROJECT_COMPONETS "${PROJECT_DIR}/scenes.cpp")
set(WIDGET_COMPONETS  "${WIDGET_DIR}/widget.cpp" "${WIDGET_DIR}/button_widget.cpp" "${WIDGET_DIR}/container.cpp" "${WIDGET_DIR}/grid_container.cpp" "${WIDGET_DIR}/label.cpp")
set(CORE_COMPONETS "${CORE_DIR}/app.cpp" "${CORE_DIR}/window.cpp" "${CORE_DIR}/space.cpp" "${CORE_DIR}/util.cpp" "${CORE_DIR}/time_componet.cpp" "${CORE_DIR}/scene.cpp")
set(SIGNAL_COMPONETS "${CORE_DIR}/messenger.cpp" "${CORE_DIR}/signal.cpp" "${CORE_DIR}/signaler.cpp")


add_executable(my_app main.cpp ${CORE_COMPONETS} ${SIGNAL_COMPONETS} ${PROJECT_COMPONETS} ${WIDGET_COMPONETS})
target_compile_features(my_app PRIVATE cxx_std_23)
target_link_libraries(my_app PkgConfig::GTK4)

if(WIN32)
    add_definitions(-Duid_t=int -Dgid_t=int)
endif()
