macro(get_project_files sources include_dirs source_ext header_ext)

    # Recursively search for every source file with chosen extension(s).
    file (GLOB_RECURSE ${sources} CONFIGURE_DEPENDS "src/${source_ext}")

    # Recursively search for every header file with chosen extension(s).
    file (GLOB_RECURSE headers CONFIGURE_DEPENDS "include/${PROJECT_NAME}/${header_ext}")

    set(${include_dirs} "") # Set the include_dirs variable to an empty string.
    
    # Get the directories for each header file.
    foreach(_headerFile ${headers})
        get_filename_component(_dir ${_headerFile} PATH)
        list(APPEND ${include_dirs} ${_dir})
    endforeach()
    list(APPEND ${include_dirs} "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/")
    list (REMOVE_DUPLICATES ${include_dirs})
endmacro()