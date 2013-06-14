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

struct PointArray* split_point_array(struct PointArray* input){
    struct PointArray *split_array = malloc(sizeof(struct PointArray)*2);
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

    split_array[0] = *output1;
    split_array[1] = *output2;
    return split_array;
}

void print_point(struct Point point){
    printf("\n x: %lf y: %lf \n", point.x, point.y);
}

struct PointArray* closest_pair(char *filename){
    struct Connection *conn = File_open(filename, "rt");
    struct PointArray *point_array = Connection_process(conn);
    Connection_close(conn);
    struct PointArray *point_array_splitted = split_point_array(point_array);
    return point_array_splitted;
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
