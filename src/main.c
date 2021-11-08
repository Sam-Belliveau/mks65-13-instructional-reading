#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "populations.h"

#define CSV_FILE "./nyc_pop.csv"

int main(int argc, char** argv)
{
    char user_input[2048];

    clear_entries();
    load_entries(CSV_FILE);
    printf("Loaded Entries From \"" CSV_FILE "\"!\n\n");

    printf("Type \"help\" for list of commands.");

    while(1)
    {
        memset(user_input, 0, sizeof(user_input));

        printf("\npop-shell> ");
        scanf("%2040s", user_input);

        switch(user_input[0])
        {
        case 'h': case 'H':
            tprintf(0, "List of Commands:\n");

            tprintf(1, "help\n");
            tprintf(2, "Lists all of the available commands\n");

            tprintf(1, "clear\n");
            tprintf(2, "Clears all of the loaded entries\n");

            tprintf(1, "load\n");
            tprintf(2, "Loads all of the entries from the csv\n");

            tprintf(1, "table\n");
            tprintf(2, "Shows all of the loaded entries with their index\n");

            tprintf(1, "add\n");
            tprintf(2, "Allows you to add data to list of entries\n");

            tprintf(1, "get\n");
            tprintf(2, "Allows you to get data from an index\n");

            tprintf(1, "set\n");
            tprintf(2, "Allows you to set/update data by index\n");

            tprintf(1, "quit\n");
            tprintf(2, "Ends the Program\n");
            break;

        case 'c': case 'C':
            clear_entries();
            printf("Data Cleared!\n");
            break;

        case 'l': case 'L':
            load_entries(CSV_FILE);
            printf("Loaded Entries From \"" CSV_FILE "\"!\n");
            break;

        case 't': case 'T':
            print_entries();
            break;

        case 'a': case 'A':
            user_save_entry();
            break;

        case 'g': case 'G':
            user_get_data();
            break;

        case 's': case 'S':
            user_set_data();
            break;

        case 'q': case 'Q':
            printf("Goodbye!\n");
            return 0;

        default:
            printf("Unknown Command: %s\n", user_input);
        }
    }
}