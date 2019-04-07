/*
#################################################
#########   This Will take every word   #########
######### in a line and add to an Array #########
#########   an  integer  "e"  must be	#########
#########   declared before  the loop   #########
#################################################

for (int n = 0; n < i; n++)
{
	recvstr = recvLines[n];
	std::cout << "\n" << recvstr << "\n";
	do {
		std::size_t pos = recvstr.find(" ") + 1;
		std::cout << "FOUND SPACE AT: " << pos << "\n";
		std::string substring;

		substring = recvstr.substr(0, pos);
		//temporary ping reply. i need to get idle time connected
		//to study incoming messages
		if (substring == "PING ")
		{
			pingpos = ( e+1 );
			pong = true;
		}
		recvWords[e] = substring;
		std::cout << "recvWords[" << e << "] = " << recvWords[e] << "\r\n";
		Beep(500, 100);
		recvstr.erase(0, pos);
		e++;
	} while (recvstr.find(" ") != std::string::npos);
}

*/