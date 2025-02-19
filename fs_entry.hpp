#include "boost/filesystem.hpp"
#include <sys/stat.h>
#include <vector>

using namespace boost::filesystem;

using stat_fs = struct stat;

class not_a_file : public std::exception
{
public:
	const char *what() const noexcept override
	{
		return "Path is not a file";
	}
};

enum Kind
{
	Regular,
	Directory
};

class FilesystemEntry
{
public:
	FilesystemEntry(Kind kind, std::string name, FilesystemEntry *parent) : kind_(kind), name_(name), parent_(parent) {};

	virtual int write(const char *buf, size_t size, off_t offset) = 0;
	virtual int read(char *buf, size_t size, off_t offset) = 0;

	stat_fs stat()
	{
		return stat_e;
	}

	std::string name()
	{
		return name_;
	}

	FilesystemEntry *parent()
	{
		return parent_;
	}

	std::vector<FilesystemEntry *> children()
	{
		return entries;
	}

	Kind kind()
	{
		return kind_;
	}

protected:
	std::vector<FilesystemEntry *> entries;
	FilesystemEntry *parent_;
	stat_fs stat_e;
	std::string name_;
	Kind kind_;

	bool is_root()
	{
		return this->parent_ == nullptr;
	}

	friend class Filesystem;
};

class Filesystem
{
public:
	virtual FilesystemEntry *create(std::string path, mode_t mode) = 0;
	virtual FilesystemEntry *open(std::string path) = 0;
	virtual FilesystemEntry *readdir(std::string path) = 0;
	virtual void mkdir(std::string path, mode_t mode) = 0;
	virtual void rmdir(std::string path) = 0;
	virtual void operator<<(std::ostream &os) const = 0;
	virtual void operator>>(std::ostream &os) const = 0;
	

protected:
	FilesystemEntry *root;
	virtual FilesystemEntry *lookup(std::string path) = 0;
};
