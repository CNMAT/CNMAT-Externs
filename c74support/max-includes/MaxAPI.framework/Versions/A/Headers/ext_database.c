/* 
	ext_database.c
	Copyright 2008 - Cycling '74
	Timothy Place, tim@cycling74.com	
*/

#include "ext.h"
#include "ext_obex.h"
#include "ext_database.h"
#include "ext_strings.h"


#define DATABASE_SQL_MAXLEN 4096
// Define the macro below to enable debug code
//#define DATABASE_DEBUG


t_max_err db_open(t_symbol *dbname, const char *fullpath, t_database **db)
{
	long	ac=0;
	t_atom	av[6];
	
	if (*db)
		object_free(*db);
	
	if (fullpath) {
		char	coercedpath[MAX_PATH_CHARS];
		short	err;
		char	dbpath[MAX_PATH_CHARS];
#ifdef MAC_VERSION
		char	*temppath;
#endif

		err = path_nameconform((char*)fullpath, coercedpath, PATH_STYLE_NATIVE_PLAT, PATH_TYPE_ABSOLUTE);
		if (err)
			strncpy_zero(coercedpath, fullpath, MAX_PATH_CHARS);
		
#ifdef MAC_VERSION
		temppath = strchr(coercedpath, ':');
		*temppath = '\0';
		temppath += 1;
		
		// at this point temppath points to the path after the volume, and coercedpath has the volume
		snprintf(dbpath, MAX_PATH_CHARS, "/Volumes/%s%s", coercedpath, temppath);
#else // WIN_VERSION
		strncpy_zero(dbpath, coercedpath, MAX_PATH_CHARS);
#endif
		
		atom_setsym(av+ac, gensym("@rambased"));
		ac++;
		atom_setlong(av+ac, 0);
		ac++;
		atom_setsym(av+ac, gensym("@filename"));
		ac++;
		atom_setsym(av+ac, gensym(dbpath));
		ac++;
	}
	atom_setsym(av+ac, gensym("@db"));
	ac++;
	atom_setsym(av+ac, dbname);
	ac++;
	
	*db = (t_database*)object_new_typed(_sym_nobox, _sym_sqlite, ac, av);
	if (!*db)
		return MAX_ERR_GENERIC;
	
	return MAX_ERR_NONE;
}

t_max_err db_close(t_database **db)
{
	object_free(*db);
	*db = NULL;
	return MAX_ERR_NONE;
}


t_max_err db_query(t_database *db, t_db_result **dbresult, const char *s, ...)
{
	char	sql[DATABASE_SQL_MAXLEN+2];
	va_list	ap;
	int		err = 0;
	int		len = 0;

	if (db) {
		va_start(ap, s);

		len = vsnprintf(sql, DATABASE_SQL_MAXLEN, s, ap);
		sql[DATABASE_SQL_MAXLEN] = '\0';
		if (len >= DATABASE_SQL_MAXLEN) {
			cpost("DB: Query Overrun!\n");
			err = MAX_ERR_GENERIC;
			goto out;
		}
		err = (int)object_method(db, _sym_execstring, sql, dbresult);
#ifdef DATABASE_DEBUG
		cpost("SQL: %s\n", sql);
		if (err)
			cpost("   ERROR: %i\n", err);
#endif // DATABASE_DEBUG		
out:
		va_end(ap);
	}
	return err;
}

t_max_err db_query_silent(t_database *db, t_db_result **dbresult, const char *s, ...)
{
	char		sql[DATABASE_SQL_MAXLEN+2];
	va_list		ap;
	long		logging = false;
	t_max_err	err;

	va_start(ap, s);
	vsnprintf(sql, DATABASE_SQL_MAXLEN, s, ap);
	sql[DATABASE_SQL_MAXLEN] = '\0';

	logging = object_attr_getlong(db, _sym_log);
	object_attr_setlong(db, _sym_log, 0);
	err = db_query(db, dbresult, sql, NULL);
	object_attr_setlong(db, _sym_log, logging);
	return err;
}

t_max_err db_query_getlastinsertid(t_database *db, long *id)
{
	*id = (long)object_method(db, _sym_getlastinsertid);
	return MAX_ERR_NONE;
}


t_max_err db_query_table_new(t_database *db, const char *tablename)
{
	char	idname[256];
	
	// SQLite won't let us add primary keys later, so we have to add it now.
	strncpy_zero(idname, tablename, 256);
	
	// singularize
	if (idname[strlen(idname)-1] == 's')
		idname[strlen(idname)-1] = 0;
	
	return db_query(db, NULL, "CREATE TABLE IF NOT EXISTS %s ( %s_id INTEGER PRIMARY KEY NOT NULL )", tablename, idname);
}

