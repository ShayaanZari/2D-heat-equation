#include <iostream>
#include "heat.h"
#include <cmath>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

// Constructor
HeatSimulator::HeatSimulator(int m, int n, double alpha_, double dt_, double dx_) {
    grid = Grid(m, n); next = Grid(m, n); alpha = alpha_; dt = dt_; dx = dx_;
}

// Driver Method
void HeatSimulator::run(int time, bool save) {
    grid.init(Grid::Pattern::HotCell);
    int step_count = ceil(time / dt);
    for (int i=0; i<step_count; i++) {
        // sleep(1);
        step_function();

        // grid.print(); cout << endl;
        if (grid.isSteadyState(next, 0.01)) {
            cout << "Steady state achieved at t = " << fixed << setprecision(2) << i * dt << ':' << endl;
            break;
        }
        else grid = next;
    }
}

// Computation Methods

// Fixed-temp BCs (Dirichlet)
void HeatSimulator::step_dirichlet() {
    int m = grid.get_height();
    int n = grid.get_width();
    for (int x=1; x<n-1; x++) {
        for (int y=1; y<m-1; y++) {
            double curr = grid.at(x,y);
            double up = grid.at(x,y+1); // change to y-1
            double left = grid.at(x-1,y);
            double right = grid.at(x+1,y);
            double below = grid.at(x,y-1); // change to y+1
            double c = dt / pow(dx, 2); // The dimensionless Courant number
            next.at(x,y) = (curr + alpha * c
            * (up + left + right + below - 4 * curr));
        }
    }
}

// Ghost cell implementation of Neumann BCs with q=0
void HeatSimulator::step_neumann() {
    int m = grid.get_height();
    int n = grid.get_width();

    // Interior pass
    for (int x=1; x<n-1; x++) {
        for (int y=1; y<m-1; y++) {
            double curr = grid.at(x,y);
                next.at(x,y) = calc(curr, grid.at(x,y-1), grid.at(x-1,y), grid.at(x+1,y), grid.at(x,y+1));
        }
    }

    // Edge passes
    for (int i=1; i<n-1; i++) { // Top edge: below, left, right, below
        next.at(i,0) = calc(grid.at(i,0), grid.at(i,1), grid.at(i-1,0), grid.at(i+1,0), grid.at(i,1));
    }
    for (int j=1; j<m-1; j++) { // Left edge: up, right, right, below
        next.at(0,j) = calc(grid.at(0,j), grid.at(0,j-1), grid.at(1,j), grid.at(1,j), grid.at(0,j+1));
    }
    for (int j=1; j<m-1; j++) { // Right edge: up, left, left, below
        next.at(n-1,j) = calc(grid.at(n-1,j), grid.at(n-1,j-1), grid.at(n-2,j), grid.at(n-2,j), grid.at(n-1,j+1));
    }
    for (int i=1; i<n-1; i++) { // Bottom edge: up, left, right, up
        next.at(i,m-1) = calc(grid.at(i,m-1), grid.at(i,m-2), grid.at(i-1,m-1), grid.at(i+1,m-1), grid.at(i,m-2));
    }

    // Corner passes
    // UL corner: below, below, right, right
    next.at(0,0) = calc(grid.at(0,0), grid.at(0,1), grid.at(0,1), grid.at(1,0), grid.at(1,0));

    // UR corner: below, below, left, left
    next.at(n-1,0) = calc(grid.at(n-1,0), grid.at(n-1,1), grid.at(n-1,1), grid.at(n-2,0), grid.at(n-2,0));

    // BR corner: up, up, left, left
    next.at(n-1,m-1) = calc(grid.at(n-1,m-1), grid.at(n-1,m-2), grid.at(n-1,m-2), grid.at(n-2,m-1), grid.at(n-2,m-1));

    // BL corner: up, up, right, right
    next.at(0,m-1) = calc(grid.at(0,m-1), grid.at(0,m-2), grid.at(0,m-2), grid.at(1,m-1), grid.at(1,m-1));
}

// Helper Functions

// Selecting BCs type
void HeatSimulator::select_bc(StepType type) {
        switch (type) {
            case StepType::Dirichlet:
                step_function = [this]() { step_dirichlet(); };
                break;
            case StepType::Neumann:
                step_function = [this]() { step_neumann(); };
                break;
            default:
                throw std::invalid_argument("Unknown step type");
        }
}

// A helper function to slightly tidy up step functions
double HeatSimulator::calc(double curr, double up, double left, double right, double below) {
    double c = dt / pow(dx, 2);
    return (curr + alpha * c * (up + left + right + below - 4 * curr));
}

/*
void save_step(string path) {
    ostringstream filename;
    filename << "heatmap" << (i+1) << ".csv";
    grid.save_to_file(filename.str());
}
*/

// Retrieve pointer to grid
Grid& HeatSimulator::get_grid() {
    return grid;
}