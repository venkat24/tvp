/**
 * @file video.h
 * Declares the Video class for display and GUI features
 */
#ifndef VIDEO_INCLUDE_VIDEO_VIDEO_H
#define VIDEO_INCLUDE_VIDEO_VIDEO_H

#include "gpu/utils.h"
#include "video/video_interface.h"

#include <array>

namespace video {

class Video : public VideoInterface {
  public:
	/**
	 * Constructor
	 */
	Video();

	/**
	 * Print the contents of the buffer to the terminal
	 */
	void paint(gpu::VideoBuffer &v_buffer);
};

} // namespace video

#endif
