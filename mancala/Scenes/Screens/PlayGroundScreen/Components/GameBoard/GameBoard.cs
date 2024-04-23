using Godot;
using System;

public partial class GameBoard : Node2D
{
	public GameSettings Settings { get; set; }
	
	public GameBoardState State => _state;

	private GameBoardState _state;

	public override void _Ready()
	{
		// Retrieve local refs
		_state = GetNode<GameBoardState>(nameof(GameBoardState));
	}
}
