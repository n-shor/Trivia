using Microsoft.Maui.Controls;
using System.Text.Json;

namespace GUI
{
    public partial class PersonalStatisticsPage : ContentPage
    {
        public PersonalStatisticsPage()
        {
            InitializeComponent();
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock,
                                   (int)4,
                                   "");
            dynamic data = Deserielizer.getResponse(ClientSocket.sock);

            getPersonalStatsResponse json = JsonSerializer.Deserialize<getPersonalStatsResponse>(data.jsonData);

            //use json
        }
    }
}
