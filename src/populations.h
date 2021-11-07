#ifndef SAM_BELLIVEAU_POP_CSV_READER
#define SAM_BELLIVEAU_POP_CSV_READER

#define CACHE_DIR "./.cache/"

struct pop_entry {
    int year;
    int population;
    char boro[32];
};

void pop_entry_print(struct pop_entry* entry);

void pop_entry_cache(struct pop_entry* entry);

void read_csv(char* file);


#endif