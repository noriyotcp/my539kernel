#include "ata.h"
#include "heap.h"

extern char dev_read(int port);
extern void dev_write(int port, int cmd);
extern void dev_write_word(int port, unsigned int cmd);

void wait_drive_until_ready() {
    int status = 0;

    do {
        status = dev_read(BASE_PORT + 7);
    } while ((status ^ 0x80) == 128);
}

void *read_disk_chs(int sector) {
    dev_write(BASE_PORT + 6, 0x0a0);
    dev_write(BASE_PORT + 2, 1);
    dev_write(BASE_PORT + 3, sector);
    dev_write(BASE_PORT + 4, 0);
    dev_write(BASE_PORT + 5, 0);
    dev_write(BASE_PORT + 7, 0x20);

    wait_drive_until_ready();

    short *buffer = kalloc(SECTOR_SIZE);

    for (int currByte = 0; currByte < (SECTOR_SIZE / 2); currByte++) {
        buffer[currByte] = dev_read(BASE_PORT);
    }

    return buffer;
}


void *read_disk(int address) {
    dev_write(BASE_PORT + 6, 0x0e0 | ((address & 0x0F000000 ) >> 24));
    dev_write(BASE_PORT + 2, 1);
    dev_write(BASE_PORT + 3, address & 0x000000FF);
    dev_write(BASE_PORT + 4, (address & 0x0000FF00) >> 8);
    dev_write(BASE_PORT + 5, (address & 0x00FF0000) >> 16);
    dev_write(BASE_PORT + 7, 0x20);

    wait_drive_until_ready();

    short *buffer = kalloc(SECTOR_SIZE);

    for (int currByte = 0; currByte < (SECTOR_SIZE / 2); currByte++) {
        buffer[currByte] = dev_read(BASE_PORT);
    }

    return buffer;
}
