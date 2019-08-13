/*
Copyright (c) 2019, Germán Luis Aracil Boned
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "fthash.h"

#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))

void *keyptr[1000000];
void *valueptr[1000000];

void *rand_string(char *str, size_t size)
{
    const char charset[] = "0123456789abcdef";
    unsigned char index;

    while (size-- > 0)
    {
        index = (double)rand() / RAND_MAX * (sizeof(charset) - 1);
        *str = charset[index];
        str++;
    }
    *str = '\0';
}

int main()
{
    uint64_t c;
    uint16_t i;
    uint8_t r;
    HASHTABLE_T table1;
    char *getvalue;
    char flag;
    _LPTR_T *t;
    clock_t begin;
    clock_t end;
    double time_spent = 0.0;
    double time_spent2 = 0.0;
    uint32_t index;

    HASH_TABLE_INIT(&table1, 24, r);

    printf("INITIAL DATA\nTable structure size %lu bytes\nIndex %i bits, size in memory %lu bytes\nBuckets %i, size in memory %lu bytes\nTotal hash collisions %i\n",
           sizeof(table1),
           table1.index_size,
           table1.itsize,
           table1.buckets,
           table1.btsize,
           table1.index_collisions);

    printf("Seting up pointers to random data...\n");

    for (c = 0; c < 1000000; c++)
    {
        valueptr[c] = malloc(9);
        keyptr[c] = malloc(50);
    }

    while (1)
    {

        puts("Inserting 1000000 of records...");

        for (c = 0; c < 1000000; c++)
        {
            sprintf(valueptr[c], "%lu", (int64_t)((double)rand() / RAND_MAX * 1000000));
            rand_string(keyptr[c], 32);

            begin = clock();
            HASH_TABLE_ADD(&table1, (char *)keyptr[c], (char *)valueptr[c], (char)c, r);
            end = clock();

            time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

            if ((c % 100000) == 0)
                printf("(%6lu) INSERT Key %s Value %s\n", c, (char *)keyptr[c], (char *)valueptr[c]);
        }

        printf("INSERTED 1000000 records\nTable structure size %lu bytes\nIndex %i bits, size in memory %lu bytes\nBuckets %i, size in memory %lu bytes\nTotal hash collisions %i\n",
               sizeof(table1),
               table1.index_size,
               table1.itsize,
               table1.buckets,
               table1.btsize,
               table1.index_collisions);

        printf("Total time in seconds %f, by record %0.12f micro seconds\n", time_spent, time_spent);
        puts("Get 1000000 random records...");

        for (c = 0; c < 1000000; c++)
        {
            index = (double)rand() / RAND_MAX * 1000000;

            begin = clock();
            HASH_TABLE_GET(&table1, (char *)keyptr[index], getvalue, flag);
            end = clock();

            HASH_TABLE_SET_FLAG(&table1, (char*)keyptr[index], 1);

            if ((c % 100000) == 0)
                printf("(%6lu) READ Key %s Value %s\n", c, (char *)keyptr[index], getvalue);

            time_spent2 += (double)(end - begin) / CLOCKS_PER_SEC;
        }

        printf("Get 1000000 of random records time in seconds %f, by record %0.12f micro seconds\n", time_spent2, time_spent2 / 10);

        for (c = 0; c < 1000000; c++)
        {
            HASH_TABLE_ERA(&table1, keyptr[c], r);
        }

        printf("ERASE DATA\nTable structure size %lu bytes\nIndex %i bits, size in memory %lu bytes\nBuckets %i, size in memory %lu bytes\nTotal hash collisions %i\n\n",
               sizeof(table1),
               table1.index_size,
               table1.itsize,
               table1.buckets,
               table1.btsize,
               table1.index_collisions);

        time_spent = 0.0;
        time_spent2 = 0.0;
    }

    exit(0);
}
