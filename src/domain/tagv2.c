#include <malloc.h>
#include <string.h>
#include "tagv2.h"
#include "../util/strings.h"

void add_tag_v2_frame(TagV2* tag, const char* id, uint32_t size, char flags[2], const char* body)
{
    TagV2Frame* new_frame = calloc(1, sizeof(TagV2Frame));

    new_frame->header.id = string_copy(id);
    new_frame->body = string_copy(body);

    new_frame->header.size = size;
    new_frame->header.flags[0] = flags[0];
    new_frame->header.flags[1] = flags[1];

    set(tag->frames, id, new_frame);
}

TagV2* new_tag_v2(uint16_t version, unsigned char flags, uint32_t size)
{
    TagV2* tag = calloc(1, sizeof(TagV2));

    memcpy(tag->header.id, ID3, 3);

    tag->header.version = version;
    tag->header.flags = flags;
    tag->header.size = size;

    tag->frames = new_hashmap();

    return tag;
}

TagV2Frame* delete_tag_v2_frame(TagV2Frame* frame)
{
    if (frame->body != NULL) {
        free(frame->body);
    }

    free(frame);

    return NULL;
}

void delete_tag_v2(TagV2* tag)
{
    if (!tag) {
        return;
    }

    delete_hashmap(tag->frames);

    free(tag);
}
