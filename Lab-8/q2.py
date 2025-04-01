import random
import time 

# Character substitution map
# Each key is a character, and its value is a list of visually similar variants
# Index 0 of each list is always the original character
CHAR_VARIANTS = {
    'A': ['A', '4', 'Λ', '∆'],
    'B': ['B', 'ß', 'β', '8'],
    'C': ['C', '(', 'ʗ', 'ϲ'],
    'D': ['D', 'Đ', 'ↁ'],
    'E': ['E', '3', '€', 'ε'],
    'F': ['F', 'Ƒ'],
    'G': ['G', '6', 'ɢ'],
    'H': ['H', 'ʰ', 'Ħ'],
    'I': ['I', '1', 'ɪ', '!'],
    'J': ['J', 'ʝ'],
    'K': ['K', 'Ⱪ', 'ʞ'],
    'L': ['L', '1', 'Ⱡ', 'Ł'],
    'M': ['M', 'ʍ', 'ϻ'],
    'N': ['N', 'ɴ', 'η'],
    'O': ['O', '0', 'Ø', 'Φ'],
    'P': ['P', 'Ƥ', 'ρ'],
    'Q': ['Q', 'ϙ', 'φ'],
    'R': ['R', 'ʀ', 'Я'],
    'S': ['S', '5', '$', '§'],
    'T': ['T', '7', 'ʇ'],
    'U': ['U', 'µ', 'υ'],
    'V': ['V', 'ʋ', '∨'],
    'W': ['W', 'ɯ', 'Ш'],
    'X': ['X', '×', '✕'],
    'Y': ['Y', 'Ɏ', '¥'],
    'Z': ['Z', '2', 'Ƶ'],
}

def get_variants(char):
    """Get all possible visual variants for a character"""
    # Check if we have variants for this character (in uppercase)
    if char.upper() in CHAR_VARIANTS:
        variants = CHAR_VARIANTS[char.upper()]
        # Handle lowercase characters by converting all variants to lowercase
        if char.islower():
            variants = [v.lower() for v in variants]
        # Make sure the original character is at index 0
        variants[0] = char
        return variants
    else:
        # No known variants, just return the original character
        return [char]

def create_population(text, size):
    """
    Create initial population of candidate solutions
    
    Each solution is represented as a list of integers, where each integer
    is an index into the variant list for that character position
    """
    population = []
    # Pre-compute variant options for each character in the text
    all_variants = [get_variants(c) for c in text]

    # Generate 'size' random solutions
    for _ in range(size):
        solution = []
        for variants in all_variants:
            # Choose a random variant for each character
            variant_idx = random.randint(0, len(variants) - 1)
            solution.append(variant_idx)
        population.append(solution)
    return population

def calculate_fitness(solution, text):
    """
    Calculate fitness score for a solution
    
    Higher score = better solution (more characters replaced with variants)
    """
    # Count how many characters are replaced with variants (not index 0)
    replaced_count = sum(1 for idx in solution if idx != 0)
    return replaced_count

def select_parent(population, fitness_scores):
    """
    Select a parent using fitness-proportional (roulette wheel) selection
    
    Solutions with higher fitness have higher chance of being selected
    """
    total_fitness = sum(fitness_scores)
    if total_fitness == 0:
        # If all fitness scores are 0, select randomly
        return random.choice(population)

    # Spin the roulette wheel
    selection_point = random.uniform(0, total_fitness)
    current_sum = 0.0
    
    for solution, fitness in zip(population, fitness_scores):
        current_sum += fitness
        if current_sum >= selection_point:
            return solution
            
    # Fallback in case of floating-point issues
    return population[-1]

def crossover(parent1, parent2):
    """
    Combine two parent solutions to create two child solutions
    using single-point crossover
    """
    if len(parent1) <= 1:
        # Can't do crossover with single-element solutions
        return parent1[:], parent2[:]

    # Pick a random crossover point
    cut_point = random.randint(1, len(parent1) - 1)
    
    # Create children by swapping parts at the cut point
    child1 = parent1[:cut_point] + parent2[cut_point:]
    child2 = parent2[:cut_point] + parent1[cut_point:]
    
    return child1, child2

