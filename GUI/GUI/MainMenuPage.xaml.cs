using Microsoft.Maui.Controls;
using System;
using System.Text.Json;

namespace GUI
{
    public partial class MainMenuPage : ContentPage
    {
        public MainMenuPage()
        {
            InitializeComponent();
        }

        private void OnCreateRoomButtonClicked(object sender, EventArgs e)
        {
            Navigation.PushAsync(new CreateRoomPage());
        }

        private void OnJoinRoomButtonClicked(object sender, EventArgs e)
        {
            Navigation.PushAsync(new JoinRoomPage());
        }

        private void OnStatisticsButtonClicked(object sender, EventArgs e)
        {
            Navigation.PushAsync(new StatisticsPage());
        }

        private void OnLogoutButtonClicked(object sender, EventArgs e)
        {
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock,
                                   (int)5,
                                   "");
            
            dynamic data = Deserielizer.getResponse(ClientSocket.sock);
            LoginResponse json = JsonSerializer.Deserialize<LoginResponse>(data.jsonData);

            // Now we check if the logout was successful.
            if (json.status == 0)
            {
                Navigation.PushAsync(new LoginPage());
            }
            else
            {
                // If login failed, display an alert
                DisplayAlert("Alert", "Logout Failed", "OK");
            }
        }
        
    }
}
