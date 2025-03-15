#pragma once

#include "metastore.hpp" // Подключаем абстрактный класс
#include <sqlite3.h>
#include <memory>
#include <string>
#include <stdexcept>

class SqliteMetaStorage : public IMetaStorage
{
public:
	explicit SqliteMetaStorage(const std::string db_path);
	std::shared_ptr<FsEntry> find_entry(std::string path) override;
	void update_entry(std::shared_ptr<FsEntry> entry) override;
	void delete_entry(std::string path) override;
	std::shared_ptr<FsEntry> get_parent(std::string path) override;

private:
	sqlite3 *db_;

	std::string extract_parent_path(const std::string &path) const;
};

class NotFoundException : public std::runtime_error
{
public:
	NotFoundException(const std::string &message) : std::runtime_error(message) {}
};