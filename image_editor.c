// Copyright Manolache Maria-Catalina 313CAb 2023-2024
#include "auxiliary.h"
#include "main_functions.h"

int main(void)
{
	char command[50];
	PHOTO imag;
	imag.photo = NULL;
	while (1) {
		scanf("%s", command);
		if (strcmp(command, "EXIT") == 0) {
			EXIT(&imag);
			break;
		} else if (strcmp(command, "LOAD") == 0) {
			LOAD(&imag);
		} else if (strcmp(command, "SELECT") == 0) {
			SELECT(&imag);
		} else if (strcmp(command, "CROP") == 0) {
			CROP(&imag); 
		} else if (strcmp(command, "SAVE") == 0) {
			SAVE(&imag);
		} else if (strcmp(command, "HISTOGRAM") == 0) {
			HISTOGRAM(&imag);
		} else if (strcmp(command, "EQUALIZE") == 0) {
			EQUALIZE(imag);
		} else if (strcmp(command, "APPLY") == 0) {
			APPLY(&imag);
		} else if (strcmp(command, "ROTATE") == 0) {
			ROTATE(&imag);
		} else {
			fgets(command, sizeof(command), stdin);
			printf("Invalid command\n");
		}
	}

	return 0;
}
