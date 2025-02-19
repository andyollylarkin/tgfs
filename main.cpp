#include "Fuse-impl.h"
#include "Fuse.h"
#include <errno.h>
#include <fuse.h>
#include <iostream>
#include <string>
#include "fs_entry.hpp"
#include "boost/filesystem.hpp"
#include <fstream>

using namespace std;
using namespace boost::filesystem;

const string example_file = "example.txt";

static const string root_path = "/";
static const string hello_str = "Hello World!\n";
static const string hello_path = "/hello";

class FuseFs : public Fusepp::Fuse<FuseFs>
{
public:
	FuseFs() {
	};

	~FuseFs() {}

	static int getattr(const char *path, struct stat *stbuf, struct fuse_file_info *)
	{
		int res = 0;

		memset(stbuf, 0, sizeof(struct stat));
		if (path == root_path)
		{
			stbuf->st_mode = S_IFDIR | 0755;
			stbuf->st_nlink = 2;
		}
		else if (path == hello_path)
		{
			stbuf->st_mode = S_IFREG | 0444;
			stbuf->st_nlink = 1;
			stbuf->st_size = hello_str.length();
		}
		else
			res = -ENOENT;

		return res;
	}

	static int readdir(const char *path, void *buf, fuse_fill_dir_t filler,
					   off_t, struct fuse_file_info *, enum fuse_readdir_flags)
	{
		if (path != root_path)
			return -ENOENT;

		filler(buf, ".", NULL, 0, (fuse_fill_dir_flags)2);
		filler(buf, "..", NULL, 0, FUSE_FILL_DIR_PLUS);
		filler(buf, hello_path.c_str() + 1, NULL, 0, FUSE_FILL_DIR_PLUS);

		return 0;
	}

	static int open(const char *path, struct fuse_file_info *fi)
	{
		if (path != hello_path)
			return -ENOENT;

		if ((fi->flags & 3) != O_RDONLY)
			return -EACCES;

		return 0;
	}

	static int read(const char *path, char *buf, size_t size, off_t offset,
					struct fuse_file_info *)
	{
		if (path != hello_path)
			return -ENOENT;

		size_t len;
		len = hello_str.length();
		if ((size_t)offset < len)
		{
			if (offset + size > len)
				size = len - offset;
			memcpy(buf, hello_str.c_str() + offset, size);
		}
		else
			size = 0;

		return size;
	}

	static int write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *)
	{
		return 0;
	}

	static int create(const char *path, mode_t mode, struct fuse_file_info *)
	{
		return 0;
	}

	static int unlink(const char *path)
	{
		return 0;
	}

	static int truncate(const char *path, off_t size, struct fuse_file_info *)
	{
		return 0;
	}

	static int mkdir(const char *path, mode_t mode)
	{
		return 0;
	}

	static int rmdir(const char *path)
	{
		return 0;
	}
};

int main(int argc, char *argv[])
{
	MyFs fs;
	return fs.run(argc, argv);
}