#include "auxiliary.h"
#include "main_functions.h"

char FILE_TYPE(char *file_name)
{
    // presupun ca fis e binar, tipul b
	char magic[3], type = 'b';
	FILE *file = fopen(file_name, "rb");
	if (!file) {
		// deschiderea fisierului a esuat, returnez tipul eroare, f
		printf("Failed to load %s\n", file_name);
		free(file_name);
		return 'f';
	}

	fscanf(file, "%s", magic);
	fclose(file);

    // daca magic wordul corespunde unui fis ascii, returnez tipul text, t
	if (strcmp(magic, "P1") == 0 || strcmp(magic, "P2") == 0 ||
		strcmp(magic, "P3") == 0)
		type = 't';

	return type;
}

void LOAD(PHOTO *imag)
{
	char *file_name = malloc(50 * sizeof(char));
	scanf("%s", file_name);
	char type = FILE_TYPE(file_name);
	if (imag->photo)
		free_mat(&imag->photo, &imag->lin);

	// tipul eroare (f) presupune terminarea executiei
	if (type == 'f') {
		imag->photo = NULL;
		return;
	} else if (type == 't') {
		// incarcare pentru fisiere ascii
		FILE *in = fopen(file_name, "r");
		if (!in) {
			printf("Failed to load %s\n", file_name);
			return;
		}

		fscanf(in, "%s", imag->magic);
		fscanf(in, "%d %d", &imag->col, &imag->lin);
		if (strcmp(imag->magic, "P1") != 0)
			fscanf(in, "%d\n", &imag->maxval);

		allocate(&imag->lin, &imag->col, &imag->photo);

		if (strcmp(imag->magic, "P3") != 0) {
			// alb-negru si grayscale
			for (int i = 0; i < imag->lin; i++)
				for (int j = 0; j < imag->col; j++)
					fscanf(in, "%d", &imag->photo[i][j].r);
		} else {
			// rgb
			for (int i = 0; i < imag->lin; i++)
				for (int j = 0; j < imag->col; j++) {
					fscanf(in, "%d", &imag->photo[i][j].r);
					fscanf(in, "%d", &imag->photo[i][j].g);
					fscanf(in, "%d", &imag->photo[i][j].b);
				}
		}
		fclose(in);
		printf("Loaded %s\n", file_name);
        // selectez toata poza dupa incarcare
		imag->x1 = 0; imag->y1 = 0;
		imag->x2 = imag->col; imag->y2 = imag->lin;
		free(file_name);
	} else if (type == 'b') {
		// incarcare pentru fisiere binare
		FILE *in = fopen(file_name, "rb");
		if (!in) {
			printf("Failed to load %s\n", file_name);
			return;
		}

		fscanf(in, "%s", imag->magic);
		fscanf(in, "%d %d", &imag->col, &imag->lin);
		if (strcmp(imag->magic, "P4") != 0)
			fscanf(in, "%d\n", &imag->maxval);

		allocate(&imag->lin, &imag->col, &imag->photo);

		if (strcmp(imag->magic, "P6") != 0) {
			// alb-negru si grayscale
			for (int i = 0; i < imag->lin; i++)
				for (int j = 0; j < imag->col; j++)
					fread(&imag->photo[i][j].r, sizeof(unsigned char), 1, in);
		} else {
			// rgb
			for (int i = 0; i < imag->lin; i++)
				for (int j = 0; j < imag->col; j++) {
					fread(&imag->photo[i][j].r, sizeof(unsigned char), 1, in);
					fread(&imag->photo[i][j].g, sizeof(unsigned char), 1, in);
					fread(&imag->photo[i][j].b, sizeof(unsigned char), 1, in);
				}
		}
		fclose(in);
		printf("Loaded %s\n", file_name);

        // selectez toata poza dupa incarcare
		imag->x1 = 0; imag->y1 = 0;
		imag->x2 = imag->col; imag->y2 = imag->lin;
		free(file_name);
	}
}

