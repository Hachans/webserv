#include "server.hpp"

void Server::process_get_request()
{
	std::fstream file(_http_request["Path"]);
	std::stringstream ss;
	std::stringstream ss2;

	char buf[1000];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
	_response["Date"] = "Date: ";
	_response["Date"] += buf;
	_response["Date"] += "\r\n";
	if (!file && _err_string == "200")
		_err_string = "404";
	if (_err_string != "200")
	{
		_response["Header"] = _http_request["Version"] + " " + _err_string + " " + _http_table[_err_string];
		_response["Server"] = "Server: Webserv\r\n";
		_response["Body"] = generate_html(_err_string);
		ss2 << _response["Body"].length();
		// _file_size = _response["Body"].length();
		_response["Content-Length"] = "Content-Length: " + ss2.str() + "\r\n";
		_response["Content-Type"] = "Content-Type: " + _mime_types[".html"];
	}
	else
	{
		ss << file.rdbuf();
		_response["Header"] = _http_request["Version"] + " 200 " + _http_table["200"];
		_response["Server"] = "Server: Webserv\r\n";
		_response["Body"] = ss.str();
		_file_size = _response["Body"].length();
		ss2 << _response["Body"].length();
		_response["Content-Length"] = "Content-Length: " + ss2.str() + "\r\n";
		_response["Content-Type"] = "Content-Type: " + _mime_types[_http_request["Content-Type"]];
	}
	_response["Connection"] = "Connection: closed\r\n";
	_err_string = "200";
}

void	Server::process_post_request()
{
	std::stringstream len;
	char buf[1000];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
	_response["Date"] = "Date: ";
	_response["Date"] += buf;
	_response["Date"] += "\r\n";

	std::string file_name = _http_request["Path"];
	if(_http_request["Content-Disposition"] != ""){
		int pos = _http_request["Content-Disposition"].find("filename=") + 10;
		file_name = _http_request["Content-Disposition"].substr(pos);
		file_name = file_name.substr(0, file_name.length() - 2);
	}
	
	std::ifstream file(file_name);
	if (file && _err_string == "200")
		_err_string = "422";
	file.close();
	if(_err_string != "200"){
		_response["Header"] = _http_request["Version"] + " " + _err_string + " " + _http_table[_err_string];
		_response["Server"] = "Server: Webserv\r\n";
		_response["Body"] = generate_html(_err_string);
		_response["Content-Length"] = "Content-Length: ";
		len << _response["Body"].length();
		_response["Content-Length"] += len.str() + "\r\n";
		_response["Content-Type"] = "Content-Type: " + _mime_types[".html"];
	}
	else{
		std::ofstream ofs(file_name);
		ofs.close();
		_response["Header"] = _http_request["Version"] + " 200 " + _http_table["200"];
		_response["Server"] = "Server: Webserv\r\n";
		_response["Body"] = post_page();
		len << _response["Body"].length();
		_response["Content-Length"] = "Content-Length: " + len.str() + "\r\n";
		_response["Content-Type"] = "Content-Type: ";
		_response["Content-Type"] += _mime_types[".html"];
	}
	_response["Connection"] = "Connection: closed\r\n";
	_err_string = "200";
}

void Server::process_delete_request(){

	std::stringstream len;
	char buf[1000];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
	_response["Date"] = "Date: ";
	_response["Date"] += buf;
	_response["Date"] += "\r\n";

	int status = remove(_http_request["Path"].c_str());
	if (status != 0 && _err_string == "200")
		_err_string = "204";
	if(_err_string != "200"){
		_response["Header"] = _http_request["Version"] + " " + _err_string + " " + _http_table[_err_string];
		_response["Server"] = "Server: Webserv\r\n";
		_response["Body"] = generate_html(_err_string);
		_response["Content-Length"] = "Content-Length: ";
		len << _response["Body"].length();
		_response["Content-Length"] += len.str() + "\r\n";
		_response["Content-Type"] = "Content-Type: " + _mime_types[".html"];
	}
	else{
		_response["Header"] = _http_request["Version"] + " 200 " + _http_table["200"];
		_response["Server"] = "Server: Webserv\r\n";
		_response["Body"] = delete_page();
		len << _response["Body"].length();
		_response["Content-Length"] = "Content-Length: " + len.str() + "\r\n";
		_response["Content-Type"] = "Content-Type: ";
		_response["Content-Type"] += _mime_types[".html"];
	}

	_response["Connection"] = "Connection: closed\r\n";
	_err_string = "200";
}
