
#include "ext.h"
#include "ext_path.h"
#include "ext_common.h"
#include "ext_strings.h"
#include "ext_sndfile.h"
#include "sf2.h"
#include "open.h"

#ifdef WIN_VERSION
#include <NumberFormatting.h>
#endif

t_sfile *sfile_new(void)
{
	t_sfile *s;
	if (s=(t_sfile *)getbytes(sizeof(t_sfile)))
		setmem(s,sizeof(t_sfile),0);
	return s;
}

void sfile_free(t_sfile *s)
{
	if (s->s_open)
		sfile_close(s); // only close a file if it's open - it might have been closed already - safety check
	freebytes(s,sizeof(t_sfile));
}

void sfile_close(t_sfile *s)
{
	s->s_open = 0;
	if (s->s_refnum) {
		sysfile_close(s->s_refnum);
		s->s_refnum = 0;
	}
	if (s->s_asyncfile) {
		asyncfile_close(s->s_asyncfile);
		s->s_asyncfile = NULL;
	}
}

long sfile_open(t_sfile *s, t_symbol *name, short vol, unsigned long filetype, long flags)
{
	short err;
	char ps[256];
	t_sndfileinfo info;
	t_filehandle fh=NULL;
					
	setmem(&info,sizeof(t_sndfileinfo),0);
	strcpy(ps,name->s_name);

	s->s_refnum = 0;
	s->s_asyncfile = NULL;

	if (filetype==SF_FILETYPE_RAW) {
		//default values
		info.sr = 44100.;
		info.comptype = FOUR_CHAR_CODE('NONE');
		info.nchans = 2;	
		info.offset = 0;
		info.sampsize = 2;
		err = path_opensysfile(ps,vol,&fh,READ_PERM);
		if (err) {
			error("error opening %s, err %d",name->s_name,err);
			return err;
		}
		sysfile_geteof(fh,&info.length);
		sysfile_close(fh);
	} else {
		err = sndfile_info(ps,vol,filetype,&info);
		if (err) {
			error("error opening %s, err %d",name->s_name,err);
			return err;
		}
	}

	if (flags&SFILE_ASYNC) {
		err = asyncfile_create(ps,vol,READ_PERM,0,&s->s_asyncfile,ASYNCFILE_NO_CACHE);
		if (err)
			return err;
	
	} else {
		err = path_opensysfile(ps,vol,&fh,READ_PERM);
		if (err) {
			error("error opening %s, err %d",name->s_name,err);
			return err;
		}
		s->s_refnum = fh;
	}
	
	s->s_srate = info.sr;
	s->s_comptype = info.comptype;
	s->s_srcchans = info.nchans;
	s->s_offset = info.offset;
	s->s_size = info.length;
	s->s_sampsize = info.sampsize; 
	s->s_frames = s->s_size / (s->s_sampsize * s->s_srcchans); 
	s->s_busy = 0;
	s->s_open = 1;
	s->s_name = name;
	s->s_byteswap = info.byteswap; 
	
	
	// copy sustain loop data from instrument block
	if ((filetype == FOUR_CHAR_CODE('AIFF') || filetype == FOUR_CHAR_CODE('AIFC')) && info.inst && ((InstrumentChunk *)info.inst)->ckSize) {
		long data;
		
		data = 0;
		if (((InstrumentChunk *)info.inst)->sustainLoop.beginLoop && info.markers)
			data = sfile_searchmarkers(info.markers,((InstrumentChunk *)info.inst)->sustainLoop.beginLoop);
		s->s_susloopstart = data;	// in samples (frames)
		
		data = 0;
		if (((InstrumentChunk *)info.inst)->sustainLoop.beginLoop && info.markers)
			data = sfile_searchmarkers(info.markers,((InstrumentChunk *)info.inst)->sustainLoop.endLoop);
		s->s_susloopend = data;	// in samples (frames)
	} 
	else {
		s->s_susloopstart = 0;	// in samples (frames)
		s->s_susloopend = 0;	// in samples (frames)
	}
		
//	post("loop points in file: %ld %ld", s->s_susloopstart, s->s_susloopend);
	
	
	return 0;
}

