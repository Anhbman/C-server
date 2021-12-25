#ifndef C_SERVER_FUNCCONSTANT_H
#define C_SERVER_FUNCCONSTANT_H

#include <postgresql/libpq-fe.h>
#include <string.h>
#include "stdlib.h"

void do_exit(PGconn *conn);

#endif //C_SERVER_FUNCCONSTANT_H