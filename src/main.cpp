#include <iostream>
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

class Life {

  private:
    Board *curr{};
    Board *prev{};
    int    rows{};
    int    cols{};

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
};

int main( [[maybe_unused]] int argc, [[maybe_unused]] char **argv ) {

    Board board( ROWS, COLS );
    board.show();

    return 0;
}
