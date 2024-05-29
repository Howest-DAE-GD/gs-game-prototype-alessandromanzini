#include "pch.h"
#include "Player.h"
#include <utils.h>
#include <math.h>

Player::Player( const Vector2f& location )
	: mk_Width{ 20.f }
	, mk_Height{ 20.f }
	, mk_SwordWidth{ 10.f }
	, mk_SwordHeight{ 70.f }
	, mk_PistolWidth{ 5.f }
	, mk_PistolHeight{ 20.f }
	, m_WeaponAngle{}
	, m_WeaponOffset{ 5.f }
	, m_Location{ location }
	, m_MouseLocation{}
	, mk_Speed{ 300.f }
	, m_IsAlive{ true }
	, m_IsPistolOut{}
	, m_IsShooting{}
	, m_IsPistolOutChanged{}
	, mk_ShootDelay{ .15f }
	, m_ShootAccumulatedTime{}
	, m_KeysState{}
	, m_PistolShots{ 3 }
	, m_ProgressToShot{ 0.f }
	, m_ProgressPerShot{ .5f }
	, m_IsDashing{}
	, m_DashElapsedTime{}
{
	//m_pTexture = new Texture( "" );
}

Player::~Player( )
{
	//delete m_pTexture;
}

Vector2f Player::GetLocation( ) const
{
	return m_Location;
}

Point2f Player::GetPistolEnd( ) const
{
	return m_PistolEnd;
}

Point2f Player::GetSwordStart( ) const
{
	return m_IsPistolOut ? Point2f{ -1000.f, -1000.f } : m_SwordStart;
}

Point2f Player::GetSwordEnd( ) const
{
	return m_IsPistolOut ? Point2f{ -1000.f, -1000.f } : m_SwordEnd;
}

bool Player::GetIsAlive( ) const
{
	return m_IsAlive;
}

Vector2f Player::GetAimDirection( ) const
{
	const Vector2f mouseDirectionVector{ (m_MouseLocation - m_Location).Normalized( ) };
	return mouseDirectionVector;
}

bool Player::GetIsShooting( )
{
	bool temp{ m_IsShooting };
	m_IsShooting = false;
	return temp;
}

int Player::GetShots( )
{
	return m_PistolShots;
}

void Player::IncreaseShotProgress( )
{
	m_ProgressToShot += m_ProgressPerShot;

	if ( m_PistolShots >= 6 )
	{
		m_ProgressPerShot = 0.f;
	}
	else if ( m_ProgressToShot >= 1.f )
	{
		m_PistolShots++;
		m_ProgressToShot--;
	}
}

void Player::Draw( ) const
{
	const Vector2f mouseDirectionVector{ (m_MouseLocation - m_Location).Normalized( ) };

	if ( !m_IsAlive )
	{
		utils::SetColor( Color4f{ 1.f, 0.f, 0.f, 1.f } );
	}
	else
	{
		utils::SetColor( Color4f{ 1.f, 1.f, 1.f, 1.f } );
	}
	utils::DrawRect( m_Location.ToPoint2f(), mk_Width, mk_Height );

	const float width{ m_IsPistolOut ? mk_PistolWidth : mk_SwordWidth };
	const float height{ m_IsPistolOut ? mk_PistolHeight : mk_SwordHeight };
	glPushMatrix();
	{
		glTranslatef( m_Location.x + mk_Width / 2.f, m_Location.y + mk_Height / 2.f, 0.f );
		
		glRotatef(m_WeaponAngle, 0.f, 0.f, 1.f );

		utils::DrawRect( m_WeaponOffset + mk_Width / 2.f, -width / 2.f, height, width );
	}
	glPopMatrix( );
}

