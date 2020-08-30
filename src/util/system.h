#ifndef _ID3_HEADER_EDITOR_UTIL_SYSTEM_H
#define _ID3_HEADER_EDITOR_UTIL_SYSTEM_H

const int UNUS = 1;

#define CPU_IS_BIG_ENDIAN ((*(char*) & UNUS) == 0)
#define CPU_IS_LITTLE_ENDIAN (!CPU_IS_BIG_ENDIAN)

#endif /* _ID3_HEADER_EDITOR_UTIL_SYSTEM_H */
