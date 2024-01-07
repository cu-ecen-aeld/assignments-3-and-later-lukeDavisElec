#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char** argv)
{
    openlog(NULL, 0, LOG_USER);

    if (argc != 3)
    {
        printf("Error, incorrect number of arguments. Usage: <writefile> <writestr>\r\n");
        syslog(LOG_ERR, "Invalid number of arguments: %d", argc);
        return 1;
    }


    char* filePath = argv[1];
    char* writeString = argv[2];
    printf("Writing %s to %s\r\n",writeString, filePath);
    syslog(LOG_DEBUG, "Writing %s to %s",writeString, filePath);

    int fd = creat(filePath, 0644);
    if(fd < 0)
    {
        perror("Error opening file: ");
        syslog(LOG_ERR, "Could not open file: %s", filePath);
        return 1;
    }

    if(write(fd, writeString, strlen(writeString)) < 0)
    {
        perror("Error writing to file: ");
        syslog(LOG_ERR, "Could not write to file: %s", filePath);
        return 1;
    }

    return 0;
}