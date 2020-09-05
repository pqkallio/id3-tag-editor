#include <malloc.h>
#include <stdint.h>
#include <string.h>
#include "parser.h"
#include "util/binary.h"

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
 * VII   A zero byte is SOMETIMES included, and should be ignored.
 * VIII  Because of the zero byte (see VII), the content's length is size-1 bytes. It is a non-null-terminated string.
 */
TagV2* parse_v2_tag(FILE* mp3file)
{
    uint16_t version;
    unsigned char flags;
    char _tag_size[] = {0, 0, 0, 0};

    fread(&version, sizeof(uint16_t), 1, mp3file);
    fread(&flags, sizeof(unsigned char), 1, mp3file);
    fread(&_tag_size, sizeof(char), 4, mp3file);

    if (feof(mp3file)) {
        return NULL;
    }

    // For the tag size, each byte has 7 bits that count, so we drop the first bit from each byte, squueze the rest
    // together and interpret that as the tag size.
    uint32_t tag_size = remove_padding(_tag_size, 4, 7);

    TagV2* tagV2 = new_tag_v2(version, flags, tag_size);

    unsigned int i = 0;
    char* body = NULL;
    char zero_byte = 'x';

    // Let's start reading frames.
    while (i < tag_size && !feof(mp3file)) {
        char frame_id[] = {0, 0, 0, 0, 0};
        uint32_t frame_size;
        char flags[] = {0, 0};
        unsigned char has_zero_byte = 1;

        fread(&frame_id, sizeof(char), 4, mp3file);
        read_big_endian_int(&frame_size, mp3file);
        fread(&flags, sizeof(char), 2, mp3file);

        if (frame_size < 1) {
            break;
        }

        // If something bad happens...
        if (feof(mp3file)) {
            return tagV2;
        }

        i += frame_size + 10; // frame size + header size

        body = realloc(body, frame_size * sizeof(char));
        body[frame_size - 1] = 0;

        fread(&zero_byte, sizeof(char), 1, mp3file);

        if (zero_byte != 0) {
            fseek(mp3file, -1, SEEK_CUR);
            has_zero_byte = 0;
        }

        fread(body, sizeof(char), has_zero_byte ? frame_size - 1 : frame_size, mp3file); // read the string from file

        add_tag_v2_frame(tagV2, frame_id, frame_size, flags, body, has_zero_byte);
        printf("%s (%d): %s\n", frame_id, frame_size, body);
    }

    return tagV2;
}

TagV2* parseMP3(FILE* mp3file)
{
    if (mp3file == NULL) {
        return NULL;
    }

    if (has_v2_tag(mp3file)) {
        return parse_v2_tag(mp3file);
    }

    return NULL;
}
