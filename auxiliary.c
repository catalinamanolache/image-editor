#include "auxiliary.h"

int clamp(int x)
{
    if (x > 255)
        return 255;
    else if (x < 0)
        return 0;
    else
        return x;
}

void allocate(int *lin, int *col, MAT ***aux)
{
    *aux = calloc((*lin), sizeof(MAT *));
    // daca esueaza alocarea, opresc executia
    DIE(!(*aux), "calloc failed");

    for (int i = 0; i < (*lin); i++) {
        (*aux)[i] = calloc((*col), sizeof(MAT));
        if (!(*aux)[i]) {
            // daca una din alocari a esuat, eliberez tot ce am alocat
            for (int j = 0; j < i; j++)
                free((*aux)[j]);

            free(*aux);
            *aux = NULL;
            DIE(1, "calloc failed");
        }
    }
}

void free_mat(MAT ***a, int *size)
{
    // eliberare pentru fiecare rand al matrice
    for (int i = 0; i < *size; i++)
        free((*a)[i]);

    // eliberare pentru toata matricea
    free(*a);
}

void swap(int *a, int *b)
{
    int aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

int word_number(char command[])
{
    char aux[50];
    strcpy(aux, command);
    char *q =  strtok(aux, " ");
    int nr = 0;
    while (q) {
        nr++;
        q = strtok(NULL, " ");
    }
    return nr;
}

int select_tokens(char **p, int *tok)
{
    if (strcmp(*p, "0") == 0 && atoi(*p) == 0) {
        *tok = 0;
        *p = strtok(NULL, " ");
    } else if (strcmp(*p, "0") != 0 && atoi(*p) == 0) {
        return 1;
    } else if (strcmp(*p, "0") != 0 && atoi(*p) != 0) {
        *tok = atoi(*p);
        *p = strtok(NULL, " ");
    }
    return 0;
}