using Microsoft.Maui.Controls;
using System;
using System.Text.Json;

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
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock,
                                   (int)1,
                                   "");
            dynamic data = Deserielizer.getResponse(ClientSocket.sock);

            GetRoomsResponse json = JsonSerializer.Deserialize<GetRoomsResponse>(data.jsonData);

            //contiue here using rooms from json

            // After joining the room, navigate to RoomPage
            await Navigation.PushAsync(new RoomPage());
        }
    }
}
