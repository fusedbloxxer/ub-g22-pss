import errno
import os
import typing as t
import itertools as it
from pathlib import Path


class Sudoku(object):
    def __init__(
        self,
        *,
        data: t.List[t.List[str]],
        N: int,
        p: int,
        q: int,
        M: int,
    ) -> None:
        super(Sudoku, self).__init__()

        if N != p * q:
            raise ValueError('N != p * q ({N} != {p} * {q})'.format(N=N, p=p, q=q))
        if N >= 36:
            raise ValueError('N ({}) must be smaller than 36.'.format(N))
        if N <= 0:
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

        self.data: t.List[t.List[str]] = data
        self.N: int = N
        self.p: int = p
        self.q: int = q
        self.M: int = M

        self.token_set = set(map(Sudoku.num2odo, range(1, N + 1)))
        self.token_empty = '0'

    @staticmethod
    def num2odo(x: int) -> str:
        if x < 0:
            raise ValueError('Input number cannot be negative.')
        if x == 0:
            return Sudoku.num2chr(x)

        q: int = x
        base: int = 36
        stack: t.List[int] = []

        while True:
            q, r = q // base, q % base
            stack.append(r)
            if q == 0:
                break

        return ''.join(map(Sudoku.num2chr, reversed(stack)))

    @staticmethod
    def odo2num(x: str) -> int:
        if len(x) == 0:
            raise ValueError('Received empty string.')

        num = 0
        for i, n in enumerate(map(Sudoku.chr2num, reversed(x))):
            num += n * 36**i
        return num

    @staticmethod
    def num2chr(x: int) -> str:
        if x < 0:
            raise ValueError('Input number cannot be negative.')
        if x < 10:
            return chr(ord('0') + x)
        if x < 36:
            return chr(ord('A') + x - 10)
        raise ValueError('Input number cannot be greater than 35.')

    @staticmethod
    def chr2num(x: str) -> int:
        if len(x) == 0:
            raise ValueError('Received empty string.')
        if len(x) != 1:
            raise ValueError('A single character must be given.')
        if x.isnumeric():
            return int(x)
        if x.isalnum():
            return ord(x) - ord('A') + 10
        raise ValueError('Invalid characters found in string {}'.format(x))

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
        return Sudoku(data=data, N=N, p=p, q=q, M=M)

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
