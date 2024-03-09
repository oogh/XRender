//
// Created by Oogh on 2020/10/20.
//

#include "XMacView.hpp"

int main(int argc, char* argv[]) {
    auto surface = std::make_unique<XMacView>(800, 600);
    getchar();
    return 0;
}