#include "include/FuncConstant.h"

void do_exit(PGconn *conn) {
    
    fprintf(stderr, "%s\n", PQerrorMessage(conn));    

    PQfinish(conn);    
    
    exit(1);
}
