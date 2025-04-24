#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"


t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file==NULL) {
        printf("Erreur d'ouverture du fichier");
        return NULL;
    }


    t_bmp8 *img = malloc(sizeof(t_bmp8));
    if (!img) {
        fclose(file);
        return NULL;
    }


    fread(img->header, sizeof(unsigned char), 54, file);
    fread(img->colorTable, sizeof(unsigned char), 1024, file);


    img->width = *(unsigned int *)&img->header[18];
    img->height = *(unsigned int *)&img->header[22];
    img->colorDepth = *(unsigned short *)&img->header[28];
    img->dataSize = *(unsigned int *)&img->header[34];


    if (img->colorDepth != 8) {
        printf("L'image n'est pas en 8 bits.\n");
        free(img);
        fclose(file);
        return NULL;
    }




    if (img->dataSize == 0) {
        img->dataSize = img->width * img->height;
    }


    img->data = malloc(img->dataSize);
    if (!img->data) {
        free(img);
        fclose(file);
        return NULL;
    }


    fread(img->data, sizeof(unsigned char), img->dataSize, file);
    fclose(file);
    return img;
}


void bmp8_saveImage(const char *filename, t_bmp8 *img) {
    FILE *file = fopen(filename, "wb");
    if (file==NULL) {
        printf("Erreur de sauvegarde");
        return;
    }


    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);
    fclose(file);
}


void bmp8_free(t_bmp8 *img) {
    if (img) {
        free(img->data);
        free(img);
    }
}


void bmp8_printInfo(t_bmp8 *img) {
    printf("Image Info:\n");
    printf("Width: %d\n", img->width);
    printf("Height: %d\n", img->height);
    printf("Color Depth: %d\n", img->colorDepth);
    printf("Data Size: %d\n", img->dataSize);
}


void bmp8_negative(t_bmp8 *img) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}


void bmp8_brightness(t_bmp8 *img, int value) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        int pixel = img->data[i] + value;
        if (pixel > 255) pixel = 255;
        if (pixel < 0) pixel = 0;
        img->data[i] = (unsigned char)pixel;
    }
}


void bmp8_threshold(t_bmp8 *img, int threshold) {
    for (unsigned int i = 0; i < img->dataSize; i++) {
        if (img->data[i] >= threshold) {
            img->data[i] = 255;
        } else {
            img->data[i] = 0;
        }
    }
}


void bmp8_applyFilter(t_bmp8 *img, float kernel[3][3]) {
    unsigned char *temp = malloc(img->dataSize);
    memcpy(temp, img->data, img->dataSize);


    for (unsigned int y = 1; y < img->height - 1; y++) {
        for (unsigned int x = 1; x < img->width - 1; x++) {
            float sum = 0.0;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int px = x + kx;
                    int py = y + ky;
                    sum += temp[py * img->width + px] * kernel[ky + 1][kx + 1];
                }
            }
            int val = (int)sum;
            if (val < 0) val = 0;
            if (val > 255) val = 255;
            img->data[y * img->width + x] = (unsigned char)val;
        }
    }


    free(temp);
}

