using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace GUI
{
    public class Serielizer
    {
        public void sendMessage(Socket clientSocket, int type, string buffer)
        {
            // Calculate the length of the message and pad it with zeroes if necessary.
            string length = buffer.Length.ToString().PadLeft(4, '0');

            // Create the message by concatenating the type, the length, and the actual buffer.
            string msg = type.ToString() + length + buffer;

            // Convert the message to bytes and send it.
            clientSocket.Send(Encoding.ASCII.GetBytes(msg));
        }

    }
}
