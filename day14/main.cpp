#include <stdio.h>

static int * robot_x;
static int * robot_y;
static int * robot_vx;
static int * robot_vy;
static int robot_count;

static void setup_robot_table()
{
    int cap = 1024;
    robot_x = new int[ cap ];
    robot_y = new int[ cap ];
    robot_vx = new int[ cap ];
    robot_vy = new int[ cap ];
}

static void read_input()
{
    FILE * file = fopen( "../day14/input.txt", "r" );

    while ( !feof( file ) ) {
        char buffer[ 1024 ];
        char * r = fgets( buffer, 1024, file );

        if ( !r ) break;

        int i = robot_count++;
        sscanf(
            buffer,
            "p=%d,%d v=%d,%d",
            robot_x + i,
            robot_y + i,
            robot_vx + i,
            robot_vy + i
        );
    }

    fclose( file );

    for ( int i = 0; i < robot_count; i++ ) {
        printf(
            "p=%d,%d v=%d,%d\n",
            robot_x[ i ],
            robot_y[ i ],
            robot_vx[ i ],
            robot_vy[ i ]
        );
    }
}

static void move_robots( int step )
{
    for ( int i = 0; i < robot_count; i++ ) {
        // move
        robot_x[ i ] += robot_vx[ i ] * step;
        robot_y[ i ] += robot_vy[ i ] * step;

        // wrap
        robot_x[ i ] %= 101;
        robot_y[ i ] %= 103;
        if ( robot_x[ i ] < 0 ) robot_x[ i ] += 101;
        if ( robot_y[ i ] < 0 ) robot_y[ i ] += 103;
    }
}

static int compute_safety()
{
    static int quad[ 4 ];

    for ( int i = 0; i < robot_count; i++ ) {
        int x = robot_x[ i ] - 101 / 2;
        int y = robot_y[ i ] - 103 / 2;

        if ( x < 0 && y < 0 ) quad[ 0 ]++;
        if ( x > 0 && y < 0 ) quad[ 1 ]++;
        if ( x < 0 && y > 0 ) quad[ 2 ]++;
        if ( x > 0 && y > 0 ) quad[ 3 ]++;
    }

    return quad[ 0 ] * quad[ 1 ] * quad[ 2 ] * quad[ 3 ];
}

static int count_robots_in( int x, int y )
{
    int c = 0;
    for ( int i = 0; i < robot_count; i++ ) {
        if ( robot_x[ i ] == x && robot_y[ i ] == y ) c++;
    }
    return c;
}

static void show_diagram()
{
    for ( int y = 0; y < 103; y++ ) {
        for ( int x = 0; x < 101; x++ ) {
            int count = count_robots_in( x, y );
            printf( "%c ", ( count > 0 ) ? '*' : ' ' );
        }
        printf( "\n" );
    }
}

static float variance()
{
    float mean_x = 0.0f;
    float mean_y = 0.0f;
    for ( int i = 0; i < robot_count; i++ ) {
        mean_x += (float) robot_x[ i ] / robot_count;
        mean_y += (float) robot_y[ i ] / robot_count;
    }

    float var = 0.0f;
    for ( int i = 0; i < robot_count; i++ ) {
        float zx = robot_x[ i ] - mean_x;
        float zy = robot_y[ i ] - mean_y;
        var += ( zx * zx + zy * zy ) / robot_count;
    }

    return var;
}

int main()
{
    printf( "meow\n" );

    setup_robot_table();
    read_input();

    // part 1
    // move_robots( 100 );
    // printf( "solution: %d\n", compute_safety() );

    // part 2
    int best_t = 0;
    float best_var = variance();

    for ( int i = 0; i < 10000; i++ ) {
        move_robots( 1 );

        float var = variance();
        if ( var < best_var ) {
            best_t = i + 1;
            best_var = var;
        }
    }

    // reload robots
    robot_count = 0;
    read_input();

    move_robots( best_t );

    show_diagram();

    printf( "time: %d\n", best_t );
}