def mutate(solution, original_text, mutation_rate):
    """
    Randomly change some values in the solution based on mutation_rate
    """
    for i in range(len(solution)):
        # For each position, mutate with probability = mutation_rate
        if random.random() < mutation_rate:
            # Get all possible variants for this character
            variants = get_variants(original_text[i])
            # Choose a new random variant
            solution[i] = random.randint(0, len(variants) - 1)
    return solution

def solution_to_text(solution, original_text):
    """Convert a solution (list of indices) back to text"""
    result = []
    for i, variant_idx in enumerate(solution):
        variants = get_variants(original_text[i])
        result.append(variants[variant_idx])
    return "".join(result)

def obfuscate_text(text, pop_size=10, generations=10, crossover_rate=0.7, mutation_rate=0.05):
    """
    Main genetic algorithm function to obfuscate text
    
    Arguments:
    - text: The original text to obfuscate
    - pop_size: Population size (number of solutions per generation)
    - generations: Number of generations to evolve
    - crossover_rate: Probability of performing crossover (vs. copying parents)
    - mutation_rate: Probability of mutating each position in a solution
    
    Returns:
    - Obfuscated version of the original text
    """
    # Step 1: Create initial random population
    population = create_population(text, pop_size)
    
    best_solution = None
    best_fitness = -1  # Starting with worst possible fitness
    
    # Step 2: Evolve over multiple generations
    for gen in range(generations):
        # Calculate fitness for each solution
        fitness_scores = [calculate_fitness(sol, text) for sol in population]
        
        # Track best solution found so far
        for solution, fitness in zip(population, fitness_scores):
            if fitness > best_fitness:
                best_fitness = fitness
                best_solution = solution[:]  # Make a copy
        
        # Report progress
        print(f"Generation {gen} | Best fitness: {best_fitness}")
        
        # Create new population for next generation
        new_population = []
        
        # Keep creating children until new population is filled
        while len(new_population) < pop_size:
            # Select parents based on fitness
            parent1 = select_parent(population, fitness_scores)
            parent2 = select_parent(population, fitness_scores)
            
            # Perform crossover with probability = crossover_rate
            if random.random() < crossover_rate:
                child1, child2 = crossover(parent1, parent2)
            else:
                # No crossover, just copy the parents
                child1, child2 = parent1[:], parent2[:]
            
            # Mutate children
            child1 = mutate(child1, text, mutation_rate)
            child2 = mutate(child2, text, mutation_rate)
            
            # Add children to new population
            new_population.append(child1)
            if len(new_population) < pop_size:  # Avoid exceeding population size
                new_population.append(child2)
        
        # Replace old population with new one
        population = new_population
    
    # Final check for best solution (in case the last generation had better solutions)
    fitness_scores = [calculate_fitness(sol, text) for sol in population]
    for solution, fitness in zip(population, fitness_scores):
        if fitness > best_fitness:
            best_fitness = fitness
            best_solution = solution[:]
    
    # Convert best solution back to text
    return solution_to_text(best_solution, text)

# Set random seed using current time
random.seed(time.time()) 

# Text to be obfuscated
original_text = "HELLO BK"

# Genetic algorithm parameters
POP_SIZE = 10        # Number of solutions per generation
GENERATIONS = 10     # Number of evolution cycles
CROSSOVER_RATE = 0.7 # Probability of performing crossover
MUTATION_RATE = 0.1  # Probability of mutation per character

# Run the genetic algorithm
obfuscated_text = obfuscate_text(
    original_text,
    pop_size=POP_SIZE,
    generations=GENERATIONS,
    crossover_rate=CROSSOVER_RATE,
    mutation_rate=MUTATION_RATE
)

# Display results
print("\nOriginal text:", original_text)
print("Obfuscated text:", obfuscated_text)