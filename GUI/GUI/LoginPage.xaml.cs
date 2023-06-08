using Microsoft.Maui.Controls;
using System;
using System.Text.Json;
using System.Net;
using System.Net.Sockets;

namespace GUI
{
    enum Statuses { Error = -1, FailedLogin = 21, UserAlreadyExists = 22, LoggedIn = 11, SignedUp = 12, NoSuchLoggedUser = 23, LoggedOut = 13 };
    public partial class LoginPage : ContentPage
    {
        public LoginPage()
        {
            InitializeComponent();
        }

        private void OnLoginButtonClicked(object sender, EventArgs e)
        {
            string username = UsernameEntry.Text;
            string password = PasswordEntry.Text;

            if (string.IsNullOrEmpty(username) || string.IsNullOrEmpty(password))
            {
                DisplayAlert("Login Failed", "Please fill out all of the relevant fields.", "OK");
                return;
            }

            var l = new LoginRequest { username = username, password = password };
            string jsonString = JsonSerializer.Serialize(l);

            Serielizer s = new Serielizer();
            s.sendMessage(ClientSocket.sock, (int)1, jsonString);

            dynamic data = Deserielizer.getResponse(ClientSocket.sock);
            LoginResponse json = JsonSerializer.Deserialize<LoginResponse>(data.jsonData);

            // Now we check if the login was successful.
            if (json.status == (int)Statuses.LoggedIn)
            {
                ClientSocket.username = username;
                Navigation.PushAsync(new MainMenuPage());
            }
            else
            {
                // If login failed, display an alert
                DisplayAlert("Alert", "Invalid username or password", "OK");
            }
        }

        private async void OnBackButtonClicked(object sender, EventArgs e)
        {
            // Navigate back to the starting page
            await Navigation.PopAsync();
        }

    }
}
