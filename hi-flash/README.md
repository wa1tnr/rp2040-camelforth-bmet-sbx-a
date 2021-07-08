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


The basic idea: The uploaded UF2 will immediately write to flashROM (0x1003D000).

The connected CP2104 will report this conversation's commentary.

If power is lost, CP2104 reports nothing at all, when things are powered back on, since the uploaded UF2 is a SRAM only program (isn't written to flashROM at all, just SRAM).

If you previously uploaded some kind of monitor program (CamelForth was used, locally, for this job) then you can freely inspect what got written to flashROM (at address 0x1003D000).

 - - - -

 shortened sample run, CamelForth (flashROM dump):

1003CFE0: FF FF FF FF  FF FF FF FF   FF FF FF FF  FF FF FF FF  ................
1003CFF0: FF FF FF FF  FF FF FF FF   FF FF FF FF  FF FF FF FF  ................
ok  10 dump
1003D000: 41 42 43 44  45 46 47 48   49 4A 4B 4C  4D 4E 4F 50  ABCDEFGHIJKLMNOP
1003D010: 51 52 53 54  55 56 57 58   59 5A 5B 5C  5D 5E 5F 60  QRSTUVWXYZ[\]^.`
1003D020: 61 62 63 64  65 66 67 68   69 6A 6B 6C  6D 6E 6F 70  abcdefghijklmnop
1003D030: 71 72 73 74  75 76 77 78   79 7A 7B 7C  7D 7E 7F 80  qrstuvwxyz{|}~..
1003D040: 81 82 83 84  85 86 87 88   89 8A 8B 8C  8D 8E 8F 90  ................

Wed Jul  7 15:49:39 UTC 2021

END.
