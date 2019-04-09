namespace IRCSharp
{
    class Settings
    {
        // User information
        private static string nick = "Poopstick";
        private static string user = "God";
        private static int mode = 0;
        private static string realname = "Packet Delivery Man";
        // Behavior settings
        //private static bool autojoin = false;
        // Connection
        private static bool gotServerName = false;
        private static string serverName = string.Empty;

        public static dynamic Fetch(string setting)
        {
            if (setting == "Nick") { return nick; }
            if (setting == "User") { return user; }
            if (setting == "Mode") { return mode; }
            if (setting == "Realname") { return realname; }
            return null;
        }

        public static bool isServerName()
        {
            return gotServerName;
        }

        public static void StoreServerName(string theServerName)
        {
            serverName = theServerName;
            gotServerName = true;
        }
        public static string FetchServerName()
        {
            return serverName;
        }


        public static dynamic Set(string setting, string value)
        {
            if (setting == "Nick") { nick = value; }
            return value;
        }
    }
}
