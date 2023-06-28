using Microsoft.Maui.Controls;
using System.Text.Json;
using System.Collections.Generic;


namespace GUI
{
    public partial class HighScoresPage : ContentPage
    {
        public HighScoresPage()
        {
            InitializeComponent();
            Serializer s = new Serializer();
            s.sendMessage(ClientSocket.sock, (int)MenuRequestTypes.GetHighScore, "");

            var data = Deserializer.getResponse(ClientSocket.sock);
            GetHighScoreResponse json = JsonSerializer.Deserialize<GetHighScoreResponse>(data.jsonData);

            // Use the deserialized high scores list to populate the ListView
            HighScoresList.ItemsSource = json.statistics;
        }

        private void OnBackButtonClicked(object sender, EventArgs e)
        {
            Navigation.PushAsync(new StatisticsPage());
        }
    }
}
