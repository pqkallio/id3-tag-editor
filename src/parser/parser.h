#ifndef MP3_HEADER_EDITOR_PARSER_H
#define MP3_HEADER_EDITOR_PARSER_H

#include <stdio.h>

typedef struct _id3_tags {
    char* artist;
    char* title;
    unsigned int* track;
} ID3Tags;

ID3Tags* parseMP3(FILE* mp3file);

#endif /* MP3_HEADER_EDITOR_PARSER_H */
