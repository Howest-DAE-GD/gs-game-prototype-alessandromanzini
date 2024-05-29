#pragma once
#include <utils.h>
class Projectile
{
public:
	Projectile( const Point2f& pos, const Vector2f& velocity );

	void Draw( ) const;
	void Update( float elapsedSec );

	Rectf GetBounds( ) const;

private:
	Point2f m_Position;
	Vector2f m_Velocity;

	static const float smk_BulletWidth;
	static const float smk_BulletHeight;
};

