#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

constexpr int ROWS = 1 << 5;
constexpr int COLS = 1 << 5;

enum class State { Dead = 0, Alive = 1 };

class Board {
  private:
    int                                 rows{};
    int                                 cols{};
    std::vector< std::vector< State > > matrix{};

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
        for ( auto &row : matrix ) {
            fill( row.begin(), row.end(), State::Alive );
        }
    }

    void clear() {
        for ( auto &row : matrix ) {
            fill( row.begin(), row.end(), State::Dead );
        }
    }

    void update( const int row, const int col, const State value ) {
        matrix[mod( row, rows )][mod( col, cols )] = value;
    }

    State retrieve( const int row, const int col ) {
        return matrix[mod( row, rows )][mod( col, cols )];
    }

    void toggle( const int row, const int col ) {
        if ( retrieve( row, col ) == State::Dead ) {
            update( row, col, State::Alive );
        } else {
            update( row, col, State::Dead );
        }
    }

    void show() {
        for ( int row = 0; row < rows; row++ ) {
            for ( int col = 0; col < cols; col++ ) {
                if ( retrieve( row, col ) == State::Alive ) {
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

Pattern block   = { 1, { { 1, 1 }, { 1, 1 } } };
Pattern beehive = { 1, { { 0, 1, 1, 0 }, { 1, 0, 0, 1 }, { 0, 1, 1, 0 } } };
Pattern loaf    = {
    1, { { 0, 1, 1, 0 }, { 1, 0, 0, 1 }, { 0, 1, 0, 1 }, { 0, 0, 1, 0 } } };
Pattern boat = { 1, { { 1, 1, 0 }, { 1, 0, 1 }, { 0, 1, 0 } } };
Pattern tub  = { 1, { { 0, 1, 0 }, { 1, 0, 1 }, { 0, 1, 0 } } };

// Blinker (Period 2) - A simple 3-cell line
Pattern blinker = { 1, { { 1, 1, 1 } } };

// Toad (Period 2) - Two 3-cell blocks overlapping
Pattern toad = {
    2, { { 0, 0, 1, 0 }, { 1, 0, 0, 1 }, { 1, 0, 0, 1 }, { 0, 1, 0, 0 } } };

// Beacon (Period 2) - Two 2x2 blocks touching at a corner
Pattern beacon = {
    2, { { 1, 1, 0, 0 }, { 1, 1, 0, 0 }, { 0, 0, 1, 1 }, { 0, 0, 1, 1 } } };

// Pulsar (Period 3) - Large symmetrical pattern (13x13)
Pattern pulsar = { 3,
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

// Pentadecathlon (Period 15) - A 3x10 vertical oscillator
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

// Glider - The most famous small spaceship
Pattern glider = { 20, { { 0, 1, 0 }, { 0, 0, 1 }, { 1, 1, 1 } } };

// Light-weight spaceship (LWSS)
Pattern lwss = { 20,
                 { { 0, 1, 1, 1, 1 },
                   { 1, 0, 0, 0, 1 },
                   { 0, 0, 0, 0, 1 },
                   { 1, 0, 0, 1, 0 } } };

// Middle-weight spaceship (MWSS)
Pattern mwss = { 20,
                 { { 0, 0, 0, 1, 0, 0 },
                   { 0, 1, 0, 0, 0, 1 },
                   { 1, 0, 0, 0, 0, 0 },
                   { 1, 0, 0, 0, 0, 1 },
                   { 1, 1, 1, 1, 1, 0 } } };

// Heavy-weight spaceship (HWSS)
Pattern hwss = { 20,
                 { { 0, 0, 0, 1, 1, 0, 0 },
                   { 0, 1, 0, 0, 0, 0, 1 },
                   { 1, 0, 0, 0, 0, 0, 0 },
                   { 1, 0, 0, 0, 0, 0, 1 },
                   { 1, 1, 1, 1, 1, 1, 0 } } };

class Life {

  private:
    Board *curr{};
    Board *prev{};
    int    rows{};
    int    cols{};

    void copy() {
        for ( int row = 0; row < rows; row++ ) {
            for ( int col = 0; col < cols; col++ ) {
                curr->update( row, col, prev->retrieve( row, col ) );
            }
        }
    }

  public:
    Life( const int rows, const int cols ) : rows( rows ), cols( cols ) {
        curr = new Board( rows, cols );
        prev = new Board( rows, cols );
    }

    ~Life() {
        delete curr;
        curr = nullptr;
        delete prev;
        prev = nullptr;
    }

    void show() {
        std::string output = "";
        for ( int row = 0; row < rows; row++ ) {
            for ( int col = 0; col < cols; col++ ) {
                if ( curr->retrieve( row, col ) == State::Alive ) {
                    output += "# ";
                } else {
                    output += ". ";
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
                        if ( prev->retrieve( row + i, col + j ) ==
                             State::Alive ) {
                            count++;
                        }
                    }
                }

                if ( prev->retrieve( row, col ) == State::Alive ) {
                    if ( count < 2 ) {
                        curr->update( row, col, State::Dead );
                    } else if ( count > 3 ) {
                        curr->update( row, col, State::Dead );
                    }
                } else {
                    if ( count == 3 ) {
                        curr->update( row, col, State::Alive );
                    }
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
                    prev->update( row, col, State::Alive );
                } else {
                    prev->update( row, col, State::Dead );
                }
            }
        }
    }

    void pattern( const std::vector< std::vector< int > > &state, const int row,
                  const int col ) {
        prev->clear();
        curr->clear();
        for ( int i = 0; i < static_cast< int >( state.size() ); i++ ) {
            for ( int j = 0; j < static_cast< int >( state[i].size() ); j++ ) {
                int value = state[i][j];
                if ( value == 1 ) {
                    prev->update( row + i, col + j, State::Alive );
                } else {
                    prev->update( row + i, col + j, State::Dead );
                }
            }
        }
        copy();
    }
};

namespace ansi {

const std::string home  = "\033[H";  // move cursor to top-left
const std::string clear = "\033[2J"; // clear screen

void reset() { std::cout << home << clear; }

} // namespace ansi

int main( [[maybe_unused]] int argc, [[maybe_unused]] char **argv ) {

    Life life( ROWS, COLS );
    // life.random();
    life.pattern( hwss.state, ROWS / 2, COLS / 2 );
    int iterations = hwss.period;
    while ( iterations-- ) {
        ansi::reset();
        life.show();
        life.update();
        std::flush( std::cout ); // print everything immediately
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }

    return 0;
}