void SELECT(PHOTO *imag)
{
	int x1, x2, y1, y2;
	char line[50];
	// prelucrez linia, indepartand \n si spatiul de la inceput
	fgets(line, sizeof(line), stdin);
	line[strlen(line) - 1] = '\0';
	for (size_t i = 0; i < strlen(line); i++)
		line[i] = line[i + 1];

	//  numarul de cuvinte al liniei
	int nr = word_number(line);
	char *p = strtok(line, " ");

	// daca se citeste optiunea ALL, se selecteaza toata poza
	if (nr == 1 && strcmp(line, "ALL") == 0) {
		if (!imag->photo) {
			printf("No image loaded\n");
			return;
		}
		imag->x1 = 0; imag->y1 = 0;
		imag->x2 = imag->col; imag->y2 = imag->lin;
		printf("Selected ALL\n");
		return;
	}
	// selectie clasica, cu orice coordonate
	if (nr == 4) {
		if (!imag->photo) {
			printf("No image loaded\n");
			return;
	}
        // daca vreuna dintre coordonate nu este valida (nu este numar)
		if (select_tokens(&p, &x1) || select_tokens(&p, &y1) ||
			select_tokens(&p, &x2) || select_tokens(&p, &y2)) {
			printf("Invalid command\n");
			return;
		}
	} else {
		// daca se citeste un numar necorespunzator de optiuni
		printf("Invalid command\n");
		return;
	}

    // daca se depaseste dimensiunea pozei sau coordonatele sunt invalide
	if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 || x1 == x2 || y1 == y2 ||
		x1 > imag->col || x2 > imag->col || y1 > imag->lin || y2 > imag->lin) {
		printf("Invalid set of coordinates\n");
		return;
	}

	// tratez cazurile x1 > x2 si y1 > y2
	if (x2 < x1)
		swap(&x1, &x2);

	if (y2 < y1)
		swap(&y1, &y2);

	// selectarea coordonatelor
	imag->x1 = x1; imag->x2 = x2;
	imag->y1 = y1; imag->y2 = y2;
	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
}

void CROP(PHOTO *imag)
{
	if (!imag->photo) {
		printf("No image loaded\n");
		return;
	}
	PHOTO aux;
	// dimensiunea selectiei curente
	int lin = imag->y2 - imag->y1, col = imag->x2 - imag->x1;

	// aloc o matrice auxiliara care are noile dimensiuni dupa taiere
	allocate(&lin, &col, &aux.photo);

	if ((strcmp(imag->magic, "P3") == 0) || (strcmp(imag->magic, "P6") == 0)) {
        // crop pentru o poza de tip rgb
		int k = 0, l = 0;
		for (int i = imag->y1; i < imag->y2; i++) {
			for (int j = imag->x1; j < imag->x2; j++) {
				aux.photo[k][l].r = imag->photo[i][j].r;
				aux.photo[k][l].g = imag->photo[i][j].g;
				aux.photo[k][l].b = imag->photo[i][j].b;
				l++;
				}
				k++;
				l = 0;
		}
		// eliberez vechea poza, actualizez dimensiunile si aloc noua poza
		free_mat(&imag->photo, &imag->lin);
		imag->lin = lin;
		imag->col = col;
		allocate(&imag->lin, &imag->col, &imag->photo);
		// actualizez vechea poza cu poza noua, taiata
		for (int i = 0; i < imag->lin; i++)
			for (int j = 0; j < imag->col; j++) {
				imag->photo[i][j].r = aux.photo[i][j].r;
				imag->photo[i][j].g = aux.photo[i][j].g;
				imag->photo[i][j].b = aux.photo[i][j].b;
				}
		// eliberez matricea auxiliara si reselectez intreaga poza
		free_mat(&aux.photo, &lin);
		imag->x1 = 0; imag->y1 = 0;
		imag->x2 = imag->col; imag->y2 = imag->lin;
		printf("Image cropped\n");
	} else {
		int k = 0, l = 0; // tai pozele de tip alb negru si grayscale
		for (int i = imag->y1; i < imag->y2; i++) {
			for (int j = imag->x1; j < imag->x2; j++) {
				aux.photo[k][l].r = (*imag).photo[i][j].r;
				l++;
				}
				k++;
				l = 0;
			}

		// eliberez vechea poza, actualizez dimensiunile si aloc noua poza
		free_mat(&imag->photo, &imag->lin);
		imag->lin = lin;
		imag->col = col;
		allocate(&imag->lin, &imag->col, &imag->photo);

		// actualizez vechea poza cu poza noua, taiata
		for (int i = 0; i < lin; i++)
			for (int j = 0; j < col; j++)
				imag->photo[i][j].r = aux.photo[i][j].r;

		// eliberez matricea auxiliara si reselectez intreaga poza
		free_mat(&aux.photo, &lin);
		imag->x1 = 0; imag->y1 = 0;
		imag->x2 = imag->col; imag->y2 = imag->lin;
		printf("Image cropped\n");
		}
}

