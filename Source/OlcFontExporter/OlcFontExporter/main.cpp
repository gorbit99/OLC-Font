#include <iostream>
#include <string>
#include <fstream>

#include "lodepng.h"

void usage(char * fileName) {
	std::cout << "Usage:\n";
	std::cout << "    " << fileName << " <path to font image>\n";
	std::cout << "    for more help type " << fileName << " -?\n";
}

void help(char * fileName) {
	std::cout << fileName << " <path> [-f #<color>] [-b #<color>] [-o <output>] [-e <true/false>]\n";
	std::cout << "Flags:\n";
	std::cout << "    -f Specifies foreground color\n";
	std::cout << "    -b Specifies background color\n";
	std::cout << "    -o Specifies output file\n";
	std::cout << "    -e Specifies if error should be thrown on unknown color\n";
	std::cout << "Colors should be in hex color codes preceded by a hashtag. Order: rrggbb\n";
}

union Color {
	unsigned int rgba;
	unsigned char values[4];
};

bool checkColorFormat(char * col) {
	int length = 0;
	while (col[length] != '\0') {
		if (!((length != 0 && ((col[length] >= '0' && col[length] <= '9') || (col[length] >= 'a' && col[length] <= 'f'))) || (length == 0 && col[length] == '#')))
			return false;
		length++;
	}
	if (length != 9)
		return false;
	return true;
}

int main(int argc, char ** argv) {
	if (argc == 1) {
		usage(argv[0]);
		return 0;
	}
	if (argc == 2) {
		if (argv[1] == "-?") {
			help(argv[0]);
			return 0;
		}
	}

	Color fg = { 0x000000ff }, bg = { 0xffffffff };
	bool errorOnUnknown = true;
	std::string inName = "", outName = "output.of";

	//READ IN ARGUMENTS
	//ARGS:
	//	-f specifies foreground color
	//	-b specifies background color
	//	-o specifies output file
	//	-e specifies if error should be thrown on unknown color
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (i == argc - 1) {
				std::cout << "No parameter after flag " << argv[i] << "!";
				return 0;
			}
			else if (argv[i + 1][0] == '-') {
				std::cout << "No parameter after flag " << argv[i] << "!";
				return 0;
			}
			char *col;
			switch (argv[i][1])
			{
			case 'f':
				col = argv[i + 1];
				if (!checkColorFormat(col)) {
					std::cout << "Invalid color format after flag -f!";
					return 0;
				}
				std::stoi(col + 1, &fg.rgba, 16);
				break;
			case 'b':
				col = argv[i + 1];
				if (!checkColorFormat(col)) {
					std::cout << "Invalid color format after flag -f!";
					return 0;
				}
				std::stoi(col + 1, &bg.rgba, 16);
				break;
			case 'o':
				outName = argv[i + 1];
				break;
			case 'e':
				if (argv[i + 1] == "true")
					errorOnUnknown = true;
				else if (argv[i + 1] == "false")
					errorOnUnknown = false;
				else {
					std::cout << "Invalid parameter after flag -e!";
					return 0;
				}
				break;
			default:
				break;
			}
			i++;
		}
		else {
			if (inName != "") {
				std::cout << "Multiple input filenames were specified!";
				return 0;
			}
			else
				inName = argv[i];
		}
	}

	//OPEN PNG
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, inName);
	if (error) {
		std::cout << "Couldn't decode file!\n";
		std::cout << "Error:\n" << lodepng_error_text(error) << '\n';
		return 0;
	}

	if (width != 128 || height != 48) {
		std::cout << "Invalid dimensions!\n";
		return 0;
	}

	//CONVERT PNG TO OTF

	auto colorAt = [image, width, height](int x, int y) {
		Color color;
		color.values[0] = image[(y * width + x) * 4 + 3];
		color.values[1] = image[(y * width + x) * 4 + 2];
		color.values[2] = image[(y * width + x) * 4 + 1];
		color.values[3] = image[(y * width + x) * 4 + 0];
		return color;
	};

	unsigned long character = 0;
	Color color;
	std::string data;
	int px = 0, py = 0;
	for (int b = 0; b < 1024; b += 4)
	{
		uint32_t r = 0;

		for (int i = 0; i < 24; i++)
		{
			color = colorAt(px, py);
			if (color.rgba != fg.rgba && color.rgba != bg.rgba && errorOnUnknown) {
				std::cout << "Unknown color in image!\n";
				return 0;
			}
			if (++py == 48)
				px++, py = 0;
			r |= (color.rgba == fg.rgba) << i;
		}

		uint32_t sym1 = (r & 0xfc0000) >> 18;
		uint32_t sym2 = (r & 0x03f000) >> 12;
		uint32_t sym3 = (r & 0x000fc0) >> 6;
		uint32_t sym4 = r & 0x00003f;
		data += sym1 + 48;
		data += sym2 + 48;
		data += sym3 + 48;
		data += sym4 + 48;
	}

	//WRITING RESULT
	std::ofstream file;
	file.open(outName, std::ios::out | std::ios::trunc);
	if (!file.is_open()) {
		std::cout << "Couldn't open output file!\n";
		return 0;
	}
	file << data;
	file.close();
}