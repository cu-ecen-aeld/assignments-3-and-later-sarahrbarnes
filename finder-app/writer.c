
#include <stdio.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
    FILE* filePtr = NULL;
    char* filePath;
    char* writeStr;

    // Setup syslog logging
    openlog(NULL, 0, LOG_USER);

    if (argc == 3)
    {
        filePath = argv[1];
        writeStr = argv[2];
        filePtr = fopen(filePath, "w+");

        if (filePtr)
        {
            fprintf(filePtr, "%s", writeStr);
            fclose(filePtr);
            syslog(LOG_DEBUG, "Writing %s to %s", writeStr, filePath);
        }
        else
        {
            syslog(LOG_ERR, "Unable to open file: %s for write", filePath);
        }

    }
    else
    {
        syslog(LOG_ERR, "Invalid number of arguments %d", argc);
        return 1;
    }

    return 0;
}
