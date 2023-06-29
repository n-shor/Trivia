using Microsoft.Maui.Controls;
using System;
using System.Globalization;
using System.Text.Json;
using Microsoft.Maui.Graphics;
using System.Collections.ObjectModel;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Maui;

namespace GUI.Converters
{
    public class BoolToColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is bool && (bool)value)
            {
                return Colors.Red;
            }
            else
            {
                return Colors.Gray;
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}

namespace GUI
{
    public partial class JoinRoomPage : ContentPage
    {
        private RoomData _selectedRoom;
        private Frame _selectedFrame;
        SolidColorBrush darkRedBrush = new SolidColorBrush(Colors.DarkRed);
        SolidColorBrush darkGrayBrush = new SolidColorBrush(Colors.DarkGray);

        CancellationTokenSource cts;
        public JoinRoomPage()
        {
            InitializeComponent();
            LoadRooms();

            cts = new CancellationTokenSource();

            // Start the task to keep updating the rooms list
            Task.Run(async () =>
            {
                while (!cts.IsCancellationRequested)
                {
                    await MainThread.InvokeOnMainThreadAsync(() =>
                    {
                        LoadRooms();
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

        private void LoadRooms()
        {
            (int type, string jsonData) data;
            JoinButton.IsEnabled = false;
            JoinButton.BackgroundColor = Colors.DarkBlue;
            Serializer s = new Serializer();
            s.sendMessage(ClientSocket.sock, (int)MenuRequestTypes.GetRooms, "");

            data = Deserializer.getResponse(ClientSocket.sock);
            
            GetRoomsResponse json = JsonSerializer.Deserialize<GetRoomsResponse>(data.jsonData);

            // Clear the existing items
            RoomsStackLayout.Children.Clear();

            // Add the received rooms
            foreach (RoomData room in json.rooms)
            {
                var roomFrame = new Frame
                {
                    BorderColor = Colors.Black,
                    Content = new Label { Text = $"Room Name: {room.name}, Admin: {room.adminName}, Players: {room.currentPlayers}/{room.maxPlayers}" },
                    BackgroundColor = room.isActive != 0 ? Colors.Red : Colors.LightGray,
                };

                var tapGesture = new TapGestureRecognizer();
                tapGesture.Tapped += (s, e) => { OnRoomSelected(room, roomFrame); };
                roomFrame.GestureRecognizers.Add(tapGesture);

                RoomsStackLayout.Children.Add(roomFrame);

                // Re-select the previously selected room if it still exists
                if (_selectedRoom != null && room.id == _selectedRoom.id)
                {
                    OnRoomSelected(room, roomFrame);
                }
            }
        }

        private void OnRoomSelected(RoomData room, Frame selectedFrame)
        {
            // Reset color of previously selected frame
            if (_selectedFrame != null)
            {
                _selectedFrame.BackgroundColor = (_selectedRoom.isActive != 0) ? Colors.Red : Colors.LightGray;
            }

            _selectedRoom = room;
            _selectedFrame = selectedFrame;
            _selectedFrame.BackgroundColor = (_selectedRoom.isActive != 0) ? Colors.DarkRed : Colors.Gray;

            JoinButton.IsEnabled = true;
            JoinButton.BackgroundColor = Colors.MediumBlue;
        }

        private async void OnJoinButtonClicked(object sender, EventArgs e)
        {
            if (_selectedRoom == null)
            {
                return;
            }

            if (_selectedRoom.isActive != 0)
            {
                await DisplayAlert("Cannot join", "The selected room is currently active. Please select a different room.", "OK");
            }
            else
            {
                Serializer s = new Serializer();

                string jsonString = JsonSerializer.Serialize(new { roomId = _selectedRoom.id });
                s.sendMessage(ClientSocket.sock, (int)MenuRequestTypes.JoinRoom, jsonString);

                // Receive and handle the response from the server
                var data = Deserializer.getResponse(ClientSocket.sock);
                JoinRoomResponse response = JsonSerializer.Deserialize<JoinRoomResponse>(data.jsonData);

                if (response.status == (int)MenuRequestStatus.joinRoomSuccessful)
                {
                    // After successfully joining the room, navigate to RoomPage
                    await Navigation.PushAsync(new RoomPage(_selectedRoom, OriginPage.JoinRoomPage));
                }
                else
                {
                    await DisplayAlert("Cannot join", "The room is currently full. Please try again later.", "OK");
                }
            }
        }

        private void OnRefreshButtonClicked(object sender, EventArgs e)
        {
            LoadRooms();
        }

        private async void OnBackButtonClicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new MainMenuPage());
        }
    }
}
