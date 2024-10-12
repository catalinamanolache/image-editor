#ifndef MAIN_FUNCTIONS_H
#define MAIN_FUNCTIONS_H

#include "auxiliary.h"

// returneaza tipul fisierului din care se va incarca imaginea
char FILE_TYPE(char *file_name);

// incarcarea in memorie a unei imagini
void LOAD(PHOTO *imag);

// selectarea unei anumite zone din imagine
void SELECT(PHOTO *imag);

// taierea imaginii conform selectiei curente
void CROP(PHOTO *imag);

// se aplica filtrul edge pe imagine
void EDGE(PHOTO *imag);

// se aplica filtrul sharpen pe imagine
void SHARPEN(PHOTO *imag);

// se aplica filtrul blur pe imagine
void BLUR(PHOTO *imag);

// se aplica filtrul gaussian blur pe imagine
void GAUSSIAN_BLUR(PHOTO *imag);

// se aplica un anumit filtru pe imagine
void APPLY(PHOTO *imag);

// se creeaza histograma imaginii
void HISTOGRAM(PHOTO *imag);

// se realizeaza egalizarea unei imagini alb-negru sau grayscale
void EQUALIZE(PHOTO imag);

// se realizeaza rotirea unei imagini
void ROTATE(PHOTO *imag);

// se salveaza o imagine in format binar
void SAVE_BIN(PHOTO *imag, char save_name[]);

// se salveaza o imagine in format ascii
void SAVE_ASCII(PHOTO *imag, char save_name[]);

// se salveaza o imagine in formatul ales
void SAVE(PHOTO *imag);

// se realizeaza eliberarea tuturor resuerselor alocate
void EXIT(PHOTO *imag);

#endif // MAIN_FUNCTIONS_H