void EDGE(PHOTO *imag)
{   
    // daca imaginea este de tip grayscale, nu se poate aplica filtrul
	if (strcmp(imag->magic, "P2") == 0 || strcmp(imag->magic, "P5") == 0) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}

	// aloc o matrice auxiliara
	PHOTO aux;
	allocate(&imag->lin, &imag->col, &aux.photo);

	// lucrez pe selectie
	for (int i = imag->y1; i < imag->y2; i++)
		for (int j = imag->x1; j < imag->x2; j++) {
			// ignor marginile, copiind valorile originale in auxiliara
			if (i == 0 || j == 0 || i == imag->lin - 1 || j == imag->col - 1) {
				aux.photo[i][j].r = imag->photo[i][j].r;
				aux.photo[i][j].g = imag->photo[i][j].g;
				aux.photo[i][j].b = imag->photo[i][j].b;
				continue;
			}
		    // aplic matricea kernel, adunand pixelii inmultiti
            // cu elementele din kernel
			aux.photo[i][j].r = -imag->photo[i - 1][j - 1].r -
						imag->photo[i - 1][j].r - imag->photo[i - 1][j + 1].r -
						imag->photo[i][j - 1].r + 8 * imag->photo[i][j].r -
						imag->photo[i][j + 1].r - imag->photo[i + 1][j - 1].r -
						imag->photo[i + 1][j].r - imag->photo[i + 1][j + 1].r;

			aux.photo[i][j].g = -imag->photo[i - 1][j - 1].g -
						imag->photo[i - 1][j].g - imag->photo[i - 1][j + 1].g -
						imag->photo[i][j - 1].g + 8 * imag->photo[i][j].g -
						imag->photo[i][j + 1].g - imag->photo[i + 1][j - 1].g -
						imag->photo[i + 1][j].g - imag->photo[i + 1][j + 1].g;

			aux.photo[i][j].b = -imag->photo[i - 1][j - 1].b -
						imag->photo[i - 1][j].b - imag->photo[i - 1][j + 1].b -
						imag->photo[i][j - 1].b + 8 * imag->photo[i][j].b -
						imag->photo[i][j + 1].b - imag->photo[i + 1][j - 1].b -
						imag->photo[i + 1][j].b - imag->photo[i + 1][j + 1].b;
		}
	// readuc rezultatele in [0,255] si actualizez vechea matrice
	for (int i = imag->y1; i < imag->y2; i++)
		for (int j = imag->x1; j < imag->x2; j++) {
			imag->photo[i][j].r = clamp(aux.photo[i][j].r);
			imag->photo[i][j].g = clamp(aux.photo[i][j].g);
			imag->photo[i][j].b = clamp(aux.photo[i][j].b);
		}

	// eliberez matricea auxiliara
	free_mat(&aux.photo, &imag->lin);
	printf("APPLY EDGE done\n");
}

