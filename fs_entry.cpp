#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <boost/filesystem.hpp>
#include "fs_entry.hpp"
#include <fstream>

using namespace boost::filesystem;

using path_ = path;

class TgFilesystem : Filesystem
{
public:
	TgFilesystem() {}

	FilesystemEntry *lookup(std::string path) override
	{
		path_ p(path);
		if (path.empty())
		{
			return nullptr;
		}

		path_ cannonical_path = resolve_to_absolute(p);

		FilesystemEntry *current = root;
		if (!current)
		{
			return nullptr;
		}

		for (auto &part : p)
		{
			for (auto &entry : current->children())
			{
				// if (current->name())
				// 	if (entry->name() == part)
				// 	{
				// 		current = entry;
				// 		break;
				// 	}
			}
		}
		return current;
	};
	FilesystemEntry *create(std::string path_, mode_t mode) override
	{
		return nullptr;
	}
};

path resolve_to_absolute(path p) noexcept(false)
{
	if (p.is_absolute())
	{
		return p;
	}
	return current_path() / p;
}