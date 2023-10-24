#include "users.h"

char *encrypt_password(char *digest, size_t size)
{
  for (int i = 0; i < size - 1; i++) {
    digest[i] ^= 0x4c;
  }
  digest[size - 1] = '\0';
  return digest;
}
