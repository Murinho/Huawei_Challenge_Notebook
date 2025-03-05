// Example using the TSP problem.

// Random number generator
mt19937 rng(random_device{}());

// Example distance matrix for TSP
const int N = 5;
int dist[N][N] = {
    {0, 10, 15, 20, 10},
    {10, 0, 35, 25, 15},
    {15, 35, 0, 30, 20},
    {20, 25, 30, 0, 25},
    {10, 15, 20, 25, 0}
};

// Evaluate the cost of a tour
int evaluate(const vi& tour) {
    int cost = 0;
    fore(i,0,N){
        cost += dist[tour[i]][tour[(i + 1) % N]];
    }
    return cost;
}

// Generate a random initial tour
vi randomTour() {
    vi tour(N);
    for (int i = 0; i < N; i++) tour[i] = i;
    shuffle(ALL(tour), rng);
    return tour;
}

// Tabu Search for TSP
vi tabuSearch(int maxIterations, int tabuTenure){ //amount of iterations, size of the tabu list.
    vi bestTour = randomTour();
    int bestCost = evaluate(bestTour);
    vi currentTour = bestTour;
    int currentCost = bestCost;

    // Tabu list stores pairs of cities that should not be swapped
    set<pii> tabuList;

    for (int iter = 0; iter < maxIterations; iter++) {
        vi bestNeighbor = currentTour;
        int bestNeighborCost = numeric_limits<int>::max();
        pii bestMove = {-1, -1};

        // Explore all pairwise swaps (2-opt neighborhood)
        for (int i = 0; i < N - 1; i++) {
            for (int j = i + 1; j < N; j++) {
                vi neighbor = currentTour;
                swap(neighbor[i], neighbor[j]);
                int neighborCost = evaluate(neighbor);

                pii move = {min(neighbor[i], neighbor[j]), max(neighbor[i], neighbor[j])};

                // Check if move is tabu
                bool isTabu = tabuList.count(move) > 0;

                // Aspiration criterion: accept move if it's better than global best
                if (neighborCost < bestNeighborCost && (!isTabu || (neighborCost < bestCost))) {
                    bestNeighbor = neighbor;
                    bestNeighborCost = neighborCost;
                    bestMove = move;
                }
            }
        }

        // Update current tour
        currentTour = bestNeighbor;
        currentCost = bestNeighborCost;

        // Update global best if improved
        if (currentCost < bestCost) {
            bestCost = currentCost;
            bestTour = currentTour;
        }

        // Update Tabu List: add move and enforce tabu tenure (recent moves are tabu for `tabuTenure` iterations)
        tabuList.insert(bestMove);
        if (sz(tabuList) > tabuTenure) {
            tabuList.erase(tabuList.begin());  // Simple FIFO expiration policy
        }

        cout << "Iteration " << iter + 1 << ": Best cost so far = " << bestCost << endl;
    }

    return bestTour;
}

/*
It used to solve optimization problems, particularly combinatorial problems like the 
Traveling Salesman Problem (TSP) or scheduling problems.
*/
