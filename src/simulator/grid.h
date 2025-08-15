#ifndef GRID_H
#define GRID_H
#include <vector>
// #include <string.h>

class Grid {
    public: 
        Grid();
        Grid(double m, double n);

        enum class Pattern {
            HotCell,
            HotRectangle
        };

        /**
        @brief Sets all elements of the grid to random real numbers.
        This function fills the underlying matrix with uniformly distributed random values in the range [min, max).
        @param min The inclusive lower bound for the generated values. Defaults to -10.
        @param max The exclusive upper bound for the generated values. Defaults to 10.
        */    
        void set_random(double min=-10, double max=10);

        void init(Pattern p);

        /**
        @brief Returns a pointer to the at() method of the grid's underlying std::vector structure.
        @param x The x-coordinate of the element to access.
        @param y The y-coordinate of the element to access.
        @return A pointer to the element at (x, y).
        */    
        double& at(int x, int y);

        /**
        @brief Prints the grid to stdout.
        */    
        void print();


        /**
        @brief Returns the width of the grid.
        @return The width of the grid.
        */    
        int get_width();

        /**
        @brief Returns the height of the grid.
        @return The height of the grid.
        */    
        int get_height();

        void save_to_file(const std::string& filename) const;

        std::vector<double>& get();

        bool isEqual(Grid other);

        /**
        \latexinclude ?
        @brief Determines if steady state has been achieved.
        This function returns true if the L-infinity norm difference between the current grid and the next grid is less than the tolerance.
        @param next The next step of the grid (assuming the function was called from the current grid).
        @param tolerance A decimal indicating the tolerance value.
        @return A boolean indicating steady state.
        */    
        bool isSteadyState(Grid& next, double tolerance);

    private:
        std::vector<double> matrix;
        int m = 3;
        int n = 3;
};

#endif