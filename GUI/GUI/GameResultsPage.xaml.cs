using Microsoft.Maui.Controls;
using System.Collections.Generic;
using System.Linq;
using System.Text.Json;

namespace GUI
{
    public partial class GameResultsPage : ContentPage
    {
        private string currentPlayerName;

        public GameResultsPage(string playerName)
        {
            InitializeComponent();
            currentPlayerName = playerName;
            FetchAndDisplayLeaderboardData();
        }

        private void FetchAndDisplayLeaderboardData()
        {
            // Send request for leaderboard data
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, (int)GameRequestTypes.LeaderBoard, "");

            // Get response
            var data = Deserielizer.getResponse(ClientSocket.sock);

            // Deserialize response into a leaderboard response object
            leaderBoardResponse response = JsonSerializer.Deserialize<leaderBoardResponse>(data.jsonData);

            // Sort the players by their scores in descending order
            var sortedPlayers = response.players.OrderByDescending(player => player.Value).ToDictionary(player => player.Key, player => player.Value);

            // Display the leaderboard
            foreach (var player in sortedPlayers)
            {
                var playerName = new Label
                {
                    Text = player.Key,
                    TextColor = player.Key == currentPlayerName ? Colors.Red : Colors.White
                };

                var playerScore = new Label
                {
                    Text = player.Value.ToString(),
                };

                LeaderBoardStackLayout.Children.Add(new StackLayout
                {
                    Orientation = StackOrientation.Horizontal,
                    Children = { playerName, playerScore }
                });
            }
        }

        void OnMainMenuButtonClicked(object sender, System.EventArgs e)
        {
            Application.Current.MainPage = new MainMenuPage();
        }
    }
}
