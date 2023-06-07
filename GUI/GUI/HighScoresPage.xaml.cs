using Microsoft.Maui.Controls;
using System.Text.Json;

public class getHighScoreResponse
{
    int status;
    List<string> statistics;
};

namespace GUI
{
    public partial class HighScoresPage : ContentPage
    {
        public HighScoresPage()
        {
            InitializeComponent();
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock,
                                   (int)6,
                                   "");



            dynamic data = Deserielizer.getResponse(ClientSocket.sock);
            GetHighScoreResponse json = JsonSerializer.Deserialize<GetHighScoreResponse>(data.jsonData);

            //use json
        }
    }
}
