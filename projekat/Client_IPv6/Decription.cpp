#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Decription.h"


void MonoalphabeticDecipher(char* fileName, char* decripted) {
	FILE* fp, *fpD;

	char ch;

	fpD = fopen(decripted, "w");
	if (fpD == NULL) {
		perror("Error in opening file.");
		exit(1);
	}

	fp = fopen(fileName, "r");
	if (fp == NULL) {
		perror("Error in reading file.");
		exit(1);
	}

	ch = fgetc(fp);
	while (ch != EOF)
	{
		fputc((ch - 1), fpD);

		ch = fgetc(fp);
	}
	fclose(fp);
	fclose(fpD);

	return;
}