#include "ata.h"
#include "filesystem.h"

void filesystem_init() {
    base_block = read_disk(BASE_BLOCK_ADDRESS);
}
