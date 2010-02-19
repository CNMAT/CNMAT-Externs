
#ifndef _SYSFILE_H_
#define _SYSFILE_H_



/** A t_filehandle is a cross-platform way of referring to an open file.
	It is an opaque structure, meaning you don’t have access to the individual 
	elements of the data structure. You can use a t_filehandle only 
	with the file routines in the Sysfile API. Do not use other platform- 
	specific file functions in conjunction with these functions. 
	The perm parameter can be either READ_PERM, WRITE_PERM, or RW_PERM.

	@ingroup files */
typedef void *t_filehandle;


/** Modes used by sysfile_setpos()
	@ingroup files */
typedef enum {
	SYSFILE_ATMARK = 0,		///< ?
	SYSFILE_FROMSTART,		///< Calculate the file position from the start of the file.
	SYSFILE_FROMLEOF,		///< Calculate the file position from the logical end of the file.
	SYSFILE_FROMMARK		///< Calculate the file position from the current file position.
} e_max_sysfile_posmodes;


/** Flags used reading and writing text files.
	@ingroup files */
typedef enum {
	TEXT_LB_NATIVE =			0x00000001L,	///< Use the linebreak format native to the current platform.
	TEXT_LB_MAC =				0x00000002L,	///< Use Macintosh line breaks
	TEXT_LB_PC =				0x00000004L,	///< Use Windows line breaks
	TEXT_LB_UNIX =				0x00000008L,	///< Use Unix line breaks
	TEXT_ENCODING_USE_FILE =	0x00000100L,	///< If this flag is not set then the encoding is forced to UTF8
	TEXT_NULL_TERMINATE	=		0x00000200L		///< Terminate memory returned from sysfile_readtextfile() with a NULL character
} e_max_sysfile_textflags;


