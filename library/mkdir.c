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
	FILE *fp = fopen(volumename, "r"); // opens file reading and writing

	//copy blocksize and nblocks locally
	SIFS_VOLUME_HEADER header;
	fread(&header, sizeof header, 1, fp);
	int blocksize;
	blocksize = header.blocksize;
	int nblocks;
	nblocks = header.nblocks;
		printf("blocksize: %i\n", blocksize);
		printf("nblocks: %i\n", nblocks);

	//copy bitmap locally and edit
	SIFS_BIT bitmap[nblocks];
	fread(bitmap, sizeof bitmap, 1, fp);
	memcpy(bitmap, bitmap, sizeof bitmap);
		printf("bitmap[0] = %c\n", bitmap[0]);
		printf("bitmap[1] = %c\n", bitmap[1]);
		printf("bitmap[2] = %c\n", bitmap[2]);
	//find location of first unused block in bitmap
	int bid = 1;
	while (bitmap[bid] != SIFS_UNUSED) {
		++bid;
	}
	bitmap[bid] = SIFS_DIR;
		printf("bitmap[0] = %c\n", bitmap[0]);
		printf("bitmap[1] = %c\n", bitmap[1]);
		printf("bitmap[2] = %c\n", bitmap[2]);
	//write bitmap back to volume
	fseek(fp, sizeof header, SEEK_SET);
	fwrite(bitmap, sizeof bitmap, 1, fp);

	//create dirblock
	SIFS_DIRBLOCK dir_block;
	strcpy(dir_block.name, dirname);
	dir_block.modtime = time(NULL);
	dir_block.nentries = 0;
	//copy dir_block to oneblock 
	char	oneblock[blocksize];
    memset(oneblock, 0, sizeof oneblock);        // cleared to all zeroes
    memcpy(oneblock, &dir_block, sizeof dir_block);
    //seek location and write dir_block
    long offset = sizeof header + sizeof bitmap + ((sizeof oneblock) * bid);
    fseek(fp, offset, SEEK_SET);
    fwrite(oneblock, sizeof oneblock, 1, fp);

    //update rootdir_block
    SIFS_DIRBLOCK	rootdir_block;
    memset(oneblock, 0, sizeof oneblock);        // cleared to all zeroes
    memcpy(oneblock, &rootdir_block, sizeof rootdir_block);
    rootdir_block.modtime	= time(NULL);
    rootdir_block.nentries += 1; // CHECK THIS
    //find next unused entry
    	printf("rootdir_block.entries[0].blockID = %i\n", rootdir_block.entries[0].blockID);
    	printf("rootdir_block.nentries = %i\n", rootdir_block.nentries);
    int entrycount = 0;
    while (rootdir_block.entries[entrycount].blockID != '\0'){
    	++entrycount;
    }
    rootdir_block.entries[entrycount].blockID = bid;  // CHECK THIS
    rootdir_block.entries[entrycount].fileindex = 0;  // CHECK THIS
    		printf("entcount = %i\n", entrycount);
        	printf("rootdir_block.entries[0].blockID = %i\n", rootdir_block.entries[0].blockID);
        	printf("rootdir_block.entries[1].blockID = %i\n", rootdir_block.entries[1].blockID);
        	printf("rootdir_block.entries[2].blockID = %i\n", rootdir_block.entries[2].blockID);
    fseek(fp, (sizeof header + sizeof bitmap), SEEK_SET);
    fwrite(oneblock, sizeof oneblock, 1, fp);








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



