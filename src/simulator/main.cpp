#include <iostream>
#include "grid.h"
#include "heat.h"
using namespace std;

/**
 * Observations:
 * - Decreasing dx leads to faster convergence to steady state
 * - Decreasing alpha causes faster convergence to steady state
 */

int main() {
    double diffusivity = 0.00127; // gold
    HeatSimulator sim = HeatSimulator(7, 11, 0.3, 0.1, 0.5);
    sim.select_bc(HeatSimulator::StepType::Neumann);
    sim.get_grid().init(Grid::Pattern::HotCell);

    cout << "Initial state: " << endl;
    sim.get_grid().print();
    sim.run(25, true);
    sim.get_grid().print();
    return 0;
}

