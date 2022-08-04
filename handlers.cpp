#include "server.hpp"
#include "CGI.hpp"

void Server::process_get_request()
{
	std::stringstream ss;
	std::stringstream ss2;

	char buf[1000];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
	_response["Date"] = "Date: ";
	_response["Date"] += buf;
	_response["Date"] += "\r\n";
	std::string test = _data->s_root() + _http_request["Path"].substr(0, _http_request["Path"].find("?"));
	std::fstream file((_data->s_root() + _http_request["Path"].substr(0, _http_request["Path"].find("?"))).c_str());
	if (!file)
	{
		file.open(_data->fileLocationParser(_http_request["Path"]).substr(0, _data->fileLocationParser(_http_request["Path"]).find("?")).c_str());
	}
	if (!file && _err_string == "200" && !_is_cgi)
		_err_string = "404";
	if (_data->s_methods().find(_http_request["Type"]) != std::string::npos && _http_request["Type"] == "GET" && _err_string != "200")
		displayFiles();
	if (_err_string == "301" || _err_string == "302")
	{
		_response["Header"] = _http_request["Version"] + " " + _err_string + " " + _http_table[_err_string];
		_response["Server"] = "Server: Webserv\r\n";
		_response["Content-Type"] = "Content-Type: text/html\r\n";
		ss2 << _response["Body"].length();
		_response["Content-Length"] = "Content-Length: " + ss2.str() + "\r\n";
	}
	else if (_err_string != "200")
	{
		_response["Header"] = _http_request["Version"] + " " + _err_string + " " + _http_table[_err_string];
		_response["Server"] = "Server: Webserv\r\n";
		if(_dir == false)
			setBodyGet(_err_string);
		ss2 << _response["Body"].length();
		_response["Content-Length"] = "Content-Length: " + ss2.str() + "\r\n";
		_response["Content-Type"] = "Content-Type: " + _mime_types[".html"];
		if (_is_cgi)
			_cgi_err = true;
	}
	else if (_is_cgi)
	{
		std::map<std::string, std::string> env = getCgiEnv();
		CGI cgi(env);
		_err_string = cgi.execCGI(env["PATH_INFO"], _data->s_root());
		if (_err_string == "200")
		{
			std::fstream fs("cgi_out_file");
			ss << fs.rdbuf();
			_cgi_response = _http_request["Version"] + " 200 " + _http_table["200"];
			_cgi_response += ss.str();
		}
		else
		{
			_response["Header"] = _http_request["Version"] + " " + _err_string + " " + _http_table[_err_string];
			_response["Server"] = "Server: Webserv\r\n";
			if(_dir == false)
				setBodyGet(_err_string);
			ss2 << _response["Body"].length();
			_response["Content-Length"] = "Content-Length: " + ss2.str() + "\r\n";
			_response["Content-Type"] = "Content-Type: " + _mime_types[".html"];
			_is_cgi = false;
		}
		std::remove("cgi_out_file");
	}
	else
	{

		ss << file.rdbuf();
		file.close();
		_response["Header"] = _http_request["Version"] + " 200 " + _http_table["200"];
		_response["Server"] = "Server: Webserv\r\n";
		if(_dir == false)
			_response["Body"] = ss.str();
		_file_size = _response["Body"].length();
		ss2 << _response["Body"].length();
		_response["Content-Length"] = "Content-Length: " + ss2.str() + "\r\n";
		_response["Content-Type"] = "Content-Type: " + _mime_types[_http_request["Extention"]];
	}
	_response["Connection"] = "Connection: Closed\r\n";
	_err_string = "200";
	_finished = true;
	_dir = false;
}

