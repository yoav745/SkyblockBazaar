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
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS bazaar_history (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            product_id TEXT NOT NULL,
            timestamp INTEGER NOT NULL,

            avg_buy_price REAL NOT NULL,
            high_buy REAL NOT NULL,
            low_buy REAL NOT NULL,
            buy_volume INTEGER NOT NULL,

            avg_sell_price REAL NOT NULL,
            high_sell REAL NOT NULL,
            low_sell REAL NOT NULL,
            sell_volume INTEGER NOT NULL
        );
    )";

    char* err_msg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &err_msg) != SQLITE_OK) {
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
        SELECT timestamp, avg_buy_price, high_buy, low_buy, buy_volume ,avg_sell_price, high_sell, low_sell
         , sell_volume
        FROM (SELECT * FROM bazaar_history WHERE product_id = ? ORDER BY timestamp DESC LIMIT 168)
        ORDER BY timestamp ASC;
    )";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, product_id.c_str(), -1, SQLITE_STATIC);

        // Loop through the results and build itemHour objects
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            long long ts = sqlite3_column_int64(stmt, 0);
            double avg_buy = sqlite3_column_double(stmt, 1);
            double high_buy = sqlite3_column_double(stmt, 2);
            double low_buy = sqlite3_column_double(stmt, 3);
            long long buy_vol = sqlite3_column_int64(stmt, 4);
            double avg_sell = sqlite3_column_double(stmt, 5);
            double high_sell = sqlite3_column_double(stmt, 6);
            double low_sell = sqlite3_column_double(stmt, 7);
            long long sell_vol = sqlite3_column_int64(stmt, 8);
;
            Price::PriceObject sell(avg_sell , high_sell, low_sell, sell_vol);
            Price::PriceObject buy(avg_buy , high_buy, low_buy, buy_vol);

            loaded_history.emplace_back(buy,sell,ts);
        }
    }
    sqlite3_finalize(stmt);
    return loaded_history;
}

// Insert the completed hour into the database
void DatabaseManager::insertHourData(const std::string& product_id, const hourNamespace::itemHour& hour_data) {
    const char* insert_sql = R"(
        INSERT INTO bazaar_history (product_id, timestamp, avg_buy_price, high_buy, low_buy,buy_volume , avg_sell_price, high_sell, low_sell, sell_volume)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt;
    
    // Prepare the statement
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        
        // Bind our C++ variables to the ? placeholders in the SQL string
        sqlite3_bind_text(stmt, 1, product_id.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int64(stmt, 2, hour_data.getTimestamp());

        sqlite3_bind_double(stmt, 3, hour_data.getBuyAveragePrice());
        sqlite3_bind_double(stmt, 4, hour_data.getBuyHighPrice());
        sqlite3_bind_double(stmt, 5, hour_data.getBuyLowPrice());
        sqlite3_bind_int64(stmt, 6, hour_data.getBuyVolume());

        sqlite3_bind_double(stmt, 7, hour_data.getSellAveragePrice());
        sqlite3_bind_double(stmt, 8, hour_data.getSellHighPrice());
        sqlite3_bind_double(stmt, 9, hour_data.getSellLowPrice());
        sqlite3_bind_int64(stmt, 10, hour_data.getSellVolume());


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