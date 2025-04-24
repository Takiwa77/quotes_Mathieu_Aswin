#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"


void showMenu() {
    printf("\nVeuillez choisir une option :\n");
    printf("1. Ouvrir une image\n");
    printf("2. Sauvegarder une image\n");
    printf("3. Appliquer un filtre\n");
    printf("4. Afficher les informations de l'image\n");
    printf("5. Quitter\n>>> ");
}


void showFilters() {
    printf("\nVeuillez choisir un filtre :\n");
    printf("1. Négatif\n");
    printf("2. Luminosité\n");
    printf("3. Binarisation\n");
    printf("4. Flou (box blur)\n");
    printf("5. Retour\n>>> ");
}


int main() {
    t_bmp8 *img = NULL;
    int choix;


    while (1) {
        showMenu();
        scanf("%d", &choix);
        if (choix == 1) {
            char path[256];
            printf("Chemin du fichier : ");
            scanf("%s", path);
            img = bmp8_loadImage(path);
            if (img) printf("Image chargée avec succès !\n");
        } else if (choix == 2 && img) {
            char path[256];
            printf("Chemin du fichier de sortie : ");
            scanf("%s", path);
            bmp8_saveImage(path, img);
            printf("Image sauvegardée avec succès !\n");
        } else if (choix == 3 && img) {
            int f;
            showFilters();
            scanf("%d", &f);
            if (f == 1) bmp8_negative(img);
            else if (f == 2) {
                int v;
                printf("Valeur de luminosité (-255 à 255) : ");
                scanf("%d", &v);
                bmp8_brightness(img, v);
            } else if (f == 3) {
                int t;
                printf("Seuil de binarisation (0-255) : ");
                scanf("%d", &t);
                bmp8_threshold(img, t);
            } else if (f == 4) {
                float blur[3][3] = {
                    {1/9.0, 1/9.0, 1/9.0},
                    {1/9.0, 1/9.0, 1/9.0},
                    {1/9.0, 1/9.0, 1/9.0}
                };
                bmp8_applyFilter(img, blur);
            }
        } else if (choix == 4 && img) {
            bmp8_printInfo(img);
        } else if (choix == 5) {
            break;
        } else {
            printf("Option invalide ou image non chargée.\n");
        }
    }


    bmp8_free(img);
    return 0;
}
