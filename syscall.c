//-------------use of I/O system calls for console//

// #include <unistd.h>
// #include <stdio.h>
// #define BUFFER_SIZE 30
// int main() {
//     char buffer[BUFFER_SIZE];
//     int n = read(0, buffer, BUFFER_SIZE); 
//     printf("Number of characters read: %d\n", n);
//     write(1, buffer, n); 
//     return 0;
// }


//-------------use of I/O system calls for File I/O//

#include <stdio.h>       
#include <stdlib.h>      
#include <fcntl.h>       
#include <unistd.h>      
#include <sys/types.h>   
#include <sys/stat.h>    

#define BUFFER_SIZE 30

int main() {
    char buffer[BUFFER_SIZE];
    int n;

    // Open the file with read-write permission
    int fd = open("hello.txt", O_RDWR);
    
    if (fd == -1) {
        perror("Failed to open file");
        exit(1);
    }

    printf("File descriptor is %d\n", fd);

    // 1. Read first 10 characters
    printf("\nReading first 10 characters from file:\n");
    n = read(fd, buffer, 10);
    write(1, buffer, n);

    // 2. Skip 5 characters from current position
    printf("\n\nSkipping 5 characters from current position:\n");
    lseek(fd, 5, SEEK_CUR);  // Move 5 bytes ahead from current
    n = read(fd, buffer, 10);
    write(1, buffer, n);

    // 3. Move 10 characters back from current position
    printf("\n\nGoing 10 characters back from current position:\n");
    lseek(fd, -10, SEEK_CUR);  // Move 10 bytes back from current
    n = read(fd, buffer, 10);
    write(1, buffer, n);

    // 4. Go to 5th last character from end of file
    printf("\n\nGoing to the 5th last character in the file:\n");
    lseek(fd, -5, SEEK_END);  // 5 bytes before end of file
    n = read(fd, buffer, 5);
    write(1, buffer, n);

    // 5. Go to 3rd character from beginning
    printf("\n\nGoing to the 3rd character in the file:\n");
    lseek(fd, 3, SEEK_SET);  // 3rd byte from start
    n = read(fd, buffer, 5);
    write(1, buffer, n);

    printf("\n");

    close(fd); // Always close the file
    return 0;
}
