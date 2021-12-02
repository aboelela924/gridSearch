#include "MyGrid.hpp"
#include <utility>
#include <cassert>
#include <algorithm>

using namespace std;

// TODO Define all member functions:
MyGrid::MyGrid(size_t rows, size_t cols, const Tile& initialTile) {
	mRows = rows;
	mCols = cols;
	mBoard = new Tile [mRows * mCols];
	//mBoard = std::unique_ptr<Tile[]>
	std::fill(&mBoard[0], &mBoard[mRows * mCols], initialTile);
}

MyGrid::MyGrid(const MyGrid& other) {

	this->mRows = other.mRows;
	this->mCols = other.mCols;
	this->mBoard = new Tile[this->mRows * this->mCols];
	std::copy(&other.mBoard[0], &other.mBoard[other.mRows * other.mCols], this->mBoard);
	/*for (size_t i = 0; (size_t)i < this->mRows * this->mCols; ++i) {
		this->mBoard[i] = other.mBoard[i];
	}*/
}

MyGrid::MyGrid(MyGrid&& other) noexcept {
	this->mRows = std::exchange(other.mRows, 0);
	this->mCols = std::exchange(other.mCols, 0);
	this->mBoard = std::exchange(other.mBoard, nullptr);
}


MyGrid& MyGrid::operator=(MyGrid&& other) noexcept {
	if (&other == this)
		return *this;

	this->mRows = other.mRows;
	this->mCols = other.mCols;
	this->mBoard = other.mBoard;

	other.mBoard = nullptr;
	other.mCols = 0;
	other.mRows = 0;

	return *this;
}

void MyGrid::print() const{
	for (size_t i = 0; i < mRows; ++i) {
		for (size_t j = 0; j < mCols; ++j) {
			cout << char_from_tile(this->mBoard[i*mCols+j]);
		}
		cout << "\n";
	}
}

MyGrid& MyGrid::operator=(const MyGrid& other) {
	this->mRows = other.mRows;
	this->mCols = other.mCols;
	this->mBoard = other.mBoard;
	return *this;
}

size_t MyGrid::rows() const {
	return mRows;
}

size_t MyGrid::cols() const {
	return mCols;
}

size_t MyGrid::size() const {
	return mRows * mCols;
}

bool MyGrid::validPosition(size_t row, size_t col) const noexcept {
	if (row >= this->mRows) return false;
	if (col >= this->mCols) return false;
	return (this->mRows * this->mCols > row*this->mCols+col);
}

Tile& MyGrid::operator()(size_t row, size_t col) {
	if (row >= this->mRows) throw std::out_of_range("out of bounds");
	if (col >= this->mCols) throw std::out_of_range("out of bounds");
	return this->mBoard[row * this->mCols + col];
}

const Tile& MyGrid::operator()(size_t row, size_t col) const{
	if (row >= this->mRows) throw std::out_of_range("out of bounds");
	if (col >= this->mCols) throw std::out_of_range("out of bounds");
	return this->mBoard[row * this->mCols + col];
}


MyGrid MyGrid::read(std::istream& in) {
	size_t cols;
	size_t rows;

	cout << "hello World" << endl;
	string line;
	in >> rows;
	in >> cols;
	
	MyGrid grid = MyGrid(rows, cols, Wall);

	
	for (size_t i = 0; i < rows; ++i) {
		in >> line;
		for (size_t j = 0; j < line.size(); ++j) {
			grid(i, j) = tile_from_char(line[j]);
		}
	}
	//grid.print();
	return grid;
}

std::ostream& operator<<(std::ostream& out, const MyGrid& grid) {
	string gridChar = "";
	for (size_t i = 0; i < grid.mRows; ++i) {
		for (size_t j = 0; j < grid.mCols; ++j) {
			gridChar += char_from_tile(grid(i, j));
		}
		gridChar += "\n";
	}
	out << grid.mRows << "\n" << grid.mCols << "\n" << gridChar;
	return out ;
}