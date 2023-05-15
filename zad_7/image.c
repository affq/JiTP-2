#include "image.h"
#include <stdio.h>
#include <stdlib.h>

ImageInfo* readBmp(const char* fileName)
{
    FILE* file = fopen(fileName,"rb");
    if(file == NULL)
    {
        return NULL;
    }

    RGBbmpHdr btmhdr;
    if (fread((void*) &btmhdr, sizeof(btmhdr), 1, file) != 1)
    {
        fclose(file);
        return NULL;
    }

    if (btmhdr.bfType != 0x4D42 ||
        btmhdr.biPlanes != 1 ||
        btmhdr.biBitCount != 24 ||
        btmhdr.biCompression != 0)
    {
        printf("Invalid bitmap header.\n");
        fclose(file);
        return NULL;
    }

    ImageInfo* imginfo = (ImageInfo *) malloc(sizeof(ImageInfo));
    if (imginfo == NULL)
    {
        fclose(file);
        return NULL;
    }
    imginfo->pImg = NULL;
    imginfo->height = abs(btmhdr.biHeight);
    imginfo->width = btmhdr.biWidth;
    imginfo->line_bytes = imginfo->width * 3;
    if (imginfo->line_bytes % 4 != 0)
        imginfo->line_bytes += 4 - (imginfo->line_bytes % 4);
    unsigned imgsize = imginfo->height * imginfo->line_bytes;

    if (fseek(file, btmhdr.bfOffBits, SEEK_SET) != 0)
    {
        fclose(file);
        freeImage(imginfo);
        return NULL;
    }
    imginfo->pImg = (unsigned char*) malloc(imgsize);
    if(imginfo->pImg == NULL)
    {
        fclose(file);
        freeImage(imginfo);
        return NULL;
    }
    if (fread(imginfo->pImg, 1, imgsize, file) != imgsize)
    {
        fclose(file);
        freeImage(imginfo);
        return NULL;
    }
    fclose(file);
    return imginfo;
}

int saveBmp(const char* fileName, ImageInfo *imageinfo)
{
    FILE *file = fopen(fileName, "wb");
    if (file == NULL)
    {
        return 1;
    }

    unsigned int lineBytes = (int) imageinfo->line_bytes;
	unsigned int imgSize = lineBytes * imageinfo->height;
    RGBbmpHdr bmpHead =
	{
	0x4D42,				// unsigned short bfType;
	sizeof(RGBbmpHdr)+imgSize,		// unsigned long  bfSize;
	0, 0,				// unsigned short bfReserved1, bfReserved2;
	sizeof(RGBbmpHdr),	// unsigned long  bfOffBits;
	40,					// unsigned long  biSize;
	(int32_t)imageinfo->width,		// long  biWidth;
	(int32_t)imageinfo->height,		// long  biHeight;
	3,					// short biPlanes;
	24,					// short biBitCount;
	0,					// unsigned long  biCompression;
	(uint32_t) imgSize, // unsigned long  biSizeImage;
	11811,				// long biXPelsPerMeter; = 300 dpi
	11811,				// long biYPelsPerMeter;
	0,					// unsigned long  biClrUsed;
	0,					// unsigned long  biClrImportant;
	};

	if (fwrite(&bmpHead, sizeof(RGBbmpHdr), 1, file) != 1)
    {
        fclose(file);
        return 2;
    }
    if (fwrite(imageinfo->pImg, 1, bmpHead.biSizeImage, file) != bmpHead.biSizeImage)
    {
        fclose(file);
        return 3;
    }
    fclose(file);
	return 0;
}

void freeImage(ImageInfo *imageinfo)
{
    if (imageinfo != NULL)
    {
        if(imageinfo->pImg != NULL)
            free(imageinfo->pImg);
        free(imageinfo);
    }
}

unsigned int min(unsigned int a, unsigned int b)
{
    return a < b ? a : b;
};

//sepia filter
ImageInfo* sepia(ImageInfo *image)
{
    unsigned image_size = image->height * image->line_bytes;

    for (unsigned int i = 0; i < image_size; i+=3)
    {
        unsigned char blue = image->pImg[i];
        unsigned char green = image->pImg[i+1];
        unsigned char red = image->pImg[i+2];

        unsigned int new_blue = min(255, (red * 0.272 + green * 0.534 + blue * 0.131 ));
        unsigned int new_green = min(255, (red * 0.349 + green * 0.686 + blue * 0.168 ));
        unsigned int new_red = min(255, (red * 0.393 + green * 0.769 + blue * 0.189));

        image->pImg[i] = new_blue;
        image->pImg[i+1] = new_green;
        image->pImg[i+2] = new_red;
    }
    return image;
}


