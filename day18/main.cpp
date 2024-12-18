#include <stdio.h>

struct node_t {
    int x;
    int y;
};

static node_t * node_grid;
static int * wall_grid;
static int grid_w = 71;
static int grid_h = 71;

static int * wall_list;
int wall_count;

static int grid( int x, int y )
{
    return y * grid_w + x;
}

static void process_input()
{
    for ( int y = 0; y < grid_h; y++ ) {
        for ( int x = 0; x < grid_w; x++ ) {
            node_grid[ grid( x, y ) ] = node_t{ x, y };
            wall_grid[ grid( x, y ) ] = 0;
        }
    }

    FILE * file = fopen( "../day18/input.txt", "r" );

    char buffer[ 1024 ];

    while ( !feof( file ) ) {
        char * r = fgets( buffer, 1024, file );

        if ( !r ) break;

        int x, y;
        sscanf( buffer, "%d, %d", &x, &y );

        wall_list[ wall_count++ ] = grid( x, y );
    }

    fclose( file );
}

static int * score_grid;

static int * open_list;
int open_count;

static int * closed_list;
int closed_count;

static int in_list( int * list, int count, int value )
{
    for ( int i = 0; i < count; i++ ) {
        if ( list[ i ] == value ) return 1;
    }
    return 0;
}

static void eval_neighbor( int parent, int x, int y )
{
    if ( x < 0 ) return;
    if ( y < 0 ) return;
    if ( x >= grid_w ) return;
    if ( y >= grid_h ) return;

    int xy = grid( x, y );
    if ( wall_grid[ xy ] ) return;

    if ( in_list( open_list, open_count, xy ) ) return;
    if ( in_list( closed_list, closed_count, xy ) ) return;

    score_grid[ xy ] = score_grid[ parent ] + 1;
    open_list[ open_count++ ] = xy;
}

static int best_open_node()
{
    int best = 0;
    for ( int i = 1; i < open_count; i++ ) {
        if ( score_grid[ open_list[ i ] ] < score_grid[ open_list[ best ] ] ) {
            best = i;
        }
    }

    return best;
}

static int pathfind()
{
    open_count = 0;
    closed_count = 0;

    int start = grid( 0, 0 );
    int end = grid( 70, 70 );

    score_grid[ start ] = 0;
    open_list[ open_count++ ] = start;

    score_grid[ end ] = -1;

    while ( open_count > 0 ) {
        int i = best_open_node();
        int node = open_list[ i ];

        if ( node == end ) break;

        open_list[ i ] = open_list[ open_count - 1 ];
        open_count--;

        closed_list[ closed_count++ ] = node;

        node_t n = node_grid[ node ];
        eval_neighbor( node, n.x + 1, n.y + 0 );
        eval_neighbor( node, n.x - 1, n.y + 0 );
        eval_neighbor( node, n.x + 0, n.y + 1 );
        eval_neighbor( node, n.x + 0, n.y - 1 );
    }

    return score_grid[ end ];
}

static void part1()
{
    process_input();

    for ( int i = 0; i < 1024; i++ ) {
        int node = wall_list[ i ];
        wall_grid[ node ] = 1;
    }

    printf( "solution: %d\n", pathfind() );
}

static void part2()
{
    process_input();

    int i;

    for ( i = 0; i < wall_count; i++ ) {
        printf( "it: %d\n", i );
        int node = wall_list[ i ];
        wall_grid[ node ] = 1;

        if ( pathfind() == -1 ) break;
    }

    int last_wall = wall_list[ i ];
    node_t node = node_grid[ last_wall ];

    printf( "solution: %d,%d\n", node.x, node.y );
}

int main()
{
    node_grid = new node_t[ grid_w * grid_h ];
    wall_grid = new int[ grid_w * grid_h ];
    wall_list = new int[ grid_w * grid_h ];
    score_grid = new int[ grid_w * grid_h ];
    open_list = new int[ grid_w * grid_h ];
    closed_list = new int[ grid_w * grid_h ];

    part1();
    part2();
}
