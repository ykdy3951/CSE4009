//2019044711
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include "cachelab.h"

typedef unsigned long long int mem_addr_t;

int verbosity = 0;
int s = 0;
int b = 0;
int E = 0;
char *trace_file = NULL;


unsigned hit_count = 0;
unsigned miss_count = 0;
unsigned eviction_count = 0;

struct Line
{
    char valid;
    mem_addr_t tag;
    unsigned LRUcounter;
};

struct Line ***cache;

void buildCache()
{
    cache = (struct Line ***)malloc(pow(2, s) * sizeof(struct Line **));
    for (int setIndex = 0; setIndex < pow(2, s); ++setIndex)
    {
        cache[setIndex] = (struct Line **)malloc(E * sizeof(struct Line *));
        for (int lineIndex = 0; lineIndex < E; ++lineIndex)
        {
            cache[setIndex][lineIndex] = (struct Line *)malloc(sizeof(struct Line));
            (cache[setIndex][lineIndex])->valid = 0;
            (cache[setIndex][lineIndex])->tag = 0;
            (cache[setIndex][lineIndex])->LRUcounter = 0;
        }
    }
}

void playTrace(char *trace_fn)
{
    char buf[1000];
    mem_addr_t addr = 0;
    unsigned int len = 0;
    FILE *trace_fp = fopen(trace_fn, "r");
    if (!trace_fp)
    {
        fprintf(stderr, "%s: %s\n", trace_fn, strerror(errno));
        exit(1);
    }
    mem_addr_t blockMask = pow(2, b) - 1;
    mem_addr_t setMask = pow(2, s) - 1;
    setMask <<= (b);
    mem_addr_t tagMask = ~(blockMask | setMask);
    while (fgets(buf, 1000, trace_fp) != NULL)
    {
        if (buf[1] == 'S' || buf[1] == 'L' || buf[1] == 'M')
        {
            sscanf(buf + 3, "%llx,%u", &addr, &len);
            if (verbosity)
                printf("%c %llx,%u\n", buf[1], addr, len);
            mem_addr_t setIndex = (setMask & addr) >> b;
            mem_addr_t tag = (tagMask & addr) >> (s + b);
            int numIterations = 1;
            switch (buf[1])
            {
            case 'M':
                numIterations = 2;
            case 'S':
            case 'L':
            {
                for (int times = 0; times < numIterations; ++times)
                {
                    char setIsFull = 1;
                    int emptyLineIndex = 0;
                    char hasHit = 0;
                    for (int lineIndex = 0; lineIndex < E; ++lineIndex)
                    {
                        if ((cache[setIndex][lineIndex])->valid)
                        {
                            if ((cache[setIndex][lineIndex])->tag == tag)
                            {
                                ++hit_count;
                                for (int i = 0; i < E; ++i)
                                    if ((cache[setIndex][i])->valid)
                                        ++((cache[setIndex][i])->LRUcounter);
                                (cache[setIndex][lineIndex])->LRUcounter = 0;
                                hasHit = 1;
                                break;
                            }
                        }
                        else
                        {
                            emptyLineIndex = lineIndex;
                            setIsFull = 0;
                        }
                    }
                    if (!hasHit)
                    {
                        ++miss_count;
                        if (!setIsFull)
                        {
                            (cache[setIndex][emptyLineIndex])->valid = 1;
                            (cache[setIndex][emptyLineIndex])->tag = tag;
                            for (int i = 0; i < E; ++i)
                                if ((cache[setIndex][i])->valid)
                                    ++((cache[setIndex][i])->LRUcounter);
                            (cache[setIndex][emptyLineIndex])->LRUcounter = 0;
                        }
                        else
                        {
                            ++eviction_count;
                            int highestLRULineIndex = 0;
                            int highestLRUcount = 0;
                            for (int i = 0; i < E; ++i)
                            {
                                if ((cache[setIndex][i])->LRUcounter > highestLRUcount)
                                {
                                    highestLRUcount = (cache[setIndex][i])->LRUcounter;
                                    highestLRULineIndex = i;
                                }
                            }
                            (cache[setIndex][highestLRULineIndex])->tag = tag;

                            for (int i = 0; i < E; ++i)
                            {
                                ++((cache[setIndex][i])->LRUcounter);
                            }
                            (cache[setIndex][highestLRULineIndex])->LRUcounter = 0;
                        }
                    }
                }
                break;
            }
            default:
                break;
            }
        }
    }
    fclose(trace_fp);
}

void freeCache()
{
    for (int setIndex = 0; setIndex < pow(2, s); ++setIndex)
    {
        for (int lineIndex = 0; lineIndex < E; ++lineIndex)
        {
            free(cache[setIndex][lineIndex]);
        }
        free(cache[setIndex]);
    }
    free(cache);
}

void printUsage(char *argv[])
{
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}

int main(int argc, char *argv[])
{
    char c;
    while ((c = getopt(argc, argv, "s:E:b:t:hv")) != -1)
    {
        switch (c)
        {
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            trace_file = optarg;
            break;
        case 'v':
            verbosity = 1;
            break;
        case 'h':
            printUsage(argv);
            exit(0);
        default:
            printUsage(argv);
            exit(1);
        }
    }
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL)
    {
        printf("%s: Missing required command line argument\n", argv[0]);
        printUsage(argv);
        exit(1);
    }
    buildCache();
    playTrace(trace_file);
    freeCache();
    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}