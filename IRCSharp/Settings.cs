namespace IRCSharp
{
    class Settings
    {
        // User information
        private static string nick = "Dev";
        private static string user = "God";
        private static int mode = 0;
        private static string realname = "Packet Delivery Man";
        // Behavior settings
        private static bool autojoin = false;

        public static dynamic Fetch(string setting)
        {
            if (setting == "Nick") { return nick; }
            if (setting == "User") { return user; }
            if (setting == "Mode") { return mode; }
            if (setting == "Realname") { return realname; }
            return null;
        }

        public static dynamic Set(string setting, string value)
        {
            if (setting == "Nick") { nick = value; }
            return value;
        }
    }
}
