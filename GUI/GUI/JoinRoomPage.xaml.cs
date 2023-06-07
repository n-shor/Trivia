using Microsoft.Maui.Controls;
using System;
using System.Globalization;
using System.Text.Json;
using Microsoft.Maui.Graphics;

namespace GUI.Converters
{
    public class BoolToColorConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is bool && (bool)value)
            {
                return new SolidColorBrush(Colors.Red);
            }
            else
            {
                return new SolidColorBrush(Colors.Grey);
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
    public class Room
    {
        public string RoomName { get; set; }
        public int PlayerCount { get; set; }
        public string AdminName { get; set; }
        public int Id { get; set; }
        public bool IsActive { get; set; }

        // This property combines the room name and player count into a single string for display purposes
        public string DisplayText => $"{RoomName} (Players: {PlayerCount})";
    }


    public partial class JoinRoomPage : ContentPage
    {
        private Room _selectedRoom;
        public JoinRoomPage()
        {
            InitializeComponent();
            LoadRooms();
        }

        private async void LoadRooms()
        {
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock,
                                   (int)1,
                                   "");
            dynamic data = await Deserielizer.getResponse(ClientSocket.sock);

            GetRoomsResponse json = JsonSerializer.Deserialize<GetRoomsResponse>(data.jsonData);

            // Update the ListView with the received rooms
            RoomsListView.ItemsSource = json.rooms;
        }

        private void OnRoomSelected(object sender, SelectedItemChangedEventArgs e)
        {
            // Save the selected room
            _selectedRoom = e.SelectedItem as Room;

            // Enable the Join button
            JoinButton.IsEnabled = _selectedRoom != null;
        }

        private async void OnJoinButtonClicked(object sender, EventArgs e)
        {
            if (_selectedRoom == null)
            {
                return;
            }

            if (_selectedRoom.IsActive)
            {
                await DisplayAlert("Cannot join", "The selected room is currently active. Please select a different room.", "OK");
            }
            else
            {
                Serielizer s = new Serielizer();

                string jsonString = JsonSerializer.Serialize(new { roomId = _selectedRoom.Id });
                s.sendMessage(ClientSocket.sock, (int)3, jsonString);

                // After joining the room, navigate to RoomPage
                await Navigation.PushAsync(new RoomPage(_selectedRoom));
            }
        }

    }
}
