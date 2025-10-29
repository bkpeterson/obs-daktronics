# CMake macOS build dependencies module

include_guard(GLOBAL)

include(buildspec_common)

# _setup_qt_submodule: Build a Qt Submodule
function(_setup_qt_submodule)
  if(OS_WINDOWS)
    set(_cmake_config RelWithDebInfo)
    set(_cmake_arch "")
    set(_cmake_extra "")
  elseif(OS_MACOS)
    set(_cmake_config Release)
    set(_cmake_arch "-DCMAKE_OSX_ARCHITECTURES:STRING='arm64;x86_64'")
    set(
      _cmake_extra
      "-DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET} -DQT_NO_APPLE_SDK_MAX_VERSION_CHECK=ON -DFEATURE_separate_debug_info:BOOL=ON"
    )
  endif()

  message(STATUS "Configure ${label} (${arch})")
  execute_process(
    COMMAND
      "${CMAKE_COMMAND}" -B build_${arch} -G Ninja "${_cmake_arch}"
      "-DCMAKE_INSTALL_PREFIX='${dependencies_dir}/${_qt6_destination}'"
      "-DCMAKE_PREFIX_PATH='${dependencies_dir}/${_qt6_destination}'" "--no-warn-unused-cli"
      "-DBUILD_SHARED_LIBS:BOOL=ON" "-DCMAKE_BUILD_TYPE=${_cmake_config}" "${_cmake_extra}"
    WORKING_DIRECTORY "${dependencies_dir}/${destination}"
    RESULT_VARIABLE _process_result
    COMMAND_ERROR_IS_FATAL ANY
  )
  message(STATUS "Configure ${label} (${arch}) - done")

  message(STATUS "Build ${label} (${_cmake_config} - ${arch})")
  execute_process(
    COMMAND "${CMAKE_COMMAND}" --build build_${arch} --config ${_cmake_config} --parallel
    WORKING_DIRECTORY "${dependencies_dir}/${destination}"
    RESULT_VARIABLE _process_result
    COMMAND_ERROR_IS_FATAL ANY
  )
  message(STATUS "Build ${label} (${_cmake_config} - ${arch}) - done")

  message(STATUS "Install ${label} (${_cmake_config} - ${arch})")
  execute_process(
    COMMAND "${CMAKE_COMMAND}" --install build_${arch} --config ${_cmake_config}
    WORKING_DIRECTORY "${dependencies_dir}/${destination}"
    RESULT_VARIABLE _process_result
    COMMAND_ERROR_IS_FATAL ANY
  )
  message(STATUS "Install ${label} (${_cmake_config} - ${arch}) - done")
endfunction()

# _check_dependencies_macos: Set up macOS slice for _check_dependencies
function(_check_dependencies_macos)
  set(arch universal)
  set(platform macos)

  file(READ "${CMAKE_CURRENT_SOURCE_DIR}/buildspec.json" buildspec)

  set(dependencies_dir "${CMAKE_CURRENT_SOURCE_DIR}/.deps")
  set(prebuilt_filename "macos-deps-VERSION-ARCH_REVISION.tar.xz")
  set(prebuilt_destination "obs-deps-VERSION-ARCH")
  set(qt6_filename "macos-deps-qt6-VERSION-ARCH-REVISION.tar.xz")
  set(qt6_destination "obs-deps-qt6-VERSION-ARCH")
  set(obs-studio_filename "VERSION.tar.gz")
  set(obs-studio_destination "obs-studio-VERSION")
  set(dependencies_list prebuilt qt6 obs-studio)

  _check_dependencies()

  execute_process(
    COMMAND "xattr" -r -d com.apple.quarantine "${dependencies_dir}"
    RESULT_VARIABLE result
    COMMAND_ERROR_IS_FATAL ANY
  )

  list(APPEND CMAKE_FRAMEWORK_PATH "${dependencies_dir}/Frameworks")
  set(CMAKE_FRAMEWORK_PATH ${CMAKE_FRAMEWORK_PATH} PARENT_SCOPE)
endfunction()

_check_dependencies_macos()
