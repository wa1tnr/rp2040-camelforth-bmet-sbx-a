#include "resets.h"
#include "gpio.h"
#include "uart.h"
#include "nvic.h"
#include "xosc.h"
#include "lib.h"
#include "systick.h"


//TODO WFE, WFI, SEV
//TODO PSR, PRIMASK, CONTROL

//TODO continue at 2.6.3.1, sys.
// sio.c sio.h rp2040.h OK

#define UNIQUE() printf(" UNIQ 351ca-94c");

uint handled_isr = 0; // serial read UART flag
extern int master_char_count;
extern void nop(void);

void delay(int t) {
	while (t--)
		asm volatile ("nop");
}

char* ch_read;

int uart_handler() {
	char buff[1];
	memset(buff, 0, 1);

        int length = uart_read(0, buff, 1);

        if (length == 1) {
            master_char_count++;
            // strncpy(ch_read, buff, 1);
            ch_read = buff;

/*
            printf("%c", ' ');
            printf("%c", 'Y');
            printf("%c", 'e');
            printf("%c", 's');
            printf("%c", ' ');
            printf("%c", ' ');

*/


            int address_wanted_ch_read = 0;
            int extra_prints_here = 0;

            // printf("%d", master_char_count);

            if (extra_prints_here) {
                if(address_wanted_ch_read) {
                    printf("%d", ch_read);
                }
                printf("%c", ' ');
                printf("%d", master_char_count);
                printf("%c", ' ');
                if(master_char_count != -411575039) {
                    printf("%c", ' ');
                    printf("%c", '!');
                    printf("%c", buff[0]);
                    printf("%c", '!');
                    printf("%c", ' ');
                }
            }
                // printf("%c", '*');
                if(master_char_count != -411575039) {
                    nop(); // printf("%c", *ch_read);
                }
                // printf("%c", '*');

/*
            printf("%c", ' ');

            printf("%c", ' ');
*/

            return 1;
        } else {
            return 0;
        }
}

// if(master_char_count != -411575039) { printf("%c", *ch_read); }
char getKey(void) {
    char* ch_returned;
/*
    int loop_count = 0;
    int loop_two_count = 0;
*/
    // printf("q");
    // printf("%c", 'q');
    // delay(9000);
    // delay(900);
    for(;;) {
        int handled_chars = uart_handler();
/*
        loop_count++;
        // if (loop_count == 955555555) {
        if (loop_count == 9555) {
            loop_count = 0;
            loop_two_count++;
            if (loop_two_count == 955) {
                printf("%c", 'W');
                loop_two_count = 0;
            }
        }
*/
        if (handled_chars == 1) {
            // printf("%c", 'G');
            ch_returned = '\0'; // ch_read;
            ch_returned = ch_read;
            char ch_found = *ch_returned;
            // printf("%c", ch_found);
            return ch_found;
        }
    }
}

void signon_msg() {
        UNIQUE();
        printf("             ");
        printf("\r\n\r\n    proto-Forth\r\n\r\n");
        printf("    This signon message is ");
        printf("what the cat dragged in.\r\n");
        // model is: // printf("systick\r\n");
}

extern void interpreter_inner(void);

void interpret(void) {
        interpreter_inner();
        // printf("Interpret-AA");
        // delay(2500000);
        // delay(50);
}

void init() {
	xosc_init();
	IO_WR(CLOCKS_BASE+0x48, 0x880); // clk_peri enble

	reset_release_wait(RESET_IO_BANK0);
	reset_release_wait(RESET_PADS_BANK0);
	reset_release_wait(RESET_UART0);

	uart_init(0, 115200);
	gpio_init(0, GPIO_FUNC_UART);
	gpio_init(1, GPIO_FUNC_UART);
	gpio_dir(0, 1);

	nvic_init();
	// uart_intr_enable(0, 1, 0);
	// nvic_register_irq(IRQ_UART0, handler);
	// nvic_enable(IRQ_UART0);

	systick_init();
	systick_set(12000000-1);

        // for (int index = 22; index > 0; index--) {
            signon_msg();
        // }
	while(1){
            interpret();
	    delay(50);
		// printf("M\r\n");
		// delay(5000000);
	}
}


/*

#define SYS_CSR    (PPB_BASE + 0xE010)
#define SYS_RVR    (PPB_BASE + 0xE014)
#define SYS_CVR    (PPB_BASE + 0xE018)
#define SYS_CALIB  (PPB_BASE + 0xE01C)
#define NVIC_ISER   (PPB_BASE + 0xE100)
#define NVIC_ICER   (PPB_BASE + 0xE180)
#define NVIC_ISPR   (PPB_BASE + 0xE200)
#define NVIC_ICPR   (PPB_BASE + 0xE280)


reg_wr(CLOCKS_BASE+0x78, 0);

reg_wr(XOSC_BASE+0x00, 0xaa0);
reg_wr(XOSC_BASE+0x0c, 47);
reg_wr(XOSC_BASE+0x00+0x2000, 0xfab000);

while(1) {
	if (((reg_rd(XOSC_BASE+0x04)) & 0x80000000)!=0){
		break;
	}
}

reg_wr(CLOCKS_BASE+0x30, 2);
reg_wr(CLOCKS_BASE+0x3c, 0);
reg_wr(CLOCKS_BASE+0x48, 0x880);

reset_release_wait(RESET_IO_BANK0);
reset_release_wait(RESET_PADS_BANK0);
reset_release_wait(RESET_UART0);


uart_init();

reg_wr(SYS_CSR, 4);
reg_wr(SYS_RVR, 12000000-1);
reg_wr(SYS_CVR, 12000000-1);
reg_wr(SYS_CSR, 7);

reg_wr(NVIC_ISER, 1 << 13);

asm volatile("cpsie i");

gpio_init(0, GPIO_FUNC_UART);
gpio_init(1, GPIO_FUNC_UART);

*/


/*
gpio irq


void irq13() {
	gpio_set(20, 0);
	delay(1000000);
	gpio_set(20, 1);
	gpio_int_ack(7);
}


	gpio_init(18, GPIO_FUNC_SIO);
	gpio_init(19, GPIO_FUNC_SIO);
	gpio_init(20, GPIO_FUNC_SIO);
	gpio_dir(18, 1);
	gpio_dir(19, 1);
	gpio_dir(20, 1);

	gpio_init(0, GPIO_FUNC_SIO);
	gpio_init(1, GPIO_FUNC_SIO);
	gpio_init(2, GPIO_FUNC_SIO);
	gpio_pullup(0, 1);
	gpio_pullup(1, 1);
	gpio_pullup(2, 1);

	gpio_init(7, GPIO_FUNC_SIO);
	gpio_pullup(7, 1);
	delay(10000);
	gpio_int_set(7, 1, GPIO_INT_EDGE_FALL);
	nvic_init();
	nvic_register_irq(13, irq13);
	nvic_enable(13);

	gpio_set(19, 1);
	gpio_set(20, 1);
	*/
