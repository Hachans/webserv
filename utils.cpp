#include "server.hpp"

std::map<std::string, std::string> initialize_mime_types(void)
{
	std::map<std::string, std::string> mime_types;

	mime_types[".aac"]      = "audio/aac\r\n";
	mime_types[".mp4"]      = "video/mp4\r\n";
	mime_types[".abw"]      = "application/x-abiword\r\n";
	mime_types[".arc"]      = "application/octet-stream\r\n";
	mime_types[".avi"]      = "video/x-msvideo\r\n";
	mime_types[".azw"]      = "application/vnd.amazon.ebook\r\n";
	mime_types[".bin"]      = "application/octet-stream\r\n";
	mime_types[".bz"]       = "application/x-bzip\r\n";
	mime_types[".bz2"]      = "application/x-bzip2\r\n";
	mime_types[".csh"]      = "application/x-csh\r\n";
	mime_types[".css"]      = "text/css\r\n";
	mime_types[".csv"]      = "text/csv\r\n";
	mime_types[".doc"]      = "application/msword\r\n";
	mime_types[".epub"]     = "application/epub+zip\r\n";
	mime_types[".gif"]      = "image/gif\r\n";
	mime_types[".htm"]      = "text/html\r\n";
	mime_types[".html"]     = "text/html\r\n";
	mime_types[".ico"]      = "image/x-icon\r\n";
	mime_types[".ics"]      = "text/calendar\r\n";
	mime_types[".jar"]      = "Temporary Redirect\r\n";
	mime_types[".jpeg"]     = "image/jpeg\r\n";
	mime_types[".jpg"]      = "image/jpeg\r\n";
	mime_types[".js"]       = "application/js\r\n";
	mime_types[".json"]     = "application/json\r\n";
	mime_types[".mid"]      = "audio/midi\r\n";
	mime_types[".midi"]     = "audio/midi\r\n";
	mime_types[".mpeg"]     = "video/mpeg\r\n";
	mime_types[".mpkg"]     = "application/vnd.apple.installer+xml\r\n";
	mime_types[".odp"]      = "application/vnd.oasis.opendocument.presentation\r\n";
	mime_types[".ods"]      = "application/vnd.oasis.opendocument.spreadsheet\r\n";
	mime_types[".odt"]      = "application/vnd.oasis.opendocument.text\r\n";
	mime_types[".oga"]      = "audio/ogg\r\n";
	mime_types[".ogv"]      = "video/ogg\r\n";
	mime_types[".ogx"]      = "application/ogg\r\n";
	mime_types[".png"]      = "image/png\r\n";
	mime_types[".pdf"]      = "application/pdf\r\n";
	mime_types[".ppt"]      = "application/vnd.ms-powerpoint\r\n";
	mime_types[".rar"]      = "application/x-rar-compressed\r\n";
	mime_types[".rtf"]      = "application/rtf\r\n";
	mime_types[".sh"]       = "application/x-sh\r\n";
	mime_types[".svg"]      = "image/svg+xml\r\n";
	mime_types[".swf"]      = "application/x-shockwave-flash\r\n";
	mime_types[".tar"]      = "application/x-tar\r\n";
	mime_types[".tif"]      = "image/tiff\r\n";
	mime_types[".tiff"]     = "image/tiff\r\n";
	mime_types[".ttf"]      = "application/x-font-ttf\r\n";
	mime_types[".txt"]      = "text/plain\r\n";
	mime_types[".vsd"]      = "application/vnd.visio\r\n";
	mime_types[".wav"]      = "audio/x-wav\r\n";
	mime_types[".weba"]     = "audio/webm\r\n";
	mime_types[".webm"]     = "video/webm\r\n";
	mime_types[".webp"]     = "image/webp\r\n";
	mime_types[".woff"]     = "application/x-font-woff\r\n";
	mime_types[".xhtml"]    = "application/xhtml+xml\r\n";
	mime_types[".xls"]      = "application/vnd.ms-excel\r\n";
	mime_types[".xml"]      = "application/xml\r\n";
	mime_types[".xul"]      = "application/vnd.mozilla.xul+xml\r\n";
	mime_types[".zip"]      = "application/zip\r\n";
	mime_types[".3gp"]      = "video/3gpp audio/3gpp\r\n";
	mime_types[".3g2"]      = "video/3gpp2 audio/3gpp2\r\n";
	mime_types[".7z"]		= "application/x-7z-compressed\r\n";

	return (mime_types);
}

