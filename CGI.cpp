#include "CGI.hpp"
#include "server.hpp"

inline bool file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

CGI::CGI(std::map<std::string, std::string> const &env) : _env(env) {}

CGI::~CGI()
{
}

std::string const &CGI::execCGI(std::string const &filePath, const std::string &root){
	//Text color modifiers
	Color::Modifier f_red(Color::Red);
	Color::Modifier f_green(Color::Green1);
	Color::Modifier reset(Color::White, 0, 1);

	std::string path_to_use(filePath);
	std::string exts[3] = {".py", ".php", ".pl"};
	size_t i = 0;
	for (; i < 3; i++){
		size_t s = exts[i].size();
		if (!filePath.compare(filePath.size() - s, s, exts[i]))
			break;
	}
	if (i == 3){ std::cerr << f_red << filePath  << ": Script type not supported" << reset << std::endl;
		return _ret_code = "415";} 

	//modify PATH_INFO
	if (!gconf->CGI->count(exts[i])){
		std::cerr << f_red << "Path for script extension: " << exts[i]
		<< " not specified. Path set to default" << reset << std::endl;
		_env["PATH_TRANSLATED"] = root;
	}
	else{
		_env["PATH_TRANSLATED"] = (*gconf->CGI)[exts[i]];
	}

	if (!file_exists(filePath)){
		path_to_use = _env["PATH_TRANSLATED"] + filePath;
		if (!file_exists(path_to_use))
			path_to_use = root + filePath;
		if (!file_exists(path_to_use)){  std::cerr  << f_red << filePath << ": file not found" << reset << std::endl; 
			std::cerr << f_red << "path: " << path_to_use << reset << std::endl;
			return _ret_code = "404"; }
	
	}

	std::cerr << f_green << "path to execve: " << path_to_use << reset << std::endl;
	//Open out file
	int file_fd = open("cgi_out_file", O_CREAT|O_WRONLY|O_TRUNC|O_NONBLOCK, 0777);
	if (file_fd == -1)
		return _ret_code = "500";

	//exec cgi script, print output to cgi_out_file
	pid_t pid;
	int fds[2];
	if (_env["REQUEST_METHOD"] == "POST")
	{
		if (pipe(fds) != 0)
			return _ret_code = "500";
		write(fds[1], _env["QUERY_STRING"].c_str(), _env["QUERY_STRING"].length());
		_env["QUERY_STRING"] = "";
	}
	//Build env
	char **env;
	if ((env = (char**)calloc(_env.size() + 1, sizeof(char*))) == NULL)
		return _ret_code = "500";
	{
		std::string env_values;
		size_t l = _env.size();
		std::map<std::string, std::string>::const_iterator it = _env.begin();
		for (size_t i = 0; i < l; i++)
		{
			env_values = it->first + "=" + it->second;
			env[i] = strdup(env_values.c_str());
			it++;
		}
	}

	if ((pid = fork()) == -1){
		perror("fork: ");
		return _ret_code = "500";
	}
	if (!pid){
		char **args = NULL;
		if (_env["REQUEST_METHOD"] == "POST")
		{
			if (dup2(fds[0], 0) == -1)
				return _ret_code = "500";
		}
		if (dup2(file_fd, 1) == -1)
			return _ret_code = "500";
		if (execve(path_to_use.c_str(), args, env) == -1){
			perror("execve");
			exit(1);
		}
		close(fds[0]); close(fds[1]);
	}
	else{
		int status;
		if (waitpid(pid, &status, 0) == -1)
			return _ret_code = "500";
		if (WIFEXITED(status) && WEXITSTATUS(status)){
			return _ret_code = "500";
		}
		close(file_fd);
		close(fds[1]);

		{
			size_t l = _env.size();
			for (size_t i = 0; i < l; i++)
				delete env[i];
			delete env;
		}
	}
	return _ret_code = "200";
}
