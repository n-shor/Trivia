using Microsoft.Maui.Controls;
using System;

namespace GUI
{
    public partial class CreateRoomPage : ContentPage
    {
        public CreateRoomPage()
        {
            InitializeComponent();
        }

        private async void OnCreateButtonClicked(object sender, EventArgs e)
        {
            // TODO: Add your room creation logic here

            // After the room is created, navigate to RoomPage
            await Navigation.PushAsync(new RoomPage());
        }
    }
}
