using Godot;
using System;

public partial class Mancala : Node2D
{
	[Export]
	public PlayerName PlayerOwner { get; set; }

	[Export]
	public uint Index { get; set; }

	private GameManager _gameState;
	private Label _label;

	private uint _pebbles;

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		// Retrieve global refs
		_label = GetNode<Label>("Label");
		_gameState = GetNode<GameManager>("/root/GameManager");
		_gameState.GameBoardCellUpdate += OnGameStateGameBoardCellUpdate;
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{
	}

	private void OnGameStateGameBoardCellUpdate(uint index, uint pebbles)
	{
		if (Index != index)
		{
			return;
		}

		Pebbles = pebbles;
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
