using Godot;
using System;
using System.Collections.Generic;
using System.Linq;

public partial class AIController : Node
{
	[Export]
	public bool IsActive { get; set; } = true;
	[Export]
	public int ThinkTime { get; set; } = 10;

	public AISearchStrategy SearchStrategy { get; } = new AIMinimaxSearchStrategy(4);

	private SceneManager _sceneManager;
	private GameManager _gameManager;
	private GameBoard _gameBoard;
	private Timer _thinkTimer;

	private Queue<Action> _actionQueue;
	private int _remainingActions;
	private double _timePassed;

	public override void _Ready()
	{
		// Retrieve global refs
		_sceneManager = GetNode<SceneManager>($"/root/{nameof(SceneManager)}");
		_gameManager = GetNode<GameManager>($"/root/{nameof(GameManager)}");
		_gameBoard = GetNode<GameBoard>($"../{nameof(GameBoard)}");

		// Listen to global signals
		_gameManager.GameNextTurn += OnGameManagerGameNextTurn;
		_actionQueue = new Queue<Action>();
		_remainingActions = 0;
	}

	public override void _Process(double delta)
	{
		if (_actionQueue.Count == 0)
		{
			return;
		}

		_timePassed += delta;

		if (TimeSpan.FromSeconds(_timePassed) > TimeSpan.FromSeconds(ThinkTime))
		{
			_timePassed = 0;
			_actionQueue.Dequeue()();
		}
	}

	private void OnGameManagerGameNextTurn(Player player)
	{
		if (!IsActive)
		{
			return;
		}

		if (player.Type != PlayerType.AI)
		{
			return;
		}

		if (_remainingActions != 0)
		{
			return;
		}

		var actions = SearchStrategy.Search(_gameBoard.State, player).ToArray();
		
		_actionQueue = new Queue<Action>();

		_remainingActions = actions.Length;

		_timePassed = 0;

		foreach (var action in actions)
		{
			var cupIndex = action;

			_actionQueue.Enqueue(() => AddTimer(cupIndex));
		}

		if (_actionQueue.Count > 0)
		{
			_actionQueue.Dequeue()();
		}
	}

	private void AddTimer(int cupIndex)
	{
		var timer = new Timer
		{
			OneShot = true,
			Autostart = true,
			WaitTime = TimeSpan.FromSeconds(ThinkTime).TotalSeconds,
		};

		var cupUI = _gameBoard.GetNode("Cups").GetChildren().Cast<Cup>()
			.First(x => x.Index == cupIndex);

		cupUI.Hover = true;

		timer.Timeout += () =>
		{
			cupUI.Hover = false;

			--_remainingActions; // warning

			_gameManager.PlayMove(cupIndex);

			timer.QueueFree();
		};

		AddChild(timer);
	}
}
