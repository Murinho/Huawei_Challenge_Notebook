// Random number generator
mt19937 rng(time(0));

// Parameters
const int POPULATION_SIZE = 100;
const double MUTATION_RATE = 0.1;
const int GENERATIONS = 100;

// Chromosome type (can be adjusted for different problems)
using Chromosome = vi;

// Problem-specific settings (binary string length)
const int CHROMOSOME_LENGTH = 20;

// Function to create a random chromosome
Chromosome randomChromosome() {
    Chromosome chrom(CHROMOSOME_LENGTH);
    for (ll &gene : chrom) {
        gene = rng() % 2;  // Binary chromosome (0 or 1)
    }
    return chrom;
}

// Example fitness function (number of 1s in the chromosome)
ld fitness(const Chromosome &chrom) {
    return std::count(ALL(chrom), 1);
}

// Selection (tournament selection)
Chromosome tournamentSelection(const std::vector<Chromosome> &population) {
    int bestIdx = rng() % POPULATION_SIZE;
    fore(i,0,3) {
        int idx = rng() % POPULATION_SIZE;
        if (fitness(population[idx]) > fitness(population[bestIdx])) {
            bestIdx = idx;
        }
    }
    return population[bestIdx];
}

// Crossover (single point crossover)
Chromosome crossover(const Chromosome &parent1, const Chromosome &parent2) {
    ll point = rng() % CHROMOSOME_LENGTH;
    Chromosome offspring = parent1;
    for (int i = point; i < CHROMOSOME_LENGTH; i++) {
        offspring[i] = parent2[i];
    }
    return offspring;
}

// Mutation (flip bits)
void mutate(Chromosome &chrom) {
    for (ll &gene : chrom) {
        if ((rng() % 100) < (MUTATION_RATE * 100)) {
            gene = 1 - gene;
        }
    }
}

// Genetic Algorithm main loop
void geneticAlgorithm() {
    // Initialize population
    vector<Chromosome> population(POPULATION_SIZE);
    for (Chromosome &chrom : population) {
        chrom = randomChromosome();
    }

    ld bestFitness;
    Chromosome bestChromosome;

    for (int gen = 0; gen < GENERATIONS; gen++) {
        vector<Chromosome> newPopulation;

        for (int i = 0; i < POPULATION_SIZE; i++) {
            Chromosome parent1 = tournamentSelection(population);
            Chromosome parent2 = tournamentSelection(population);

            Chromosome offspring = crossover(parent1, parent2);
            mutate(offspring);

            newPopulation.push_back(offspring);
        }

        population = newPopulation;

        for (const Chromosome &chrom : population) {
            ld this_fitness = fitness(chrom);
            if (bestFitness < this_fitness){
                bestFitness = this_fitness;
                bestChromosome = chrom;
            }
        }
        std::cout << "Generation " << gen << " - Best fitness: " << bestFitness << "\n";
    }

    // Print the best chromosome (best solution) and its fitness.
}

// Main
int main() {
    geneticAlgorithm();
    return 0;
}
