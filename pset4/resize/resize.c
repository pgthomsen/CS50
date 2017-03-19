/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"
#define HEADEROFFSET 54

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy scalar(1-100) infile outfile\n");
        return 1;
    }
    
    // check size of scalar
    int *scalar = malloc(sizeof(int));
    *scalar = atoi(argv[1]);
    
    if (*scalar < 1 || *scalar > 100)
    {
        fprintf(stderr, "Usage: scalar must be in the range 1 to 100 inclusive of 1 and 100\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 1;
    }
    
    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    BITMAPINFOHEADER smallbi = bi;
    
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 1;
    }
    
    // Set new biWidth and biHeight to be included in the new files headers
    bi.biWidth = bi.biWidth * *scalar;
    bi.biHeight = bi.biHeight * *scalar;
    
    // determine padding for the new image
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // determine the new biSizeImage and bfSize for the headers of the new file
    bi.biSizeImage = ((bi.biWidth * sizeof(RGBTRIPLE))+ padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + HEADEROFFSET;
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    long int *location = malloc(sizeof(long int));
    
    // determine the padding for the original image
    int smallpadding =  (4 - (smallbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(smallbi.biHeight); i < biHeight; i++)
    {
        // get initial location within file for this section of copies of pixels
        *location = ftell(inptr);
        for (int m = 0; m < *scalar; m++)
        {
            // take location in input file that is being used for the scalar coppies of this line and go back to that
            // in order to make a new copy of this line of pixels
            fseek(inptr, *location, SEEK_SET);
            // iterate over pixels in scanline
            for (int j = 0; j < smallbi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                
                
                // write RGB triple to outfile making scalar versions of RGBTRIPLE following the first
                for(int p = 0; p < *scalar; p++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            
            // skip over padding, if any
            fseek(inptr, smallpadding, SEEK_CUR);
            
            // add padding to output file
            for (int k = 0; k < padding; k++)
                    {
                        fputc(0x00, outptr);
                    }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);
    
    // free malloc() reserved space
    free(scalar);
    free(location);

    // success
    return 0;
    
    
}
