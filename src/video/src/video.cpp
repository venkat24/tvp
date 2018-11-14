/**
 * @file video.cpp
 * Defines display and GUI features
 */

#include "video/video.h"
#include "gpu/utils.h"

#include <iostream>
using namespace std;
using namespace gpu;

namespace video {

Video::Video() {}

void Video::paint(VideoBuffer &v_buffer) {
	for (int i = 0; i < v_buffer.size(); ++i) {
		switch (v_buffer[i]) {
		case Pixel::ZERO:
			cout << "\033[1;37m███\033[0m";
			break;
		case Pixel::ONE:
			cout << " " << static_cast<int>(v_buffer[i]) << " ";
			break;
		case Pixel::TWO:
			cout << "\033[1;30m███\033[0m";
			break;
		case Pixel::THREE:
			cout << " " << static_cast<int>(v_buffer[i]) << " ";
			break;
		}

		if (i % SCREEN_WIDTH == (SCREEN_WIDTH - 1)) {
			cout << endl;
		}
	}
	cout << endl << endl;
}

} // namespace video
