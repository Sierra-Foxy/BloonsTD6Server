//
// Created by Rosier on 2023-01-02.
//

#include "dgdata.h"

bool isDgdataEncoded(const string &data)
{
    if (data.substr(0, 6) == "DGDATA") { return true; }
    return false;
}

string decodeDgdata(const string &data) {
    std::basic_string<unsigned char> result(data.begin(), data.end());
    for (int i{14}; i < data.length(); i++) {
        result[i] = result[i] - 21;
        result[i] = result[i] - ((i - 14) % 6);
    }

    return {result.begin() + 14, result.end()};
}