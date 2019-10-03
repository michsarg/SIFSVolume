#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sifs-internal.h"


void test (int number) {
	printf("test %i\n", number);
}
// make a new directory within an existing volume
int SIFS_mkdir(const char *volumename, const char *dirname)
{

	/*
	// ENSURE THAT RECEIVED PARAMETERS ARE VALID

		//check existence of volume
	        if(access(volumename, F_OK) != 0) {
				SIFS_errno	= SIFS_ENOVOL;
				return 1;
	    }
		// check proposed foldername length
			if((strlen(dirname)+1) > SIFS_MAX_NAME_LENGTH) {
				SIFS_errno	= SIFS_ENAMELEN;
				return 1;
	    }
	*/

	// ENSURE THAT REQUESTED DIRECTORY DOES NOT ALREADY EXIST
	test(1);
	// check bitmap for directory blocks
	// open volume for writing
	FILE *vol = fopen(volumename, "w");
	test(2);

	/* 
	// check if other directory blocks have same directory name already
    for(int b=1 ; b<nblocks ; ++b) {
    	//find directory block
        if (bitmap[b] == SIFS_DIR) {
        	// check for same name
        	if (strcmp(dirname, SIFS_DIRBLOCK.name) != 1) {  //not right for accessing names
        		// exit with error if same named directory exists
        		SIFS_errno = SIFS_EEXIST; 
        		return 1;
        	}
        }
    }
	*/

	// ATTEMPT TO CREATE THE NEW DIRECTORY - OPEN VOLUME FOR WRITING
	// use bitmap to find first free block (directory block is 1 block)
	
	/*
	uint32_t bid = 0;
	while (bitmap[bid] != SIFS_UNUSED) {
		++bid;
		}

	// 	UPDATE THE BITMAP
	bitmap[bid] = SIFS_DIR;
	*/
	// UPDATE THE ROOT DIRECORY
	// how?

	//read from header and bitmap
	SIFS_VOLUME_HEADER  header;
	fread(&header, sizeof header, 1, vol);
	SIFS_BIT	bitmap[header.nblocks];
	fread(&bitmap, sizeof bitmap, 1, vol);

	test(3);

	// DEFINE AND INTIALISE VARIABLES FOR DIRECTORY BLOCK
	size_t blocksize = header.blocksize;
    char	oneblock[blocksize];

    SIFS_DIRBLOCK	dir_block;
    memset(&dir_block, 0, sizeof dir_block);	// cleared to all zeroes
    
    strcpy(dir_block.name, dirname);
    dir_block.modtime	= time(NULL);
    dir_block.nentries	= 0;
    memset(oneblock, 0, sizeof oneblock);        // cleared to all zeroes
    memcpy(oneblock, &dir_block, sizeof dir_block);
    
    // SEEK THE CORRECT LOCATION
    long offset = ((sizeof header + sizeof bitmap) + ((sizeof oneblock) * 1));
    fseek(vol, offset, SEEK_SET);
    
    // WRITE THE DIR BLOCK TO THE LOCATION
	fwrite(&dir_block, sizeof dir_block, 1, vol);	// write dir

	// FINISHED, CLOSE THE VOLUME
	fclose(vol);
	test(4);
	/*
	// DIRECTORY CREATION FAILED
		// return error message
    SIFS_errno	= SIFS_ENOTYET;
    return 1;
	*/

	// RETURN INDICATING SUCCESS
    return 0;


}
