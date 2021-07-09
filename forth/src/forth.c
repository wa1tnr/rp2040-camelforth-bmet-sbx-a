/****h* camelforth/forth.c
 * NAME
 *  forth.c
 * DESCRIPTION
 *  Interactive Forth interpreter written in C.
 *  Allows C routines to be executed, and data to be examined, 
 *  from a command line interface.
 * NOTES
 *  Ref. Norman E. Smith, "Write Your Own Programming Language
 *  Using C++", although the implementation here is entirely 
 *  different.
 * AUTHOR
 *  Brad Rodriguez
 * TODO
 *  split ROM and RAM space
 * HISTORY
 *  12 dec 2017 bjr - released as v0.1
 *  14 feb 2016 bjr - first implementation
 ******
 * LICENSE TERMS
 *  CamelForth in C 
 *  copyright (c) 2016,2017 Bradford J. Rodriguez.
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *  
 *  Commercial inquiries should be directed to the author at 
 *  115 First St., #105, Collingwood, Ontario L9Y 4W3 Canada
 *  or via email to bj@camelforth.com
 */

// #include <stdint.h>
#include <stdbool.h>
#include "forth.h"

/*
 * DATA STACKS
 * stacks grow downward to allow positive index from psp,rsp
 */

unsigned int pstack[PSTACKSIZE];    /* grows down from end */
unsigned int rstack[RSTACKSIZE];    /* grows down from end */
unsigned int *psp, *rsp;            /* stack pointers */
void *ip;                           /* interpreter pointer */
bool run;                           /* "run" flag */

unsigned int lstack[LSTACKSIZE];    /* grows down from end */
unsigned int uservars[USERSIZE];
unsigned char tibarea[TIBSIZE];
unsigned char padarea[PADSIZE];
unsigned char holdarea[HOLDSIZE];

/* STUBS TBD */
unsigned char RAMDICT[8192];
unsigned char ROMDICT[1024];

int pjpjp = 0;
