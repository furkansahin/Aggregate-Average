-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION frkn_avg" to load this file. \quit

CREATE TYPE my_state;

CREATE FUNCTION my_state_in(cstring)
RETURNS
    my_state
AS '$libdir/frkn_avg'
Language
    C
IMMUTABLE
STRICT;

CREATE FUNCTION my_state_out(my_state)
RETURNS
    cstring
AS '$libdir/frkn_avg'
Language
    C
IMMUTABLE
STRICT;

CREATE TYPE my_state (
    input = my_state_in,
    output = my_state_out,
    internallength = 16,
    alignment = double
);

CREATE FUNCTION my_sfunc(state my_state, nextValue bigint)
RETURNS
    my_state
AS
    'frkn_avg.so', 'my_sfunc'
Language
    C
IMMUTABLE
STRICT;

CREATE FUNCTION my_sfunc_float(state my_state, nextValue float)
RETURNS
    my_state
AS
    'frkn_avg.so', 'my_sfunc_float'
Language
    C
IMMUTABLE
STRICT;

CREATE FUNCTION my_ffunc(state my_state)
RETURNS
    float
AS
    'frkn_avg.so', 'my_ffunc'
Language
    C
STRICT
IMMUTABLE;

CREATE AGGREGATE my_avg(bigint) (
    stype = my_state,
    sfunc = my_sfunc,
    finalfunc = my_ffunc,
    initcond = '(0, 0)'
);

CREATE AGGREGATE my_avg(float) (
    stype = my_state,
    sfunc = my_sfunc_float,
    finalfunc = my_ffunc,
    initcond = '(0, 0)'
);
