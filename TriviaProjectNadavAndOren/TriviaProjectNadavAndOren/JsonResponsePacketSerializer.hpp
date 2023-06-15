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
    static std::vector<unsigned char> serializeResponse(const LoginResponse& response) 
    {
        json j = { {"status", response.status} };
        return createBuffer(1, j.dump());
    }

    static std::vector<unsigned char> serializeResponse(const SignupResponse& response)
    {
        json j = { {"status", response.status} };
        return createBuffer(2, j.dump());
    }

    static std::vector<unsigned char> serializeResponse(const ErrorResponse& response) 
    {
        json j = { {"message", response.message} };
        return createBuffer(3, j.dump());
    }

    static std::vector<unsigned char> serializeResponse(const LogoutResponse& response)
    {
        json j = { {"status", response.status} };
        return createBuffer(3, j.dump());
    }

    static std::vector<unsigned char> serializeResponse(const GetRoomsResponse& response)
    {
        std::vector<json> r;
        json j = { {"status", response.status} };
        for (int i = 0; i < response.rooms.size(); i++) {
            r.push_back(json{ {"id" , response.rooms[i].id},
                              {"isActive" , response.rooms[i].isActive},
                              {"maxPlayers", response.rooms[i].maxPlayers},
                              {"name", response.rooms[i].name},
                              {"adminName", response.rooms[i].adminName},
                              {"currentPlayers", response.rooms[i].currentPlayers},
                              {"numOfQuestionsInGame", response.rooms[i].numOfQuestionsInGame},
                              {"timePerQuestion", response.rooms[i].timePerQuestion}
                });
        }
        j["rooms"] = r;
        return createBuffer(3, j.dump());

    }

    static std::vector<unsigned char> serializeResponse(const GetPlayesInRoomResponse& response)
    {
        json j = {};
        j["players"] = response.players;
        return createBuffer(3, j.dump());
    }

    static std::vector<unsigned char> serializeResponse(const JoinRoomResponse& response)
    {
        json j = { {"status", response.status} };
        return createBuffer(3, j.dump());
    }

    static std::vector<unsigned char> serializeResponse(const CreateRoomResponse& response)
    {
        json j = {
            { "status", response.status },
            { "roomId", response.roomId },
            { "adminName", response.adminName }
        };
        return createBuffer(3, j.dump());
    }


    static std::vector<unsigned char> serializeResponse(const getHighScoreResponse& response)
    {
        json j = { {"status", response.status }, {"HighScores", response.statistics}};
        return createBuffer(3, j.dump());
    }

    static std::vector<unsigned char> serializeResponse(const getPersonalStatsResponse& response)
    {
        json j = { {"status", response.status }, {"statistics", response.statistics} };
        return createBuffer(3, j.dump());
    }

    static std::vector<unsigned char> serializeResponse(const LeaveRoomResponse& response)
    {
        json j = { {"status", response.status} };
        return createBuffer(1, j.dump()); //ask nadav of code
    }

    static std::vector<unsigned char> serializeResponse(const GetRoomStateResponse& response)
    {
        json j = { {"status", response.status}, {"players", response.players}, {"hasGameBegun", response.hasGameBegun}, {"questionCount", response.questionCount}, {"answerTimeout", response.answerTimeout}};
        return createBuffer(1, j.dump()); //ask nadav of code
    }

    static std::vector<unsigned char> serializeResponse(const StartGameResponse& response)
    {
        json j = { {"status", response.status} };
        return createBuffer(1, j.dump()); //ask nadav of code
    }

    static std::vector<unsigned char> serializeResponse(const CloseRoomResponse& response)
    {
        json j = { {"status", response.status} };
        return createBuffer(1, j.dump()); //ask nadav of code
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