void SHARPEN(PHOTO *imag)
{
    // daca imaginea este de tip grayscale, nu se poate aplica filtrul
	if (strcmp(imag->magic, "P2") == 0 || strcmp(imag->magic, "P5") == 0) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}

	// aloc o matrice auxiliara
	PHOTO aux;
	allocate(&imag->lin, &imag->col, &aux.photo);

	// lucrez pe selectie
	for (int i = imag->y1; i < imag->y2; i++)
		for (int j = imag->x1; j < imag->x2; j++) {
			// ignor marginile, copiind valorile originale in auxiliara
			if (i == 0 || j == 0 || i == imag->lin - 1 || j == imag->col - 1) {
				aux.photo[i][j].r = imag->photo[i][j].r;
				aux.photo[i][j].g = imag->photo[i][j].g;
				aux.photo[i][j].b = imag->photo[i][j].b;
				continue;
			}
		    // aplic matricea kernel, adunand pixelii inmultiti
            // cu elementele din kernel
			aux.photo[i][j].r = 0 * imag->photo[i - 1][j - 1].r -
					imag->photo[i - 1][j].r + 0 * imag->photo[i - 1][j + 1].r -
					imag->photo[i][j - 1].r + 5 * imag->photo[i][j].r -
					imag->photo[i][j + 1].r + 0 * imag->photo[i + 1][j - 1].r -
					imag->photo[i + 1][j].r + 0 * imag->photo[i + 1][j + 1].r;

			aux.photo[i][j].g = 0 * imag->photo[i - 1][j - 1].g -
					imag->photo[i - 1][j].g + 0 * imag->photo[i - 1][j + 1].g -
					imag->photo[i][j - 1].g + 5 * imag->photo[i][j].g -
					imag->photo[i][j + 1].g + 0 * imag->photo[i + 1][j - 1].g -
					imag->photo[i + 1][j].g + 0 * imag->photo[i + 1][j + 1].g;

			aux.photo[i][j].b = 0 * imag->photo[i - 1][j - 1].b -
					imag->photo[i - 1][j].b + 0 * imag->photo[i - 1][j + 1].b -
					imag->photo[i][j - 1].b + 5 * imag->photo[i][j].b -
					imag->photo[i][j + 1].b + 0 * imag->photo[i + 1][j - 1].b -
					imag->photo[i + 1][j].b + 0 * imag->photo[i + 1][j + 1].b;
			}
	// readuc rezultatele in [0,255] si actualizez vechea matrice
	for (int i = imag->y1; i < imag->y2; i++)
		for (int j = imag->x1; j < imag->x2; j++) {
			imag->photo[i][j].r = clamp(aux.photo[i][j].r);
			imag->photo[i][j].g = clamp(aux.photo[i][j].g);
			imag->photo[i][j].b = clamp(aux.photo[i][j].b);
		}

	// eliberez matricea auxiliara
	free_mat(&aux.photo, &imag->lin);
	printf("APPLY SHARPEN done\n");
}

void BLUR(PHOTO *imag)
{
    // daca imaginea este de tip grayscale, nu se poate aplica filtrul
	if (strcmp(imag->magic, "P2") == 0 || strcmp(imag->magic, "P5") == 0) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}

	// aloc o matrice auxiliara
	PHOTO aux;
	allocate(&imag->lin, &imag->col, &aux.photo);

	// lucrez pe selectie
	for (int i = imag->y1; i < imag->y2; i++)
		for (int j = imag->x1; j < imag->x2; j++) {
			// ignor marginile, copiind valorile originale in auxiliara
			if (i == 0 || j == 0 || i == imag->lin - 1 || j == imag->col - 1) {
				aux.photo[i][j].r = imag->photo[i][j].r;
				aux.photo[i][j].g = imag->photo[i][j].g;
				aux.photo[i][j].b = imag->photo[i][j].b;
				continue;
			}
		    // aplic matricea kernel, adunand pixelii inmultiti cu
            // elementele din kernel
			aux.photo[i][j].r = (imag->photo[i - 1][j - 1].r +
						imag->photo[i - 1][j].r + imag->photo[i - 1][j + 1].r +
						imag->photo[i][j - 1].r + imag->photo[i][j].r +
						imag->photo[i][j + 1].r + imag->photo[i + 1][j - 1].r +
						imag->photo[i + 1][j].r + imag->photo[i + 1][j + 1].r)
						/ 9;

			aux.photo[i][j].g = (imag->photo[i - 1][j - 1].g +
						imag->photo[i - 1][j].g + imag->photo[i - 1][j + 1].g +
						imag->photo[i][j - 1].g + imag->photo[i][j].g +
						imag->photo[i][j + 1].g + imag->photo[i + 1][j - 1].g +
						imag->photo[i + 1][j].g + imag->photo[i + 1][j + 1].g)
						/ 9;

			aux.photo[i][j].b = (imag->photo[i - 1][j - 1].b +
						imag->photo[i - 1][j].b + imag->photo[i - 1][j + 1].b +
						imag->photo[i][j - 1].b + imag->photo[i][j].b +
						imag->photo[i][j + 1].b + imag->photo[i + 1][j - 1].b +
						imag->photo[i + 1][j].b + imag->photo[i + 1][j + 1].b)
						/ 9;
			}

	// readuc rezultatele in [0,255] si actualizez vechea matrice
	for (int i = imag->y1; i < imag->y2; i++)
		for (int j = imag->x1; j < imag->x2; j++) {
			imag->photo[i][j].r = clamp(aux.photo[i][j].r);
			imag->photo[i][j].g = clamp(aux.photo[i][j].g);
			imag->photo[i][j].b = clamp(aux.photo[i][j].b);
		}

	// eliberez matricea auxiliara
	free_mat(&aux.photo, &imag->lin);
	printf("APPLY BLUR done\n");
}

