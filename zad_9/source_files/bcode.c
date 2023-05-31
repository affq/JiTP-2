#include "bcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


ImageInfo* readBmp(const char* fileName)
{
    FILE* file = fopen(fileName,"rb");
    if(file == NULL)
    {
        return NULL;
    }

    bmpHdr btmhdr;
    if (fread((void*) &btmhdr, sizeof(btmhdr), 1, file) != 1)
    {
        fclose(file);
        return NULL;
    }

    if (btmhdr.bfType != 0x4D42 ||
        btmhdr.biPlanes != 1 ||
        (btmhdr.biBitCount != 1 &&
        btmhdr.biBitCount != 4) ||
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
    imginfo->bitsperpel = btmhdr.biBitCount;

    unsigned pixels_per_byte = 8 / imginfo->bitsperpel;
    imginfo->line_bytes = (imginfo->width + pixels_per_byte - 1) / pixels_per_byte;
    if (imginfo->line_bytes % 4 != 0)
        imginfo->line_bytes += 4 - (imginfo->line_bytes % 4);

    // reading lookup table
    unsigned lut_entries = 1 << imginfo->bitsperpel;
    if (fread((void*) &(imginfo->LUT), 4, lut_entries, file) != lut_entries)
    {
        fclose(file);
        freeImage(imginfo);
        return NULL;
    }

    if (fseek(file, btmhdr.bfOffBits, SEEK_SET) != 0)
    {
        fclose(file);
        freeImage(imginfo);
        return NULL;
    }

    imginfo->pImg = (unsigned char*) malloc(btmhdr.biSizeImage);
    if(imginfo->pImg == 0)
    {
        fclose(file);
        freeImage(imginfo);
        return NULL;
    }
    if (fread(imginfo->pImg, 1, btmhdr.biSizeImage, file) != btmhdr.biSizeImage)
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

    unsigned int lineBytes = imageinfo->line_bytes;
	unsigned int imgSize = lineBytes * imageinfo->height;

    unsigned int lut_entries = 1 << imageinfo->bitsperpel;
    unsigned int headerSize = sizeof(bmpHdr) + 4 * lut_entries;

    bmpHdr bmpHead =
	{
	0x4D42,				// unsigned short bfType;
	headerSize+imgSize,		// unsigned long  bfSize;
	0, 0,				// unsigned short bfReserved1, bfReserved2;
	headerSize,	        // unsigned long  bfOffBits;
	40,					// unsigned long  biSize;
	(int32_t)imageinfo->width,		// long  biWidth;
	(int32_t)imageinfo->height,		// long  biHeight;
	1,					// short biPlanes;
	imageinfo->bitsperpel,		// short biBitCount;
	0,					// unsigned long  biCompression;
	(uint32_t) imgSize, // unsigned long  biSizeImage;
	11811,				// long biXPelsPerMeter; = 300 dpi
	11811,				// long biYPelsPerMeter;
	lut_entries,		// unsigned long  biClrUsed;
	lut_entries,		// unsigned long  biClrImportant;
	};

	if (fwrite(&bmpHead, sizeof(bmpHdr), 1, file) != 1)
    {
        fclose(file);
        return 2;
    }
    if (fwrite(&imageinfo->LUT, 4, lut_entries, file) != lut_entries)
    {
        fclose(file);
        return 3;
    }
    if (fwrite(imageinfo->pImg, 1, bmpHead.biSizeImage, file) != bmpHead.biSizeImage)
    {
        fclose(file);
        return 4;
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

static unsigned char bw_lut[] = {0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0};
static unsigned char sixteen_lut[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00,
    0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00,
    0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00,
    0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00,
    0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00,
    0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00,
    };

ImageInfo* createImage(unsigned int width, unsigned int height, unsigned int bpp)
{
    if (width == 0 || height == 0 || (bpp != 1 && bpp != 4))
        return NULL;
    ImageInfo *pImg = malloc(sizeof(ImageInfo));
    if (pImg == NULL)
        return NULL;

    pImg->width = width;
    pImg->height = height;
    pImg->bitsperpel = bpp;
    unsigned pixels_per_byte = 8 / pImg->bitsperpel;
    pImg->line_bytes = (pImg->width + pixels_per_byte - 1) / pixels_per_byte;
    if (pImg->line_bytes % 4 != 0)
        pImg->line_bytes += 4 - (pImg->line_bytes % 4);

    // LUT filling
    if (pImg->bitsperpel == 1)
        memcpy(pImg->LUT, bw_lut, sizeof(bw_lut));
    else
        memcpy(pImg->LUT, sixteen_lut, sizeof(sixteen_lut));

    pImg->pImg = malloc(pImg->line_bytes * pImg->height);
    if (pImg->pImg == NULL)
    {
        free(pImg);
        return NULL;
    }
    return pImg;
}

void showLookupTable(ImageInfo *imageinfo)
{
    unsigned int lut_entries = 1 << imageinfo->bitsperpel;
    for (unsigned int idx = 0; idx < lut_entries; ++idx)
    {
        unsigned char *plut = imageinfo->LUT + 4 * idx;
        printf("%2d:  %02X %02X %02X %02X\n", idx, plut[0], plut[1], plut[2], plut[3]);
    }
}

//new functions
// Przedostatnie zadanie, z którym przyjdzie się Państwu zmierzyć w 
// tym semestrze, to wygenerowanie bitmapy zawierającej kod 
// paskowy. Ponieważ kod paskowy składa się z czarnych pasków na 
// białym tle bitmapa będzie minimalna: do kodowania każdego piksela 
// będzie użyty jeden bit (to ma dość istotne konsekwencje dla operacji 
// wykonywanych na pikselach). 
// Żeby odejść nieco od utartych schematów Państwa zadaniem 
// będzie wygenerowanie kodu RM4SCC. Dobrym źródłem informacji 
// o tym rodzaju kodu (między innymi) jest Wikipedia: 
// https://en.wikipedia.org/wiki/RM4SCC. Czeka Państwa trochę 
// czytania i myślenia, jak reprezentować w programie graficzną 
// postać cyfr i liter, jak policzyć sumę kontrolną i, wreszcie, jak 
// zapisać obraz kodu paskowego w bitmapie. 
 
// JTP2 lato 2022/2023 Ćwiczenia 13 2/4 
// Operacje na obrazie 
// Szczęśliwie mamy dostępny program, napisany w starym dobrym C, 
// w którym jest zaimplementowane czytanie i zapis plików obrazów w 
// formacie BMP czarno-białych (1 bpp – 1 bit na piksel) i z 16-
// elementową paletą kolorów (4 bpp – 4 bity na piksel). Mamy też 
// przykład inwersji jednego piksela takiej bitmapy.  
// Uwaga: kod jest zwodniczo podobny do używanego w zadaniu z 
// filtrem sepii, ale to są innego rodzaju bitmapy! 
// Kod można oczywiście wykorzystać w programie, ale proszę 
// zwrócić uwagę na efektywność rozwiązania. Można składać kody 
// paskowe z pojedynczych pikseli, ale doceniane będą rozwiązania, w 
// których jest to rozwiązane lepiej, co w tym przypadku oznacza: 
// bardziej efektywnie. 
  
// JTP2 lato 2022/2023 Ćwiczenia 13 3/4 
// Brakujące funkcje 
// W zasadzie, to „z zewnątrz” powinna być widoczna tylko jedna 
// funkcja:  
// imgInfo* rm4scc_gen(size_t width, size_t height,  
// const char* text); 
// Przy takiej definicji jest dość trudno przekazać na zewnątrz 
// informacje o błędach (a nie ma wyjątków w C) – przyjmijmy, że te 
// informacje są przez rm4scc_gen wyświetlane na konsoli. Ustalenie, 
// jakie błędne sytuacje mogą się pojawić pozostawiam Państwu. 
// Sprawdzenie poprawności kodu paskowego (bo będą niewątpliwie 
// przypadki pomyślne) polega na przekazaniu wygenerowanego pliku 
// do czytnika kodów on-line (np. https://online-barcode-
// reader.inliteresearch.com/ i sprawdzeniu, czy wynik zgadza się z 
// sekwencją cyfr, dla których kod paskowy został wygenerowany 
// (trzeba wybrać typu kodu: Postal).

// new functions
//set_pixel_black function
void set_pixel_black(ImageInfo *imageinfo, unsigned int x, unsigned int y)
{
    unsigned int pixels_per_byte = 8 / imageinfo->bitsperpel;
    unsigned int byte_idx = x / pixels_per_byte;
    unsigned int bit_idx = x % pixels_per_byte;
    unsigned char *pbyte = imageinfo->pImg + y * imageinfo->line_bytes + byte_idx;
    *pbyte |= (1 << (pixels_per_byte - bit_idx - 1));
}

void make_rectangle(ImageInfo *imageinfo, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
            set_pixel_black(imageinfo, x + i, y + j);
}

enum BarType* char_to_bar(char c, enum BarType* bar)
{
    switch(c)
    {
        case 'A':
            bar[0] = tracker;
            bar[1] = ascender;
            bar[2] = descender;
            bar[3] = tracker;
            break;
        
        case 'B':
            bar[0] = tracker;
            bar[1] = ascender;
            bar[2] = full;
            bar[3] = tracker;
            break;
        
        case 'C':
            bar[0] = tracker;
            bar[1] = descender;
            bar[2] = ascender;
            bar[3] = tracker;
            break;
        
        case 'D':
            bar[0] = tracker;
            bar[1] = descender;
            bar[2] = full;
            bar[3] = tracker;
            break;
        
        case 'E':
            bar[0] = ascender;
            bar[1] = tracker;
            bar[2] = descender;
            bar[3] = tracker;
            break;
    }
    return bar;
}

void make_bar(ImageInfo *imageinfo, unsigned int x, unsigned int y, enum BarType* bar)
{
    for (unsigned int i = 0; i < 4; ++i)
    {
        switch(bar[i])
        {
            case tracker:
                make_rectangle(imageinfo, x, y, 1, 1);
                break;
            
            case ascender:
                make_rectangle(imageinfo, x, y, 1, 3);
                break;
            
            case descender:
                make_rectangle(imageinfo, x, y + 2, 1, 3);
                break;
            
            case full:
                make_rectangle(imageinfo, x, y, 1, 5);
                break;
        }
        x++;
    }
}

//draw start bar which is an ascender
void draw_start_bar(ImageInfo *imageinfo, unsigned int x, unsigned int y)
{
    make_rectangle(imageinfo, x, y, 1, 3);
}

//draw stop bar which is a full
void draw_stop_bar(ImageInfo *imageinfo, unsigned int x, unsigned int y)
{
    make_rectangle(imageinfo, x, y, 1, 5);
}

ImageInfo* rm4scc_gen(size_t width, size_t height, const char* text)
{
    ImageInfo* imageinfo = createImage(width, height, 1);
    if (imageinfo == NULL)
        return NULL;
    
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int i = 0;
    enum BarType bar[4];
    while(text[i] != '\0')
    {
        char_to_bar(text[i], bar);
        make_bar(imageinfo, x, y, bar);
        x += 5;
        i++;
    }
    return imageinfo;
}



