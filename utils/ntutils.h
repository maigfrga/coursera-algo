struct Connection{
  FILE *file;
};

struct IntArray{
    int *array;
    int length;

};

void die(const char *);
struct Connection* File_open(char *filename, char *mode);
void Connection_close(struct Connection *conn);
