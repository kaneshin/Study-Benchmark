#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlite3.h>

#include "leap.h"

#define SUCCESS 0
#define FAILURE 1

sqlite3 *connect_db();
void free_db(sqlite3*);
int insert_db(sqlite3**, long, double);
double average_time(sqlite3**, long);

int
main(int argc, char* argv[])
{
    long i, n;
    double rt;
    clock_t t;

    if (argc > 1)
        n = atoi(argv[1]);
    else
        n = 100;

    t = clock();
    for (i = 0; i < n; ++i)
        output_leap();

    rt = ((double)(clock() - t) / CLOCKS_PER_SEC);

    // print query
    if (argc > 3 && 0 == strcmp("-q", argv[2])) {
        printf("INSERT INTO %s (user, num, time) VALUES (\"__USER__\", %ld, %f);\n", argv[3], n, rt);
        return SUCCESS;
    }

    // print stdout
    printf("Finished Benchmark Test (Loop => %ld)\n", n);
    printf("Time: %6f\n", rt);

    // insert data
    if (argc > 3 && 0 == strcmp("-db", argv[2])) {
        sqlite3 *db = connect_db(argv[3]);
        if (NULL == db) {
            return FAILURE;
        }
        insert_db(&db, n, rt);
        rt = average_time(&db, n);
        printf("Average of %ld: %lf\n", n, rt);
        free_db(db);
    }

    return SUCCESS;
}

sqlite3 *
connect_db(char *name)
{
    sqlite3 *db;
    int err;
    char sql[255];
    char *err_msg;

    err = sqlite3_open(name, &db);
    if (err) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sprintf(sql, "CREATE TABLE benches("  \
           "id      INTEGER PRIMARY KEY," \
           "num     INTEGER NOT NULL," \
           "time    NUMERIC NOT NULL);");
    err = sqlite3_exec(db, sql, NULL, 0, &err_msg);
    if(err != SQLITE_OK) {
        sqlite3_free(err_msg);
    }
    return db;
}

void
free_db(sqlite3 *db)
{
    sqlite3_close(db);
    db = NULL;
}

int
insert_db(sqlite3 **db, long num, double time)
{
    int err;
    char sql[255];
    char *err_msg;

    sprintf(sql,
            "INSERT INTO benches (num, time) VALUES (%ld, %lf);", num, time);
    err = sqlite3_exec(*db, sql, NULL, 0, &err_msg);
    if (SQLITE_OK != err) {
        fprintf(stderr, "SQL error: (%d) %s\n", err, err_msg);
        sqlite3_free(err_msg);
        return FAILURE;;
    }
    return SUCCESS;
}

double
average_time(sqlite3 **db, long num)
{
    sqlite3_stmt *stmt;
    int err;
    char *err_msg;
    const char *tail;
    double avg = 0.0;
    int c = 0;

    err = sqlite3_prepare_v2(*db,
            "SELECT * FROM benches order by id", 10000, &stmt, &tail);
    if (SQLITE_OK != err) {
        fprintf(stderr, "SQL error: (%d)\n", err);
        return 0.0;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (num == sqlite3_column_int(stmt, 1)) {
            ++c;
            avg += sqlite3_column_double(stmt, 2);
        }
    }
    sqlite3_finalize(stmt);
    return avg / (double)c;
}


