using Godot;
using System;
using System.Linq;

public partial class AIController : Node
{
	[Export]
	public bool IsActive { get; set; } = true;
	[Export]
	public uint ThinkTime { get; set; } = 10u;

	public AISearchStrategy SearchStrategy { get; } = new AIMinimaxSearchStrategy();

	private SceneManager _sceneManager;
	private GameManager _gameManager;
	private GameBoard _gameBoard;
	private Timer _thinkTimer;

	public override void _Ready()
	{
		// Retrieve global refs
		_sceneManager = GetNode<SceneManager>($"/root/{nameof(SceneManager)}");
		_gameManager = GetNode<GameManager>($"/root/{nameof(GameManager)}");
		_gameBoard = GetNode<GameBoard>($"../{nameof(GameBoard)}");

		// Listen to global signals
		_gameManager.GameNextTurn += OnGameManagerGameNextTurn;
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

		var actions = SearchStrategy.Search(_gameBoard.State, player).ToArray();

		Action timerChain = () => {};

		for (int actionIndex = actions.Count() - 1; actionIndex >= 0; actionIndex--)
		{
			var cupIndex = actionIndex;

			if (actionIndex == actions.Count() - 1)
			{
				timerChain = () => AddTimer(actions[cupIndex], () => { });
			}
			else
			{
				timerChain = () => AddTimer(actions[cupIndex], timerChain);
			}
		}

		timerChain();
	}

	private Timer AddTimer(uint cupIndex, Action action)
	{
		GD.Print(cupIndex);

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

			_gameManager.PlayMove(cupIndex);

			timer.QueueFree();

			action();
		};

		AddChild(timer);

		return timer;
	}
}
