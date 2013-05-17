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

struct MatrixArray* split_matrix(struct Matrix matrix){
    int split_row_count = matrix.row_count / 2;

    struct Matrix *quadrant1 = malloc(sizeof(struct Matrix));
    quadrant1->row_count = split_row_count;
    struct Row *array_rows1 = malloc(sizeof(struct Row)*split_row_count);

    struct Matrix *quadrant2 = malloc(sizeof(struct Matrix));
    quadrant2->row_count = split_row_count;
    struct Row *array_rows2 = malloc(sizeof(struct Row)*split_row_count);

    struct Matrix *quadrant3 = malloc(sizeof(struct Matrix));
    quadrant3->row_count = split_row_count;
    struct Row *array_rows3 = malloc(sizeof(struct Row)*split_row_count);

    struct Matrix *quadrant4 = malloc(sizeof(struct Matrix));
    quadrant4->row_count = split_row_count;
    struct Row *array_rows4 = malloc(sizeof(struct Row)*split_row_count);

    struct Row *row1;
    struct Row *row2;
    int i=0, j=0, k=0;
    for(i=0; i<matrix.row_count; i++){
        //split row
        struct IntArraySplitted *arr_split = split_array(matrix.rows[i].elements);
        row1 = malloc(sizeof(struct Row));
        row1->elements = arr_split->intArray1;
        row2 = malloc(sizeof(struct Row));
        row2->elements =  arr_split->intArray2;

        //build quadrants
        if(i < split_row_count){
            array_rows1[j] = *row1;
            array_rows2[j] = *row2;
            j++;
        }else{
            array_rows3[k] = *row1;
            array_rows4[k] = *row2;
            k++;
        }

    }

    quadrant1->rows = array_rows1;
    quadrant2->rows = array_rows2;
    quadrant3->rows = array_rows3;
    quadrant4->rows = array_rows4;

    struct MatrixArray *result = malloc(sizeof(struct MatrixArray));
    struct Matrix *matrix_array = malloc(sizeof(struct Matrix)*4);
    matrix_array[0] = *quadrant1;
    matrix_array[1] = *quadrant2;
    matrix_array[2] = *quadrant3;
    matrix_array[3] = *quadrant4;
    result->length = 4;
    result->array = matrix_array;
    return result;
}



struct Matrix* sum(struct Matrix matrix1, struct Matrix matrix2){

    struct Matrix *result = malloc(sizeof(struct Matrix));
    struct Row *row;
    struct Row *array_rows = malloc(sizeof(struct Row)*matrix1.row_count);
    int i,j;
    int *elements_array;
    struct IntArray *intArray_row;
    for(i=0; i<matrix1.row_count; i++){
        intArray_row = malloc(sizeof(struct IntArray));
        elements_array = malloc(sizeof(int)*matrix1.rows[i].elements->length);
        for(j=0; j<matrix1.rows[i].elements->length; j++){
            elements_array[j] = matrix1.rows[i].elements->array[j] +
                                matrix2.rows[i].elements->array[j];
        }
        intArray_row->length = j;
        intArray_row->array = elements_array;
        row = malloc(sizeof(struct Row));
        row->elements = intArray_row;
        array_rows[i] = *row;

    }
    result->rows = array_rows;
    result->row_count = matrix1.row_count;
    return result;
}


struct Matrix* merge_cuadrants(struct Matrix q1, struct Matrix q2){
    struct Matrix *result = malloc(sizeof(struct Matrix));
    struct Row *row;
    struct IntArray *intArray_row;
    int total_columns = q1.rows[0].elements->length + q2.rows[0].elements->length;
    struct Row *array_rows = malloc(sizeof(struct Row)*(q1.row_count));
    int *elements_array;
    int i, j, k, h, y, z  ;

