#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include "libpq-fe.h"

int main(int argc, char **argv) {
        const char *conninfo;
        PGconn     *conn;
        PGresult   *res;
        PGnotify   *notify;
        char db_name[] = "perepelitsya_ruslan";

        conn = PQsetdbLogin(NULL,NULL,NULL,NULL,db_name,NULL,NULL);
        if (PQstatus(conn) != CONNECTION_OK) {
                fprintf(stderr, "Connection failed: %s", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
        }
        res = PQexec(conn, "LISTEN perepelytsia");
        if(PQresultStatus(res) != PGRES_COMMAND_OK) {
                fprintf(stderr, "Listening failed: %s", PQerrorMessage(conn));
                PQclear(res);
                PQfinish(conn);
                exit(1);
        }
        PQclear(res);
        while ( 1==1 ) {
                PQconsumeInput(conn);
                while ((notify = PQnotifies(conn)) != NULL) {
                        fprintf(stderr, "NOTIFY of '%s' received from server-PID %d:%s\n", notify->relname, notify->be_pid, notify->extra);
                        PQfreemem(notify);
                        PQconsumeInput(conn);
                }
        }
        fprintf(stdout, "Done.\n");
        PQfinish(conn);
        return 0;
}
