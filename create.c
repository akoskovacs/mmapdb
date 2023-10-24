#include <stdio.h>
#include "users.h"
#include <unistd.h>

int main(int argc, char *argv[])
{
  struct UsersDatabase *db = create_users_database(DB_NAME);
  if (db == NULL) {
    fprintf(stderr, "Cannot open database %s\n", DB_NAME);
    return 1;
  }

  create_user(db, "akoskovacs", "Akos Kovacs", "Valami0");
  create_user(db, "admin", "Admin Admin", "VERYSAFEPASSS120");
  create_user(db, "root", "Root", "verysafepasss120");
  create_user(db, "toor", "Toor", "kj2h323lckhj4");
  create_user(db, "jhonnie", "John A. Smith", "lovejagirl");

  close_users_database(db);

  return 0;
}


