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

// #include "resets.h"
// #include "gpio.h"
// #include "uart.h"
// #include "nvic.h"
// #include "xosc.h"
#include "lib.h"
// #include "systick.h"

extern void delay(int); // init.c

/*
 * DATA STACKS
 * stacks grow downward to allow positive index from psp,rsp
 */

unsigned int pstack[PSTACKSIZE];    /* grows down from end */
unsigned int rstack[RSTACKSIZE];    /* grows down from end */
unsigned int *psp, *rsp;            /* stack pointers */
void *ip, *xxip;                    /* interpreter pointer */
bool run;                           /* "run" flag */

unsigned int lstack[LSTACKSIZE];    /* grows down from end */
unsigned int uservars[USERSIZE];
unsigned char tibarea[TIBSIZE];
unsigned char padarea[PADSIZE];
unsigned char holdarea[HOLDSIZE];

/* STUBS TBD */
unsigned char RAMDICT[8192];
unsigned char ROMDICT[1024];

void Fdocon (void * pfa) {
    *--psp = *(unsigned int *)pfa;
}

void Fdorom (void * pfa) {
    *--psp = (unsigned int)pfa;
}

// int pjpjp = 0;

void Fenter (void * pfa) {
    *--rsp = (unsigned int)ip;      /* push old IP on return stack */
    ip = pfa;                       /* IP points to thread */
}

void Fdouser (void * pfa) {
    unsigned int i;
    i = *(unsigned int *)pfa;               /* pf holds user var index */
    *--psp = (unsigned int)(&uservars[i]);  /* stack adrs of user var */
}

CODE(exit) {
    ip = (void *)(*rsp++);        /* pop IP from return stack */
    xxip = pfa;                   /* fake out the compiler - KLUDGE */
}

PRIMITIVE(exit);

THREAD(u0) = { Fdouser, LIT(0) };

extern const struct Header Hcold;

THREAD(uinit) = { Fdorom,
    LIT(0),  LIT(0),  LIT(10), LIT(0),  // u0 >in base state
    RAMDICT, LIT(0),  LIT(0),  Hcold.nfa,    // dp source latest
    LIT(0),  LIT(0),  ROMDICT, LIT(0) };  // hp lp idp newest
THREAD(ninit) = { Fdocon, LIT(16*CELL) };

THREAD(cold) = { Fenter, Texit, };



// interpreter here:

/*
 * INNER INTERPRETER
 */

void interpreter_inner(void) //  'void interpreter(void)' in upstream - restore that later
{
    void (*xt)(void *);     /* pointer to code function */
    void *w, *x;            /* generic pointers */

    psp = &pstack[PSTACKSIZE-1];
    rsp = &rstack[RSTACKSIZE-1];
    ip = &Tcold;
    ip += CELL;
    run = 1;                /* set to zero to terminate interpreter */
    while (run) {
        w = *(void **)ip;       /* fetch word address from thread */
        ip += CELL;
        x = *(void **)w;        /* fetch function adrs from word def */
        xt = (void (*)())x;     /* too much casting! */
        w += CELL;
        (*xt)(w);               /* call function w/adrs of word def */
        printf("yyy");
        delay(50000);
    }
}



//  ////////////////




#define NULL 0 // KLUDGE
const struct Header Hexit = {  NULL, Texit, 0, "\004EXIT"  };

HEADER(cold, exit, 0, "\004COLD");
// END.
