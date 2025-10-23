#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "GameState.h"
#include "UI.h"
#include <vector>
#include <random>

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
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Strahovku svorovali :(");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    addRussianSupport("C:\\Users\\Admin\\CLionProjects\\cjj\\insurancecompany\\font.ttf");

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 8.0f;
    style.ScrollbarRounding = 8.0f;
    style.GrabRounding = 6.0f;
    style.TabRounding = 6.0f;

    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.6f);

    sf::Texture background_texture;
    sf::Texture game_over_texture;

    if (!background_texture.loadFromFile("C:\\Users\\Admin\\CLionProjects\\cjj\\insurancecompany\\media\\HelpyMain.png")) {
        return -1;
    }

    if (!game_over_texture.loadFromFile("C:\\Users\\Admin\\CLionProjects\\cjj\\insurancecompany\\media\\HelpyLost.png")) {
        return -1;
    }

    sf::Sprite background_sprite(background_texture);
    sf::Sprite game_over_sprite(game_over_texture);

    background_sprite.setScale(
        {1200.0f / background_texture.getSize().x,
        800.0f / background_texture.getSize().y}
    );

    game_over_sprite.setScale(
        {1200.0f / game_over_texture.getSize().x,
        800.0f / game_over_texture.getSize().y}
    );

    std::vector<std::string> music_files = {"C:\\Users\\Admin\\CLionProjects\\cjj\\insurancecompany\\media\\Helpy1.mp3", "C:\\Users\\Admin\\CLionProjects\\cjj\\insurancecompany\\media\\Helpy2.mp3"};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, music_files.size() - 1);

    sf::Music music;
    if (!music.openFromFile(music_files[dis(gen)])) {
        return -1;
    }
    music.setLooping(true);
    music.play();

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

        if (game_state.bankrupt || game_state.win) {
            music.stop();
        }

        window.clear(sf::Color(50, 50, 50));

        if (game_state.bankrupt) {
            window.draw(game_over_sprite);
        } else {
            window.draw(background_sprite);
        }

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}