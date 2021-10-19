
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

void normalizeWord(char* word)
{
    for(char* i = word; *i != '\0'; i++) {
        *i = tolower(*i);
    }
}