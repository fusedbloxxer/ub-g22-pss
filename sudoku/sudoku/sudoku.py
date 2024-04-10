import errno
import os
import typing as t
import itertools as it
from pathlib import Path
from dataclasses import dataclass, field


@dataclass
class Sudoku(object):
    data: t.List[t.List[str]] = field(kw_only=True)
    N: int = field(kw_only=True)
    p: int = field(kw_only=True)
    q: int = field(kw_only=True)
    M: int = field(kw_only=True)

    def __post_init__(self) -> None:
        assert self.N < 36, 'N must be smaller than 36!'
        self.token_set: t.Set[str] = set(map(lambda x: str(x) if x < 10 else chr(ord('A') + x - 10), range(1, self.N + 1)))
        self.token_empty: str = '0'

    def __str__(self) -> str:
        output: t.List[str] = []
        for i in range(self.N):
            for j in range(self.N):
                output.extend([self.data[i][j], ' '])
                if (j + 1) % self.q == 0 and (j + 1) != self.N:
                    output.extend(['|', ' '])
            if (i + 1) % self.p == 0 and (i + 1) != self.N:
                output.extend(os.linesep)
                output.extend(['-', ' '] * (self.N + self.p - 1))
            output.extend(os.linesep)
        return ''.join(output)

    @staticmethod
    def from_file(path: Path | str) -> 'Sudoku':
        if isinstance(path, str):
            path = Path(path)
        if not path.exists():
            raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), str(path))
        with open(path, 'r', encoding='utf-8') as input_file:
            content = input_file.readlines()
            N, p, q, M = map(int, content[0].rstrip(os.linesep).split(' '))
            data = []
            for line in content[1:]:
                data.append(line.rstrip(os.linesep).replace('-', '0').split(' '))
        return Sudoku.from_data(data=data, N=N, p=p, q=q, M=M)

    @staticmethod
    def from_data(data: t.List[t.List[str]], N: int, p: int, q: int, M: int) -> 'Sudoku':
        if N != p * q:
            raise ValueError('N != p * q ({N} != {p} * {q})'.format(N=N, p=p, q=q))
        if N == 0:
            raise ValueError('A strictly positive number of tokens may be provided.')
        if len(data) % p != 0:
            raise ValueError('The number of blocks per row must be divisible by {p}'.format(p))
        if len(data[0]) % q != 0:
            raise ValueError('The number of block per col must be divisible by {q}'.format(q))
        if len(data) != N:
            raise ValueError('Found less rows than N: {rows} < {N}'.format(rows=len(data), N=N))
        if len(data[0]) != N:
            raise ValueError('Found less cols than N: {cols} < {N}'.format(cols=len(data[0]), N=N))
        if M != (F := sum(map(lambda _: 1, filter(lambda x: x != '0', it.chain(*data))))):
            raise ValueError('The expected amount of given tokens differs from M: {F} != {M}'.format(F=F, M=M))
        return Sudoku(data=data, N=N, p=p, q=q, M=M)

