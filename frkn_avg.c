#include "postgres.h"
#include "fmgr.h"
#include "utils/bytea.h"
#include "utils/builtins.h"

typedef struct {
    float sum;
    float count;
} state;

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(my_state_in);
Datum
my_state_in(PG_FUNCTION_ARGS)
{
    char       *str = PG_GETARG_CSTRING(0);
    double      sum,
                count;
    state    *result;

    if (sscanf(str, "(%lf, %lf)", &sum, &count) != 2)
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                 errmsg("invalid input syntax for complex: \"%s\"",
                        str)));

    result = (state *) palloc(sizeof(state));
    result->sum = sum;
    result->count = count;
    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(my_state_out);
Datum
my_state_out(PG_FUNCTION_ARGS)
{
    state * my_state = (state *) PG_GETARG_POINTER(0);
    char *result;
    result = psprintf("(%g, %g)", my_state -> sum, my_state -> count);
    PG_RETURN_CSTRING(result);
}

PG_FUNCTION_INFO_V1(my_sfunc);
Datum
my_sfunc(PG_FUNCTION_ARGS){
    state * my_state = (state *) PG_GETARG_POINTER(0);
    float num = (float)PG_GETARG_INT32(1);
    my_state -> sum += (num);
    my_state -> count += 1;

    PG_RETURN_POINTER(my_state);
}


PG_FUNCTION_INFO_V1(my_sfunc_float);
Datum
my_sfunc_float(PG_FUNCTION_ARGS){
    state * my_state = (state *) PG_GETARG_POINTER(0);
    float num = PG_GETARG_FLOAT8(1);
    my_state -> sum += (num);
    my_state -> count += 1;

    PG_RETURN_POINTER(my_state);
}

PG_FUNCTION_INFO_V1(my_ffunc);
Datum
my_ffunc(PG_FUNCTION_ARGS){
    state * my_state = (state *) PG_GETARG_POINTER(0);

    PG_RETURN_FLOAT8(my_state -> sum / my_state -> count);
}
