#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DEFAULT_SIZE 16

typedef struct {
    uint8_t flags;
    uint8_t id;
    uint8_t size;
    uint8_t items[DEFAULT_SIZE];
} Chunk;

typedef struct {
    Chunk chunks[DEFAULT_SIZE];
    uint8_t count;
} Database;

enum Type {
    INT = 0,
    CHAR,
    COUNT
};

bool write_database_to_file(Database db, char *path) {
    FILE *f = fopen(path, "wb");
    if(f == NULL) return false;
    
    fwrite(&db, sizeof(Database), 1, f);
    
    fclose(f);
    return true;
}

bool read_database_from_file(Database *db, char *path) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) return false;

    fread(db, sizeof(Database), 1, f);

    for (int i = 0; i < db->count; ++i) {
        printf("Chunk %d:\n", i);
        printf("    Items: | ");
        for (int j = 0; j < db->chunks[i].size; ++j) {
            printf("%d | ", db->chunks[i].items[j]);
        }
        printf("\n");
        printf("    Size: %d\n", db->chunks[i].size);
        printf("\n");
    }
    fclose(f);
    return true;
}

int main(int argc, char **argv) {
    // TODO: Implement better flag recognition system
    if (argc < 3 || argc > 3) {
        fprintf(stderr, "ERROR: invalid arguments\n");
        fprintf(stderr, "Usage: %s [option] <file>\n", argv[0]);
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "    write - generates database binary\n");
        fprintf(stderr, "    read  - read from binary\n");
        exit(1);
    }

    char *file_path = argv[2];
    char *opt = argv[1];

    Database db = {0};
    
    if (strcmp(opt, "read") == 0) {
        if (!read_database_from_file(&db, file_path)) {
            fprintf(stderr, "ERROR: failed to read %s\n", file_path);
            exit(1);
        }
    } else if (strcmp(opt, "write") == 0) {
        // temporario
        db.count = DEFAULT_SIZE;
        
        for (size_t i = 0; i < db.count; ++i) {
            db.chunks[i].size = DEFAULT_SIZE;
            db.chunks[i].flags = 65;
        }

        if (!write_database_to_file(db, file_path)) {
            fprintf(stderr, "ERROR: Could not write database to %s\n", file_path);
        }
    }
    
    return 0;
}
