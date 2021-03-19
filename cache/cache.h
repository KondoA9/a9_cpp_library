#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include "../filesystem/filesystem.h"

namespace a9 {
	namespace cache {
		namespace internal {
			std::string filepath(const std::string& _cache_name) {
				return ".a9cache/" + _cache_name + ".a9cache";
			}

			std::vector<std::string> load_str(const std::string& _cache_name) {
				std::vector<std::string> vector;
				std::ifstream f(filepath(_cache_name));

				std::string buf;
				while (std::getline(f, buf)) {
					vector.push_back(buf);
				}

				return vector;
			}

			std::vector<int> load_int(const std::string& _cache_name) {
				std::vector<int> vector;
				std::ifstream f(filepath(_cache_name));

				while (!f.eof()) {
					int buf;
					f >> buf;
					vector.push_back(buf);
				}

				return vector;
			}

			void make(const std::vector<std::string>& _vector, const std::string& _cache_name) {
				filesystem::create_directory(".a9cache/");
				std::ofstream f(filepath(_cache_name));
				for (auto& str : _vector) {
					f << str << "\n";
				}
				f.close();
			}

			void make(const std::vector<int>& _vector, const std::string& _cache_name) {
				filesystem::create_directory(".a9cache/");
				std::ofstream f(filepath(_cache_name));
				for (auto& str : _vector) {
					f << str << "\n";
				}
				f.close();
			}
		}

		bool is_exist(const std::string& _cache_name) {
			std::ifstream f(internal::filepath(_cache_name));
			return f.is_open();
		}

		std::vector<int> load(const std::string& _cache_name, std::function<std::vector<int>()> _make_cache_func) {
			if (!a9::cache::is_exist(_cache_name)) {
				if (_make_cache_func != nullptr) {
					console::print(console::print_type::information, "Creating cache: " + _cache_name);
					std::vector<int> items = _make_cache_func();
					internal::make(items, _cache_name);
					console::print(console::print_type::information, "Cache created: " + _cache_name);
					return items;
				}
				console::print(console::print_type::error, "The cache is not exist, but _make_cache_func is nullptr");
				return std::vector<int>();
			}
			else {
				return internal::load_int(_cache_name);
			}
		}

		std::vector<std::string> load(const std::string& _cache_name, std::function<std::vector<std::string>()> _make_cache_func) {
			if (!a9::cache::is_exist(_cache_name)) {
				if (_make_cache_func != nullptr) {
					console::print(console::print_type::information, "Creating cache: " + _cache_name);
					const std::vector<std::string> items = _make_cache_func();
					internal::make(items, _cache_name);
					console::print(console::print_type::information, "Cache created: " + _cache_name);
					return items;
				}
				console::print(console::print_type::error, "The cache is not exist, but _make_cache_func is nullptr");
				return std::vector<std::string>();
			}
			else {
				return internal::load_str(_cache_name);
			}
		}
	}
}