#ifndef config_hpp
#define config_hpp

#include "server.hpp"

class Server;

/* returns true if 'value' belongs in range between 'low' and 'high', or is equal to 'high'. Otherwise returns false */
template <typename T>
bool isInBounds(const T& value, const T& low, const T& high) {
	return !(value < low) && (value <= high);
}

std::string &removeDuplWhitespace(std::string &str);
bool IsPathsDir(std::string const &str);

//TEXT COLOR CHANGE
namespace Color {
    enum Code {
		Black = 0,
		Maroon = 1,
		Green = 2,
		Olive = 3,
		Navy = 4,
		Purple = 5,
		Teal = 6,
		Silver = 7,
		Grey0 = 8,
		Red = 9,
		Lime = 10,
		Yellow = 11,
		Blue = 12,
		Fuchsia = 13,
		Aqua = 14,
		White = 15,
		Grey = 16,
		NavyBlue = 17,
		DarkBlue = 18,
		Blue3 = 19,
		Blue3_1 = 20,
		Blue1  = 21,
		DarkGreen = 22,
		DeepSkyBlue4 = 23,
		DeepSkyBlue4_1 = 24,
		DeepSkyBlue4_2 = 25,
		DodgerBlue3 =26,
		DodgerBlue2 = 27,
		Green4 = 28,
		SpringGreen4 = 29,
		Turquoise4 = 30,
		DeepSkyBlue3 = 31,
		DeepSkyBlue3_1 = 32,
		DodgerBlue1 = 33,
		Green3 = 34,
		SpringGreen3 = 35,
		DarkCyan = 36,
		LightSeaGreen = 37,
		DeepSkyBlue2 = 38,
		DeepSkyBlue	 = 39,
		Green3_1 = 40,
		SpringGreen = 41,
		SpringGreen2 = 42,
		Cyan3 = 43,
		DarkTurquoise = 44,
		Turquoise2 = 45,
		Green1 = 46,
		SpringGreen2_1 = 47,
		SpringGreen1 = 48,
		MediumSpringGree = 49,
		Cyan2 = 50,
		Cyan1 = 51,
		DarkRed = 52,
		DeepPink4 = 53,
		Purple4 = 54,
		Purple_3 = 55,
		Purple3 = 56,
		BlueViolet = 57,
		Orange4 = 58,
		Grey37 = 59,
		MediumPurple4 = 60,
		SlateBlue3 = 61,
		SlateBlue3_1 = 62,
		RoyalBlue1 = 63,
		Chartreuse4 = 64,
		DarkSeaGreen4 = 65,
		PaleTurquoise4 = 66,
		SteelBlue = 67,
		SteelBlue3 = 68,
		CornflowerBlue = 69,
		Chartreuse3 = 70,
		DarkSeaGreen4_1 = 71,
		CadetBlue = 72,
		CadetBlue_1 = 73,
		SkyBlue3 = 74,
		SteelBlue1 = 75,
		Chartreuse3_1 = 76,
		PaleGreen3 = 77,
		SeaGreen3 = 78,
		Aquamarine3 = 79,
		MediumTurquoise = 80,
		SteelBlue1_1 = 81,
		Chartreuse2 = 82,
		SeaGreen2 = 83,
		SeaGreen1 = 84,
		SeaGreen1_1 = 85,
		Aquamarine1_1 = 86,
		DarkSlateGray2 = 87,
		DarkRed_1 = 88,
		DeepPink4_1 = 89,
		DarkMagenta = 90,
		DarkMagenta_1 = 91,
		DarkViolet = 92,
		Purple_1 = 93,
		Orange4_1 = 94,
		LightPink4 = 95,
		Plum4 = 96,
		MediumPurple3 = 97,
		MediumPurple3_1 = 98,
		SlateBlue1 = 99,
		Yellow4 = 100,
		Wheat4 = 101,
		Grey53 = 102,
		LightSlateGrey = 103,
		MediumPurple = 104,
		LightSlateBlue = 105,
		Yellow4_1 = 106,
		DarkOliveGreen3 = 107,
		DarkSeaGreen = 108,
		LightSkyBlue3 = 109,
		LightSkyBlue3_1 = 110,
		SkyBlue2 = 111,
		Chartreuse2_1 = 112,
		DarkOliveGreen3_2 = 113,
		PaleGreen3_1 = 114,
		DarkSeaGreen3 = 115,
		DarkSlateGray3 = 116,
		SkyBlue1 = 117,
		Chartreuse1 = 118,
		LightGreen = 119,
		LightGreen_1 = 120,
		PaleGreen1 = 121,
		Aquamarine1_2 = 122,
		DarkSlateGray = 123,
		Red3 = 124,
		DeepPink4_2 = 125,
		MediumVioletRed = 126,
		Magenta3 = 127,
		DarkViolet_1 = 128,
		Purple_2 = 129,
		DarkOrange3 = 130,
		IndianRed = 131,
		HotPink3 = 132,
		MediumOrchid3 = 133,
		MediumOrchi = 134,
		MediumPurple2 = 135,
		DarkGoldenrod = 136,
		LightSalmon3 = 137,
		RosyBrown = 138,
		Grey63 = 139,
		MediumPurple_1 = 140,
		MediumPurple_2 = 141,
		Gold3 = 142,
		DarkKhaki = 143,
		NavajoWhite = 144,
		Grey69 = 145,
		LightSteelBlue3 = 146,
		LightSteelBlue = 147,
		Yellow3 = 148,
		DarkOliveGreen3_1 = 149,
		DarkSeaGreen3_1 = 150,
		DarkSeaGreen2 = 151,
		LightCyan3 = 152,
		LightSkyBlue1 = 153,
		GreenYellow = 154,
		DarkOliveGreen2 = 155,
		PaleGreen1_1 = 156,
		DarkSeaGreen2_1 = 157,
		DarkSeaGreen1 = 158,
		PaleTurquoise1 = 159,
		Red3_1 = 160 ,
		DeepPink3 = 161,
		DeepPink3_1 = 162,
		Magenta3_1 = 163,
		Magenta3_2 = 164,
		Magenta2 = 165,
		DarkOrange3_1 = 166,
		IndianRed_1 = 167,
		HotPink3_1 = 168,
		HotPink2 = 169,
		Orchid = 170,
		MediumOrchid1 = 171,
		Orange3 = 172,
		LightSalmon = 173,
		LightPink3 = 174,
		Pink3 = 175,
		Plum3 = 176,
		Violet = 177,
		Gold3_1 = 178,
		LightGoldenrod3 = 179,
		Tan = 180,
		MistyRose3 = 181,
		Thistle3 = 182,
		Plum2 = 183,
		Yellow3_1 = 184,
		Khaki3 = 185,
		LightGoldenrod2 = 186,
		LightYellow3 = 187,
		Grey84 = 188,
		LightSteelBlue1 = 189,
		Yellow2 = 190,
		DarkOliveGreen1 = 191,
		DarkOliveGreen1_1 = 192,
		DarkSeaGreen1_1 = 193,
		Honeydew2 = 194,
		LightCyan1 = 195,
		Red1 = 196,
		DeepPink2 = 197,
		DeepPink1 = 198,
		DeepPink1_1 = 199,
		Magenta2_1 = 200,
		Magenta1 = 201,
		OrangeRed1 = 202,
		IndianRed1 = 203,
		IndianRed1_1 = 204,
		HotPink = 205,
		HotPink_1 = 206,
		MediumOrchid1_1 = 207,
		DarkOrange = 208,
		Salmon1 = 209,
		LightCoral = 210,
		PaleVioletRed1 = 211,
		Orchid2 = 212,
		Orchid1 = 213,
		Orange1 = 214,
		SandyBrown = 215,
		LightSalmon1 = 216,
		LightPink1 = 217,
		Pink1 = 218,
		Plum1 = 219,
		Gold1 = 220,
		LightGoldenrod2_1 = 221,
		LightGoldenrod2_2 = 222,
		NavajoWhite1 = 223,
		MistyRose1 = 224,
		Thistle1 = 225,
		Yellow1 = 226,
		LightGoldenrod1 = 227,
		Khaki1 = 228 ,
		Wheat1 = 229,
		Cornsilk1 = 230,
		Grey100 = 231,
		Grey3 = 232,
		Grey7 = 233,
		Grey11 = 234,
		Grey15 = 235,
		Grey19 = 236,
		Grey23 = 237,
		Grey27 = 238,
		Grey30 = 239,
		Grey35 = 240,
		Grey39 = 241,
		Grey42 = 242,
		Grey46 = 243,
		Grey50 = 244,
		Grey54 = 245,
		Grey58 = 246,
		Grey62 = 247,
		Grey66 = 248,
		Grey70 = 249,
		Grey74 = 250,
		Grey78 = 251,
		Grey82 = 252,
		Grey85 = 253,
		drey89 = 254,
		Grey93 = 255
    };
    class Modifier {
		private:
			Code code;
			bool bold;
			bool reset;
		public:
			Modifier(Code pCode, bool bold = false, bool reset = false) : code(pCode), bold(bold), reset(reset) {}
			friend std::ostream &operator<<(std::ostream& os, const Modifier& mod){
				if (mod.bold)
					return os << "\033[38;5;" << mod.code << ";1m";
				else if (mod.reset)
					return os << "\033[0m";
				else
					return os << "\033[38;5;" << mod.code << "m";
			}
    };
}
///////////////////

