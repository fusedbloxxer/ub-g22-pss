using Godot;
using System;

public partial class PlayerMenuScreen : Node2D
{
	[Signal]
	public delegate void StartGameEventHandler(PlayerType playerA, PlayerType playerB);

	private PlayerSelect _playerSelectA;
	private PlayerSelect _playerSelectB;

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		var canvasLayer = GetNode<CanvasLayer>("CanvasLayer");
		var startButton = canvasLayer.GetNode<TextureButton>("StartButton");
		
		_playerSelectA = canvasLayer.GetNode<PlayerSelect>("PlayerSelectA");
		_playerSelectB = canvasLayer.GetNode<PlayerSelect>("PlayerSelectB");
		
		startButton.ButtonUp += OnStartButtonUp;
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{
	}

	private void OnStartButtonUp()
	{
		var playerTypeA = Convert.ToInt32(_playerSelectA.PlayerType);
		var playerTypeB = Convert.ToInt32(_playerSelectB.PlayerType);
		EmitSignal(SignalName.StartGame, playerTypeA, playerTypeB);
	}
}
