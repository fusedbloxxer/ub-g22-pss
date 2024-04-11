from dataclasses import dataclass, field
from abc import ABC, abstractmethod
import random as rng
import numpy as np

from .visitor import SearchVisitor, DummyVisitor, ExploreVisitor
from .visitor import VisitEvent
from .problem import Problem
from .problem import State


@dataclass
class SearchAlgorithm(ABC):
    @abstractmethod
    def search(self, problem: Problem, visitor: SearchVisitor=DummyVisitor()) -> State:
        raise NotImplementedError()


@dataclass
class RandomRestartHillClimbingStochastic(SearchAlgorithm):
    threshold_restart: int = field(init=True, kw_only=True)

    def search(self, problem: Problem, visitor: SearchVisitor=DummyVisitor()) -> State:
        current: State = problem.sample_random()
        visitor.visit(problem, current, event=VisitEvent.INITIAL)

        while True:
            if current.dist == 0:
                visitor.visit(problem, current, event=VisitEvent.SOLUTION)
                return current

            for attempt in range(self.threshold_restart + 2):
                neighbor: State = problem.sample_successor(current)
                visitor.visit(problem, neighbor, event=VisitEvent.SAMPLE)

                if neighbor.dist <= current.dist:
                    current = neighbor
                    visitor.visit(problem, current, event=VisitEvent.NEXT)
                    break

                if attempt > self.threshold_restart:
                    current = problem.sample_random()
                    visitor.visit(problem, current, event=VisitEvent.RESTART)
                    break


@dataclass
class RandomRestartHillClimbingBestFirst(SearchAlgorithm):
    def search(self, problem: Problem, visitor: SearchVisitor=DummyVisitor()) -> State:
        current: State = problem.sample_random()
        visitor.visit(problem, current, event=VisitEvent.INITIAL)

        while True:
            if current.dist == 0:
                visitor.visit(problem, current, event=VisitEvent.SOLUTION)
                return current

            neighbor: State = problem.best_successor(current)
            visitor.visit(problem, neighbor, event=VisitEvent.SAMPLE)

            if neighbor.dist < current.dist:
                current = neighbor
                visitor.visit(problem, current, event=VisitEvent.NEXT)
                continue

            current = problem.sample_random()
            visitor.visit(problem, current, event=VisitEvent.RESTART)


@dataclass
class RandomRestartSimmulatedAnnealing(SearchAlgorithm):
    threshold_restart: int = 100_000

    def search(self, problem: Problem, visitor: SearchVisitor=DummyVisitor()) -> State:
        current: State = problem.sample_random()
        visitor.visit(problem, current, event=VisitEvent.INITIAL)

        t = -1
        while True:
            t += 1
            T: float = 1 - t / self.threshold_restart

            if current.dist == 0:
                visitor.visit(problem, current, event=VisitEvent.SOLUTION)
                return current

            if T == 0:
                current = problem.sample_random()
                visitor.visit(problem, current, event=VisitEvent.RESTART)
                t = 0
                continue

            next: State = problem.sample_successor(current)
            visitor.visit(problem, next, event=VisitEvent.SAMPLE)
            delta_E: int = current.dist - next.dist

            if delta_E > 0:
                current = next
                visitor.visit(problem, current, event=VisitEvent.NEXT)
            else:
                prob: float = np.exp(delta_E / T)
                choice: float = np.random.binomial(n=1, p=prob)

                if choice == 1:
                    current = next
                    visitor.visit(problem, current, event=VisitEvent.NEXT)