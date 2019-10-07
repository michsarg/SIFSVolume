#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sifs-internal.h"

// make a new directory
int SIFS_mkdir(const char *volumename, const char *dirname)
{
//	ENSURE THAT RECEIVED PARAMETERS ARE VALID
    if(volumename == NULL || dirname == NULL
    	|| strlen(volumename) > SIFS_MAX_NAME_LENGTH
    	|| strlen(dirname) > SIFS_MAX_NAME_LENGTH) {
	SIFS_errno	= SIFS_EINVAL;
	return 1;
    }

//	ENSURE THAT THE REQUESTED VOLUME DOES EXIST    	
    if(access(volumename, F_OK) != 0) {
	SIFS_errno	= SIFS_EEXIST;
	return 1;
    }   

	// open volume for writing
	FILE *fp = fopen(volumename, "r+"); // opens file reading and writing


// 	CHECK HEADER
	//copy blocksize and nblocks locally
	fseek(fp, 0, SEEK_SET); 				// seek to start of file
	SIFS_VOLUME_HEADER header;				// create local header
	fread(&header, sizeof header, 1, fp);	// read from file header to local header
	int blocksize = header.blocksize;		// store blocksize in local variable
	int nblocks = header.nblocks;			// store nblocks in local variable


//	ENSURE THAT THE REQUESTED DIR DOES NOT ALREADY EXIST
	// write to check just first level of directory
    // search bitmap for dir files
	fseek(fp, sizeof header, SEEK_SET); 	// seek to start of bitmap
	SIFS_BIT checkbitmap[nblocks];				// create local bitmap
	fread(checkbitmap, sizeof checkbitmap, 1, fp);	// read from file bitmap to local bitmap
	// check dir block for same name

    // search for matching names
	SIFS_DIRBLOCK checkdirblock;					// create local rootdir_block
	for (int i = 0; i < nblocks; ++i) {
		if (checkbitmap[i] == SIFS_DIR) {
			fseek(fp, (sizeof header + sizeof checkbitmap + (blocksize * i)), SEEK_SET);	// seek to rootdir in file
    		fread(&checkdirblock, sizeof checkdirblock, 1, fp);
    		if (strcmp(checkdirblock.name, dirname) == 0) {
    				fclose(fp);
    				SIFS_errno	= SIFS_EINVAL;
					return 1;

    		}
		}
	}


//	UPDATE BITMAP
	//copy bitmap locally and edit
	fseek(fp, sizeof header, SEEK_SET); 	// seek to start of bitmap
	SIFS_BIT bitmap[nblocks];				// create local bitmap
	fread(bitmap, sizeof bitmap, 1, fp);	// read from file bitmap to local bitmap

	//find location of first unused block in bitmap
	int bid = 1;
	while (bitmap[bid] != SIFS_UNUSED) {
		++bid;
	}
	bitmap[bid] = SIFS_DIR;

	//write bitmap back to volume
	fseek(fp, sizeof header, SEEK_SET);		// seek to start of bitmap
	fwrite(bitmap, sizeof bitmap, 1, fp);	// write local bitmap to file bitmap

// 	CREATE NEW DIR BLOCK
	//create dirblock
	SIFS_DIRBLOCK dir_block;			// create local dir_block
	strcpy(dir_block.name, dirname);	// copy the name from parameters
	dir_block.modtime = time(NULL);		// set access time from time()
	dir_block.nentries = 0;				// set number of entries to 0
	//copy dir_block to oneblock 
	char	oneblock[blocksize];					// create an empty block
    memset(oneblock, 0, sizeof oneblock);        	// cleared to all zeroes
    memcpy(oneblock, &dir_block, sizeof dir_block);	// copy dir_block to oneblock
    //seek location and write dir_block
    long offset = sizeof header + sizeof bitmap + 
    			((sizeof oneblock) * bid);			// determine offset for write
    fseek(fp, offset, SEEK_SET);					// seek to offset
    fwrite(oneblock, sizeof oneblock, 1, fp);		// write oneblock to location

//	UPDATE ROOT DIR BLOCK
    //update root dir block
    fseek(fp, (sizeof header + sizeof bitmap), SEEK_SET);	// seek to rootdir in file
    SIFS_DIRBLOCK dirblock;						// create local rootdir_block
    fread(&dirblock, sizeof dirblock, 1, fp);
    dirblock.name[0] = '\0';
    dirblock.modtime = time(NULL);
    int blankentry = 0;
    while (dirblock.entries[blankentry].blockID != 0){
    	++blankentry;
    }
    dirblock.entries[blankentry].blockID = bid;

    int entrycount = 0;
    for (int i = 0; i < SIFS_MAX_ENTRIES; ++i) {
    	if (dirblock.entries[i].blockID != 0) {
    		++entrycount;
    	}
    }
    dirblock.nentries = entrycount;

    fseek(fp, (sizeof header + sizeof bitmap), SEEK_SET);
    fwrite(&dirblock, sizeof dirblock, 1, fp);

//	FINISHED, CLOSE THE VOLUME
	fclose(fp);

//	RETURN INDICATING SUCCESS
    return 0;

}
