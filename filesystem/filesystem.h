#pragma once

#include <Windows.h>

#include <filesystem>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "../console/console.h"
#include "../util/util.h"

namespace a9 {
    namespace filesystem {
        enum class copy_option {
            // Copy the source content
            copy,

            // Rename the source content
            // Source content will be removed
            rename
        };

        namespace _util {
            std::string wstring2string(const std::wstring& wstr) {
                const int buffer_size = WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str(), -1, (char*)NULL, 0, NULL, NULL);
                char* buffer          = new char[buffer_size];

                WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str(), -1, buffer, buffer_size, NULL, NULL);

                const std::string str(buffer, buffer + buffer_size - 1);

                delete[] buffer;

                return str;
            }

            std::string safe_path2string(const std::filesystem::path& _path) {
                try {
                    return _path.string();
                } catch (...) {
                    return wstring2string(_path.wstring());
                }
            }
        }

        std::string get_extension(const std::string& _filepath) {
            const std::filesystem::path path = _filepath;
            if (auto ext = path.extension().string(); ext.size() > 0) {
                return ext.erase(0, 1);
            }
            return path.extension().string();
        }

        // Get the file name includes extension
        std::string get_filename(const std::string& _filepath) {
            std::filesystem::path path = _filepath;
            return path.filename().string();
        }

        std::string get_parent_directory(const std::string& _path) {
            const std::filesystem::path path = _path;
            return path.parent_path().string();
        }

        std::vector<std::string> get_directory_contents(const std::string& _directory_path, bool _recursive) {
            std::vector<std::string> contents;
            if (_recursive) {
                for (const auto& file : std::filesystem::recursive_directory_iterator(_directory_path)) {
                    contents.push_back(_util::safe_path2string(file.path()));
                }
            } else {
                for (const auto& file : std::filesystem::directory_iterator(_directory_path)) {
                    contents.push_back(file.path().string());
                }
            }
            return contents;
        }

        bool is_directory(const std::string& _filepath) {
            return std::filesystem::is_directory(_filepath);
        }

        bool is_image(const std::string& _filepath) {
            if (is_directory(_filepath)) {
                return false;
            }

            const std::vector<std::string> image_extensions{"jpg", "png", "tiff", "bmp", "gif"};
            const auto extension = get_extension(_filepath);
            return std::any_of(image_extensions.begin(), image_extensions.end(), [extension](const std::string& ext) {
                return ext == extension;
            });
        }

        bool create_directory(const std::string& _path) {
            const auto path = is_directory(_path) ? _path : get_parent_directory(_path);
            return std::filesystem::create_directory(path);
        }

        bool create_directories(const std::string& _path) {
            const auto path = is_directory(_path) ? _path : get_parent_directory(_path);
            return std::filesystem::create_directories(path);
        }

        bool copy(const std::string& _from, const std::string& _to, copy_option _option) {
            try {
                create_directories(_to);
                switch (_option) {
                case a9::filesystem::copy_option::copy:
                    std::filesystem::copy(_from, _to);
                    break;
                case a9::filesystem::copy_option::rename:
                    std::filesystem::rename(_from, _to);
                    break;
                }
            } catch (std::filesystem::filesystem_error e) {
                console::print(console::print_type::error, e.what());
                return false;
            }
            return true;
        }
    }
}
