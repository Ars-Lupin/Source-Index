
#include <stdio.h>
#include "index.h"
#include "search.h"
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <index/search> <input_file> <query/stopwords_file>", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "index") == 0)
    {
        index_documents(argv[2]);
    }
    else if (strcmp(argv[1], "search") == 0)
    {
        search_documents(argv[2], argv[3]);
    }
    else
    {
        printf("Invalid option. Use 'index' or 'search'.");
        return 1;
    }

    return 0;
}
