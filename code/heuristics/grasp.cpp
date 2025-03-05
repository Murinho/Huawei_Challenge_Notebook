// Example structure for a generic GRASP
struct Solution {
    vi elements;
    double cost;

    bool operator<(const Solution& other) const {
        return cost < other.cost;
    }
};

// Random number generator
std::mt19937 rng(std::random_device{}());

// Function to evaluate solution cost (problem-specific)
double evaluate(const Solution& solution) {
    // Replace with actual evaluation logic
    return 1.0;
}

// Greedy randomized construction
Solution greedyRandomizedConstruction(const vi& candidates, double alpha) {
    Solution solution;

    vi available = candidates;

    while (sz(available)) {
        // Evaluate candidate costs
        vector<pair<int, double>> candidateCosts;
        for (int c : available) {
            // In a real problem, you would evaluate adding `c` to the solution
            double cost = static_cast<double>(c);  // Example cost (problem-specific)
            candidateCosts.emplace_back(c, cost);
        }

        // Sort candidates by cost
        sort(ALL(candidateCosts), [](const auto& a, const auto& b) { return a.snd < b.snd; });

        // Restricted Candidate List (RCL)
        double minCost = candidateCosts.front().snd;
        double maxCost = candidateCosts.back().snd;
        double threshold = minCost + alpha * (maxCost - minCost);

        vi RCL;
        for (const auto& [candidate, cost] : candidateCosts) {
            if (cost <= threshold) {
                RCL.push_back(candidate);
            }
        }

        // Select a random candidate from the RCL
        uniform_int_distribution<int> dist(0, sz(RCL) - 1);
        int selected = RCL[dist(rng)];

        // Add to solution
        solution.elements.push_back(selected);

        // Remove selected from available
        available.erase(remove(ALL(available), selected), available.end());
    }

    solution.cost = evaluate(solution);
    return solution;
}

// Local search (problem-specific)
Solution localSearch(const Solution& initial) {
    Solution best = initial;

    // Example: Simple swap local search (problem-dependent)
    for (int i = 0; i < sz(initial.elements); ++i) {
        for (int j = i + 1; j < sz(initial.elements); ++j) {
            Solution neighbor = initial;
            swap(neighbor.elements[i], neighbor.elements[j]);
            neighbor.cost = evaluate(neighbor);

            if (neighbor.cost < best.cost) {
                best = neighbor;
            }
        }
    }

    return best;
}

// GRASP algorithm
Solution grasp(const vi& candidates, int maxIterations, double alpha) {
    Solution best;
    best.cost = numeric_limits<double>::infinity();

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        Solution constructed = greedyRandomizedConstruction(candidates, alpha);
        Solution improved = localSearch(constructed);

        if (improved.cost < best.cost) {
            best = improved;
        }
    }

    return best;
}

// Example problem
int main() {
    vi candidates = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    int maxIterations = 100;
    double alpha = 0.3; // Controls greediness/randomness balance

    Solution best = grasp(candidates, maxIterations, alpha);

    cout << "Best cost: " << best.cost << "\nElements: ";
    for (int e : best.elements) {
        std::cout << e << " ";
    }
    cout << "\n";

    return 0;
}
