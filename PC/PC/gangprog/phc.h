// PHC protocol definitions

#ifndef _PHC_H

// first byte for classes
#define INPUT_BASE  0x00
#define OUTPUT_BASE 0x40
#define DIMMER_BASE 0xA0


// bootloader command definitions, repeated from the uC header file

#define CMD_ACK          0
#define CMD_PING         1
#define CMD_REBOOT       2
#define CMD_EEPROM_WRITE 3
#define CMD_LOAD_PAGE    4
#define CMD_FLASH_PAGE   5
#define CMD_EEPROM_READ  6
#define CMD_FLASH_READ   7

#define BLOCKSIZE        16 // how many bytes to transfer in CMD_LOAD_PAGE

#define PAGESIZE 64 // size of a flash page, in bytes


#endif // #ifndef _PHC_H
