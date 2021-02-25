#pragma once

#include <iostream>
#include <filesystem>
#include <functional>
#include <vector>
#include <string>
#include <thread>
#include <mutex>

#include "../console/console.h"

namespace a9 {
    namespace filesystem {
        enum class copy_option {
            // Copy the source content
            copy,

            // Rename the source content
            // Source content will be removed
            rename
        };

        namespace util {
            [[nodiscard]] std::vector<std::string> remove_if(std::vector<std::string> _vector, std::function<bool(std::string)> _func) {
                auto result = std::remove_if(_vector.begin(), _vector.end(), _func);
                return std::vector<std::string>(_vector.begin(), result);
            }
        }

        bool is_directory(const std::string& _filepath) {
            return std::filesystem::is_directory(_filepath);
        }

        std::string get_extension(const std::string& _filepath) {
            const std::filesystem::path path = _filepath;
            if (auto ext = path.extension().string(); ext.size() > 0) {
                return ext.erase(0, 1);
            }
            return path.extension().string();
        }

        std::string get_parent_directory(const std::string& _path) {
            const std::filesystem::path path = _path;
            return path.parent_path().string();
        }

        bool create_directories(const std::string& _path) {
            const auto path = is_directory(_path) ? _path : get_parent_directory(_path);
            if (!std::filesystem::create_directories(path)) {
                throw "Failed to create directories";
                return false;
            }
            return true;
        }

        bool copy(const std::string& _from, const std::string& _to, copy_option _option) {
            try {
                create_directories(_to);
                switch (_option)
                {
                case a9::filesystem::copy_option::copy:
                    std::filesystem::copy(_from, _to);
                    break;
                case a9::filesystem::copy_option::rename:
                    std::filesystem::rename(_from, _to);
                    break;
                }
            }
            catch (std::filesystem::filesystem_error e) {
                console::print(console::print_type::error, e.what());
                return false;
            }
            catch (const char* e) {
                console::print(console::print_type::error, e);
                return false;
            }
            return true;
        }

        std::vector<std::string> get_directory_contents(const std::string& _directory_path, bool _recursive = false) {
            std::vector<std::string> contents;
            if (_recursive) {
                for (const auto& file : std::filesystem::recursive_directory_iterator(_directory_path)) {
                    contents.push_back(file.path().string());
                }
            }
            else {
                for (const auto& file : std::filesystem::directory_iterator(_directory_path)) {
                    contents.push_back(file.path().string());
                }
            }
            return contents;
        }

        bool rename_with(const std::vector<std::string>& _from, const std::vector<std::string>& _to, size_t _threads_count, copy_option _option, std::function<std::string(const std::string&, const std::string&)> _with_processing_func = nullptr) {
            std::vector<std::thread> threads;
            std::mutex mtx;

            if (_threads_count > std::thread::hardware_concurrency()) {
                console::print(console::print_type::warning, "Specified number of threads exceeds the hardware limit", "Number of threads was set automatically");
                _threads_count = std::thread::hardware_concurrency();
            }

            const size_t from_count = _from.size();
            const size_t to_count = _to.size();
            const size_t interval = static_cast<size_t>(from_count / _threads_count);

            if ((from_count != to_count) || from_count == 0 || to_count == 0) {
                console::print(console::print_type::error, "The size of \"from paths\" and \"to paths\" are not match");
                return false;
            }

            size_t processed_count = 1;

            for (size_t i = 0; i < _threads_count; i++) {
                threads.emplace_back([=, &processed_count, &mtx]() {
                    const size_t start = i * interval;
                    const size_t end = (i == _threads_count - 1) ? from_count : (i + 1) * interval;
                    for (size_t j = start; j < end; j++) {
                        const std::string to_path = _with_processing_func == nullptr ? _to[j] : _with_processing_func(_from[j], _to[j]);
                        copy(_from[j], to_path, _option);
                        std::lock_guard<std::mutex> lock(mtx);
                        processed_count++;
                        console::print(console::print_type::information, "processing: " + std::to_string(processed_count) + "/" + std::to_string(from_count));
                    }
                    });
            }

            for (auto& thread : threads) {
                thread.join();
            }

            return true;
        }

        bool rename_with(const std::vector<std::string>& _from, size_t _threads_count, copy_option _option, std::function<std::string(const std::string&)> _make_to_path_func) {
            std::vector<std::thread> threads;
            std::mutex mtx;

            if (_threads_count > std::thread::hardware_concurrency()) {
                console::print(console::print_type::warning, "Specified number of threads exceeds the hardware limit", "Number of threads was set automatically");
                _threads_count = std::thread::hardware_concurrency();
            }

            const size_t from_count = _from.size();
            const size_t interval = static_cast<size_t>(from_count / _threads_count);

            if (from_count == 0) {
                console::print(console::print_type::error, "The size of \"from paths\" and \"to paths\" are not match");
                return false;
            }

            size_t processed_count = 1;

            for (size_t i = 0; i < _threads_count; i++) {
                threads.emplace_back([=, &processed_count, &mtx]() {
                    const size_t start = i * interval;
                    const size_t end = (i == _threads_count - 1) ? from_count : (i + 1) * interval;
                    for (size_t j = start; j < end; j++) {
                        const std::string to_path = _make_to_path_func(_from[j]);
                        copy(_from[j], to_path, _option);
                        std::lock_guard<std::mutex> lock(mtx);
                        processed_count++;
                        console::print(console::print_type::information, "processing: " + std::to_string(processed_count) + "/" + std::to_string(from_count));
                    }
                    });
            }

            for (auto& thread : threads) {
                thread.join();
            }

            return true;
        }

    }
}