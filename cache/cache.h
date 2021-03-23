#pragma once

#include "../filesystem/filesystem.h"

#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include <filesystem>

namespace a9 {
	namespace cache {
		namespace internal {
			std::string filepath(const std::string& _cache_name) {
				return ".a9cache/" + _cache_name + ".a9cache";
			}

			/*
			// Loading cache functions
			*/
			template<typename T>
			std::vector<T> load_number(const std::string& _cache_name) {
				std::vector<T> vector;
				std::ifstream f(internal::filepath(_cache_name));
				while (!f.eof()) {
					T buf;
					f >> buf;
					vector.push_back(buf);
				}
				return vector;
			}

			std::vector<std::string> load_string(const std::string& _cache_name) {
				std::vector<std::string> vector;
				std::ifstream f(filepath(_cache_name));
				std::string buf;
				while (std::getline(f, buf)) {
					vector.push_back(buf);
				}

				return vector;
			}

			template<typename T>
			std::vector<T> load(const std::string& _cache_name) {
				console::print(console::print_type::error, "This type is not supported for caching");
				return std::vector<T>();
			}
			template<>
			std::vector<int> load(const std::string& _cache_name) { return load_number<int>(_cache_name); }
			template<>
			std::vector<double> load(const std::string& _cache_name) { return load_number<double>(_cache_name); }
			template<>
			std::vector<std::string> load(const std::string& _cache_name) { return load_string(_cache_name); }

			/*
			// Making cache functions
			*/
			template<typename T>
			void make_number(const std::vector<T>& _vector, const std::string& _cache_name) {
				filesystem::create_directory(".a9cache/");
				std::ofstream f(filepath(_cache_name));
				for (auto& str : _vector) {
					f << str << "\n";
				}
				f.close();
			}

			template<typename T>
			void make_string(const std::vector<T>& _vector, const std::string& _cache_name) {
				filesystem::create_directory(".a9cache/");
				std::ofstream f(filepath(_cache_name));
				for (auto& str : _vector) {
					f << str << "\n";
				}
				f.close();
			}

			template<typename T>
			void make(const std::vector<T>& _vector, const std::string& _cache_name) {
				console::print(console::print_type::error, "This type is not supported for caching");
			}
			template<>
			void make(const std::vector<int>& _vector, const std::string& _cache_name) { make_number(_vector, _cache_name); }
			template<>
			void make(const std::vector<double>& _vector, const std::string& _cache_name) { make_number(_vector, _cache_name); }
			template<>
			void make(const std::vector<std::string>& _vector, const std::string& _cache_name) { make_string(_vector, _cache_name); }
		}

		bool is_exist(const std::string& _cache_name) {
			std::ifstream f(internal::filepath(_cache_name));
			return f.is_open();
		}

		template<typename T>
		std::vector<T> load(const std::string& _cache_name, std::function<std::vector<T>()> _make_cache_func) {
			if (!a9::cache::is_exist(_cache_name)) {
				if (_make_cache_func != nullptr) {
					console::print(console::print_type::information, "Creating cache: " + _cache_name);
					const std::vector<T> items = _make_cache_func();
					internal::make<T>(items, _cache_name);
					console::print(console::print_type::information, "Cache created: " + _cache_name);
					return items;
				}
				console::print(console::print_type::error, "The cache is not exist, but _make_cache_func is nullptr");
				return std::vector<T>();
			}
			else {
				return internal::load<T>(_cache_name);
			}
		}
	}
}