// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize factor infile outfile\n");
        return 1;
    }

    // String to float
    float factor = atof(argv[1]);
    if (factor <= 0 || factor > 100)
    {
        fprintf(stderr, "Factor should be greater than 0 and less than or equal to 100.0\n");
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
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0) // Check bi.biCompression
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    LONG original_width = bi.biWidth;
    LONG original_height = bi.biHeight;

    // determine padding for scanlines for input file
    int padding_in = (4 - (original_width * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biWidth *= factor;

    // determine padding for scanlines for output file
    int padding_out = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biHeight *= factor;

    // size of image is height times (row of pixels times size of pixel + padding)
    bi.biSizeImage = (abs(bi.biHeight) * (bi.biWidth * sizeof(RGBTRIPLE) + padding_out));
    // total size includes headers and size of image
    bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bi.biSizeImage;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // n will control loop differently based on whether the image is to be magnified or shrunk
    int n;
    if (factor >= 1.0)
    {
        n = abs(bi.biHeight);
    }
    else
    {
        n = abs(original_height);
    }
    // Finding the reciprocal of the factor change will be useful later
    float reciprocal = 1.0 / factor;
    // iterate over infile's scanlines
    for (int i = 0, c = 0; i < n; i++) ////////////////////////
    {

        if (factor < 1.0)
        {
            if (i % (int)(reciprocal) != 0)
            {
                // Skipping rows according to the value of reciprocal and i (that's what it means to shrink vertically)
                fseek(inptr, (padding_in + (original_width * sizeof(RGBTRIPLE))), SEEK_CUR);
                continue;
            }
        }
        // iterate over pixels in scanline
        for (int j = 0; j < original_width; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            if (factor < 1.0)
            {
                // Skipping pixels based on reciprocal (skip 1 for rec == 2, 2 for rec == 3, ...)
                fseek(inptr, (reciprocal - 1) * sizeof(RGBTRIPLE), SEEK_CUR);
                for (int z = 0; z < reciprocal - 1; ++z)
                {
                    ++j;
                }
            }
            if (factor >= 1.0)
            {
                for (int k = 0; k < factor; ++k)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }
            else
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        }

        // skip over padding, if any
        fseek(inptr, padding_in, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding_out; k++)
        {
            fputc(0x00, outptr);
        }

        if (factor > 1.0)
        {
            if (c < factor - 1)
            {
                // Reusing scanlines in the input file a certain number of times when expanding
                fseek(inptr, -1 * (padding_in + (original_width * sizeof(RGBTRIPLE))), SEEK_CUR);
                ++c;
            }
            else
            {
                c = 0;
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
