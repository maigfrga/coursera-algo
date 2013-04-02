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

struct IntArraySplitted* split_array(struct IntArray *input){
    struct IntArraySplitted *split_output = malloc(sizeof(struct IntArraySplitted));
    struct IntArray *output1 = malloc(sizeof(struct IntArray));
    struct IntArray *output2 = malloc(sizeof(struct IntArray));

    //check if the length of the array is pair or not
    if(input->length % 2 == 0){
        output1->length = (input->length/2);
        output2->length = output1->length;
    }else{
        output1->length = (input->length)/2 + 1;
        output2->length = output1->length -1 ;
    }

    int i=0,j=0;

    output1->array = malloc(sizeof(int)*output1->length);
    output1->array = input->array;
    output2->array = malloc(sizeof(int)*output2->length);

    for(i=output1->length; i< input->length; i++){
        output2->array[j] = input->array[i];
        j++;
    }

    split_output->intArray1 = output1;
    split_output->intArray2 = output2;

    return split_output;
}
