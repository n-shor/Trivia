using Microsoft.Maui.Controls;
using System;
using System.Text.Json;
using System.Net;
using System.Net.Sockets;
using System.Text.RegularExpressions;

namespace GUI
{
    public partial class SignupPage : ContentPage
    {
        public SignupPage()
        {
            InitializeComponent();
        }

        private void OnRegisterButtonClicked(object sender, EventArgs e)
        {
            string username = UsernameEntry.Text;
            string password = PasswordEntry.Text;
            string email = EmailEntry.Text;

            if (string.IsNullOrEmpty(username) || string.IsNullOrEmpty(password) || string.IsNullOrEmpty(email))
            {
                DisplayAlert("Signup Failed", "Please fill out all of the relevant fields.", "OK");
                return;
            }

            if (IsValidEmail(email))
            {
                var l = new SignupRequest { username = username, password = password, email = email };
                string jsonString = JsonSerializer.Serialize(l);

                Serielizer s = new Serielizer();
                s.sendMessage(ClientSocket.sock, (int)2, jsonString);

                dynamic data = Deserielizer.getResponse(ClientSocket.sock);
                SignupResponse json = JsonSerializer.Deserialize<SignupResponse>(data.jsonData);

                // Now we check if the registration was successful.
                if (json.status == (int)Statuses.SignedUp) // Assuming that status code 1 means success.
                {
                    ClientSocket.username = username;
                    Navigation.PushAsync(new MainMenuPage());
                }
                else
                {
                    // If registration failed (user already exists), display an alert
                    DisplayAlert("Alert", "User already exists", "OK");
                }
            }
            else
            {
                // If email is invalid, display an alert
                DisplayAlert("Alert", "Invalid email address", "OK");
            }
        }

        private bool IsValidEmail(string email)
        {
            // A simple email validation.
            if (string.IsNullOrEmpty(email))
            {
                return false;
            }

            try
            {
                return (new System.Net.Mail.MailAddress(email)).Address == email;
            }
            catch
            {
                return false;
            }
        }

        private void OnBackButtonClicked(object sender, EventArgs e)
        {
            // Navigate back to the starting page
            Navigation.PopAsync();
        }

    }
}
