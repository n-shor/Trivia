using Microsoft.Maui.Controls;
using System;

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
            // TODO: Implement logout logic here
            // After logging out, navigate back to LoginPage
            Navigation.PushAsync(new LoginPage());
        }
    }
}
