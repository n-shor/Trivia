using Microsoft.Maui.Controls;
using System.Text.Json;
using System.Collections.ObjectModel;
using System;
using System.Globalization;

namespace GUI
{
    public partial class HighScoresPage : ContentPage
    {
        public ObservableCollection<ScoreEntry> HighScores { get; set; }

        public HighScoresPage()
        {
            InitializeComponent();

            HighScores = new ObservableCollection<ScoreEntry>();
            HighScoresList.ItemsSource = HighScores;

            FetchHighScores();
        }

        private void FetchHighScores()
        {
            Serializer s = new Serializer();
            s.sendMessage(ClientSocket.sock, (int)MenuRequestTypes.GetHighScore, "");

            var data = Deserializer.getResponse(ClientSocket.sock);
            GetHighScoreResponse json = JsonSerializer.Deserialize<GetHighScoreResponse>(data.jsonData);

            // Use the deserialized high scores list to populate the ListView
            int rank = 1;
            foreach (var score in json.statistics)
            {
                HighScores.Add(new ScoreEntry { Rank = $"{rank++}.", UserName = " " + score });
            }
        }

        private void OnBackButtonClicked(object sender, System.EventArgs e)
        {
            Navigation.PushAsync(new StatisticsPage());
        }
    }

    public class ScoreEntry
    {
        public string Rank { get; set; }
        public string UserName { get; set; }
        public Color TextColor => (UserName == " " + ClientSocket.username) ? Colors.Red : Colors.White;
    }

    public class GetHighScoreResponse
    {
        public int status { get; set; }
        public ObservableCollection<string> statistics { get; set; }
    }
}
