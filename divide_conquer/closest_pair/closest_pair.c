#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ntutils.h"
#define CHUNK_SIZE 256 /* read 256 bytes at a time */

struct Point{
    float x;
    float y;
};

struct PointArray{
    struct Point *array;
    int length;

};

struct PointArraySplitted{
    struct PointArray *array1;
    struct PointArray *array2;
};

struct OrderedPointArray{
    struct PointArray *x_ordered;
    struct PointArray *y_ordered;
};

/*process the text file and returns a PointArray struct
 * ready to apply closest_pair
 * Split string by tokens: http://stackoverflow.com/questions/2523467/how-to-split-a-string-to-2-strings-in-c
 */
struct PointArray *Connection_process(struct Connection *conn){
    char *buffer = malloc(CHUNK_SIZE);
    char *file_line = NULL;
    char *file_line_token = NULL;
    char *file_line_space = NULL;
    char *file_line_space_token = NULL;
    char *point_token = NULL;
    char *coordinate_token = NULL;
    int nread = 0;
    int int_point = 0;
    char *point_control = NULL;
    struct PointArray *point_array = malloc(sizeof(struct PointArray));

    int array_index = 0;
    struct Point *point = malloc(sizeof(struct PointArray));
    struct Point *arr_point = NULL;

    if(!buffer) die("Memory error !! \n");
    //read every line of the file
    nread = fread(buffer, 1, CHUNK_SIZE, conn->file);
    while (nread > 0) {
        //copy the buffer to a string
        file_line = strdup(buffer);
        if(file_line != NULL){
           //split the buffer line by line
            while ((file_line_token = strsep(&file_line, "\n")) != NULL){
                if (strcmp(file_line_token, "" )==0){
                    break;
                }
                //split every line by blank spaces
                file_line_space = strdup(file_line_token);
                while ((file_line_space_token = strsep(&file_line_space, " ")) != NULL){
                    if (strcmp(file_line_space_token, "" )==0){
                        break;
                    }

                    arr_point = (struct Point*) realloc(arr_point, sizeof(struct Point) * (array_index+1));
                    point = malloc(sizeof(struct Point));
                    //split point
                    if(file_line_space_token != NULL){
                        point_token = strdup(file_line_space_token);
                        while ((point_token = strsep(&file_line_space_token, ",")) != NULL){
                            if (strcmp(point_token, "" )==0 ){
                                break;
                            }
                            if(NULL == point_control){
                                point->x = atoi(point_token);
                                point_control = "A";
                            }else{
                                point->y = atoi(point_token);
                                point_control = NULL;
                            }
                        }
                            arr_point[array_index] = *point;
                            array_index++;

                    }
                }
                free(file_line_token);
            }
        }
        nread = fread(buffer, 1, CHUNK_SIZE, conn->file);
        free(file_line);
    }
    point_array->array = arr_point;
    point_array->length = array_index;
    return point_array;
}


float euclidian_distance(struct Point p1, struct Point p2){
    float result=sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2));
    return result;
}




//Split a Point array in two
struct PointArraySplitted *split_point_array(struct PointArray* input){
    struct PointArraySplitted *split_array = malloc(sizeof(struct PointArraySplitted));
    struct PointArray *output1 = malloc(sizeof(struct PointArray));
    struct PointArray *output2 = malloc(sizeof(struct PointArray));
    struct Point *array1;
    struct Point *array2;
    //check if the length of the array is pair or not
    if(input->length % 2 == 0){
        output1->length = (input->length/2);
        output2->length = output1->length;
    }else{
        output1->length = (input->length)/2 + 1;
        output2->length = output1->length -1 ;
    }

    array1 = malloc(sizeof(struct Point) * output1->length);
    array2 = malloc(sizeof(struct Point) * output2->length);

    array1 = input->array;
    output1->array = array1;

    int i=0, j=0;
    for(i=output1->length; i< input->length; i++){
        array2[j] = input->array[i];
        j++;
    }
    output2->array = array2;

    split_array->array1 = output1;
    split_array->array2 = output2;
    return split_array;
}

void print_point(struct Point point){
    printf("\n x: %lf y: %lf \n", point.x, point.y);
}


