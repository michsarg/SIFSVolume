#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sifs-internal.h"


void test (int number) {
	printf("test: %i\n", number);
}


int check_dir_exists(const char *volumename, const int dirblockID, const char *dirname) {

	// open volume for reading
	FILE *fp = fopen(volumename, "r");
test(0);
	// check header
	fseek(fp, 0, SEEK_SET);
	SIFS_VOLUME_HEADER header;
	fread(&header, sizeof header, 1, fp);
	int blocksize = header.blocksize;
	int nblocks = header.nblocks;
test(1);
	//create local bitmap, oneblock
	fseek(fp, sizeof header, SEEK_SET);
	SIFS_BIT bitmap[nblocks];
	fread(bitmap, sizeof bitmap, 1, fp);



	char	oneblock[blocksize];
	memset(oneblock, 0, sizeof oneblock); 
test(2);
	// retrieve dirblock by parameter dirblockID
	fseek(fp, (sizeof header + sizeof bitmap + (sizeof(oneblock) * dirblockID)), SEEK_SET);
	SIFS_DIRBLOCK sup_dirblock;
	fread(&sup_dirblock, sizeof sup_dirblock, 1, fp);
test(3);
	for (int i = 0; i < sup_dirblock.nentries; ++i) {
test(4);
//printf("blockID = %i\n", blockID);
printf("bitmap[sup_dirblock.entries[%i].blockID] = %c\n", i, bitmap[sup_dirblock.entries[i].blockID]);

		if (bitmap[sup_dirblock.entries[i].blockID] == SIFS_DIR) {
test(5);
			SIFS_DIRBLOCK sub_dirblock;
			fseek(fp, (sizeof header + sizeof bitmap + 
				(sizeof(oneblock) * sup_dirblock.entries[i].blockID)), SEEK_SET); // check I here
    		memset(&sub_dirblock, 0, sizeof sub_dirblock);
    		fread(&sub_dirblock, sizeof(sub_dirblock), 1, fp);
test(6);
printf("sub_dirblock.name = %s\n", sub_dirblock.name);
    		if (strcmp(sub_dirblock.name, dirname) == 0 ) {
				//dir found
				fclose(fp);
				return 0;
test(7);				
			}
		}

	}
	//dir not found
	fclose(fp);
	return 1;





	//// retard
	/*
	test(3);
	// search for directories in dirblock.entries
	fseek(fp, sizeof header, SEEK_SET);
	fread(bitmap, sizeof bitmap, 1, fp);
	test(4);
	SIFS_DIRBLOCK	sub_dirblock;	
	// check bitmap for directories
	for (int i = 0; i < nblocks; ++i) {
		if (bitmap[i] == SIFS_DIR && i != dirblockID) {	

			fseek(fp, (sizeof header + sizeof bitmap + (sizeof(oneblock) * i)), SEEK_SET);
    		memset(&sub_dirblock, 0, sizeof sub_dirblock);
    		fread(&sub_dirblock, sizeof(sub_dirblock), 1, fp);	//copy that dirblock

    		//check if the file index in entries of sup block matches dirblockID
    		//if (sup_dirblock.entries[].fileindex == dirblockID) // relation index array to 

	test(5);

	printf("sub_dirblock.name = %s\n", sub_dirblock.name);
	printf("sub_dirblock.nentries = %i\n", sub_dirblock.nentries); // should be not zero
	printf("sub_dirblock.entries[0].blockID = %u\n", sub_dirblock.entries[0].blockID);
	printf("sub_dirblockID = %i\n", dirblockID);

		// still need to check for rootfile match
		if (strcmp(sub_dirblock.name, dirname) == 0 ) {
			//dirfound
			fclose(fp);
			return 0;
		}

	
			// check through the fileindex for match to dirblock ID
    		for (int i = 0; i < dirblock.nentries; ++i) {	//somethings wrong
    			if((int)dirblock.entries[i].fileindex == dirblockID) {
				test(6);
	    			// check if filename matches
		    		if (strcmp(dirblock.name, dirname) == 0) {
					test(7);		    			
		    			// dir found
	    				fclose(fp);
						return 0;

	    			}
    			}
    		}

		
	//}
	// dir not found
	//fclose(fp);
	//return 1;



	// retreive dirblocks of directories
	// extract dirblock names
	// compare dirblock names to proposed dirname
	// return 0 if success, 1 if fail


	//	FINISHED, CLOSE THE VOLUME

	*/

}


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
	FILE *fp = fopen(volumename, "r+");

