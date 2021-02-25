#pragma once

#include <iostream>

namespace a9 {
    namespace console {

        int console_print_level = 1;

        enum class print_type {
            information,
            warning,
            error
        };

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
            console_print_level = _level;
        }

        template <class... Args>
        void print(print_type _type, Args... _lines) {
            const bool printable =
                (console_print_level == 1)
                || (console_print_level == 2 && _type != print_type::information)
                || (console_print_level == 3 && _type != print_type::information && _type != print_type::warning);

            if (printable) {
                print_first_line(_type, std::forward<Args>(_lines)...);
                std::cout << std::endl;
            }
        }

        void print_lines() {}

        template <class... Args>
        void print_lines(const char* _line, Args... _lines) {
            std::cout << "   " << _line << std::endl;

            print_lines(std::forward<Args>(_lines)...);
        }

        template <class... Args>
        void print_first_line(print_type _type, const std::string& _line, Args... _lines) {
            switch (_type)
            {
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
    };
}