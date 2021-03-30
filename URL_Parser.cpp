#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

void displayVersion();
string protocols(string URL);
string domains(string URL);
string ports(string URL);
string file_paths(string URL);
string parameters(string URL);

string VERSION_NUM = "1.2";

int main(int argc, char* argv[])
{
	//Commenting 
	//Comment
	displayVersion();
	//Obtains the URL
	string URL;
	for(int i = 1; i < argc; i++)
	{
		URL += argv[i];
	}

	//Obtains the different parts of the URL
	string Protocol, Domain, Port, FilePath, Parameters;
	bool protocolErr = false, domainErr = false, portErr = false, filePathErr = false;

	Protocol = protocols(URL);
	if(Protocol.find("ERROR:") != -1)
	{
		Protocol = Protocol.substr(6, Protocol.length());
		protocolErr = true;
	}
	URL =  URL.substr(Protocol.length() + 3);

	Domain = domains(URL);
	if(Domain.find("ERROR:") != -1)
	{
		Domain = Domain.substr(6, Domain.length());
		domainErr = true;
	}
	URL = URL.substr(Domain.length());

	Port = ports(URL);
	if(Port.find("ERROR:") != -1)
	{
		Port = Port.substr(6, Port.length());
		portErr = true;
	}
    if(Port.compare("NONE") != 0)
    {
	    URL = URL.substr(Port.length() + 1);
    }

	FilePath = file_paths(URL);
	if(FilePath.find("ERROR:") != -1)
	{
		FilePath = FilePath.substr(6, FilePath.length());
		filePathErr = true;
	}
	URL = URL.substr(FilePath.length());

	Parameters = parameters(URL); 

	//Finds Errors
	if((protocolErr == true) || (domainErr == true) || (portErr == true) || (filePathErr == true))
		cout << "Invalid URL with erroneous components:\n";

	if(protocolErr == true)
		cout << "Error: " + Protocol + " is not a valid protocol\n";
	
	if(domainErr == true)
	{
		Domain = Domain.substr(Domain.rfind('.') + 1);
		cout << "Error: " + Domain + " is an invalid domain\n";
	}

	if(portErr == true)
		cout << "Error: " + Port + " is invalid(It must be between 1 and 65535)\n";

	if(filePathErr == true)
		cout << "Error: " + FilePath + " is not a valid file path\n";

	//Prints Result
	if((protocolErr == false) && (domainErr == false) && (portErr == false) && (filePathErr == false))
	{
		cout << left << setw(15) << "Protocol:" << Protocol << "\n";
	    cout << left << setw(15) << "Domain:" << Domain << "\n";
        if(Port.compare("NONE") != 0)
		    cout << left << setw(15) << "Port:" << Port + "\n";
		cout << left << setw(15) << "FilePath:" << FilePath + "\n";
        if(Parameters.compare("NONE") != 0)
		    cout << left << setw(15) << "Parameters:" << Parameters + "\n";
	}
}

//Checks if the URL contains a valid protocol
string protocols(string URL)
{
	string output = "" ;

	//Finds if it contains 'http', 'https', 'ftp', and 'ftps'
	bool http = (URL.find("http:") != -1);
	bool https = (URL.find("https:") != -1);
	bool ftp = (URL.find("ftp:") != -1);
	bool ftps = (URL.find("ftps:") != -1);

	//Checks if it containts only 1 protocol
	if(http ^ https ^ ftp ^ ftps)
	{
		output = URL.substr(0, URL.find(':'));
	}

	//Error
	else
	{
		output = "ERROR:" +  URL.substr(0, URL.find(':'));
	}

	return output;
}

//Checks if the URL contains a valid domain
string domains(string URL)
{
	string output = "";
	string x = "", y = "", z = "";
	int dotCount = 0;

	//Checks if there is 3 parts
	for(int i = 0;(i < URL.length()) &&  (URL[i] != '/'); i++)
	{
		if(URL[i] == '.')
			dotCount++;
	}

	//If there are 3 parts
	if(dotCount == 2)
	{
		//Obtains the 3 parts
		x = URL.substr(0, URL.find('.'));
		y = URL.substr(x.length() + 1, URL.find('.', x.length() + 1) - 3);
		z = URL.substr((y.length() + 1) + (x.length() + 1), URL.length() - 1);

		//Checks if the URL contains a port
		if(z.find(':') != -1)
		{
			z = URL.substr((y.length() + 1) + (x.length() + 1), z.find(':'));
		}
		
		else
		{
			z = URL.substr((y.length() + 1) + (x.length() + 1), z.find('/'));
		}
		
		//Checks if it z is valid
		if((z.compare("com") == 0) || (z.compare("net") == 0) || (z.compare("edu") == 0) || (z.compare("biz") == 0) || (z.compare("gov") == 0))
		{
			output = x + "." + y + "." + z;
		}

		else
		{
			output = "ERROR:" + x + "." + y + "." + z;
		}
	}

	//Error
	else
	{
		//Checks if the domain contains a port
		if(z.find(':') != -1)
		{
			output = "ERROR:" + URL.substr(0, z.find(':'));
		}
		else
		{
			output = "ERROR:" + URL.substr(0, z.find('/'));
		}
	}

	return output;
}

string file_paths(string URL)
{
	string output = "";

	//Checks if it ends with .html or .htm and starts with /
	if((URL[0] == '/') && ((URL.find(".htm") != -1) || (URL.find(".html") != -1)))
	{
		if(URL.find(".html") != -1)
		{
			output = URL.substr(0, URL.find("html") + 4);
		}
		else
		{
			output = URL.substr(0, URL.find("htm") + 3);
		}
	}

	//Error
	else
	{
		//Checks if it contains a parameter
		if(URL.find('?') != -1)
		{
			output = "ERROR:" + URL.substr(0, URL.find("?"));
		}
		else
		{
			output = "ERROR:" + URL.substr(0, URL.length());
		}
	}

	return output;
}

string ports(string URL)
{
	string output = "";

	//Checks if a port is present
	if(URL.find(":") != -1)
	{
		string port = URL.substr(1, URL.find("/") - 1);
		
		//Changes the string into an int
		double portNum = stod(port);

		//Checks if it's a valid port
		if(portNum >= 1 && portNum <= 65535)
		{
			output = port;
		}
		else
		{
			output = "ERROR:" + port;
		}
	}

	//No port is present
	else
	{
		output = "NONE";
	}

	return output;
}

string parameters(string URL)
{
	string output = "";

	//Checks if parameters are present
	if(URL.find("?") != -1)
	{
		output = URL.substr(1);
	}
	
	else
	{
		output = "NONE";
	}

	return output;
}

void displayVersion()
{
    cout << "Version: " << VERSION_NUM << "\n";
}
