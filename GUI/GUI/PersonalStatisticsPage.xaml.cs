using Microsoft.Maui.Controls;
using System.Text.Json;
using System.Collections.Generic;

public class GetPersonalStatsResponse
{
    public int status { get; set; }
    public List<string> statistics { get; set; }
}

namespace GUI
{
    public partial class PersonalStatisticsPage : ContentPage
    {
        public PersonalStatisticsPage()
        {
            InitializeComponent();
            Serializer s = new Serializer();
            s.sendMessage(ClientSocket.sock, (int)MenuRequestTypes.GetStatistics, "");

            var data = Deserializer.getResponse(ClientSocket.sock);
            GetPersonalStatsResponse json = JsonSerializer.Deserialize<GetPersonalStatsResponse>(data.jsonData);

            // Use the deserialized personal statistics list to populate the ListView
            PersonalStatsList.ItemsSource = json.statistics;
        }

        private async void OnBackButtonClicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new StatisticsPage());
        }
    }
}
