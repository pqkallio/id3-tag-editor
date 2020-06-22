#include <malloc.h>
#include <stdint.h>
#include <string.h>
#include "parser.h"
#include "util/binary.h"

typedef struct _frame_target {
    char* frame_id;
    size_t tag_offset;
} FrameTarget;

const FrameTarget frame_targets[] = {
    {.frame_id = "TALB", .tag_offset = (size_t)offsetof(ID3Tags, album)},
    {.frame_id = "TPE1", .tag_offset = (size_t)offsetof(ID3Tags, artist)},
    {.frame_id = "TIT2", .tag_offset = (size_t)offsetof(ID3Tags, title)},
    {.frame_id = "TRCK", .tag_offset = (size_t)offsetof(ID3Tags, track)},
};

size_t* get_tag_offset(char* label)
{
    size_t* offset = NULL;

    for (unsigned int i = 0; i < 4; i++) {
        if (!memcmp(label, frame_targets[i].frame_id, 4)) {
            offset = malloc(sizeof(size_t));

            *offset = frame_targets[i].tag_offset;

            break;
        }
    }

    return offset;
}

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
    char _tag_size[] = {0, 0, 0, 0};

    fread(&version, sizeof(uint16_t), 1, mp3file);
    fread(&flags, sizeof(unsigned char), 1, mp3file);
    fread(&_tag_size, sizeof(char), 4, mp3file);

    if (feof(mp3file)) {
        return;
    }

    // each byte has 7 bits that count
    unsigned int tag_size = remove_padding(_tag_size, 4, 7);

    unsigned int i = 0;

    while (i < tag_size && !feof(mp3file)) {
        char frame_id[] = {0, 0, 0, 0};
        uint32_t frame_size;
        char flags[] = {0, 0};

        fread(&frame_id, sizeof(char), 4, mp3file);
        fread(&frame_size, sizeof(uint32_t), 1, mp3file);
        fread(&flags, sizeof(char), 2, mp3file);

        if (feof(mp3file)) {
            return;
        }

        size_t* tag_offset = get_tag_offset(frame_id);

        if (tag_offset != NULL) {
            fseek(mp3file, 1, SEEK_CUR);
            char** base = (char**)tags;

            char** member_ptr = (char**)(base + *tag_offset);
            *member_ptr = calloc(sizeof(char), frame_size);

            fread(*member_ptr, sizeof(char), frame_size - 1, mp3file);

            free(tag_offset);
        } else {
            fseek(mp3file, frame_size, SEEK_CUR);
        }

        i += frame_size + 10; // frame size + header size
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
