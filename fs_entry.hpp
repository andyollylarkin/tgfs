#pragma once
// #include "boost/filesystem.hpp"
#include <vector>
#include <string>
#include <sys/stat.h>

#define PART_DELIMITER ','

// using namespace boost::filesystem;

enum EntryType
{
	RFILE,
	DIRECTORY
};

std::string resolve_path_to_absolute(const std::string &path);

std::vector<std::string> split_by_delim(std::string path, char delim);

class FsEntry
{
public:
	std::string path;
	EntryType t;
	struct stat stat;
	std::vector<std::string> parts;
	int ctime;
	int atime;
};
