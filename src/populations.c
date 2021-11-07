#include "populations.h"

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * Hashing algorithm based off of XOR64Hash provided here:
 * https://github.com/Sam-Belliveau/JavaStringHash/blob/main/XOR64Hash.java
 */
uint64_t pop_entry_get_code(struct pop_entry* entry)
{
    const static uint64_t INITIAL_SEED = 0xc4ceb9fe1a85ec53L;
    const static uint64_t INPUT_MULT = 0xff51afd7ed558ccdL;
    
    int i;
    uint64_t pool = INITIAL_SEED;

    for(i = 0; i < sizeof(entry->boro); ++i)
    {
        pool ^= pool << 13;
        pool ^= pool >> 7;
        pool ^= pool << 17;
        pool ^=((uint64_t) entry->boro[i]) * INPUT_MULT;
    }
    
    return pool;
}


void pop_entry_print(struct pop_entry* entry)
{
    printf("Y-%d: %d", entry->year, entry->population);
}

void pop_entry_print_full(struct pop_entry* entry)
{
    tprintf(0, "Boro: %s\n", entry->boro);
    tprintf(1, "Year: %d\n", entry->year);
    tprintf(1, "Population: %d\n", entry->population);
}

int pop_entry_save(struct pop_entry* entry)
{
    mkdir(ENTRY_DIR, 0777);

    if (entry)
    {
        char file_path[256];
        sprintf(file_path, GET_ENTRY_FMT_PATH(entry));

        int cached_file = open(file_path, O_WRONLY | O_CREAT | O_APPEND, 0777);

        if(
            (cached_file < 0) || 
            (write(cached_file, entry, sizeof(*entry)) != sizeof(*entry))
        ) { 
            printf("Unable to cache \"%s\" [%d: %s]\n", file_path, errno, strerror(errno));
            return 1;
        }

        close(cached_file);

        return 0;
    }

    return 1;
}

void read_csv(char* file)
{
    int year;
    struct pop_entry 
        m={0,0,"Manhattan"},
        bk={0,0,"Brooklyn"},
        q={0,0,"Queens"},
        br={0,0,"Bronx"},
        s={0,0,"Staten Island"};
    FILE* pops = fopen(file, "r");

    if(
        (pops) && 
        (fscanf(pops, "Year,Manhattan,Brooklyn,Queens,Bronx,Staten Island\n") == 0)
    ) {
        while(fscanf(pops, "%d,%d,%d,%d,%d,%d\n", &year, &m.population, &bk.population, &q.population, &br.population, &s.population) == 6)
        {
            m.year = bk.year = q.year = br.year = s.year = year;
            
            pop_entry_save(&m);
            pop_entry_save(&bk);
            pop_entry_save(&q);
            pop_entry_save(&br);
            pop_entry_save(&s);
        }
    }
}

void clear_data()
{
    system("rm -r " ENTRY_DIR);
}

void read_data()
{
    DIR* d;
    struct dirent* file_entry;
    int b_i = 0, y_i = 0;
    int file;
    struct pop_entry entry;

    if(!(d = opendir(ENTRY_DIR)))
    {
        printf("Error Reading " ENTRY_DIR " [%d: %s]", errno, strerror(errno));
        return;
    }

    tprintf(0, "Population Entries:\n");

    chdir(ENTRY_DIR);

    while((file_entry = readdir(d)))
    {
        if(file_entry->d_type == DT_REG)
        {
            file = open(file_entry->d_name, O_RDONLY);
            
            if(file < 0)
            {
                tprintf(1, "[%d] Error Reading %s [%d: %s]\n", b_i, file_entry->d_name, errno, strerror(errno));
            }

            else if(read(file, &entry, sizeof(entry)) == sizeof(entry))
            {
                y_i = 0;
                tprintf(1, "[%d] %s\n", b_i, entry.boro);

                do {
                    tprintf(2, "[%d/%d] ", b_i, y_i); 
                    pop_entry_print(&entry);
                    printf("\n");
                    y_i += 1;
                } while(read(file, &entry, sizeof(entry)) == sizeof(entry));
            }

            b_i += 1;
            close(file);
        }
    }

    chdir("..");
    closedir(d);
}

struct pop_entry get_user_entry()
{
    struct pop_entry user_entry;

    tprintf(0, "\nCreate New Entry:\n");

    tprintf(1, "Boro: ");
    scanf("%31s", user_entry.boro);

    tprintf(2, "Year: ");
    scanf("%d", &user_entry.year);

    tprintf(2, "Population: ");
    scanf("%d", &user_entry.population);

    return user_entry;
}

void add_data()
{
    struct pop_entry user_entry = get_user_entry();

    if(!pop_entry_save(&user_entry))
    {
        printf("\nCreated Entry For:\n");
        pop_entry_print_full(&user_entry);
    } else {
        printf("\nUnable To Create User Entry!\n");
    }
}

int get_file_desc(int boro, int year, int opt)
{
    DIR* d;
    struct dirent* file_entry;
    int file = -1;

    if(!(d = opendir(ENTRY_DIR)))
    {
        printf("Error Reading " ENTRY_DIR " [%d: %s]", errno, strerror(errno));
        return file;
    }

    chdir(ENTRY_DIR);

    while((file_entry = readdir(d)))
    {
        if(file_entry->d_type == DT_REG && !(boro--))
        {
            file = open(file_entry->d_name, opt);
            
            if(file < 0) printf("Error Opening %s [%d: %s]\n", file_entry->d_name, errno, strerror(errno));
            else lseek(file, year * sizeof(struct pop_entry), SEEK_SET);
            
            break;
        }
    }

    chdir("..");
    closedir(d);

    return file;
}

struct pop_entry get_data(int boro, int year)
{    
    int file;
    struct pop_entry entry = {};
    
    file = get_file_desc(boro, year, O_RDONLY);

    if(file > 0) 
    {
        read(file, &entry, sizeof(entry));
        close(file);
    }

    return entry;
}

void set_data(int boro, int year, struct pop_entry* entry)
{    
    int file;
    
    file = get_file_desc(boro, year, O_WRONLY);

    if(file > 0) 
    {
        write(file, entry, sizeof(*entry));
        close(file);
    }
}

void user_get_data()
{
    int b, n;
    struct pop_entry entry;

    tprintf(0, "Enter Entry Index in the format \"B/N\":\n");
    tprintf(1, "Index: ");
    
    if(scanf("%d/%d", &b, &n) == 2)
    {
        entry = get_data(b, n);

        printf("\nRequsted Data:\n");
        pop_entry_print_full(&entry);
        printf("\n");
    } else
    {
        tprintf(2, "Invalid Index... No Data Returned\n");
    }
}

void user_set_data()
{
    int b, n;
    struct pop_entry entry;

    tprintf(0, "Enter Entry Index in the format \"B/N\":\n");
    tprintf(1, "Index: ");
    
    if(scanf("%d/%d", &b, &n) == 2)
    {
        entry = get_data(b, n);

        printf("\nOld Entry:\n");
        pop_entry_print_full(&entry);

        entry = get_user_entry();
        set_data(b, n, &entry);
    } else
    {
        tprintf(2, "Invalid Index... No Data Returned\n");
    }
}
