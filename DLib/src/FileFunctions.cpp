/*
 * File: FileFunctions.cpp
 * Author: Dorian Galvez-Lopez
 * Date: June 2009
 * Description: file system functions
 * License: see the LICENSE.txt file
 *
 */

#include "FileFunctions.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;
using namespace DUtils;

bool FileFunctions::MkDir(const char *path)
{
//    if(mkdir(path, 0755) == -1) {
//        return false;
//    }

    if(system(("mkdir -p " + std::string(path)).c_str()) < 0) {
        cout << "Failed to mkdir: " << path << endl;
        return false;
    }

    return true;
}

bool FileFunctions::RmDir(const char *path)
{
	// empty path
	vector<string> files = FileFunctions::Dir(path, "");
	for(vector<string>::iterator it = files.begin(); it != files.end(); it++){
		remove(it->c_str());
	}

//    if(rmdir(path) < 0) {
//        cout << "Failed to rmdir: " << path << endl;
//        return false;
//    }
    if(system(("exec rm -r " + std::string(path) + "*").c_str()) < 0) {
        cout << "Failed to rmdir: " << path << endl;
        return false;
    }

    return true;
}

bool FileFunctions::RmFile(const char *path)
{
    if(remove(path) < 0) {
        cout << "Failed to RmFile: " << path << endl;
        return false;
    }

//    if(system(("exec rm " + std::string(path)).c_str()) < 0) {
//        cout << "Failed to RmFile: " << path << endl;
//        return false;
//    }

    return true;
}

bool FileFunctions::FileExists(const char *filename)
{
//	std::fstream f(filename, ios::in);

//	if(f.is_open()){
//		f.close();
//		return true;
//	}else
//		return false;
    if( access( filename, F_OK ) != -1 ) {
        // file exists
        return true;
    }
    else {
        // file doesn't exist
        return false;
    }
}

bool FileFunctions::DirExists(const char *path)
{
    DIR *dirp;
	if((dirp = opendir(path)) != NULL){
		closedir(dirp);
		return true;
	}else
		return false;
}

std::vector<std::string> FileFunctions::Dir(const char *path, const char *right,
  bool sorted)
{
	DIR *dirp;
	struct dirent *entry;
	vector<string> ret;

	if((dirp = opendir(path)) != NULL){
		while((entry = readdir(dirp)) != NULL){
			string name(entry->d_name);
			string r(right);
			if((name.length() >= r.length()) && 
				(name.substr(name.length() - r.length()).compare(r) == 0))
			{
				ret.push_back(string(path) + "/" + entry->d_name);
			}
		}
		closedir(dirp);
	}
	
	if(sorted) sort(ret.begin(), ret.end());
	
    return ret;
}

bool FileFunctions::IsEmptyFolder(const char *path)
{
    vector<string> files = FileFunctions::Dir(path, "");
    if(files.empty()) {
        return true;
    }

    return false;
}

std::string FileFunctions::FileName(const std::string filepath)
{
	string::size_type p = filepath.find_last_of('/');
	string::size_type p2 = filepath.find_last_of('\\');
	if(p2 != string::npos && p2 > p) p = p2;
	return filepath.substr(p+1);
}

void FileFunctions::FileParts(const std::string filepath, std::string &path,
						   std::string &filename, std::string &ext)
{
	string::size_type p = filepath.find_last_of('/');
	string::size_type p2 = filepath.find_last_of('\\');
	if(p == string::npos || (p2 != string::npos && p2 > p)) p = p2;

	std::string filext;

	if(p == string::npos){
		path = "";
		filext = filepath;
	}else{
		path = filepath.substr(0, p);
		filext = filepath.substr(p+1);
	}

	p = filext.find_last_of('.');
	if(p == string::npos){
		filename = filext;
		ext = "";
	}else{
		filename = filext.substr(0, p);
		ext = filext.substr(p+1);
	}
}

