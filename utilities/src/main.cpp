#include <iostream>
#include <string>
#include <sstream>

#include <manta.h>
#include <raw_file.h>
#include <image_plane.h>
#include <image_handling.h>

int main() {
	std::string fname;

	std::cout << "Enter filename: ";
	std::cin >> fname;

	manta::RawFile rawFile;
	manta::ImagePlane sceneBuffer;
	bool result = rawFile.readRawFile(fname.c_str(), &sceneBuffer);

	if (!result) {
		std::cout << "Failed to open file" << std::endl;
		return 0;
	}

	std::cout << "Width: " << sceneBuffer.getWidth() << std::endl;
	std::cout << "Height: " << sceneBuffer.getHeight() << std::endl;
	std::cout << "Max: " << sceneBuffer.getMax() << std::endl;
	std::cout << "Min: " << sceneBuffer.getMin() << std::endl;

	// Remove the file extension
	for (int i = (int)fname.length(); i >= 0; i--) {
		if (fname[i] == '.') {
			fname = fname.substr(0, i);
		}
	}

	manta::ImagePlane temp;
	temp.copyFrom(&sceneBuffer);
	manta::ImagePlane prev;

	bool prevValid = false;
	int iteration = 0;

	// Editing loop
	bool done = false;
	while (!done) {
		std::string command;
		std::cout << "-------------------------------" << std::endl;
		std::cout << "Command: ";
		std::cin >> command;

		bool write = false;
		if (command == "scale" || command == "s") {
			manta::math::real scale;
			std::cout << "Scale factor: ";
			std::cin >> scale;

			temp.scale(scale);

			write = true;
		}
		else if (command == "undo" || command == "u") {
			if (prevValid) {
				temp.destroy();
				temp.copyFrom(&prev);
				write = true;
			}
			else {
				std::cout << "Nothing to undo" << std::endl;
			}
		}
		else if (command == "gamma" || command == "g") {
			manta::math::real gamma_div, gamma_num;
			std::cout << "Scale factor: " << std::endl;
			std::cout << "  Num: ";
			std::cin >> gamma_num;
			std::cout << "  Div: ";
			std::cin >> gamma_div;

			temp.applyGammaCurve(gamma_num / gamma_div);

			write = true;
		}
		else if (command == "reset" || command == "r") {
			if (temp.isInitialized()) temp.destroy();
			temp.copyFrom(&sceneBuffer);
			write = true;
		}
		else if (command == "quit" || command == "q") {
			write = false;
			done = true;
		}

		if (write) {
			prev.copyFrom(&temp);
			prevValid = true;

			std::stringstream ss;
			ss << "_E" << iteration;

			manta::SaveImageData(temp.getBuffer(), temp.getWidth(), temp.getHeight(), (fname + ss.str() + ".bmp").c_str());

			iteration++;
		}
	}

	if (temp.isInitialized()) temp.destroy();
	if (prev.isInitialized()) prev.destroy();
	if (sceneBuffer.isInitialized()) sceneBuffer.destroy();

	return 0;
}
