//
// Created by Rosier on 2023-01-17.
//

#include "Towers.h"

string camelToSpace(const string &str)
{
    string out{str};
    for (int i{0}; i < out.length(); i++)
    {
        if (isupper(out[i]) && i != 0)
        {
            out.insert(i++, " ");
        }
    }
    return out;
}