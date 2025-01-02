#pragma once

#include <cxx.h>

#define SQLITE_OPEN_READWRITE        0x00000002  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_CREATE           0x00000004  /* Ok for sqlite3_open_v2() */
#define SQLITE_OPEN_FULLMUTEX        0x00010000  /* Ok for sqlite3_open_v2() */

#define SQLITE_OK           0   /* Successful result */
#define SQLITE_ROW         100  /* sqlite3_step() has another row ready */
#define SQLITE_DONE        101  /* sqlite3_step() has finished executing */

struct sqlite3;
struct sqlite3_stmt;

extern int (*sqlite3_open_v2)(const char *filename, sqlite3 **ppDb, int flags, const char *zVfs);
extern int (*sqlite3_close)(sqlite3 *db);
extern const char *(*sqlite3_errstr)(int);

// Transparent wrappers of sqlite3_stmt
struct DbValues {
    const char *get_text(int index);
    int get_int(int index);
    ~DbValues() = delete;
};
struct DbStatement {
    int bind_text(int index, rust::Str val);
    int bind_int64(int index, int64_t val);
    ~DbStatement() = delete;
};

using StringSlice = rust::Slice<rust::String>;
using sql_bind_callback = int(*)(void*, int, DbStatement&);
using sql_exec_callback = void(*)(void*, StringSlice, DbValues&);

bool load_sqlite();
sqlite3 *open_and_init_db();
int sql_exec(sqlite3 *db, rust::Str zSql,
             sql_bind_callback bind_cb = nullptr, void *bind_cookie = nullptr,
             sql_exec_callback exec_cb = nullptr, void *exec_cookie = nullptr);