#include "config.hpp"

void validate(std::vector<conf_data*> *d, t_gconf *c){
	std::string buff;
	{
		std::map<std::string, std::string>::iterator it = c->CGI->end();
		for (std::map<std::string, std::string>::iterator i = c->CGI->begin(); i != it; i++){
			buff += " ";
			buff += (*i).second;
		}
	}
	{
		std::vector<conf_data*>::const_iterator it = d->begin();
		for (size_t i = 0; i < d->size(); i++)
		{
			buff += " ";
			buff += (*it)->s_root();
			std::map<std::string, std::string>::const_iterator conit = (*it)->s_fileLocations().end();
			for (std::map<std::string, std::string>::const_iterator i = (*it)->s_fileLocations().begin(); i != conit; i++)
			{
				buff += " ";
				buff += (*i).first;
			}
		}
	}
	{
		std::vector<conf_data*>::const_iterator it = d->begin();
		for (size_t i = 0; i < d->size(); i++)
		{
			buff += " ";
			buff += (*it)->s_root();
			std::map<std::string, std::string>::const_iterator conit = (*it)->s_defAnswers().end();
			for (std::map<std::string, std::string>::const_iterator i = (*it)->s_defAnswers().begin(); i != conit; i++)
			{
				buff += " ";
				buff += (*i).first;
			}
		}
	}
	removeDuplWhitespace(buff);
	if (!IsPathsDir(buff))
		throw std::invalid_argument("not a directory");
	
}

bool IsPathsDir(std::string const &str)
{
	Color::Modifier f_red(Color::Red);
	Color::Modifier reset(Color::White, 0, 1);
	struct stat buffer;
	std::stringstream s(str);
	std::string token;
	while (std::getline (s, token, ' ')){
		bzero(&buffer, sizeof(buffer));
		stat (token.c_str(), &buffer);
		if (!S_ISDIR(buffer.st_mode)){
			std::cout << f_red << "\nPath: " << token << " is not a directory" << reset << std::endl;
			return 0;
		}
	}
	return 1;
}

static bool BothAreWhitespace(char lhs, char rhs) {
	return std::isspace(rhs) && std::isspace(lhs);
}

static bool itIsWhiteSpace(char c){
	return std::isspace(c);
}

std::string &removeDuplWhitespace(std::string &str){
	for (std::string::iterator i = str.begin(); std::isspace(*i);)
		str.erase(i);
	{
		std::string::iterator i;
		for (i = --str.end(); std::isspace(*i);)
			--i;
		++i;
		while (i != str.end() && std::isspace(*i))
			str.erase(i);
	}
	
	std::string::iterator new_end = std::unique(str.begin(), str.end(), BothAreWhitespace);
	str.erase(new_end, str.end());
	std::replace_if(str.begin(), str.end(), itIsWhiteSpace, ' ');
	return str;
}

