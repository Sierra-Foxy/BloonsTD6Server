#include "Message.h"
#include "Message.cpp"

#include <boost/beast/http.hpp>

namespace http = beast::http;

template class Message<http::response<http::string_body>>;
template class Message<http::request<http::string_body>>;