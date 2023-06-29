using Microsoft.Maui.Controls;
using System;
using System.Net.Sockets;
using System.Net;
using System.Text.Json;

namespace GUI
{
    public partial class StartPage : ContentPage
    {
        static bool firstTime = true;
        public StartPage()
        {
            InitializeComponent();

            if (!firstTime) //make sure to not run the connection code twice
                return;

            IPHostEntry ipHost = Dns.GetHostEntry(Dns.GetHostName());
            IPAddress ipAddr = System.Net.IPAddress.Parse("127.0.0.1");
            IPEndPoint localEndPoint = new IPEndPoint(ipAddr, 8080); // choose port and ip here
            Socket sender = new Socket(ipAddr.AddressFamily,
                   SocketType.Stream, ProtocolType.Tcp);
            sender.Connect(localEndPoint);
            ClientSocket.sock = sender;

            firstTime = false;
        }

        private async void OnLoginButtonClicked(object sender, EventArgs e)
        {
            // Navigate to the Login Page
            await Navigation.PushAsync(new LoginPage());
        }

        private async void OnSignupButtonClicked(object sender, EventArgs e)
        {
            // Navigate to the Signup Page
            await Navigation.PushAsync(new SignupPage());
        }

        private void OnExitButtonClicked(object sender, EventArgs e)
        {
            // Send the exit message to the server
            Serializer s = new Serializer();
            s.sendMessage(ClientSocket.sock, (int)3, "end");

            // Exit the application
            System.Environment.Exit(0);
        }

    }
}
