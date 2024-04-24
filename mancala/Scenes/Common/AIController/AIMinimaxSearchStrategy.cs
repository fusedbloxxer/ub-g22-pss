using Godot;
using System;
using System.Collections.Generic;
using System.Linq;

public partial class AIMinimaxSearchStrategy : AISearchStrategy
{
	private int cutoff;

	public AIMinimaxSearchStrategy(int cutoff)
	{
		this.cutoff = cutoff;
	}

	public override IEnumerable<int> Search(GameBoardState board, Player maxPlayer)
	{
		var state = new State(board, maxPlayer.Name);

		var (_, move) = MaxValue(state, maxPlayer, int.MinValue, int.MaxValue, 0);

		return move?.Index ?? new List<int>();
	}

	private (int, GameAction) MaxValue(State state, Player maxPlayer, int alpha, int beta, int depth)
	{
		if (state.IsTerminal())
		{
			return (state.Utility(), null);
		}

		if (depth > cutoff)
		{
			return (state.Eval(maxPlayer.Name), null);
		}

		int value = int.MinValue;
		GameAction move = null;

		foreach (var action in state.Actions())
		{
			var (childValue, childAction) = MinValue(state.Result(action), maxPlayer, alpha, beta, depth + 1);
		
			if (childValue > value)
			{
				(value, move) = (childValue, action);
				alpha = Math.Max(alpha, value);
			}

			if (value >= beta)
			{
				return (value, move);
			}
		}

		return (value, move);
	}

	private (int, GameAction) MinValue(State state, Player maxPlayer, int alpha, int beta, int depth)
	{
		if (state.IsTerminal())
		{
			return (state.Utility(), null);
		}

		if (depth > cutoff)
		{
			return (state.Eval(maxPlayer.Name), null);
		}

		int value = int.MaxValue;
		GameAction move = null;

		foreach (var action in state.Actions())
		{
			var (childValue, childAction) = MaxValue(state.Result(action), maxPlayer, alpha, beta, depth + 1);

			if (childValue < value)
			{
				(value, move) = (childValue, action);
				beta = Math.Min(beta, value);
			}

			if (value <= alpha)
			{
				return (value, move);
			}
		}
	
		return (value, move);
	}

	class State : ICloneable
	{
		public PlayerName Player { get; set; }

		public GameBoardState BoardState { get; set; }

		public State(GameBoardState state, PlayerName player)
		{
			BoardState = state.Clone() as GameBoardState;
			Player = player;
		}

		public IEnumerable<GameAction> Actions()
		{
			// Queue used to flatten the multi-turns
			var queue = new Queue<ActionNode<State>>();

			// The list of actions an agent may take
			var actions = new List<GameAction>();

			// Consider all moves at step 0
			foreach (var cupIndex in CupsToSelect())
			{
				var action = new GameAction(cupIndex);

				var state = Result(action);

				queue.Enqueue(new ActionNode<State>(state, cupIndex));
			}

			// Flatten action paths
			while (queue.Count > 0)
			{
				var item = queue.Dequeue();

				if (item.Content.Player != Player)
				{
					actions.Add(new GameAction(item.Path().ToArray()));
					continue;
				}

				foreach (var cupIndex in item.Content.CupsToSelect())
				{
					var action = new GameAction(cupIndex);

					var state = item.Content.Result(action);

					queue.Enqueue(new ActionNode<State>(state, cupIndex, item));
				}
			}

			// Actions that represent one or more moves
			return actions;
		}

		public State Result(GameAction action)
		{
			var state = Clone() as State;

			GameBoardAction lastAction;

			foreach (var index in action.Index)
			{
				lastAction = state.BoardState.DoAction(index);

				state.Player = lastAction.NextPlayer;
			}

			return state;
		}

		public int Eval(PlayerName player)
		{
			return Utility(player);
		}

		public int Utility(PlayerName player)
		{
			return BoardState.Cells.First(x => x is GameBoardMancala && x.OwnerPlayer == player).Pebbles;
		}

		public int Utility()
		{
			return Utility(Player);
		}

		public bool IsTerminal()
		{
			return BoardState.Cells
				.Where(x => x is GameBoardCup cup && cup.Pebbles != 0)
				.Count() == 0;
		}

		public PlayerName ToMove()
		{
			return Player;
		}

		public IEnumerable<int> CupsToSelect()
		{
			return BoardState.Cells
				.Where(x => x is GameBoardCup && x.OwnerPlayer == Player && x.Pebbles != 0)
				.Select(x => x.Index);
		}

		public object Clone()
		{
			return new State(BoardState, Player);
		}
	}

	class GameAction
	{
		public IEnumerable<int> Index { get; set; }

		public GameAction(params int[] indices)
		{
			Index = indices;
		}
	}

	class ActionNode<T> where T : class, ICloneable
	{
		public ActionNode<T> Parent;

		public int Index;

		public T Content;

		public ActionNode(T content, int index, ActionNode<T> parent = null)
		{
			Content = content.Clone() as T;
			Parent = parent;
			Index = index;
		}

		public ActionNode<T> Chain(T content, int index)
		{
			return new ActionNode<T>(content, index, this);
		}

		public IEnumerable<int> Path()
		{
			var stack = new Stack<int>();

			for (var node = this; node != null; node = node.Parent)
			{
				stack.Push(node.Index);
			}

			return stack.ToList();
		}
	}
}
