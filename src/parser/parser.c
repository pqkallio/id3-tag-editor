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
    {.frame_id = "TALB", .tag_offset = offsetof(struct _id3_tags, album)},
    {.frame_id = "TPE1", .tag_offset = offsetof(struct _id3_tags, artist)},
    {.frame_id = "TIT2", .tag_offset = offsetof(struct _id3_tags, title)},
    {.frame_id = "TRCK", .tag_offset = offsetof(struct _id3_tags, track)},
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

/**
 * ID3 v2 tag structure
 * ____________________
 *
 * TAG Header:
 *
 *          |---------|---------|-----------|----------------|-----------|-----|------------------|
 *          | ID3     | 03 00   | abc0 0000 | xXXX XXXX (x4) | e.g. TALB | ... | e.g. TIT2        |
 *          |---------|---------|-----------|----------------|-----------|-----|------------------|
 * bytes:   | 0...2   | 3...4   | 5         | 6...9          | 10...19   | ... | size-11...size-1 |
 * content: | TAG id  | TAG ver | Flags     | Size of TAG    | Frame 1   | ... | Frame n          |
 * notes:   | I       |         | II        | III            |           | ... |                  |
 *
 * I     Must be "ID3".
 * II    a: Unsynchronization
 *       b: Extended header
 *       c: Experimental indicator
 * III   From each byte the first bit is ignored. The size doesn't contain the header, so the total size is
 *       size + 10 bytes.
 *
 *
 * Frame:
 *
 *          |-----------|------------|-------------------|-----------|---------------------|
 *          | e.g. TALB | e.g. 257   | abc00000 ijk00000 | 00        | "Blood and Thunder" |
 *          |-----------|------------|-------------------|-----------|---------------------|
 * bytes:   | 0...3     | 4...7      | 8...9             | 10        | 11...size-2         |
 * content: | frame id  | frame size | flags             | zero byte | frame body          |
 * notes:   | IV        | V          | VI                | VII       | VIII                |
 *
 * IV    Four character id, e.g. "TRCK" for track number or "TIT2" for song name.
 * V     Frame size that DOESN'T include the frame header, but DOES include the zero byte (see VII), i.e. the total size
 *       in bytes is size + 10 bytes.
 * VI    a: TAG alter preservation
 *       b: File alter preservation
 *       c: Read only
 *       i: Compression
 *       j: Encryption
 *       k: Grouping identity
 * VII   A zero byte is always included, but should be ignored.
 * VIII  Because of the zero byte (see VII), the content's length is size-1 bytes. It is a non-null-terminated string.
 */
void parse_v2_tag(ID3Tags* tags, FILE* mp3file)
{
    uint16_t version; // ignore for now
    unsigned char flags; // ignore for now
    char _tag_size[] = {0, 0, 0, 0};

    fread(&version, sizeof(uint16_t), 1, mp3file);
    fread(&flags, sizeof(unsigned char), 1, mp3file);
    fread(&_tag_size, sizeof(char), 4, mp3file);

    if (feof(mp3file)) {
        return;
    }

    // For the tag size, each byte has 7 bits that count, so we drop the first bit from each byte, squueze the rest
    // together and interpret that as the tag size.
    unsigned int tag_size = remove_padding(_tag_size, 4, 7);

    unsigned int i = 0;

    // Let's start reading frames.
    while (i < tag_size && !feof(mp3file)) {
        char frame_id[] = {0, 0, 0, 0, 0};
        uint32_t frame_size;
        char flags[] = {0, 0};

        fread(&frame_id, sizeof(char), 4, mp3file);
        read_big_endian_int(&frame_size, mp3file);
        fread(&flags, sizeof(char), 2, mp3file);

        // If something bad happens...
        if (feof(mp3file)) {
            return;
        }

        i += frame_size + 10; // frame size + header size

        size_t* tag_offset = get_tag_offset(frame_id);

        if (tag_offset == NULL) {
            // move along if this is not to be parsed
            fseek(mp3file, frame_size, SEEK_CUR);
            continue;
        }

        fseek(mp3file, 1, SEEK_CUR); // skip the zero byte

        // calculate the address of the member that the string is to be pointed from and allocate size for it
        char** base = (char**)tags;
        char** member_ptr = (char**)(base + (*tag_offset / 8));
        *member_ptr = (char*)calloc(sizeof(char), frame_size);

        fread(*member_ptr, sizeof(char), frame_size - 1, mp3file); // read the string from file

        free(tag_offset);
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
