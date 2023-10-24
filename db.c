#include "users.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

struct UsersDatabase *create_users_database(const char *dbname)
{
  struct UsersDatabase *db = malloc(sizeof(struct UsersDatabase));
  if (db == NULL) {
    return NULL;
  }

  int fd = open(dbname, O_CREAT|O_RDWR, 0644);
  if (fd < 0) {
    fprintf(stderr, "Cannot open users database!\n");
    return NULL;
  }

  unsigned long int mapsize = 4096 * 1;
  ftruncate(fd, mapsize);
  sync();
  
  void *mapping = mmap(NULL, mapsize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (mapping == NULL) {
    fprintf(stderr, "Cannot map database!\n");
    return NULL;
  }

  struct UsersHeader *header = (struct UsersHeader *)mapping;

  strncpy(header->magic, "_UDB", 4);
  header->mapsize = mapsize;
  header->user_count = 0;

  struct User *users = (struct User *)(mapping + sizeof(struct UsersHeader));

  db->fd = fd;
  db->db_name = dbname;
  db->header = header;
  db->users = users;
  return db;
}

struct UsersDatabase *open_users_database(const char *dbname)
{
  struct UsersDatabase *db = malloc(sizeof(struct UsersDatabase));
  if (db == NULL) {
    return NULL;
  }

  int fd = open(dbname, O_RDWR, 0644);
  if (fd < 0) {
    fprintf(stderr, "Cannot open users database!\n");
    return NULL;
  }

  // TODO: Fix this
  unsigned long int mapsize = 4096 * 1;
  void *mapping = mmap(NULL, mapsize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if (mapping == NULL) {
    fprintf(stderr, "Cannot map database!\n");
    return NULL;
  }

  struct UsersHeader *header = (struct UsersHeader *)mapping;

  if (strncmp("_UDB", header->magic, 4) != 0) {
    fprintf(stderr, "Database file is corrupt!\n");
    return NULL;
  }
  struct User *users = (struct User *)(mapping + sizeof(struct UsersHeader));

  db->fd = fd;
  db->db_name = dbname;
  db->header = header;
  db->users = users;
  return db;
}

bool create_user(struct UsersDatabase *db, const char *name, const char *full_name, const char *password)
{
  if (db == NULL) {
    return false;
  }

  struct User *user = (struct User *)((void *)db->users + (sizeof(struct User) * db->header->user_count));
  (db->header->user_count)++;
  //db->header->user_count = db->header->user_count + 1;
  user->id = db->header->user_count;
  strncpy(user->name, name, 40 - 1);
  strncpy(user->full_name, full_name, 120 - 1);
  strncpy(user->password_digest, password, 256 - 1);
  encrypt_password(user->password_digest, 256);
  return true;
}

struct User *find_user_by_id(struct UsersDatabase *db, unsigned long id)
{
  for (unsigned long int i = 0; i < db->header->user_count; i++) {
    struct User *user = (struct User *)((void *)db->users + (sizeof(struct User) * i));
    if (user->id == id) {
      return user;
    }
  }
  return NULL;
}

void select_all_users(struct UsersDatabase *db, user_lambda_t on_select)
{
  for (unsigned long int i = 0; i < db->header->user_count; i++) {
    struct User *user = (struct User *)((void *)db->users + (sizeof(struct User) * i));
    on_select(user, i);
  }
}

void close_users_database(struct UsersDatabase *db)
{
  sync();
  munmap(db, db->header->mapsize);
  close(db->fd);
  free(db);
}


