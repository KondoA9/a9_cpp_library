#pragma once

#include <fstream>
#include <vector>
#include <string>

#include "../util/util.h"

namespace a9 {
	class csv {
		std::vector<std::vector<std::string>> cells_;
		size_t row_ = 0, column_ = 0;
		char delimiter_ = ',';
		std::string path_;
		bool is_open_ = false;

	public:
		csv() {}

		csv(const std::string& _path, size_t _header_rows = 0) {
			open(_path, _header_rows);
		}

		~csv() {}

		bool open(const std::string& _path, size_t _header_rows = 0) {
			path_ = _path;
			is_open_ = parse_csv(_header_rows);
			return is_open_;
		}

		bool save(const std::string& _path) {
			std::ofstream file(_path);
			if (!file.is_open()) {
				return false;
			}

			for (const auto& row : cells_) {
				for (const auto& cell : row) {
					file << cell << ',';
				}
				file << '\n';
			}

			file.close();
			return true;
		}

		void clear() {
			std::vector<std::vector<std::string>>().swap(cells_);
		}

		size_t rows()const {
			return cells_.size();
		}

		size_t columns(size_t _row)const {
			return cells_[_row].size();
		}

		void write(const std::string& _val) {
			if (cells_.size() <= row_) {
				cells_.push_back(std::vector<std::string>());
			}
			cells_[row_].push_back(_val);
		}

		void write(int _val) {
			write(std::to_string(_val));
		}

		void write(double _val) {
			write(std::to_string(_val));
		}

		template <typename T, class... Args>
		void write_line(T _val, Args ..._line) {
			write(_val);
			write_line(std::forward<Args>(_line)...);
		}

		void write_new_line() {
			cells_.push_back(std::vector<std::string>());
			row_ += 1;
		}

		std::vector<std::vector<std::string>> as_vector()const {
			return cells_;
		}

		void set_delimiter(char c) {
			delimiter_ = c;
		}

		operator bool()const {
			return is_open_;
		}

		std::vector<std::string> operator[] (int _i) const {
			return cells_[_i];
		}

	private:
		bool parse_csv(size_t _header_rows) {
			std::ifstream file(path_);
			if (!file.is_open()) {
				return false;
			}
			
			size_t header_count = 0;
			std::string buf;
			while (std::getline(file, buf)) {
				header_count++;
				if (header_count <= _header_rows) {
					continue;
				}

				const auto values = util::split(buf, delimiter_, true, true);
				cells_.push_back(values);
			}
			file.close();
			return true;
		}

		void write_line() {
			write("\n");
		}
	};
}