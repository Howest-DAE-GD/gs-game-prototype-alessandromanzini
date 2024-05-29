#pragma once
#include "BaseGame.h"
#include "Player.h"
#include "CultMember.h"
#include "Projectile.h"
#include <vector>
class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	Texture m_ScoreText;
	Texture m_YouWinText;
	Texture m_YouLoseText;
	Texture m_RestartText;

	Player* m_pPlayer;
	std::vector<CultMember> m_CultMembers;

	std::vector<Projectile> m_Projectiles;

	int m_Score;

	float m_EnemySpawnDelay{ 3.f };
	const float mk_EnemyMaxDelayReduction{ 2.8f };
	const float mk_DefaultProgressPerKill{ .05f };
	float m_Progress{};
	float m_EnemySpawnAccumulatedTime{};

	const float mk_ProjectileSpeed{ 240.f };

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void SpawnEnemy( );
	void SpawnProjectile( );

	void IncreaseScore( );

	CultMember GetRandomCultMember( );

	// Ken De Decker
	// Game's too easy. Maybe add ranged attacks. Enemies can spawn on top of you.
	// Game progression in difficulty.
	// Enemies diversity.
	// New weapons.

	// Nils Hammond
	// Sword hitbox seems awkward. Maybe make it longer.
	// Gun should keep shooting if holding down mouse button.
	// Game should get harder.

	// Koen Samyn
	// The game should start easier.
	// Perhaps a controller would work better.
	// Create a controls screen to make it more understandable.

	// Mikhail Lukach
	// Info on screen
	// Build up

	// Jenson Pauwels
	// Fix the sword, it feels weird to use
	// bullet piercing needs to have a limit, it's too powerful
	// Right clicking should toggle between sword and gun

	// Iva Petrova
	// I love the mechanics
	// Feels too hard in the beginning, maybe make the level harder with the kills increasing
	// The contrast between the pink and the white is too low

	// Georgi Georgiev
	// Cool concept but the game feels like a solved game. If you run around in circles and have experience
	// in kiting games, you can survive without feeling much of a challange
	// Idea for the gun: drops where you can have a machine gun instead of a boring pistol
	// Sword feels interesting to say the least cant define it
	 
	// Rani Nagels
	// I like the concept, but it becomes overwelming quickly. Maybe start at a lower speed and then speed up as the score increases
	// the method of killing the enemies are also a bit ackward

	// Daphny Huys
	// I think it could be nice if the enmies have a certain % to drop some type of coin which 
	// can be used in a shop that adds 
	// certain ways to make it easier for the player, 
	// even if its just a slight increase in sword size or how fast bullets shoot

	// Ken De Decker
	// you could maybe add some maps obstacles
	// You win the game too fast
	// a bit of variation in the enemies' pathing scheme.
	// I didn't know about the gun.
	// color identifying powerups
	// Instead of a cut to the win screen you should be able to kill all remaining enemies.

	// David Capmare
	// Make the sword bigger
	// use toggle instead of hold for gun.
	// The game feels hard

	// Liam Goeman
	// The gun should be nerfed, or limited (reduce firerate and limit piercing).
	// More lives for the player
	// Maybe a boss level.

	};