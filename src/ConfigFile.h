//
// Created by Rosier on 2022-12-15.
//

#ifndef BLOONSTD6SERVER_CONFIGFILE_H
#define BLOONSTD6SERVER_CONFIGFILE_H

#include "common.h"

class ConfigFile {
// Variables
private:
    string m_configDir = "~/.local/share/BTD6Server/config";
    string m_cgroupRoot = "~/.local/share/BTD6Server/cgroup-root";
    string m_proxyAddress = "127.0.0.1";
    string m_proxyPort = "44333";
    bool m_blockBFB = false;

public:

// Functions
private:

public:
    ConfigFile(string configDir);

    void readFile();
};


#endif //BLOONSTD6SERVER_CONFIGFILE_H
