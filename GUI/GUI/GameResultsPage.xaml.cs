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
            Serializer s = new Serializer();
            s.sendMessage(ClientSocket.sock, (int)GameRequestTypes.LeaderBoard, "");

            var data = Deserializer.getResponse(ClientSocket.sock);

            leaderBoardResponse response = JsonSerializer.Deserialize<leaderBoardResponse>(data.jsonData);

            var sortedPlayers = response.players.OrderByDescending(player => player.Value).ToList();

            WinnerLabel.Text += sortedPlayers[0].Key;
            if (sortedPlayers[0].Key == currentPlayerName)
            {
                WinnerLabel.TextColor = Colors.Red;
            }

            // Create a list of leaderboard entries
            var leaderboardEntries = new List<LeaderboardEntry>();
            for (int i = 0; i < sortedPlayers.Count; i++)
            {
                var entry = new LeaderboardEntry
                {
                    RankPlayer = $"{i + 1}.",
                    PlayerName = sortedPlayers[i].Key,
                    Score = $"Score: {sortedPlayers[i].Value}",
                    Color = sortedPlayers[i].Key == currentPlayerName ? Colors.Red : Colors.White
                };
                leaderboardEntries.Add(entry);
            }

            // Bind the ListView to the list of leaderboard entries
            LeaderboardListView.ItemsSource = leaderboardEntries;
        }

        void OnMainMenuButtonClicked(object sender, System.EventArgs e)
        {
            Navigation.PushAsync(new MainMenuPage());
        }
    }

    public class LeaderboardEntry
    {
        public string RankPlayer { get; set; }
        public string PlayerName { get; set; }
        public string Score { get; set; }
        public Color Color { get; set; }
    }
}
