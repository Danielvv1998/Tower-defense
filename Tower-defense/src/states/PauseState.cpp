#include "MenuState.h"
#include "../helpers/GameStateManager.h"

PauseState::PauseState(sf::RenderWindow& window, const Player & player):
	State(window),
	menu(window, {{
			window,
			std::function<void()>([&window]() {
				GameStateManager::popState(); //Pops the pause state to return to playstate
			}),
			{250, 75},
			{static_cast<float>(window.getSize().x) / 2 - 250 / 2, 150},
			{"Resume Game", font, 20}
		}, {
			window,
			std::function<void()>([&window, &player]() {
				GameStateManager::popState(); //Pops the pause state to return to playstate
				GameStateManager::pushState(std::make_unique<ScoreState>(window, player));
			}),
			{250, 75},
			{static_cast<float>(window.getSize().x) / 2 - 250 / 2, 350},
			{"Forfeit Game", font, 20}
		}
	}),
	player(player){}

void PauseState::init() {
	if (!font.loadFromFile("resources/fonts/consola.ttf")) {
		std::cout << "Could not load consola.ttf" << std::endl;
	}
	text.setFont(font);
	text.setString("Game Paused..");
	// TODO: Better center d;)
	text.setPosition({static_cast<float>(window.getSize().x) / 2 - 7 * 24, 24.0f});
}

void PauseState::update() {}

void PauseState::render() const {
	window.draw(text);
	menu.render();
}

void PauseState::cleanUp() {}

void PauseState::onKeyPressed(sf::Event& evt) {
	switch (evt.key.code) {
	case sf::Keyboard::Return:
		menu.onPress();
		break;
	case sf::Keyboard::Up:
		menu.selectNext();
		break;
	case sf::Keyboard::Down:
		menu.selectPrevious();
		break;
	}
}

void PauseState::onMouseButtonPressed(sf::Event& evt) {
	menu.onPress();
}

void PauseState::onMouseMoved(sf::Event& evt) {
	menu.onMouseMoved(evt);
}