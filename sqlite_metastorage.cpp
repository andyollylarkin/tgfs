#include "sqlite_metastorage.hpp"
#include <string>
#include <sqlite3.h>
#include <stdexcept>
#include <iostream>
#include <memory>

SqliteMetaStorage::SqliteMetaStorage(const std::string db_path)
{
	sqlite3 *db;

	if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK)
	{
		throw std::runtime_error("invalid sqlite db initialization: " + std::string(sqlite3_errmsg(db_)));
	};
}

void SqliteMetaStorage::delete_entry(std::string path)
{
	char *err_msg = nullptr;
	std::string sql = "DELETE FROM filesystem WHERE path = ?";

	sqlite3_stmt *stmt;
	if (sqlite3_prepare_v2(this->db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(this->db_)));
	}

	if (sqlite3_bind_text(stmt, 1, path.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw std::runtime_error("Failed to bind value: " + std::string(sqlite3_errmsg(this->db_)));
	}

	if (sqlite3_step(stmt) != SQLITE_DONE)
	{
		sqlite3_finalize(stmt);
		throw std::runtime_error("Failed to execute statement: " + std::string(sqlite3_errmsg(this->db_)));
	}

	sqlite3_finalize(stmt);
}

std::shared_ptr<FsEntry> SqliteMetaStorage::find_entry(std::string path)
{
	std::string sql = "SELECT path,size,mode,parts,atime,ctime FROM filesystem WHERE path = ?";
	sqlite3_stmt *stmt;

	if (sqlite3_prepare_v2(this->db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
	{
		throw std::runtime_error("Failed to prepare statement: " + std::string(sqlite3_errmsg(this->db_)));
	}

	if (sqlite3_bind_text(stmt, 1, path.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		throw std::runtime_error("Failed to bind value: " + std::string(sqlite3_errmsg(this->db_)));
	}

	std::shared_ptr<FsEntry> entry = nullptr;
	if (sqlite3_step(stmt) == SQLITE_ROW)
	{
		entry = std::make_shared<FsEntry>();
		entry->path = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
		entry->stat.st_size = sqlite3_column_int(stmt, 1);
		entry->t = EntryType(sqlite3_column_int(stmt, 2));
		entry->stat.st_mode = sqlite3_column_int(stmt, 3);
		entry->parts = std::vector<std::string>{split_by_delim(std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4))), PART_DELIMITER)};
		entry->atime = sqlite3_column_int64(stmt, 5);
		entry->ctime = sqlite3_column_int64(stmt, 6);
	}

	sqlite3_finalize(stmt);
	return entry;
}

void SqliteMetaStorage::update_entry(std::shared_ptr<FsEntry> entry) {}

std::shared_ptr<FsEntry> get_parent(std::string path) {}