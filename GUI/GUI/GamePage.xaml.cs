using Microsoft.Maui.Controls;
using System;
using System.Threading;
using System.Threading.Tasks;
using System.Text.Json;

namespace GUI
{
    public partial class GamePage : ContentPage
    {
        private RoomData roomData;
        private CancellationTokenSource timerCts;
        private CancellationTokenSource gameEndCheckCts;
        private int remainingTime;
        private int correctAnswerCount = 0;
        private int answeredQuestionCount = 0;
        private String playerName;
        public GamePage(RoomData roomData, String playerName)
        {
            InitializeComponent();
            this.roomData = roomData;
            this.playerName = playerName;
            StartGame();
        }

        private void StartGame()
        {
            remainingTime = roomData.timePerQuestion;
            timerCts = new CancellationTokenSource();

            RemainingQuestionsLabel.Text = $"Remaining questions: {roomData.numOfQuestionsInGame}";

            FetchQuestionData();
        }

        private void FetchQuestionData()
        {
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, (int)GameRequestTypes.getQuestionReq, "");

            var data = Deserielizer.getResponse(ClientSocket.sock);

            if (data.jsonData != "{\"message\":\"irrelevant message\"}")
            {
                timerCts?.Cancel();
                GetQuestionResponse response = JsonSerializer.Deserialize<GetQuestionResponse>(data.jsonData);

                QuestionLabel.Text = response.question;
                Option1Button.Text = response.answers[0];
                Option2Button.Text = response.answers[1];
                Option3Button.Text = response.answers[2];
                Option4Button.Text = response.answers[3];

                StartTimer();
            }
            else
            {
                FetchQuestionData();
            }
        }


        private async void StartTimer()
        {
            timerCts = new CancellationTokenSource();
            remainingTime = roomData.timePerQuestion;

            try
            {
                while (remainingTime > 0)
                {
                    await Task.Delay(1000, timerCts.Token);
                    remainingTime--;
                    RemainingTimeLabel.Text = $"Time left to answer: {remainingTime}";
                }

                // The time is up. Send an answer with id 5 which is 100% wrong
                SubmitAnswer(5);
            }
            catch (OperationCanceledException)
            {
                // The timer was cancelled
            }
        }



        private void OnOptionButtonClicked(object sender, EventArgs e)
        {
            timerCts.Cancel();

            uint selectedOption = 0;
            if (sender == Option1Button) selectedOption = 0;
            else if (sender == Option2Button) selectedOption = 1;
            else if (sender == Option3Button) selectedOption = 2;
            else if (sender == Option4Button) selectedOption = 3;

            SubmitAnswer(selectedOption);
        }

        private void SubmitAnswer(uint selectedOption)
        {
            var request = new SubmitAnswerRequest { answerId = selectedOption };
            var jsonString = JsonSerializer.Serialize(request);

            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, (int)GameRequestTypes.submitAnswerReq, jsonString);

            var data = Deserielizer.getResponse(ClientSocket.sock);

            SubmitAnswerResponse response = JsonSerializer.Deserialize<SubmitAnswerResponse>(data.jsonData);

            if (response.status == (int)SubmitAnswerStatus.correctAnswer)
            {
                correctAnswerCount++;
                CorrectAnswersLabel.Text = $"Correct answers: {correctAnswerCount}";
            }
            answeredQuestionCount++;
            RemainingQuestionsLabel.Text = $"Remaining questions: {roomData.numOfQuestionsInGame - answeredQuestionCount}";

            if (answeredQuestionCount < roomData.numOfQuestionsInGame)
            {
                FetchQuestionData();
            }
            else
            {
                // Start the game end check thread
                CheckIfGameHasEnded();
            }

        }

        private void CheckIfGameHasEnded()
        {
            gameEndCheckCts = new CancellationTokenSource();
            Task.Run(async () =>
            {
                while (!gameEndCheckCts.IsCancellationRequested)
                {
                    Serielizer s = new Serielizer();
                    s.sendMessage(ClientSocket.sock, (int)GameRequestTypes.checkForEnd, "");

                    var data = Deserielizer.getResponse(ClientSocket.sock);

                    CheckForEndReponse response = JsonSerializer.Deserialize<CheckForEndReponse>(data.jsonData);

                    if (response.gameEnded == 1)
                    {
                        gameEndCheckCts.Cancel();
                        await Navigation.PushAsync(new GameResultsPage(playerName));
                    }

                    // Wait for 3 seconds before the next check
                    await Task.Delay(3000);
                }
            }, gameEndCheckCts.Token);
        }
    }
}