#ifdef __cplusplus
extern "C" {
#endif


/**	Close a file opened with sysfile_open().
	This function is similar to FSClose() or fclose(). 
	It should be used instead of system-specific file closing routines in order to make max external 
	code that will compile cross-platform. 
	
	@ingroup	files
	@param	f	The #t_filehandle structure of the file the user wants to close. 
	@return		An error code.
*/
extern long sysfile_close(t_filehandle f);


/**	Read a file from disk.
	This function is similar to FSRead() or fread(). It should be used instead of 
	these functions (or other system-specific file reading routines) in order 
	to make max external code that will compile cross-platform. It reads 
	“count” bytes from file handle at current file position into “bufptr”. 
	The byte count actually read is set in “count”, and the file position is 
	updated by the actual byte count read.

	@ingroup files
	@param	f		The #t_filehandle structure of the file the user wants to open.
	@param	count	Pointer to the number of bytes that will be read from the file at the current file position. 
					The byte count actually read is returned to this value.
	@param	bufptr	Pointer to the buffer that the data will be read into.
	@return			An error code.
*/
extern long sysfile_read(t_filehandle f, long *count, void *bufptr);


/**	Read the contents of a file into a handle.
	@ingroup files
	@param	f	The open #t_filehandle structure to read into the handle.
	@param	h	The address of a handle into which the file will be read.
	@return 	An error code.
	@remark		You should free the pointer, when you are done with it, using sysmem_freehandle().
*/
extern long sysfile_readtohandle(t_filehandle f, char ***h);


/**	Read the contents of a file into a pointer.
	@ingroup files
	@param	f	The open #t_filehandle structure to read into the handle.
	@param	p	The address of a pointer into which the file will be read.
	@return 	An error code.
	@remark		You should free the pointer, when you are done with it, using sysmem_freeptr().
*/
extern long sysfile_readtoptr(t_filehandle f, char **p);


/** Write part of a file to disk.
	This function is similar to FSWrite() or fwrite(). It should be used instead 
	of these functions (or other system-specific file reading routines) in 
	order to make max external code that will compile cross-platform. The 
	function writes “count” bytes from “bufptr” into file handle at current 
	file position. The byte count actually written is set in "count", and the
	file position is updated by the actual byte count written.

	@ingroup files
	@param	f		The t_filehandle structure of the file to which the user wants to write.
	@param	count	Pointer to the number of bytes that will be written to the file at the current file position. 
					The byte count actually written is returned to this value.
	@param	bufptr	Pointer to the buffer that the data will be read from. 
	@return			An error code.
*/
extern long sysfile_write(t_filehandle f, long *count, const void *bufptr);


/**	Set the size of a file handle.
	This function is similar to and should be used instead of SetEOF(). 
	The function sets the size of file handle in bytes, specified by “logeof”. 
	
	@ingroup		files
	@param	f		The file's #t_filehandle structure.
	@param	logeof	The file size in bytes.
	@return			An error code.
*/
extern long sysfile_seteof(t_filehandle f, long logeof);


/**	Get the size of a file handle.
	This function is similar to and should be used instead of GetEOF().
	The function gets the size of file handle in bytes, and places it in “logeof”. 
	
	@ingroup		files
	@param	f		The file's #t_filehandle structure.
	@param	logeof	The file size in bytes is returned to this value.
	@return			An error code.
*/
extern long sysfile_geteof(t_filehandle f, long *logeof);


/**	Set the current file position of a file handle.
	This function is similar to and should be used instead of SetFPos(). 
	It is used to set the current file position of file handle to by the given 
	number of offset bytes relative to the mode used, as defined in #e_max_sysfile_posmodes.
	
	@ingroup		files
	@param	f		The file's #t_filehandle structure.
	@param	mode	Mode from which the offset will be calculated, as defined in #e_max_sysfile_posmodes.
	@param	offset	The offset in bytes relative to the mode.
	@return			An error code.
*/
extern long sysfile_setpos(t_filehandle f, long mode, long offset);


/**	Get the current file position of a file handle.
	This function is similar to and should be used instead of GetFPos(). 
	The function gets the current file position of file handle in bytes, and places it in "filepos". 
	
	@ingroup		files
	@param	f		The file's #t_filehandle structure.
	@param	filepos	The address of a variable to hold the current file position of file handle in bytes. 
	@return			An error code.
*/
extern long sysfile_getpos(t_filehandle f, long *filepos);


/**	Copy the contents of one file handle to another file handle.
	@ingroup files
	@param	src		The file handle from which to copy.
	@param	dst		The file handle to which the copy is written.
	@param	size	The number of bytes to copy.  If 0 the size of src will be used.
	@return			An error code.
*/
long sysfile_spoolcopy(t_filehandle src, t_filehandle dst, long size);


// private
void sysfile_setobject(t_filehandle f, t_object *o);



/**	Read a text file from disk.
	This function reads up to the maximum number of bytes given by 
	maxlen from file handle at current file position into the htext 
	handle, performing linebreak translation if set in flags.

	@ingroup		files
	@param	f		The #t_filehandle structure of the text file the user wants to open.
	@param	htext	Handle that the data will be read into.
	@param	maxlen	The maximum length in bytes to be read into the handle. 
					Passing the value 0L indicates no maximum (i.e. read the entire file).
	@param	flags	Flags to set linebreak translation as defined in #e_max_sysfile_textflags.
	@return			An error code.
*/
extern long sysfile_readtextfile(t_filehandle f, t_handle htext, long maxlen, long flags);


/**	Write a text file to disk.
	This function writes a text handle to a text file performing linebreak 
	translation if set in flags.

	@ingroup		files
	@param	f		The #t_filehandle structure of the text file to which the user wants to write.
	@param	htext	Handle that the data that will be read from.
	@param	flags	Flags to set linebreak translation as defined in #e_max_sysfile_textflags.
	@return			An error code.
*/
extern long sysfile_writetextfile(t_filehandle f, t_handle htext, long flags);


/**	Create a #t_filehandle from a pre-existing handle.
	@ingroup	files	
	@param		h		A handle for some data.
	@param		flags	Pass 0 (additional flags are private).
	@param		fh		The address of a #t_filehandle which will be allocated.
	@return				An error code.
*/
short sysfile_openhandle(char **h, long flags, t_filehandle *fh);


/**	Create a #t_filehandle from a pre-existing pointer.
	@ingroup	files	
	@param		p		A pointer to some data.
	@param		length	The size of p.
	@param		flags	Pass 0 (additional flags are private).
	@param		fh		The address of a #t_filehandle which will be allocated.
	@return				An error code.
*/
short sysfile_openptrsize(char *p, long length, long flags, t_filehandle *fh);


#ifdef __cplusplus
}
#endif

#endif // _SYSFILE_H_

