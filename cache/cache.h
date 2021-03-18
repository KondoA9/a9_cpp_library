#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include "../filesystem/filesystem.h"

namespace a9 {
	namespace cache {
		bool is_exist(const std::string& _name) {
			std::ifstream f(".a9cache/" + _name + ".a9cache");
			return f.is_open();
		}

		std::vector<std::string> load(const std::string& _name) {
			std::vector<std::string> vector;
			std::ifstream f(".a9cache/" + _name + ".a9cache");

			std::string buf;
			while (std::getline(f, buf)) {
				vector.push_back(buf);
			}

			return vector;
		}

		void make(const std::vector<std::string>& _vector, const std::string& _name) {
			filesystem::create_directory(".a9cache/");
			std::ofstream f(".a9cache/" + _name + ".a9cache");
			for (auto& str : _vector) {
				f << str << "\n";
			}
			f.close();
		}
	}
}