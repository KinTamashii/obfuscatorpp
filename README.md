
# Obfuscator++

![Example](examples/Screen%20Shot%202022-11-24%20at%203.12.43%20AM.png)
A C++ rewrite of the PythonTextObfuscator.

It translates texts through multiple languages with Google Translate, resulting in a poor translation of the original text.

  

# Dependencies

    cmake

    wxWidgets

    libcurl

    CURLpp

    gumbo

# Setup
After installing the dependencies, run cmake and build with the following options:

    -DLIBRARIES_PATH=${PATH_TO_YOUR_LIBRARIES}

    -DCURPP_PATH=${PATH_TO_CURLPP}

# Features
Opening text and loading into the input box.

Saving the contents of the output box to a file.

Saving the contents of both the input and output boxes into one directory.

Preforming an obfuscation on multiple files, or all files in multiple directories through a file dialog.

Generate a comparison spreadsheet with multiple obfuscations.

Preforming the Generate action on files/directories.

Swap Button which puts the output text into the input box and immediately obfuscates.

Accumulate Button which is similar to the Swap Button but appends the text to the end of the input text rather than replacing it.

Option to choose whether to include non-letter characters in the translation requests (which typically cause the result to balloon dramatically in size).

Choice of what languages to use for the obfuscation.
