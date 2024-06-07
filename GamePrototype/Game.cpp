#include "pch.h"
#include "Game.h"
#include "string.h"

Game::Game( const Window& window ) 
	:BaseGame{ window }
	, m_ControlsScreen{ "controls_screen.png" }
	, m_ScoreText{ "0", "F_cuphead_vogue_xbold.otf", 30, Color4f{ 1.f, 1.f, 1.f, 1.f } }
	, m_YouWinText{ "You Win!", "F_cuphead_vogue_xbold.otf", 30, Color4f{ 1.f, 1.f, 1.f, 1.f } }
	, m_YouLoseText{ "You Lose!", "F_cuphead_vogue_xbold.otf", 30, Color4f{ 1.f, 1.f, 1.f, 1.f } }
	, m_RestartText{ "Press R to restart", "F_cuphead_vogue_xbold.otf", 20, Color4f{ 1.f, 1.f, 1.f, 1.f } }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_pPlayer = new Player( Vector2f{ GetViewPort().width/2, GetViewPort().height/2} );
	m_CultMembers = std::vector<CultMember>{};
	m_Powerups = std::vector<Rectf>{};
	m_Progress = 0.f;
	m_Score = 0;
	m_ScoreText = Texture{ std::to_string( m_Score ), "F_cuphead_vogue_xbold.otf", 30, Color4f{ 1.f, 1.f, 1.f, 1.f } };
}

void Game::Cleanup( )
{
	delete m_pPlayer;
}

void Game::Update( float elapsedSec )
{
	if ( m_Progress >= 1.f || m_Pause )
	{
		return;
	}

	if( rand( ) % 1000 == 0 )
	{
		m_Powerups.push_back( Rectf{ float( rand( ) % int( GetViewPort( ).width ) ), float( rand( ) % int( GetViewPort( ).height ) ), 20.f, 20.f } );
	}

	m_EnemySpawnAccumulatedTime += elapsedSec;

	m_pPlayer->Update( elapsedSec );
	const Point2f swordStart{ m_pPlayer->GetSwordStart() };
	const Point2f swordEnd{ m_pPlayer->GetSwordEnd( ) };

	for ( Projectile& projectile : m_Projectiles )
	{
		projectile.Update( elapsedSec );
	}

	for ( CultMember& member : m_CultMembers )
	{
		member.Update( elapsedSec, m_pPlayer->GetLocation( ) );

		bool collision{};

		collision = member.CheckCollision( swordStart, swordEnd );
		if ( collision && !member.GetIsAlive( ) && m_pPlayer->GetIsAlive( ) )
		{
			m_pPlayer->IncreaseShotProgress( );
		}
		
		for ( const Projectile& projectile : m_Projectiles )
		{
			collision = collision || member.CheckCollision( projectile.GetBounds( ) );
		}
		if ( collision && !member.GetIsAlive( ) && m_pPlayer->GetIsAlive( ) )
		{
			IncreaseScore( );
		}
		

		m_pPlayer->CheckCollision( member.GetBounds() );
	}

	for( Rectf& powerup : m_Powerups )
	{
		if ( utils::IsOverlapping( m_pPlayer->GetBounds( ), powerup ) )
		{
			powerup.left = -1000.f;
			m_pPlayer->IncreaseSwordSize( );
		}
	}

	if ( m_pPlayer->GetIsAlive( ) )
	{
		SpawnProjectile( );
	}
	SpawnEnemy( );
}

