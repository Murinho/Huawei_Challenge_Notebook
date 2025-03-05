// Objective function - you can change this to your problem
ld objective(ld x) {
    return x * x;  // Minimize x^2 (parabola)
}

// Generates a neighbor (for continuous space problems)
ld random_neighbor(ld x, ld step_size) {
    return x + ((rand() / (ld)RAND_MAX) * 2 - 1) * step_size;  // Move x left or right
}

// Cooling schedule
ld reduce_temperature(ld T, ld alpha = 0.99) {
    return T * alpha;  // Geometric cooling
}

// Simulated Annealing
ld simulated_annealing(ld initial_x, ld initial_temperature, ld final_temperature) {
    ld x = initial_x;
    ld best_x = x;
    ld T = initial_temperature;

    while (T > final_temperature) {
        ld next_x = random_neighbor(x, 1.0);  // step size 1.0
        ld delta = objective(next_x) - objective(x);

        if (delta < 0 || exp(-delta / T) > ((ld)rand() / RAND_MAX)) {
            x = next_x;
        }

        if (objective(x) < objective(best_x)) {
            best_x = x;
        }

        T = reduce_temperature(T);  // Cool down
    }
    return best_x;
}

/*
objective(x) is the function we want to minimize.
random_neighbor() gives a new candidate state near the current state.
exp(-delta / T) gives the probability of accepting worse solutions.
reduce_temperature() gradually cools the system.
*/