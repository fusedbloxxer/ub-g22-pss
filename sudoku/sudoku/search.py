from abc import ABC, abstractmethod
from dataclasses import dataclass

from .problem import Problem, State
from .visitor import DummyVisitor, SearchVisitor


@dataclass
class SearchAlgorithm(ABC):
    @abstractmethod
    def search(self, problem: Problem, visitor: SearchVisitor=DummyVisitor()) -> State:
        raise NotImplementedError()


@dataclass
class RandomRestartHillClimbing(SearchAlgorithm):
    def search(self, problem: Problem, visitor: SearchVisitor=DummyVisitor()) -> State:
        return State()
