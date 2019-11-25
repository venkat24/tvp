/**
 * @file video.cpp
 * Defines display and GUI features
 */

#include "video/video.h"
#include "gpu/utils.h"
#include "util/helpers.h"
#include "util/log.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <optional>

using namespace gpu;
using namespace controller;
using namespace cartridge;

const auto MULT = 3;

namespace video {

Video::Video(ControllerInterface *controller,
             CartridgeMetadata *cartridge_metadata)
    : window(std::make_unique<sf::RenderWindow>(
          sf::VideoMode(SCREEN_WIDTH * MULT, SCREEN_HEIGHT * MULT),
          "Welcome to TVP")),
      window_texture(std::make_unique<sf::Texture>()),
      window_image(std::make_unique<sf::Image>()),
      window_sprite(std::make_unique<sf::Sprite>()), controller(controller),
      cartridge_metadata(cartridge_metadata) {

	// Set some window properties
	window->setVerticalSyncEnabled(true);
	window->setFramerateLimit(59.97f);

	// Set the image size
	window_image->create(SCREEN_WIDTH * MULT, SCREEN_HEIGHT * MULT);

	// Set the title of the Window if rom_title is not empty
	if (!cartridge_metadata->title.empty())
		window->setTitle(cartridge_metadata->title);
}

std::optional<Button> get_button_from_code(int64_t code) {
	switch (code) {
	case sf::Keyboard::W:
		return Button::UP;
	case sf::Keyboard::S:
		return Button::DOWN;
	case sf::Keyboard::A:
		return Button::LEFT;
	case sf::Keyboard::D:
		return Button::RIGHT;
	case sf::Keyboard::K:
		return Button::A;
	case sf::Keyboard::L:
		return Button::B;
	case sf::Keyboard::BackSpace:
		return Button::SELECT;
	case sf::Keyboard::Return:
		return Button::START;
	default:
		return {};
	}
}

void Video::event_handler() {
	sf::Event event;
	while (window->pollEvent(event)) {
		// Handle key presses
		if (event.type == sf::Event::KeyPressed) {
			if (auto button = get_button_from_code(event.key.code)) {
				controller->press_button(*button);
			}
		}

		if (event.type == sf::Event::KeyReleased) {
			if (auto button = get_button_from_code(event.key.code)) {
				controller->release_button(*button);
			}
		}

		if (event.type == sf::Event::Closed) {
			window->close();
		}
	}
}

void Video::paint(VideoBuffer &v_buffer) {
	// Handle window events
	event_handler();

	// Create pixel buffer from input
	for (int i = 0; i < PIXEL_COUNT; ++i) {
		auto pixel = v_buffer[i];
		sf::Color color;
		switch (pixel) {
		case Pixel::ZERO:
			color = sf::Color::White;
			break;
		case Pixel::ONE:
			color = sf::Color(85, 85, 85);
			break;
		case Pixel::TWO:
			color = sf::Color(170, 170, 170);
			break;
		case Pixel::THREE:
			color = sf::Color::Black;
			break;
		}
		for (int k = 0; k < MULT; k++) {
			for (int l = 0; l < MULT; l++) {
				auto a = (i % SCREEN_WIDTH) * MULT + k;
				auto b = (i / SCREEN_WIDTH) * MULT + l;
				window_image->setPixel(a, b, color);
			}
		}
	}

	// Draw window
	window->clear();
	window_texture->loadFromImage(*(window_image));
	window_sprite->setTexture(*(window_texture), true);
	window->draw(*(window_sprite));
	window->display();
}

} // namespace video