typedef struct s_gconf{
	std::map<std::string, std::string> *CGI;
	std::map<size_t, std::string> *error_pages;
}t_gconf;

class conf_data{

	private:
		std::string server_names; //
		std::string root;//
		std::string host;//
		std::string methods;//
		std::map<size_t, std::string> error_pages;//
		std::map<std::string, std::string> file_locations;
		std::map<std::string, std::string> def_answer_if_dir;
		size_t port;//
		std::vector<std::string> listing;
		std::string const empty_string;
		std::map<std::string, std::string> redir_url;
	public:

		std::string CGI_extensions;
		std::string full_file_path;

		conf_data() :server_names("EKM_amazing_server"), root(""), host("127.0.0.1"), methods("GET_POST_DELETE"), port(4242)
		{
			std::stringstream ss;
			char ptr[1000];
			getcwd(ptr, 1000);
			root = ptr;
			root += "/";
		}

		conf_data(conf_data const & other){
			this->server_names = other.server_names;
			root = other.root;
			host = other.host;
			methods = other.methods;
			error_pages = other.error_pages;
			file_locations = other.file_locations;
			def_answer_if_dir = other.def_answer_if_dir;
			port = other.port;
			redir_url = other.redir_url;
			listing = other.listing;
		}
		virtual~conf_data(){}

