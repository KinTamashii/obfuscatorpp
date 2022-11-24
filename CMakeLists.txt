cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(obfuscator++ LANGUAGES CXX)

include(cmake/getProjectFiles.cmake)
include(cmake/ThirdParty.cmake)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++2a")
set(CMAKE_CXX_FLAGS_DEBUG  "-g")
message(${PROJECT_NAME})

option(CURLPP_PATH "CURLPP_PATH" /opt/homebrew/Cellar/curlpp/0.8.1_1)

get_project_files(OBFUSCATOR_PP_SOURCES OBFUSCATOR_PP_INCLUDE_DIRS "*.cpp" "*.hpp")

message(${OBFUSCATOR_PP_INCLUDE_DIRS})


find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(CURLpp REQUIRED)


include_directories(${OBFUSCATOR_PP_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} ${CURLPP_PATH}/include/ ${wxWidgets_INCLUDE_DIRS})


function(resource_macosx VAR SOURCE_PATH DESTINATION PATTERN)
    file(GLOB_RECURSE _LIST CONFIGURE_DEPENDS ${SOURCE_PATH}/${PATTERN})
    foreach (RESOURCE ${_LIST})
        get_filename_component(_PARENT ${RESOURCE} DIRECTORY)
        if (${_PARENT} STREQUAL ${SOURCE_PATH})
            set(_DESTINATION ${DESTINATION})
        else ()
            file(RELATIVE_PATH _DESTINATION ${SOURCE_PATH} ${_PARENT})
            set(_DESTINATION ${DESTINATION}/${_DESTINATION})
        endif ()
        set_property(SOURCE ${RESOURCE} PROPERTY MACOSX_PACKAGE_LOCATION ${_DESTINATION})
    endforeach (RESOURCE)
    set(${VAR} ${_LIST} PARENT_SCOPE)
endfunction()

if (APPLE)
    resource_macosx(APP_SRC ${PROJECT_SOURCE_DIR}/resources/macosx Resources *)
    add_executable("obfuscator++" MACOSX_BUNDLE ${OBFUSCATOR_PP_SOURCES} ${APP_SRC})
    set_target_properties("obfuscator++" PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/resources/Info.plist OUTPUT_NAME "Obfuscator++")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    enable_language("RC")
    set (WIN32_RESOURCES ${PROJECT_SOURCE_DIR}/resources/windows/Obfuscator++.rc)
    add_executable("obfuscator++" MACOSX_BUNDLE ${OBFUSCATOR_PP_SOURCES} ${WIN32_RESOURCESC} OUTPUT_NAME "Obfuscator++")
else()


endif()

target_link_libraries("obfuscator++"  ${CURLPP_LDFLAGS} ${CURL_LIBRARIES} -lcurlpp -lcurl -L${CURLPP_PATH}/lib/ ${wxWidgets_LIBRARIES})