t_max_err db_query_table_addcolumn(t_database *db, const char *tablename, const char *columnname, const char *columntype, const char *flags)
{
	if (flags)
		return db_query_silent(db, NULL, "ALTER TABLE %s ADD COLUMN %s %s %s", tablename, columnname, columntype, flags);
	else
		return db_query_silent(db, NULL, "ALTER TABLE %s ADD COLUMN %s %s", tablename, columnname, columntype);
}


t_max_err db_transaction_start(t_database *db)
{
	return (t_max_err)object_method(db, _sym_starttransaction);
}

t_max_err db_transaction_end(t_database *db)
{
	return (t_max_err)object_method(db, _sym_endtransaction);
}

t_max_err db_transaction_flush(t_database *db)
{
	return (t_max_err)object_method(db, _sym_flush);
}


// DB VIEWS

t_max_err db_view_create(t_database *db, const char *sql, t_db_view **dbview)
{
	*dbview = NULL;
	
	if(sql && sql[0])
		*dbview = (t_db_view *)object_method(db, gensym("createview"), gensym((char*)sql));
	else
		*dbview = (t_db_view *)object_method(db, gensym("createview"), NULL);
		
	if (!*dbview)
		return MAX_ERR_GENERIC;

	return MAX_ERR_NONE;
}

t_max_err db_view_remove(t_database *db, t_db_view **dbview)
{
	if (db && *dbview)
		object_method(db, gensym("removeview"), *dbview);
	*dbview = NULL;
	return MAX_ERR_NONE;
}

t_max_err db_view_getresult(t_db_view *dbview, t_db_result **result)
{
	if (result){
		*result = (t_db_view *)object_method(dbview, gensym("getresult"));
		return MAX_ERR_NONE;
	}
	return MAX_ERR_GENERIC;
}

t_max_err db_view_setquery(t_db_view *dbview, char *newquery)
{
	if (dbview)
		return object_attr_setsym(dbview, _sym_query, gensym(newquery));
	else
		return MAX_ERR_GENERIC;
}


// DB RESULTS

char** db_result_nextrecord(t_db_result *result)
{
	if (result)
		return (char **)object_method(result, _sym_nextrecord);
	else
		return NULL;
}

void db_result_reset(t_db_result *result)
{
	object_method(result, _sym_reset);
}

void db_result_clear(t_db_result *result)
{
	object_method(result, _sym_clear);
}

long db_result_numrecords(t_db_result *result)
{
	if (result)
		return (long)object_method(result, _sym_numrecords);
	else
		return 0;
}

long db_result_numfields(t_db_result *result)
{
	if (result)
		return (long)object_method(result, _sym_numfields);
	else
		return 0;
}

char* db_result_fieldname(t_db_result *result, long fieldindex)
{
	return (char *)object_method(result, _sym_fieldnamebyindex, fieldindex);
}

char* db_result_string(t_db_result *result, long recordindex, long fieldindex)
{
	return (char *)object_method(result, _sym_valuebyindex, recordindex, fieldindex);
}

long db_result_long(t_db_result *result, long recordindex, long fieldindex)
{
	char*	c = (char *)object_method(result, _sym_valuebyindex, recordindex, fieldindex);
	long	l = 0;
	
	if (c)
		sscanf(c, "%ld", &l);
	return l;
}

float db_result_float(t_db_result *result, long recordindex, long fieldindex)
{
	char*	c = (char *)object_method(result, _sym_valuebyindex, recordindex, fieldindex);
	float	f = 0;
	
	if (c)
		sscanf(c, "%f", &f);
	return f;
}

unsigned long db_result_datetimeinseconds(t_db_result *result, long recordindex, long fieldindex)
{
	char			*str = db_result_string(result, recordindex, fieldindex);
	unsigned long	seconds = 0;
	
	if (str)
		db_util_stringtodate(str, &seconds);
	return seconds;
}


// UTILITIES

void db_util_stringtodate(const char *string, unsigned long *date)
{
	t_datetime	datetime;
	
	sscanf(string, "%4lu-%02lu-%02lu %02lu:%02lu:%02lu",
		   &datetime.year, &datetime.month, &datetime.day, 
		   &datetime.hour, &datetime.minute, &datetime.second);
	*date = systime_datetoseconds(&datetime);	
}

void db_util_datetostring(const unsigned long date, char *string)
{
	t_datetime	datetime;
	
	systime_secondstodate(date, &datetime);
	sprintf(string, "%4lu-%02lu-%02lu %02lu:%02lu:%02lu", 
			datetime.year, datetime.month, datetime.day, 
			datetime.hour, datetime.minute, datetime.second);
}

