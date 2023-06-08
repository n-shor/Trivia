using Microsoft.Maui.Controls;
using System;
using System.Collections.Generic;
using System.Text.Json;

namespace GUI
{
    public class User
    {
        public string Username { get; set; }
    }

    public enum OriginPage
    {
        CreateRoomPage,
        JoinRoomPage,
    }

    public partial class RoomPage : ContentPage
    {
        private Room _currentRoom;
        private OriginPage _originPage;

        public RoomPage(Room room, OriginPage originPage)
        {
            InitializeComponent();

            _currentRoom = room;
            _originPage = originPage;

            RoomNameLabel.Text += _currentRoom.RoomName;
            AdminNameLabel.Text += _currentRoom.AdminName;

            // Call updateUserList to populate the user list
            updateUserList();
        }


        private async void updateUserList()
        {
            // Create a request to get the players in the room
            var request = new GetPlayersInRoomRequest { roomId = _currentRoom.Id };

            // Serialize the request to JSON
            var jsonString = JsonSerializer.Serialize(request);

            // Send the request to the server
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, (int)2, jsonString);

            // Receive and deserialize the response
            dynamic data = await Deserielizer.getResponse(ClientSocket.sock);
            GetPlayesInRoomResponse response = JsonSerializer.Deserialize<GetPlayesInRoomResponse>(data.jsonData);

            // Populate the user list, excluding the admin
            List<User> users = new List<User>();
            foreach (var username in response.players)
            {
                if (username != _currentRoom.AdminName)  // Exclude the admin
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
