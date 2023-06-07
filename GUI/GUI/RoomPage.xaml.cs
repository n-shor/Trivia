using Microsoft.Maui.Controls;
using System;

namespace GUI
{
    public partial class RoomPage : ContentPage
    {
        public RoomPage()
        {
            InitializeComponent();
        }

        public RoomPage(string roomName, string adminName, string[] users)
        {
            InitializeComponent();

            RoomNameLabel.Text += roomName;
            AdminNameLabel.Text += adminName;

            UsersListView.ItemsSource = users;
        }
    }
}
