import random, copy

class BaseAgent:
    def __init__(self, name):
        self.name = name
        self.prediction = 0.0
        self.actual = 0.0
        self.score = 0.0
        self.confidence = random.uniform(0.4, 0.6)
        self.generation = 0

    def predict(self, input_value):
        self.prediction = input_value * self.confidence
        return self.prediction

    def evaluate_performance(self):
        error = abs(self.prediction - self.actual)
        self.score = max(0, 1 - error / max(self.actual, 0.01))
        self.confidence = 0.9 * self.confidence + 0.1 * self.score

    def cross(self, other):
        child = copy.deepcopy(self)
        child.name = f"{self.name}-{other.name}_child"
        child.confidence = (self.confidence + other.confidence) / 2
        return child

    def mutate(self):
        mutation = random.uniform(-0.05, 0.05)
        self.confidence = max(0, min(1, self.confidence + mutation))
        self.name += "_mut"

    def __repr__(self):
        return f"{self.name}: score={self.score:.3f}, conf={self.confidence:.2f}"
