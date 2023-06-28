﻿using Microsoft.Maui.Controls;
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

            Serializer s = new Serializer();
            s.sendMessage(ClientSocket.sock, (int)MenuRequestTypes.CreateRoom, jsonString);

            var data = Deserializer.getResponse(ClientSocket.sock);

            CreateRoomResponse response = JsonSerializer.Deserialize<CreateRoomResponse>(data.jsonData);

            if (response.status != (int)MenuRequestStatus.CreateRoomSuccessful)
            {
                if (response.status == (int)RoomAdminRequeststatus.theServerDoesntHaveEnoughQuestions)
                {
                    await DisplayAlert("Room Creation Failed", "There are not enough questions in the database. Please try again with a smaller number of questions.", "OK");
                }
                else
                {
                    await DisplayAlert("Room Creation Failed", "The room could not be created. Please try again.", "OK");
                }
                return;
            }

            RoomData createdRoom = new RoomData
            {
                name = RoomNameEntry.Text,
                currentPlayers = 1, // as the room got just created, only admin is there.
                adminName = response.adminName,
                id = response.roomId,
                maxPlayers = maxUsers,
                numOfQuestionsInGame = questionCount,
                timePerQuestion = answerTimeout,
                isActive = 0,
            };

            await Navigation.PushAsync(new RoomPage(createdRoom, OriginPage.CreateRoomPage));

        }

        private async void OnBackButtonClicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new MainMenuPage());
        }


    }
}