void Game::Draw( ) const
{
	ClearBackground( );

	utils::SetColor( Color4f{ 0.f, 0.f, 1.f, 1.f } );
	for ( const Rectf& powerup : m_Powerups )
	{
		utils::FillRect( powerup );
	}

	utils::SetColor( Color4f{ 1.f, 1.f, 1.f, 1.f } );
	m_pPlayer->Draw( );

	for ( const CultMember& member : m_CultMembers )
	{
		member.Draw( );
	}

	for ( const Projectile& projectile : m_Projectiles )
	{
		projectile.Draw();
	}

	{
		utils::SetColor( Color4f{ 1.f, 1.f, 1.f, 1.f } );
		Point2f startingShotsUI{ 800.f, 20.f };
		const float width{ 10.f }, height{ 25.f };
		for ( int i{}; i < m_pPlayer->GetShots( ); ++i )
		{
			utils::DrawRect( startingShotsUI, width, height );
			startingShotsUI.x += width + 5;
		}
	}

	m_ScoreText.Draw( Point2f{ 1200, 680 } );

	utils::SetColor( Color4f{ 1.f, 1.f, 1.f, 1.f } );
	utils::DrawRect( Point2f{ 20.f, 20.f }, 120.f, 20.f );
	utils::FillRect( Point2f{ 20.f, 20.f }, 120.f * m_Progress, 20.f );

	if ( m_Progress >= 1.f )
	{
		m_YouWinText.Draw( Point2f{ GetViewPort( ).width / 2 - m_YouWinText.GetWidth( ) / 2, GetViewPort( ).height / 2 - m_YouWinText.GetHeight( ) / 2 } );
	}
	else if ( !m_pPlayer->GetIsAlive( ) )
	{
		m_YouLoseText.Draw( Point2f{ GetViewPort( ).width / 2 - m_YouLoseText.GetWidth( ) / 2, GetViewPort( ).height / 2 - m_YouLoseText.GetHeight( ) / 2 } );
		m_RestartText.Draw( Point2f{ GetViewPort( ).width / 2 - m_RestartText.GetWidth( ) / 2, GetViewPort( ).height / 2 - m_RestartText.GetHeight( ) / 2 - 30.f } );
	}

	if( m_Pause )
	{
		m_ControlsScreen.Draw( GetViewPort() );
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	m_pPlayer->ProcessKeyPressEvent( e );

	switch ( e.keysym.sym )
	{
	case SDLK_r:
		Initialize( );
		break;
	case SDLK_ESCAPE:
		m_Pause = !m_Pause;
		break;	
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	m_pPlayer->ProcessKeyPressEvent( e );
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
	m_pPlayer->ProcessMouseMotionEvent( e );
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	m_pPlayer->ProcessMousePressEvent( e );
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	m_pPlayer->ProcessMousePressEvent( e );
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::SpawnEnemy( )
{
	const float spawnDelay{ m_EnemySpawnDelay - (m_Progress * mk_EnemyMaxDelayReduction) };

	if ( m_EnemySpawnAccumulatedTime >= spawnDelay )
	{
		m_EnemySpawnAccumulatedTime -= spawnDelay;

		for ( CultMember& member : m_CultMembers )
		{
			if ( !member.GetIsAlive( ) )
			{
				member = GetRandomCultMember();
				return;
			}
		}
		m_CultMembers.push_back( GetRandomCultMember() );
	}
}

void Game::SpawnProjectile( )
{
	if ( m_pPlayer->GetIsShooting( ) )
	{
		for ( Projectile& projectile : m_Projectiles )
		{
			const Point2f pos{ projectile.GetBounds( ).left, projectile.GetBounds( ).bottom };
			const Rectf viewport{ GetViewPort( ) };
			if ( pos.x < 0 || pos.x > viewport.width || pos.y < 0 || pos.y > viewport.height )
			{
				projectile = Projectile( m_pPlayer->GetPistolEnd( ), m_pPlayer->GetAimDirection( ) * mk_ProjectileSpeed );
				return;
			}
		}
		m_Projectiles.push_back( Projectile( m_pPlayer->GetPistolEnd(), m_pPlayer->GetAimDirection( ) * mk_ProjectileSpeed ) );
	}
}

void Game::IncreaseScore( )
{
	++m_Score;
	m_Progress += (1 - m_Progress) * mk_DefaultProgressPerKill;
	if ( m_Progress + 0.02 >= 1.f )
		m_Progress = 1.f;
	m_ScoreText = Texture{ std::to_string( m_Score ), "F_cuphead_vogue_xbold.otf", 30, Color4f{ 1.f, 1.f, 1.f, 1.f } };

}

CultMember Game::GetRandomCultMember( )
{
	const Rectf viewport{ GetViewPort( ) };

	float x{ float( rand( ) % int( viewport.width ) ) - viewport.width / 2 };
	float y{ float( rand( ) % int( viewport.height ) ) - viewport.height / 2 };
	if ( x > 0 )
	{
		x += viewport.width;
	}
	if ( y > 0 )
	{
		y += viewport.height;
	}
	return CultMember( Vector2f{ float( x ), float( y ) } );
}
