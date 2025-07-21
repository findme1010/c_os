#include <stdio.h>

int main() {
    char fn[100];
    printf("Enter source filename: ");
    scanf("%99s", fn);
    if (remove(fn) == 0) 
        printf("File removed successfully.\n");
    else 
        perror("Error");
    return 0;
}
