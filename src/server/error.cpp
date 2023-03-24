//
// Created by Rosier on 2023-03-09.
//

#include "error.hpp"

#include <iostream>

void beastFail(beast::error_code Ec, const char* What) {
    std::cerr << What << ": " << Ec.message() << '\n';
}