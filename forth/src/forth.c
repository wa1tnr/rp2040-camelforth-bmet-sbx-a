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
#include "nvic.h"
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

// 155 CODE(lit) {
CODE(lit) {
    *--psp = *(unsigned int*)ip;     /* fetch inline value */
    ip += CELL;
    xxip = pfa;                   /* fake out the compiler - KLUDGE */
}

// 174 CODE(drop) {
CODE(drop) {
    psp++;
    xxip = pfa;                   /* fake out the compiler - KLUDGE */
}

// 510 CODE(cmove) {   /* src dst u -- */
CODE(cmove) {   /* src dst u -- */
    unsigned char *dst, *src;
    unsigned int u;
    u = *psp++;
    dst = (unsigned char *)*psp++;
    src = (unsigned char *)*psp++;
    while (u-- > 0) *dst++ = *src++;
    xxip = pfa;                   /* fake out the compiler - KLUDGE */
}

// 408
/* BRANCH AND LOOP */

CODE(branch) {     /* Tbranch,-4  loops back to itself */
    int offset;                 /* Tbranch,+4  is a no-op */
    offset = *(unsigned int*)ip;     /* fetch inline offset */
    ip += offset;
    xxip = pfa;                   /* fake out the compiler - KLUDGE */
}

// 416

/*
 * // 620 HIGH LEVEL WORD DEFINITIONS
 */

// 623 PRIMITIVE(exit);
PRIMITIVE(exit);
// 625 PRIMITIVE(lit);
PRIMITIVE(lit);
// 628 PRIMITIVE(drop);
PRIMITIVE(drop);
// 684 PRIMITIVE(branch);
PRIMITIVE(branch);
// 695 PRIMITIVE(cmove);
PRIMITIVE(cmove);

// 697 THREAD(itod) = { Fcmove };  /* synonym */
THREAD(itod) = { Fcmove };  /* synonym */

//  716 THREAD(u0) = { Fdouser, LIT(0) };
THREAD(u0) = { Fdouser, LIT(0) };

//  728 extern const struct Header Hcold;
extern const struct Header Hcold;

//  730 THREAD(uinit) = { Fdorom,

THREAD(uinit) = { Fdorom,
    LIT(0),  LIT(0),  LIT(10), LIT(0),  // u0 >in base state
    RAMDICT, LIT(0),  LIT(0),  Hcold.nfa,    // dp source latest
    LIT(0),  LIT(0),  ROMDICT, LIT(0) };  // hp lp idp newest
THREAD(ninit) = { Fdocon, LIT(16*CELL) };

// 869  THREAD(count) = { Fenter, Tdup, Tcharplus, Tswap, Tcfetch, Texit };
// 872 THREAD(space) = { Fenter, Tlit, LIT(0x20), Temit, Texit };
THREAD(space) = { Fenter, Tlit, LIT(0x20), // Temit,
        Tdrop, Texit };

// 1017 extern const void * Tabort[];   /* forward reference */
extern const void * Tabort[];   /* forward reference */

// 1042 THREAD(quit) = { Fenter, Tl0, Tlp, Tstore,
THREAD(quit) = { Fenter, // Tl0, Tlp, Tstore,
        Tspace,
        // Tr0, Trpstore, Tzero, Tstate, Tstore,
 /*1*/  // Ttib, Tdup, Ttibsize, Taccept, Tspace, Tinterpret,
        // Tcr, Tstate, Tfetch, Tzeroequal, Tqbranch, OFFSET(5 /*2*/),
        // Tlit, okprompt, Ticount, Titype,
 /*2*/  Tbranch, OFFSET(-2 /*1*/) };     // never exits

// 1049 THREAD(abort) = { Fenter, Ts0, Tspstore, Tquit };
THREAD(abort) = { Fenter, // Ts0, Tspstore,
                  Tquit };

const char coldprompt[] = "\042CamelForth in C v0.1 - 14 Feb 2016";

// 1207 THREAD(cold) = { Fenter,
// THREAD(cold) = { Fenter, Texit, };
THREAD(cold) = { Fenter, // Texit, };
    // Tuinit, Tu0, Tninit, Titod,     /* important initialization! */
    // Tlit, coldprompt, Tcount, Ttype, Tcr,
    Tabort, };                      /* Tabort never exits */

/*
 * INNER INTERPRETER
 */

extern char getKey(void);
extern uint handled_isr; //  = 0; // serial read UART flag


void nop(void) {
    asm volatile ("nop");
}


int master_char_count = 0;

void interpreter_inner(void) //  'void interpreter(void)' in upstream - restore that later
{
    char ch = '\0';
    int testing = 0;
    delay(900);
    ch = getKey();
    if (ch == 27) { printf(" ESCape! pressed "); }

    // the first char is to be rejected after powering on.
    if(master_char_count != -411575039) {
        printf("%c",  ch);
    /*
        printf("%c", ' ');
        printf("%c", '%');

        printf("%c", '\'');
        printf("%c",  ch);
        printf("%c", '\'');

        printf("%c", '~');
        printf("%d",  ch); // 'A' prints as '65' and 'B' as '66' here.
        printf("%c", '~');

        printf("%c", '%');
        printf("%c", ' ');
    */
    }
    //  if (1) { printf("%c", ch); }

    handled_isr = 0; // stop assertion it is read
    // getKey();
    if (testing) {
        printf("r"); // getKey() returned

        printf("%c", '>');
        if (0) { printf("%c", ch); }
    // ch = '7';
    }
    if (testing) {
        printf("%c", ch);
        printf("%c", '<');
        printf("%c", 'P');

        printf("t");
    }

/*
    void (*xt)(void *);     // pointer to code function
    void *w, *x;            // generic pointers

    psp = &pstack[PSTACKSIZE-1];
    rsp = &rstack[RSTACKSIZE-1];
    ip = &Tcold;
    ip += CELL;
    run = 1;                // set to zero to terminate interpreter
    delay(1500000);
    printf("last good ghost"); delay(500000);
    while (run) {
        printf("yyy"); delay(500000);
        w = *(void **)ip;       // fetch word address from thread
        ip += CELL; // enough to cause malfunction in this bare-metal version at this stage of development
        x = *(void **)w;        // fetch function adrs from word def
        xt = (void (*)())x;     // too much casting!
        w += CELL;
        (*xt)(w);               // call function w/adrs of word def
    }
*/
}



//  ////////////////




#define NULL 0 // KLUDGE
const struct Header Hexit = {  NULL, Texit, 0, "\004EXIT"  };
// 1244 HEADER(lit, execute, 0, "\003lit");
HEADER(lit, exit, 0, "\003lit");
// 1247 HEADER(drop, qdup, 0, "\004DROP");
HEADER(drop, lit, 0, "\004DROP");
// 1402 HEADER(space, cr, 0, "\005SPACE");
HEADER(space, drop, 0, "\005SPACE");
// 1439 HEADER(quit, evaluate, 0, "\004QUIT");
HEADER(quit, space, 0, "\004QUIT");
// 1440 HEADER(abort, quit, 0, "\005ABORT");
HEADER(abort, quit, 0, "\005ABORT");
// 1493 HEADER(cold, words, 0, "\004COLD");
HEADER(cold, abort, 0, "\004COLD");
// END.
