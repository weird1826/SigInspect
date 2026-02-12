#include<stdio.h>
#include<string.h>

void open_file(const char *filepath){
    FILE *f = fopen(filepath, "rb");

    if(f == NULL) {
        fprintf(stderr, "Error opening file %s: ", filepath);
        perror("");
        return;
    }

    printf("File opened successfully!");
}

int main(int argc, char *argv[]){
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file_to_scan>\n", argv[0]);
    }

    open_file(argv[1]);

    return 0;
}