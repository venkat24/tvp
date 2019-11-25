/**
 * @file video.h
 * Declares the Video class for display and GUI features
 */
#pragma once

#include "cartridge/meta_cartridge.h"
#include "controller/controller_interface.h"
#include "gpu/utils.h"
#include "video/video_interface.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>

namespace video {

class Video : public VideoInterface {
  private:
	/**
	 * Window context that the application will be rendered in
	 */
	std::unique_ptr<sf::RenderWindow> window;

	/**
	 * Window texture, that serves as an in-memory buffer of the output pixels
	 */
	std::unique_ptr<sf::Texture> window_texture;

	/**
	 * Window image, that the texture is loaded
	 */
	std::unique_ptr<sf::Image> window_image;

	/**
	 * Window sprite that the texture is loaded to
	 */
	std::unique_ptr<sf::Sprite> window_sprite;

	/**
	 * Pointer to controller instance
	 */
	controller::ControllerInterface *controller;

	/**
	 * Pointer to cartridge instance
	 */
	cartridge::CartridgeMetadata *cartridge_metadata;

	/**
	 * Processes all external events like key presses
	 */
	void event_handler();

  public:
	/**
	 * Constructor
	 */
	Video(controller::ControllerInterface *controller,
	      cartridge::CartridgeMetadata *cartridge_metadata);

	/**
	 * Print the contents of the buffer to the terminal
	 */
	void paint(gpu::VideoBuffer &v_buffer);
};

} // namespace video
