using Godot;
using System;

public partial class GameHUD : Node
{
	public GameSettings Settings
	{
		get
		{
			return _gameSettings;
		}

		set
		{
			_gameSettings = value;

			if (_labelPlayerA is not null)
			{
				_labelPlayerA.Text = string.Format("Player ({0})", Enum.GetName(_gameSettings.PlayerA.Type));
			}

			if (_labelPlayerB is not null)
			{
				_labelPlayerB.Text = string.Format("Player ({0})", Enum.GetName(_gameSettings.PlayerB.Type));
			}
		}
	}

	private Button _nextTurnButton;
	private Button _mainMenuButton;

	private CanvasLayer _canvasLayer;
	private Label _labelPlayerA;
	private Label _labelPlayerB;

	private SceneManager _sceneManager;
	private GameSettings _gameSettings;
	private GameManager _gameManager;
	private GameBoard _gameBoard;

	public override void _Ready()
	{
		// Retrieve global refs
		_gameManager = GetNode<GameManager>($"/root/{nameof(GameManager)}");
		_sceneManager = GetNode<SceneManager>($"/root/{nameof(SceneManager)}");

		// Retrieve local refs
		_canvasLayer = GetNode<CanvasLayer>("CanvasLayer");
		_labelPlayerA = _canvasLayer.GetNode<Label>("LabelPlayerA");
		_labelPlayerB = _canvasLayer.GetNode<Label>("LabelPlayerB");
		_mainMenuButton = _canvasLayer.GetNode<Button>("MainMenuButton");
		_nextTurnButton = _canvasLayer.GetNode<Button>("NextTurnButton");
		_gameBoard = GetNode<GameBoard>($"../{nameof(GameBoard)}");

		// Listen to global signals
		_gameManager.GameNextTurn += OnGameManagerGameNextTurn;
		_gameManager.GameStart += OnGameManagerGameStart;
		_gameManager.GameOver += OnGameManagerGameOver;

		// Listen to local signals
		_nextTurnButton.ButtonUp += OnNextTurnButtonUp;
		_mainMenuButton.ButtonUp += OnMainMenuButtonUp;
	}

	private void OnGameManagerGameStart()
	{
		// A new game may have different settings
		Settings = _gameBoard.Settings;
	}

	private void OnGameManagerGameOver()
	{
		_labelPlayerB.AddThemeColorOverride("font_color", Colors.Yellow);
		_labelPlayerB.Text = "Game Over!";
		_labelPlayerA.Text = "";
	}

	private void OnGameManagerGameNextTurn(Player player)
	{
		var currentPlayerLabel = player.Name switch
		{
			PlayerName.A => _labelPlayerA,
			PlayerName.B => _labelPlayerB,
			_ => throw new Exception()
		};

		var otherPlayerLabel = player.Name switch
		{
			PlayerName.A => _labelPlayerB,
			PlayerName.B => _labelPlayerA,
			_ => throw new Exception()
		};

		currentPlayerLabel.AddThemeColorOverride("font_color", Colors.Yellow);
		otherPlayerLabel.AddThemeColorOverride("font_color", Colors.White);
	}

	private void OnMainMenuButtonUp()
	{
		_sceneManager.LoadMainMenu();
	}

	private void OnNextTurnButtonUp()
	{
		_gameManager.NextTurn(togglePlayerTurn: true);
	}
}
