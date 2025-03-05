//Example with TSP problem.

// Parameters
const int NUM_ANTS = 10;
const int NUM_CITIES = 5;
const int MAX_ITERATIONS = 100;
const double ALPHA = 1.0;  // influence of pheromone
const double BETA = 2.0;   // influence of heuristic (1/distance)
const double EVAPORATION = 0.5;  // pheromone evaporation rate
const double Q = 100.0;     // pheromone deposited

// Distance matrix (example for 5 cities)
vector<vector<double>> distances = {
    {0, 2, 2, 3, 7},
    {2, 0, 4, 3, 6},
    {2, 4, 0, 5, 3},
    {3, 3, 5, 0, 6},
    {7, 6, 3, 6, 0}
};

// Pheromone matrix
vector<vector<double>> pheromones(NUM_CITIES, vector<double>(NUM_CITIES, 1.0));

random_device rd;
mt19937 rng(rd());

// Ant class
struct Ant {
    vi tour;
    double tourLength = 0.0;

    void visitCity(ll city) {
        tour.push_back(city);
    }

    bool visited(ll city) const {
        return find(ALL(tour), city) != tour.end();
    }

    ll lastCity() const {
        return tour.back();
    }

    void calculateTourLength() {
        tourLength = 0.0;
        for (int i = 0; i < sz(tour) - 1; ++i) {
            tourLength += distances[tour[i]][tour[i + 1]];
        }
        tourLength += distances[tour.back()][tour.front()];  // Return to start
    }

    void reset() {
        tour.clear();
        tourLength = 0.0;
    }
};

// Probability function for ant to choose next city
int selectNextCity(const Ant& ant) {
    int currentCity = ant.lastCity();
    vector<double> probabilities(NUM_CITIES, 0.0);

    double sum = 0.0;
    fore(nextCity,0,NUM_CITIES) {
        if (!ant.visited(nextCity)) {
            double pheromone = pow(pheromones[currentCity][nextCity], ALPHA);
            double heuristic = pow(1.0 / distances[currentCity][nextCity], BETA);
            probabilities[nextCity] = pheromone * heuristic;
            sum += probabilities[nextCity];
        }
    }

    // Normalize probabilities
    fore(i,0,NUM_CITIES){
        probabilities[i] /= sum;
    }

    // Roulette wheel selection
    double r = uniform_real_distribution<>(0, 1)(rng);
    double cumulative = 0.0;

    for (int i = 0; i < NUM_CITIES; ++i) {
        if (!ant.visited(i)) {
            cumulative += probabilities[i];
            if (r <= cumulative) {
                return i;
            }
        }
    }

    // Fallback (shouldn't happen)
    for (int i = 0; i < NUM_CITIES; ++i) {
        if (!ant.visited(i)) {
            return i;
        }
    }

    return -1;  // Error case (shouldn't reach here)
}

// Pheromone update
void updatePheromones(vector<Ant>& ants) {
    // Evaporation
    fore(i,0,NUM_CITIES){
        fore(j,0,NUM_CITIES){
            pheromones[i][j] *= (1 - EVAPORATION);
        }
    }

    // Deposit pheromones
    for (const auto& ant : ants) {
        double contribution = Q / ant.tourLength;
        for (int i = 0; i < sz(ant.tour) - 1; ++i) {
            int from = ant.tour[i];
            int to = ant.tour[i + 1];
            pheromones[from][to] += contribution;
            pheromones[to][from] += contribution;
        }
        // Complete tour (back to start)
        pheromones[ant.tour.back()][ant.tour.front()] += contribution;
        pheromones[ant.tour.front()][ant.tour.back()] += contribution;
    }
}

// Main ACO loop
void antColonyOptimization() {
    vector<Ant> ants(NUM_ANTS);

    double bestLength = numeric_limits<double>::max();
    vi bestTour;

    for (int iter = 0; iter < MAX_ITERATIONS; ++iter) {
        // Reset ants
        for (auto& ant : ants) {
            ant.reset();
            int startCity = uniform_int_distribution<>(0, NUM_CITIES - 1)(rng);
            ant.visitCity(startCity);
        }

        // Build tours
        for (int step = 1; step < NUM_CITIES; ++step) {
            for (auto& ant : ants) {
                int nextCity = selectNextCity(ant);
                ant.visitCity(nextCity);
            }
        }

        // Calculate tour lengths
        for (auto& ant : ants) {
            ant.calculateTourLength();
            if (ant.tourLength < bestLength) {
                bestLength = ant.tourLength;
                bestTour = ant.tour;
            }
        }

        // Update pheromones
        updatePheromones(ants);
    }

    // Output best tour found
    cout << "Best Tour Length: " << bestLength << nl;
    cout << "Best Tour: ";
    for (int city : bestTour) {
        cout << city << " ";
    }
    cout << nl;
}
