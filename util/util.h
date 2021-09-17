#pragma once

#include <vector>
#include <string>

namespace a9 {
	namespace util {
		template<class T, class Func>
		[[nodiscard]] std::vector<T> remove_if(std::vector<T> _vector, Func _func) {
			auto result = std::remove_if(_vector.begin(), _vector.end(), _func);
			return std::vector<T>(_vector.begin(), result);
		}

        std::vector<std::string> split(const std::string& _str, char _delimiter, bool _push_empty = false, bool _use_closure = false, char _ignore_delimiter_closure = '\"') {
            std::vector<std::string> splitted_str;
            std::string item;

            for (size_t i = 0; i < _str.size(); i++) {
                if (_str[i] == _delimiter) {
                    if (!item.empty() || _push_empty) {
                        splitted_str.push_back(item);
                    }
                    item.clear();
                }
                else {
                    item += _str[i];
                }
                
                if(i == _str.size() - 1 && (!item.empty() || _push_empty)){
                    splitted_str.push_back(item);
                }
            }

            if (!_use_closure) {
                return splitted_str;
            }

            bool ignore = false;
            size_t index = 0;
            std::vector<std::string> result;
            for (size_t i = 0; i < splitted_str.size(); i++) {
                if (!ignore && splitted_str[i][0] == _ignore_delimiter_closure) {
                    index = i;
                    ignore = true;
                }
                if (!ignore) {
                    result.push_back(splitted_str[i]);
                }
                if (ignore && splitted_str[i][splitted_str[i].size() - 1] == _ignore_delimiter_closure) {
                    std::string connected_str = "";
                    for (size_t j = index; j <= i; j++) {
                        connected_str += splitted_str[j];
                    }
                    result.push_back(connected_str);
                    ignore = false;
                }
            }

            return result;
        }
	}
}