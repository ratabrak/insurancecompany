#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "GameState.h"
#include "UI.h"

int main() {
	sf::RenderWindow window(sf::VideoMode({1200, 800}), "Just a game");
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);

	GameState game_state;
	UI ui;

	sf::Clock clock_delta;
	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			ImGui::SFML::ProcessEvent(window, event.value());
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		ImGui::SFML::Update(window, clock_delta.restart());
		ui.Update(game_state);

		window.clear(sf::Color(50, 50, 50));
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}