#include <stdio.h>
#include <stdlib.h>
#include "sifs.h"

//  Written by Michael Sargeant 22737938@student.uwa.edu.au, Oct 2019

//  REPORT HOW THIS PROGRAM SHOULD BE INVOKED
void usage(char *progname)
{
    fprintf(stderr, "Usage: %s volumename dirname\n", progname);
    exit(EXIT_FAILURE);
}



int main(int argcount, char *argvalue[])
{
    char *volumename;
    char *dirname;
    printf("error0\n");
//  OBTAIN volumename AND dirname FROM PARAMETERS    
    if (argcount == 3) {
    volumename = argvalue[1]; 
    dirname = argvalue[2];
    }     
    else {
    usage(argvalue[0]);
    exit(EXIT_FAILURE);
    }

//  ATTEMPT TO CREATE THE NEW DIRECTORY
    if(SIFS_mkdir(volumename, dirname) != 0) {
    printf("error2\n");
	SIFS_perror(argvalue[0]);
	exit(EXIT_FAILURE);
    }
    printf("existsuccess\n");
    return EXIT_SUCCESS;
}

