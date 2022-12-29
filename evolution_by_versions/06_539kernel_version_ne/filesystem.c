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

char **list_files() {
    if (base_block->head == 0) {
        return -1;
    }

    char **list;
    list = kalloc(get_files_number() * sizeof(char *));

    metadata_t *curr_file = load_metadata(base_block->head);
    int idx = 0;

    while (1) {
        list[idx] = curr_file->filename;

        if (curr_file->next_file_address == 0) {
            break;
        }

        curr_file = load_metadata(curr_file->next_file_address);
        idx++;
    }

    return list;
}

int get_files_number() {
    if (base_block->head == 0) {
        return 0;
    }

    int files_number = 1;
    metadata_t *curr_file = load_metadata(base_block->head);

    while (1) {
        files_number++;

        if (curr_file->next_file_address == 0) {
            break;
        }

        curr_file = load_metadata(curr_file->next_file_address);
    }

    return files_number;
}
