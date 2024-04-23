using Godot;
using System;

public partial class PlayerSelect : Node2D
{
	[Export]
	public PlayerName Player
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
	private PlayerName _player;

	private OptionButton _optionButton;
	private Label _label;

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

	private void OnOptionButtonItemSelected(long index)
	{
		_playerType = Enum.Parse<PlayerType>(_optionButton.GetItemText(Convert.ToInt32(index)));
	}
}
