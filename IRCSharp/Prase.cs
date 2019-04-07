using System;

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
            while (message.IndexOf("\r\n") != -1)
            {
                Int32 f = message.IndexOf("\r\n");
                getParameters(message.Substring(0, f + 2));
                message = message.Remove(0, f + 2);
            }
        }

        public static void getParameters(string message)
        {
            Console.WriteLine("SERVER: {0}", message);
            // Locate Nick, User, Host, Type, and Message Strings
            // Incoming message e.g:
            // :their_nick!user@host.sex.party.com PRIVMSG OUR_NICK :hello my name is, slim anus./r/n
            // : NICK ! USER @ HOST MSGTYPE OUR_NICK :MESSAGE BODY/r/n
            // /r/n indicates a new line.
            string nick = string.Empty;
            string user = string.Empty;
            string host = string.Empty;
            string msgType = string.Empty;
            string parameters = string.Empty;
            bool isChannel;
            Int32 beginPos;
            Int32 endPos;
            Int32 length;
            if (message.IndexOf("!") < message.IndexOf("@") && message.IndexOf("@") < message.IndexOf(" ", FindNth(1, " ", message)))
            {
                // Find Nick
                beginPos = message.IndexOf(":", 0) + 1;
                endPos = message.IndexOf("!", 0) - 2;
                length = beginPos + endPos;
                nick = message.Substring(1, length);
                message = message.Remove(0, length + 1);

                // Find User
                beginPos = message.IndexOf("!", 0);
                endPos = message.IndexOf("@", 0);
                length = beginPos + endPos;
                user = message.Substring(beginPos + 1, length - 1);
                message = message.Remove(beginPos, length);

                // Find Host
                beginPos = message.IndexOf("@", 0) + 1;
                endPos = message.IndexOf(" ", FindNth(1, " ", message));
                length = beginPos + endPos;
                host = message.Substring(1, length - 2);
                message = message.Remove(0, length);

                // Find MsgType
                beginPos = 0;
                endPos = message.IndexOf(" ", FindNth(1, " ", message));
                length = beginPos + endPos;
                msgType = message.Substring(0, length);
                message = message.Remove(0, length + 1);

                // Is this a channel?
                if (message.Contains("#") && message.Contains(" ") && message.IndexOf("#") < message.IndexOf(" ", FindNth(1, " ", message)))
                {
                    isChannel = true;
                    Connection.Send("PRIVMSG #Dev This message was from a channel: " + isChannel);
                }
                else if (!message.Contains("#") && message.IndexOf(Settings.Fetch("Nick")) < message.IndexOf(" ", FindNth(1, " ", message)))
                {
                    isChannel = false;
                    Connection.Send("PRIVMSG #Dev I got a private message from: " + nick);
                    Connection.Send("PRIVMSG #Dev This message was from a channel: " + isChannel);
                }



                Console.WriteLine("This string: {0}", message);
            }
            else if (message.Contains("PING ") && message.IndexOf("PING ", 0) < message.IndexOf(" ", FindNth(1, " ", message)))
            {
                // PING :unique_string
                // PONG :unique_string
                beginPos = message.IndexOf(":", 0);
                string subString = message.Remove(0, beginPos);
                Connection.Send("PONG " + subString);
                Connection.Send("JOIN #Dev");
            }
        }

    } // End Class Parse
}
