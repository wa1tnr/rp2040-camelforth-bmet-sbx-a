FLASH


```
void flash_init();
void flash_sector_read(uint addr, char *d);
void flash_sector_write(uint addr, char *d);
void flash_sector_erase(uint addr);
uint flash_busy();
```

Terse instructions de wa1tnr:

 * press and hold BOOTSEL to reprogram the RP2040 target board.
 * plug in Raspberry Pi Pico RP2040 via USB to host PC (Linux amd64 reference system)
 * release BOOTSEL - RPI_RP2 drive will appear (at OS level)

 * also plug in CP2104 Friend (Adafruit) .. or equivalent.
 * start minicom (or equivalent) on /dev/ttyUSB0 (the CP2104 bridge chip)
 * monitor RP2040 UART0 on CP2104 bridge connection (via USB).

In those conditions:

 * upload the relevant .UF2 built from this repository, to the RP2040 Pico board.

   The new firmware will run from SRAM, and print messages to UART0, which your CP2104 Friend (or equivalent) reports to your host PC, via a USB port and a telecomm program (minicom, seyon, microcom, picocom, putty, screen &c.)


The basic idea: The uploaded UF2 will immediately write to flashROM (0x10040000).

The connected CP2104 will report this conversation's commentary.

If power is lost, CP2104 reports nothing at all, when things are powered back on, since the uploaded UF2 is a SRAM only program (isn't written to flashROM at all, just SRAM).

If you previously uploaded some kind of monitor program (CamelForth was used, locally, for this job) then you can freely inspect what got written to flashROM (at address 0x10040000).

Wed Jul  7 12:52:31 UTC 2021

END.
