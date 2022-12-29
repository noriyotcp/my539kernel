#include "ata.h"
#include "filesystem.h"
#include "heap.h"

void filesystem_init() {
    base_block = read_disk(BASE_BLOCK_ADDRESS);
}

void create_file(char *filename, char *buffer) {
    int metadata_lba = (base_block->head == 0) ? BASE_BLOCK_ADDRESS + 1 : base_block->tail + 2;
    int file_lba = metadata_lba + 1;

    metadata_t *metadata = kalloc(sizeof(metadata_t));

    metadata->next_file_address = 0;

    int currIdx;

    for (currIdx = 0; *filename != '\0' && currIdx < FILENAME_LENGTH - 1; currIdx++, filename++) {
        metadata->filename[currIdx] = *filename;
    }

    metadata->filename[currIdx] = '\0';

    write_disk(metadata_lba, metadata);
    write_disk(file_lba, buffer);

    if (base_block->head == 0) {
        update_base_block(metadata_lba, metadata_lba);
    } else {
        metadata_t *tail_metadata = load_metadata(base_block->tail);

        tail_metadata->next_file_address = metadata_lba;

        write_disk(base_block->tail, tail_metadata);
        update_base_block(base_block->head, metadata_lba);
    }

}

void update_base_block(int new_head, int new_tail) {
    base_block->head = new_head;
    base_block->tail = new_tail;

    write_disk(BASE_BLOCK_ADDRESS, base_block);
}

metadata_t *load_metadata(int address) {
    metadata_t *metadata = read_disk(address);
    return metadata;
}
