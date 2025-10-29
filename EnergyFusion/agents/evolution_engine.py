import random, numpy as np

class EvolutionEngine:
    def __init__(self, population):
        self.population = population
        self.generation = 0

    def evaluate_generation(self):
        for agent in self.population:
            agent.evaluate_performance()
        self.population.sort(key=lambda a: a.score, reverse=True)

    def evolve(self):
        self.generation += 1
        survivors = self.population[:len(self.population)//2]
        children = []

        for _ in range(len(survivors)//2):
            parent1, parent2 = random.sample(survivors, 2)
            child = parent1.cross(parent2)
            children.append(child)

        for child in children:
            if random.random() < 0.25:
                child.mutate()

        self.population = survivors + children
        print(f"🧬 Generation {self.generation}: Best={self.population[0].score:.3f}, Mean={np.mean([a.score for a in self.population]):.3f}")

    def get_best_agent(self):
        return max(self.population, key=lambda a: a.score)
