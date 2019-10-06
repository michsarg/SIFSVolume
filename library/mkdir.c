#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sifs-internal.h"


void test (int number) {
	printf("test %i\n", number);
}


int SIFS_mkdir(const char *volumename, const char *dirname)
{

	// open volume for writing
	FILE *fp = fopen(volumename, "r+"); // opens file reading and writing

	// HEADER
	//copy blocksize and nblocks locally
	fseek(fp, 0, SEEK_SET); 				// seek to start of file
	SIFS_VOLUME_HEADER header;				// create local header
	fread(&header, sizeof header, 1, fp);	// read from file header to local header
	int blocksize = header.blocksize;		// store blocksize in local variable
	int nblocks = header.nblocks;			// store nblocks in local variable
		printf("blocksize: %i\n", blocksize);
		printf("nblocks: %i\n", nblocks);

	// BITMAP
	//copy bitmap locally and edit
	fseek(fp, sizeof header, SEEK_SET); 	// seek to start of bitmap
	SIFS_BIT bitmap[nblocks];				// create local bitmap
	fread(bitmap, sizeof bitmap, 1, fp);	// read from file bitmap to local bitmap
		printf("bitmap[0] = %c\n", bitmap[0]);	// root dir
		printf("bitmap[1] = %c\n", bitmap[1]);	// unused
		printf("bitmap[2] = %c\n", bitmap[2]);	// unused
	//find location of first unused block in bitmap
	int bid = 1;
	while (bitmap[bid] != SIFS_UNUSED) {
		++bid;
	}
	bitmap[bid] = SIFS_DIR;
		printf("bid = %i\n", bid);
		printf("bitmap[0] = %c\n", bitmap[0]);	// root dir
		printf("bitmap[1] = %c\n", bitmap[1]);	// dir1
		printf("bitmap[2] = %c\n", bitmap[2]);	// unused
	//write bitmap back to volume
	fseek(fp, sizeof header, SEEK_SET);		// seek to start of bitmap
	fwrite(bitmap, sizeof bitmap, 1, fp);	// write local bitmap to file bitmap

	// NEW DIR BLOCK
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

    // ROOT DIR BLOCK - - - - help!
    //update rootdir_block
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

    //print test
    printf("dirblock.name = %s\n", dirblock.name);
    printf("dirblock.modtime = %li\n", dirblock.modtime);
    printf("dirblock.nentries = %i\n", dirblock.nentries);
    for (int i = 0; i <10; ++i) {
    	printf("dirblock.entries[%i].blockID = %i\n", i, dirblock.entries[i].blockID);
    	printf("dirblock.entries[%i].fileindex = %u\n", i, dirblock.entries[i].fileindex);
    }

    fseek(fp, (sizeof header + sizeof bitmap), SEEK_SET);
    fwrite(&dirblock, sizeof dirblock, 1, fp);




    /*
    fread(rootdir_block, sizeof rootdir_block, 1, fp);	// read from file rootdir to local rootdir
    	printf("rootdir_block.modtime = %i\n", (int)rootdir_block.modtime);
    	printf("rootdir_block.nentries = %i\n", (int)rootdir_block.nentries);
    
    rootdir_block.modtime	= time(NULL);	// set rootdir_block time
    rootdir_block.nentries = 1;				// increment rootdir block entries

    	printf("rootdir_block.modtime = %i\n", (int)rootdir_block.modtime);
    	printf("rootdir_block.nentries = %i\n", (int)rootdir_block.nentries);

    //find next unused entry
    	printf("rootdir_block.entries[0].blockID = %i\n", 
    			rootdir_block.entries[0].blockID);
    	printf("rootdir_block.nentries = %i\n", 
    			rootdir_block.nentries);
    int entrycount = 0;
    while (rootdir_block.entries[entrycount].blockID != '\0'){
    	++entrycount;
    }
    rootdir_block.entries[entrycount].blockID = bid;  // CHECK THIS
    rootdir_block.entries[entrycount].fileindex = 0;  // CHECK THIS
    		printf("entcount = %i\n", entrycount);
        	printf("rootdir_block.entries[0].blockID = %i\n", 
        			rootdir_block.entries[0].blockID);
        	printf("rootdir_block.entries[1].blockID = %i\n", 
        			rootdir_block.entries[1].blockID);
        	printf("rootdir_block.entries[2].blockID = %i\n", 
        			rootdir_block.entries[2].blockID);
    fseek(fp, (sizeof header + sizeof bitmap), SEEK_SET);
    fwrite(oneblock, sizeof oneblock, 1, fp);
	*/







	/*
	//blanked from open to close
	//read from header and bitmap
	SIFS_VOLUME_HEADER  header;
    fread(&header, sizeof header, 1, fp);
    printf("blocksize=%i,  nblocks=%i\n",  
    	(int)header.blocksize, (int)header.nblocks);

	// need to set vol to the bitmap location
	SIFS_BIT	bitmap[header.nblocks];
	fread(bitmap, sizeof bitmap, 1, fp);

	//search the bitmap for unused
	uint32_t bid = 0;
	while (bitmap[bid] != SIFS_UNUSED) {
		++bid;
		}

	//update the volume header
	fwrite(bitmap, sizeof bitmap, 1, fp);

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
    long offset = ((sizeof header + sizeof bitmap) + ((sizeof oneblock) * bid));
    fseek(fp, offset, SEEK_SET);
    
    // WRITE THE DIR BLOCK TO THE LOCATION
	fwrite(&dir_block, sizeof dir_block, 1, fp);	// write dir
	*/
	// FINISHED, CLOSE THE VOLUME
	fclose(fp);

	// RETURN INDICATING SUCCESS
    return 0;



}



