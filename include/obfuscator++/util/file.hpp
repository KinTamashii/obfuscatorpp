#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <wx/wx.h>



namespace file {
    namespace fs = std::filesystem;

    std::string read(fs::path path);

    void write(const std::string &str, fs::path path);

    

    namespace path {
        inline std::string basename(const std::string & path) {
            return path.substr(0, path.rfind('.'));
        }
        inline std::string extension(const std::string & path) {
            size_t pos = path.find('.');
            return path.substr(pos, path.size()-pos);
        }

        inline void mkdir(const std::string & path) {
            if (!wxDirExists(path)) {
                wxMkdir(path);
            }
        }

    };


    inline void write(const std::string &str, const std::string &path, const std::string &file) {
        path::mkdir(path+'/');
        write(str, path+'/'+file);
    }
    
};



