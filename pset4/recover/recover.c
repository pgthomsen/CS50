#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover recovery file\n");
        return 1;
    }
    
    // open recovery file
    FILE *recovery = fopen(argv[1], "r");
    
    // ensure the file opened properly, else return error
    if (recovery == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }
    
    // initialize the variables to be used to track image number and output
    // file information
    int inum = 0;
    char filename[8];
    FILE *img;
    
    // initialize the buffer
    unsigned char buffer[512];
    
    // iterate through until a block of less than 512 is reached
    // which indicates EOF reached
    while(fread(buffer, 1, 512, recovery) == 512)
    {
        // check if new file is present, if so, close current image
        // and create new file to store new image
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // ensure that the first run through img won't be attempted to be
            // closed as it is not yet open
            if (inum != 0)
            {
                fclose(img);
            }
            sprintf(filename, "%03i.jpg", inum);
            img = fopen(filename, "w");
            inum++;
            fwrite(&buffer, 1, 512, img);
        }
        // if not a new file and not still searching for first file
        // continuing copying content into currently open .jpg file
        else if(inum != 0)
        {
            fwrite(&buffer, 1, 512, img);
        }
    }
    
    // close all open files
    fclose(recovery);
    fclose(img);
    return 0;
}