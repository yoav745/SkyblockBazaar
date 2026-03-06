#include "DatabaseManager.h"
#include <iostream>

// Constructor
DatabaseManager::DatabaseManager(const std::string& db_name) {
    // Open the connection. If the file doesn't exist, SQLite creates it instantly.
    if (sqlite3_open(db_name.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Fatal Error: Cannot open database: " << sqlite3_errmsg(db) << std::endl;
    } else {
        initializeTable();
    }
}

// Destructor
DatabaseManager::~DatabaseManager() {
    sqlite3_close(db);
}

// Create the Time-Series Table
void DatabaseManager::initializeTable() {
    const char* create_table_sql = R"(
        CREATE TABLE IF NOT EXISTS bazaar_history (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            product_id TEXT NOT NULL,
            timestamp INTEGER NOT NULL,
            average_price REAL NOT NULL,
            high_price REAL NOT NULL,
            low_price REAL NOT NULL,
            volume INTEGER NOT NULL
        );
    )";

    char* err_msg = nullptr;
    if (sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg) != SQLITE_OK) {
        std::cerr << "SQL Error creating table: " << err_msg << std::endl;
        sqlite3_free(err_msg);
    }
}

void DatabaseManager::deleteHour(const std::string& product_id, long long timestamp) {
    const char* sql = "DELETE FROM bazaar_history WHERE product_id = ? AND timestamp = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, product_id.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int64(stmt, 2, timestamp);
        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
}


std::vector<hourNamespace::itemHour> DatabaseManager::loadHistory(const std::string& product_id) {
    std::vector<hourNamespace::itemHour> loaded_history;

    // This clever SQL trick gets the NEWEST 168 rows, but returns them in ASCENDING order
    // so we can push them into your deque in correct chronological sequence.
    const char* sql = R"(
        SELECT timestamp, average_price, high_price, low_price, volume
        FROM (SELECT * FROM bazaar_history WHERE product_id = ? ORDER BY timestamp DESC LIMIT 168)
        ORDER BY timestamp ASC;
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, product_id.c_str(), -1, SQLITE_STATIC);

        // Loop through the results and build itemHour objects
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            long long ts = sqlite3_column_int64(stmt, 0);
            double avg = sqlite3_column_double(stmt, 1);
            double high = sqlite3_column_double(stmt, 2);
            double low = sqlite3_column_double(stmt, 3);
            long long vol = sqlite3_column_int64(stmt, 4);

            loaded_history.emplace_back(ts, avg, high, low, vol);
        }
    }
    sqlite3_finalize(stmt);
    return loaded_history;
}

// Insert the completed hour into the database
void DatabaseManager::insertHourData(const std::string& product_id, const hourNamespace::itemHour& hour_data) {
    const char* insert_sql = R"(
        INSERT INTO bazaar_history (product_id, timestamp, average_price, high_price, low_price, volume)
        VALUES (?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt;
    
    // Prepare the statement
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        
        // Bind our C++ variables to the ? placeholders in the SQL string
        sqlite3_bind_text(stmt, 1, product_id.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int64(stmt, 2, hour_data.getTimestamp());
        sqlite3_bind_double(stmt, 3, hour_data.getAveragePrice());
        sqlite3_bind_double(stmt, 4, hour_data.getHighPrice());
        sqlite3_bind_double(stmt, 5, hour_data.getLowPrice());
        sqlite3_bind_int64(stmt, 6, hour_data.getVolume());

        // Execute the insertion
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Failed to insert data: " << sqlite3_errmsg(db) << std::endl;
        }
    } else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }

    // Clean up the statement memory
    sqlite3_finalize(stmt);
}