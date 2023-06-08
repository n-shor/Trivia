using Microsoft.Maui.Controls;
using System;

namespace GUI
{
    public partial class StatisticsPage : ContentPage
    {
        public StatisticsPage()
        {
            InitializeComponent();
        }

        private async void OnPersonalStatisticsButtonClicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new PersonalStatisticsPage());
        }

        private async void OnHighScoresButtonClicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new HighScoresPage());
        }

        private async void OnBackButtonClicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new MainMenuPage());
        }

    }
}

