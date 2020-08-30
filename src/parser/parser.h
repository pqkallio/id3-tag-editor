#ifndef _ID3_TAG_EDITOR_PARSER_H
#define _ID3_TAG_EDITOR_PARSER_H

#include <stdio.h>

// TODO: Remove after tag-v2-struct is ready
typedef struct _id3_tags {
    char* artist;
    char* title;
    char* album;
    char* track;
} ID3Tags;

ID3Tags* parseMP3(FILE* mp3file);

#endif /* _ID3_TAG_EDITOR_PARSER_H */
