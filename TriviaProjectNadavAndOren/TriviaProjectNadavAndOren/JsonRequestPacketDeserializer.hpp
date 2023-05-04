#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include "json.hpp"
#include <chrono>
#include "IRequestHandler.h"

using json = nlohmann::json;

class JsonRequestPacketDeserializer 
{
public:
    static LoginRequest deserializeLoginRequest(const RequestInfo& requestInfo) {
        //std::string jsonString(requestInfo.messageContent.begin(), requestInfo.messageContent.end());
        json j = json::parse(requestInfo.messageContent);
        LoginRequest request;
        request.username = j.at("username").get<std::string>();
        request.password = j.at("password").get<std::string>();
        return request;
    }

    static SignupRequest deserializeSignUpRequest(const RequestInfo& requestInfo) {
        std::string jsonString(requestInfo.messageContent.begin(), requestInfo.messageContent.end());
        json j = json::parse(jsonString);
        SignupRequest request;
        request.username = j.at("username").get<std::string>();
        request.password = j.at("password").get<std::string>();
        request.email = j.at("email").get<std::string>();
        return request;
    }
};
