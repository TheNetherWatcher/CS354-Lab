#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <limits>
#include "exprtk/exprtk.hpp"

using namespace std;

/**
 * Global random number generator initialized with current time
 */
static mt19937 random_engine(static_cast<unsigned long>(time(nullptr)));

/**
 * Individual struct - represents a single solution in the genetic algorithm
 */
struct Individual {
    vector<int> genes;    // Binary representation of the solution
    double fitness;       // Fitness score of this individual
};

/**
 * Converts a binary chromosome to its integer value
 */
int binaryToInt(const vector<int> &genes) {
    int value = 0;
    // Left-shift and OR operation to convert binary to decimal
    for (int bit : genes) {
        value = (value << 1) | bit;
    }
    return value;
}

/**
 * Calculates the fitness of an individual using the provided expression
 * @param ind Individual to evaluate
 * @param expr Mathematical expression to evaluate
 * @param x Reference to the x variable used in the expression
 * @param maximize Whether to maximize (true) or minimize (false) the function
 * @return Fitness value
 */
double calculateFitness(const Individual &ind, exprtk::expression<double> &expr, double &x, bool maximize) {
    // Set x to the chromosome's decoded value
    x = binaryToInt(ind.genes);
    
    // Evaluate the mathematical expression with this x value
    double value = expr.value();
    
    // Return fitness based on optimization direction
    return maximize ? value : -value;  // Negate if minimizing
}

/**
 * Selects an individual using roulette wheel selection
 * Individuals with higher fitness have higher chance of being selected
 */
Individual selectParent(const vector<Individual> &population) {
    // Calculate sum of all fitnesses
    double total = 0.0;
    for (auto &ind : population) {
        total += ind.fitness;
    }

    // If all fitnesses are non-positive, select randomly
    if (total <= 0) {
        uniform_int_distribution<int> dist(0, population.size() - 1);
        return population[dist(random_engine)];
    }

    // Roulette wheel selection
    uniform_real_distribution<double> dist(0.0, total);
    double spin = dist(random_engine);
    
    double sum = 0.0;
    for (auto &ind : population) {
        sum += ind.fitness;
        if (sum >= spin) {
            return ind;
        }
    }
    
    // Fallback (shouldn't normally reach here)
    return population.back();
}

/**
 * Performs single-point crossover between two parent chromosomes
 * Swaps genes after a randomly selected point
 */
void crossover(vector<int> &parent1, vector<int> &parent2) {
    if (parent1.size() <= 1) return; // Need at least 2 genes for crossover
    
    // Select a random crossover point (not at the beginning)
    uniform_int_distribution<int> dist(1, parent1.size() - 1);
    int point = dist(random_engine);
    
    // Swap genes after the crossover point
    for (int i = point; i < parent1.size(); ++i) {
        swap(parent1[i], parent2[i]);
    }
}

/**
 * Applies mutation to a chromosome with given probability
 * Each gene has a chance to be flipped (0→1 or 1→0)
 */
void mutate(vector<int> &genes, double mutation_rate) {
    uniform_real_distribution<double> dist(0.0, 1.0);
    
    // Check each gene for possible mutation
    for (int i = 0; i < genes.size(); ++i) {
        if (dist(random_engine) < mutation_rate) {
            // Flip the bit (0→1 or 1→0)
            genes[i] = 1 - genes[i];
        }
    }
}

/**
 * Main function - implements the genetic algorithm with ExprTk
 */