    for(i=0; i<q1.row_count; i++){
        row = malloc(sizeof(struct Row));
        intArray_row = malloc(sizeof(struct IntArray));
        elements_array = malloc(sizeof(int)*total_columns);

        //k:row index h:column index
        h = 0;
        for(j=0; j<total_columns; j++){
            if(j<q1.row_count){
                elements_array[j] = q1.rows[i].elements->array[h];
                h++;
            }else{
                if(j == q1.row_count){
                    h = 0;
                }
                elements_array[j] = q2.rows[i].elements->array[h];
                h++;
            }

        }
        intArray_row->length = total_columns;
        intArray_row->array = elements_array;
        row->elements = intArray_row;
        array_rows[i] = *row;
    }

    result->rows = array_rows;
    result->row_count = q1.row_count;
    return result;

}

struct Matrix* merge(struct Matrix q1, struct Matrix q2,
                     struct Matrix q3, struct Matrix q4){

    struct Matrix *top_merged = malloc(sizeof(struct Matrix));
    struct Matrix *bottom_merged = malloc(sizeof(struct Matrix));
    struct Matrix *result = malloc(sizeof(struct Matrix));
    struct Row *row;
    struct IntArray *intArray_row;
    int total_rows = q1.row_count + q3.row_count;
    struct Row *array_rows = malloc(sizeof(struct Row)*(total_rows));

    top_merged = merge_cuadrants(q1, q2);
    bottom_merged = merge_cuadrants(q3, q4);

    int i,j=0,k=0;
    for (i=0; i<total_rows; i++){
        if(i<top_merged->row_count){
            array_rows[i] = top_merged->rows[j];
            j++;
        }else{
            array_rows[i] = bottom_merged->rows[k];
            k++;
        }
    }

    result->rows = array_rows;
    result->row_count = total_rows;
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
    }else{
        //Split matrices in cuadrants
        struct MatrixArray *split_matrices1 = split_matrix(matrix1);
        struct MatrixArray *split_matrices2 = split_matrix(matrix2);

        struct Matrix *A_E = malloc(sizeof(struct Matrix));
        struct Matrix *B_G = malloc(sizeof(struct Matrix));
        struct Matrix *A_F = malloc(sizeof(struct Matrix));
        struct Matrix *B_H = malloc(sizeof(struct Matrix));
        struct Matrix *C_E = malloc(sizeof(struct Matrix));
        struct Matrix *D_G = malloc(sizeof(struct Matrix));
        struct Matrix *C_F = malloc(sizeof(struct Matrix));
        struct Matrix *D_H = malloc(sizeof(struct Matrix));

        A_E = product(split_matrices1->array[0], split_matrices2->array[0]);
        B_G = product(split_matrices1->array[1], split_matrices2->array[2]);
        A_F = product(split_matrices1->array[0], split_matrices2->array[1]);
        B_H = product(split_matrices1->array[1], split_matrices2->array[3]);
        C_E = product(split_matrices1->array[2], split_matrices2->array[0]);
        D_G = product(split_matrices1->array[3], split_matrices2->array[2]);
        C_F = product(split_matrices1->array[2], split_matrices2->array[1]);
        D_H = product(split_matrices1->array[3], split_matrices2->array[3]);

        struct Matrix *quadrant1 = malloc(sizeof(struct Matrix));
        struct Matrix *quadrant2 = malloc(sizeof(struct Matrix));
        struct Matrix *quadrant3 = malloc(sizeof(struct Matrix));
        struct Matrix *quadrant4 = malloc(sizeof(struct Matrix));

        quadrant1 = sum(*A_E, *B_G);
        quadrant2 = sum(*A_F, *B_H);
        quadrant3 = sum(*C_E, *D_G);
        quadrant4 = sum(*C_F, *D_H);

        result = merge(*quadrant1, *quadrant2, *quadrant3, *quadrant4);
/*        printf("\n quadrant1 \n");
        print_matrix(*quadrant1);

        printf("\n quadrant2 \n");
        print_matrix(*quadrant2);

        printf("\n quadrant3 \n");
        print_matrix(*quadrant3);

        printf("\n quadrant4 \n");
        print_matrix(*quadrant4);*/


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
