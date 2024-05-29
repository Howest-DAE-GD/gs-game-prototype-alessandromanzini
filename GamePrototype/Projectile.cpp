#include "pch.h"
#include "Projectile.h"

const float Projectile::smk_BulletWidth{ 5.f };
const float Projectile::smk_BulletHeight{ 5.f };

Projectile::Projectile( const Point2f& pos, const Vector2f& velocity )
	: m_Position{ pos }
	, m_Velocity{ velocity }
{
}

void Projectile::Draw( ) const
{
	utils::SetColor( Color4f{ 1.f,1.f,1.f,1.f } );
	utils::FillRect( m_Position.x, m_Position.y, smk_BulletWidth, smk_BulletHeight );
}

void Projectile::Update( float elapsedSec )
{
	m_Position += m_Velocity * elapsedSec;
}

Rectf Projectile::GetBounds( ) const
{
	return Rectf( m_Position.x, m_Position.y, smk_BulletWidth, smk_BulletHeight );
}

