#include "olcPixelGameEngine.h"

namespace olc {
	class Font : public PGEX {
	public:
		Font(std::string filename) {
			std::ifstream file;
			file.open(filename, std::ios::in);
			if (!file.is_open())
				throw std::exception("Couldn't open file!");
			std::string data;
			file >> data;
			file.close();

			//Yes, literally stolen from the pge
			fontSprite = new olc::Sprite(128, 48);
			int px = 0, py = 0;
			for (int b = 0; b < 1024; b += 4)
			{
				uint32_t sym1 = (uint32_t)data[b + 0] - 48;
				uint32_t sym2 = (uint32_t)data[b + 1] - 48;
				uint32_t sym3 = (uint32_t)data[b + 2] - 48;
				uint32_t sym4 = (uint32_t)data[b + 3] - 48;
				uint32_t r = sym1 << 18 | sym2 << 12 | sym3 << 6 | sym4;

				for (int i = 0; i < 24; i++)
				{
					int k = r & (1 << i) ? 255 : 0;
					fontSprite->SetPixel(px, py, olc::Pixel(k, k, k, k));
					if (++py == 48) { px++; py = 0; }
				}
			}
		}

		//Also completely copy pasted from the pge
		void DrawString(int32_t x, int32_t y, std::string sText, Pixel col = olc::WHITE, uint32_t scale = 1) {
			int32_t sx = 0;
			int32_t sy = 0;
			for (auto c : sText)
			{
				if (c == '\n')
				{
					sx = 0; sy += 8 * scale;
				}
				else
				{
					int32_t ox = (c - 32) % 16;
					int32_t oy = (c - 32) / 16;

					if (scale > 1)
					{
						for (uint32_t i = 0; i < 8; i++)
							for (uint32_t j = 0; j < 8; j++)
								if (fontSprite->GetPixel(i + ox * 8, j + oy * 8).r > 0)
									for (uint32_t is = 0; is < scale; is++)
										for (uint32_t js = 0; js < scale; js++)
											pge->Draw(x + sx + (i*scale) + is, y + sy + (j*scale) + js, col);
					}
					else
					{
						for (uint32_t i = 0; i < 8; i++)
							for (uint32_t j = 0; j < 8; j++)
								if (fontSprite->GetPixel(i + ox * 8, j + oy * 8).r > 0)
									pge->Draw(x + sx + i, y + sy + j, col);
					}
					sx += 8 * scale;
				}
			}
		};
	private:
		Sprite *fontSprite;
	};
}