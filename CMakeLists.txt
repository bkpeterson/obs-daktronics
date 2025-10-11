cmake_minimum_required(VERSION 3.28...3.30)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

#if(WIN32)
#  set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
#endif()

project(${_name} VERSION ${_version})

option(ENABLE_FRONTEND_API "Use obs-frontend-api for UI functionality" OFF)
option(ENABLE_QT "Use Qt functionality" OFF)

include(compilerconfig)
include(defaults)
include(helpers)

add_library(${CMAKE_PROJECT_NAME} MODULE)

find_package(libobs REQUIRED CONFIG)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::libobs)

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PANGO pango)
  pkg_check_modules(CAIRO cairo)
  pkg_check_modules(PANGOCAIRO pangocairo)
endif()

if(WIN32)
  include_directories("c:/vcpkg/installed/x64-windows/include/pango-1.0" "c:/vcpkg/installed/x64-windows/include/glib-2.0" "c:/vcpkg/installed/x64-windows/include/harfbuzz" "c:/vcpkg/installed/x64-windows/include/freetype" "c:/vcpkg/installed/x64-windows/include/cairo")
  find_package(GLib2 REQUIRED)
endif()

include_directories(${PANGO_INCLUDE_DIRS} ${CAIRO_INCLUDE_DIRS} ${PANGOCAIRO_INCLUDE_DIRS})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${PANGO_LIBRARIES} ${CAIRO_LIBRARIES} ${PANGOCAIRO_LIBRARIES})

if(ENABLE_FRONTEND_API)
  find_package(obs-frontend-api REQUIRED)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::obs-frontend-api)
endif()

if(ENABLE_QT)
  find_package(Qt6 COMPONENTS Widgets Core)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Core Qt6::Widgets)
  target_compile_options(
    ${CMAKE_PROJECT_NAME}
    PRIVATE $<$<C_COMPILER_ID:Clang,AppleClang>:-Wno-quoted-include-in-framework-header -Wno-comma>
  )
  set_target_properties(
    ${CMAKE_PROJECT_NAME}
    PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON
  )
endif()

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE src)
target_sources(
  ${CMAKE_PROJECT_NAME}
  PRIVATE
    src/plugin-main.cpp
    src/dak-data-utils.cpp
    src/dak-sport-data.cpp
    src/daktronics-filter.cpp
    src/daktronics-source.cpp
    src/dak-source-support.c
)

set_target_properties_plugin(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${_name})
