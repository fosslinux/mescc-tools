/* Copyright (C) 2020 fosslinux
 * This file is part of mescc-tools
 *
 * mescc-tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mescc-tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mescc-tools.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/* Define all of the constants */
// CONSTANT FALSE 0
#define FALSE 0
// CONSTANT TRUE 1
#define TRUE 1
// CONSTANT MAX_STRING 4096
#define MAX_STRING 4096

int match(char* a, char* b);
void file_print(char* s, FILE* f);
char* copy_string(char* target, char* source);
void require(int bool, char* error);

int copy_file(char* source, char* dest)
{
	/* Open source and dest as FILE*s */
	FILE* fsource = fopen(source, "r");
	require(fsource != NULL, "Error opening source file\n");
	FILE* fdest = fopen(dest, "w");
	require(fdest >= 0, "Error opening destination file\n");

	/*
	 * The following loop reads a character from the source and writes it to the
	 * dest file. This is all M2-Planet supports.
	 */
	char c = fgetc(fsource);
	while(c != EOF)
	{
		fputc(c, fdest);
		c = fgetc(fsource);
	}

	/* Cleanup */
	fclose(fsource);
	fclose(fdest);
}

int main(int argc, char** argv)
{
	/* Initialize variables */
	char* source = NULL;
	char* dest = NULL;

	int i = 1;
	/* Loop arguments */
	while(i <= argc)
	{
		if(NULL == argv[i])
		{ /* Ignore and continue */
			i = i + 1;
		}
		else if(match(argv[i], "-V") || match(argv[i], "--version"))
		{ /* Output version */
			file_print("cp version 1.1.0\n", stdout);
			exit(EXIT_SUCCESS);
		}
		else if(argv[i][0] != '-')
		{ /* It is not an option */
			/*
			 * We can tell if this is the source file or the destination file
			 * through looking at whether the source file is already set. We
			 * require the source file to be given first so if the source file
			 * is already set we know it must be the destination.
			 */
			if(source != NULL)
			{ /* We are setting the destination */
				dest = calloc(MAX_STRING, sizeof(char));
				copy_string(dest, argv[i]);
			}
			else
			{ /* We are setting the source */
				source = calloc(MAX_STRING, sizeof(char));
				copy_string(source, argv[i]);
			}
			i = i + 1;
		}
		else
		{ /* Unknown argument */
			file_print("UNKNOWN_ARGUMENT\n", stderr);
			exit(EXIT_FAILURE);
		}
	}

	/* Sanitize values */
	/* Ensure the two values have values */
	require(source != NULL, "Provide a source file\n");
	require(dest != NULL, "Provide a destination file\n");

	/* Perform the actual copy */
	copy_file(source, dest);
}
