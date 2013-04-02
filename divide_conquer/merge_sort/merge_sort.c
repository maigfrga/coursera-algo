#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "ntutils.h"
#define CHUNK_SIZE 256 /* read 256 bytes at a time */

struct Merge{
    struct IntArray *input;
    struct IntArray *output;
};


/*process the text file and returns a Merge strcut with an input int array
 * ready to apply merge sort
 * Split string by tokens: http://stackoverflow.com/questions/2523467/how-to-split-a-string-to-2-strings-in-c
 */
struct Merge *Connection_process(struct Connection *conn){
    char *buffer = malloc(CHUNK_SIZE);
    char *file_line = NULL;
    char *file_line_token = NULL;
    char *file_line_space = NULL;
    char *file_line_space_token = NULL;
    int nread = 0;
    int *array_numbers;
    int array_index=0;
    struct Merge *m_struct = malloc(sizeof(struct Merge));
    struct IntArray *input = malloc(sizeof(struct IntArray));


    if(!buffer) die("Memory error !! \n");
    //read every line of the file
    array_numbers = (int*) malloc(sizeof(int) * CHUNK_SIZE);
    nread = fread(buffer, 1, CHUNK_SIZE, conn->file);

    while (nread > 0) {
        //copy the buffer to a string
        file_line = strdup(buffer);
        if(file_line != NULL){
            //split the buffer line by line
            while ((file_line_token = strsep(&file_line, "\n")) != NULL)
            {
                file_line_space = strdup(file_line_token);
                //split by space and store elements on array
                if(file_line_space != NULL){
                    while ((file_line_space_token = strsep(&file_line_space, " ")) != NULL){
                        if (!strcmp(file_line_space_token, "") == 0){
                            array_numbers[array_index] = atoi(file_line_space_token);
                            array_index++;
                        }
                    }
                    free(file_line_space_token);

                }
                free(file_line_space);
            }
            free(file_line_token);
        }else{
            break;
        }
        nread = fread(buffer, 1, CHUNK_SIZE, conn->file);

        if(nread > 0){
           array_numbers = (int*) realloc(array_numbers, CHUNK_SIZE);
        }
        free(file_line);
    }
    input->array = (int*)malloc(sizeof(int)*array_index);
    input->array = array_numbers;
    input->length = array_index;
    m_struct->input = input;
    return m_struct;
}


struct IntArray* merge(struct IntArray *input1, struct IntArray *input2){
    int length = input1->length + input2->length;
    int i=0,j=0,k=0;
    struct IntArray *output_return = malloc(sizeof(struct IntArray));
    output_return->array = malloc(sizeof(int)*length);
    output_return->length = length;
    for(k=0; k<length; k++){
        if (input1->length > i && input2->length > j){
            if(input1->array[i] < input2->array[j]){
                output_return->array[k] = input1->array[i];
                i++;
            }else{
                output_return->array[k] = input2->array[j];
                j++;
            }
        }
        else if(input1->length > i){
            output_return->array[k] = input1->array[i];
            i++;
        }

        else if(input2->length > j){
            output_return->array[k] = input2->array[j];
            j++;
        }
        else{printf("no deberia i: %d, j: %d , k: %d,  \n", i, j, k);}
    }
    return output_return;
}



struct IntArray* sort(struct IntArray *input){
    struct IntArray *output1 = malloc(sizeof(struct IntArray));
    struct IntArray *output2 = malloc(sizeof(struct IntArray));
    struct IntArray *output_return = malloc(sizeof(struct IntArray));
    struct IntArray *output = malloc(sizeof(struct IntArray));
    output_return->array = malloc(sizeof(int)*input->length);
    output->array = malloc(sizeof(int)*input->length);

    if(input->length==1){
        return input;
    }
    else{
        if(input->length % 2 == 0){
            output1->length = (input->length/2);
            output2->length = output1->length;

        }else{
            output1->length = (input->length-1)/2 + 1;
            output2->length = output1->length-1;
        }
        output1->array = malloc(sizeof(int)*output1->length);
        output1->array = input->array;
        output2->array = malloc(sizeof(int)*output2->length);
        int i=0, j=0;
        for(i=output1->length; i < input->length; i++){
            output2->array[j] = input->array[i];
            j++;
        }

        if(output1->length > 1){
            output1 = sort(output1);
        }
        if(output2->length > 1){
            output2 = sort(output2);
        }

        return merge(output1, output2);
    }

}


struct IntArray* merge_sort(char *filename){
    struct Connection *conn = File_open(filename, "rt");
    struct Merge *m_struct = Connection_process(conn);
    Connection_close(conn);

    int h=0;
    struct IntArray *input1 = malloc(sizeof(struct IntArray));
    struct IntArray *input2 = malloc(sizeof(struct IntArray));
    struct IntArray *output1 = malloc(sizeof(struct IntArray));
    struct IntArray *output2 = malloc(sizeof(struct IntArray));

    int i=0,j=0;

    //check if the length of the array is pair or not
    if(m_struct->input->length % 2 == 0){
        input1->length = (m_struct->input->length/2);
        input2->length = input1->length;
    }else{
        input1->length = (m_struct->input->length)/2;
        input2->length = input1->length -1 ;
    }

    input1->array = malloc(sizeof(int)*input1->length);
    input1->array = m_struct->input->array;
    input2->array = malloc(sizeof(int)*input2->length);

    for(i=input1->length; i< m_struct->input->length; i++){
        if(m_struct != NULL){
            input2->array[j] = m_struct->input->array[i];
            j++;
        }
    }


    output1 = sort(input1);
    output2 = sort(input2);
    m_struct->output = merge(output1, output2);

    return m_struct->output;
}


/* merge_sort.c : Merge sort implementation as personal practice for Standford online course
 * Algorithms: Design and Analysis, Part I https://www.coursera.org/course/algo
 * Usage: merge_sort <file>
 * file must be a text file with the numbers to be ordered separated by blank spaces, the output will
 * be ordered array printed at console
 */
int main(int argc, char *argv[])
{

    if (argc != 2) die("USAGE: count_inversion <file> ");
    char *filename = argv[1];
    int n_inversions = 0;
    n_inversions = count_inversions(filename);
    printf("%d", n_inversions);
    return 0;
}
