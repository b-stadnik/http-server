#include "db_handler.h"
#include "msg_parser.h"

#include <iostream>

SqLiteHandler::SqLiteHandler(const std::string& dbFilename)
    : mDb(dbFilename, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE), messageQuery(mDb, ""), configurationQuery(mDb, "")
{
    createTables();
    messageQuery = SQLite::Statement(mDb, "INSERT INTO messages (pressure, temperature, velocity) VALUES (?, ?, ?);");
    configurationQuery = SQLite::Statement(mDb, "INSERT INTO configurations (frequency, debug) VALUES (?, ?);");

    std::cout << "Connected to database" << std::endl;
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
    constexpr size_t msg_size(3);
    std::vector<std::string> tokens = message_parser::splitString(message, msg_size);

    if(tokens.size() != msg_size)
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

void SqLiteHandler::updateConfig(const std::string& message)
{
    constexpr size_t msg_size(4);
    std::vector<std::string> tokens = message_parser::splitString(message, msg_size);

    if(tokens.size() != msg_size || tokens.back().find("ok") == std::string::npos)
    {
        std::cerr << "Invalid message format: " << message << std::endl;
        return;
    }

    configurationQuery.bind(1, tokens[1]);
    configurationQuery.bind(2, tokens[2]);
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
