#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "ntutils.h"
#define CHUNK_SIZE 256

struct Row{
    struct IntArray *elements;
};

struct Matrix{
    struct Row *rows;
    int row_count;
};

struct MatrixArray{
    struct Matrix *array;
    int length;
};

/*process the text file and returns a Matrix struct array ready to apply strassen
 * Split string by tokens: http://stackoverflow.com/questions/2523467/how-to-split-a-string-to-2-strings-in-c
 */
struct MatrixArray *Connection_process(struct Connection *conn){
    char *buffer = malloc(CHUNK_SIZE);
    char *file_line = NULL;
    char *file_line_token = NULL;
    char *file_line_matrix = NULL;
    char *file_line_row = NULL;
    char *file_line_space_token = NULL;
    int nread = 0;
    int n_lines=1;
    int n_rows=0;

    //martix structre variables
    //
    struct MatrixArray *matrix_array_result = malloc(sizeof(struct MatrixArray));
    struct Matrix *matrix_array = malloc(sizeof(struct Matrix)*CHUNK_SIZE);
    struct Matrix *matrix = malloc(sizeof(struct Matrix));
    int array_matrix_index = 0;

    struct Row *row;
    struct Row *array_rows = malloc(sizeof(struct Row));

    int array_rows_index = 0;
    int *array_numbers;
    int array_numbers_index = 0;
    struct IntArray *intArray_row;

    if(!buffer) die("Memory error !! \n");
    //read every line of the file
    nread = fread(buffer, 1, CHUNK_SIZE, conn->file);

    while (nread > 0) {
        n_lines++;
        //copy the buffer to a string
        file_line = strdup(buffer);
        if(file_line != NULL){
           //split the buffer line by line
            while ((file_line_token = strsep(&file_line, "\n")) != NULL)
            {

                if (strcmp(file_line_token, "" )!=0){
                    file_line_matrix = strdup(file_line_token);
                    matrix = malloc(sizeof(struct Matrix));
                    array_rows_index = 0;
                    array_rows = malloc(sizeof(struct Row));
                    if(file_line_matrix != NULL){
                        while ((file_line_row = strsep(&file_line_matrix, ";")) != NULL){
                            if (strcmp(file_line_row, "" )==0){
                                break;
                            }
                            //split by matrix rows, every row is divided by ; char


                            //split by rows elements
                            if(file_line_row != NULL){
                                array_numbers = malloc(sizeof(int) * CHUNK_SIZE);
                                array_numbers_index = 0;
                                while ((file_line_space_token = strsep(&file_line_row, " ")) != NULL){
                                    if (!strcmp(file_line_space_token, "") == 0){
                                        array_numbers[array_numbers_index] = atoi(file_line_space_token);
                                        array_numbers_index++;
                                    }
                                }
                                free(file_line_space_token);
                                intArray_row = malloc(sizeof(struct IntArray));
                                intArray_row->array = array_numbers;
                                intArray_row->length = array_numbers_index;
                                row = malloc(sizeof(struct Row));
                                row->elements = intArray_row;

                                array_rows = (struct Row*) realloc(array_rows, sizeof(struct Row) * (array_rows_index+1));
                                array_rows[array_rows_index] = *row;
                                array_rows_index++;
                            }
                        }
                        matrix->rows = array_rows;
                        matrix->row_count = array_rows_index;
                        free(file_line_row);

                    }
                    free(file_line_matrix);
                    matrix_array[array_matrix_index] = *matrix;
                    array_matrix_index++;
                }
            }
            free(file_line_token);
        }else{
            break;
        }
        nread = fread(buffer, 1, CHUNK_SIZE, conn->file);

        free(file_line);
    }

    matrix_array_result->length = array_matrix_index - 1;
    matrix_array_result->array = matrix_array;
    return matrix_array_result;
}

void print_matrix(struct Matrix matrix){
    int i=0, j=0;
    printf("\n count %d \n", matrix.row_count);
    printf("[");
    for(i=0; i<matrix.row_count; i++){
        for(j=0; j<matrix.rows[i].elements->length; j++){
            printf(" %d ", matrix.rows[i].elements->array[j]);
        }
        printf(";");

    }
    printf("]");
}

struct Matrix* strassen(int A, int B, int C, int D, int E, int F, int G, int H){
    struct Matrix *result = malloc(sizeof(struct Matrix));
    struct Row *row;
    struct Row *array_rows = malloc(sizeof(struct Row)*2);

    int *array_numbers;
    int P1=0, P2=0, P3=0, P4=0, P5=0, P6=0, P7=0;
    struct IntArray *intArray_row;


    P1 = A * (F - H);
    P2 = (A +B ) * H;
    P3 = (C + D) * E;
    P4 = D * (G - E);
    P5 = (A + D) * (E + H);
    P6 = (B - D) * (G +H);
    P7 = (A - C) * (E + F);

    array_numbers = malloc(sizeof(int)*2);
    array_numbers[0] = P5 + P4 - P2 + P6;
    array_numbers[1] = P1 + P2;
    intArray_row = malloc(sizeof(struct IntArray)*2);
    intArray_row->length = 2;
    intArray_row->array = array_numbers;
    row = malloc(sizeof(struct Row));
    row->elements = intArray_row;
    array_rows[0] = *row;

    array_numbers = malloc(sizeof(int)*2);
    array_numbers[0] = P3 + P4;
    array_numbers[1] = P1 + P5  - P3 -P7;
    intArray_row = malloc(sizeof(struct IntArray)*2);
    intArray_row->length = 2;
    intArray_row->array = array_numbers;
    row = malloc(sizeof(struct Row));
    row->elements = intArray_row;
    array_rows[1] = *row;

    result->rows = array_rows;
    result->row_count = 2;
    return result;
}

struct Matrix* product(struct Matrix matrix1, struct Matrix matrix2){
    struct Matrix *result = malloc(sizeof(struct Matrix));

    if(matrix1.row_count == 2 && matrix1.row_count == 2){
           result = strassen(matrix1.rows[0].elements->array[0],
                             matrix1.rows[0].elements->array[1],
                             matrix1.rows[1].elements->array[0],
                             matrix1.rows[1].elements->array[1],
                             matrix2.rows[0].elements->array[0],
                             matrix2.rows[0].elements->array[1],
                             matrix2.rows[1].elements->array[0],
                             matrix2.rows[1].elements->array[1]
                             );
    }


    return result;
}

struct Matrix* matrix_product(char* filename){
    struct Connection *conn = File_open(filename, "rt");
    struct Matrix *result = malloc(sizeof(struct Matrix));
    struct MatrixArray *input = Connection_process(conn);
    Connection_close(conn);
    result = product(input->array[0], input->array[1]);
    return result;
}



/* strassen.c : Strassen's matrix multiplication implementation as personal practice for
 * Standford online course
 * Algorithms: Design and Analysis, Part I https://www.coursera.org/course/algo
 * Usage: strassen <file>
 * file must be a text file with numbers separated by blank spaces, the output will
 * be matrix result printed at console
 */
int main(int argc, char *argv[])
{

    if (argc != 2) die("USAGE: strassen <file> ");
    struct Matrix *output = malloc(sizeof(struct Matrix));
    char *filename = argv[1];
    output = matrix_product(filename);
    print_matrix(*output);
    return 0;
}
