
/* 
	jit.file.h
	Copyright 2001-2004 - Cycling '74
	Joshua Kit Clayton jkc@cycling74.com

	- platform independent file calls
*/

#ifndef _JIT_FILE_H_
#define _JIT_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

typedef long t_jit_fileref;

t_jit_err jit_file_fsclose(t_jit_fileref refnum);
t_jit_err jit_file_fsread(t_jit_fileref refnum, long *count, void *bufptr);
t_jit_err jit_file_fswrite(t_jit_fileref refnum, long *count, const void *bufptr);
t_jit_err jit_file_seteof(t_jit_fileref refnum, long logeof);
t_jit_err jit_file_geteof(t_jit_fileref refnum, long *logeof);
t_jit_err jit_file_setfpos(t_jit_fileref refnum, long mode, long offset);
t_jit_err jit_file_getfpos(t_jit_fileref refnum, long *filepos);

#if C74_PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif C74_PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

#ifdef __cplusplus
}
#endif

#endif /* _JIT_FILE_H_ */
