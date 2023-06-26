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

        private async void OnLogoutButtonClicked(object sender, EventArgs e)
        {
            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock,
                                   (int)MenuRequestTypes.Logout,
                                   "");
            
            var data = Deserielizer.getResponse(ClientSocket.sock);
            LoginResponse json = JsonSerializer.Deserialize<LoginResponse>(data.jsonData);

            // Now we check if the logout was successful.
            if (json.status == (int)MenuRequestStatus.signedOut)
            {
                await Navigation.PushAsync(new LoginPage());
            }
            else
            {
                // If login failed, display an alert
                await DisplayAlert("Alert", "Logout Failed", "OK");
            }
        }
        
    }
}
