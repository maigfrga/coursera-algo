#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include "ntutils.h"


void die(const char *message){
  if(errno){
    perror(message);
  }else{
    printf("ERROR %s : \n", message);
  }
  exit(1);
}


//Open a file and return a Connection pointer
struct Connection* File_open(char *filename, char *mode){
    struct Connection *conn = malloc(sizeof(struct Connection));
    if (!conn) die("Memory error!! \n");

    conn->file = fopen(filename, mode);

    if(! conn->file )die("Fail open the file!! \n");
    return conn;

}

void Connection_close(struct Connection *conn){
  if(conn){
    if(conn->file)fclose(conn->file);
    free(conn);
  }
}
