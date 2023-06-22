using Microsoft.Maui.Controls;
using System;
using System.Threading;
using System.Threading.Tasks;

namespace GUI
{
    public partial class GamePage : ContentPage
    {
        private RoomData roomData;
        private GameData gameData;
        private CancellationTokenSource timerCts;
        private int remainingTime;
        private bool gameEnded = false;

        public GamePage(RoomData roomData)
        {
            InitializeComponent();
            this.roomData = roomData;
            StartGame();
        }

        private void StartGame()
        {
            // Fetch game data from the server
            FetchGameData();

            // Initialize and start the timer
            remainingTime = roomData.timePerQuestion;
            timerCts = new CancellationTokenSource();
            StartTimer();

            // Display the number of remaining questions
            RemainingQuestionsLabel.Text = $"Remaining questions: {roomData.numOfQuestionsInGame}";
        }

        private async void StartTimer()
        {
            while (remainingTime > 0 && !timerCts.Token.IsCancellationRequested)
            {
                await Task.Delay(1000);
                remainingTime--;
                RemainingTimeLabel.Text = "Time left to answer: " + remainingTime;
            }

            if (!timerCts.Token.IsCancellationRequested)
            {
                // If the timer wasn't cancelled, it means that the time is up
                // In this case, you might want to automatically send an answer to the server
                // You can decide what you want to do in this situation
            }
        }

        private void FetchGameData()
        {
            // Implement code to fetch game data from the server
            // For the purpose of this example, I'll initialize it with some dummy data
            gameData = new GameData
            {
                currentQuestion = new Question
                {
                    QuestionText = "Sample question",
                    PossibleAnswers = new List<string> { "Option A", "Option B", "Option C", "Option D" },
                    CorrectAnswerId = 0
                },
                correctAnswerCount = 0,
                wrongAnswerCount = 0,
                averageAnswerTime = 0
            };

            UpdateQuestion();
        }

        private void UpdateQuestion()
        {
            QuestionLabel.Text = gameData.currentQuestion.QuestionText;
            Option1Button.Text = gameData.currentQuestion.PossibleAnswers[0];
            Option2Button.Text = gameData.currentQuestion.PossibleAnswers[1];
            Option3Button.Text = gameData.currentQuestion.PossibleAnswers[2];
            Option4Button.Text = gameData.currentQuestion.PossibleAnswers[3];
        }

        private void OnOptionButtonClicked(object sender, EventArgs e)
        {
            // Cancel the timer
            timerCts.Cancel();

            // Determine which option was selected
            int selectedOption = 0;
            if (sender == Option1Button) selectedOption = 0;
            else if (sender == Option2Button) selectedOption = 1;
            else if (sender == Option3Button) selectedOption = 2;
            else if (sender == Option4Button) selectedOption = 3;

            // Send the selected option to the server
            // You'll need to implement this

            // Update correct answer count
            if (selectedOption == gameData.currentQuestion.CorrectAnswerId)
            {
                gameData.correctAnswerCount++;
                CorrectAnswersLabel.Text = $"Correct answers: {gameData.correctAnswerCount}";
            }

            // If game has ended, display the waiting message and navigate to the results page
            if (gameEnded)
            {
                WaitingLabel.Text = "Game has ended. Waiting for all players to finish...";
                // Wait for a while and then navigate to the results page
                // You'll need to decide when and how to navigate to the results page
            }
        }
    }
}