long sfile_searchmarkers(MarkerChunk *markers, short id)
{
	short i;
	long offset;
	Marker *mp;
	
	for (i = 0, mp = markers->Markers; i < markers->numMarkers; i++) {
		if (mp->id == id)
			return mp->position;
		offset = mp->markerName[0] + 1 + sizeof(long) + sizeof(MarkerIdType);
		offset += (-offset) & 1; // no idea what to do here
		mp = (Marker *)(((Byte *)mp) + offset);
	}
	return 0;
}



long sfile_aiffopen(t_sfile *s, t_symbol *name, short vol, long flags)
{
	return sfile_open(s,name,vol,SF_FILETYPE_AIFF,flags);
}

long sfile_auopen(t_sfile *s, t_symbol *name, short vol, long flags)
{
	return sfile_open(s,name,vol,SF_FILETYPE_AU,flags);
}

long sfile_waveopen(t_sfile *s, t_symbol *name, short vol, long flags)
{
	return sfile_open(s,name,vol,SF_FILETYPE_WAVE,flags);
}

long sfile_rawopen(t_sfile *s, t_symbol *name, short vol, long flags)
{
	return sfile_open(s,name,vol,SF_FILETYPE_RAW,flags);
}

// since it's already in the kernel like the others, we might as well use it instead of the one below
long sfile_sd2open(t_sfile *s, t_symbol *name, short vol, long flags)
{
	return sfile_open(s,name,vol,SF_FILETYPE_SD2,flags);
}
/*
//I've left this stuff for now, not sure about resource stuff
long sfile_sd2open(t_sfile *s, t_symbol *name, short vol, long flags)
{
	char ps[256];
	char srs[1024];
	long offset,size,nchans,ssize,srate,count;
	double fsrate;
	short refNum, err,oldResFile, sdResFile;
	StringHandle sh;
	
	s->s_refnum = 0;
	s->s_asyncfile = NULL;

	oldResFile = CurResFile();
	strcpy(ps,name->s_name);
	err = path_openfile(ps,vol,&refNum,READ_PERM);
	if (err) {
		error("error opening %s, err %d",name->s_name,err);
		return err;
	}
	err = path_openresfile(ps,vol,&sdResFile,READ_PERM);
	if (sdResFile == -1) {
		error("problem with SD II file");
		return -1;
	}
	UseResFile(sdResFile);
	sh = (StringHandle)Get1Resource('STR ',1000);
	if (sh) StringToNum(*sh,&ssize);
	sh = (StringHandle)Get1Resource('STR ',1001);
	if (sh) {
		ptoccpy(srs,*sh);
		sscanf(srs,"%lf",&fsrate);
		//post("sampling rate %2.4f",fsrate);
		srate = fsrate;
	}
	sh = (StringHandle)Get1Resource('STR ',1002);
	if (sh) StringToNum(*sh,&nchans);
	CloseResFile(sdResFile);
	UseResFile(oldResFile);
	GetEOF(refNum,&size);

	if (flags&SFILE_ASYNC) {
		FSClose(refNum);
		err = asyncfile_create(ps,vol,READ_PERM,0,&s->s_asyncfile,ASYNCFILE_NO_CACHE);
		if (err)
			return err;
	} else {
		s->s_refnum = refNum;
	}

	s->s_comptype = 'NONE'; //for now only support integer types(not sure if sd2 supports others)
	s->s_srate = srate;
	s->s_srcchans = nchans;
	s->s_offset = 0;
	s->s_size = size;
	s->s_frames = size / (ssize * nchans); 
	s->s_sampsize = ssize;
	s->s_busy = 0;
	s->s_open = 1;
	s->s_name = name;
#if C74_LITTLE_ENDIAN
	s->s_byteswap = 1;
#else
	s->s_byteswap = 0;
#endif
	
	// might add check for 16-bit samples
	return 0;
}*/

void sfile_async_done(t_asyncfile_params *p)
{
	t_sfile *s = (t_sfile *)p->arg;
	s->s_busy = 0;
	asyncfile_params_free(p);
}
