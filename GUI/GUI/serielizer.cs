using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

enum LoginRequestStatus { AlreadyLoggedIn = 36, Error = -1, FailedLogin = 37, UserAlreadyExists, LoggedIn, SignedUp, NoSuchLoggedUser, LoggedOut };
enum SubmitAnswerStatus { incorrectAnswer = 11, correctAnswer };
enum GameRequestTypes { getQuestionReq = 4, submitAnswerReq, getGameResultsReq, leaveGameReq };
enum GameRequeststatus { getQuestionSuccessful = 8, gameEnded, leaveGameResponse };
enum MenuRequestTypes { CreateRoom = 21, GetRooms, GetPlayersInRoom, JoinRoom, GetStatistics, Logout, GetHighScore, AddQuestion = 110 };
enum MenuRequestStatus { signedOut = 28, GetRoomsSuccessful, GetPlayersInRoomSuccessful, GetPersonalStatsSuccessful, GetHighScoreSuccessful, joinRoomSuccessful, joinRoomUnSuccessful, CreateRoomSuccessful , InvalidQuestion = 170, QuestionAdded };
enum RoomAdminRequestTypes { CloseRoom = 43, StartGame = 2 };
enum RoomAdminRequeststatus { closeRoomSuccessful = 13, startGameSuccessful, startGameUnsuccessful, theServerDoesntHaveEnoughQuestions };
enum RoomMemberRequestTypes { LeaveRoom = 17, GetRoomsState };
enum RoomMemberRequeststatus { leaveRoomSuccessful = 19, getRoomsStateRes };


namespace GUI
{
    public class Serielizer
    {
        public void sendMessage(Socket clientSocket, int type, string buffer)
        {
            // Convert the length to a byte array
            byte[] lengthBytes = BitConverter.GetBytes(buffer.Length);

            byte[] message = new byte[5 + buffer.Length];
            message[0] = Convert.ToByte(type);

            Array.Copy(lengthBytes, 0, message, 1, 4);

            byte[] bufferBytes = Encoding.ASCII.GetBytes(buffer);
            Array.Copy(bufferBytes, 0, message, 5, buffer.Length);

            clientSocket.Send(message);
        }

    }
}
