using Microsoft.Maui.Controls;
using System;

namespace GUI
{
    public partial class JoinRoomPage : ContentPage
    {
        public JoinRoomPage()
        {
            InitializeComponent();
        }

        private async void OnJoinButtonClicked(object sender, EventArgs e)
        {
            // TODO: Add your room joining logic here

            // After joining the room, navigate to RoomPage
            await Navigation.PushAsync(new RoomPage());
        }
    }
}
