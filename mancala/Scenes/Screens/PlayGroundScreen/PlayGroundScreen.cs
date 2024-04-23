using Godot;
using System;

public partial class PlayGroundScreen : Node2D
{
	[Signal]
	public delegate void GameOverEventHandler();

	private GameBoard _gameBoard;
	private GameHUD _gameHUD;

	public override void _Ready()
	{
		// Retrieve local refs
		_gameBoard = GetNode<GameBoard>(nameof(GameBoard));
		_gameHUD = GetNode<GameHUD>(nameof(GameHUD));
	}
}
