#pragma once

#include <fstream>
#include <vector>
#include <string>

namespace a9 {
	class csv {
		std::vector<std::vector<std::string>> cells_;
		size_t row_ = 0, column_ = 0;
		char delimiter_ = ',';
		std::string path_;
		bool is_open_ = false;

	public:
		csv() {}

		csv(const std::string& _path) {
			open(_path);
		}

		~csv() {
			close();
		}

		bool open(const std::string& _path) {
			path_ = _path;
			is_open_ = parse_csv();
			return is_open_;
		}

		void close() {

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

		template <class... Args>
		void write_line(Args ..._line) {
			write_line(std::forward<Args>(_line)...);
			cells_.push_back(std::vector<std::string>());
			row_ += 1;
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
		bool parse_csv() {
			std::ifstream file(path_);
			if (!file.is_open()) {
				return false;
			}

			std::string buf;
			while (std::getline(file, buf)) {
				std::vector<std::string> row;
				row = split_row(buf);
				cells_.push_back(row);
			}
			file.close();
			return true;
		}

		std::vector<std::string> split_row(const std::string _row) {
			std::vector<std::string> row;
			std::string cell;
			for (size_t i = 0; i < _row.size();i++) {
				const char c = _row[i];
				bool push = false;
				if (c == delimiter_ && cell.size() != 0) {
					push = true;
				}
				else if (i == _row.size() - 1) {
					cell += c;
					push = true;
				}
				else {
					cell += c;
				}

				if (push) {
					if (cell.size() > 2 && cell[0] == '\"' && cell[cell.size() - 1] == '\"') {
						cell.erase(0, 1);
						cell.pop_back();
						cell.shrink_to_fit();
					}
					row.push_back(cell);
					cell = "";
				}
			}
			return row;
		}

		template <typename T, class... Args>
		void write_line(T _val, Args ..._line) {
			write(_val);
			write_line(std::forward<Args>(_line)...);
		}

		void write_line() {
			write("\n");
		}
	};
}