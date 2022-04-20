#include <unordered_map>
#include <vector>
#include <time.h>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "image.h"

#define DEBUG false

struct FPixel {
    float r;
    float g;
    float b;
    float a;
};

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "Watercolor";
	}

public:
	bool OnUserCreate() override
	{
        SetPixelMode(olc::Pixel::ALPHA);

		reset();
		return true;
	}

    void reset() {
        start_color = {
            uint8_t(rand() % 100 + 128),
            uint8_t(rand() % 100 + 128),
            uint8_t(rand() % 100 + 128),
        };
        olc::Pixel base_color = olc::Pixel(0, 0, 0);
        if (use_alpha) base_color = start_color;

        for (int y = 0; y < ScreenHeight(); ++y) {
            for (int x = 0; x < ScreenWidth(); ++x) {
                Draw(x, y, base_color);
            }
        }
        visited.clear();
        stack.clear();

        if (mode == 0) {
            for (int i = 0; i < ScreenWidth(); ++i) {
                int dummy = 0;
                handlePixel(i, dummy);
            }
        }
    }

    void handlePixel(int i, int &iters) {
        static int dx[] = { 1, 0, -1, 0 };
        static int dy[] = { 0, 1, 0, -1 };
        int x = i % ScreenWidth();
        int y = i / ScreenWidth();

        float R = 0, G = 0, B = 0, A = 0;
        int cnt = 0;
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            int next = ny * ScreenWidth() + nx;
            if (visited.count(next)) {
                auto col = visited[next];
                R += col.r;
                G += col.g;
                B += col.b;
                A += col.a;
                cnt++;
            }
        }

        olc::Pixel col;
        if (cnt == 0) {
            R = start_color.r;
            G = start_color.g;
            B = start_color.b;
            A = start_color.a;
        } else {
            const float mut = use_alpha ? 2.0f : 8.0f;
            R = R/cnt + (drand48() * mut) - (mut/2);
            G = G/cnt + (drand48() * mut) - (mut/2);
            B = B/cnt + (drand48() * mut) - (mut/2);

            R = std::fmin(255, std::fmax(0, R));
            G = std::fmin(255, std::fmax(0, G));
            B = std::fmin(255, std::fmax(0, B));

            A = A/cnt + (drand48() * mut) - (mut/2);
            A = std::fmin(255, std::fmax(150, A));
        }

        if (!use_alpha) A = 255;

        col = olc::Pixel(R, G, B, A);
        visited.insert({i, {R, G, B, A}});
        Draw(x, y, col);
        iters--;

        int perm[4] = {0, 1, 2, 3};

        // Shuffle the order of the neighbours
        for (int i = 0; i < 4; i++) {
            int j = rand() % 4;
            int tmp = perm[i];
            perm[i] = perm[j];
            perm[j] = tmp;
        }

        for (int i = 0; i < 4; i++) {
            int j = perm[i];
            int nx = x + dx[j];
            int ny = y + dy[j];
            int next = ny * ScreenWidth() + nx;
            if (nx < 0 || nx >= ScreenWidth() || ny < 0 || ny >= ScreenHeight()) {
                continue;
            }
            if (visited.count(next) == 0) {
                stack.push_back(next);
            }
        }
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
        sprintf(filename, "water_%lu.ppm", (unsigned long)time(NULL));
        printf("Saving %s with (%dx%d)\n", filename, ScreenWidth(), ScreenHeight());
        img.save(filename);
    }

	bool OnUserUpdate(float fElapsedTime) override
	{
        if (GetMouse(0).bPressed) {
            int x = GetMouseX();
            int y = GetMouseY();
            int i = y * ScreenWidth() + x;
            int dummy = 0;
            handlePixel(i, dummy);
        }

        if (GetKey(olc::Key::SPACE).bPressed) {
            reset();
        } else if (GetKey(olc::Key::RIGHT).bPressed) {
            mode = (mode + 1) % 2;
            reset();
        } else if (GetKey(olc::Key::LEFT).bPressed) {
            mode = (mode + 1) % 2;
            reset();
        } else if (GetKey(olc::Key::Q).bPressed || GetKey(olc::Key::ESCAPE).bPressed) {
            return false;
        } else if (GetKey(olc::Key::A).bPressed) {
            use_alpha = !use_alpha;
            reset();
        }

#ifndef __EMSCRIPTEN__
        if (GetKey(olc::Key::S).bPressed) {
            save_as_image();
        }
#endif

        constexpr float NUM_SECONDS = 8;
        int iters = fElapsedTime * ScreenHeight() * ScreenWidth() / NUM_SECONDS;

        while (stack.size() > 0 && iters > 0) {
            int rand_idx = rand() % stack.size();
            std::swap(stack[stack.size() - 1], stack[rand_idx]);
            int i = stack.back();
            stack.pop_back();
            if (visited.count(i))
                continue;
            handlePixel(i, iters);
        }
        return !DEBUG;
	}

    olc::Pixel start_color;
    bool use_alpha = false;
    int mode = 0;
    std::unordered_map<int, FPixel> visited;
    std::vector<int> stack;
};

int main(int argc, char**argv)
{
    printf("- Click to start painting from a point.\n");
    printf("- Space to reset.\n");
    printf("- Right/Left to change mode.\n");
    printf("- A to turn on/off alpha blending.\n");
	Example demo;
	if (demo.Construct(800, 500, 2, 2))
		demo.Start();
	return 0;
}
