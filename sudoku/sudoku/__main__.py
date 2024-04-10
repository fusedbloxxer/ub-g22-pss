import sys
import pygame
import errno
import os
import random as rng
import pathlib as pb
import os.path as path
import typing as t
import itertools as it
import functools as fun
from pathlib import Path
from copy import deepcopy
from pprint import pprint
from functools import reduce
from collections import OrderedDict
from abc import ABC, abstractmethod
from dataclasses import dataclass, field, InitVar

from sudoku.problem import State

from .problem import Problem
from .sudoku import Sudoku


# x = 0

# for i in range(15):
#     for i in range(15):
#         for i in range(15):
#             for i in range(15):
#                 for i in range(15):
#                     for i in range(15):
#                         print(x)
#                         x += 1

# exit(0)
rng.seed(42)
sudoku: Sudoku = Sudoku.from_file(path.join('resources', 'hard.txt'))
problem = Problem(sudoku)
threshold_sample = 10000
threshold_step = 100
iteration = 0
hidden = 0
sample = 0
step = 0

state: State = problem.sample_random()
prev = state.dist
print(sudoku)

while True:
    print('Iteration: {}, Step: {}, Hidden: {}'.format(iteration, step, hidden))

    if state.dist == 0:
        print('Solution Found: ')
        print(problem.str(state))
        break

    if step > threshold_step:
        print('Reached step threshold ({}). Perform restart: '.format(threshold_step))
        state = problem.sample_random()
        print(problem.str(state))
        iteration += 1
        sample = 0
        step = 0
        continue

    state = problem.best_successor(state)

    if state.dist == prev:
        step = threshold_step + 1
        continue

    print('Dist: ', state.dist)
    prev = state.dist
    step += 1

    # while True:
    #     print('Sample: {}, Step: {}'.format(sample, step))
    #     new_state: State = problem.sample_successor(state)
    #     hidden += 1
    #     sample += 1

    #     print('DECISION: {} - {}'.format(new_state.dist, state.dist))

    #     if new_state.dist <= state.dist:
    #         print('Better state found: {} better than {}'.format(new_state.dist, state.dist))
    #         print(problem.str(new_state))
    #         state = new_state
    #         sample = 0
    #         step += 1
    #         break

    #     if sample > threshold_sample:
    #         print('Reached sample threshold ({}). Perform restart: '.format(threshold_sample))
    #         state = problem.sample_random()
    #         print(problem.str(state))
    #         iteration += 1
    #         sample = 0
    #         step = 0
    #         break