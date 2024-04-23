using Godot;
using System;

public partial class SceneManager : Node
{
    [Signal]
    public delegate void GameStartEventHandler(GameBoardState state);

    private PlayGroundScreen _playGround;
    private PlayerMenuScreen _playerMenu;
    private StartMenuScreen _startMenu;
    private Node _root;
    
    // Called when the node enters the scene tree for the first time.
    public override void _Ready()
    {
        // Retrieve local refs
        _root = GetNode<Node>("/root/Main");
        //_startMenu = _root.GetNode<StartMenuScreen>("StartMenuScreen");
        //_playerMenu = _root.GetNode<PlayerMenuScreen>("PlayerMenuScreen");
        _playGround = _root.GetNode<PlayGroundScreen>("PlayGroundScreen");
        EmitSignal(SignalName.GameStart, _playGround.GetNode<GameBoardState>("GameBoard/GameBoardState"));

        //// Choose initial scene
        //_root.RemoveChild(_playerMenu);
        //_root.RemoveChild(_playGround);

        //// Listen to local signals
        //_startMenu.StartGame += OnStartMenuStartGame;
        //_playerMenu.StartGame += OnPlayerMenuStartGame;
    }

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{
	}

    private void OnStartMenuStartGame()
    {
        _root.RemoveChild(_startMenu);
        _root.AddChild(_playerMenu);
    }

    private void OnPlayerMenuStartGame(PlayerType playerTypeA, PlayerType playerTypeB)
    {
        _root.RemoveChild(_playerMenu);
        _root.AddChild(_playGround);
        EmitSignal(SignalName.GameStart);
    }
}