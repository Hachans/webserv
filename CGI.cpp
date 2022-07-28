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

int CGI::execCGI(std::string const &filePath){
	//Text color modifiers
	Color::Modifier f_red(Color::Red);
	Color::Modifier reset(Color::White, 0, 1);

	std::string path_to_use(filePath);
	std::string exts[2] = {".py", ".php"};
	size_t i = 0;
	for (; i < 2; i++){
		size_t s = exts[i].size();
		if (!filePath.compare(filePath.size() - s, s, exts[i]))
			break;
	}
	if (i == 2){ std::cerr << f_red << filePath  << ": Script type not supported" << reset << std::endl; return 1;} 

	//modify PATH_INFO
	if (!gconf->CGI->count(exts[i])){
		std::cerr << f_red << "Path for script extension: " << exts[i]
		<< " not specified. Path set to default" << reset << std::endl;
	}
	else{
		_env["PATH_TRANSLATED"] = (*gconf->CGI)[exts[i]];
	}
	

	if (!file_exists(filePath)){
		path_to_use = _env["PATH_TRANSLATED"] + filePath;
		if (!file_exists(path_to_use)){  std::cerr  << f_red << filePath << ": file not found" << reset << std::endl; return 1; }
	}

	//Build env
	char **env;
	if ((env = (char**)calloc(_env.size() + 1, sizeof(char*))) == NULL)
		return 1;
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


	//Open out file
	int file_fd = open("cgi_out_file", O_CREAT|O_WRONLY|O_TRUNC|O_NONBLOCK, 0777);
	if (file_fd == -1)
		return 1;

	//exec cgi script, print output to cgi_out_file
	pid_t pid;
	if ((pid = fork()) == -1){
		perror("fork: ");
		return 1;
	}
	if (!pid){
		if (dup2(file_fd, 1) == -1)
			return 1;
		if (execve(path_to_use.c_str(), NULL, env) == -1)
			return 1;
	}
	else{
		int status;
		if (waitpid(pid, &status, 0) == -1)
			return 1;
		if (WIFEXITED(status) && WEXITSTATUS(status))
			return 1;
		close(file_fd);

		{
			size_t l = _env.size();
			for (size_t i = 0; i < l; i++)
				delete env[i];
			delete env;
		}
	}
	return 200;
}


