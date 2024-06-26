#ifndef DB_HANDLER_H
#define DB_HANDLER_H

#include "interfaces.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>

class SqLiteHandler : public DataBase
{
  public:
    explicit SqLiteHandler(const std::string& dbFilename);
    void storeMessage(const std::string& message) override;
    void updateConfig(const std::string& message) override;
    void storeMultipleMessages(const std::vector<std::string>& messages) override;

  private:
    SQLite::Database mDb;
    SQLite::Statement messageQuery;
    SQLite::Statement configurationQuery;

    void createTables();
};

#endif // DB_HANDLER_H