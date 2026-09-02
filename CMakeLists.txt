cmake_minimum_required(VERSION 3.21)

set(VCPKG_OVERLAY_PORTS "")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibSSE/cmake/ports/commonlib-shared/portfile.cmake")
	list(APPEND VCPKG_OVERLAY_PORTS "${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibSSE/cmake/ports")
else()
	message(FATAL_ERROR "commonlib-shared overlay port not found in any CommonLibSSE checkout")
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ports")
	list(APPEND VCPKG_OVERLAY_PORTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ports")
endif()

message(STATUS "vcpkg overlay ports: ${VCPKG_OVERLAY_PORTS}")

# project
project(CombatPathingRevolution VERSION 0.30.1 LANGUAGES CXX)
option(BUILD_SKYRIMAE "Build for Skyrim AE" OFF)

# Set your project version and specify C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(
	SKSE_SUPPORT_XBYAK
)

if(BUILD_SKYRIMAE)
	add_compile_definitions(SKYRIM_SUPPORT_AE)
endif()

# Include SKSEPlugin.cmake from the same directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(SKSEPlugin)

if(MSVC_VERSION GREATER_EQUAL 1936 AND MSVC_IDE) # 17.6+
    file(CONFIGURE OUTPUT "${CMAKE_BINARY_DIR}/Directory.Build.props" CONTENT [==[
    <Project>
      <ItemDefinitionGroup>
        <ClCompile>
          <BuildStlModules>false</BuildStlModules>
        </ClCompile>
      </ItemDefinitionGroup>
    </Project>
    ]==] @ONLY)
endif()