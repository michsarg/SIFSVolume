#include "sifs-internal.h"

// make a new directory within an existing volume
int SIFS_mkdir(const char *volumename, const char *dirname)
{

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


	// ENSURE THAT REQUESTED DIRECTORY DOES NOT ALREADY EXIST

		// check bitmap for directory blocks
			// open volume for writing
	    	fopen(volumename, "w");

			// check found directory blocks for directory name
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

	// ATTEMPT TO CREATE THE NEW DIRECTORY - OPEN VOLUME FOR WRITING
		// use bitmap to find first free block (directory block is 1 block)
			int b = 0;
			while (bitmap[b] != SIFS_UNUSED) {
				++b;
				}
				//edit the bitmap to say this is now a dirblock
				bitmap[b] = SIFS_DIR;
				//create the dirblock where?
			    SIFS_DIRBLOCK	dir_block;
			    dir_block.name[0]   = '\0';
			    dir_block.modtime	= time(NULL);
			    dir_block.nentries	= 0;




		// create directory block struct containing:
			// name
			// modification time
			// number of entries (subfiles or directories)
			// block id for each subfile or directory

		// close volume
	

	// DIRECTORY CREATION FAILED
		// return error message


	// RETURN INDICATING SUCCESS


    SIFS_errno	= SIFS_ENOTYET;
    return 1;
}