void GAUSSIAN_BLUR(PHOTO *imag)
{
    // daca imaginea este de tip grayscale, nu se poate aplica filtrul
	if (strcmp(imag->magic, "P2") == 0 || strcmp(imag->magic, "P5") == 0) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}

	// aloc o matrice auxiliara
	PHOTO aux;
	allocate(&imag->lin, &imag->col, &aux.photo);

	// lucrez pe selectie
	for (int i = imag->y1; i < imag->y2; i++)
		for (int j = imag->x1; j < imag->x2; j++) {
			// ignor marginile, copiind valorile originale in auxiliara
			if (i == 0 || j == 0 || i == imag->lin - 1 || j == imag->col - 1) {
				aux.photo[i][j].r = imag->photo[i][j].r;
				aux.photo[i][j].g = imag->photo[i][j].g;
				aux.photo[i][j].b = imag->photo[i][j].b;
				continue;
			}
		    // aplic matricea kernel, adunand pixelii inmultiti
            // cu elementele din kernel
			aux.photo[i][j].r = (imag->photo[i - 1][j - 1].r +
					2 * imag->photo[i - 1][j].r + imag->photo[i - 1][j + 1].r +
					2 * imag->photo[i][j - 1].r + 4 * imag->photo[i][j].r +
					2 * imag->photo[i][j + 1].r + imag->photo[i + 1][j - 1].r +
					2 * imag->photo[i + 1][j].r + imag->photo[i + 1][j + 1].r)
					/ 16;

			aux.photo[i][j].g = (imag->photo[i - 1][j - 1].g +
					2 * imag->photo[i - 1][j].g + imag->photo[i - 1][j + 1].g +
					2 * imag->photo[i][j - 1].g + 4 * imag->photo[i][j].g +
					2 * imag->photo[i][j + 1].g + imag->photo[i + 1][j - 1].g +
					2 * imag->photo[i + 1][j].g + imag->photo[i + 1][j + 1].g)
					/ 16;

			aux.photo[i][j].b = (imag->photo[i - 1][j - 1].b +
					2 * imag->photo[i - 1][j].b + imag->photo[i - 1][j + 1].b +
					2 * imag->photo[i][j - 1].b + 4 * imag->photo[i][j].b +
					2 * imag->photo[i][j + 1].b + imag->photo[i + 1][j - 1].b +
					2 * imag->photo[i + 1][j].b + imag->photo[i + 1][j + 1].b)
					/ 16;
		}

	// readuc rezultatele in [0,255] si actualizez vechea matrice
	for (int i = imag->y1; i < imag->y2; i++)
		for (int j = imag->x1; j < imag->x2; j++) {
			imag->photo[i][j].r = clamp(aux.photo[i][j].r);
			imag->photo[i][j].g = clamp(aux.photo[i][j].g);
			imag->photo[i][j].b = clamp(aux.photo[i][j].b);
		}

	// eliberez matricea auxiliara
	free_mat(&aux.photo, &imag->lin);
	printf("APPLY GAUSSIAN_BLUR done\n");
}

void APPLY(PHOTO *imag)
{
	char option[16];
	fgets(option, sizeof(option), stdin);

	// prelucrez optiunea, indepartand \n si spatiul de la inceput
	option[strlen(option) - 1] = '\0';
	for (size_t i = 0; i < strlen(option); i++)
		option[i] = option[i + 1];

	if (!imag->photo) {
        // nu se poate aplica filtrul daca nu exista poza
		printf("No image loaded\n");
		return;
	} else if (strcmp(option, "EDGE") == 0) {
        // aplic filtrul EDGE
		EDGE(imag);
		return;
	} else if (strcmp(option, "SHARPEN") == 0) {
        // aplic filtrul SHARPEN
		SHARPEN(imag);
		return;
	} else if (strcmp(option, "BLUR") == 0) {
        // aplic filtrul BLUR
		BLUR(imag);
		return;
	} else if (strcmp(option, "GAUSSIAN_BLUR") == 0) {
        // aplic filtrul GAUSSIAN_BLUR
		GAUSSIAN_BLUR(imag);
		return;
	} else if (strcmp(option, "") == 0) {
        // daca nu se citeste nicio optiune
		printf("Invalid command\n");
		return;
	}
	// daca se citeste o optiune care nu exista
	printf("APPLY parameter invalid\n");
}

