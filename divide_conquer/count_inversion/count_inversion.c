#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "ntutils.h"
#define CHUNK_SIZE 256 /* read 256 bytes at a time */
struct Inversion{
    int count;
    struct IntArray *array;
};


/*process the text file and returns an Inversion struct with an input int array
 * ready to apply count inversions
 * Split string by tokens: http://stackoverflow.com/questions/2523467/how-to-split-a-string-to-2-strings-in-c
 */
struct Inversion *Connection_process(struct Connection *conn){
    char *buffer = malloc(CHUNK_SIZE);
    char *file_line = NULL;
    char *file_line_token = NULL;
    char *file_line_space = NULL;
    char *file_line_space_token = NULL;
    int nread = 0;
    int *array_numbers;
    int array_index=0;
    struct Inversion *output_inversion = malloc(sizeof(struct Inversion));
    struct IntArray *struct_array = malloc(sizeof(struct IntArray));


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
    struct_array->array = (int*)malloc(sizeof(int)*array_index);
    struct_array->array = array_numbers;
    struct_array->length = array_index;
    output_inversion->count = 0;
    output_inversion->array = struct_array;
    return output_inversion;
}

struct Inversion* merge_count(struct Inversion *left_inversion, struct Inversion *right_inversion){
    int length = left_inversion->array->length + right_inversion->array->length;
    int i=0, j=0, k=0, inversion_count = 0;

    struct IntArray *output_array = malloc(sizeof(struct IntArray));
    output_array->array = malloc(sizeof(int)*length);
    output_array->length = length;
    i=0;

    for(k=0; k<length; k++){
        if (left_inversion->array->length > i && right_inversion->array->length > j){
            if(left_inversion->array->array[i] < right_inversion->array->array[j]){
                output_array->array[k] = left_inversion->array->array[i];
                i++;
            }else{
                output_array->array[k] = right_inversion->array->array[j];
                j++;
                inversion_count += (left_inversion->array->length-i);
            }
        }
        else if(left_inversion->array->length > i){
            output_array->array[k] = left_inversion->array->array[i];
            i++;
        }

        else if(right_inversion->array->length > j){
            output_array->array[k] = right_inversion->array->array[j];
            j++;
        }
        else{printf("i: %d, j: %d , k: %d,  \n", i, j, k);die("error in merge_count");}

    }

    struct Inversion *inversion_return = malloc(sizeof(struct Inversion));
    inversion_return->count = inversion_count;
    inversion_return->array = output_array;
    return inversion_return;
}


struct Inversion* sort_count(struct Inversion *input){
    if(input->array->length==1){
        return input;
    }
    else{
        struct IntArraySplitted *split_output = malloc(sizeof(struct IntArraySplitted));
        split_output = split_array(input->array);

        struct Inversion *left_inversion = malloc(sizeof(struct Inversion));
        struct Inversion *right_inversion = malloc(sizeof(struct Inversion));

        left_inversion->count = 0;
        left_inversion->array = split_output->intArray1;
        right_inversion->count = 0;
        right_inversion->array = split_output->intArray2;
        if(left_inversion->array->length > 1){
            left_inversion = sort_count(left_inversion);
        }

        if(right_inversion->array->length > 1){
            right_inversion = sort_count(right_inversion);
        }
        struct Inversion *out_inversion = malloc(sizeof(struct Inversion));
        out_inversion = merge_count(left_inversion, right_inversion);
        out_inversion->count+= (left_inversion->count + right_inversion->count);
        return out_inversion;
    }

}


struct Inversion *count_inversion(char *filename){
    struct Connection *conn = File_open(filename, "rt");
    struct Inversion *input = Connection_process(conn);
    struct Inversion *left_inversion = malloc(sizeof(struct Inversion));
    struct Inversion *right_inversion = malloc(sizeof(struct Inversion));
    struct Inversion *output = malloc(sizeof(struct Inversion));
    struct IntArraySplitted *split_output = malloc(sizeof(struct IntArraySplitted));

    Connection_close(conn);
    split_output = split_array(input->array);

    left_inversion->count = 0;
    left_inversion->array = split_output->intArray1;
    right_inversion->count = 0;
    right_inversion->array = split_output->intArray2;
    left_inversion = sort_count(left_inversion);
    right_inversion = sort_count(right_inversion);


    output = merge_count(left_inversion, right_inversion);
    output->count += left_inversion->count;
    output->count += right_inversion->count;

    return output;
}


/* count_inversion.c : Count inversions implementation as personal practice for Standford online course
 * Algorithms: Design and Analysis, Part I https://www.coursera.org/course/algo
 * Usage: count_inversion <file>
 * file must be a text file with numbers separated by blank spaces, the output will
 * be inversion number printed at console
 */
int main(int argc, char *argv[])
{

    if (argc != 2) die("USAGE: count_inversion <file> ");
    struct Inversion *output = malloc(sizeof(struct Inversion));
    char *filename = argv[1];
    output = count_inversion(filename);
    printf("\n array length: %d \n number of inversions: %d \n",
           output->array->length, output->count);

    return 0;
}
