//
// Created by Rosier on 2023-01-03.
//

#ifndef BLOONSTD6SERVER_HMAC_H
#define BLOONSTD6SERVER_HMAC_H

#include <openssl/evp.h>

#include <cstring>
#include <sstream>
#include <iomanip>

#include "common.h"

string hmac(const string &data, const string &key);


#endif //BLOONSTD6SERVER_HMAC_H
