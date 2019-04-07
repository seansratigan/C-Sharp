using System;
using System.Net.Sockets;
using System.Threading;

namespace IRCSharp
{

    class Connection
    {
        private static TcpClient s;
        private static byte[] data;
        private static NetworkStream iostream;

        public static void Connect(string host, int port)
        {
            s = new TcpClient(host, port);
            iostream = s.GetStream();
        } //  end connect

        public static void Send(string message)
        {
            Byte[] data = System.Text.Encoding.ASCII.GetBytes(message + "\r\n");
            iostream.Write(data, 0, data.Length);
            Console.WriteLine("<Me> {0}", message);
        } // end send

        public static void Clean()
        {
            iostream.Close();
            s.Close();
        } // end clean

        public static void Listen()
        {
            string receiveData = String.Empty;
            data = new byte[512];
            while (data != null)
            {

                Int32 bytes = iostream.Read(data, 0, data.Length);
                receiveData = System.Text.Encoding.ASCII.GetString(data, 0, data.Length);
                Parse.getLines(receiveData);
                Array.Clear(data, 0, data.Length);
                // Close the stream and socket if we get disconnected
                if (receiveData.Contains("Closing Link"))
                {
                    Clean();
                    break;
                }
                // Connection Registration
                // TEMP looking for :test.environment.server NOTICE * :*** Looking up your hostname...
                // to know we are connected.
                if (receiveData.Contains("Looking up"))
                {
                    // RFC 1459 Section 4.1
                    Send("NICK " + Settings.Fetch("Nick") + "\r\n");
                    /*
                     Command:        USER
                     Parameters:     <username> <mdoe> <unused> <realname>
                     It must be noted that realname parameter must be the last parameter,
                     because it may contain space characters and must be prefixed with a
                     colon (':') to make sure this is recognised as such.
                     */
                    Send("USER " + Settings.Fetch("User") + " " + Settings.Fetch("Mode") + " * :" + Settings.Fetch("Realname") + "\r\n");
                }
            }
        } // End Class Connection
    }

    class Program
    {
        static void Main()
        {
            Connection.Connect("localhost", 6667);
            ThreadStart listenref = new ThreadStart(() => Connection.Listen());
            Thread Listener = new Thread(listenref);
            Listener.Start();
        }

    } // End Class Program

} // End Namespace

