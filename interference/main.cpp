#include <unordered_map>
#include <vector>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "image.h"

#include "fleshgrad.h"
#include "shortgold.h"
#include "orangeblackpurple.h"
#include "threecolor.h"
#include "greenblue.h"

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
		sAppName = "Interference";
	}

public:
	bool OnUserCreate() override
	{
		return true;
	}

    float value_for(int x, int y, int px, int py, float scale) {
        float dx = px - x;
        float dy = py - y;
        float d = sqrt(dx * dx + dy * dy);
        float value = 0;
        if (pattern == 0) {
            value = sin(d * scale);
        } else if (pattern == 1) {
            value = tan(d * scale);
        }
        return value;
    }

    void save_as_image() {
        auto img = Image(ScreenWidth(), ScreenHeight());
        auto buffer = GetDrawTarget();

        for (int y = 0; y < buffer->height; ++y) {
            for (int x = 0; x < buffer->width; ++x) {
                auto col = buffer->GetPixel(x, y);
                img.set_pixel(x, y, col.r, col.g, col.b);
            }
        }
        char filename[256];
        sprintf(filename, "dfs_%lu.ppm", (unsigned long)time(NULL));
        printf("Saving %s with (%dx%d)\n", filename, ScreenWidth(), ScreenHeight());
        img.save(filename);
    }

	bool OnUserUpdate(float fElapsedTime) override
	{
        bool update = false;

        int mx = GetMouseX();
        int my = GetMouseY();

        if (px != mx || py != my)
            update = true;

        if (GetMouse(0).bPressed) {
            points.push_back({mx, my, scale});
        } else if (GetMouse(1).bPressed) {
            if (points.size() > 0)
                points.pop_back();
            update = true;
        } else if (GetKey(olc::Key::SPACE).bPressed) {
            points.clear();
            update = true;
        } else if (GetKey(olc::Key::RIGHT).bPressed) {
            pattern = (pattern + 1) % 2;
            update = true;
        } else if (GetKey(olc::Key::LEFT).bPressed) {
            pattern = (pattern + 1) % 2;
            update = true;
        } else if (GetKey(olc::Key::UP).bPressed) {
            color = (color + 1) % 6;
            update = true;
        } else if (GetKey(olc::Key::DOWN).bPressed) {
            color = (color + 5) % 6;
            update = true;
        } else if (GetKey(olc::Key::Z).bPressed) {
            scale *= 1.2f;
            update = true;
        } else if (GetKey(olc::Key::X).bPressed) {
            scale /= 1.2f;
            update = true;
        }

#ifndef __EMSCRIPTEN__
        if (GetKey(olc::Key::Q).bPressed || GetKey(olc::Key::ESCAPE).bPressed) {
            return false;
        } else if (GetKey(olc::Key::S).bPressed) {
            save_as_image();
        }
#endif

        if (!update)
            return true;

        // #pragma omp parallel for schedule(dynamic)
        for (int y = 0; y < ScreenHeight(); ++y) {
            for (int x = 0; x < ScreenWidth(); ++x) {

                float N = points.size();
                float total = 0.0;
                for (auto const& p : points)
                    total += value_for(x, y, p.x, p.y, p.scale);

                int mx = GetMouseX();
                int my = GetMouseY();
                if (mx != 0 && mx != ScreenWidth() - 1 && my != 0 && my != ScreenHeight() - 1) {
                    N += 1;
                    total += value_for(x, y, mx, my, this->scale);
                }
                total = (total / N + 1) * 128;

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
                } else if (color == 5) {
                    col = greenblue[std::min(255, std::max(0, (int)total))];
                }
                Draw(x, y, col);
            }
        }

        px = mx;
        py = my;

        return !DEBUG;
	}

    int pattern = 0;
    int color = 0;

    int px = -1;
    int py = -1;

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
    printf("- Left/right to change between sin/tan.\n");
    printf("- Up/down to change color mappings.\n");

	if (demo.Construct(800, 500, 2, 2))
		demo.Start();
	return 0;
}
