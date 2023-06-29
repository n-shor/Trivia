using Microsoft.Maui.Controls;
using System.Text.Json;
using System.Collections.ObjectModel;
using System;
using System.Globalization;

namespace GUI
{
    public class HighScore
    {
        public string Rank { get; set; }
        public string UserName { get; set; }
        public Color TextColor { get; set; }
    }

    public partial class HighScoresPage : ContentPage
    {
        public ObservableCollection<HighScore> HighScores { get; set; }

        public HighScoresPage()
        {
            InitializeComponent();

            HighScores = new ObservableCollection<HighScore>();
            HighScoresList.ItemsSource = HighScores;

            FetchHighScores();
        }

        private void FetchHighScores()
        {
            Serializer s = new Serializer();
            s.sendMessage(ClientSocket.sock, (int)MenuRequestTypes.GetHighScore, "");

            var data = Deserializer.getResponse(ClientSocket.sock);
            GetHighScoreResponse json = JsonSerializer.Deserialize<GetHighScoreResponse>(data.jsonData);

            for (int i = 0; i < json.statistics.Count; i++)
            {
                var score = json.statistics[i];
                var name = score.Split(':')[0];
                var textColor = (name == ClientSocket.username) ? Colors.Red : Colors.White;
                HighScores.Add(new HighScore { Rank = (i + 1).ToString() + ". ", UserName = score, TextColor = textColor });
            }
        }

        private void OnBackButtonClicked(object sender, System.EventArgs e)
        {
            Navigation.PushAsync(new StatisticsPage());
        }
    }

    public class GetHighScoreResponse
    {
        public int status { get; set; }
        public ObservableCollection<string> statistics { get; set; }
    }
}
