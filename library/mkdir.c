#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sifs-internal.h"


void test (int number) {
	printf("test: %i\n", number);
}


/*
SIFS_VOLUME_HEADER read_header (const char *volumename) {
	FILE *fp = fopen(volumename, "r");
	fseek(fp, 0, SEEK_SET);
	SIFS_VOLUME_HEADER header;
	fread(&header, sizeof header, 1, fp);
	fclose(fp);
	return header;
}


void write_header (const char *volumename, SIFS_VOLUME_HEADER header) {
	FILE *fp = fopen(volumename, "r+");
	fseek(fp, 0, SEEK_SET);
	fwrite(&header, sizeof header, 1, fp);
	fclose(fp);
}


SIFS_BIT * read_bitmap (const char *volumename) {
	FILE *fp = fopen(volumename, "r");
	SIFS_VOLUME_HEADER header = read_header(volumename);
	int nblocks = header.nblocks;
	fseek(fp, sizeof header, SEEK_SET);
	SIFS_BIT bitmap[nblocks];
	fread(bitmap, sizeof bitmap, 1, fp);
	fclose(fp);
	return &bitmap;
}


void write_bitmap (const char *volumename, *bitmap) {
	FILE *fp = fopen(volumename, "r+");
	SIFS_VOLUME_HEADER header = read_header(volumename);
	fseek(fp, sizeof header, SEEK_SET);
	fwrite(bitmap, sizeof bitmap, 1, fp);
	fclose(fp);
}


SIFS_DIRBLOCK read_dirblock (const char *volumename, int blockID) {
	FILE *fp = fopen(volumename, "r");
	SIFS_VOLUME_HEADER header = read_header(volumename);
	int nblocks = header.nblocks;
	int blocksize = header.blocksize;
	bitmap[SIFS_BIT] = read_bitmap(volumename)
	char	oneblock[blocksize];
	SIFS_DIRBLOCK dirblock;
	fseek(fp, (sizeof header + sizeof bitmap + (sizeof(oneblock) * blockID)), SEEK_SET);
	fread(&dirblock, sizeof dirblock, 1, fp);
	fclose(fp);
	return dirblock;
}


void write_dirblock (const char *volumename, SIFS_DIRBLOCK dirblock, int blockID) {
	FILE *fp = fopen(volumename, "r+");
	SIFS_VOLUME_HEADER header = read_header(volumename);
	int nblocks = header.nblocks;
	int blocksize = header.blocksize;
	bitmap[SIFS_BIT] = read_bitmap(volumename)
	char	oneblock[blocksize];
	fseek(fp, (sizeof header + sizeof bitmap + (sizeof(oneblock) * blockID)), SEEK_SET);
	fwrite(&dirblock, sizeof dirblock, 1, fp)
	fclose(fp);
}
*/	 


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
	SIFS_DIRBLOCK parent_dirblock;
	fread(&parent_dirblock, sizeof parent_dirblock, 1, fp);
test(3);
	for (int i = 0; i < parent_dirblock.nentries; ++i) {
test(4);
//printf("blockID = %i\n", blockID);
printf("bitmap[parent_dirblock.entries[%i].blockID] = %c\n", i, bitmap[parent_dirblock.entries[i].blockID]);

		if (bitmap[parent_dirblock.entries[i].blockID] == SIFS_DIR) {
test(5);
			SIFS_DIRBLOCK child_dirblock;
			fseek(fp, (sizeof header + sizeof bitmap + 
				(sizeof(oneblock) * parent_dirblock.entries[i].blockID)), SEEK_SET); // check I here
    		memset(&child_dirblock, 0, sizeof child_dirblock);
    		fread(&child_dirblock, sizeof(child_dirblock), 1, fp);
test(6);
printf("child_dirblock.name = %s\n", child_dirblock.name);
    		if (strcmp(child_dirblock.name, dirname) == 0 ) {
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

    int dirtotal = 0;
    while(token != NULL) {
	token = strtok(NULL, s);
	++dirtotal;
    }

	printf("dirtotal: %i\n", dirtotal);

	int dircount = 0;
    while(token != NULL) {
    	printf( " %s\n", token );
    	token = strtok(NULL, s);

    	//obtain block ID of parent
    	int parent_blockID;
    	if (dircount == 0) {
    		parent_blockID = 0;
    	} 
    	else {
    		///////

    		;

    	}

    	//if (check_dir_exists(volumename, parent_blockID, token) == 0)



    	/*
    	if ((check_dir_exists(currentdirID, token) == 1) {
    		;
    	} 
		 */
    	++dircount;
    }

printf("does dir exist: %i\n", check_dir_exists(volumename, 0, dirname) );






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

// 	CREATE NEW DIR BLOCK (child dir)
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

//	UPDATE ROOT DIR BLOCK (parent dir)
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
