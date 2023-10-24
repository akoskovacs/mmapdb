#ifndef USERS_H

#include <sys/types.h>
#include <stdbool.h>

#define DB_NAME "users.db"

struct User {
  unsigned long int id;
  char name[40];
  char full_name[120];
  char password_digest[256];
};

struct UsersHeader {
  char magic[4];
  unsigned long int user_count;
  unsigned long int mapsize;
  char reserved[32];
};

struct UsersDatabase {
  struct UsersHeader *header;
  struct User *users;
  const char *db_name;
  int fd;
};

typedef void (user_lambda_t)(const struct User *, unsigned long int i);

struct UsersDatabase *create_users_database(const char *db_name);
struct UsersDatabase *open_users_database(const char *db_name);
bool create_user(struct UsersDatabase *db, const char *name, const char *full_name, const char *password);
struct User *find_user_by_id(struct UsersDatabase *db, unsigned long int id);
void select_all_users(struct UsersDatabase *db, user_lambda_t on_select);
void close_users_database(struct UsersDatabase *db);

char *encrypt_password(char *digest, size_t size);

#endif // !USERS_H
