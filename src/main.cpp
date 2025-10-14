#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "GameState.h"
#include "UI.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(1200, 800), "Insurance");
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 8.0f;
	style.ChildRounding = 8.0f;
	style.FrameRounding = 6.0f;
	style.PopupRounding = 8.0f;
	style.ScrollbarRounding = 8.0f;
	style.GrabRounding = 6.0f;
	style.TabRounding = 6.0f;

	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.85f);

	GameState game_state;
	UI ui;

	sf::Clock clock_delta;
	while (window.isOpen() && !game_state.exit_program) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();
		}

		ImGui::SFML::Update(window, clock_delta.restart());
		ui.Update(game_state);

		window.clear(sf::Color(50, 50, 50));
		ImGui::SFML::Render(window);
		window.display();

		if (game_state.curr_month > 24 || game_state.bankrupt) {
			if (game_state.bankrupt) {
				sf::sleep(sf::seconds(2));
			}
			else {
				sf::sleep(sf::seconds(3));
			}
			window.close();
		}
	}

	ImGui::SFML::Shutdown();
	return 0;
}