//
// Created by Rosier on 2023-01-19.
//

#ifndef BLOONSTD6TOOLS_MESSAGEPART_H
#define BLOONSTD6TOOLS_MESSAGEPART_H

#include <sstream>

#include "common.h"

class MessagePart{
public:
    // Reading functions
    template<typename T>
    T readLE(const string &data)
    {
        T out{0};
        for (size_t i{0}; i < sizeof(T); i++)
        {
            out |= data[i] << i * 8;
        }
        return out;
    }

    template<typename T>
    T readLE(std::stringstream &data)
    {
        T out{0};
        for (size_t i{0}; i < sizeof(T); i++)
        {
            out |= data.get() << i * 8;
        }
        return out;
    }

    template<typename T>
    string readString(const string &data)
    {
        int len = readLE<T>(data);
        return {data.begin()+sizeof(T), data.begin()+sizeof(T)+len};
    }

    template<typename T>
    string readString(std::stringstream &data)
    {
        int len = readLE<T>(data);
        string out(len, 0);
        data.read(out.data(), len);
        return {out.begin(), out.end()};
    }

    // Writing functions
    template<typename T>
    string writeLE(T number)
    {
        string out;
        for (int i{0}; i < sizeof(number); i++)
        {
            out.push_back(static_cast<uint8_t>(number >> (i * 8)));
        }
        return out;
    }

    template<typename T>
    void writeLE(T number, string &str)
    {
        for (int i{0}; i < sizeof(number); i++)
        {
            str.push_back(static_cast<uint8_t>(number >> (i * 8)));
        }
    }

    template<typename T>
    string writeString(const string &str)
    {
        string out;
        writeLE(static_cast<T>(str.length()), out);
        out.append(str);
        return out;
    }

    template<typename T>
    void writeString(const string &inStr, string &dst)
    {
        writeLE(static_cast<T>(inStr.length()), dst);
        dst.append(inStr);
    }
};


#endif //BLOONSTD6TOOLS_MESSAGEPART_H


















