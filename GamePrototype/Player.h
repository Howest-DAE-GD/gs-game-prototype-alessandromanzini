#pragma once
#include <Vector2f.h>
#include <Texture.h>
#include "KeysState.h"
#include "vector"
class Player
{
public:
	explicit Player( const Vector2f& location );
	~Player( );

	Vector2f GetLocation( ) const;
	Point2f GetPistolEnd( ) const;
	Point2f GetSwordStart() const;
	Point2f GetSwordEnd() const;
	bool GetIsAlive( ) const;
	Vector2f GetAimDirection( ) const;

	bool GetIsShooting( );
	int GetShots( );

	void IncreaseShotProgress( );

	void Draw( ) const;
	void Update( float elapsedSec );

	void CheckCollision( const Rectf& bounds );

	void ProcessKeyPressEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMousePressEvent( const SDL_MouseButtonEvent& e );

private:
	Texture* m_pTexture;
	float mk_Width;
	float mk_Height;
	float mk_SwordWidth;
	float mk_SwordHeight;
	float mk_PistolWidth;
	float mk_PistolHeight;

	bool m_IsAlive;

	Vector2f m_Location;
	Vector2f m_MouseLocation;
	const float mk_Speed;

	bool m_IsPistolOut;
	bool m_IsPistolOutChanged;
	bool m_IsShooting;
	const float mk_ShootDelay;
	float m_ShootAccumulatedTime;

	float m_WeaponAngle;
	float m_WeaponOffset;
	Point2f m_SwordStart;
	Point2f m_SwordEnd;
	Point2f m_PistolEnd;

	KeysState m_KeysState;

	int m_PistolShots;
	float m_ProgressToShot;
	float m_ProgressPerShot;

	bool m_IsDashing;
	float m_DashElapsedTime;
	Vector2f m_DashDirection;
};

