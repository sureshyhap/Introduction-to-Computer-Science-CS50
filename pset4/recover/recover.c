#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Enter exactly 1 command line argument (other than program name)\n");
        return 1;
    }

    FILE *fread_ptr = fopen(argv[1], "r");
    if (fread_ptr == NULL)
    {
        fprintf(stderr, "Error, could not open file for reading.\n");
        return 2;
    }

    const int BLOCK_SIZE = 512;
    const int NUM_IMAGES = 50;

    // char used here because it holds 1 byte not because what it holds will be interpreted as a character
    char data[BLOCK_SIZE];

    // skip bytes until first possible jpg
    while (fgetc(fread_ptr) != 0xff)
    {
        ;
    }
    for (int i = 0; i < NUM_IMAGES; ++i)
    {
        if (fgetc(fread_ptr) == 0xd8)
        {
            if (fgetc(fread_ptr) == 0xff)
            {
                char byte4 = fgetc(fread_ptr);
                if (byte4 == (char) 0xe0 || byte4 == (char) 0xe1 || byte4 == (char) 0xe2 || byte4 == (char) 0xe3
                    || byte4 == (char) 0xe4 || byte4 == (char) 0xe5 || byte4 == (char) 0xe6 || byte4 == (char) 0xe7
                    || byte4 == (char) 0xe8 || byte4 == (char) 0xe9 || byte4 == (char) 0xea || byte4 == (char) 0xeb
                    || byte4 == (char) 0xec || byte4 == (char) 0xed || byte4 == (char) 0xee || byte4 == (char) 0xef)
                {
                    // Return to the start of the found file
                    fseek(fread_ptr, -4 * sizeof(char), SEEK_CUR);

                    // String for name of file including '\0'
                    char file_number[9];

                    // Formatting the file name depending on the file number
                    if (i < 10)
                    {
                        sprintf(file_number, "00%i.jpg", i);
                    }
                    else
                    {
                        sprintf(file_number, "0%i.jpg", i);
                    }

                    FILE *fwrite_ptr = fopen(file_number, "w");

                    fread(data, BLOCK_SIZE, 1, fread_ptr);
                    fwrite(data, BLOCK_SIZE, 1, fwrite_ptr);

                    // special case for last image to write data until end of reading file
                    if (i == NUM_IMAGES - 1)
                    {
                        while (fgetc(fread_ptr) != EOF)
                        {
                            fseek(fread_ptr, -1 * sizeof(char), SEEK_CUR);
                            fread(data, BLOCK_SIZE, 1, fread_ptr);
                            fwrite(data, BLOCK_SIZE, 1, fwrite_ptr);
                        }
                    }

                    while (1)
                    {
                        char first_bytes[4];

                        // read in bytes for testing
                        fread(first_bytes, sizeof(char), 4, fread_ptr);

                        // testing if bytes are beginning of a new jpg
                        if (first_bytes[0] == (char) 0xff &&
                            first_bytes[1] == (char) 0xd8 &&
                            first_bytes[2] == (char) 0xff &&
                            (first_bytes[3] >= (char) 0xe0 && first_bytes[3] <= (char) 0xef))
                        {
                            // backtrack -4 + 1 to move "forward" one byte from start before the four bytes were read
                            fseek(fread_ptr, -3 * sizeof(char), SEEK_CUR);
                            break;
                        }
                        else
                        {
                            // Go back to where the position was before the read
                            fseek(fread_ptr, -4 * sizeof(char), SEEK_CUR);
                            fread(data, BLOCK_SIZE, 1, fread_ptr);
                            fwrite(data, BLOCK_SIZE, 1, fwrite_ptr);
                        }
                    }
                }
                // These elses keep reading until a potential new jpg is found (a 0xff is read)
                else
                {
                    while (fgetc(fread_ptr) != 0xff)
                    {
                        ;
                    }
                    --i;
                }
            }
            else
            {
                while (fgetc(fread_ptr) != 0xff)
                {
                    ;
                }
                --i;
            }
        }
        else
        {
            while (fgetc(fread_ptr) != 0xff)
            {
                ;
            }
            --i;
        }
    }


    return 0;
}