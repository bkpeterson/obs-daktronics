cmake_minimum_required(VERSION 3.28...3.30)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common/bootstrap.cmake" NO_POLICY_SCOPE)

project(${_name} VERSION ${_version})

option(ENABLE_FRONTEND_API "Use obs-frontend-api for UI functionality" OFF)
option(ENABLE_QT "Use Qt functionality" OFF)

include(compilerconfig)
include(defaults)
include(helpers)

add_library(${CMAKE_PROJECT_NAME} MODULE)

#if(WIN32)
#  include_directories("c:/vcpkg/installed/x64-windows/include/pango-1.0" "c:/vcpkg/installed/x64-windows/include/glib-2.0" "c:/vcpkg/installed/x64-windows/lib/glib-2.0/include" "c:/vcpkg/installed/x64-windows/include/harfbuzz" "c:/vcpkg/installed/x64-windows/include/freetype" "c:/vcpkg/installed/x64-windows/include/cairo")
#elseif(OS_MACOS)
#  include_directories("/opt/homebrew/include/pango")
  #include_directories("/usr/local/share/vcpkg/installed/arm64-osx/include/pango-1.0" "/usr/local/share/vcpkg/installed/arm64-osx/include/glib-2.0" "/usr/local/share/vcpkg/installed/arm64-osx/lib/glib-2.0/include" "/usr/local/share/vcpkg/installed/arm64-osx/include/harfbuzz" "/usr/local/share/vcpkg/installed/arm64-osx/include/freetype" "/usr/local/share/vcpkg/installed/arm64-osx/include/cairo")
  #include_directories("/usr/local/share/vcpkg/installed/x64-osx/include/pango-1.0" "/usr/local/share/vcpkg/installed/x64-osx/include/glib-2.0" "/usr/local/share/vcpkg/installed/x64-osx/lib/glib-2.0/include" "/usr/local/share/vcpkg/installed/x64-osx/include/harfbuzz" "/usr/local/share/vcpkg/installed/x64-osx/include/freetype" "/usr/local/share/vcpkg/installed/x64-osx/include/cairo")
  #link_directories("/usr/local/share/vcpkg/installed/arm64-osx/lib")
#else()
#  include_directories("/usr/include/pango-1.0" "/usr/include/glib-2.0" "/usr/lib/x86_64-linux-gnu/glib-2.0/include" "/usr/include/harfbuzz" "/usr/include/freetype" "/usr/include/cairo")
#endif()

find_package(libobs REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OBS::libobs)

#find_path(PANGO_INCLUDE_DIR NAMES pango/pango.h)
#target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "/opt/homebrew/include" "/opt/homebrew/include/pango")
        

find_package(PkgConfig REQUIRED)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PANGO REQUIRED pango)
  pkg_check_modules(CAIRO REQUIRED cairo)
  pkg_check_modules(PANGOCAIRO REQUIRED pangocairo)
  target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PANGO_INCLUDE_DIRS} ${CAIRO_INCLUDE_DIRS} ${PANGOCAIRO_INCLUDE_DIRS})

  find_library(PANGO_LIBRARY NAMES pango-1.0)
  find_library(CAIRO_LIBRARY NAMES cairo)
  find_library(PANGOCAIRO_LIBRARY NAMES pangocairo-1.0)
  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${PANGO_LIBRARY} ${CAIRO_LIBRARY} ${PANGOCAIRO_LIBRARY})

#  link_directories("/opt/homebrew/lib")
#  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${PANGO_LIBRARIES} ${CAIRO_LIBRARIES} ${PANGOCAIRO_LIBRARIES})
#  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE "/opt/homebrew/lib/libpango-1.0.a" "/opt/homebrew/lib/libcairo.a" "/opt/homebrew/lib/libpangocairo-1.0.a" "/opt/homebrew/lib/libharfbuzz.a" "/opt/homebrew/lib/libgobject-2.0.a" "/opt/homebrew/lib/libglib-2.0.a" "/opt/homebrew/lib/libintl.a")
#  target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE "/opt/homebrew/lib/libpango-1.0.a" "/opt/homebrew/lib/libcairo.a" "/opt/homebrew/lib/libpangocairo-1.0.a")
endif()

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