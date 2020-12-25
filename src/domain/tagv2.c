#include <string.h>
#include "tagv2.h"
#include "../util/strings.h"
#include "../mem/mem.h"

void add_tag_v2_frame(
    TagV2 *tag,
    const char *id,
    uint32_t size,
    char flags[2],
    const char *body,
    unsigned char has_zero_byte)
{
    const MemMap *memmap = tag->memmap;
    TagV2Frame *new_frame = memmap->allocate(memmap, 1, sizeof(TagV2Frame));

    new_frame->memmap = memmap;
    new_frame->header.id = string_copy(memmap, id);
    new_frame->body = string_copy(memmap, body);

    new_frame->header.size = size;
    new_frame->header.flags[0] = flags[0];
    new_frame->header.flags[1] = flags[1];
    new_frame->header.has_zero_byte = has_zero_byte;

    tag->frames->set(tag->frames, id, new_frame);
}

TagV2 *new_tag_v2(const MemMap *memmap, uint16_t version, unsigned char flags, uint32_t size)
{
    TagV2 *tag = memmap->allocate(memmap, 1, sizeof(TagV2));

    memcpy(tag->header.id, ID3, 3);

    tag->header.version = version;
    tag->header.flags = flags;
    tag->header.size = size;

    tag->memmap = memmap;
    tag->frames = new_hashmap(memmap);

    return tag;
}

void delete_tag_v2_frame(TagV2Frame *frame)
{
    if (frame == NULL)
        return;

    const MemMap *mem = frame->memmap;

    mem->free(mem, frame->body);

    mem->free(mem, frame->header.id);

    mem->free(mem, frame);
}

void delete_tag_v2_frame_from_map(const void *item)
{
    delete_tag_v2_frame((TagV2Frame *)item);
}

void delete_tag_v2(TagV2 *tag)
{
    if (!tag)
    {
        return;
    }

    tag->frames->foreach (tag->frames, delete_tag_v2_frame_from_map);
    tag->frames->clear(tag->frames);
    delete_hashmap(tag->frames);

    tag->memmap->free(tag->memmap, tag);
}

const TagV2Frame *get_tag_v2_frame(const TagV2 *tagV2, const char *frame_id)
{
    if (!tagV2 || !tagV2->frames)
    {
        return NULL;
    }

    return tagV2->frames->get(tagV2->frames, frame_id);
}
