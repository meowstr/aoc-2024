#include <stdio.h>

using ivec2 = int[ 2 ];

static char * data_grid;
static int grid_w = 141;
static int grid_h = 141;
// static int grid_w = 15;
// static int grid_h = 15;

static ivec2 start;
static ivec2 end;

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

static void process_input()
{
    FILE * file = fopen( "../day16/input.txt", "r" );

    for ( int y = 0; y < grid_h; y++ ) {
        char buffer[ 1024 ];
        char * r = fgets( buffer, 1024, file );

        for ( int x = 0; x < grid_w; x++ ) {
            data_grid[ grid( x, y ) ] = buffer[ x ];

            if ( buffer[ x ] == 'S' ) {
                start[ 0 ] = x;
                start[ 1 ] = y;
            } else if ( buffer[ x ] == 'E' ) {
                end[ 0 ] = x;
                end[ 1 ] = y;
            }
        }
    }

    print_grid();

    fclose( file );
}

struct node_t {
    int x;
    int y;
    int dir;
};

node_t * node_list;
int node_count;

int * score_list;

int * open_list;
int * closed_list;

int open_count;
int closed_count;

static int find_node( int x, int y, int dir )
{
    return 4 * grid_w * y + 4 * x + dir;
}

static int in_list( int * list, int count, int node )
{
    for ( int i = 0; i < count; i++ ) {
        if ( list[ i ] == node ) return 1;
    }

    return 0;
}

static void eval_neighbor( int parent, int x, int y, int dir )
{
    if ( x < 0 ) return;
    if ( y < 0 ) return;
    if ( x >= grid_w ) return;
    if ( y >= grid_h ) return;

    int node = find_node( x, y, dir );
    int xy = grid( x, y );

    // skip walls
    if ( data_grid[ xy ] == '#' ) return;

    int score = score_list[ parent ] + 1;

    if ( node_list[ parent ].dir != dir ) {
        score += 1000;
    }

    if ( in_list( open_list, open_count, node ) ) {
        return;
    } else if ( in_list( closed_list, closed_count, node ) ) {
        return;
    } else {
        // score
        score_list[ node ] = score;

        // append to open list
        int i = open_count++;
        open_list[ i ] = node;
    }
}

static int cheapest_open()
{
    int least = 0;
    for ( int i = 1; i < open_count; i++ ) {
        if ( score_list[ open_list[ i ] ] < score_list[ open_list[ least ] ] ) {
            least = i;
        }
    }

    return least;
}

static int pathfind()
{
    node_list = new node_t[ grid_w * grid_h * 4 ];
    score_list = new int[ grid_w * grid_h * 4 ];

    open_list = new int[ grid_w * grid_h * 4 ];
    closed_list = new int[ grid_w * grid_h * 4 ];

    open_count = 0;
    closed_count = 0;

    int start_node = find_node( start[ 0 ], start[ 1 ], 0 );

    score_list[ start_node ] = 0;

    open_count++;
    open_list[ 0 ] = start_node;

    for ( int x = 0; x < grid_w; x++ ) {
        for ( int y = 0; y < grid_h; y++ ) {
            for ( int d = 0; d < 4; d++ ) {
                node_list[ find_node( x, y, d ) ] = node_t{ x, y, d };
            }
        }
    }

    while ( open_count > 0 ) {
        // choose closest open
        int i = cheapest_open();
        int cur = open_list[ i ];

        // remove from open
        open_list[ i ] = open_list[ open_count - 1 ];
        open_count--;

        // add to closed
        int j = closed_count++;
        closed_list[ j ] = cur;

        node_t node = node_list[ cur ];

        // find neighbors and assign scores to them
        eval_neighbor( cur, node.x + 1, node.y + 0, 0 );
        eval_neighbor( cur, node.x - 1, node.y + 0, 1 );
        eval_neighbor( cur, node.x + 0, node.y + 1, 2 );
        eval_neighbor( cur, node.x + 0, node.y - 1, 3 );
    }

    return score_list[ find_node( end[ 0 ], end[ 1 ], 3 ) ];
}

void mark_neighbor( int child, int x, int y, int from_dir )
{
    if ( x < 0 ) return;
    if ( y < 0 ) return;
    if ( x >= grid_w ) return;
    if ( y >= grid_h ) return;

    int xy = grid( x, y );
    if ( data_grid[ xy ] == '#' ) return;

    for ( int dir = 0; dir < 4; dir++ ) {
        int node = find_node( x, y, dir );

        int score = score_list[ node ] + 1;

        if ( from_dir != dir ) {
            score += 1000;
        }

        if ( score == score_list[ child ] ) {
            int i = open_count++;
            open_list[ i ] = node;
        }
    }
}

void mark_paths()
{
    open_count = 1;
    open_list[ 0 ] = find_node( end[ 0 ], end[ 1 ], 3 );

    while ( open_count > 0 ) {
        int cur = open_list[ open_count - 1 ];
        open_count--;

        node_t node = node_list[ cur ];

        data_grid[ grid( node.x, node.y ) ] = 'O';

        mark_neighbor( cur, node.x + 1, node.y + 0, 1 );
        mark_neighbor( cur, node.x - 1, node.y + 0, 0 );
        mark_neighbor( cur, node.x + 0, node.y + 1, 3 );
        mark_neighbor( cur, node.x + 0, node.y - 1, 2 );
    }
}

int on_best_path( int x, int y )
{
    return data_grid[ grid( x, y ) ] == 'O';
}

int main()
{
    data_grid = new char[ grid_w * grid_h ];
    process_input();
    int best_score = pathfind();
    mark_paths();
    print_grid();

    printf( "score: %d\n", best_score );

    int seats = 0;
    for ( int x = 0; x < grid_w; x++ ) {
        for ( int y = 0; y < grid_h; y++ ) {
            if ( on_best_path( x, y ) ) seats++;
        }
    }

    printf( "seats: %d\n", seats );
}
