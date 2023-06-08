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
            if (string.IsNullOrWhiteSpace(RoomNameEntry.Text) || string.IsNullOrWhiteSpace(TimeQuestionEntry.Text) ||
                string.IsNullOrWhiteSpace(QuestionCountEntry.Text) || string.IsNullOrWhiteSpace(PlayersNumberEntry.Text))
            {
                await DisplayAlert("Error", "All fields must be filled out.", "OK");
                return;
            }

            if (!int.TryParse(TimeQuestionEntry.Text, out int answerTimeout) || !int.TryParse(QuestionCountEntry.Text, out int questionCount) ||
                !int.TryParse(PlayersNumberEntry.Text, out int maxUsers))
            {
                await DisplayAlert("Error", "Please enter valid numbers in numeric fields.", "OK");
                return;
            }

            if (answerTimeout <= 0 || questionCount <= 0 || maxUsers <= 0)
            {
                await DisplayAlert("Error", "Please enter positive numbers.", "OK");
                return;
            }

            var request = new CreateRoomRequest
            {
                roomName = RoomNameEntry.Text,
                maxUsers = maxUsers,
                answerTimeout = answerTimeout,
                questionCount = questionCount
            };

            string jsonString = JsonSerializer.Serialize(request);

            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, (int)0, jsonString);

            dynamic data = await Deserielizer.getResponse(ClientSocket.sock);

            CreateRoomResponse response = JsonSerializer.Deserialize<CreateRoomResponse>(data.jsonData);

            if (response.status != 1)
            {
                await DisplayAlert("Error", "Room could not be created. Please try again.", "OK");
                return;
            }

            Room createdRoom = new Room
            {
                RoomName = RoomNameEntry.Text,
                PlayerCount = 1, // as the room just created, only admin is there.
                AdminName = response.adminName, // put the username here.
                Id = response.roomId // you need to include the roomId in the response.
            };

            await Navigation.PushAsync(new RoomPage(createdRoom, OriginPage.CreateRoomPage));

        }

        private async void OnBackButtonClicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new MainMenuPage());
        }


    }
}
