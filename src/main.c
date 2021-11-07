#include <stdio.h>
#include "populations.h"

int main(int argc, char** argv)
{
    printf("Hello World!\n");

    struct pop_entry t ={123, 231, "sams_house"};

    read_csv("./nyc_pop.txt");
}