#ifndef SAM_BELLIVEAU_POP_CSV_READER
#define SAM_BELLIVEAU_POP_CSV_READER

#include <stdint.h>

#define ENTRY_DIR "./.data/"
#define GET_ENTRY_FMT_PATH(entry) ENTRY_DIR "%016llux.bin", pop_entry_get_code(entry)

#define tprintf(n, args...) do{for(int TPFL=(n);TPFL>1;--TPFL){printf("│   ");}if(n){printf("├───");}printf(args);}while(0)

struct pop_entry {
    int year;
    int population;
    char boro[32];
};

uint64_t pop_entry_get_code(struct pop_entry* entry);

void pop_entry_print(struct pop_entry* entry);
void pop_entry_print_full(struct pop_entry* entry);

int pop_entry_save(struct pop_entry* entry);

void clear_entries();
void load_entries(char* file);

void print_entries();

struct pop_entry get_entry(int boro, int year);
void set_entry(int boro, int year, struct pop_entry* entry);

struct pop_entry user_create_entry();

void user_save_entry();

void user_get_data();
void user_set_data();

#endif