std::vector<conf_data*> *readConfFile(t_gconf *gconf, std::string const &file = "conf/default.conf"){
	if (file.compare(file.size() - 5, 5, ".conf"))
		throw std::invalid_argument("configuration file: invalid name");
	std::vector<conf_data*> *co = new std::vector<conf_data*>();

	struct {
		bool port;
		bool root;
		bool host;
		bool methods;
		bool redir;
		bool CGI;
	} doubles;

	std::ifstream f(file, std::ifstream::in);
	if (f.fail())
		throw std::runtime_error("configuration file not found or corrupt");

	//CHECK FOR INVALID "{}"
	{
		std::ifstream c(file, std::ifstream::in);
		if (c.fail())
			throw std::runtime_error("configuration file not found or corrupt");
		char cont[100000];
		bzero(cont, 100000);
		c.read(cont, 100000);
		if (!strchr(cont, '{'))
			throw std::invalid_argument("missing token '{'");
		for (size_t i = 0; cont[i]; i++)
		{
			if (cont[i] == '{')
				while (cont[i] !='}' && cont[i])
					i++;
			if (!cont[i])
				throw std::invalid_argument("no matching '}'");
		}
	}

	while (!f.eof()){
		char line[10000];
		f.getline(line, 10000, '{');
		std::string l_ine(line);
		
		l_ine.erase(std::remove_if(l_ine.begin(), l_ine.end(), isspace), l_ine.end());

		/* CASE "SERVER" */
		if (l_ine == "server"){
			doubles.port = doubles.root = doubles.host = doubles.methods = doubles.redir = false;
			co->push_back(new conf_data());

			std::vector<conf_data*>::iterator it= --co->end();
			f.getline(line, 10000, '}');
			std::stringstream content(line);
			std::string const rules[10] = {"server_names", "port", "error_page", "root", "host", "allowed_methods", "return", "location", "CGI", ""};

			while (!content.eof())
			{
				content.getline(line, 10000, ':');
				std::string li_ne(line);
				li_ne.erase(remove_if(li_ne.begin(), li_ne.end(), isspace), li_ne.end());
				int pos = 0;
				for (size_t i = 0; i < 10; ++i)
				{
					if (li_ne == rules[i]){
						pos = i + 1;
						break;
					}
				}
				switch (pos)
				{
					case 1:
						content.getline(line, 10000, ';');
						li_ne = line;
						if (content.eof())
								throw std::invalid_argument("no ';' found");
						(*it)->addServerNames(li_ne);
						break;
					
					case 2:
						content.getline(line, 10000, ';');
						if (doubles.port)
							break;
						else
							doubles.port = true;
						char *end;
						if (content.eof())
							throw std::invalid_argument("no ';' found");
						li_ne = line;
						li_ne.erase(remove_if(li_ne.begin(), li_ne.end(), isspace), li_ne.end());
						(*it)->port = strtol(li_ne.c_str(), &end, 10);
						if (!(*it)->port || *end){
							std::cerr << "invalid port value: '" << li_ne << "', port set to default" << std::endl;
							(*it)->port = 4242;
						}
						break;
					case 3:
						content.getline(line, 10000, ';');
						{
							if (content.eof())
									throw std::invalid_argument("no ';' found");
							std::vector<size_t> codes;
							li_ne = line;
							char *end;
							for(size_t j = 0; line[j]; j++)
							{
								if (isdigit(line[j])){
									size_t c = strtol(&line[j], &end, 10);
									if (!std::isspace(*end) || (!isInBounds<size_t>(c, 100, 103) && !isInBounds<size_t>(c, 200, 208) && !isInBounds<size_t>(c, 300, 308)
										&& !isInBounds<size_t>(c, 400, 451) && !isInBounds<size_t>(c, 500, 511)))
										throw std::invalid_argument("invalid error code");
									j += 3;
									codes.push_back(c);
								}
								else if(!isdigit(line[j]) && !isspace(line[j])){
									std::string temp(&line[j]);
									removeDuplWhitespace(temp);
									if (temp.find_first_of(' ') != temp.npos)
										throw std::invalid_argument("only one filepath allowed");
									for (std::vector<size_t>::iterator i = codes.begin(); i != codes.end(); ++i)
										(*it)->error_pages.insert(std::make_pair(*i, temp));
									break;
								}
							}
						}
						break;
					case 4:
						content.getline(line, 10000, ';');
						if (doubles.root)
							break;
						else
							doubles.root = true;
						li_ne = line;
						if (content.eof())
								throw std::invalid_argument("no ';' found");
						(*it)->root = removeDuplWhitespace(li_ne);
						break;
					case 5:
						content.getline(line, 10000, ';');
						if (doubles.host)
							break;
						else
							doubles.host = true;
						li_ne = line;
						if (content.eof())
								throw std::invalid_argument("no ';' found");
						(*it)->host = removeDuplWhitespace(li_ne);
						break;
					case 6:
						{
							content.getline(line, 10000, ';');
							if (doubles.methods)
								break;
							else
								doubles.methods = true;
							std::string const methods[3] = {"GET", "POST", "DELETE"};
							if (content.eof())
									throw std::invalid_argument("no ';' found");
							li_ne = line;
							char *token = strtok(line, " \n\t");
							size_t j = 0;
							for (size_t i = 0; token; ++i){
								for (j = 0; j < 3; j++)
									if (token == methods[j])
										break;
								token = strtok(NULL, " \n\t");
							}
							if (j == 3)
								throw std::invalid_argument("Invalid HTTP method");
							(*it)->methods = removeDuplWhitespace(li_ne);
						}
						break;
					case 7:
						content.getline(line, 10000, ';');
						if (doubles.redir)
							break;
						else
							doubles.redir = true;
						{
							if (content.eof())
									throw std::invalid_argument("no ';' found");
							size_t code;
							li_ne = line;
							char *end;
							for(size_t j = 0; line[j]; j++)
							{
								if (isdigit(line[j])){
									code = strtol(&line[j], &end, 10);
									if (!std::isspace(*end) || (!isInBounds<size_t>(code, 100, 103) && !isInBounds<size_t>(code, 200, 208) && !isInBounds<size_t>(code, 300, 308)
										&& !isInBounds<size_t>(code, 400, 451) && !isInBounds<size_t>(code, 500, 511)))
										throw std::invalid_argument("invalid error code");
									j += 3;
								}
								else if(isalpha(line[j])){
									(*it)->redir_url = std::make_pair(code, &line[j]);
									break;
								}
							}
						}
						break;
					case 8:
						content.getline(line, 10000, '(');
						if (content.eof())
							throw std::invalid_argument("expected '('");
						{
							std::string Fname;
							std::string Fpath;
							Fname = line;
							removeDuplWhitespace(Fname);
							if (Fname.find_first_of(" \t\n\v\f\r") != Fname.npos)
								throw std::invalid_argument("only one filename allowed");
							content.getline(line, 10000, ')');
							if (content.eof())
								throw std::invalid_argument("expected ')'");
							Fpath = line;
							removeDuplWhitespace(Fpath);
							if (Fpath[Fpath.length() - 1] != ';')
								throw std::invalid_argument("no ';' found");
							size_t pos = 0;
							std::string sub;
							while ((pos = Fpath.find(';')) != std::string::npos) {
								sub = Fpath.substr(0, pos);
								removeDuplWhitespace(sub);
								std::string const cases[2] = {"autoindex", "index"};
								int ind = 0;
								for (size_t i = 0; i < 2; i++)
								{
									if (!sub.compare(0, cases[i].length(), cases[i])){
										ind = i + 1;
										break;
									}
								}
								switch (ind)
								{
								case 1:
									if (sub.compare(10, 2, "on"))
										throw std::invalid_argument("only allowed value for 'autoindex' is 'on'");
									(*it)->listing.push_back(Fname);
									break;
								case 2:
								{
									std::string def_files = sub.substr(6);
									(*it)->def_answer_if_dir.insert(std::make_pair(Fname, removeDuplWhitespace(def_files)));
								}
									break;
								default:
									if (sub == " " || sub == "")
										break;
									(*it)->file_locations.insert(std::make_pair(Fname, removeDuplWhitespace(sub)));
									break;
								}
								Fpath.erase(0, pos + 1);
							}
						}
						break;
					case 9:
						content.getline(line, 10000, ';');
						li_ne = line;
						if (content.eof())
								throw std::invalid_argument("no ';' found");
						(*it)->CGI_extensions = removeDuplWhitespace(li_ne);
						break;
					case 10:
						break;
					default:
						throw std::invalid_argument("invalid rule: " + li_ne);
						break;
				}
				
			}
		}
		/* CASE "ERROR_PAGE" */
		else if(l_ine == "error_page"){
			f.getline(line, 10000, '}');
			std::stringstream content(line);
			std::string const rules[2] = {"error_page", ""};

			while (!content.eof())
			{
				content.getline(line, 10000, ':');
				std::string li_ne(line);
				li_ne.erase(remove_if(li_ne.begin(), li_ne.end(), isspace), li_ne.end());
				int pos = 0;
				for (size_t i = 0; i < 2; i++)
				{
					if (li_ne == rules[i]){
						pos = i + 1;
						break;
					}
				}
				switch (pos)
				{
					case 1:
						content.getline(line, 10000, ';');
						{
							if (content.eof())
									throw std::invalid_argument("no ';' found");
							std::vector<size_t> codes;
							li_ne = line;
							char *end;
							for(size_t j = 0; line[j]; j++)
							{
								if (isdigit(line[j])){
									size_t c = strtol(&line[j], &end, 10);
									if (!std::isspace(*end) || (!isInBounds<size_t>(c, 100, 103) && !isInBounds<size_t>(c, 200, 208) && !isInBounds<size_t>(c, 300, 308)
										&& !isInBounds<size_t>(c, 400, 451) && !isInBounds<size_t>(c, 500, 511)))
										throw std::invalid_argument("invalid error code");
									j += 3;
									codes.push_back(c);
								}
								else if(isalpha(line[j])){
									std::string temp(&line[j]);
									removeDuplWhitespace(temp);
									if (temp.find_first_of(' ') != temp.npos)
										throw std::invalid_argument("only one filepath allowed");
									for (std::vector<size_t>::iterator it = codes.begin(); it != codes.end(); it++)
										gconf->error_pages->insert(std::make_pair(*it, &line[j]));
									break;
								}
							}
						}
						break;
					case 2:
						break;
					default:
						throw std::invalid_argument("invalid rule: " + li_ne);
						break;
				}
			}
		}
		/* CASE CGI */
		else if(l_ine == "CGI"){
			f.getline(line, 10000, '}');
			std::stringstream content(line);
			std::string const rules[2] = {"extension", ""};

			while (!content.eof())
			{
				content.getline(line, 10000, ':');
				std::string li_ne(line);
				li_ne.erase(remove_if(li_ne.begin(), li_ne.end(), isspace), li_ne.end());
				int pos = 0;
				for (size_t i = 0; i < 2; i++)
				{
					if (li_ne == rules[i]){
						pos = i + 1;
						break;
					}
				}
				switch (pos)
				{
					case 1:
						content.getline(line, 10000, '(');
						if (content.eof())
							throw std::invalid_argument("expected '('");
						{
							std::string Fext;
							std::string Fpath;
							Fext = line;
							removeDuplWhitespace(Fext);
							if (Fext.find_first_of(" \t\n\v\f\r") != Fext.npos)
								throw std::invalid_argument("only one file extension per rule allowed");
							content.getline(line, 10000, ')');
							if (content.eof())
								throw std::invalid_argument("expected ')'");
							Fpath = line;
							removeDuplWhitespace(Fpath);
							int pos = Fpath.find_first_of(';');
							if (pos == static_cast<int>(Fpath.npos))
								throw std::invalid_argument("no ';' found");
							std::string paths = Fpath.substr(0, pos);
							gconf->CGI->insert(std::make_pair(Fext, paths));
						}
						break;
					case 2:
						break;
					default:
						throw std::invalid_argument("invalid rule: " + li_ne);
						break;
				}
			}
		}
		else if (l_ine == ""){}
		else
			throw std::invalid_argument("invalid rule: " + l_ine);
	}
	return co;
}

