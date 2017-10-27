#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <hashtable.h>

int main(int argc, char ** argv) {

  char buffer[100];
  hash_table table;

  initTable(&table, 2);
  while (strcmp (buffer,"quit") != 0) {
    printf("Enter command\n");
    scanf("%s", buffer);

    if (strcmp (buffer, "retrieve") == 0) {
      printf("Retrieving: ");

      scanf("%s", buffer);
      hash_node * node = retrieve(&table, buffer);
      printNode(node);
    }

    if (strcmp (buffer, "insert") == 0) {
      char buffer2[100];
      printf("Inserting: ");
      scanf("%s %s", buffer, buffer2);
      insert(&table, buffer, atoi(buffer2));
      printTable(&table);
    }

    if (strcmp (buffer, "print") == 0) {
      printf("Printing\n");
      printTable(&table);
    }

    if (strcmp (buffer, "remove") == 0) {
      printf("Removing: ");
      scanf("%s", buffer);
      removeNode(&table, buffer);
    }

  } //end while
  destroyTable(&table);

  return 0;
}
