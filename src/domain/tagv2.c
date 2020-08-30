#include <malloc.h>
#include <string.h>
#include "tagv2.h"

void add_frame(TagV2* tag, unsigned char id[4], uint32_t size, uint16_t flags, char* body)
{
    TagV2Frame* new_frame = calloc(1, sizeof(TagV2Frame));

    memcpy(new_frame->header.id, id, 4);

    new_frame->header.size = size;
    new_frame->header.flags = flags;
    new_frame->body = body;

    if (tag->first_frame == NULL) {
        tag->first_frame = new_frame;
    }

    if (tag->last_frame != NULL) {
        tag->last_frame->next = new_frame;
    }

    tag->last_frame = new_frame;
}

TagV2* new_tag_v2(uint16_t version, unsigned char flags, uint32_t size)
{
    TagV2* tag = calloc(1, sizeof(TagV2));

    memcpy(tag->header.id, ID3, 3);

    tag->header.version = version;
    tag->header.flags = flags;
    tag->header.size = size;

    tag->add_frame = add_frame;

    return tag;
}
