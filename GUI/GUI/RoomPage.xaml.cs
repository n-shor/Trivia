using Microsoft.Maui.Controls;
using System;
using System.Collections.Generic;
using System.Text.Json;
using Microsoft.Maui.Graphics;
using Microsoft.Maui.Controls;
using System.Globalization;


namespace GUI
{

    public class BoolToColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is bool isCurrentUser && isCurrentUser)
            {
                return Colors.Red;
            }
            else
            {
                return Colors.White;
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }

    public class User
    {
        public string Username { get; set; }
        public Color TextColor { get; set; } // New property

        public override string ToString() => Username;
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
        private string _currentUser;

        public RoomPage(RoomData room, OriginPage originPage)
        {
            InitializeComponent();

            _currentRoom = room;
            _originPage = originPage;
            _currentUser = ClientSocket.username;

            RoomNameLabel.Text += _currentRoom.name;
            AdminNameLabel.Text += _currentRoom.adminName;

            if (_currentUser == _currentRoom.adminName)
            {
                AdminNameLabel.TextColor = Colors.Red;
                StartGameButton.IsVisible = true;
                CloseRoomButton.IsVisible = true;
                LeaveRoomButton.IsVisible = false;
            }
            else
            {
                LeaveRoomButton.IsVisible = true;
                StartGameButton.IsVisible = false;
                CloseRoomButton.IsVisible = false;
            }

            updateUserList();
        }

        private void updateUserList()
        {
            var request = new GetPlayersInRoomRequest { roomId = _currentRoom.id };
            var jsonString = JsonSerializer.Serialize(request);

            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, (int)2, jsonString);

            dynamic data = Deserielizer.getResponse(ClientSocket.sock);
            GetPlayersInRoomResponse response = JsonSerializer.Deserialize<GetPlayersInRoomResponse>(data.jsonData);

            List<User> users = new List<User>();
            foreach (var username in response.players)
            {
                if (username != _currentRoom.adminName)
                {
                    users.Add(new User
                    {
                        Username = username,
                        TextColor = username == _currentUser ? Colors.Red : Colors.White // set TextColor
                    });
                }
            }

            UsersListView.ItemsSource = users;
        }

        private void OnRefreshButtonClicked(object sender, EventArgs e)
        {
            updateUserList();
        }

        private void OnStartGameButtonClicked(object sender, EventArgs e)
        {
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, 2, "");
            DisplayAlert("Game Started", "The admin has started the game.", "OK");
        }

        private async void OnCloseRoomButtonClicked(object sender, EventArgs e)
        {
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, 0, "");
            await Navigation.PushAsync(new MainMenuPage());
            await DisplayAlert("Room Closed", "The room has been closed by the admin.", "OK");
        }

        private async void OnLeaveRoomButtonClicked(object sender, EventArgs e)
        {
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, 0, "");
            await Navigation.PushAsync(new MainMenuPage());
        }
    }
}
