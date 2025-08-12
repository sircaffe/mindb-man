#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    uint8_t flags;
    size_t count;
    void *items;
} Chunk;

typedef struct {
    Chunk *chunks;
    size_t count;
} Database;

#define TEMP_CHUNKS 2

enum Type {
    INT = 0,
    CHAR,
    COUNT
};

void dump_database(Database db) {
    printf("--- BEGIN DUMP ---\n");
    for (size_t i = 0; i < db.count; ++i) {
        for (size_t j = 0; j < db.chunks->count; ++j) {
            int item = *((int *)(db.chunks[i].items + j));
            
            printf("%d | ", item);
        }
    }
    printf("\n--- END DUMP ---\n");
}

bool write_database_to_file(Database db, char *path) {
    FILE *f = fopen(path, "wb");
    if(f == NULL) return false;

    fwrite(db.chunks, sizeof(Chunk), db.count, f);
    
    fclose(f);
    return true;
}

int main(void) {
    Database db = {0};

    // temporario
    db.chunks = malloc(sizeof(Chunk) * TEMP_CHUNKS);
    db.count = TEMP_CHUNKS;
    
    for (size_t i = 0; i <= db.count; ++i) {
        db.chunks[i].items = malloc(sizeof(void*));
        db.chunks[i].count = 1;
        db.chunks[i].flags = 65;
    }

    int it = 66;
    db.chunks[0].items = &it;
    db.chunks[1].items = &it;
    
    dump_database(db);
    char *save_path = "db.bin";
    if (!write_database_to_file(db, save_path)) {
        fprintf(stderr, "ERROR: Could not write database to %s\n", save_path);
    }
    return 0;
}