struct OrderedPointArray* merge_point_array(struct OrderedPointArray *left, struct OrderedPointArray *right){
    int length = left->x_ordered->length + right->x_ordered->length;
    int i=0, j=0, k=0, inversion_count = 0;
    //ordered by x
    struct PointArray *x_ordered = malloc(sizeof(struct PointArray)*length);
    x_ordered->length = length;
    struct Point *x_ordered_array = malloc(sizeof(struct Point)*length);
    for(i=0; i<length; i++){
        if(left->x_ordered->array[j].x > right->x_ordered->array[k].x && k < right->x_ordered->length){
            x_ordered_array[i] = right->x_ordered->array[k];
            k++;
        }else if(j< left->x_ordered->length){
            x_ordered_array[i] = left->x_ordered->array[j];
            j++;
        }else if(k < right->x_ordered->length){
            x_ordered_array[i] = right->x_ordered->array[k];
            k++;
        }

    }
    x_ordered->array = x_ordered_array;

    //ordered by y
    i=0, j=0, k=0;
    struct PointArray *y_ordered = malloc(sizeof(struct PointArray)*length);
    y_ordered->length = length;
    struct Point *y_ordered_array = malloc(sizeof(struct Point)*length);
    for(i=0; i<length; i++){
         if(left->y_ordered->array[j].y > right->y_ordered->array[k].y && k < right->y_ordered->length){
            y_ordered_array[i] = right->y_ordered->array[k];
            k++;
        }else if(j< left->y_ordered->length){
            y_ordered_array[i] = left->y_ordered->array[j];
            j++;
        }else if(k < right->y_ordered->length){
            y_ordered_array[i] = right->y_ordered->array[k];
            k++;
        }
    }
    y_ordered->array = y_ordered_array;

    struct OrderedPointArray *array_ordered = malloc(sizeof(struct OrderedPointArray));
    array_ordered->x_ordered = x_ordered;
    array_ordered->y_ordered = y_ordered;

    return array_ordered;
}

struct OrderedPointArray* split_sort(struct OrderedPointArray *input){
    if(input->x_ordered->length == 1){
        return input;
    }else{
        struct PointArraySplitted *split_array = split_point_array(input->x_ordered);

        struct OrderedPointArray *ordered1 = malloc(sizeof(struct OrderedPointArray));
        ordered1->x_ordered = split_array->array1;
        ordered1->y_ordered = split_array->array1;
        struct OrderedPointArray *arr1 = split_sort(ordered1);
        int p=0;
        struct OrderedPointArray *ordered2 = malloc(sizeof(struct OrderedPointArray));
        ordered2->x_ordered = split_array->array2;
        ordered2->y_ordered = split_array->array2;
        struct OrderedPointArray *arr2 = split_sort(ordered2);


        struct OrderedPointArray *array_ordered = merge_point_array(arr1, arr2);
        return array_ordered;
    }

}

struct PointArray* closest_pair(char *filename){
    struct Connection *conn = File_open(filename, "rt");
    struct PointArray *point_array = Connection_process(conn);
    Connection_close(conn);
    struct PointArraySplitted *split_array = split_point_array(point_array);

    struct OrderedPointArray *ordered1 = malloc(sizeof(struct OrderedPointArray));
    ordered1->x_ordered = split_array->array1;
    ordered1->y_ordered = split_array->array1;

    struct OrderedPointArray *ordered2 = malloc(sizeof(struct OrderedPointArray));
    ordered2->x_ordered = split_array->array2;
    ordered2->y_ordered = split_array->array2;

    struct PointArray *result;
    struct OrderedPointArray *result1 = split_sort(ordered1);
    struct OrderedPointArray *result2 = split_sort(ordered2);

    struct OrderedPointArray *result_merged = merge_point_array(result1, result2);

    int i =0;
    for (i=0; i<result_merged->x_ordered->length; i++){
        print_point(result_merged->x_ordered->array[i]);
    }

    printf("\n y : --------------------------\n");
    for (i=0; i<result_merged->y_ordered->length; i++){
        print_point(result_merged->y_ordered->array[i]);
    }

    return result;
}





/* closest_pair.c : Closest pair implementation as personal practice for Standford online course
 * Algorithms: Design and Analysis, Part I https://www.coursera.org/course/algo
 * Usage: closest_pair <file>
 * file must be a text file with the points x,y  every point must be separated by blank spaces, the output will
 * be the closest pair
 */
int main(int argc, char *argv[])
{

    if (argc != 2) die("USAGE: closest_pair <file> ");
    struct PointArray *result = malloc(sizeof(struct PointArray));
    char *filename = argv[1];
    result = closest_pair(filename);
    /*
    printf("closest pair points :\n");
    printf("point 1 :\n");
    print_point(result->array[0]);
    printf("point 2 :\n");
    print_point(result->array[1]);
    */
    return 0;
}
