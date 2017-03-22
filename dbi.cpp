#include <string.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sqlite3.h>

//#include "utils.h"
#include "log.h"
#include "dbi.h"

using namespace std;

sqlite3 * DBI::_db_handle = NULL;

int DBI::Query(const char * table, const char * field, DBResult & result, const char * cond) 
{
	char sql[512 + 128];
	char ** presult = NULL;
	int nrow, ncol;
	int i, j;
	char * errmsg;
	snprintf(sql, 512, "SELECT %s FROM [%s]", field?field:"*", table);
	if (cond) {
		strcat(sql, " where ");
		strcat(sql, cond);
	}

    dlog(LOG_DEBUG, "[QUERY]: %s", sql);

	int nResult = sqlite3_get_table(_db_handle, sql, &presult, &nrow, &ncol, &errmsg);
    dlog(LOG_DEBUG, "%s.%d nResult : %d, nrow %d ncol %d errmsg %s",
         __func__, __LINE__, nResult, nrow, ncol, errmsg);

	result.resize(nrow);
	for (i = 0; i < nrow; i++) {
		vector<string> & row = result[i];
		row.resize(ncol);
		for (j = 0; j < ncol; j++) {
            if (presult[(i+1)*ncol + j] != NULL)
                row[j] = presult[(i+1)*ncol + j];
            else
                row[j] = "";
		}
	}

	sqlite3_free_table(presult);

	return nrow;
}

int DBI::Insert(const char * table, const char * field, const char * value)
{
	char sql[1024];

	snprintf(sql, 1024, "INSERT INTO [%s] (%s) VALUES(%s)", table, field, value);
    dlog(LOG_DEBUG, "[INSERT]: %s", sql);

	return exec(sql, NULL);
}

int DBI::Delete(const char * table, const char * cond)
{
	char sql[128];

	snprintf(sql, 128, "DELETE FROM [%s]", table);
	if (cond) {
		strcat(sql, " where ");
		strcat(sql, cond);
	} else {
		// WARNING
	}
    dlog(LOG_DEBUG, "[DELETE]: %s", sql);

	return exec(sql, NULL);
}

int DBI::Update(const char * table, const char * key_value, const char * cond)
{
	char sql[1024 + 128];

	snprintf(sql, 1024, "UPDATE [%s] SET %s", table, key_value);
	if (cond) {
		strcat(sql, " where ");
		strcat(sql, cond);
	}
	//LOG << "[UPDATE]: " << sql << endl;

	return exec(sql, NULL);
}

int DBI::exec(const char * sql, char ** errmsg)
{
	int err;

    dlog(LOG_DEBUG, "[SQL]: %s", sql);
	err = sqlite3_exec(_db_handle, sql, NULL, NULL, errmsg);
	if (err != SQLITE_OK)
        dlog(LOG_ERR, "[SQL]: %s", sqlite3_errmsg(_db_handle));

	return err;
}



