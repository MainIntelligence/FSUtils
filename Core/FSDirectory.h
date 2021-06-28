#pragma once
#include <vector>
#include <string>

struct Directory {
	std::vector<Directory*> pSubdirs;
	std::string dirname;
	std::vector<std::string> files;
	
	Directory();
	//load the files and stuff too.
	Directory(std::string name);
	
	void ReloadFiles();
	void ReloadRecursive();
	
	//create a new file in this directory
	void AddFile(const char* str);
	
	//create a new subdirectory within this directory
	int AddSubdirectory(std::string str);
	void Display();
};

char* GetCwd(char *buff, int cap);
void GetContents(const char * dirname, std::vector<std::string>& files, std::vector<std::string>& dirs);
void GetFileContents(const char * dirname, std::vector<std::string>& files);
