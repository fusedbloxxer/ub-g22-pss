using System;

public enum PlayerName
{
    A = 0,
    B = 1,
}

public enum PlayerType
{
    Human = 0,
    AI = 1,
}

public partial class Player : Godot.GodotObject, ICloneable
{
    public PlayerName Name { get; init; }

    public PlayerType Type { get; set; }

    public object Clone()
    {
        return new Player { Name = Name, Type = Type };
    }
}