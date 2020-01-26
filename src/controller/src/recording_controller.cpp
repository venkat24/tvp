#include "controller/recording_controller.h"

using namespace controller;

RecordingController::RecordingController(std::string json_file_name) {
	Controller();
	json_file.open(json_file_name);
}

void RecordingController::press_button(Button button) {
	frame_button_value["frame"] = Json::Value::UInt64(ticks);
	frame_button_value["button"] = Json::Value((int)button);
	json_file << frame_button_value << std::endl;
	set_button(button, true);
}

void RecordingController::tick() { ticks++; }