void HISTOGRAM(PHOTO *imag)
{
	int X, Y;
	char line[20];

	// prelucrez linia, indepartand \n si spatiul de la inceput
	fgets(line, sizeof(line), stdin);
	line[strlen(line) - 1] = '\0';
	for (size_t i = 0; i < strlen(line); i++)
		line[i] = line[i + 1];

	//  numarul de cuvinte al liniei
	int nr = word_number(line);
	char *p = strtok(line, " ");

	if (nr == 2) {
		if (!imag->photo) {
			printf("No image loaded\n");
			return;
	} 
        // daca vreuna dintre coordonate nu este valida
		if (select_tokens(&p, &X) || select_tokens(&p, &Y)) {
			printf("Invalid command\n");
			return;
		}
	} else {
		// daca se citeste un numar necorespunzator de optiuni
		if (!imag->photo) {
			printf("No image loaded\n");
			return;
		}
		printf("Invalid command\n");
		return;
	}

    // daca imaginea este rgb, nu se poate crea histograma
	if (strcmp(imag->magic, "P3") == 0 || strcmp(imag->magic, "P6") == 0) {
		printf("Black and white image needed\n");
		return;
	} else if (Y % 2 == 1 || Y < 2 || Y > 256) {
		printf("Invalid set of parameters\n");
		return;
	}

	int frecv[256] = {0}, dist[256] = {-1}, k = 0;
	int maxfr = 0;
	// vectorul de frecventa al pixelilor
	for (int i = 0; i < imag->lin; i++)
		for (int j = 0; j < imag->col; j++)
			frecv[imag->photo[i][j].r]++;

	// valorile distincte si frecventa maxima
	for (int i = 0; i < 256; i++) {
		if (frecv[i] != 0) {
			dist[k] = i;
			k++;
		}
		if (frecv[i] > maxfr)
			maxfr = frecv[i];
	}

	// afisarea corespunzatoare a histogramei
	int nrstel = 0;
	for (int i = 0; i < Y; i++) {
		if (dist[i] == -1) {
			printf("0\t|\t\n");
		} else {
			nrstel = (frecv[dist[i]] * X) / maxfr;
			printf("%d\t|\t", nrstel);
			for (int j = 0; j < nrstel; j++)
				printf("*");
			printf("\n");
			nrstel = 0;
		}
	}
}

void EQUALIZE(PHOTO imag)
{
	if (!imag.photo) {
		printf("No image loaded\n");
		return;
	} else if (strcmp(imag.magic, "P3") == 0 || strcmp(imag.magic, "P6") == 0) {
        // daca imaginea este rgb, nu se poate aplica equalize
		printf("Black and white image needed\n");
		return;
	}

	long long area = imag.lin * imag.col, s = 0;
	int frecv[256] = {0};
	double fraction;

	// calculez frecventa fiecarui pixel
	for (int i = 0; i < imag.lin; i++)
		for (int j = 0; j < imag.col; j++)
			frecv[imag.photo[i][j].r]++;

	// realizez egalizarea pozei dupa algoritmul furnizat
	for (int i = 0; i < imag.lin; i++)
		for (int j = 0; j < imag.col; j++) {
			s = 0;
			for (int k = 0; k <= imag.photo[i][j].r; k++)
				s += frecv[k];
			fraction = (255 * s) / area;
			// aduc valorile in int [0,255]
			imag.photo[i][j].r = clamp((round(fraction)));
		}

	printf("Equalize done\n");
}

void ROTATE(PHOTO *imag)
{
	int angle;
	scanf("%d", &angle);
	if (!imag->photo) {
		printf("No image loaded\n");
		return;
	} else if (angle % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return;
	} else if (imag->x1 != 0 && imag->y1 != 0 && imag->x2 != imag->col &&
		imag->y2 != imag->lin && imag->x2 - imag->x1 != imag->y2 - imag->y1) {
		// daca selectia nu e patratica sau e diferita de intreaga poza
		printf("The selection must be square\n");
		return;
	}
	printf("Rotated %d\n", angle);
}

