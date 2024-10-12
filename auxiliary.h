#ifndef AUXILIARY_H
#define AUXILIARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DIE(assertion, call_description) \
    do { \
        if (assertion) { \
            fprintf(stderr, "(%s, %d): ", __FILE__, __LINE__); \
            perror(call_description); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

// structura pentru pixeli
typedef struct MAT {
    int r;
    int g;
    int b;
} MAT;

// structura pentru imagine
typedef struct PHOTO {
    int lin, col;
    int maxval;
    char magic[3];
    int x1, x2, y1, y2;
    MAT **photo;
} PHOTO;

// incadreaza rezultatele in intervalul [0,255]
int clamp(int x);

// aloca memorie pentru matrice
void allocate(int *lin, int *col, MAT ***aux);

// eliberare pentru fiecare rand al matrice
void free_mat(MAT ***a, int *size);

// swap pentru doua numere intregi
void swap(int *a, int *b);

// numara cuvintele dintr-un sir de caractere
int word_number(char command[]);

// se verifica daca vreunul dintre argumentele functiei SELECT este invalid
int select_tokens(char **p, int *tok);

#endif // AUXILIARY_H