#include "db_handler.h"
#include "msg_parser.h"

#include <iostream>

SqLiteHandler::SqLiteHandler(const std::string& dbFilename)
    : mDb(dbFilename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE), messageQuery(mDb, ""), configurationQuery(mDb, "")
{
    createTables();
    messageQuery =
        std::move(SQLite::Statement(mDb, "INSERT INTO messages (pressure, temperature, velocity) VALUES (?, ?, ?);"));
    configurationQuery =
        std::move(SQLite::Statement(mDb, "INSERT INTO configurations (frequency, debug) VALUES (?, ?);"));
}

void SqLiteHandler::storeMultipleMessages(const std::vector<std::string>& messages)
{
    SQLite::Transaction transaction(mDb);

    for(const auto& message : messages)
    {
        storeMessage(message);
    }

    transaction.commit();
}

void SqLiteHandler::storeMessage(const std::string& message)
{
    std::vector<std::string> tokens = message_parser::splitString(message);

    if(tokens.size() != 3)
    {
        std::cerr << "Invalid message format: " << message << std::endl;
        return;
    }

    messageQuery.bind(1, std::stod(tokens[0]));
    messageQuery.bind(2, std::stod(tokens[1]));
    messageQuery.bind(3, std::stod(tokens[2]));
    messageQuery.exec();
    messageQuery.reset();
}

void SqLiteHandler::updateConfig(int frequency, bool debug)
{
    configurationQuery.bind(1, frequency);
    configurationQuery.bind(2, debug);
    configurationQuery.exec();
    configurationQuery.reset();
}

void SqLiteHandler::createTables()
{
    mDb.exec("CREATE TABLE IF NOT EXISTS messages (id INTEGER PRIMARY KEY, pressure REAL, temperature REAL, velocity "
             "REAL, timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);");
    mDb.exec("CREATE TABLE IF NOT EXISTS configurations (id INTEGER PRIMARY KEY, frequency INTEGER, debug BIT, "
             "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);");
}
