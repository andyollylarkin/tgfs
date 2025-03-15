#pragma once
#include <memory>
#include <string>
#include "fs_entry.hpp"

class IMetaStorage
{
public:
	virtual std::shared_ptr<FsEntry> find_entry(std::string path) = 0;
	virtual void update_entry(std::shared_ptr<FsEntry> entry) noexcept(false) = 0;
	virtual void delete_entry(std::string path) noexcept(false) = 0;
	virtual std::shared_ptr<FsEntry> get_parent(std::string path) = 0;
};