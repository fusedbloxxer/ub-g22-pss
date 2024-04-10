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
    row_dist: t.List[int] = field(init=False, default_factory=list)
    col_dist: t.List[int] = field(init=False, default_factory=list)
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

    def values(self, state: State) -> t.Sequence[str]:
        return state.values[self.start: self.end]

    def range(self) -> t.Generator[int, None, None]:
        return (i for i in range(self.start, self.end))

    @overload
    def __getitem__(self, pos: int) -> t.Tuple[int, int]:
        ...
    @overload
    def __getitem__(self, pos: t.Tuple[int, int]) -> int:
        ...
    def __getitem__(self, pos: t.Tuple[int, int] | int) -> t.Tuple[int, int] | int:
        return self.start + self.pos2var[pos] if isinstance(pos, tuple) else self.var2pos[pos - self.start]


@dataclass
class Problem(object):
    sudoku: Sudoku

    def __post_init__(self) -> None:
        self.blks: t.List[Block] = [Block() for _ in range(self.sudoku.N)]

        for i in range(self.sudoku.N):
            for j in range(self.sudoku.N):
                blk_idx: int = self.get_blk_idx(i, j)

                if self.sudoku.token_empty == (element := self.sudoku.data[i][j]):
                    self.blks[blk_idx].pos2var[(i, j)] = len(self.blks[blk_idx].var2pos)
                    self.blks[blk_idx].var2pos.append((i, j))
                    continue

                self.blks[blk_idx].fix_set.add(element)

        for i, blk in enumerate(self.blks):
            blk.var_set = self.sudoku.token_set.difference(blk.fix_set)
            blk.start = 0 if i == 0 else self.blks[i - 1].end
            blk.size = len(blk.var_set)
            blk.end = blk.start + blk.size

        init_state = self.sample_random()
        next_state = self.sample_successor(init_state)

        print('init')
        print(self.str(init_state))
        print('succ')
        print(self.str(next_state))

    def sample_random(self) -> State:
        state = State()
        for blk in self.blks:
            population = list(blk.var_set)
            rng.shuffle(population)
            state.values.extend(population)
        for i in range(self.sudoku.N):
            rdist: int = self.rdist(state, i)
            cdist: int = self.cdist(state, i)
            state.row_dist.append(rdist)
            state.col_dist.append(cdist)
            state.dist += rdist + cdist
        return state

    def best_successor(self, state: State) -> State:
        # Copy in-depth to avoid side effects
        best = state
        for blk in self.blks:
            for i in range(blk.start, blk.end - 1):
                for j in range(i + 1, blk.end):
                    succesor = self.swap(deepcopy(state), i, j, blk)
                    if succesor.dist < best.dist:
                        best = succesor
        return best

    def sample_successor(self, state: State) -> State:
        # Copy in-depth to avoid side effects
        state = deepcopy(state)

        # Sample a region (TODO: case for filled blocks)
        blk: Block = rng.choice(self.blks)

        # Sample two elements (TODO: case for 1 elem)
        population: t.List[int] = rng.sample(list(blk.range()), k=2)
        idx_A: int = population[0]; idx_B: int = population[1]
        return self.swap(state, idx_A, idx_B, blk)

    def swap(self, state: State, idx_A: int, idx_B: int, blk: Block) -> State:
        # Swap a pair of elements inside a single region
        state.values[idx_A], state.values[idx_B] = state.values[idx_B], state.values[idx_A]

        # Recompute cached dist
        pos_A: t.Tuple[int, int] = blk[idx_A]
        pos_B: t.Tuple[int, int] = blk[idx_B]

        # Remove previous values
        if pos_A[0] != pos_B[0]:
            state.dist -= state.row_dist[pos_B[0]]
        state.dist -= state.row_dist[pos_A[0]]

        if pos_A[1] != pos_B[1]:
            state.dist -= state.col_dist[pos_B[1]]
        state.dist -= state.col_dist[pos_A[1]]

        # Cache new values
        state.row_dist[pos_A[0]] = self.rdist(state, pos_A[0])
        state.row_dist[pos_B[0]] = self.rdist(state, pos_B[0])
        state.col_dist[pos_A[1]] = self.cdist(state, pos_A[1])
        state.col_dist[pos_B[1]] = self.cdist(state, pos_B[1])

        # Compute new distance
        if pos_A[0] != pos_B[0]:
            state.dist += state.row_dist[pos_B[0]]
        state.dist += state.row_dist[pos_A[0]]

        if pos_A[1] != pos_B[1]:
            state.dist += state.col_dist[pos_B[1]]
        state.dist += state.col_dist[pos_A[1]]

        # Continue searching
        return state

    def rdist(self, state: State, i: int) -> int:
        seen: t.Dict[str, int] = defaultdict(int)
        for j in range(self.sudoku.N):
            seen[self.get_state_item(state, i, j)] += 1
        return sum([val - 1 for val in seen.values()])

    def cdist(self, state: State, j: int) -> int:
        seen: t.Dict[str, int] = defaultdict(int)
        for i in range(self.sudoku.N):
            seen[self.get_state_item(state, i, j)] += 1
        return sum([val - 1 for val in seen.values()])

    def get_state_item(self, state: State, i: int, j: int) -> str:
        if (element := self.sudoku.data[i][j]) != self.sudoku.token_empty:
            return element
        state_idx: int = self.blks[self.get_blk_idx(i, j)][(i, j)]
        return state.values[state_idx]

    @overload
    def get_blk(self, i: int) -> Block:
        ...
    @overload
    def get_blk(self, i: int, j: int) -> Block:
        ...
    def get_blk(self, i: int, j: t.Optional[int] = None) -> Block:
        if j is None:
            return self.blks[i]
        return self.blks[self.get_blk_idx(i, j)]

    def get_blk_idx(self, i: int, j: int) -> int:
        blk_row: int = i // self.sudoku.p
        blk_col: int = j // self.sudoku.q
        blk_idx: int = blk_row * self.sudoku.p + blk_col
        return blk_idx

    def str(self, state: State) -> str:
        output: t.List[str] = []
        for i in range(self.sudoku.N):
            for j in range(self.sudoku.N):
                item: str = self.get_state_item(state, i, j)
                output.extend([item, ' '])
                if (j + 1) % self.sudoku.q == 0 and (j + 1) != self.sudoku.N:
                    output.extend(['|', ' '])
            if (i + 1) % self.sudoku.p == 0 and (i + 1) != self.sudoku.N:
                output.extend(os.linesep)
                output.extend(['-', ' '] * (self.sudoku.N + self.sudoku.p - 1))
            output.extend(os.linesep)
        return ''.join(output)
