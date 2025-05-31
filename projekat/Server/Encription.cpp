#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Encription.h"


void MonoalphabeticCipher(char* fileName, char* encripted) {
	FILE* fp, *fpE;
	fpE = fopen(encripted, "w");

	fp = fopen(fileName, "r");
	if (fp == NULL) {
		perror("Error in reading file.");
		exit(1);
	}

	int n;
	char ch;

	ch = fgetc(fp);
	while (ch != EOF)
	{
		/* Write to destination file */
		fputc((ch + 1), fpE);

		/* Read next character from source file */
		ch = fgetc(fp);
	}
	fclose(fp);
	fclose(fpE);

	return;
}