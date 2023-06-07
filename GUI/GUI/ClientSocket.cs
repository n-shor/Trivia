using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Xml.Linq;

namespace GUI
{
    public class ClientSocket
    {
        private static Socket _sock;
        public static Socket sock
        {
            get { return _sock; }
            set { _sock = value; }
        }

        private static string _username;
        public static string username
        {
            get { return _username; }
            set { _username = value; }
        }
    }
}