void Player::Update( float elapsedSec )
{
	if ( !m_IsAlive ) return;

	const Vector2f mouseDirectionVector{ GetAimDirection( ) };

	if ( m_KeysState.dashPressed && m_DashElapsedTime < 0.f )
	{
		m_IsDashing = true;
		m_DashDirection = mouseDirectionVector;
		m_DashElapsedTime = 0.f;
	}

	if ( m_IsDashing )
	{
		m_DashElapsedTime += elapsedSec;
		if ( m_DashElapsedTime >= 0.2f )
		{
			m_DashElapsedTime = .5f;
			m_IsDashing = false;
		}
		else
		{
			m_Location += m_DashDirection * 1000.f * elapsedSec;
		}
	}
	else
	{
		m_DashElapsedTime -= elapsedSec;

		const float adjustedSpeed{ mk_Speed * elapsedSec };
		Vector2f movement{};

		if ( m_KeysState.facingUpPressed )
		{
			movement.y += 1;
		}
		if ( m_KeysState.facingDownPressed )
		{
			movement.y += -1;
		}
		if ( m_KeysState.facingRightPressed )
		{
			movement.x += 1;
		}
		if ( m_KeysState.facingLeftPressed )
		{
			movement.x += -1;
		}
		m_Location += movement.Normalized( ) * adjustedSpeed;
	}

	if ( m_Location.x < 0 )
	{
		m_Location.x = 0;
	}
	else if ( m_Location.x > 1280 - mk_Width )
	{
		m_Location.x = 1280 - mk_Width;
	}

	if ( m_Location.y < 0 )
	{
		m_Location.y = 0;
	}
	else if ( m_Location.y > 720 - mk_Height )
	{
		m_Location.y = 720 - mk_Height;
	}

	if ( m_ShootAccumulatedTime < mk_ShootDelay )
	{
		m_ShootAccumulatedTime += elapsedSec;
	}

	m_WeaponAngle = atan2f( mouseDirectionVector.y, mouseDirectionVector.x ) * 180.f / float( M_PI );

	m_SwordStart = (m_Location + m_WeaponOffset * mouseDirectionVector).ToPoint2f();
	m_SwordEnd = m_SwordStart + mouseDirectionVector * mk_SwordHeight;
	m_PistolEnd = (m_Location + (m_WeaponOffset + mk_PistolHeight) * mouseDirectionVector).ToPoint2f( );
}

void Player::CheckCollision( const Rectf& bounds )
{
	if ( utils::IsOverlapping( Rectf{ m_Location.x , m_Location.y, mk_Width, mk_Height }, bounds ) )
	{
		m_IsAlive = false;
	}
}

void Player::ProcessKeyPressEvent( const SDL_KeyboardEvent& e )
{
	const bool pressed{ e.state == SDL_PRESSED };
	switch ( e.keysym.sym )
	{
	case SDLK_w:
		m_KeysState.facingUpPressed = pressed;
		break;
	case SDLK_a:
		m_KeysState.facingLeftPressed = pressed;
		break;
	case SDLK_s:
		m_KeysState.facingDownPressed = pressed;
		break;
	case SDLK_d:
		m_KeysState.facingRightPressed = pressed;
		break;
	case SDLK_LSHIFT:
	case SDLK_e:
		m_KeysState.dashPressed = pressed;
		break;
	}
}

void Player::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	m_MouseLocation.x = float( e.x );
	m_MouseLocation.y = float( e.y );
}

void Player::ProcessMousePressEvent( const SDL_MouseButtonEvent& e )
{
	const bool pressed{ e.state == SDL_PRESSED };
	
	if ( e.button == SDL_BUTTON_RIGHT )
	{
		m_IsPistolOutChanged = m_IsPistolOut != pressed;
		m_IsPistolOut = pressed;
	}
	else if( e.button == SDL_BUTTON_LEFT )
	{
		if ( pressed && m_IsPistolOutChanged && m_IsPistolOut )
		{
			if ( m_ShootAccumulatedTime >= mk_ShootDelay && m_PistolShots > 0 )
			{
				m_ShootAccumulatedTime -= mk_ShootDelay;
				--m_PistolShots;
				m_IsShooting = true;
			}
		}
	}
}
