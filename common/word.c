
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "word.h"

void normalizeWord(char* word)
{
    for(char* i = word; *i != '\0'; i++) {
        *i = tolower(*i);
    }
}