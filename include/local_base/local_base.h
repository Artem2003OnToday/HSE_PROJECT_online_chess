#pragma once
#include "sqlite3.h"
#include <String.hpp>
#include <cstdio>
#include <string>
#include <vector>

class local_base {
private:
  sqlite3 *localBase = nullptr;
  char *errorMessage = 0;
  int status = 0;
  const char *data = 0;
  static int callback(void *p_data, int num_fields, char **p_fields,
                      char **p_col_names);

public:
  local_base();
  ~local_base();
  void createTable();
  void insertInTable(sf::String login, sf::String password);
  void setSession(std::string key);
  std::vector<std::vector<std::string>> getTable();
  std::string getLogin();
  std::string getPassword();
  std::string getSession();
};