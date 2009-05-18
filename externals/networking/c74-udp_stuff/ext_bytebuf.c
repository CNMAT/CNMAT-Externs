//bytebuf class///////////////////////////////////////////////////////////////
typedef struct bytebuf
{
	long size;//absolute size of the bytebuffer obj
	long n;  //number of valid bytes in n -- could be shorter than size 
	char *buf;

}t_bytebuf;

t_bytebuf* bytebuf_new(long size);
void bytebuf_free(t_bytebuf *bb);
void bytebuf_copyto(t_bytebuf *bb, char *buf, int length);


t_bytebuf* bytebuf_new(long size)
{
	t_bytebuf *bb = (t_bytebuf*)getbytes(sizeof(t_bytebuf));
	if(bb)
	{
		bb->buf = (char *)sysmem_newptr(size);
		if(!bb->buf)
		{
			freebytes(bb,sizeof(t_bytebuf));
			return NULL;
		}

		bb->size = size;
		return bb;
	}
	return NULL;
}

//copy length bytes from buf into bytebuf->buf
void bytebuf_copyto(t_bytebuf *bb, char *buf, int length)
{
	if(length > bb->size)
	{
		bb->buf = (char *)sysmem_resizeptr(bb->buf, length);
		bb->size = length;
	}
//sysmem_copyptr(const void *src, void *dst, long bytes);
	sysmem_copyptr((const void *)buf, (void *)bb->buf,length);
	bb->n = length;
}

void bytebuf_free(t_bytebuf *bb)
{
	sysmem_freeptr(bb->buf);	
	freebytes(bb,sizeof(t_bytebuf));
}

//end bytebuf class//////////////////////////////////////////////////////////
