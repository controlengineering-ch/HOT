from base_agent import BaseAgent
from evolution_engine import EvolutionEngine
import random

population = [BaseAgent(f"Agent_{i}") for i in range(10)]
engine = EvolutionEngine(population)

for gen in range(5):
    for agent in engine.population:
        agent.actual = random.uniform(0.5, 1.5)
        agent.predict(agent.actual * random.uniform(0.8, 1.2))
    engine.evaluate_generation()
    engine.evolve()

print(f"\n🌟 Best agent: {engine.get_best_agent()}")
