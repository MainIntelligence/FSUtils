#include "FSDirectory.h"
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>


Directory::Directory() {
	char tmp[256];
	getcwd(tmp, 256);
	dirname = tmp;
	
	std::vector<std::string> subdirs {};
	GetContents(dirname.c_str(), files, subdirs);
	for ( int i = 0; i < subdirs.size(); i++ ) {
		pSubdirs.emplace_back(new Directory(dirname + '/' + subdirs[i]));
	}
}

Directory::Directory(std::string name) : dirname(name) {
	std::vector<std::string> subdirs {};
	GetContents(dirname.c_str(), files, subdirs);
	for ( int i = 0; i < subdirs.size(); i++ ) {
		pSubdirs.emplace_back(new Directory(dirname + '/' + subdirs[i]));
	}
}

void Directory::ReloadFiles() {
	GetFileContents(dirname.c_str(), files);
};

//ASSUMPTIONS: FILES IN FILESYSTEM ARE NOT DELETED EXTERNALLY FROM APPLICATION.
void Directory::ReloadRecursive() {
	std::vector<std::string> subdirs;
	GetContents(dirname.c_str(), files, subdirs);
	//hoping there will be consistency in order of filename production here.
	
	std::vector<Directory*> newsubdirs;
	
	int ia = 0;
	int in = 0;
	for ( int i = 0; i != subdirs.size() ; i++) {
		if (pSubdirs[ia]->dirname != subdirs[i]) {
			//either a file was removed (pSubdirs needs to remove an element)
			//Or a file was added (pSubdirs needs to insert at ia,
			//Ah shit, I'm gonna need a hash or atleast a map. (TODO)
			newsubdirs.emplace_back(new Directory(dirname + '/' + std::move(subdirs[i])));
		}
		newsubdirs.emplace_back(pSubdirs[ia]);
	}
	pSubdirs = std::move(newsubdirs);
}

//create a new file in this directory
void Directory::AddFile(const char* str) {
	FILE *fp = fopen(str,"a");
	if (fp != NULL) {
		fclose(fp);
		ReloadFiles();
	}
}
//create a new subdirectory within this directory
int Directory::AddSubdirectory(std::string str) {
    int status = mkdir((dirname + '/' + str).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    
    return mkdir((dirname + '/' + str).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void Directory::Display() {
    fprintf(stdout,		"~%s~\n", dirname.c_str());
    fprintf(stdout, 		"|__________________________________________________.\n");
    fprintf(stdout, 		"|    SUBDIRECTORIES                                 \n");
    fprintf(stdout, 		"|__________________________________________________ \n");
    for (int i = 0; i != pSubdirs.size() ; i++) {
    	fprintf(stdout, 	"|-%s\n", pSubdirs[i]->dirname.c_str());
    }
    fprintf(stdout, 		"|__________________________________________________.\n");
    fprintf(stdout, 		"|    FILES                                          \n");
    fprintf(stdout, 		"|__________________________________________________ \n");
    for (int i = 0; i != files.size() ; i++) {
    	fprintf(stdout, 	"|-%s\n", files[i].c_str());
    }
    fprintf(stdout, 		"|\n");
    
}


char * GetCwd(char *buff, int cap) {
	getcwd(buff, cap);
	return buff;
}

void GetContents(const char * dirname, std::vector<std::string>& files, std::vector<std::string>& dirs) {
	files.clear();
	dirs.clear();
	
	struct dirent * ent;
	DIR *dirp = opendir(dirname);
	
	while ( (ent = readdir(dirp)) != nullptr ) {
		if (ent->d_type == DT_DIR) {
			if (*(ent->d_name) == '.') { continue; }
			dirs.push_back(std::string(ent->d_name));
			continue;
		}
		files.push_back(std::string(ent->d_name));
        }
        
	closedir(dirp);
}

void GetFileContents(const char * dirname, std::vector<std::string>& files) {
	files.clear();
	
	struct dirent * ent;
	DIR * dirp = opendir(dirname);
	
	while ( (ent = readdir(dirp)) != nullptr ) {
		if (ent->d_type == DT_DIR) { continue; }
		files.push_back(std::string(ent->d_name));
        }
        
	closedir(dirp);
}
