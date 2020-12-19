#ifndef _ID3_TAG_EDITOR_PARSER_H
#define _ID3_TAG_EDITOR_PARSER_H

#include <stdio.h>
#include "../domain/tagv2.h"
#include "../mem/mem.h"

TagV2 *parseMP3(const MemMap *memmap, FILE *mp3file);

#endif /* _ID3_TAG_EDITOR_PARSER_H */
