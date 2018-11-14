/**
 * @file video_interface.h
 * Declares the Interface for Video - to display a buffer to the screen
 */
#ifndef VIDEO_INCLUDE_VIDEO_VIDEO_INTERFACE_H
#define VIDEO_INCLUDE_VIDEO_VIDEO_INTERFACE_H

#include "gpu/utils.h"

#include <array>

namespace video {

class VideoInterface {
  public:
	/**
	 * This method takes a VideoBuffer array, and outputs it to the display
	 *
	 * @param v_buffer Buffer to display
	 */
	virtual void paint(gpu::VideoBuffer &v_buffer) = 0;
};

} // namespace video

#endif
