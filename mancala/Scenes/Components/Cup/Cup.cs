using Godot;
using System;

public partial class Cup : Node2D
{
	[Export]
	public Player OwnerPlayer { get; set; }

	[Export]
	public uint Index { get; set; }

	public bool IsActive => _isActive;

	private CollisionShape2D _shape;
	private Sprite2D _hoverSprite;
	private GameState _gameState;
	private Area2D _cupArea;
	private Label _label;

	private bool _isInside;
	private bool _isActive;
	private uint _pebbles;

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		// Retrieve global refs
		_gameState = GetNode<GameState>("/root/GameState");
		
		// Retrieve local refs
		_label = GetNode<Label>("Label");
		_cupArea = GetNode<Area2D>("Area");
		_hoverSprite = GetNode<Sprite2D>("HoverSprite");

		// Listen to global signals
		_gameState.GameBoardCellUpdate += OnGameStateGameBoardCellUpdate;
		_gameState.PlayerTurnChanged += OnGameStatePlayerTurnChanged;

		// Listen to local signals
		_cupArea.MouseEntered += OnAreaMouseEntered;
		_cupArea.MouseExited += OnAreaMouseExited;

		// Set initial state
		_isActive = _gameState.CurrentPlayer == OwnerPlayer;
		Pebbles = 4;
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{
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

		_gameState.SelectCupCell(Index);
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
			_hoverSprite.Visible = false;
		}
	}

	private void OnGameStatePlayerTurnChanged(Player currentPlayer)
	{
		_isActive = OwnerPlayer == currentPlayer;

		if (!_isActive)
		{
			_hoverSprite.Visible = false;
		}
	}

	private void OnAreaMouseEntered()
	{
		_isInside = true;

		if (_isActive && Pebbles != 0)
		{
			_hoverSprite.Visible = true;
		}
	}

	private void OnAreaMouseExited()
	{
		_isInside = false;

		if (_isActive)
		{
			_hoverSprite.Visible = false;
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
}
