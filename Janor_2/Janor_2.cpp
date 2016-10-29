// Janor_2.cpp : Defines the entry point for the console application.
// set a list for all symbols to be ignored
//

#include "stdafx.h"

#define FILE_EXTENSION ".jnr"
#define JANOR_HEADER "<?JANOR"
#define FIRST_LINE_OF_FILE file_contents[0]
#define JANOR_TAG '$'

namespace filesystem = std::tr2::sys;
int LINE_NUMBER = 1;
int LINE_WITH_ERROR;
std::string CURRENT_TAG;
void check_dir_status(filesystem::path &dir, int &err_no);
void handle_dir_errors(int &err_no);
void parse_file_contents(std::vector<std::string>&file_contents,int &err_no,std::vector<std::string>&Tags,std::vector<char>&symbols);
void parse_file_header(std::vector<std::string>&file_contents,int &err_no);
void handle_file_parsing_errors(int &err_no);
void check_for_other_existing_headers(std::vector<std::string>&file_contents, int &err_no);
std::string merge_all_tags(std::vector<std::string>&Tags);
void handle_functions(std::string &j_function, int &err_no,std::vector<std::string>&Tags);
std::string create_new_header_without_spaces(std::string &func);
void process_function(int &instruction,std::string &contents);
std::string merge_symbols(std::vector<char>&symbols);

enum errors{
	DIRECTORY_INVALID,
	DIRECTORY_EMPTY,
	DIRECTORY_NOT_CONTAINING_JANOR_FILES,
	FILE_DOES_NOT_CONTAIN_JANOR_HEADER,
	FILE_CANNOT_CONTAIN_MORE_THAN_ONE_HEADER,
	FILE_CONTAINS_INVALID_TAGS
};

int parse_no;

enum parse_instructions {
	UPPER,
	LOWER,
	REVERSE
};

int main()
{
	int err_no;
	std::string directory;
	std::string filename;
	std::string contents;
	std::vector<std::string>file_contents;
	std::vector<std::string>Tags = {
		"upper","lower","reverse"
	};
	std::vector<char>symbols = {
		'$',
	};

	std::cout << "Enter file directory: " << std::flush;
	std::getline(std::cin, directory);

	filesystem::path dir = directory;
	
	check_dir_status(dir, err_no);

	handle_dir_errors(err_no);

	std::cout << "Enter filename: " << std::flush;

	std::getline(std::cin, filename);

	std::ifstream file;

	file.open((directory + "\\" + filename).c_str());

	if (!file.is_open()) {
		std::cout << "Could not open file. Check if file exists" << '\n';
		system("pause");
		return 4;
	}

	while (!file.eof()) {
		std::getline(file, contents);
		file_contents.push_back(contents);
	}

	parse_file_header(file_contents,err_no);

	check_for_other_existing_headers(file_contents, err_no);

	parse_file_contents(file_contents,err_no,Tags,symbols);

	handle_file_parsing_errors(err_no);

	file.close();
	system("pause");
    return 0;
}

