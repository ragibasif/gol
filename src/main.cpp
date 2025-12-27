#include <chrono>
#include <climits>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

class Board {
  private:
    int                                rows{};
    int                                cols{};
    std::vector< std::vector< bool > > matrix{};

    static int mod( const int a, const int b ) {
        if ( b == 0 ) { // b == 0 is Undefined Behavior/Division by zero error
            return 0;
        }
        if ( a == INT_MIN && b == -1 ) {
            return 0; // mathematically 0 but UB because of overflow
        }
        int r = a % b;
        if ( r < 0 ) { r += abs( b ); }
        return r;
    }

  public:
    Board( int rows, int cols ) : rows( rows ), cols( cols ) {
        for ( int row = 0; row < rows; row++ ) { matrix.emplace_back( cols ); }
    }

    void set() {
        for ( auto &row : matrix ) { fill( row.begin(), row.end(), true ); }
    }

    void clear() {
        for ( auto &row : matrix ) { fill( row.begin(), row.end(), false ); }
    }

    void update( const int row, const int col, const bool value ) {
        matrix[mod( row, rows )][mod( col, cols )] = value;
    }

    bool retrieve( const int row, const int col ) {
        return matrix[mod( row, rows )][mod( col, cols )];
    }

    void toggle( const int row, const int col ) {
        if ( retrieve( row, col ) == false ) {
            update( row, col, true );
        } else {
            update( row, col, false );
        }
    }

    void show() {
        for ( int row = 0; row < rows; row++ ) {
            for ( int col = 0; col < cols; col++ ) {
                if ( retrieve( row, col ) == true ) {
                    std::cout << 1;
                } else {
                    std::cout << 0;
                }
            }
            std::cout << "\n";
        }
    }
};

struct Pattern {
    int                               period;
    std::vector< std::vector< int > > state;
};

Pattern block    = { 1, { { 1, 1 }, { 1, 1 } } };
Pattern bee_hive = { 1, { { 0, 1, 1, 0 }, { 1, 0, 0, 1 }, { 0, 1, 1, 0 } } };
Pattern loaf     = {
    1, { { 0, 1, 1, 0 }, { 1, 0, 0, 1 }, { 0, 1, 0, 1 }, { 0, 0, 1, 0 } } };
Pattern boat    = { 1, { { 1, 1, 0 }, { 1, 0, 1 }, { 0, 1, 0 } } };
Pattern tub     = { 1, { { 0, 1, 0 }, { 1, 0, 1 }, { 0, 1, 0 } } };
Pattern blinker = { 2, { { 1, 1, 1 } } };
Pattern toad    = {
    2, { { 0, 0, 1, 0 }, { 1, 0, 0, 1 }, { 1, 0, 0, 1 }, { 0, 1, 0, 0 } } };
Pattern beacon = {
    2, { { 1, 1, 0, 0 }, { 1, 1, 0, 0 }, { 0, 0, 1, 1 }, { 0, 0, 1, 1 } } };
