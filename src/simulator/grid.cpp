#include <iostream>
#include "grid.h"
#include <random>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <string.h>
using namespace std;

// Constructor Methods

Grid::Grid() {
    matrix.resize(m*n, 0.0);
}

Grid::Grid(double height_, double width_) {
    m = height_;
    n = width_;
    matrix.resize(m*n, 32.0);
}

// Initialization Methods

void Grid::init(Pattern p) {    
    double radius = ceil(min(m,n) / 4.0); // e.g. 2, half of the radius of a circle inscribed within the rectangle
    double cold = 16.0;
    double warm = 150.0;
    double hot = 250.0;

    if (p == Pattern::HotCell) { // Cold grid, hot rectangle center with warm border
        double center_width = n / 2.0;
        double center_height = m / 2.0;
        for (int x=0; x<n; x++) {
            for (int y=0; y<m; y++) {
                double dist = pow(x-center_width,2) + pow(y-center_height,2);
                if (dist < radius) matrix[x+y*n] = hot;
                else if (dist < pow(radius,2)) matrix[x+y*n] = warm;
                else matrix[x+y*n] = cold;
            }
        }
    }
    else if (p == Pattern::HotRectangle) { // Cold grid, hot single cell center with warm border
        int center_width = n / 2;
        int center_height = m / 2;
        for (int y=0; y<m; y++) {
            for (int x=0; x<n; x++) {
                int x_dist = abs(x-center_width);
                int y_dist = abs(y-center_height);
                // diagonals
                if (x_dist == 1 && y_dist == 1) matrix[x+y*n] = warm;
                // plus
                else if (x_dist == 1 && y_dist == 0 || x_dist == 0 && y_dist == 1) matrix[x+y*n] = warm;
                // center
                else if (x == center_width && y == center_height) {
                    matrix[x+y*n] = hot;
                }
                // everything else
                else matrix[x+y*n] = cold;
            }
        }
    }
}

void Grid::set_random(double min, double max) {
    // modern random number generation 
    // Source: Lavavej's rand Considered Harmful https://learn.microsoft.com/en-us/events/goingnative-2013/rand-considered-harmful
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(min, max);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i+n*j] = dist(mt);
        }
    }
    // Assigning the same random number to all elements: matrix.assign(height*width, dist(mt));
}

// Helper Functions

// Steady State Convergence
bool Grid::isSteadyState(Grid& next, double tolerance) {
    std::vector<double> next_matrix = next.get();
    double absTol = 1e-6;
    double relTol = 1e-8;
    for (int i = 0; i < n*m; i++) {
        double diff = fabs(next_matrix[i] - matrix[i]);
        if (diff > tolerance) return false;
    }
    return true;
}

// If determining convergence, it is better to use a norm-based method, defined below.
bool Grid::isEqual(Grid other) {
    for (int i = 0; i < n*m; i++) {
        if (matrix[i] != other.get()[i]) return false;
    }
    return true;
}

// Printing formatted grids to terminal
void Grid::print() {
    for (int y = 0; y < m; y++) {
        for (int x = 0; x < n; x++) {
            cout << fixed << setprecision(1) << setw(6) << matrix[x+y*n];
        }
        cout << endl;
    }
    cout << endl;
}

// Saving data to output file
void Grid::save_to_file(const string& filename) const {
    ofstream output_file("/your/path/here" + filename);
    if (!output_file.is_open()) {
        std::cerr << "error: could not open file " << filename << std::endl;
        return;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            output_file << matrix[i+n*j];
            if (j < m - 1) {
                output_file << ",";
            }
        }
        output_file << std::endl;
    }
}

// Accessor and Mutator Methods

std::vector<double>& Grid::get() {
    return matrix;
}

double& Grid::at(int x, int y) {
    return matrix.at(x+n*y);
}

int Grid::get_width() {
    return n;
}

int Grid::get_height() {
    return m;
}
