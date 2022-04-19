#include <unordered_map>
#include <vector>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define DEBUG false

struct FPixel {
    float r;
    float g;
    float b;
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
		reset();
		return true;
	}

    void reset() {
        for (int y = 0; y < ScreenHeight(); ++y) {
            for (int x = 0; x < ScreenWidth(); ++x) {
                Draw(x, y, olc::BLACK);
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

        float R = 0, G = 0, B = 0;
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
                cnt++;
            }
        }

        olc::Pixel col;
        if (cnt == 0) {
            R = 255.0;
            G = 128.0;
            B = 177.0;
        } else {
            const float mut = 10.0f;
            R = R/cnt + (drand48() * mut) - (mut/2);
            G = G/cnt + (drand48() * mut) - (mut/2);
            B = B/cnt + (drand48() * mut) - (mut/2);
            R = std::fmin(255, std::fmax(0, R));
            G = std::fmin(255, std::fmax(0, G));
            B = std::fmin(255, std::fmax(0, B));
        }

        col = olc::Pixel(R, G, B);
        visited.insert({i, {R, G, B}});
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
        }


		int iters = 100;
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

    int mode = 0;
    std::unordered_map<int, FPixel> visited;
    std::vector<int> stack;
};

int main()
{
    printf("- Click to start painting from a point.\n");
    printf("- Space to reset.\n");
    printf("- Right/Left to change mode.\n");
	Example demo;
	if (demo.Construct(800, 500, 2, 2))
		demo.Start();
	return 0;
}