Pattern pulsar         = { 3,
                           { { 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
                             { 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
                             { 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
                             { 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0 },
                             { 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
                             { 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
                             { 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                             { 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0 } } };
Pattern pentadecathlon = { 15,
                           { { 0, 1, 0 },
                             { 0, 1, 0 },
                             { 1, 0, 1 },
                             { 0, 1, 0 },
                             { 0, 1, 0 },
                             { 0, 1, 0 },
                             { 0, 1, 0 },
                             { 1, 0, 1 },
                             { 0, 1, 0 },
                             { 0, 1, 0 } } };
Pattern glider         = { 20, { { 0, 1, 0 }, { 0, 0, 1 }, { 1, 1, 1 } } };
Pattern lwss           = { 20,
                           { { 0, 1, 1, 1, 1 },
                             { 1, 0, 0, 0, 1 },
                             { 0, 0, 0, 0, 1 },
                             { 1, 0, 0, 1, 0 } } };
Pattern mwss           = { 20,
                           { { 0, 0, 0, 1, 0, 0 },
                             { 0, 1, 0, 0, 0, 1 },
                             { 1, 0, 0, 0, 0, 0 },
                             { 1, 0, 0, 0, 0, 1 },
                             { 1, 1, 1, 1, 1, 0 } } };
Pattern hwss           = { 20,
                           { { 0, 0, 0, 1, 1, 0, 0 },
                             { 0, 1, 0, 0, 0, 0, 1 },
                             { 1, 0, 0, 0, 0, 0, 0 },
                             { 1, 0, 0, 0, 0, 0, 1 },
                             { 1, 1, 1, 1, 1, 1, 0 } } };

class Life {

  private:
    std::unique_ptr< Board > curr;
    std::unique_ptr< Board > prev;
    int                      rows{};
    int                      cols{};

  public:
    Life( const int rows, const int cols ) : rows( rows ), cols( cols ) {
        curr = std::make_unique< Board >( rows, cols );
        prev = std::make_unique< Board >( rows, cols );
    }

    void show() {
        std::string output = "";
        for ( int row = 0; row < rows; row++ ) {
            for ( int col = 0; col < cols; col++ ) {
                if ( curr->retrieve( row, col ) == true ) {
                    output += "# ";
                } else {
                    output += "  ";
                }
            }
            output += "\n";
        }

        std::cout << output;
    }

    void update() {
        for ( int row = 0; row < rows; row++ ) {
            for ( int col = 0; col < cols; col++ ) {

                int count = 0;
                // neighbors
                for ( int i = -1; i <= 1; i++ ) {
                    for ( int j = -1; j <= 1; j++ ) {
                        if ( prev->retrieve( row + i, col + j ) == true ) {
                            count++;
                        }
                    }
                }

                if ( prev->retrieve( row, col ) == true ) {
                    if ( count < 2 ) {
                        curr->update( row, col, false );
                    } else if ( count > 3 ) {
                        curr->update( row, col, false );
                    }
                } else {
                    if ( count == 3 ) { curr->update( row, col, true ); }
                }
            }
        }

        for ( int row = 0; row < rows; row++ ) {
            for ( int col = 0; col < cols; col++ ) {
                prev->update( row, col, curr->retrieve( row, col ) );
            }
        }
    }

    void random() {
        // seed the generator with a hardware-based random device
        std::random_device rd;
        std::mt19937       gen( rd() );

        // will uniformly choose between 0 and 1
        std::uniform_int_distribution<> dis( 0, 1 );

        // fill board randomly with 0 or 1
        for ( int row = 0; row < rows; row++ ) {
            for ( int col = 0; col < cols; col++ ) {
                int value = dis( gen );
                if ( value == 1 ) {
                    prev->update( row, col, true );
                } else {
                    prev->update( row, col, false );
                }
            }
        }
    }

    void pattern( const std::vector< std::vector< int > > &state, const int row,
                  const int col ) {
        for ( int i = 0; i < static_cast< int >( state.size() ); i++ ) {
            for ( int j = 0; j < static_cast< int >( state[i].size() ); j++ ) {
                int value = state[i][j];
                if ( value == 1 ) {
                    prev->update( row + i, col + j, true );
                } else {
                    prev->update( row + i, col + j, false );
                }
            }
        }
        std::swap( curr, prev );
    }
};

namespace ansi {

[[maybe_unused]] const std::string home  = "\033[H";  // move cursor to top-left
[[maybe_unused]] const std::string clear = "\033[2J"; // clear screen
[[maybe_unused]] const std::string hide  = "\033[?25l"; // hide cursor
[[maybe_unused]] const std::string show  = "\033[?25h"; // show cursor

void reset() { std::cout << hide << home << clear; }

} // namespace ansi

int main() {

    constexpr int rows = 1 << 5;
    constexpr int cols = 1 << 6;

    Life life( rows, cols );
    life.random();
    life.pattern( hwss.state, rows / 2, cols / 2 );
    life.pattern( lwss.state, rows / 4, cols / 3 );
    life.pattern( mwss.state, rows / 3, cols / 2 );
    life.pattern( pulsar.state, rows / 6, cols / 6 );
    life.pattern( glider.state, rows / 5, cols / 5 );
    while ( true ) {
        ansi::reset();
        life.show();
        life.update();
        std::flush( std::cout ); // print everything immediately
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }
}
