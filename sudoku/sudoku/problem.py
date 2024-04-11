import os
import typing as t
import random as rng
from copy import deepcopy
from pprint import pprint
from typing import overload
from dataclasses import field
from dataclasses import dataclass
from collections import defaultdict

from .sudoku import Sudoku


@dataclass
class State(object):
    values: t.List[str] = field(default_factory=list)
    dist_row: t.List[int] = field(init=False, default_factory=list)
    dist_col: t.List[int] = field(init=False, default_factory=list)
    dist: int = field(init=False, default=0)


@dataclass
class Block(object):
    pos2var: t.Dict[t.Tuple[int, int], int] = field(default_factory=dict)
    var2pos: t.List[t.Tuple[int, int]] = field(default_factory=list)

    var_set: t.Set[str] = field(default_factory=set)
    fix_set: t.Set[str] = field(default_factory=set)

    start: int = field(default=0)
    size: int = field(default=0)
    end: int = field(default=0)

    def __contains__(self, pos: t.Tuple[int, int]) -> bool:
        return pos in self.pos2var

    def __iter__(self) -> t.Generator[int, None, None]:
        return (i for i in range(self.start, self.end))

    def empty(self) -> bool:
        return self.size == 0

    @overload
    def __getitem__(self, pos: int) -> t.Tuple[int, int]:
        ...
    @overload
    def __getitem__(self, pos: t.Tuple[int, int]) -> int:
        ...
    def __getitem__(self, pos: t.Tuple[int, int] | int) -> t.Tuple[int, int] | int:
        return self.start + self.pos2var[pos] if isinstance(pos, tuple) else self.var2pos[pos - self.start]


class Board(object):
    def __init__(self, sudoku: Sudoku) -> None:
        super(Board, self).__init__()
        self.q: int = sudoku.q
        self.p: int = sudoku.p
        self.N: int = sudoku.N
        self.__sudoku: Sudoku = sudoku
        self.__blocks: t.List[Block] = [Block() for _ in range(self.__sudoku.N)]

        for i in range(self.__sudoku.N):
            for j in range(self.__sudoku.N):
                block: Block = self.get((i, j), where='block')
                if self.__sudoku.token_empty == (element := self.__sudoku.data[i][j]):
                    block.pos2var[(i, j)] = len(block.var2pos)
                    block.var2pos.append((i, j))
                    continue
                block.fix_set.add(element)

        for i, blk in enumerate(self.__blocks):
            blk.var_set = self.__sudoku.token_set.difference(blk.fix_set)
            blk.start = 0 if i == 0 else self.__blocks[i - 1].end
            blk.size = len(blk.var_set)
            blk.end = blk.start + blk.size

    def __iter__(self) -> t.Generator[Block, None, None]:
        return (block for block in self.__blocks)

    def __getitem__(self, pos: t.Tuple[int, int]) -> str:
        return self.get(pos, where='board')

    @overload
    def get(self, pos: t.Tuple[int, int], where: t.Literal['board'], state: State | None=None) -> str:
        ...
    @overload
    def get(self, pos: t.Tuple[int, int] | int, where: t.Literal['block'],  state: None=None) -> Block:
        ...
    def get(self, pos: t.Tuple[int, int] | int, where: t.Literal['board', 'block'], state: State | None = None) -> Block | str:
        if where == 'block':
            if state is not None:
                raise ValueError('When querying by block, no state is expected.')
            if isinstance(pos, int):
                return self.__blocks[pos]
            if isinstance(pos, tuple):
                block_row: int = pos[0] // self.__sudoku.p
                block_col: int = pos[1] // self.__sudoku.q
                block_idx: int = block_row * self.__sudoku.p + block_col
                return self.__blocks[block_idx]
        if isinstance(pos, int):
            raise ValueError('A board may be indexed only by a pair (i, j).')
        if (element := self.__sudoku.data[pos[0]][pos[1]]) != self.__sudoku.token_empty:
            return element
        return '0' if state is None else state.values[self.get(pos, where='block')[pos]]

    def __len__(self) -> int:
        return len(self.__blocks)