void	Server::process_post_request()
{
	storePostData();

	if(_finished == true){
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
		std::ifstream file((_data->s_root() + file_name).c_str());
		if (file && _err_string == "200" && !_is_cgi)
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
		else if (_is_cgi)
		{
			std::stringstream ss;
			std::stringstream ss2;
			std::map<std::string, std::string> env = getCgiEnv();
			
			env["QUERY_STRING"] = _http_request["Body"];
			CGI cgi(env);
			_err_string = cgi.execCGI(env["PATH_INFO"], _data->s_root());
			if (_err_string == "200")
			{
				std::fstream fs("cgi_out_file");
				ss << fs.rdbuf();
				_cgi_response = _http_request["Version"] + " 200 " + _http_table["200"];
				_cgi_response += ss.str();
			}
			else
			{
				_response["Header"] = _http_request["Version"] + " " + _err_string + " " + _http_table[_err_string];
				_response["Server"] = "Server: Webserv\r\n";
				if(_dir == false)
					setBodyGet(_err_string);
				ss2 << _response["Body"].length();
				_response["Content-Length"] = "Content-Length: " + ss2.str() + "\r\n";
				_response["Content-Type"] = "Content-Type: " + _mime_types[".html"];
				_is_cgi = false;
			}
			std::remove("cgi_out_file");
		}
		else{
			std::ofstream ofs((_data->s_root() + file_name).c_str());
			ofs << _storage_data;
			ofs.close();
			if(_http_request["Expect"] == "100-continue")
				_response["Header"] = _http_request["Version"] + _http_request["Expect"];
			else
				_response["Header"] = _http_request["Version"] + " 200 " + _http_table["200"];
			_response["Server"] = "Server: Webserv\r\n";
			_response["Body"] = post_page();
			len << _response["Body"].length();
			_response["Content-Length"] = "Content-Length: " + len.str() + "\r\n";
			_response["Content-Type"] = "Content-Type: ";
			_response["Content-Type"] += _mime_types[".html"];
		}
		_response["Connection"] = "Connection: Closed\r\n";
		_err_string = "200";
		}
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

		_response["Connection"] = "Connection: Closed\r\n";
		_err_string = "200";
		_finished = true;
}

void Server::setBodyGet(std::string err_str){

	std::stringstream ss;
	std::stringstream code_str(err_str);
	size_t code;
	code_str >> code;
	std::string page = _data->findErrorPage(code);

	std::fstream file2(page.c_str());
		
	if(file2.is_open()){
		ss << file2.rdbuf();
		_response["Body"] = ss.str();
		file2.close();
	}
	else
		_response["Body"] = generate_html(err_str);
}

void Server::storePostData()
{
	if (_http_request["Content-Type"].find("multipart/form-data") != std::string::npos)
	{
		int pos = (int)std::string::npos;
		int pos2 = (int)std::string::npos;
		static bool start = true;
		if((pos = _storage.find("--" + _http_request["Boundary"])) != (int)std::string::npos){
			if (start == true)
			{
				_storage = _storage.substr(pos + _http_request["Boundary"].length() + 3);
				start = false;
			}
			if ((pos = _storage.find("\r\n\r\n")) != (int)std::string::npos)
				_storage_data = _storage.substr(pos + 4);
			else
				_storage_data += _storage;
			if((pos2 = _storage_data.find("--" + _http_request["Boundary"] + "--")) != (int)std::string::npos)
			{
				_storage_data = _storage_data.substr(0, pos2);
				_finished = true;
				start = true;
			}
		}
		else
			_storage_data += _storage;
	}
	else
	{
		_http_request["Body"] = _storage.substr(_storage.find("\r\n\r\n") + 4);
		_storage_data = _storage.substr(_storage.find("\r\n\r\n") + 4);
		_storage_data = _storage_data.substr(0, _data->s_bodySize());
		_finished = true;
	}
}

void Server::displayFiles(){

	// std::cout << _http_request["Path"] << std::endl;

	char buff[254];
	std::string str = _data->s_root() + _http_request["Path"];
	if (*str.rbegin() != '/')
		str += "/";
	for(std::vector<std::string>::const_iterator it = _data->s_listing().begin(); it != _data->s_listing().end(); ++it){
		if(((*it).compare(str)) == 0)
			goto a;
		strcpy(buff, (*it).c_str());
		char* token = strtok(buff, "/");
		while(token){
			if(!strcmp(token, _http_request["Path"].c_str()))
				goto a;
			token = strtok(NULL, "/");
		}
	}
	return;


a:	struct stat buffer;
	stat((str).c_str(), &buffer);
	if(S_ISDIR(buffer.st_mode)){
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir((str).c_str())) != NULL){
			_response["Body"] = "<!DOCTYPE html><html><body>";
			while ((ent = readdir(dir)) != NULL){
				_response["Body"] += "<a href=\"/" + _http_request["Path"];
				if (*_response["Body"].rbegin() != '/')
					_response["Body"] += "/";
				_response["Body"] += ent->d_name;
				_response["Body"] += "\">";
				_response["Body"] += ent->d_name;
				_response["Body"] += "</a><br>";
			}
			closedir(dir);
			_response["Body"] += "</body></html>";
			_dir = true;
			_err_string = "200";
			_http_request["Extention"] = ".html";
		}
		else _err_string = "401";
	}
}