void SAVE_BIN(PHOTO *imag, char save_name[])
{
	FILE *out = fopen(save_name, "wb");
	if (!out)
		return;

	// convertire tip, daca imaginea e in ascii se transforma in binar
	if (strcmp(imag->magic, "P1") == 0)
		strcpy(imag->magic, "P4");
	else if (strcmp(imag->magic, "P2") == 0)
		strcpy(imag->magic, "P5");
	else if (strcmp(imag->magic, "P3") == 0)
		strcpy(imag->magic, "P6");

	fprintf(out, "%s\n", imag->magic);
	fprintf(out, "%d %d\n", imag->col, imag->lin);
	if (strcmp(imag->magic, "P4") != 0)
		fprintf(out, "%d\n", imag->maxval);

	if (strcmp(imag->magic, "P6") != 0) {
		// salvare imagine alb negru sau grayscale binar
		for (int i = 0; i < imag->lin; i++)
			for (int j = 0; j < imag->col; j++)
				fwrite(&imag->photo[i][j].r, sizeof(char), 1, out);
	} else {
		// salvare imagine rgb binar
		for (int i = 0; i < imag->lin; i++)
			for (int j = 0; j < imag->col; j++) {
				fwrite(&imag->photo[i][j].r, sizeof(char), 1, out);
				fwrite(&imag->photo[i][j].g, sizeof(char), 1, out);
				fwrite(&imag->photo[i][j].b, sizeof(char), 1, out);
			}
	}
	fclose(out);
	printf("Saved %s\n", save_name);
}

void SAVE_ASCII(PHOTO *imag, char save_name[])
{
	FILE *out = fopen(save_name, "w");
	if (!out)
		return;
	// convertire tip, daca imaginea e in binar se transforma in ascii
	if (strcmp(imag->magic, "P4") == 0)
		strcpy(imag->magic, "P1");
	else if (strcmp(imag->magic, "P5") == 0)
		strcpy(imag->magic, "P2");
	else if (strcmp(imag->magic, "P6") == 0)
		strcpy(imag->magic, "P3");

	fprintf(out, "%s\n", imag->magic);
	fprintf(out, "%d %d\n", imag->col, imag->lin);
	if (strcmp(imag->magic, "P1") != 0)
		fprintf(out, "%d\n", imag->maxval);

	if (strcmp(imag->magic, "P3") != 0) {
		// salvare imagine alb negru si grayscale format text
		for (int i = 0; i < imag->lin; i++) {
			for (int j = 0; j < imag->col; j++)
				fprintf(out, "%d ", imag->photo[i][j].r);
			fprintf(out, "\n");
		}
	} else {
		// salvare imagine rgb format text
		for (int i = 0; i < imag->lin; i++) {
			for (int j = 0; j < imag->col; j++) {
				fprintf(out, "%d ", imag->photo[i][j].r);
				fprintf(out, "%d ", imag->photo[i][j].g);
				fprintf(out, "%d ", imag->photo[i][j].b);
				}
			fprintf(out, "\n");
		}
		}
	fclose(out);
	printf("Saved %s\n", save_name);
}

void SAVE(PHOTO *imag)
{
	char save_name[100];
	fgets(save_name, sizeof(save_name), stdin);

	// prelucrez inputul
	save_name[strlen(save_name) - 1] = '\0';
	for (size_t i = 0; i < strlen(save_name); i++)
		save_name[i] = save_name[i + 1];

	// voi numara cuvintele, iar daca se citeste si optiunea ascii aceasta
	// va fi copiata intr-o auxiliara pentru a o putea verifica la apelare
	char *option = strtok(save_name, " "), *aux = calloc(6, sizeof(char));
	int nr = 0;
	while (option) {
		if (nr == 1)
			strcpy(aux, option);
		option = strtok(NULL, " ");
		nr++;
	}

	if (!imag->photo) {
		printf("No image loaded\n");
		free(aux);
		return;
	} else if (nr == 2 && strcmp(aux, "ascii") == 0) {
		// daca am introdus optiunea ascii, salvez in fisier text
		SAVE_ASCII(imag, save_name);
	} else if (nr == 1) {
		// daca am 1 cuvant si nu exista optiunea ascii, salvez in fisier binar
		SAVE_BIN(imag, save_name);
	}

	free(aux);
}

void EXIT(PHOTO *imag)
{
	if (!imag->photo) {
		printf("No image loaded\n");
		return;
	}
	// daca am o imagine incarcata
	if (imag->photo)
		free_mat(&imag->photo, &imag->lin);
}