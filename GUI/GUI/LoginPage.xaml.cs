using Microsoft.Maui.Controls;
using System;
using System.Text.Json;
using System.Net;
using System.Net.Sockets;

namespace GUI
{
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

            Serializer s = new Serializer();
            s.sendMessage(ClientSocket.sock, (int)1, jsonString);

            var data = Deserializer.getResponse(ClientSocket.sock);
            LoginResponse json = JsonSerializer.Deserialize<LoginResponse>(data.jsonData);

            // Now we check if the login was successful.
            if (json.status == (int)LoginRequestStatus.LoggedIn)
            {
                ClientSocket.username = username;
                Navigation.PushAsync(new MainMenuPage());
            }
            else
            {
                if(json.status == (int)LoginRequestStatus.AlreadyLoggedIn)
                {
                    //if account is already logged in
                    DisplayAlert("Login Failed", "The account you are attempting to log in as is already logged in.", "OK");
                }
                else
                {
                    // If login failed, display an alert
                    DisplayAlert("Login Failed", "Invalid username or password", "OK");
                }
            }
        }

        private async void OnBackButtonClicked(object sender, EventArgs e)
        {
            await Navigation.PushAsync(new StartPage());
        }

    }
}
