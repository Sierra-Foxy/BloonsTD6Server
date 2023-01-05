#include <iostream>
#include <fstream>
#include <filesystem>
#include <cctype>
#include <cstring>

namespace fs = std::filesystem;

#include "common.h"
#include "Proxy.h"
#include "Message.h"

int main() {
//    for (auto const& dirEntry : fs::directory_iterator{"/proc"}) {
//        try {
//            std::stoi(dirEntry.path().filename().string());
//        } catch (...) { continue; }
//
//        char buffer[64];
//        std::ifstream procFile{dirEntry.path().string() + "/stat"};
//        procFile.ignore(25, '(');
//        procFile.getline(buffer, 64, ')');
//        if (strcmp(buffer, "BloonsTD6.exe") == 0) {
//            cout << buffer << endl;
//            procFile.close();
//            break;
//        }
//    }

    Proxy proxy;
    proxy.run();
    return 0;
}
