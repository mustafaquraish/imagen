#pragma once

#include <stdint.h>

typedef uint8_t u8;

struct RGB {
    u8 r;
    u8 g;
    u8 b;
};

struct Image {
    int width = 0;
    int height = 0;
    u8* data;

    Image(int width, int height)
        : width(width)
        , height(height)
        , data(new u8[width * height * 3])
    {
    }

    ~Image() { delete[] data; }

    void save(const char* filename)
    {
        FILE* fp = fopen(filename, "wb");
        if (!fp) {
            fprintf(stderr, "Failed to open file %s\n", filename);
            return;
        }
        fprintf(fp, "P6\n%d %d\n255\n", width, height);
        fwrite(data, 1, width * height * 3, fp);
        fclose(fp);
    }

    void set_pixel(int i, int j, u8 R, u8 G, u8 B)
    {
        int idx = (i + j * width) * 3;
        data[idx + 0] = R;
        data[idx + 1] = G;
        data[idx + 2] = B;
    }

    void set_pixel(int i, int j, RGB &rgb)
    {
        set_pixel(i, j, rgb.r, rgb.g, rgb.b);
    }

    RGB pixel(int i, int j)
    {
        int idx = (i + j * width) * 3;
        return RGB {
            data[idx + 0],
            data[idx + 1],
            data[idx + 2]
        };
    }
};
