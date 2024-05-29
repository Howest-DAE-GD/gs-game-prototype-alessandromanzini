#include "pch.h"
#include "CultMember.h"

const float CultMember::smk_InvicibilityTimer{ .25f };

CultMember::CultMember( const Vector2f& location )
	: m_IsAlive{ true }
	, m_Location{ location }
	, m_Speed{ 100.f }
	, m_SpeedModifier{ 1.1 }
	, mk_Width{ 25.f }
	, mk_Height{ 25.f }
	, m_HP{ 2 }
	, m_IsInvincible{}
	, m_InvincibilityAccumulatedTime{}
{
}

CultMember& CultMember::operator=( const CultMember& other )
{
	this->m_Location = other.m_Location;
	this->m_HP = other.m_HP;
	this->m_IsAlive = other.m_IsAlive;
	this->m_IsInvincible = other.m_IsInvincible;
	this->m_InvincibilityAccumulatedTime = other.m_InvincibilityAccumulatedTime;

	return *this;
}

void CultMember::Draw( ) const
{
	if ( !m_IsAlive ) return;

	utils::SetColor( Color4f{ 1.f, 1.f - (0.25f * float( m_HP - 1 )), 1.f, 1.f } );
	utils::DrawRect( m_Location.ToPoint2f( ), mk_Width, mk_Height );
}

void CultMember::Update( float elapsedSec, const Vector2f& playerLocation )
{
	if ( !m_IsAlive ) return;
	const float adjustedSpeed{ m_Speed * (m_SpeedModifier * m_HP) * elapsedSec };

	const Vector2f directionVector{ (playerLocation - m_Location).Normalized() };
	m_Location += directionVector * adjustedSpeed;

	m_InvincibilityAccumulatedTime += elapsedSec;
	if ( m_InvincibilityAccumulatedTime >= smk_InvicibilityTimer )
	{
		m_IsInvincible = false;
	}
}

Rectf CultMember::GetBounds( ) const
{
	return Rectf( m_Location.x, m_Location.y, mk_Width, mk_Height );
}

bool CultMember::GetIsAlive( ) const
{
	return m_IsAlive;
}

bool CultMember::CheckCollision( const Point2f& swordStart, const Point2f& swordEnd )
{
	if ( utils::IsOverlapping( swordStart, swordEnd, Rectf{ m_Location.x, m_Location.y, mk_Width, mk_Height } ) && !m_IsInvincible )
	{
		Hit( );
		return true;
	}
	return false;
}

bool CultMember::CheckCollision( const Rectf& bounds )
{
	if ( utils::IsOverlapping( bounds, Rectf{ m_Location.x, m_Location.y, mk_Width, mk_Height } ) && !m_IsInvincible )
	{
		Hit( );
		return true;
	}
	return false;
}

void CultMember::Hit( )
{
	--m_HP;
	if ( m_HP <= 0 )
	{
		m_IsAlive = false;
		m_Location = Vector2f{ -100.f, -100.f };
	}
	else
	{
		m_IsInvincible = true;
		m_InvincibilityAccumulatedTime = 0.f;
	}
}