		void clear(){ //reset object to default 
			server_names = "EKM_amazing_server";
			root = "";
			host = "localhost";
			port = 4242;
			methods.clear();
			error_pages.clear();
			file_locations.clear();
			def_answer_if_dir.clear();
			redir_url.clear();
			listing.clear();
		}

		//SET CONFIGURATION DATA//
		void addServerNames(std::string const &s){
			if (server_names != "ECM_amazing_server")
				server_names = "";
			server_names += " ";
			server_names += s;
			server_names = removeDuplWhitespace(server_names);
		}
		void addFilesToLocation(std::string const &path, std::string const &file){
			std::map<std::string, std::string>::iterator it = file_locations.find(path);
			if (it != file_locations.end()) { (*it).second += ' '; (*it).second += file; }
			else file_locations.insert(std::make_pair(path, file));
		}

		//ACCESS CONFIGURATION DATA//
		std::string const &s_names() const {
			return server_names;
		}
		std::string const &s_root() const {
			return root;
		}
		size_t s_port() const {
			return this->port;
		}
		std::vector<std::string> const &s_listing() const {
			return this->listing;
		}
		std::string const &s_host() const {
			return this->host;
		}
		std::string const &s_methods() const {
			return this->methods;
		}
		std::map<size_t, std::string> const &s_errorCodes() const {
			return error_pages;
		}
		std::map<std::string, std::string>  &s_fileLocations()  {
			return file_locations;
		}
		std::map<std::string, std::string> const &s_defAnswers() const {
			return def_answer_if_dir;
		}
		std::map<std::string, std::string> const &s_HTTP_redir() const {
			return this->redir_url;
		}
		//////////////////

