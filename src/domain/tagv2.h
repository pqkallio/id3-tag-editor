#ifndef _ID3_TAG_EDITOR_DOMAIN_TAGV2_H
#define _ID3_TAG_EDITOR_DOMAIN_TAGV2_H

#include <stdint.h>

#define ID3 "ID3"

typedef struct _tag_v2_header {
    char id[4];
    uint16_t version;
    unsigned char flags;
    uint32_t size;
} TagV2Header;

typedef struct _tag_v2_frame_header {
    const char* id;
    uint32_t size;
    uint16_t flags;
} TagV2FrameHeader;

typedef struct _tag_v2_frame {
    TagV2FrameHeader header;
    char* body;
    struct _tag_v2_frame* next;
} TagV2Frame;

typedef struct _tag_v2 {
    TagV2Header header;
    TagV2Frame* first_frame;
    TagV2Frame* last_frame;
} TagV2;

TagV2* new_tag_v2(uint16_t version, unsigned char flags, uint32_t size);
void delete_tag_v2(TagV2* tag);

void add_tag_v2_frame(TagV2* tag, const char* id, uint32_t size, uint16_t flags, const char* body);

#endif /* _ID3_TAG_EDITOR_DOMAIN_TAGV2_H */
