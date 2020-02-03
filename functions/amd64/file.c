/* Copyright (C) 2016 Jeremiah Orians
 * This file is part of M2-Planet.
 *
 * M2-Planet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * M2-Planet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with M2-Planet.  If not, see <http://www.gnu.org/licenses/>.
 */

// CONSTANT stdin 0
// CONSTANT stdout 1
// CONSTANT stderr 2
// CONSTANT EOF 0xFFFFFFFF

int fgetc(FILE* f)
{
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %8"
	"LOAD_INTEGER_rdi"
	"LOAD_IMMEDIATE_rax %0"
	"PUSH_RAX"
	"LOAD_EFFECTIVE_ADDRESS_rsi %0"
	"LOAD_IMMEDIATE_rdx %1"
	"SYSCALL"
	"LOAD_IMMEDIATE_rbx %0"
	"CMP"
	"POP_RAX"
	"JUMP_NE %FUNCTION_fgetc_Done"
	"LOAD_IMMEDIATE_rax %-1"
	":FUNCTION_fgetc_Done");
}

void fputc(char s, FILE* f)
{
	asm("LOAD_IMMEDIATE_rax %1"
	"LOAD_EFFECTIVE_ADDRESS_rdi %8"
	"LOAD_INTEGER_rdi"
	"LOAD_EFFECTIVE_ADDRESS_rsi %16"
	"LOAD_IMMEDIATE_rdx %1"
	"SYSCALL");
}

/* Important values needed for open
 * O_RDONLY => 0
 * O_WRONLY => 1
 * O_RDWR => 2
 * O_CREAT => 64
 * O_TRUNC => 512
 * S_IRWXU => 00700
 * S_IXUSR => 00100
 * S_IWUSR => 00200
 * S_IRUSR => 00400
 */

FILE* open(char* name, int flag, int mode)
{
	asm("LOAD_EFFECTIVE_ADDRESS_rdi %24"
	"LOAD_INTEGER_rdi"
	"LOAD_EFFECTIVE_ADDRESS_rsi %16"
	"LOAD_INTEGER_rsi"
	"LOAD_EFFECTIVE_ADDRESS_rdx %8"
	"LOAD_INTEGER_rdx"
	"LOAD_IMMEDIATE_rax %2"
	"SYSCALL");
}

FILE* fopen(char* filename, char* mode)
{
	FILE* f;
	if('w' == mode[0])
	{ /* 577 is O_WRONLY|O_CREAT|O_TRUNC, 384 is 600 in octal */
		f = open(filename, 577 , 384);
	}
	else
	{ /* Everything else is a read */
		f = open(filename, 0, 0);
	}

	/* Negative numbers are error codes */
	if(0 > f)
	{
		return 0;
	}
	return f;
}
