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
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, (int)6, "");

            dynamic data = Deserielizer.getResponse(ClientSocket.sock);
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
