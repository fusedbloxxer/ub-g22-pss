using Godot;
using System;

public partial class Cup : Node2D
{
	[Export]
	public PlayerName OwnerPlayer { get; set; }
	[Export]
	public uint Index { get; set; }

	public bool Hover
	{
		get
		{
			return _hover;
		}

		set
		{
			_hover = value;

			if (_hoverSprite != null)
			{
				_hoverSprite.Visible = value;
			}
		}
	}

	public uint Pebbles
	{
		get => _pebbles;

		set
		{
			_pebbles = value;

			_label.Text = _pebbles.ToString();

			_label.AddThemeColorOverride("font_color", _pebbles switch
			{
				0 => Colors.White,
				<= 4 => Colors.Yellow,
				<= 12 => Colors.BlueViolet,
				_ => Colors.DarkRed,
			});
		}
	}

	public bool IsActive => _isActive;

	private SceneManager _sceneManager;
	private GameManager _gameManager;
	private CollisionShape2D _shape;
	private Sprite2D _hoverSprite;
	private Area2D _cupArea;
	private Label _label;

	private PlayerType _playerType;
	private bool _isInside;
	private bool _isActive;
	private uint _pebbles;
	private bool _hover;

	public override void _Ready()
	{
		// Retrieve global refs
		_sceneManager = GetNode<SceneManager>($"/root/{nameof(SceneManager)}");
		_gameManager = GetNode<GameManager>("/root/GameManager");

		// Retrieve local refs
		_label = GetNode<Label>("Label");
		_cupArea = GetNode<Area2D>("Area");
		_hoverSprite = GetNode<Sprite2D>("HoverSprite");

		// Listen to global signals
		_gameManager.GameBoardCellUpdate += OnGameStateGameBoardCellUpdate;
		_gameManager.GameNextTurn += OnGameManagerGameNextTurn;
		_gameManager.GameOver += OnGameManagerGameOver;

		// Listen to local signals
		_cupArea.MouseEntered += OnAreaMouseEntered;
		_cupArea.MouseExited += OnAreaMouseExited;

		// Listen to global signals
		_sceneManager.ShowPlayGroundScene += OnSceneManagerShowPlayGroundScene;

		// Set initial state
		_isActive = false;
		Pebbles = 4;
	}

	public override void _Input(InputEvent @event)
	{
		if (@event is not InputEventMouseButton mouseBtnEvent)
		{
			return;
		}

		if (mouseBtnEvent.ButtonIndex != MouseButton.Left)
		{
			return;
		}

		if (!mouseBtnEvent.IsReleased())
		{
			return;
		}

		if (!_isInside || !_isActive)
		{
			return;
		}

		_gameManager.PlayMove(Index);
	}

	private void OnSceneManagerShowPlayGroundScene(GameBoard gameBoard, GameSettings settings)
	{
		_playerType = OwnerPlayer == settings.PlayerA.Name
			? settings.PlayerA.Type
			: settings.PlayerB.Type;
	}

	private void OnGameStateGameBoardCellUpdate(uint index, uint pebbles)
	{
		if (Index != index)
		{
			return;
		}

		Pebbles = pebbles;

		if (Pebbles == 0)
		{
			Hover = false;
		}
	}

	private void OnGameManagerGameNextTurn(Player player)
	{
		_isActive = OwnerPlayer == player.Name;

		if (!_isActive)
		{
			Hover = false;
		}
	}

	private void OnAreaMouseEntered()
	{
		if (_playerType == PlayerType.AI)
		{
			return;
		}

		_isInside = true;

		if (_isActive && Pebbles != 0)
		{
			Hover = true;
		}
	}

	private void OnAreaMouseExited()
	{
		if (_playerType == PlayerType.AI)
		{
			return;
		}

		_isInside = false;

		if (_isActive)
		{
			Hover = false;
		}
	}

	private void OnGameManagerGameOver()
	{
		_isActive = false;
	}
}
