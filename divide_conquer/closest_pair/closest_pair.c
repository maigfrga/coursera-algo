#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "ntutils.h"
#define CHUNK_SIZE 256 /* read 256 bytes at a time */

struct Point{
    int x;
    int y;
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
    int nread = 0;
    int *array_numbers;
    int array_index=0;
    struct PointArray *point_array = malloc(sizeof(struct PointArray));


    if(!buffer) die("Memory error !! \n");
    //read every line of the file
    nread = fread(buffer, 1, CHUNK_SIZE, conn->file);
    while (nread > 0) {
        //copy the buffer to a string
        file_line = strdup(buffer);
        if(file_line != NULL){
           //split the buffer line by line
            while ((file_line_token = strsep(&file_line, "\n")) != NULL){
                //split every line by blank spaces
                file_line_space = strdup(file_line_token);
                while ((file_line_space_token = strsep(&file_line_space, " ")) != NULL){
                    if (strcmp(file_line_space_token, "" )==0){
                        break;
                    }
                    printf("\n -  %s - \n", file_line_space_token);
                }
                free(file_line_token);
            }
        }
    }
    return point_array;
}







struct PointArray* closest_pair(char *filename){
    struct Connection *conn = File_open(filename, "rt");
    struct PointArray *point_array = Connection_process(conn);
    Connection_close(conn);

    return point_array;
}


void print_point(struct Point point){
    printf("\n x: %d y: %d \n", point.x, point.y);
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
    printf("closest pair points :\n");
    printf("point 1 :\n");
    print_point(result->array[0]);
    printf("point 2 :\n");
    print_point(result->array[1]);
    return 0;
}
