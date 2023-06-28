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
    static const LoginRequest deserializeLoginRequest(const RequestInfo& requestInfo) {
        std::string jsonString(requestInfo.messageContent.begin(), requestInfo.messageContent.end());
        json j = json::parse(jsonString);
        LoginRequest request;
        request.username = j.at("username").get<std::string>();
        request.password = j.at("password").get<std::string>();
        return request;
    }

    static const SignupRequest deserializeSignUpRequest(const RequestInfo& requestInfo) {
        std::string jsonString(requestInfo.messageContent.begin(), requestInfo.messageContent.end());
        json j = json::parse(jsonString);
        SignupRequest request;
        request.username = j.at("username").get<std::string>();
        request.password = j.at("password").get<std::string>();
        request.email = j.at("email").get<std::string>();
        return request;
    }

    static const GetPlayersInRoomRequest deserializeGetPlayersRequest(const RequestInfo& requestInfo) {
        std::string jsonString(requestInfo.messageContent.begin(), requestInfo.messageContent.end());
        json j = json::parse(jsonString);
        GetPlayersInRoomRequest request;
        request.roomId = j.at("roomId").get<unsigned int>();
        return request;
    }

    static const JoinRoomRequest deserializeJoinRoomRequest(const RequestInfo& requestInfo) {
        std::string jsonString(requestInfo.messageContent.begin(), requestInfo.messageContent.end());
        json j = json::parse(jsonString);
        JoinRoomRequest request;
        request.roomId = j.at("roomId").get<unsigned int>();
        return request;
    }

    static const CreateRoomRequest deserializeCreateRoomRequest(const RequestInfo& requestInfo) {
        std::string jsonString(requestInfo.messageContent.begin(), requestInfo.messageContent.end());
        json j = json::parse(jsonString);
        CreateRoomRequest request;
        request.answerTimeout = j.at("answerTimeout").get<unsigned int>();
        request.maxUsers = j.at("maxUsers").get<unsigned int>();
        request.questionCount = j.at("questionCount").get<unsigned int>();
        request.roomName = j.at("roomName").get<std::string>();
        return request;
    }

    static const SubmitAnswerRequest deserializeSubmitAnswerRequest(const RequestInfo& requestInfo) {
        std::string jsonString(requestInfo.messageContent.begin(), requestInfo.messageContent.end());
        json j = json::parse(jsonString);
        SubmitAnswerRequest request;
        request.answerId = j.at("answerId").get<unsigned int>();
        return request;
    }

    static const AddQuestionRequest deserializeAddQuestionRequest(const RequestInfo& requestInfo) {
        std::string jsonString(requestInfo.messageContent.begin(), requestInfo.messageContent.end());
        json j = json::parse(jsonString);
        AddQuestionRequest request;
        request.question = j.at("question").get<std::string>();
        request.optionA = j.at("optionA").get<std::string>();
        request.optionB = j.at("optionB").get<std::string>();
        request.optionC = j.at("optionC").get<std::string>();
        request.optionD = j.at("optionD").get<std::string>();
        request.correctAnswer = (j.at("correctAnswer").get<std::string>())[0];
        return request;
    }
};
