using Microsoft.Maui.Controls;
using System;
using System.Text.RegularExpressions;

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

            // TODO: Add your server communication code here to send and validate login info.

            // If login is successful (for testing, we're ignoring login verification and always navigating to MainMenuPage)
            Navigation.PushAsync(new MainMenuPage());
        }


        private void OnRegisterButtonClicked(object sender, EventArgs e)
        {
            string username = UsernameEntry.Text;
            string password = PasswordEntry.Text;
            string email = EmailEntry.Text;

            if (IsValidEmail(email))
            {
                // TODO: Add your server communication code here to send registration info.

                // If registration is successful (for testing, we're ignoring registration verification and always navigating to MainMenuPage)
                Navigation.PushAsync(new MainMenuPage());
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
    }
}
