#include <malloc.h>
#include <stdint.h>
#include <string.h>
#include "parser.h"

int has_v2_tag(FILE* mp3file)
{
    if (mp3file == NULL) { 
        return 0;
    }

    char tag[4];

    fgets(tag, 4, mp3file);
    
    if (feof(mp3file)) {
        return 0;
    }

    return !memcmp(tag, "ID3", 3);
}

void parse_v2_tag(ID3Tags* tags, FILE* mp3file)
{
    uint16_t version;
    unsigned char flags;
    uint32_t tag_size;

    fread(&version, sizeof(uint16_t), 1, mp3file);
    fread(&flags, sizeof(unsigned char), 1, mp3file);
    fread(&tag_size, sizeof(uint32_t), 1, mp3file);
    
    if (feof(mp3file)) {
        return;
    }


}

ID3Tags* parseMP3(FILE* mp3file)
{
    if (mp3file == NULL) {
        return NULL;
    }

    ID3Tags* tags = calloc(sizeof(ID3Tags), 1);

    if (has_v2_tag(mp3file)) {
        parse_v2_tag(tags, mp3file);
    }
    
    return tags;
}
