import os.path as path
import random as rng
import typing as t

from .search import RandomRestartSimmulatedAnnealing, RandomRestartHillClimbingBestFirst, RandomRestartHillClimbingStochastic
from .visitor import ExploreVisitor
from .problem import Problem
from .sudoku import Sudoku


sudoku: Sudoku = Sudoku.from_file(path.join('resources', 'hard.txt'))

visitor = ExploreVisitor()

problem = Problem(sudoku)

# strategy = RandomRestartSimmulatedAnnealing(threshold_restart=500)
strategy = RandomRestartHillClimbingStochastic(threshold_restart=750)
# strategy = RandomRestartHillClimbingBestFirst()

strategy.search(problem, visitor)
