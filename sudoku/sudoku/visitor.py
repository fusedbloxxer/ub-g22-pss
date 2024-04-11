from abc import ABC, abstractmethod
from dataclasses import dataclass
from enum import StrEnum

from .problem import Problem
from .problem import State


class VisitEvent(StrEnum):
    SOLUTION = 'solution'
    INITIAL = 'initial'
    RESTART = 'restart'
    SAMPLE = 'sample'
    NEXT = 'next'


@dataclass
class SearchVisitor(ABC):
    @abstractmethod
    def visit(self, problem: Problem, state: State, event: VisitEvent) -> None:
        raise NotImplementedError()


@dataclass
class DummyVisitor(SearchVisitor):
    def visit(self, problem: Problem, state: State, event: VisitEvent) -> None:
        pass


@dataclass
class ExploreVisitor(SearchVisitor):
    def visit(self, problem: Problem, state: State, event: VisitEvent) -> None:
        match event:
            case VisitEvent.INITIAL:
                print('Sample random initial state:')
                print('Distance: {}'.format(state.dist))
                print(problem.str(state))
                self.step = 0
            case VisitEvent.SOLUTION:
                print('Found a solution at step {}:'.format(self.step))
                print(problem.str(state))
            case VisitEvent.SAMPLE:
                # print('Sample a neighbor at step {}:'.format(self.step))
                # print('Distance: {}'.format(state.dist))
                # print(problem.str(state))
                self.step += 1
            case VisitEvent.NEXT:
                # print('Accept neighbor at step {}:'.format(self.step))
                # print('Distance: {}'.format(state.dist))
                # print(problem.str(state))
                pass
            case VisitEvent.RESTART:
                # print('Restarting at step {}:'.format(self.step))
                # print(problem.str(state))
                pass