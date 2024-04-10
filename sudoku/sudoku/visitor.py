from abc import ABC, abstractmethod
from dataclasses import dataclass

from .problem import State


class SearchVisitor(ABC):
    @abstractmethod
    def visit(self, state: State) -> None:
        raise NotImplementedError()


@dataclass
class DummyVisitor(SearchVisitor):
    def visit(self, state: State) -> None:
        pass
