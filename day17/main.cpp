#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int * program;
static int program_count;

static int * out_list;
static int out_count;

static long long reg_a;
static long long reg_b;
static long long reg_c;

static int ip = 0;

static int debug = 0;

static void process_input()
{
    program_count = 0;
    ip = 0;

    FILE * file = fopen( "../day17/input.txt", "r" );

    char * r;

    char buffer[ 1024 ];
    char program_buffer[ 1024 ];

    r = fgets( buffer, 1024, file );
    sscanf( buffer, "Register A: %lld", &reg_a );

    r = fgets( buffer, 1024, file );
    sscanf( buffer, "Register B: %lld", &reg_b );

    r = fgets( buffer, 1024, file );
    sscanf( buffer, "Register C: %lld", &reg_c );

    r = fgets( buffer, 1024, file );
    // blank

    r = fgets( buffer, 1024, file );
    sscanf( buffer, "Program: %s", program_buffer );

    program = new int[ 1024 ];

    char * token = strtok( program_buffer, "," );
    while ( token ) {
        int i = program_count++;
        program[ i ] = atoi( token );

        token = strtok( nullptr, "," );
    }

    fclose( file );
}

long long combo_operand( int operand )
{
    if ( operand <= 3 ) {
        return operand;
    } else if ( operand == 4 ) {
        return reg_a;
    } else if ( operand == 5 ) {
        return reg_b;
    } else if ( operand == 6 ) {
        return reg_c;
    } else if ( operand == 7 ) {
        // invalid
        assert( 0 );
    }

    // invalid
    assert( 0 );
}

static void simulate()
{
    out_list = new int[ 1024 ];
    out_count = 0;
repeat:

    int opcode = program[ ip ];
    int operand = program[ ip + 1 ];

    if ( opcode == 0 ) { // adv
        reg_a = reg_a >> combo_operand( operand );

        if ( !debug ) goto no_print;
        printf(
            "[a: %lld  b: %lld  c: %lld  o: %d  reg_a = reg_a / ( 1 << "
            "combo_operand( "
            "operand ) );\n",
            reg_a,
            reg_b,
            reg_c,
            operand
        );
    } else if ( opcode == 1 ) { // bxl
        reg_b = reg_b ^ operand;

        if ( !debug ) goto no_print;
        printf(
            "[a: %lld  b: %lld  c: %lld  o: %d  reg_b = reg_b ^ operand;\n",
            reg_a,
            reg_b,
            reg_c,
            operand
        );
    } else if ( opcode == 2 ) { // bst
        reg_b = combo_operand( operand ) % 8;

        if ( !debug ) goto no_print;
        printf(
            "[a: %lld  b: %lld  c: %lld  o: %d  reg_b = combo_operand( operand "
            ") %% "
            "8;\n",
            reg_a,
            reg_b,
            reg_c,
            operand
        );
    } else if ( opcode == 3 ) { // jnz
        if ( reg_a ) ip = operand - 2;

        if ( !debug ) goto no_print;
        printf(
            "[a: %lld  b: %lld  c: %lld  o: %d  if ( reg_a ) ip = operand - "
            "2;\n",
            reg_a,
            reg_b,
            reg_c,
            operand
        );
    } else if ( opcode == 4 ) { // bxc
        reg_b = reg_b ^ reg_c;

        if ( !debug ) goto no_print;
        printf(
            "[a: %lld  b: %lld  c: %lld  o: %d  reg_b = reg_b ^ reg_c;\n",
            reg_a,
            reg_b,
            reg_c,
            operand
        );
    } else if ( opcode == 5 ) { // out
        out_list[ out_count++ ] = combo_operand( operand ) % 8;

        if ( !debug ) goto no_print;
        printf(
            "[a: %lld  b: %lld  c: %lld  o: %d  printf( '%%d', combo_operand( "
            "operand ) %% 8 );\n",
            reg_a,
            reg_b,
            reg_c,
            operand
        );
    } else if ( opcode == 6 ) { // bdv
        reg_b = reg_a >> combo_operand( operand );

        if ( !debug ) goto no_print;
        printf(
            "[a: %lld  b: %lld  c: %lld  o: %d  reg_b = reg_a / ( 1 << "
            "combo_operand( operand ) );\n",
            reg_a,
            reg_b,
            reg_c,
            operand
        );
    } else if ( opcode == 7 ) { // cdv
        reg_c = reg_a >> combo_operand( operand );

        if ( !debug ) goto no_print;
        printf(
            "[a: %lld  b: %lld  c: %lld  o: %d  reg_c = reg_a / ( 1 << "
            "combo_operand( operand ) );\n",
            reg_a,
            reg_b,
            reg_c,
            operand
        );
    }

no_print:

    ip += 2;

    if ( ip < program_count ) {
        goto repeat;
    }

    printf( "out: " );
    for ( int i = 0; i < out_count; i++ ) {
        if ( i > 0 ) {
            printf( "," );
        }
        printf( "%d", out_list[ i ] );
    }
    printf( "\n" );
}

static void part1()
{
    process_input();
    simulate();
}

static int value_list[ 16 ];

long long transform_to_int()
{
    long long x = 0;
    for ( int i = 0; i < 16; i++ ) {
        x <<= 3;
        x |= value_list[ i ];
    }

    return x;
}

static int try_input( int i )
{
    if ( i >= 16 ) return 1;

    for ( int v = 0; v < 8; v++ ) {
        value_list[ i ] = v;

        int out_i = 15 - i;

        process_input();
        reg_a = transform_to_int();
        simulate();

        if ( out_list[ out_i ] == program[ out_i ] ) {
            if ( try_input( i + 1 ) ) return 1;
        }
    }

    return 0;
}

static void part2()
{
    if ( try_input( 0 ) ) {
        printf( "solution: %lld\n", transform_to_int() );
    }
}

int main()
{
    part1();
    part2();
}
