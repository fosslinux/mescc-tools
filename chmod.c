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
#include <sys/stat.h>

/* Define all of the constants */
// CONSTANT FALSE 0
#define FALSE 0
// CONSTANT TRUE 1
#define TRUE 1
// CONSTANT MAX_STRING 4096
#define MAX_STRING 4096
// CONSTANT MAX_ARRAY 256
#define MAX_ARRAY 256

/* Prototypes for external funcs */
void file_print(char* s, FILE* f);
char* prepend_char(char a, char* s);
int numerate_string(char *a);
void require(int bool, char* error);
char* copy_string(char* target, char* source);
int match(char* a, char* b);
/* Globals */
int verbose;

/* UTILITY FUNCTIONS */

/* Function to find the length of a char**; an array of strings */
int array_length(char** array)
{
	int length = 0;
	while(array[length] != NULL)
	{
		length = length + 1;
	}
	return length;
}

/* PROCESSING FUNCTIONS */

int main(int argc, char** argv)
{
	/* Initialize variables */
	char** files = calloc(MAX_ARRAY, sizeof(char*));
	require(files != NULL, "Memory initialization of files failed\n");
	int files_index = 0;
	char* mode = NULL;

	/* Set defaults */
	verbose = FALSE;

	int i = 1;
	/* Loop arguments */
	while(i <= argc)
	{
		if(NULL == argv[i])
		{ /* Ignore and continue */
			i = i + 1;
		}
		else if(match(argv[i], "-h") || match(argv[i], "--help"))
		{
			file_print("Usage: ", stdout);
			file_print(argv[0], stdout);
			file_print(" [-h | --help] [-V | --version] [-v | --verbose]\n", stdout);
			exit(EXIT_SUCCESS);
		}
		else if(match(argv[i], "-V") || match(argv[i], "--version"))
		{ /* Output version */
			file_print("chmod version 1.1.0\n", stdout);
			exit(EXIT_SUCCESS);
		}
		else if(match(argv[i], "-v") || match(argv[i], "--verbose"))
		{
			verbose = TRUE;
			i = i + 1;
		}
		else if(argv[i][0] != '-')
		{ /* It must be the file or the mode */
			if(mode == NULL)
			{ /* Mode always comes first */
				mode = calloc(MAX_STRING, sizeof(char));
				require(mode != NULL, "Memory initialization of mode failed\n");
				copy_string(mode, argv[i]);
			}
			else
			{ /* It's a file, as the mode is already done */
				files[files_index] = calloc(MAX_STRING, sizeof(char));
				require(files[files_index] != NULL, "Memory initialization of files[files_index] failed\n");
				copy_string(files[files_index], argv[i]);
				files_index = files_index + 1;
			}
			i = i + 1;
		}
		else
		{ /* Unknown argument */
			file_print("UNKNOWN_ARGUMENT\n", stderr);
			exit(EXIT_FAILURE);
		}
	}

	/* Ensure the two values have values */
	require(mode != NULL, "Provide a mode\n");
	require(files[0] != NULL, "Provide a file\n");

	/* Convert the mode str into octal */
	if(mode[0] != '0')
	{ /* We need to indicate it is octal */
		mode = prepend_char('0', mode);
	}
	int omode = numerate_string(mode);

	/* Loop over files to be operated on */
	FILE* test;
	for(i = 0; i < array_length(files); i = i + 1)
	{
		/* Make sure the file can be opened */
		test = fopen(files[i], "r");
		require(test != NULL, "A file cannot be read\n");
		fclose(test);

		/* Verbose message */
		if(verbose)
		{
			file_print("mode of '", stdout);
			file_print(files[i], stdout);
			file_print("' changed to ", stdout);
			file_print(mode, stdout);
			file_print("\n", stdout);
		}

		/* Perform the chmod */
		chmod(files[i], omode);

		free(files[i]);
	}

	free(mode);
	free(files);
}