		void printErrorCodes(){
			Color::Modifier f_bold_ggold(Color::LightGoldenrod2_1, 1);
			Color::Modifier f_green(Color::SeaGreen2);
			Color::Modifier f_magenta(Color::Magenta3_1);
			Color::Modifier reset(Color::NavajoWhite1, 0, 1);

			std::cout << f_bold_ggold << "\tPreset error codes:\n\n" << reset;
			for (std::map<size_t, std::string>::iterator i = error_pages.begin(); i != error_pages.end(); i++)
				std::cout << f_magenta << "\tserver_block_error_code: " << f_green << i->first
						  << f_magenta << " page: " << f_green << i->second << reset << std::endl;
		}
		void printFileLocations(){
			Color::Modifier f_green(Color::SeaGreen2);
			Color::Modifier f_magenta(Color::Magenta3_1);
			Color::Modifier f_bold_gold(Color::LightGoldenrod2_1, 1);
			Color::Modifier reset(Color::NavajoWhite1, 0, 1);

			std::cout << f_bold_gold << "\tPreset filepaths:\n\n" << reset;
			for (std::map<std::string, std::string>::iterator i = file_locations.begin(); i != file_locations.end(); i++)
				std::cout << f_magenta << "\tpath: " << f_green << i->first
						  << f_magenta << " filename(s): " << f_green << i->second << reset << std::endl;
			std::cout << std::endl;
		}
		void printDefaultAnswers(){
			Color::Modifier f_green(Color::SeaGreen2);
			Color::Modifier f_bold_gold(Color::LightGoldenrod2_1, 1);
			Color::Modifier f_magenta(Color::Magenta3_1);
			Color::Modifier reset(Color::NavajoWhite1, 0, 1);

			std::cout << f_bold_gold << "\tDefault answers to filepaths:\n\n" << reset;
			for (std::map<std::string, std::string>::iterator i = def_answer_if_dir.begin(); i != def_answer_if_dir.end(); i++)
				std::cout << f_magenta << "\tpath: " << f_green << i->first
						  << f_magenta << " answer file(s): " << f_green << i->second << reset << std::endl;
			std::cout << std::endl;
		}

		/* Searches file_locations map for "file". If file exists, full_file_path is set to the path to the file, as set in the configuration file,
			and a reference to it is returned. If it does not exist, full_file_path is set to "file" and a reference to it is returned. */
		std::string &fileLocationParser(std::string const &file) {
			std::map<std::string, std::string>::const_iterator it;
			std::string token;
			for (it = file_locations.begin(); it != file_locations.end(); ++it){
				std::string buff(it->second);
				size_t pos = 0;
				 while ((pos = buff.find(' ')) != buff.npos){
					 token = buff.substr(0, pos);
					 if (token == file)
					 	return full_file_path = it->first + token;
					buff.erase(0, pos + 1);
				 }
				 if (buff == file)
				 	return full_file_path = it->first + buff;
			}
			return full_file_path = file;
		}
		/* Returns the error page path, matching a specific error code. An exception is thrown if the error code is invalid. If no path is specifed in the .conf file,
			returns a reference to an empty string instead */
		std::string const &findErrorPage(size_t error_code){
			if (!isInBounds<size_t>(error_code, 100, 103) && !isInBounds<size_t>(error_code, 200, 208) && !isInBounds<size_t>(error_code, 300, 308)
				&& !isInBounds<size_t>(error_code, 400, 451) && !isInBounds<size_t>(error_code, 500, 511))
				throw std::invalid_argument("invalid error code");
			std::map<size_t, std::string>::const_iterator it;
			if ((it = error_pages.find(error_code)) != error_pages.end())
				return it->second;
			return empty_string;
		}
		/* Returns a reference to the filepath of the 1st file specified under the "index" label in a "location" block. Subsequent calls
			with the same filepath will return any additional files provided under the "index" label or an empty string after the call to the last file.
			Optional argument "reset": if set to true, resets the function's tracking of the specific filepath. NOTE: The function call with the reset set
			to true counts as the 1st call */
		std::string &findDefaultAnswerToFilepath(std::string const &filepath, bool reset = false){
			std::map<std::string, std::string>::const_iterator it;
			static std::string filepath_buff(filepath);

			if ((it = def_answer_if_dir.find(filepath)) != def_answer_if_dir.end()){
				static std::string buff(it->second);
				if (filepath_buff != filepath || reset == true){
					filepath_buff = filepath;
					buff = it->second;
				}
				size_t pos = buff.find(' ');
				std::string token = buff.substr(0, pos);
				buff.erase(0, pos + 1);
				if (buff == "")
					return full_file_path = "";
				if (buff == token)
					buff.clear();
				return full_file_path = token;
			}
			return full_file_path = "";
		}

	friend std::vector<conf_data*> *readConfFile(t_gconf *gconf, std::string const &file);
};

std::vector<conf_data*> *readConfFile(t_gconf *gconf, std::string const &file);
void validate(std::vector<conf_data*> *d, t_gconf *c);


#endif
