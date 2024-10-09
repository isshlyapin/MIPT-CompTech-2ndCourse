#include <stdio.h>
#include <stdlib.h>

int compare_md5(const char* file1, const char* file2) {
    char command[256];
    
    snprintf(command, sizeof(command), "md5sum %s %s | awk '{print $1}' | uniq | wc -l", file1, file2);
    
    int result = system(command);
    
    return result == 1;
}