#pragma once
#include "SFML\Graphics.hpp"
#include "SFML\Audio.hpp"
#include <string>
#include <array>

#pragma region comment
/**
* @struct Encapsulates all the (modifiable) data that is needed by Tower.
*/
#pragma endregion

struct TowerData {
	float radius;
	sf::Time reload_time;
	float damage;
	int32_t cost;
	const std::array<sf::Sprite, 3> turret_sprites;
};