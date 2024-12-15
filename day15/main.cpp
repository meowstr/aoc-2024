#include <stdio.h>
#include <string.h>

static char * data_grid;
static int grid_w = 50;
static int grid_h = 50;

static int robot_x;
static int robot_y;

static int grid( int x, int y )
{
    return grid_w * y + x;
}

static void print_grid()
{
    for ( int y = 0; y < grid_h; y++ ) {
        for ( int x = 0; x < grid_w; x++ ) {
            printf( "%c", data_grid[ grid( x, y ) ] );
        }
        printf( "\n" );
    }
}

static void new_cell( int * out_x, int * out_y, int x, int y, char dir )
{
    if ( dir == '<' ) {
        *out_x = x - 1;
        *out_y = y;
    } else if ( dir == '>' ) {
        *out_x = x + 1;
        *out_y = y;
    } else if ( dir == '^' ) {
        *out_x = x;
        *out_y = y - 1;
    } else if ( dir == 'v' ) {
        *out_x = x;
        *out_y = y + 1;
    }
}

static void fill_hole( int nx, int ny, int x, int y )
{
    data_grid[ grid( x, y ) ] = data_grid[ grid( nx, ny ) ];
    data_grid[ grid( robot_x, robot_y ) ] = '.';
    data_grid[ grid( nx, ny ) ] = '@';
    robot_x = nx;
    robot_y = ny;
}

static void move( char dir )
{
    int nx;
    int ny;
    new_cell( &nx, &ny, robot_x, robot_y, dir );

    int cx = robot_x;
    int cy = robot_y;
    for ( int i = 0; i < 50; i++ ) {
        new_cell( &cx, &cy, cx, cy, dir );

        // we hit a wall
        if ( data_grid[ grid( cx, cy ) ] == '#' ) break;

        if ( data_grid[ grid( cx, cy ) ] == '.' ) {
            // we found a hole
            fill_hole( nx, ny, cx, cy );
            return;
        }
    }

    // failed to move
}

static int can_push( int x, int y, char dir )
{
    int lx;
    int rx;

    if ( data_grid[ grid( x, y ) ] == '[' ) {
        lx = x;
        rx = x + 1;
    } else if ( data_grid[ grid( x, y ) ] == ']' ) {
        lx = x - 1;
        rx = x;
    } else if ( data_grid[ grid( x, y ) ] == '.' ) {
        return 1;
    } else {
        return 0;
    }

    if ( dir == '>' ) {
        return can_push( rx + 1, y, dir );
    } else if ( dir == '<' ) {
        return can_push( lx - 1, y, dir );
    } else if ( dir == '^' ) {
        return can_push( lx, y - 1, dir ) && can_push( rx, y - 1, dir );
    } else if ( dir == 'v' ) {
        return can_push( lx, y + 1, dir ) && can_push( rx, y + 1, dir );
    }

    return 0;
}

static void push( int x, int y, char dir )
{
    int lx;
    int rx;

    if ( data_grid[ grid( x, y ) ] == '[' ) {
        lx = x;
        rx = x + 1;
    } else if ( data_grid[ grid( x, y ) ] == ']' ) {
        lx = x - 1;
        rx = x;
    } else {
        return;
    }

    if ( dir == '>' ) {
        push( rx + 1, y, dir );
        data_grid[ grid( lx, y ) ] = '.';
        data_grid[ grid( lx + 1, y ) ] = '[';
        data_grid[ grid( rx + 1, y ) ] = ']';
    } else if ( dir == '<' ) {
        push( lx - 1, y, dir );
        data_grid[ grid( rx, y ) ] = '.';
        data_grid[ grid( lx - 1, y ) ] = '[';
        data_grid[ grid( rx - 1, y ) ] = ']';
    } else if ( dir == '^' ) {
        push( lx, y - 1, dir );
        push( rx, y - 1, dir );
        data_grid[ grid( lx, y ) ] = '.';
        data_grid[ grid( rx, y ) ] = '.';
        data_grid[ grid( lx, y - 1 ) ] = '[';
        data_grid[ grid( rx, y - 1 ) ] = ']';
    } else if ( dir == 'v' ) {
        push( lx, y + 1, dir );
        push( rx, y + 1, dir );
        data_grid[ grid( lx, y ) ] = '.';
        data_grid[ grid( rx, y ) ] = '.';
        data_grid[ grid( lx, y + 1 ) ] = '[';
        data_grid[ grid( rx, y + 1 ) ] = ']';
    }
}

static void move2( char dir )
{
    int nx;
    int ny;
    new_cell( &nx, &ny, robot_x, robot_y, dir );

    if ( !can_push( nx, ny, dir ) ) return;

    push( nx, ny, dir );

    data_grid[ grid( robot_x, robot_y ) ] = '.';
    data_grid[ grid( nx, ny ) ] = '@';

    robot_x = nx;
    robot_y = ny;
}

static void process_input()
{
    FILE * file = fopen( "../day15/input.txt", "r" );

    for ( int y = 0; y < grid_h; y++ ) {
        char buffer[ 1024 ];
        char * r = fgets( buffer, 1024, file );

        for ( int x = 0; x < grid_w; x++ ) {
            data_grid[ grid( x, y ) ] = buffer[ x ];

            if ( buffer[ x ] == '@' ) {
                robot_x = x;
                robot_y = y;
            }
        }
    }

    print_grid();

    while ( !feof( file ) ) {
        char c = fgetc( file );
        if ( !strchr( "<>^v", c ) ) continue;

        move( c );

        // NOTE: remove for interactive mode
        // getchar();
        // print_grid();
    }

    fclose( file );
}

static void process_input2()
{
    FILE * file = fopen( "../day15/input.txt", "r" );

    for ( int y = 0; y < grid_h; y++ ) {
        char buffer[ 1024 ];
        char * r = fgets( buffer, 1024, file );

        for ( int x = 0; x < 50; x++ ) {
            char c = buffer[ x ];
            if ( c == '#' ) {
                data_grid[ grid( x * 2 + 0, y ) ] = '#';
                data_grid[ grid( x * 2 + 1, y ) ] = '#';
            }
            if ( c == 'O' ) {
                data_grid[ grid( x * 2 + 0, y ) ] = '[';
                data_grid[ grid( x * 2 + 1, y ) ] = ']';
            }
            if ( c == '.' ) {
                data_grid[ grid( x * 2 + 0, y ) ] = '.';
                data_grid[ grid( x * 2 + 1, y ) ] = '.';
            }
            if ( c == '@' ) {
                data_grid[ grid( x * 2 + 0, y ) ] = '@';
                data_grid[ grid( x * 2 + 1, y ) ] = '.';
                robot_x = x * 2;
                robot_y = y;
            }
        }
    }

    print_grid();

    while ( !feof( file ) ) {
        char c = fgetc( file );
        if ( !strchr( "<>^v", c ) ) continue;

        move2( c );

        // NOTE: remove for interactive mode
        // getchar();
        // print_grid();
    }

    fclose( file );
}

static int measure()
{
    int t = 0;
    for ( int y = 0; y < grid_h; y++ ) {
        for ( int x = 0; x < grid_w; x++ ) {
            char c = data_grid[ grid( x, y ) ];
            if ( c != 'O' && c != '[' ) continue;

            t += 100 * y + x;
        }
    }

    return t;
}

int main()
{
    data_grid = new char[ grid_w * grid_h ];
    process_input();
    printf( "total: %d\n", measure() );

    grid_w = 100;
    data_grid = new char[ grid_w * grid_h ];
    process_input2();
    printf( "total: %d\n", measure() );
}
