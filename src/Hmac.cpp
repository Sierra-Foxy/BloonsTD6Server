//
// Created by Rosier on 2023-01-03.
//

#include "Hmac.h"

string hmac(const string &data, const string &key)
{
    unsigned char blockKey[64];
    std::memset(blockKey, 0, 64);

    unsigned char hashed[20];
    if (key.length() > 64) // Shorten key with hash
    {

        EVP_Q_digest(nullptr, "sha1", nullptr, data.c_str(), data.length(), hashed, nullptr);
        std::memcpy(blockKey, hashed, 20);
    }
    else
    {
        std::memcpy(blockKey, key.c_str(), key.length());
    }

    unsigned char innerPad[64];
    std::memset(innerPad, 0x36, 64);
    unsigned char outerPad[64];
    std::memset(outerPad, 0x5c, 64);

    unsigned char innerKeyPadded[64];
    unsigned char outerKeyPadded[64];

    for (int i{0}; i < 64; i++)
    {
        innerKeyPadded[i] = blockKey[i] ^ innerPad[i];
        outerKeyPadded[i] = blockKey[i] ^ outerPad[i];
    }

    string innerToBeHashed((char *)innerKeyPadded, 64);
    innerToBeHashed.append(data);
    EVP_Q_digest(nullptr, "sha1", nullptr, innerToBeHashed.c_str(), innerToBeHashed.length(), hashed, nullptr);

    string outerToBeHashed((char *)outerKeyPadded, 64);
    outerToBeHashed.append(string((char *)hashed, 20));
    EVP_Q_digest(nullptr, "sha1", nullptr, outerToBeHashed.c_str(), outerToBeHashed.length(), hashed, nullptr);

    std::stringstream hexHash;
    hexHash << std::hex;

    for (int i{0}; i < 20; i++)
    {
        hexHash << std::setw(2) << std::setfill('0') << (unsigned int)hashed[i];
    }

    return hexHash.str();
}
