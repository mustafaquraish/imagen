#include <unordered_map>
#include <vector>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "fleshgrad.h"
#include "shortgold.h"

#define DEBUG false

struct Point2D {
    int x;
    int y;
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
            points.push_back({GetMouseX(), GetMouseY()});
        } else if (GetMouse(1).bPressed) {
            points.pop_back();
        } else if (GetKey(olc::Key::SPACE).bPressed) {
            points.clear();
        } else if (GetKey(olc::Key::RIGHT).bPressed) {
            pattern = (pattern + 1) % 2;
        } else if (GetKey(olc::Key::LEFT).bPressed) {
            pattern = (pattern + 1) % 2;
        } else if (GetKey(olc::Key::UP).bPressed) {
            color = (color + 1) % 3;
        } else if (GetKey(olc::Key::DOWN).bPressed) {
            color = (color + 2) % 3;
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
                        total += sin(d * SIN_SCALE) / N;
                    } else if (pattern == 1) {
                        total += tan(d * SIN_SCALE) / N;
                    }
                }
                float dx = GetMouseX() - x;
                float dy = GetMouseY() - y;
                float d = sqrt(dx * dx + dy * dy);
                if (pattern == 0) {
                    total += sin(d * SIN_SCALE) / N;
                } else {
                    total += tan(d * SIN_SCALE) / N;
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
                }
                Draw(x, y, col);
            }
        }

        return !DEBUG;
	}

    int pattern = 0;
    int color = 0;
    std::vector<Point2D> points;
};

int main()
{
	Example demo;
	if (demo.Construct(256, 256, 4, 4))
		demo.Start();
	return 0;
}
