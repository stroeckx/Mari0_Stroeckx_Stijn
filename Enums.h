#pragma once
//-----------------------------------------------------------------
// Student data
// Name: Stroeckx Stijn
// Group: 1DAE06
//-----------------------------------------------------------------

enum class ActorType
{
	MARIO,
	BIGMARIO,
	DEAD,
	ENEMY,
	TERRAIN,
	SHRINK,
	FEET,
};

enum class EnemyType
{
	GOOMBA,
	MUSHROOM,
	EMPTY,
	KOOPA,
	SHELL,
	STAR,
	FIREBALL,
	AUTOEXPLODEFIREBALL,
};

enum ActorCollideType
{
	MARIO = 0x0001,
	ENEMY = 0x0002,
	SHELL = 0x0004,
	TERRAIN = 0x0008,
	BOUNDARY = 0x0016,
};

enum class PortalType
{
	BLUE,
	ORANGE,
};