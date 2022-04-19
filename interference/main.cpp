#include <unordered_map>
#include <vector>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "fleshgrad.h"
#include "shortgold.h"
#include "orangeblackpurple.h"
#include "threecolor.h"

#define DEBUG false

struct Point2D {
    int x;
    int y;
    float scale;
};

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "Fancy Image Thingy";
	}

public:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		int iters = 100;

        if (GetMouse(0).bPressed) {
            points.push_back({GetMouseX(), GetMouseY(), scale});
        } else if (GetMouse(1).bPressed) {
            points.pop_back();
        } else if (GetKey(olc::Key::SPACE).bPressed) {
            points.clear();
        } else if (GetKey(olc::Key::RIGHT).bPressed) {
            pattern = (pattern + 1) % 2;
        } else if (GetKey(olc::Key::LEFT).bPressed) {
            pattern = (pattern + 1) % 2;
        } else if (GetKey(olc::Key::UP).bPressed) {
            color = (color + 1) % 5;
        } else if (GetKey(olc::Key::DOWN).bPressed) {
            color = (color + 4) % 5;
        } else if (GetKey(olc::Key::Z).bPressed) {
            scale += 0.05f;
        } else if (GetKey(olc::Key::X).bPressed) {
            scale -= 0.05f;
        }

        const float SIN_SCALE = 0.1f;
        for (int y = 0; y < ScreenHeight(); ++y) {
            for (int x = 0; x < ScreenWidth(); ++x) {
                float N = points.size() + 1;
                float total = 0.0;
                for (auto const& p : points) {
                    float dx = p.x - x;
                    float dy = p.y - y;
                    float d = sqrt(dx * dx + dy * dy);
                    if (pattern == 0) {
                        total += sin(d * p.scale) / N;
                    } else if (pattern == 1) {
                        total += tan(d * p.scale) / N;
                    }
                }
                float dx = GetMouseX() - x;
                float dy = GetMouseY() - y;
                float d = sqrt(dx * dx + dy * dy);
                if (pattern == 0) {
                    total += sin(d * scale) / N;
                } else {
                    total += tan(d * scale) / N;
                }

                total += 1;
                total *= 128;

                olc::Pixel col;
                if (color == 0) {
                    col = olc::Pixel(total, total, total);
                } else if (color == 1) {
                    col = fleshgrad[std::min(255, std::max(0, (int)total))];
                } else if (color == 2) {
                    col = shortgold[std::min(255, std::max(0, (int)total))];
                } else if (color == 3) {
                    col = orangeblackpurple[std::min(255, std::max(0, (int)total))];
                } else if (color == 4) {
                    col = threecolor[std::min(255, std::max(0, (int)total))];
                }
                Draw(x, y, col);
            }
        }

        return !DEBUG;
	}

    int pattern = 0;
    int color = 0;
    float scale = 0.1;
    std::vector<Point2D> points;
};

int main()
{
	Example demo;

    printf("- Click to add a point.\n");
    printf("- Right click to remove previous point.\n");
    printf("- Space to clear all points.\n");
    printf("- Z/X to change frequency of point.\n");
    printf("- left/right to change between sin/tan.\n");
    printf("- up/down to change color mappings.\n");

	if (demo.Construct(512, 512, 2, 2))
		demo.Start();
	return 0;
}
