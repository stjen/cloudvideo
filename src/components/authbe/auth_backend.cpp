#include "auth_backend.h"
#include <cstdlib>
#include <iostream>

using namespace authbe;

bool AuthBackend::verifyAPIKey(const std::string key) { return true; }

std::string AuthBackend::generateAPIToken(const std::string key) {
    std::string token = "";
    for (size_t i = 0; i < 10; i++)
    {
        auto r = rand()  % 27 + 65;
        std::cout << r << std::endl;
        token += (char)(r);
    }
    return token;
}