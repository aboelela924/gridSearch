#include <cstddef>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <functional>
#include <cassert>

#include "testing.hpp"
#include "MyGrid.hpp"

void check_grid(MyGrid &g, size_t rows, size_t cols, const Tile &initialTile) {
    // Check that the grid size matches.
    assert(g.rows() == rows);
    assert(g.cols() == cols);
    // Check all tiles.
    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            // Check that validPosition works correctly.
            assert(g.validPosition(row, col));
            assert(!g.validPosition(row + rows, col));
            assert(!g.validPosition(row - rows, col));
            assert(!g.validPosition(row, col + cols));
            assert(!g.validPosition(row, col - cols));
            // Check that both ()-operators works correctly.
            assert(g(row, col) == initialTile);
            g(row, col) = initialTile;
            assert(g(row, col) == initialTile);
            THROWS(g(row + rows, col);, std::out_of_range);
            THROWS(g(row - rows, col);, std::out_of_range);
            THROWS(g(row, col + cols);, std::out_of_range);
            THROWS(g(row, col - cols);, std::out_of_range);
        }
    }
}

void test_constructor_and_destructor() {
    TESTCASE(test_constructor_and_destructor);
    std::cout << "Testing the primary constructor." << std::endl;
    MyGrid g1(0, 0, Wall);
    check_grid(g1, 0, 0, Wall);
    MyGrid g2(1, 0, Floor);
    check_grid(g2, 1, 0, Floor);
    MyGrid g3(0, 1, Path);
    check_grid(g3, 0, 1, Path);
    MyGrid g4(100, 100, Wall);
    check_grid(g4, 100, 100, Wall);

    MyGrid g = {5, 5, Floor};
    check_grid(g, 5, 5, Floor);

    std::cout << "Testing the copy constructor." << std::endl;
    MyGrid g5{g};
    check_grid(g5, 5, 5, Floor);
    check_grid(g, 5, 5, Floor);

    std::cout << "Testing the copy assignment operator." << std::endl;
    g5 = g;
    check_grid(g5, 5, 5, Floor);
    check_grid(g, 5, 5, Floor);

    std::cout << "Testing the copy assignment operator self assignment." << std::endl;
    g5 = g5;
    check_grid(g5, 5, 5, Floor);

    std::cout << "Testing the move constructor." << std::endl;
    // Test the move constructor.
    MyGrid g6(std::move(g5));
    check_grid(g6, 5, 5, Floor);
    check_grid(g5, 0, 0, Floor);

    std::cout << "Testing the move assignment operator." << std::endl;
    MyGrid g7 = g1;
    g7 = std::move(g6);
    check_grid(g7, 5, 5, Floor);
    check_grid(g6, 0, 0, Floor);

    std::cout << "Testing self assignment." << std::endl;
    g7 = std::move(g7);
    check_grid(g7, 5, 5, Floor);
}

void check_printer(const MyGrid &g, const char *output) {
    std::stringstream s;
    s << g;
    assert(s.str() == output);
}

void test_print() {
    TESTCASE(test_print);
    MyGrid g1(0, 0, Wall);
    check_printer(g1, "0\n0\n");
    MyGrid g2(1, 1, Wall);
    check_printer(g2, "1\n1\n#\n");
    MyGrid g3(2, 3, Floor);
    check_printer(g3, "2\n3\n...\n...\n");
}

void test_read() {
    TESTCASE(test_read);
    std::stringstream s1("0\n0\n");
    MyGrid g1 = MyGrid::read(s1);
    check_grid(g1, 0, 0, Wall);

    std::stringstream s2("1\n1\n#\n");
    MyGrid g2 = MyGrid::read(s2);
    check_grid(g2, 1, 1, Wall);

    std::stringstream s3("2\n3\n...\n...\n");
    MyGrid g3 = MyGrid::read(s3);
    check_grid(g3, 2, 3, Floor);
}

int main() {
    test_constructor_and_destructor();
    test_print();
    test_read();
    std::cout << "Congratulations, all tests passed successfully." << std::endl;
}
