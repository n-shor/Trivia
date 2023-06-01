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
        std::string jsonString(requestInfo.messageContent.begin(), requestInfo.messageContent.end());
        json j = json::parse(jsonString);
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

    static GetPlayersInRoomRequest deserializeGetPlayersRequest(const RequestInfo& requestInfo) {
        std::string jsonString(requestInfo.messageContent.begin(), requestInfo.messageContent.end());
        json j = json::parse(jsonString);
        GetPlayersInRoomRequest request;
        request.roomId = j.at("roomId").get<unsigned int>();
        return request;
    }

    static JoinRoomRequest deserializeJoinRoomRequest(const RequestInfo& requestInfo) {
        std::string jsonString(requestInfo.messageContent.begin(), requestInfo.messageContent.end());
        json j = json::parse(jsonString);
        JoinRoomRequest request;
        request.roomId = j.at("roomId").get<unsigned int>();
        return request;
    }

    static CreateRoomRequest deserializeCreateRoomRequest(const RequestInfo& requestInfo) {
        std::string jsonString(requestInfo.messageContent.begin(), requestInfo.messageContent.end());
        json j = json::parse(jsonString);
        CreateRoomRequest request;
        request.answerTimeout = j.at("answerTimeout").get<unsigned int>();
        request.maxUsers = j.at("maxUsers").get<unsigned int>();
        request.questionCount = j.at("questionCount").get<unsigned int>();
        request.roomName = j.at("roomName").get<std::string>();
        return request;
    }
};
