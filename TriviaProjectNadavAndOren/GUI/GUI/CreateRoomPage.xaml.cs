using Microsoft.Maui.Controls;
using System;
using System.Text.Json;

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
            var l = new CreateRoomRequest { roomName = RoomNameEntry.Text, maxUsers = int.Parse(PlayersNumberEntry.Text), answerTimeout = int.Parse(TimeQuestionEntry.Text), questionCount  = int.Parse(questionCountEntry.Text) };
            string jsonString = JsonSerializer.Serialize(l);
            // If login is successful (for testing, we're ignoring login verification and always navigating to MainMenuPage)
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock,
                                   (int)0,
                                   jsonString);
            dynamic data = Deserielizer.getResponse(ClientSocket.sock);

            CreateRoomResponse json = JsonSerializer.Deserialize<CreateRoomResponse>(data.jsonData);
            // After the room is created, navigate to RoomPage
            await Navigation.PushAsync(new RoomPage());
        }
    }
}
