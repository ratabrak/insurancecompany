#include <SFML/Window.hpp>


int main() {
	sf::Window window(sf::VideoMode({700, 700}), "The");

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();
		}
	}

	return 0;
}

