#pragma once

#include <memory>
#include <stdexcept>
#include <iostream>
#include "Tile.hpp"

using namespace std;

class MyGrid {
public:
    MyGrid(size_t rows, size_t cols, const Tile &initialTile);

    MyGrid(const MyGrid &other);

    MyGrid(MyGrid &&other) noexcept;

    MyGrid &operator=(MyGrid &&other) noexcept;

    // The Destructor
    // Frees all resources associated with this grid.
    ~MyGrid() = default;

    // The Copy Assignment Operator
    // Overwrites the size and contents of a grid with those of another
    // supplied grid.
    MyGrid &operator=(const MyGrid &other);

    // The Grid Size Access Functions
    // These functions should return the grid size that have been passed to
    // the constructor.
    [[nodiscard]] size_t rows() const;
    [[nodiscard]] size_t cols() const;
    [[nodiscard]] size_t size() const;

    // The Index Checker
    // Returns true when the supplied indices reference a valid grid
    // element, and false otherwise.
    [[nodiscard]] bool validPosition(size_t row, size_t col) const noexcept;

    // The Element Access Function
    // Returns a constant reference to the tile at position row, col of the
    // grid.  Throws an invalid_grid_position exception if the specified
    // grid position is not valid.
    Tile &operator()(size_t row, size_t col);

    // The Element Mutation Function
    // Returns a mutable reference to the tile at position row, col of the
    // grid.  Throws an invalid_grid_position exception if the specified
    // grid position is not valid.
    const Tile &operator()(size_t row, size_t col) const;

    //Create and read in a new MyGrid
    static MyGrid read(std::istream &in);

    // Declare std::ostreams << operator as 'friend'.  Otherwise, its
    // implementation wouldn't be allowed to access the protected functions
    // of the Grid class.
    // Prints a Grid to the supplied output stream.
    friend std::ostream& operator<<(std::ostream& out, const MyGrid& grid);


    void print() const;
private:
    // TODO Add custom member functions and variables here:
    size_t mRows, mCols;
    Tile* mBoard;
    // int agentPosition[2] = {0, 0};
};
