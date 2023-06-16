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

        // New properties for GetRoomStateResponse
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

            // Start the task to keep updating the room data
            StartUpdating();
        }

        CancellationTokenSource cts;
        private void StartUpdating()
        {
            cts = new CancellationTokenSource();
            // Start the task to keep updating the room data
            Task.Run(async () =>
            {
                while (!cts.IsCancellationRequested)
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

            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, 1, jsonString);

            dynamic data = Deserielizer.getResponse(ClientSocket.sock);

            // Attempt to deserialize to ErrorResponse and check for a message property
            try
            {
                ErrorResponse errorResponse = JsonSerializer.Deserialize<ErrorResponse>(data.jsonData);

                // If the error message indicates the room is closed, handle it accordingly
                if (errorResponse.message == "room closed")
                {
                    // Handle room closing
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
                // If deserialization to ErrorResponse failed, it means it's not an ErrorResponse
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

            // Update the new properties
            _hasGameBegun = response.hasGameBegun;
            _questionCount = response.questionCount;
            _answerTimeout = response.answerTimeout;

            if (_hasGameBegun)
            {
                await MainThread.InvokeOnMainThreadAsync(async () =>
                {
                    await DisplayAlert("Game Started", "The admin has started the game.", "OK");
                });
                //Navigation.PushAsync(new GamePage());
            }
        }


        private void OnStartGameButtonClicked(object sender, EventArgs e)
        {
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, 2, "");
            //Navigation.PushAsync(new GamePage());
        }

        private async void OnCloseRoomButtonClicked(object sender, EventArgs e)
        {
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, 0, "");
            await Navigation.PushAsync(new MainMenuPage());
        }

        private async void OnLeaveRoomButtonClicked(object sender, EventArgs e)
        {
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, 0, "");
            await Navigation.PushAsync(new MainMenuPage());
        }
    }
}
