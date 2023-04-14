#pragma once

enum class CollisionType
{
	Point2D,
	CirCle,
	Rect,
	OrientedRect,

	Point3D,
	Sphere3D,
	AABBBox3D,
	OBBBox3D,
	MAX,
};

enum class RenderingOrder
{
	BackBuffer,
	World,
	UI,
	Debug,
	Mouse
};

enum class CollisionCheck
{
	None,
	Begin
};