// int main(int ac, char **av){

// 	std::vector<conf_data*> *co;
// 	t_gconf *gconf = new t_gconf;
// 	gconf->error_pages = new std::map<size_t, std::string>();
// 	gconf->CGI = new std::map<std::string, std::string>();

// 	try
// 	{
// 		if (ac == 2)
// 			co = readConfFile(gconf, av[1]);
// 		else
// 			throw std::invalid_argument("Invalid argument");
// 		validate(co, gconf);
// 	}
// 	catch(const std::exception& e)
// 	{
// 		Color::Modifier f_red(Color::Red);
// 		Color::Modifier reset(Color::White, 0, 1);
// 		std::cerr << f_red << "\nError: " << e.what() << '\n';
// 		std::cerr << "Initiating with default settings" << reset <<"\n\n" ;
// 		gconf->error_pages->clear();
// 		gconf->CGI->clear();
// 		co = readConfFile(gconf);
// 	}
	
// 	Color::Modifier f_green(Color::SeaGreen2);
// 	Color::Modifier f_magenta(Color::Magenta3_1);
// 	Color::Modifier f_yellow(Color::Yellow2);
// 	Color::Modifier f_grey_b(Color::Grey93, 1);
// 	Color::Modifier f_grey_lighter(Color::Grey100);
// 	Color::Modifier reset(Color::White, 0, 1);
// 	std::vector<conf_data*>::iterator it = co->begin();
// 	for (size_t i = 0; i < co->size(); i++)
// 	{
// 		std::cout << f_grey_b << "\nSERVER: " << i + 1 << " {" << reset
// 		<< f_magenta << "\n\n\tserver_names: " << f_green << (*it)->s_names() 
// 		<< f_magenta << "\n\tport: " << f_green << (*it)->s_port()
// 		<< f_magenta << "\n\troot_dir: " << f_green << (*it)->s_root()
// 		<< f_magenta << "\n\tallowed methods: " << f_green << (*it)->s_methods()
// 		<< f_magenta << "\n\tHTTP redirection(code, URL): " << f_green << (*it)->s_HTTP_redir().first << ", " << (*it)->s_HTTP_redir().second
// 		<< f_yellow << "\n\n\tdirectory_listing on for:\n" << f_green;
// 		for (std::vector<std::string>::const_iterator i = (*it)->s_listing().begin(); i != (*it)->s_listing().end(); i++)
// 			std::cout << "\t\t" << *i << std::endl;
// 		std::cout << f_magenta << "\n\thost: " << f_green << (*it)->s_host() << std::endl << std::endl;
// 		(*it)->printFileLocations();
// 		(*it)->printDefaultAnswers();
// 		(*it)->printErrorCodes();
// 		std::cout << f_grey_b << "}" << std::endl << std::endl;
// 		++it;
// 	}
	
// 	Color::Modifier f_blue(Color::DeepSkyBlue2);
// 	std::cout << f_grey_b << "DEFAULT ERROR CODES\n\n" << reset;
// 	for (std::map<size_t, std::string>::iterator i = gconf->error_pages->begin(); i != gconf->error_pages->end(); i++)
// 	{
// 		std::cout << f_blue << "default_error_code: " << reset << i->first 
// 				  << f_blue << " path: " << reset << i->second << std::endl;
// 	}

// 	std::cout << f_grey_b << "\nCGI PATHS\n\n" << reset;

// 	for (std::map<std::string, std::string>::iterator i = gconf->CGI->begin(); i != gconf->CGI->end(); i++)
// 	{
// 		std::cout << f_blue << "CGI file extension: " << reset << i->first 
// 				  << f_blue << " paths: " << reset << i->second << std::endl;
// 	}
	
// 	for (std::vector<conf_data*>::iterator ite = co->begin(); ite != co->end(); ++ite)
// 		delete *ite;
	
// 	delete co;
// 	delete gconf->error_pages;
// 	delete gconf->CGI;
// 	delete gconf;
// 	return 0;
// }
