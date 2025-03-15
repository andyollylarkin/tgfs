#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <boost/filesystem.hpp>
#include <filesystem>
#include <sstream>
#include "fs_entry.hpp"
#include <fstream>

using namespace boost::filesystem;

std::string resolve_path_to_absolute(const std::string &path)
{
	if (path.empty())
	{
		return std::filesystem::current_path().string();
	}

	std::filesystem::path fs_path;

	// Check if path is relative
	if (path[0] == '.' || (path[0] != '/' && path.length() > 1 && path[1] != ':'))
	{
		fs_path = std::filesystem::current_path() / path;
	}
	else
	{
		fs_path = path;
	}

	// First make the path absolute
	fs_path = std::filesystem::absolute(fs_path);

	// Then normalize it to remove .. and . components
	fs_path = fs_path.lexically_normal();

	// If the path exists, use canonical to resolve symlinks
	try
	{
		if (std::filesystem::exists(fs_path))
		{
			fs_path = std::filesystem::canonical(fs_path);
		}
	}
	catch (const std::filesystem::filesystem_error &e)
	{
		// Ignore errors, just use the normalized path
	}

	return fs_path.string();
}

std::vector<std::string> split_by_delim(std::string path, char delim)
{
	std::string current = "";

	std::vector<std::string> result = {};

	for (auto p : path)
	{
		if (p == delim)
		{
			result.push_back(current);
			current = "";
			continue;
		}

		current.push_back(p);
	}

	return result;
}