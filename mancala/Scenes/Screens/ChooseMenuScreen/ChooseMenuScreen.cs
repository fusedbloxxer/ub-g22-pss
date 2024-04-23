using Godot;
using System;

public partial class ChooseMenuScreen : Node2D
{
	[Signal]
	public delegate void StartGameEventHandler(GameSettings settings);

	private PlayerSelect _playerSelectA;
	private PlayerSelect _playerSelectB;

	public override void _Ready()
	{
		var canvasLayer = GetNode<CanvasLayer>("CanvasLayer");
		var startButton = canvasLayer.GetNode<TextureButton>("StartButton");
		
		_playerSelectA = canvasLayer.GetNode<PlayerSelect>("PlayerSelectA");
		_playerSelectB = canvasLayer.GetNode<PlayerSelect>("PlayerSelectB");
		
		startButton.ButtonUp += OnStartButtonUp;
	}

	private void OnStartButtonUp()
	{
		var gameSettings = new GameSettings
		{
			PlayerA = new Player
			{
				Name = PlayerName.A,
				Type = _playerSelectA.PlayerType,
			},

			PlayerB = new Player
			{
				Name = PlayerName.B,
				Type = _playerSelectB.PlayerType,
			},
		};

		EmitSignal(SignalName.StartGame, gameSettings);
	}
}
