#include "PlayState.h"
#include "../helpers/GameStateManager.h"


PlayState::PlayState(sf::RenderWindow& window):
	State(window),
	tileSize(31),
	player(window, 15, 375, 0),
	grid(window, tileSize),
	dummyTower(nullptr)
{}

void PlayState::rebuildGrid() {
	grid.clearGrid();

	for (auto& action : player.actions) {
		switch (action.type) {
		case Action::ACTION_TYPE::PLACE_TOWER:
			//change 31 to tileSize
			grid.placeTower(
				action.x,
				action.y,
				std::make_unique<Tower>(
					window,
					static_cast<float>(31),
					sf::Vector2f{static_cast<float>(action.x) * 31,
					static_cast<float>(action.y) * 31},
					static_cast<int>(32) * 2,
					dummyEnemies,
					750)
			);
			break;
		case Action::ACTION_TYPE::SELL_TOWER:
			//TODO: REMOVE TOWER
			//TODO: RETURN GOLD OF TOWER
			//TODO: Call grid.sellTower(), or something like that
			break;
		case Action::ACTION_TYPE::UPGRADE_TOWER:
			//TODO: UPGRADE TOWERS
			//TODO: TAKE GOLD FOR UPGRADING THE TOWER
			//TODO: Call grid.upgradeTower(), or something like that
			break;
		}
	}
}

void PlayState::init() {
	if (!font.loadFromFile("resources/fonts/consola.ttf")) {
		std::cout << "Could not load consola.ttf" << std::endl;
	}
	text.setFont(font);
	text.setString("Press Esc to go back to menu");
	// TODO: Better center d;)
	text.setPosition({static_cast<float>(window.getSize().x) / 2 - 7 * 24, static_cast<float>(window.getSize().y) / 2 - 24});

	player.addAction(20, 10, Action::ACTION_TYPE::PLACE_TOWER, 10);
}

void PlayState::update() {
	grid.update();
	if (dummyTower != nullptr) {
		dummyTower->setPosition(placePosition);
	}
}

void PlayState::render() const {
	grid.render();
	window.draw(text);
	if (dummyTower != nullptr) {
		dummyTower->render();
	}
}

void PlayState::cleanUp() {}

void PlayState::onKeyPressed(sf::Event& evt) {
	if (evt.key.code ==  sf::Keyboard::Escape) {
		// TODO: Pass player score not 1337
		GameStateManager::pushState(std::make_unique<ScoreState>(window, 1337));
	} else if (evt.key.code == sf::Keyboard::A) {
		dummyTower = std::make_unique<Tower>(
			window,
			tileSize,
			placePosition,
			static_cast<int>(tileSize) * 2,
			dummyEnemies,
			750
		);	
	} else if (evt.key.code == sf::Keyboard::U) {
		player.undoAction();
		rebuildGrid();
	}
};

void PlayState::onMouseButtonPressed(sf::Event& evt) {
	// TODO: The grid should have a position insted of a x/y offset, so that we can substract the position instead of 3
	float fullSize = tileSize + lineSize;
	uint8_t x = static_cast<uint8_t>(ceil(static_cast<float>(placePosition.x) / fullSize)) - 3;
	uint8_t y = static_cast<uint8_t>(ceil(static_cast<float>(placePosition.y) / fullSize)) - 3;

	// sf::RenderWindow & window, float size, sf::Vector2f pos, int radius, std::vector<std::shared_ptr<Enemy>>& enemies, int reload_time
	if (grid.canBePlaced(x,y)) {
		grid.placeTower(x,y,std::make_unique<Tower>(window, tileSize, placePosition, static_cast<int>(tileSize) * 2, dummyEnemies, 750));
		std::cout << "Success!" << std::endl;
		// TODO: Save action in actions list, Move tower cost to grid class
		if (player.gold >= dummyCost) {
			player.gold -= dummyCost; // TODO: replace this with tower cost
			player.addAction(x, y, Action::ACTION_TYPE::PLACE_TOWER, dummyCost);
		}
	} else {
		std::cout << "Oei" << std::endl;
	}
}

void PlayState::onMouseMoved(sf::Event& evt) {
	if (dummyTower != nullptr) {
		float fullSize = tileSize + lineSize;
		auto indexes = sf::Vector2f(
			ceil(static_cast<float>(evt.mouseMove.x) / fullSize) - 3,
			ceil(static_cast<float>(evt.mouseMove.y) / fullSize) - 3
		);

		placePosition = sf::Vector2f(indexes.x * fullSize + 80, indexes.y * fullSize + 88);

		if (!grid.canBePlaced(static_cast<uint8_t>(indexes.x), static_cast<uint8_t>(indexes.y))) {
			dummyTower->setColor(sf::Color::Red);
		} else if (player.gold < dummyCost) { // TODO: replace this with tower cost
			dummyTower->setColor(sf::Color::Yellow);
		} else {
			dummyTower->setColor(sf::Color::Green);
		}
	}
}
