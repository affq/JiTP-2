#include "bcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>



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
    ImageInfo *pImg = (ImageInfo*)malloc(sizeof(ImageInfo));
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

    pImg->pImg = (unsigned char*) malloc(pImg->line_bytes * pImg->height);
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
// 0 to 9, then A to Z
const char* codes[] = {
    "00110011", "01010011", "01100011", "10010011", "10100011", "11000011",
    "00110101", "01010101", "01100101", "10010101", "10100101", "11000101",
    "00110110", "01010110", "01100110", "10010110", "10100110", "11000110",
    "00111001", "01011001", "01101001", "10011001", "10101001", "11001001",
    "00111010", "01011010", "01101010", "10011010", "10101010", "11001010",
    "00111100", "01011100", "01101100", "10011100", "10101100", "11001100"
};

void getCodes(const char* str, char* codesArray) {
    int length = strlen(str);
    for (int i = 0; i < length; i++) {
        char c = str[i];
        if (c >= '0' && c <= '9') {
            strcpy(&codesArray[i * 8], codes[c - '0']);
        } else if (c >= 'A' && c <= 'Z') {
            strcpy(&codesArray[i * 8], codes[c - 'A' + 10]);
        } else {
            strcpy(&codesArray[i * 8], "00000000");  // Jeśli znak nie jest cyfrą ani wielką literą, przyjmuje pusty kod
        }
    }

    printf("%s\n", codesArray);
}

void set_pixel_black(ImageInfo* imageinfo, unsigned int x, unsigned int y)
{
     if (x >= imageinfo->width || y >= imageinfo->height)
        return;
    unsigned pixels_per_byte = 8 / imageinfo->bitsperpel;
    unsigned int byte_idx = y * imageinfo->line_bytes + x / pixels_per_byte;
    unsigned int bit_idx = x % pixels_per_byte;
    unsigned char *pByte = imageinfo->pImg + byte_idx;
    *pByte &= ~(0x80 >> bit_idx);
}

void make_black_bar(ImageInfo* imageinfo, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    for (unsigned int i = x; i < x + width; i++)
    {
        for (unsigned int j = y; j < y + height; j++)
        {
            set_pixel_black(imageinfo, i, j);
        }
    }
}

char* check_character_type(const char* codesArray) {
    int length = strlen(codesArray);
    size_t numPairs = length / 8;
    //print the value of numPairs
    printf("numPairs: %d\n", numPairs);
    printf ("jestem w check_character_type przed mallocem\n");
    //malloc nie działa
    char* characterTypes = (char*)malloc(numPairs);
    printf ("jestem w check_character_type po mallocu\n");
    
    for (int i = 0; i < length; i += 8) {
        //create an array of 8 chars
        printf ("jestem w check_character_type w forze\n");
        char* pair = (char*)malloc(8);
        printf ("jestem w check_character_type po mallocu w forze\n");
        //copy the 8 chars from codesArray to pair
        strncpy(pair, &codesArray[i], 8);
        pair[8] = '\0';
        //print pair
        printf("pair: %s\n", pair);
        
        for (int j = 0; j <= 3; j++)
        {
            //print pair[j] and pair[j+4]
            printf("pair[%d]: %c\n", j, pair[j]);
            printf("pair[%d]: %c\n", j + 4, pair[j + 4]);
            printf ("jestem w check_character_type w forze w forze\n");

            int bottom = pair[j] - '0';
            int top = pair[j + 4] - '0';

            if (bottom == 0 && top == 0)
            {
                characterTypes[i / 8] = 'T';
            }
            else if (bottom == 0 && top == 1)
            {
                characterTypes[i / 8] = 'A';
            }
            else if (bottom == 1 && top == 0)
            {
                characterTypes[i / 8] = 'D';
            }
            else if (bottom == 1 && top == 1)
            {
                characterTypes[i / 8] = 'F';
            }
            else {
                printf("Error: bottom or top is not 0 or 1\n");
            }
            printf ("polelse\n");
        }
        free(pair);
        printf ("jestem w check_character_type przed free\n");
    }
   
    printf ("caloha");
    return characterTypes;
}

void make_bar(ImageInfo *imageinfo, unsigned int x, unsigned int y, unsigned int width, unsigned int height, char type) {
    unsigned int bar_height = 0;
    switch (type) {
        case 'T':
            bar_height = height / 3;
            y += height / 3;
            break;
        case 'A':
            bar_height = 2 * height / 3;
            y += height / 3;
            break;
        case 'D':
            bar_height = 2 * height / 3;
            break;
        case 'F':
            bar_height = height;
            break;
    }

    make_black_bar(imageinfo, x, y, width, bar_height);
}

ImageInfo *rm4scc_gen(unsigned int width, unsigned int height, const char *text)
{
    printf ("jestem przed imageinfo\n");
    ImageInfo *imageinfo = createImage(width, height, 1);
    printf ("jestem przed codesArray\n");
    char* codesArray = (char*) malloc(strlen(text) * 8 * sizeof(char));
    printf ("jestem przed getCodes\n");
    getCodes(text, codesArray);
    printf ("jestem przed characterTypes\n");
    char* characterTypes = check_character_type(codesArray);

    unsigned int bar_width = width / strlen(text);
    unsigned int space = bar_width;

    printf ("jestem przed pętlą\n");
    for (int i = 0; i < strlen(text); i++) {
        make_bar(imageinfo, i * space, 0, bar_width, height, characterTypes[i]);
    }

    printf ("jestem przed free\n");
    free(codesArray);
    free(characterTypes);
    return imageinfo;
}
