struct Connection{
  FILE *file;
};

struct IntArray{
    int *array;
    int length;

};

struct IntArraySplitted{
    struct IntArray *intArray1;
    struct IntArray *intArray2;
};

void die(const char *);
struct Connection* File_open(char *filename, char *mode);
void Connection_close(struct Connection *conn);
struct IntArraySplitted* split_array(struct IntArray *input);
