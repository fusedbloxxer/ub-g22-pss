using Godot;
using System;

public partial class PlayerSelect : Node2D
{
	[Export]
	public Player Player
	{
		get => _player;

		set
		{
			_player = value;

			if (_label != null)
			{
				_label.Text = string.Format("Player {0}", Enum.GetName(value));
			}
		}
	}

	public PlayerType PlayerType { get => _playerType; }

	private PlayerType _playerType;
	private Player _player;

	private OptionButton _optionButton;
	private Label _label;

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		// Retrieve local refs
		_label = GetNode<Label>("Label");
		_optionButton = GetNode<OptionButton>("Options");

		// Listen to local signals
		_optionButton.ItemSelected += OnOptionButtonItemSelected;

		// Set initial state
		_label.Text = string.Format("Player {0}", Enum.GetName(Player));
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{
	}

	private void OnOptionButtonItemSelected(long index)
	{
		var option = _optionButton.GetItemText(Convert.ToInt32(index));

		_playerType = option switch
		{
			"AI" => PlayerType.AI,
			"Human" => PlayerType.Human,
			_ => throw new NotSupportedException()
		};
	}
}