// 	CHECK HEADER
	fseek(fp, 0, SEEK_SET);
	SIFS_VOLUME_HEADER header;
	fread(&header, sizeof header, 1, fp);
	int blocksize = header.blocksize;
	int nblocks = header.nblocks;

//	DETECT DIRS WITHIN THE INPUT & CHECK EXISTENCE
	char str[SIFS_MAX_NAME_LENGTH];
    strcpy(str, dirname);
    const char s[2] = "/";
    char *token;

    //currentdirID = 0;
    token = strtok(str, s);

    while(token != NULL) {
    	printf( " %s\n", token );
    	token = strtok(NULL, s);

    	/*
    	if ((check_dir_exists(currentdirID, token) == 1) {
    		;
    	} 
		 */  
    }

printf("does dir exist: %i\n", check_dir_exists(volumename, 0, dirname) );


    	/*
	int inputlength = strlen(dirname);
	int dircount = 1;
	for (int i = 0; i < inputlength; ++i) {
		if (dirname[i] == '/') {
			++dircount;
		}
	}
	int dirlevelcount = 0;
	int dircharcount = 0;
	char dirlist[dircount][SIFS_MAX_NAME_LENGTH];
	for (int i = 0; i < inputlength; ++i) {
		// if char
		if (dirname[i] != '/') {
			dirlist[dirlevelcount][dircharcount] = dirname[i];
			++dircharcount;
		}
		// if '/'
		else if (dirname[i] == '/') {
			dirlist[dirlevelcount][dircharcount] = '\0';
			++dirlevelcount;
			dircharcount = 0;
		}
	}
	dirlist[dirlevelcount][dircharcount] = '\0';
	*/



//	ENSURE THAT THE REQUESTED DIR DOES NOT ALREADY EXIST
	fseek(fp, sizeof header, SEEK_SET);
	SIFS_BIT checkbitmap[nblocks];
	fread(checkbitmap, sizeof checkbitmap, 1, fp);
	SIFS_DIRBLOCK checkdirblock;
	for (int i = 0; i < nblocks; ++i) {
		if (checkbitmap[i] == SIFS_DIR) {
			fseek(fp, (sizeof header + sizeof checkbitmap + (blocksize * i)), SEEK_SET);	// seek to rootdir in file
    		fread(&checkdirblock, sizeof checkdirblock, 1, fp);
    		if (strcmp(checkdirblock.name, dirname) == 0) {
    				fclose(fp);
    				SIFS_errno	= SIFS_EEXIST;
					return 1;
    		}
		}
	}

//	UPDATE BITMAP
	fseek(fp, sizeof header, SEEK_SET);
	SIFS_BIT bitmap[nblocks];
	fread(bitmap, sizeof bitmap, 1, fp);

	int bid = 1;
	while (bitmap[bid] != SIFS_UNUSED) {
		++bid;
	}
	bitmap[bid] = SIFS_DIR;

	fseek(fp, sizeof header, SEEK_SET);
	fwrite(bitmap, sizeof bitmap, 1, fp);

// 	CREATE NEW DIR BLOCK
	SIFS_DIRBLOCK dir_block;
	strcpy(dir_block.name, dirname);
	dir_block.modtime = time(NULL);
	dir_block.nentries = 0;

	char	oneblock[blocksize];
    memset(oneblock, 0, sizeof oneblock);
    memcpy(oneblock, &dir_block, sizeof dir_block);	

    long offset = sizeof header + sizeof bitmap + 
    			((sizeof oneblock) * bid);			
    fseek(fp, offset, SEEK_SET);					
    fwrite(oneblock, sizeof oneblock, 1, fp);		

//	UPDATE ROOT DIR BLOCK
    fseek(fp, (sizeof header + sizeof bitmap), SEEK_SET);	
    SIFS_DIRBLOCK dirblock;						
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
    //printf("entrycount = %i\n", entrycount);
    //printf("dirblock.nentries = %i\n", dirblock.nentries);

    fseek(fp, (sizeof header + sizeof bitmap), SEEK_SET);
    fwrite(&dirblock, sizeof dirblock, 1, fp);

//	FINISHED, CLOSE THE VOLUME
	fclose(fp);

//	RETURN INDICATING SUCCESS
    return 0;

}
