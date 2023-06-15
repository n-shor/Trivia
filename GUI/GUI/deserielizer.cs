using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;


public class LoginRequest
{
    public string username { get; set; }
    public string password { get; set; }
};

public class SignupRequest
{
    public string username { get; set; }
    public string password { get; set; }
    public string email { get; set; }
};

public class LoginResponse
{
    public int status { get; set; }
};

public class SignupResponse
{
    public int status { get; set; }
};

public class RoomData
{
    public int id { get; set; }
    public string name { get; set; }
    public int maxPlayers { get; set; }
    public int numOfQuestionsInGame { get; set; }
    public int timePerQuestion { get; set; }
    public int isActive { get; set; }
    public int currentPlayers { get; set; }
    public string adminName { get; set; }
    public string DisplayText => $"{name} (Max Players: {maxPlayers})";

};

public class CreateRoomRequest
{
    public string roomName { get; set; }
    public int maxUsers { get; set; }
    public int questionCount { get; set; }
    public int answerTimeout { get; set; }
};

public class GetPlayersInRoomRequest
{
    public int roomId { get; set; }
};

public class JoinRoomRequest
{
    public int roomId { get; set; }
};

public class GetPlayersInRoomResponse
{
    public List<string> players { get; set; }
};

public class GetHighScoreResponse
{
    public int status { get; set; }
    public List<string> statistics { get; set; }
};

public class getPersonalStatsResponse
{
    public int status { get; set; }
    public List<string> statistics { get; set; }
};

public class JoinRoomResponse
{
    public int status { get; set; }
};

public class CreateRoomResponse
{
    public int status { get; set; }
    public int roomId { get; set; }
    public string adminName { get; set; }
}

public class GetRoomsResponse
{
    public int status { get; set; }
    public List<RoomData> rooms { get; set; }
};

public class ErrorResponse
{
    public string message { get; set; }
};

struct GetRoomStateResponse
{
    public int status { get; set; }
    public bool hasGameBegun { get; set; }
    public List<string> players { get; set; }
    public int questionCount { get; set; }
    public int answerTimeout { get; set; }
};


namespace GUI
{
    internal class Deserielizer
    {
        public static dynamic getResponse(Socket sock)
        {
            // Receive the message type
            byte[] typeBuffer = new byte[1];
            sock.Receive(typeBuffer);
            int type = typeBuffer[0];

            // Receive the JSON data size
            byte[] sizeBuffer = new byte[4];
            sock.Receive(sizeBuffer);
            int jsonSize = BitConverter.ToInt32(sizeBuffer.Reverse().ToArray(), 0);

            // Receive the JSON data
            byte[] jsonBuffer = new byte[jsonSize];
            int received = 0;
            while (received < jsonSize)
            {
                received += sock.Receive(jsonBuffer, received, jsonSize - received, SocketFlags.None);
            }

            // Convert the bytes into string
            string jsonData = Encoding.ASCII.GetString(jsonBuffer);

            return new { type, jsonData = jsonData };
        }

    }
}
