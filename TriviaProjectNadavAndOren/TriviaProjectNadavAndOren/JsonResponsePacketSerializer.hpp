#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include "json.hpp"
#include "IRequestHandler.h"

using json = nlohmann::json;

class JsonResponsePacketSerializer 
{
public:
    static std::vector<unsigned char> serializeLoginResponse(const LoginResponse& response) 
    {
        json j = { {"status", response.status} };
        return createBuffer(1, j.dump());
    }

    static std::vector<unsigned char> serializeSignUpResponse(const SignupResponse& response)
    {
        json j = { {"status", response.status} };
        return createBuffer(2, j.dump());
    }

    static std::vector<unsigned char> serializeErrorResponse(const ErrorResponse& response) 
    {
        json j = { {"message", response.message} };
        return createBuffer(3, j.dump());
    }

private:
    static std::vector<unsigned char> createBuffer(uint8_t code, const std::string& jsonString)
    {
        uint32_t jsonSize = static_cast<uint32_t>(jsonString.size());
        std::vector<unsigned char> buffer(5 + jsonSize);

        buffer[0] = code;
        buffer[1] = (jsonSize >> 24) & 0xFF;
        buffer[2] = (jsonSize >> 16) & 0xFF;
        buffer[3] = (jsonSize >> 8) & 0xFF;
        buffer[4] = jsonSize & 0xFF;

        std::copy(jsonString.begin(), jsonString.end(), buffer.begin() + 5);
        return buffer;
    }
};
