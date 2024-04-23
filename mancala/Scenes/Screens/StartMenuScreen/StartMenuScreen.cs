using Godot;
using System;

public partial class StartMenuScreen : Node2D
{
	[Signal]
	public delegate void StartGameEventHandler();

	private TextureButton _startButton;

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		_startButton = GetNode<TextureButton>("CanvasLayer/StartButton");
		_startButton.ButtonUp += () => EmitSignal(SignalName.StartGame);
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{
	}
}