std::map<std::string, std::string> http_table(void)
{
	std::map<std::string, std::string> http;

	http[ "100" ] = "Continue\r\n";
	http[ "101" ] = "Switching Protocols\r\n";
	http[ "102" ] = "Processing\r\n";
	http[ "103" ] = "Checkpoint\r\n";

	http[ "200" ] = "OK\r\n";
	http[ "201" ] = "Created\r\n";
	http[ "202" ] = "Accepted\r\n";
	http[ "203" ] = "Non-Authoritative Information\r\n";
	http[ "204" ] = "No Content\r\n";
	http[ "205" ] = "Reset Content\r\n";
	http[ "206" ] = "Partial Content\r\n";
	http[ "207" ] = "Multi-Status\r\n";
	http[ "208" ] = "Already Reported\r\n";

	http[ "300" ] = "Multiple Choices\r\n";
	http[ "301" ] = "Moved Permanently\r\n";
	http[ "302" ] = "Found\r\n";
	http[ "303" ] = "See Other\r\n";
	http[ "304" ] = "Not Modified\r\n";
	http[ "305" ] = "Use Proxy\r\n";
	http[ "306" ] = "Switch Proxy\r\n";
	http[ "307" ] = "Temporary Redirect\r\n";
	http[ "308" ] = "Permanent Redirect\r\n";

	http[ "400" ] = "Bad Request\r\n";
	http[ "401" ] = "Unauthorized\r\n";
	http[ "402" ] = "Payment Required\r\n";
	http[ "403" ] = "Forbidden\r\n";
	http[ "404" ] = "Not Found\r\n";
	http[ "405" ] = "Method Not Allowed\r\n";
	http[ "406" ] = "Not Acceptable\r\n";
	http[ "407" ] = "Proxy Authentication Required\r\n";
	http[ "408" ] = "Request Timeout\r\n";
	http[ "409" ] = "Conflict\r\n";
	http[ "410" ] = "Gone\r\n";
	http[ "411" ] = "Length Required\r\n";
	http[ "412" ] = "Precondition Failed\r\n";
	http[ "413" ] = "Payload Too Large\r\n";
	http[ "414" ] = "URI Too Long\r\n";
	http[ "415" ] = "Unsupported Media Type\r\n";
	http[ "416" ] = "Requested Range Not Satisfiable\r\n";
	http[ "417" ] = "Expectation Failed\r\n";
	http[ "418" ] = "I'm a teapot\r\n";
	http[ "421" ] = "Misdirected Request\r\n";
	http[ "422" ] = "Unprocessable Entity\r\n";
	http[ "423" ] = "Locked\r\n";
	http[ "424" ] = "Failed Dependency\r\n";
	http[ "426" ] = "Upgrade Required\r\n";
	http[ "428" ] = "Precondition Required\r\n";
	http[ "429" ] = "Too Many Request\r\n";
	http[ "431" ] = "Request Header Fields Too Large\r\n";
	http[ "451" ] = "Unavailable For Legal Reasons\r\n";

	http[ "500" ] = "Internal Server Error\r\n";
	http[ "501" ] = "Not Implemented\r\n";
	http[ "502" ] = "Bad Gateway\r\n";
	http[ "503" ] = "Service Unavailable\r\n";
	http[ "504" ] = "Gateway Timeout\r\n";
	http[ "505" ] = "HTTP Version Not Supported\r\n";
	http[ "506" ] = "Variant Also Negotiates\r\n";
	http[ "507" ] = "Insufficient Storage\r\n";
	http[ "508" ] = "Loop Detected\r\n";
	http[ "510" ] = "Not Extended\r\n";
	http[ "511" ] = "Network Authentication Required\r\n";

	return http;
}

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return (&(reinterpret_cast<struct sockaddr_in *>(sa)->sin_addr));
	return (&(reinterpret_cast<struct sockaddr_in6 *>(sa)->sin6_addr));
}

