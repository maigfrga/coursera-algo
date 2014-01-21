#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

struct Node{
    int value;
    struct Node *next_node;
};




/**Append an int at the end of the list.
 * Because traverse the whole list is required in order
 * to get the last value the cost of this operation is O(n)
 * */
void append_node(struct Node *linked_list, int value){
    struct Node *temp_node = malloc(sizeof(struct Node));


    if(linked_list->next_node){

        temp_node->next_node = linked_list->next_node;
        while(temp_node->next_node){
            temp_node = temp_node->next_node;
        }
        struct Node *new_node = malloc(sizeof(struct Node));
        new_node->value = value;
        new_node->next_node = NULL;
        temp_node->next_node = new_node;

    }else{
        temp_node->value = value;
        temp_node->next_node = NULL;
        linked_list->next_node = temp_node;
    }



}


/** Travese list operation cost: O(n)
 * */
void traverse_list(struct Node *linked_list){
    struct Node *temp_node = linked_list;
    printf("\n%d ", temp_node->value);

    while(temp_node->next_node){
        temp_node = temp_node->next_node;
        printf("%d ", temp_node->value);
    }

    printf("\n");

}

/**
 * IF position is 0 the operation cost is O(1), otherwise O(n)
 * */
struct Node *insert_node(struct Node *linked_list, int position, int value){
    struct Node *new_node = malloc(sizeof(struct Node));
    new_node->value = value;

    if(position == 0){
        struct Node *temporal = linked_list;
        new_node->next_node = temporal;
        linked_list = new_node;
    }
    else{
        int position_counter = 0;
        struct Node *temp_node = linked_list;

        //if position is greater that list len valud will be inserted at the end
        while(temp_node->next_node && position_counter < position){
            temp_node = temp_node->next_node;
            position_counter += 1;
        }

        //If there if more elements in the list
        if(temp_node->next_node){
            new_node->next_node = temp_node->next_node;
            temp_node->value = new_node->value;
        }else{
            //insert element at the end of the list
            temp_node->next_node = new_node;
        }
    }
    return linked_list;
}

/**
 * Build a list of len elements
* */
struct Node *build_list(int n){
    struct Node *linked_list = malloc(sizeof(struct Node));
    int i;

    linked_list->value = 0;
    linked_list->next_node = NULL;
    for (i=1; i<n; i++){
        append_node(linked_list, i);
    }
    return linked_list;
}



/**Reverse list cost O(n)
 * */
struct Node *head;
struct Node *reverse_list(struct Node *linked_list){

    if (linked_list->next_node == NULL){
        head = linked_list;
        return;
    }

    reverse_list(linked_list->next_node);
    struct Node *temp = linked_list->next_node;
    temp->next_node = linked_list;
    linked_list->next_node = NULL;
    return head;
}


/**
 * Linked List Implementation Example
**/
int main(int argc, char *argv[])
{
    int list_len = 4;

    if (argc == 2){
        list_len = atoi(argv[1]);
    }
    struct Node *linked_list = build_list(list_len);
    traverse_list(linked_list);

    srand(time(NULL));
    int random_int = rand();
    printf("inserting %d value at the beginning of the list: \n", random_int);
    linked_list = insert_node(linked_list, 0, random_int);
    traverse_list(linked_list);


    random_int = rand();
    printf("inserting %d value at the middle of the list: \n", random_int);
    linked_list = insert_node(linked_list, list_len/2, random_int);
    traverse_list(linked_list);

    random_int = rand();
    printf("inserting %d value at the end of the list: \n", random_int);
    linked_list = insert_node(linked_list, list_len, random_int);
    traverse_list(linked_list);

    printf("Reversing the list: \n");
    linked_list = reverse_list(linked_list);
    traverse_list(linked_list);
    return 0;
}
