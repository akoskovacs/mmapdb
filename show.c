#include <stdio.h>
#include "users.h"
#include <unistd.h>
#include <stdlib.h>

void print_user_details(struct User *user)
{
  if (user == NULL)
    return;
  
  printf("User id: %ld, name: @%s, full name: '%s'\n", user->id, user->name, user->full_name);
}

void show_on_select(struct User *user, unsigned long int index)
{
  print_user_details(user);
}

int main(int argc, char *argv[])
{
  struct UsersDatabase *db = open_users_database(DB_NAME);
  if (db == NULL) {
    fprintf(stderr, "Cannot open database %s\n", DB_NAME);
    return 1;
  }

  int uid = -1;
  struct User *user = NULL;
  if (argc > 1) {
    uid = atoi(argv[1]);
    user = find_user_by_id(db, uid);
    if (user == NULL) {
      fprintf(stderr, "User of id = %d is not found!\n", uid);
    } else {
      print_user_details(user);
    }
  } else {
    select_all_users(db, show_on_select);
  }

  close_users_database(db);

  return 0;
}