class Problem(object):
    def __init__(self, sudoku: Sudoku) -> None:
        self.board = Board(sudoku)

    def sample_random(self) -> State:
        state = State()

        for block in self.board:
            population = list(block.var_set)
            rng.shuffle(population)
            state.values.extend(population)

        for i in range(len(self.board)):
            rdist: int = self.rdist(state, i)
            cdist: int = self.cdist(state, i)
            state.dist_row.append(rdist)
            state.dist_col.append(cdist)
            state.dist += rdist + cdist

        return state

    def best_successor(self, state: State) -> State:
        best: State = state

        for block in self.board:
            for i in range(block.start, block.end):
                for j in range(i + 1, block.end):
                    succesor: State = self.swap(state, i, j, block)

                    if succesor.dist < best.dist:
                        best = succesor

        return best

    def sample_successor(self, state: State) -> State:
        # Sample one block from the non-empty ones
        block: Block = rng.choice(list(filter(lambda block: not block.empty(), self.board)))

        # Sample two elements
        population: t.List[int] = rng.sample(list(block), k=2)
        idx_A: int = population[0]
        idx_B: int = population[1]

        # Interchange them to generate a new state
        return self.swap(state, idx_A, idx_B, block)

    def swap(self, state: State, idx_A: int, idx_B: int, block: Block) -> State:
        # Make all operations on a copy
        state = deepcopy(state)

        # Swap a pair of elements inside a single region
        state.values[idx_A], state.values[idx_B] = state.values[idx_B], state.values[idx_A]

        # Recompute cached dist
        pos_A: t.Tuple[int, int] = block[idx_A]
        pos_B: t.Tuple[int, int] = block[idx_B]
        # print(f'Generate State: {pos_A} <-> {pos_B}')

        # Remove previous values
        if pos_A[0] != pos_B[0]:
            state.dist -= state.dist_row[pos_B[0]]
        state.dist -= state.dist_row[pos_A[0]]

        if pos_A[1] != pos_B[1]:
            state.dist -= state.dist_col[pos_B[1]]
        state.dist -= state.dist_col[pos_A[1]]

        # Cache new values
        state.dist_row[pos_A[0]] = self.rdist(state, pos_A[0])
        state.dist_row[pos_B[0]] = self.rdist(state, pos_B[0])
        state.dist_col[pos_A[1]] = self.cdist(state, pos_A[1])
        state.dist_col[pos_B[1]] = self.cdist(state, pos_B[1])

        # Compute new distance
        if pos_A[0] != pos_B[0]:
            state.dist += state.dist_row[pos_B[0]]
        state.dist += state.dist_row[pos_A[0]]

        if pos_A[1] != pos_B[1]:
            state.dist += state.dist_col[pos_B[1]]
        state.dist += state.dist_col[pos_A[1]]

        # Child state
        return state

    def rdist(self, state: State, i: int) -> int:
        seen: t.Dict[str, int] = defaultdict(int)
        for j in range(len(self.board)):
            seen[self.board.get((i, j), 'board', state)] += 1
        return sum([val - 1 for val in seen.values()])

    def cdist(self, state: State, j: int) -> int:
        seen: t.Dict[str, int] = defaultdict(int)
        for i in range(len(self.board)):
            seen[self.board.get((i, j), 'board', state)] += 1
        return sum([val - 1 for val in seen.values()])

    def str(self, state: State) -> str:
        output: t.List[str] = []
        for i in range(len(self.board)):
            for j in range(len(self.board)):
                item: str = self.board.get((i, j), 'board', state)
                output.extend([item, ' '])
                if (j + 1) % self.board.q == 0 and (j + 1) != self.board.N:
                    output.extend(['|', ' '])
            if (i + 1) % self.board.p == 0 and (i + 1) != self.board.N:
                output.extend(os.linesep)
                output.extend(['-', ' '] * (self.board.N + self.board.p - 1))
            output.extend(os.linesep)
        return ''.join(output)
