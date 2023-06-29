using Microsoft.Maui.Controls;
using System;
using System.Collections.Generic;
using System.Text.Json;
using Microsoft.Maui.Graphics;
using Microsoft.Maui.Controls;
using System.Globalization;
using System.Threading.Tasks;

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
        public Color TextColor { get; set; }

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
        private bool _hasGameBegun;
        private int _questionCount;
        private int _answerTimeout;

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

            StartUpdating();
        }

        CancellationTokenSource cts;
        private void StartUpdating()
        {
            cts = new CancellationTokenSource();
            Task.Run(async () =>
            {
                while (!cts.IsCancellationRequested && !_hasGameBegun)
                {
                    await MainThread.InvokeOnMainThreadAsync(() =>
                    {
                        updateRoomData();
                        return Task.CompletedTask;
                    });

                    await Task.Delay(3000);
                }
            });
        }


        protected override void OnDisappearing()
        {
            base.OnDisappearing();
            cts.Cancel();
        }

        private async void updateRoomData()
        {
            var request = new GetPlayersInRoomRequest { roomId = _currentRoom.id };
            var jsonString = JsonSerializer.Serialize(request);

            Serializer s = new Serializer();
            s.sendMessage(ClientSocket.sock, (int)RoomMemberRequestTypes.GetRoomsState, jsonString);

            (int type, string jsonData) data;


            data = Deserializer.getResponse(ClientSocket.sock);

            if (data.jsonData != "{\"message\":\"irrelevant message\"}")
            {
                try
                {
                    ErrorResponse errorResponse = JsonSerializer.Deserialize<ErrorResponse>(data.jsonData);

                    if (errorResponse.message == "room closed")
                    {
                        await MainThread.InvokeOnMainThreadAsync(async () =>
                        {
                            await Navigation.PushAsync(new MainMenuPage());
                            await DisplayAlert("Room Closed", "The room has been closed by the admin.", "OK");
                        });
                        return;
                    }
                }
                catch (JsonException)
                {
                }

                GetRoomStateResponse response = JsonSerializer.Deserialize<GetRoomStateResponse>(data.jsonData);

                List<User> users = new List<User>();
                foreach (var username in response.players)
                {
                    if (username != _currentRoom.adminName)
                    {
                        users.Add(new User
                        {
                            Username = username,
                            TextColor = username == _currentUser ? Colors.Red : Colors.White
                        });
                    }
                }

                await MainThread.InvokeOnMainThreadAsync(() =>
                {
                    UsersListView.ItemsSource = users;
                });

                _hasGameBegun = response.hasGameBegun;
                _questionCount = response.questionCount;
                _answerTimeout = response.answerTimeout;

                if (_hasGameBegun)
                {
                    cts.Cancel(); // cancel the auto refresher
                    await MainThread.InvokeOnMainThreadAsync(async () =>
                    {
                        await DisplayAlert("Game Started", "The admin has started the game.", "OK");
                    });
                    await Navigation.PushAsync(new GamePage(_currentRoom, _currentUser));
                }

            }
        }

        private async void OnStartGameButtonClicked(object sender, EventArgs e)
        {
            Serializer s = new Serializer();
            s.sendMessage(ClientSocket.sock, (int)RoomAdminRequestTypes.StartGame, "");

            var data = Deserializer.getResponse(ClientSocket.sock);
            StartGameResponse response = JsonSerializer.Deserialize<StartGameResponse>(data.jsonData);

            if (response.status == (int)RoomAdminRequeststatus.startGameSuccessful)
            {
                await Navigation.PushAsync(new GamePage(_currentRoom, _currentUser));
            }
            else if (response.status == (int)RoomAdminRequeststatus.theServerDoesntHaveEnoughQuestions)
            {
                await DisplayAlert("Game Start Failed", "The database does not contain enough questions for the game. Please create a new room with fewer questions.", "OK");
            }
            else
            {
                await DisplayAlert("Game Start Failed", "Unable to start the game, please try again later.", "OK");
            }
        }

        private async void OnCloseRoomButtonClicked(object sender, EventArgs e)
        {
            Serializer s = new Serializer();
            s.sendMessage(ClientSocket.sock, (int)RoomAdminRequestTypes.CloseRoom, "");

            var data = Deserializer.getResponse(ClientSocket.sock);
            CloseRoomResponse response = JsonSerializer.Deserialize<CloseRoomResponse>(data.jsonData);

            if (response.status == (int)RoomAdminRequeststatus.closeRoomSuccessful)
            {
                await Navigation.PushAsync(new MainMenuPage());
            }
            else
            {
                await DisplayAlert("Room Close Failed", "Unable to close the room, please try again later.", "OK");
            }
        }

        private async void OnLeaveRoomButtonClicked(object sender, EventArgs e)
        {
            Serializer s = new Serializer();
            s.sendMessage(ClientSocket.sock, (int)RoomMemberRequestTypes.LeaveRoom, "");

            var data = Deserializer.getResponse(ClientSocket.sock);
            LeaveRoomResponse response = JsonSerializer.Deserialize<LeaveRoomResponse>(data.jsonData);

            if (response.status == (int)RoomMemberRequeststatus.leaveRoomSuccessful)
            {
                await Navigation.PushAsync(new MainMenuPage());
            }
            else
            {
                await DisplayAlert("Room Leave Failed", "Unable to leave the room, please try again later.", "OK");
            }
        }
    }
}
