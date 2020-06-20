#ifndef MP3_HEADER_EDITOR_PARSER_H
#define MP3_HEADER_EDITOR_PARSER_H

#include <stdio.h>

typedef struct _mp3_tags {

} MP3Tags;

MP3Tags* parseMP3(FILE* mp3file);

#endif /* MP3_HEADER_EDITOR_PARSER_H */