void	Server::parse_first_line(std::string line){
	size_t pos, lpos;

	pos = line.find(' ');
	_http_request["Type"] = line.substr(0, pos);
	pos++;
	lpos = pos;

	pos = line.find(' ', lpos);
	_http_request["Path"] = line.substr(lpos, (pos - lpos));
	pos++;
	lpos = pos;
	if (_http_request["Path"] == "/")
		_http_request["Path"] = "/index.html";
	_http_request["Path"].erase(0, 1);

	pos = line.find('\n', lpos);
	_http_request["Version"] = line.substr(lpos, (pos - lpos) - 1);

	pos = _http_request["Path"].find('.');
	_is_cgi = false;

	for (std::map<std::string, std::string>::const_iterator it = _data->s_HTTP_redir().begin(); it != _data->s_HTTP_redir().end(); it++)
	{
		std::string str = _data->s_root();
		if (str == "")
		{
			char ptr[1000];
			bzero(ptr, 1000);
			getcwd(ptr, 1000);
			str = ptr;
			str += "/";
		}
		if (it->first == str + _http_request["Path"])
		{
			_err_string = it->second.substr(0, it->second.find(" "));
			_response["Type"] = "GET";
			_response["Body"] = redir_body(it->second.substr(it->second.find(" ") + 1));
			return ;
		}
	}

	if(pos == std::string::npos && _err_string == "200" && _http_request["Type"] == "GET" && !_is_cgi)
		_err_string = "415";
	else if (pos != std::string::npos)
	{
		if (_http_request["Path"].find("?") == std::string::npos)
			_http_request["Extention"] = _http_request["Path"].substr(pos, _http_request["Path"].length() - pos);
		else
		{
			std::string temp = _http_request["Path"].substr(0, _http_request["Path"].find("?"));
			_http_request["Extention"] = temp.substr(pos, temp.length() - pos);
		}
		for (std::vector<std::string>::iterator it = _cgi_types.begin(); it != _cgi_types.end(); it++)
		{
			if (*it == _http_request["Extention"])
				_is_cgi = true;
		}
		if (_mime_types[_http_request["Extention"]] == "" && !_is_cgi)
			_err_string = "415";
	}
}

void Server::parse_header(char* buff){
	std::stringstream ss(buff);
	std::string line;
	size_t	pos;

	std::getline(ss, line, '\n');
	while(std::getline(ss, line, '\n')){
		pos = line.find(": ", 0);
		if(_http_request["Content-Type"] == "" && line.substr(0, pos) == "Content-Type")
			_http_request["Content-Type"] = line.substr(pos + 2, line.length() - 1 - pos);
		else if (line.substr(0, pos) != "Content-Type" && pos != std::string::npos && line != "")
			_http_request[line.substr(0, pos)] = line.substr(pos + 2, line.length() - 1 - pos);

	}
	if(_http_request["Content-Type"] != ""){
		int pos = _http_request["Content-Type"].find("boundary=");
		if(pos != (int)std::string::npos){
			_http_request["Boundary"] = _http_request["Content-Type"].substr(pos + 9);
		}
	}
	if(_http_request["Type"] == "POST" && _http_request["Boundary"] != "")
		_http_request["Boundary"].erase(_http_request["Boundary"].length() - 1, 1);
}

void	Server::check_values(){
	if((_http_request["Version"] != "HTTP/1.1"))
		if(_err_string == "200")
			_err_string = "505";
	if((_http_request["Type"] != "GET") && (_http_request["Type"] != "POST") && (_http_request["Type"] != "DELETE"))
		if(_err_string == "200")
			_err_string = "400";
}

std::string	Server::redir_body(std::string url)
{
	std::string ret = "<meta http-equiv=\"refresh\" content=\"0; URL=" + url + "\" />";
	return (ret);
}
