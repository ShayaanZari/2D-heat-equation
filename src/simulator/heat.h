#ifndef HEAT_H
#define HEAT_H

#include "grid.h"
#include<vector>
#include<functional>

class HeatSimulator {
    public:

        /**
        @brief Constructor for a 2D Heat Diffusion simulation.
        Loads essential grid details: dimensions, thermal diffusivity, time step, length step.
        @param height The height of the grid.
        @param width The width of the grid.
        @param alpha The thermal diffusivity of the material of the grid.
        @param dt The time step for running the simmulation.
        @param dx The length step for running the simulation. 
        */    
        HeatSimulator(int height, int width, double alpha, double dt, double dx);

        /**
        @brief Runs the given step function for the given length of time.
        This function runs FDM with Dirichlet or FDM with Neumann. If save=true, each step is saved to file as well.
        @param time The time length (in sec) to run FDM for; FDM iterates time/dt times.
        @param save A boolean indicating to save each step to file.
        */    
        void run(int step_count, bool save);

        enum class StepType {
            Dirichlet,
            Neumann
        };

        /**
        @brief Selects the boundary conditions for the simultation.
        Currently supported: Dirichlet, Neumann (q=0).
        @param type The desired boundary conditions. 
        */    
        void select_bc(StepType type);

        /**
        @brief Returns a pointer to the underlying grid of the simulation.
        @return A pointer to the underlying grid.
        */    
        Grid& get_grid();

    private:
        double alpha = 0.1;
        double dt;
        double dx;
        Grid grid;
        Grid next;
        using StepFunction = std::function<void()>;
        StepFunction step_function;
        /**
        @brief Explicit second-order Neumann scheme using center difference ghost cells.
        */
        void step_neumann();
        /**
        @brief Explicit second-order Dirichlet scheme.
        */
        void step_dirichlet();
        /**
        @brief A helper function to slightly tidy up step functions.
        It 
        */
        double calc(double curr, double up, double left, double right, double below);
};

#endif