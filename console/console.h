#pragma once

#include <iostream>

namespace a9 {
    namespace console {
        enum class print_type { information, warning, error };

        namespace internal {
            int print_level = 1;

            void print_lines() {}

            void print_lines(const std::string& _line) {
                std::cout << "   " << _line << std::endl;
            }

            template <class... Args>
            void print_lines(const std::string& _line, Args... _lines) {
                std::cout << "   " << _line << std::endl;

                print_lines(std::forward<Args>(_lines)...);
            }

            template <class... Args>
            void print_first_line(print_type _type, const std::string& _line, Args... _lines) {
                switch (_type) {
                case print_type::information:
                    std::cout << "I: ";
                    break;

                case print_type::warning:
                    std::cout << "W: ";
                    break;

                case print_type::error:
                    std::cout << "E: ";
                    break;
                }

                std::cout << _line << std::endl;

                print_lines(std::forward<Args>(_lines)...);
            }
        }

        /**
         * @brief Set the console printing level
         * @param _level
         * @param 0: Do not print
         * @param 1: Print information, warning and error
         * @param 2: Print warning and error
         * @param 3: Print Error
         */
        void set_print_level(int _level) {
            if (_level < 0 || _level > 3) {
                return;
            }
            internal::print_level = _level;
        }

        template <class... Args>
        void print(print_type _type, Args... _lines) {
            const bool printable =
                (internal::print_level == 1) || (internal::print_level == 2 && _type != print_type::information)
                || (internal::print_level == 3 && _type != print_type::information && _type != print_type::warning);

            if (printable) {
                internal::print_first_line(_type, std::forward<Args>(_lines)...);
                std::cout << std::endl;
            }
        }
    };
}
