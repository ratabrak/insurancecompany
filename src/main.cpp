#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "GameState.h"
#include "UI.h"


void addRussianSupport(const char *path) {
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();

	ImFont* font = io.Fonts->AddFontFromFileTTF(
		path,
		14.0f,
		nullptr,
		io.Fonts->GetGlyphRangesCyrillic()
	);

	IM_ASSERT(font != nullptr);

	ImGui::SFML::UpdateFontTexture();
}


int main() {
	sf::RenderWindow window(sf::VideoMode({1200, 800}), "Insurance");
	window.setFramerateLimit(60);
	ImGui::SFML::Init(window);

	addRussianSupport("path/to/font.ttf");

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
		while (std::optional event = window.pollEvent()) {
			ImGui::SFML::ProcessEvent(window, event.value());
			if (event->is<sf::Event::Closed>())
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