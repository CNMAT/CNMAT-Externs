#ifndef _EXT_PATH_H_
#define _EXT_PATH_H_

// ext_path.h -- path definitions

#if defined(powerpc) || defined(WIN_VERSION)
typedef FSSpec PATH_SPEC;
typedef short FILE_REF;
#define SEPARATOR_CHAR ':'
#endif

// fileinfo flags 
enum {	
	FILEINFO_ALIAS = 1,	
	FILEINFO_FOLDER = 2
};
	
// permissions, these are the same as on the Mac
enum {	
	READ_PERM = 1,	
	WRITE_PERM = 2,	
	RW_PERM = 3
};

enum {	
	STARTUP_PATH = 0,
	SEARCH_PATH = 1,	
	ACTION_PATH = 2,	
	HELP_PATH = 3
};

// indices allow for fast access
typedef struct _fileinfo{	
	long type;	
	long creator;		// Mac-only I guess	
	long date;	
	long flags;
} t_fileinfo;

typedef struct _path{	
	void *p_name;		// all we have on Windows, may just be folder name on Mac	
	long p_dirID;		// directory ID on Mac	
	short p_vRefNum;	// volume on Mac	
	short p_wd;			// working directory on the Mac (compatibility mode)
} t_path;

typedef struct _pathlink{	
	short l_path;		// index into path table, or p_wd in compatibility mode	
	short l_parent;		// logical parent, i.e., what path caused this path to be included	
	short l_permanent;	// non-zero if a permanent path (saved in preferences)	
	struct _pathlink *l_next;
} t_pathlink;

#ifndef __PATHMETHOD__
typedef Boolean (*pathmethod)(void *arg, PATH_SPEC *par, short patharg);
#endif __PATHMETHOD__

#endif _EXT_PATH_H_