void handle_dir_errors(int &err_no) {
	switch (err_no)
	{
	case errors::DIRECTORY_INVALID:
		std::cout << "Directory does not exist" <<'\n';
		system("pause");
		exit(0);
		break;
	case errors::DIRECTORY_EMPTY:
		std::cout << "Directory empty" << '\n';
		system("pause");
		exit(1);
		break;
	case errors::DIRECTORY_NOT_CONTAINING_JANOR_FILES:
		std::cout << "Directory not containing .jnr files" << '\n';
		system("pause");
		exit(2);
		break;
	default:
		std::cout << "Directory set" << std::endl;
		break;
	}
}
void check_dir_status(filesystem::path &dir, int &err_no) {
	if (!exists(dir)) {
		err_no = errors::DIRECTORY_INVALID;
	}
	else if (is_empty(dir)) {
		err_no = errors::DIRECTORY_EMPTY;
	}
	else {
		for (const auto num : filesystem::directory_iterator(dir)) {
			filesystem::path p = num;
			if (p.extension() != FILE_EXTENSION) {
				err_no = errors::DIRECTORY_NOT_CONTAINING_JANOR_FILES;
			}
		}
	}
}
void parse_file_header(std::vector<std::string>&file_contents, int & err_no) {
	std::stringstream ss;
	std::for_each(file_contents[0].begin(), file_contents[0].end(), [&](char character) {
		if (!isspace(character)) {
			ss << character;
		}
	});
	file_contents[0] = ss.str();
	if (FIRST_LINE_OF_FILE != JANOR_HEADER) {
		err_no = errors::FILE_DOES_NOT_CONTAIN_JANOR_HEADER;
	}
}
void parse_file_contents(std::vector<std::string>&file_contents,int &err_no,std::vector<std::string>&Tags,std::vector<char>&symbols) {
	for (size_t i = 1; i < file_contents.size(); i++,LINE_NUMBER++) {
		if (ispunct(file_contents[i][0]) && merge_symbols(symbols).find(file_contents[i][0])!=std::string::npos) {
				handle_functions(file_contents[i], err_no, Tags);
		}
		else if (file_contents[i][0]=='/'&&file_contents[i][1]=='/') {
			continue;
		}
		else {
			if (CURRENT_TAG == "$upper") {
				parse_no = parse_instructions::UPPER;
				process_function(parse_no,file_contents[i]);
			}
			else if (CURRENT_TAG == "$lower") {
				parse_no = parse_instructions::LOWER;
				process_function(parse_no,file_contents[i]);
			}
			else if (CURRENT_TAG == "$reverse") {
				parse_no = parse_instructions::REVERSE;
				process_function(parse_no,file_contents[i]);
			}
		}
	}
}
void handle_file_parsing_errors(int &err_no) {
	switch (err_no)
	{
	case errors::FILE_DOES_NOT_CONTAIN_JANOR_HEADER:
		std::cout << "File does not contain <?janor" << '\n';
		system("pause");
		exit(5);
		break;
	case errors::FILE_CANNOT_CONTAIN_MORE_THAN_ONE_HEADER:
		std::cout << "File cannot contain more than one <?janor header" << '\n';
		system("pause");
		exit(6);
	case errors::FILE_CONTAINS_INVALID_TAGS:
		std::cout << "Code contains invalid tags in line: " <<LINE_WITH_ERROR+1<< '\n';
		system("pause");
		exit(7);
		break;
	default:
		std::cout << "Code Compiled" << '\n';
		break;
	}
}
void check_for_other_existing_headers(std::vector<std::string>&file_contents,int &err_no) {
	std::stringstream ss;
	for (size_t i = 1; i < file_contents.size(); i++) {
		ss << file_contents[i];
		ss << '\n';
	}
	std::regex pattern("<?JANOR");
	if (std::regex_search(ss.str(), pattern)) {
		err_no = FILE_CANNOT_CONTAIN_MORE_THAN_ONE_HEADER;
	}
}
std::string merge_all_tags(std::vector<std::string>&Tags) {
	std::stringstream ss;
	for (const auto tag : Tags) {
		ss << JANOR_TAG;
		ss << tag;
		ss << " ";
	}
	return ss.str();
}
void handle_functions(std::string &j_function, int &err_no,std::vector<std::string>&Tags) {
	std::stringstream ss;
	for (size_t i = 0; i < j_function.size(); i++) {
		if (!isspace(j_function[i])) {
			ss << j_function[i];
		}
	}
	int postion = merge_all_tags(Tags).find(ss.str());
	if (postion == std::string::npos) {
		std::cout << "Tag: " << ss.str() << " is not a janor tag" << '\n';
		err_no = errors::FILE_CONTAINS_INVALID_TAGS;
		LINE_WITH_ERROR = LINE_NUMBER;
	}
	else {
		CURRENT_TAG = ss.str();
	}
}
std::string create_new_header_without_spaces(std::string&func) {
	std::stringstream ss;
	for(size_t i=0;i<func.length();i++){
		if (!isspace(func[i])) {
			ss << func[i];
		}
	};
	return ss.str();
}
void process_function(int &instruction,std::string &content) {
	switch (instruction)
	{
	case parse_instructions::UPPER:
		for (size_t i = 0; i < content.size(); i++) {
			std::cout << static_cast<char>(toupper(content[i]));
		}
		std::cout << '\n';
		break;
	case parse_instructions::LOWER:
		for (size_t i = 0; i < content.size(); i++) {
			std::cout << static_cast<char>(tolower(content[i]));
		}
		std::cout << '\n';
		break;
	case parse_instructions::REVERSE:
		std::reverse(std::begin(content),std::end(content));
		for (size_t i = 0; i < content.size(); i++) {
			std::cout << content[i];
		}
		std::cout << '\n';
		break;
	default:
		break;
	}
}
std::string merge_symbols(std::vector<char>&symbols) {
	std::stringstream ss;
	for (const auto sym : symbols) {
		ss << sym;
		ss << " ";
	}
	return ss.str();
}
