//
// Created by Rosier on 2023-03-09.
//

#ifndef BLOONSTOOLS_SRC_SERVER_ERROR_HPP
#define BLOONSTOOLS_SRC_SERVER_ERROR_HPP

#include <boost/beast/core/error.hpp>

namespace beast = boost::beast;

void beastFail(beast::error_code Ec, const char* What);

#endif // BLOONSTOOLS_SRC_SERVER_ERROR_HPP
