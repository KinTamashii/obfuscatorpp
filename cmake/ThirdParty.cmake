# import a third party library
function(import_third_party LIB_NAME REPO)
    find_package(${LIB_NAME} QUIET)
    if(${LIB_NAME}_FOUND)
        message(STATUS "Found preinstalled ${LIB_NAME} libraries at ${${LIB_NAME}_LIBRARIES}")
        add_library(${LIB_NAME}_external INTERFACE)
    else() # If the library is not present, install it.
        message(STATUS "Preinstalled ${LIB_NAME} not found.")
        message(STATUS "Will download and install ${LIB_NAME} in ${STAGED_INSTALL_PREFIX}")

        include(ExternalProject)
        ExternalProject_Add(${LIB_NAME}_external
            GIT_REPOSITORY
            ${REPO}
            GIT_TAG
            master
            UPDATE_COMMAND
            ""
            CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
            -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
            -DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
            -DwxBUILD_SHARED=OFF
            CMAKE_CACHE_ARGS
            -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
            TEST_AFTER_INSTALL
            0
            DOWNLOAD_NO_PROGRESS
            1
            LOG_CONFIGURE
            1
            LOG_BUILD
            1
            LOG_INSTALL
            1
        )
        set(${LIB_NAME}_ROOT_DIR ${STAGED_INSTALL_PREFIX})
        file(TO_NATIVE_PATH "${${LIB_NAME}_ROOT_DIR}" OpenCV_ROOT_DIR)
        set(${LIB_NAME}_ROOT_DIR ${${LIB_NAME}_ROOT_DIR} CACHE INTERNAL "${LIB_NAME} installation dir")
    endif()


endfunction()
