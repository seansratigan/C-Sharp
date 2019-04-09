using System;
using System.Collections;
using System.Linq;

namespace IRCSharp
{
    class IRCMessageHandler
    {

        public static Int32 FindNth(Int32 n, string s, string message)
        {
            Int32 x = 0;
            if (message.IndexOf(s) == -1)
            {
                return -1;
            }
            else
            {
                for (int i = 1; i <= n; i++)
                {
                    x = message.IndexOf(s, x + 1);
                }
            }
            return x;
        }

        public static void getLines(string message)
        {
            if (!Settings.isServerName())
            {
                Settings.StoreServerName(message.Substring(1, FindNth(1, " ", message)));
            }
            while (message.IndexOf("\r\n") != -1)
            {
                Int32 f = message.IndexOf("\r\n");
                MessageRouter(message.Substring(0, f + 2));
                message = message.Remove(0, f + 2);
            }
        }

        public static void MessageRouter(string message)
        {
            if (message.IndexOf(Settings.FetchServerName(), 0) == 1)
            {
                serverMessage(message);
            }
            else if (message.Contains("@") && message.Contains("!") && !message.Contains(Settings.Fetch("Nick")))
            {
                userMessage(message);
            }
            else if (message.Contains("PING ") && message.IndexOf("PING ", 0) < message.IndexOf(" ", FindNth(1, " ", message)))
            {
                // PING :unique_string
                // PONG :unique_string
                int beginPos = message.IndexOf(":", 0);
                string subString = message.Remove(0, beginPos);
                Connection.Send("PONG " + subString);
                Connection.Send("JOIN #Dev");
            }
        }

        public static void serverMessage(string serverMessage)
        {
            Console.WriteLine("SERVER: {0}", serverMessage);
        }
        public static void userMessage(string message)
        {
            Console.WriteLine("USER: {0}", message);
            // Locate Nick, User, Host, Type, and Message Strings
            // Incoming message e.g:
            // :their_nick!user@host.sex.party.com PRIVMSG OUR_NICK :hello my name is, slim anus./r/n
            // : NICK ! USER @ HOST MSGTYPE OUR_NICK :MESSAGE BODY/r/n
            // /r/n indicates a new line.
            string nick = string.Empty;
            string user = string.Empty;
            string host = string.Empty;
            string msgType = string.Empty;
            string cr = "\r";
            string lr = "\n";
            string sp = "";

            bool isChannel = false;
            string channel = string.Empty;

            if (message.Contains("@") && message.Contains("!") && !message.Contains(Settings.Fetch("Nick")))
            {
                char[] delim = { ':', '!', '@', ':', ' '};
                string[] msgArray = message.Split(delim);
                nick = msgArray[1];
                user = msgArray[2];
                host = msgArray[3];
                msgType = msgArray[4];
                msgArray = msgArray.Where(val => val != nick).ToArray();
                msgArray = msgArray.Where(val => val != user).ToArray();
                msgArray = msgArray.Where(val => val != host).ToArray();
                msgArray = msgArray.Where(val => val != msgType).ToArray();
                msgArray = msgArray.Where(val => val != cr).ToArray();
                msgArray = msgArray.Where(val => val != lr).ToArray();
                msgArray = msgArray.Where(val => val != sp).ToArray();
                Connection.Send("PRIVMSG #Dev Nick: " + nick);
                Connection.Send("PRIVMSG #Dev User: " + user);
                Connection.Send("PRIVMSG #Dev Host: " + host);
                Connection.Send("PRIVMSG #Dev msgType: " + msgType);
                Connection.Send("PRIVMSG #Dev Server: " + Settings.FetchServerName()); 
                Connection.Send("PRIVMSG #dev The user information from message: ");
                foreach (string element in msgArray)
                {
                    Connection.Send("PRIVMSG #Dev " + element);
                }
            }
        }

    } // End Class Parse
}
