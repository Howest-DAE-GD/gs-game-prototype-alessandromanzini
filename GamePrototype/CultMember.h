#pragma once
#include <utils.h>
class CultMember
{
public:
	explicit CultMember( const Vector2f& location );
	CultMember& operator=( const CultMember& other );

	void Draw( ) const;
	void Update( float elapsedSec, const Vector2f& playerLocation );

	Rectf GetBounds( ) const;
	bool GetIsAlive( ) const;

	bool CheckCollision( const Point2f& swordStart, const Point2f& swordEnd );
	bool CheckCollision( const Rectf& bounds );

	void Hit( );

private:
	bool m_IsAlive;
	int m_HP;

	bool m_IsInvincible;
	float m_InvincibilityAccumulatedTime;
	static const float smk_InvicibilityTimer;
	//Texture* m_pTexture;
	float mk_Width;
	float mk_Height;

	Vector2f m_Location;
	const float m_Speed;
	const float m_SpeedModifier;
};

