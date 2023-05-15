#include <stdio.h>
#include <stdlib.h>
#include "image.h"

int main(int argc, char *argv[])
{
    printf("My first C program!\n");
    if (sizeof(RGBbmpHdr) != 54)
    {
        printf("Size of the standard RGB bitmap header is: %d\n Check compilation options.\n", sizeof(RGBbmpHdr));
        return 1;
    }
    for (int i=0; i < argc; ++i)
        printf("Arg #%d > %s\n", i, argv[i]);
    if (argc < 2)
    {
        printf("Usage:\n \t%s input_filename output_filename\n", argv[0]);
        return 2;
    }
    ImageInfo *ptrImage = readBmp(argv[1]);
    if (ptrImage == NULL)
    {
        printf("Error reading input bitmap.\n");
        return 3;
    }

    // here some image processing has to be implemented

    //sepia filter application
    ptrImage = sepia(ptrImage);

    if (saveBmp(argv[2], ptrImage) != 0)
    {
        printf("Error writing output bitmap.\n");
        freeImage(ptrImage);
        return 4;
    }

    freeImage(ptrImage);
    return 0;
}
