#include "populations.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void pop_entry_print(struct pop_entry* entry)
{
    printf("[year: %d, boro: %s, pop: %d]", entry->year, entry->boro, entry->population);
}

void pop_entry_cache(struct pop_entry* entry)
{
    mkdir(CACHE_DIR, 0777);

    if (entry)
    {
        char file_path[256];
        sprintf(file_path, CACHE_DIR "pop-%d-%s.dat", entry->year, entry->boro);

        int cached_file = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0777);

        if(cached_file<0) printf("Unable to cache \"%s\" (%d: %s)\n", file_path, errno, strerror(errno));
        else write(cached_file, entry, sizeof(*entry));

        close(cached_file);
    }
}

void read_csv(char* file)
{
    int year, manh, brok, quee, bron, stat;
    struct pop_entry 
        m={0,0,"Manhattan"},
        bk={0,0,"Brooklyn"},
        q={0,0,"Queens"},
        br={0,0,"Bronx"},
        s={0,0,"Staten Island"};
    FILE* pops = fopen(file, "r");

    if(pops)
    {
        fscanf(pops, "Year,Manhattan,Brooklyn,Queens,Bronx,Staten Island\n");

        while(fscanf(pops, "%d,%d,%d,%d,%d,%d\n", &year, &m.population, &bk.population, &q.population, &br.population, &s.population) == 6)
        {
            m.year = bk.year = q.year = br.year = s.year = year;
            
            pop_entry_cache(&m);
            pop_entry_cache(&bk);
            pop_entry_cache(&q);
            pop_entry_cache(&br);
            pop_entry_cache(&s);
        }
    }
}
