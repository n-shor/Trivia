using Microsoft.Maui.Controls;
using System;
using System.Collections.Generic;
using System.Text.Json;

namespace GUI
{
    public class User
    {
        public string Username { get; set; }

        public override string ToString()
        {
            return Username;
        }
    }


    public enum OriginPage
    {
        CreateRoomPage,
        JoinRoomPage,
    }

    public partial class RoomPage : ContentPage
    {
        private RoomData _currentRoom;
        private OriginPage _originPage;

        public RoomPage(RoomData room, OriginPage originPage)
        {
            InitializeComponent();

            _currentRoom = room;
            _originPage = originPage;

            RoomNameLabel.Text += _currentRoom.name;
            AdminNameLabel.Text += _currentRoom.adminName;
            
            // Call updateUserList to populate the user list
            updateUserList();
        }


        private void updateUserList()
        {
            // Create a request to get the players in the room
            var request = new GetPlayersInRoomRequest { roomId = _currentRoom.id };

            // Serialize the request to JSON
            var jsonString = JsonSerializer.Serialize(request);

            // Send the request to the server
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, (int)2, jsonString);

            // Receive and deserialize the response
            dynamic data = Deserielizer.getResponse(ClientSocket.sock);
            GetPlayesInRoomResponse response = JsonSerializer.Deserialize<GetPlayesInRoomResponse>(data.jsonData);

            // Populate the user list, excluding the admin
            List<User> users = new List<User>();
            foreach (var username in response.players)
            {
                if (username != _currentRoom.adminName)  // Exclude the admin
                {
                    users.Add(new User { Username = username });
                }
            }

            // Update the ListView
            UsersListView.ItemsSource = users;
        }

        private void OnRefreshButtonClicked(object sender, EventArgs e)
        {
            updateUserList();
        }

        private void OnBackButtonClicked(object sender, EventArgs e)
        {
            switch (_originPage)
            {
                case OriginPage.CreateRoomPage:
                    Navigation.PushAsync(new CreateRoomPage());
                    break;
                case OriginPage.JoinRoomPage:
                    Navigation.PushAsync(new JoinRoomPage());
                    break;
            }
        }

    }
}