int main() {
    // -----------------------------------------
    // 1. Setup custom mathematical expression
    // -----------------------------------------
    string function_expr;
    cout << "Enter a mathematical function in terms of x (e.g., x^2 + sin(x)): ";
    // cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
    getline(cin, function_expr);

    // Initialize ExprTk components
    exprtk::symbol_table<double> symbol_table;
    double x = 0;  // Variable to be optimized
    symbol_table.add_variable("x", x);
    
    // Create expression and register symbol table
    exprtk::expression<double> expr;
    expr.register_symbol_table(symbol_table);

    // Parse the user-provided function
    exprtk::parser<double> parser;
    if (!parser.compile(function_expr, expr)) {
        cerr << "Error: Invalid function input. Exiting...\n";
        return 1;
    }

    // -----------------------------------------
    // 2. Get user parameters for the algorithm
    // -----------------------------------------
    cout << "Enter 0 to MAXIMIZE or 1 to MINIMIZE the chosen function: ";
    bool minimize;
    cin >> minimize;
    bool maximize = !minimize;

    cout << "Enter the maximum integer value x can take (e.g., 31): ";
    int max_value;
    cin >> max_value;

    cout << "Enter the binary string length (e.g., 5 for up to 31): ";
    int gene_length;
    cin >> gene_length;

    cout << "Enter population size (default 8): ";
    int pop_size;
    cin >> pop_size;

    cout << "Enter number of generations (default 10): ";
    int max_generations;
    cin >> max_generations;

    cout << "Enter crossover probability (e.g., 0.7): ";
    double crossover_rate;
    cin >> crossover_rate;

    cout << "Enter mutation probability (e.g., 0.01): ";
    double mutation_rate;
    cin >> mutation_rate;

    // -----------------------------------------
    // 3. Create initial random population
    // -----------------------------------------
    uniform_int_distribution<int> bit_dist(0, 1);
    vector<Individual> population(pop_size);
    
    // Generate random chromosomes for each individual
    for (int i = 0; i < pop_size; ++i) {
        population[i].genes.resize(gene_length);
        for (int j = 0; j < gene_length; ++j) {
            population[i].genes[j] = bit_dist(random_engine);
        }
    }

    // -----------------------------------------
    // 4. Calculate initial fitness values
    // -----------------------------------------
    for (auto &ind : population) {
        ind.fitness = calculateFitness(ind, expr, x, maximize);
    }

    // Track the best solution found so far
    Individual best_solution = population[0];

    // Helper function to update the best solution if needed
    auto updateBest = [&](const Individual &candidate) {
        if (candidate.fitness > best_solution.fitness) {
            best_solution = candidate;
        }
    };

    // -----------------------------------------
    // 5. Main genetic algorithm loop
    // -----------------------------------------
    cout << "\nGeneration | Best Chromosome | x   | f(x)\n"
         << "------------------------------------------\n";

    for (int gen = 0; gen < max_generations; ++gen) {
        // Find best solution in current population
        for (auto &ind : population) {
            updateBest(ind);
        }

        // Calculate actual function value (not fitness)
        int best_x = binaryToInt(best_solution.genes);
        double actual_value = maximize ? best_solution.fitness : -best_solution.fitness;

        // Print current best solution
        cout << gen << "           ";
        for (int bit : best_solution.genes) {
            cout << bit;
        }
        cout << "       " << best_x << "   " << actual_value << "\n";

        // Create new population
        vector<Individual> new_population(pop_size);

        // Generate offspring in pairs
        for (int i = 0; i < pop_size; i += 2) {
            // Select parents using roulette wheel selection
            Individual parent1 = selectParent(population);
            Individual parent2 = selectParent(population);

            // Apply crossover with given probability
            uniform_real_distribution<double> prob_dist(0.0, 1.0);
            if (prob_dist(random_engine) < crossover_rate) {
                crossover(parent1.genes, parent2.genes);
            }

            // Apply mutation to both offspring
            mutate(parent1.genes, mutation_rate);
            mutate(parent2.genes, mutation_rate);

            // Add offspring to new population
            new_population[i] = parent1;
            if (i + 1 < pop_size) {  // Check if we have room for the second child
                new_population[i + 1] = parent2;
            }
        }

        // Calculate fitness for new population
        for (auto &ind : new_population) {
            ind.fitness = calculateFitness(ind, expr, x, maximize);
        }

        // Replace old population with new one
        population = new_population;
    }

    // -----------------------------------------
    // 6. Final results
    // -----------------------------------------
    // Check if final population has better solutions
    for (auto &ind : population) {
        updateBest(ind);
    }
    
    // Calculate and display final best solution
    int best_x = binaryToInt(best_solution.genes);
    double final_value = maximize ? best_solution.fitness : -best_solution.fitness;

    cout << "\nFinal Best Solution:\n";
    cout << "Chromosome: ";
    for (int bit : best_solution.genes) {
        cout << bit;
    }
    cout << "\nDecoded x = " << best_x << "\n";
    cout << "f(x) = " << final_value << "\n";

    return 0;
}