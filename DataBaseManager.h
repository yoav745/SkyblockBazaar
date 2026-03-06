#ifndef PROJECTSKYBLOCK_DATABASEMANAGER_H
#define PROJECTSKYBLOCK_DATABASEMANAGER_H

#include <string>
#include "lib/sqlite/sqlite3.h"
#include "itemHour.h"

class DatabaseManager {
public:
    // Constructor opens the connection and ensures the table exists
    explicit DatabaseManager(const std::string& db_name);

    // Destructor cleanly closes the connection
    ~DatabaseManager();

    // The core function to log a completed hour
    void insertHourData(const std::string& product_id, const hourNamespace::itemHour& hour_data);
    std::vector<hourNamespace::itemHour> loadHistory(const std::string& product_id);
    void deleteHour(const std::string& product_id, long long timestamp);
private:
    sqlite3* db;

    // Helper function to set up the SQL table
    void initializeTable();
};

#endif //PROJECTSKYBLOCK_DATABASEMANAGER_H