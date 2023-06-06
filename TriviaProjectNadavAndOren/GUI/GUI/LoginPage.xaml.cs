using Microsoft.Maui.Controls;
using System;
using System.Text.RegularExpressions;
using System.Text.Json;
using System.Net;
using System.Net.Sockets;
using System.Text;
enum Statuses { Error = -1, FailedLogin = 21, UserAlreadyExists = 22, LoggedIn = 11, SignedUp = 12, NoSuchLoggedUser = 23, LoggedOut = 13 };

namespace GUI
{
    public partial class LoginPage : ContentPage
    {
        public LoginPage()
        {
            InitializeComponent();
            IPHostEntry ipHost = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress ipAddr = System.Net.IPAddress.Parse("127.0.0.1");
            IPEndPoint localEndPoint = new IPEndPoint(ipAddr, 8080); // choose port and ip here
            Socket sender = new Socket(ipAddr.AddressFamily,
                   SocketType.Stream, ProtocolType.Tcp);
            sender.Connect(localEndPoint);
            ClientSocket.sock = sender;
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
    }
}
