#include <unordered_map>
#include <stack>

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
		// Called once at the start, so create things here
        stack.push(0);
		return true;
	}

    void handlePixel(int i, int& iters) 
    {
        static int dx[] = { 1, 0, -1, 0 };
        static int dy[] = { 0, 1, 0, -1 };
        int x = i % ScreenWidth();
        int y = i / ScreenWidth();

        float R = 0, G = 0, B = 0;
        int cnt = 0;
        for (int i = 0; i < 4; i++) {
            int nx = (x + dx[i] + ScreenWidth()) % ScreenWidth();
            int ny = (y + dy[i] + ScreenHeight()) % ScreenHeight();
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
            const float mut = 25.0f;
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
            int nx = (x + dx[j] + ScreenWidth()) % ScreenWidth();
            int ny = (y + dy[j] + ScreenHeight()) % ScreenHeight();
            int next = ny * ScreenWidth() + nx;
            if (visited.count(next) == 0) {
                stack.push(next);
            }
        }
    }

	bool OnUserUpdate(float fElapsedTime) override
	{
		int iters = 100;

        while (stack.size() > 0 && iters > 0) {
            int i = stack.top();
            stack.pop();
            if (visited.count(i))
                continue;
            handlePixel(i, iters);
        }
        return !DEBUG;
	}

    std::unordered_map<int, FPixel> visited;
    std::stack<int> stack;
};

int main()
{
	Example demo;
	if (demo.Construct(256, 256, 4, 4))
		demo.Start();
	return 0;
}
