#pragma once

#include <vector>
#include <string>

#include <Windows.h>

namespace a9 {
	namespace util {
		template<class T, class Func>
		[[nodiscard]] std::vector<T> remove_if(std::vector<T> _vector, Func _func) {
			auto result = std::remove_if(_vector.begin(), _vector.end(), _func);
			return std::vector<T>(_vector.begin(), result);
		}

        std::vector<std::string> split(const std::string& str, char deliminator) {
            std::vector<std::string> vector;
            std::string item;

            for (auto& s : str) {
                if (s == deliminator) {
                    if (!item.empty()) {
                        vector.push_back(item);
                    }
                    item.clear();
                }
                else {
                    item += s;
                }
            }

            return vector;
        }
	}
}