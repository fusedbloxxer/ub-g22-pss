using Godot;
using System;

public partial class SceneManager : Node
{
	[Signal]
	public delegate void ShowPlayGroundSceneEventHandler(GameBoard board, GameSettings settings);
	[Signal]
	public delegate void ShowChooseMenuScreenEventHandler();
	[Signal]
	public delegate void ShowMenuScreenEventHandler();

	private PlayGroundScreen _playGround;
	private ChooseMenuScreen _chooseMenu;
	private StartMenuScreen _startMenu;
	private GameManager _gameManager;
	private Node _root;
	
	public override void _Ready()
	{
		// Retrieve app root
		_root = GetNode<Node>("/root/Main");
		_gameManager = GetNode<GameManager>($"/root/{nameof(GameManager)}");

		// Load scenes from disk
		_startMenu = ResourceLoader
			.Load<PackedScene>($"res://Scenes/Screens/{nameof(StartMenuScreen)}/{nameof(StartMenuScreen)}.tscn")
			.Instantiate<StartMenuScreen>();
		_chooseMenu = ResourceLoader
			.Load<PackedScene>($"res://Scenes/Screens/{nameof(ChooseMenuScreen)}/{nameof(ChooseMenuScreen)}.tscn")
			.Instantiate<ChooseMenuScreen>();
		_playGround = ResourceLoader
			.Load<PackedScene>($"res://Scenes/Screens/{nameof(PlayGroundScreen)}/{nameof(PlayGroundScreen)}.tscn")
			.Instantiate<PlayGroundScreen>();

		// Load the start menu
		LoadMainMenu();
	}

	public void LoadMainMenu()
	{
		LoadScene(_startMenu);
		_startMenu.StartGame += OnStartMenuStartGame;
		EmitSignal(SignalName.ShowMenuScreen);
	}

	private void OnStartMenuStartGame()
	{
		_startMenu.StartGame -= OnStartMenuStartGame; 
		LoadScene(_chooseMenu);
		_chooseMenu.StartGame += OnChooseMenuStartGame;
		EmitSignal(SignalName.ShowChooseMenuScreen);
	}

	private void OnChooseMenuStartGame(GameSettings settings)
	{
		_chooseMenu.StartGame -= OnChooseMenuStartGame;
		LoadScene(_playGround);
		_playGround.GameOver += OnPlayGroundGameOver;
		EmitSignal(SignalName.ShowPlayGroundScene, _playGround.GetNode<GameBoard>(nameof(GameBoard)), settings);
	}

	private void OnPlayGroundGameOver()
	{
		_playGround.GameOver -= OnPlayGroundGameOver;
		LoadMainMenu();
	}

	private void LoadScene(Node scene)
	{
		// Make room to load the initial scene
		foreach (var child in _root.GetChildren())
		{
			_root.RemoveChild(child);
		}

		// Load the scene
		_root.AddChild(scene);
	}
}
