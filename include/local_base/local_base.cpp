#include "local_base.h"

int local_base::callback(void *p_data, int num_fields, char **p_fields,
                         char **p_col_names) {
  std::vector<std::vector<std::string>> *records =
      static_cast<std::vector<std::vector<std::string>> *>(p_data);
  records->emplace_back(p_fields, p_fields + num_fields);
  return 0;
}

local_base::local_base() {
  this->status = sqlite3_open("temp.db", &this->localBase);
  if (this->status) {
    fprintf(stderr, "Can't open database: %s\n",
            sqlite3_errmsg(this->localBase));
  }
  this->createTable();
}

local_base::~local_base() {
  this->status = sqlite3_exec(this->localBase,
                              "DELETE from user_info where ID=1;"
                              "DROP table user_info;",
                              NULL, NULL, &this->errorMessage);
  if (this->status != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", this->errorMessage);
    sqlite3_free(this->errorMessage);
  }
  sqlite3_close(this->localBase);
}

void local_base::createTable() {
  this->status = sqlite3_exec(this->localBase,
                              "CREATE TABLE user_info("
                              "ID INT PRIMARY KEY NOT NULL,"
                              "LOGIN CHAR(20) NOT NULL,"
                              "PASSWORD CHAR(20) NOT NULL,"
                              "SESSION INT);",
                              0, 0, &this->errorMessage);
  if (this->status != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", this->errorMessage);
    sqlite3_free(this->errorMessage);
  }
}

void local_base::insertInTable(sf::String login, sf::String password) {
  std::string statement =
      "INSERT INTO user_info (ID,LOGIN,PASSWORD,SESSION) VALUES (1,'" +
      login.toAnsiString() + "','" + password.toAnsiString() + "', 0);";
  this->status = sqlite3_exec(this->localBase, statement.c_str(), 0, 0,
                              &this->errorMessage);
  if (this->status != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", this->errorMessage);
    sqlite3_free(this->errorMessage);
  }
}

void local_base::setSession(std::string key) {
  this->status = sqlite3_exec(
      this->localBase,
      (std::string("UPDATE user_info set SESSION = " + key + ";")).c_str(), 0,
      0, &this->errorMessage);
  if (this->status != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", this->errorMessage);
    sqlite3_free(this->errorMessage);
  }
}

std::vector<std::vector<std::string>> local_base::getTable() {
  std::vector<std::vector<std::string>> records;
  this->status = sqlite3_exec(this->localBase, "SELECT * FROM user_info;",
                              callback, &records, &this->errorMessage);
  if (this->status != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", this->errorMessage);
    sqlite3_free(this->errorMessage);
  }
  return records;
}

std::string local_base::getLogin() { return this->getTable()[0][1]; }

std::string local_base::getPassword() { return this->getTable()[0][2]; }

std::string local_base::getSession() { return this->getTable()[0][3]; }
