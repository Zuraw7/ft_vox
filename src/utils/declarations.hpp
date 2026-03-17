#ifndef FT_VOX_DECLARATIONS_HPP
#define FT_VOX_DECLARATIONS_HPP

/* SCREEN RESOLUTION */
struct Resolution {
    int width;
    int height;
    constexpr static float aspect = 16.0f / 9.0f;

    Resolution(const int w, const int h) : width(w), height(h) {}
};

extern Resolution qHD;
extern Resolution HD;
extern Resolution HDP;
extern Resolution FHD;
extern Resolution QHD;

constexpr int MAX_PER_FRAME = 4;

constexpr int CHUNK_SIZE = 16;
constexpr int WORLD_HEIGHT = 256;
constexpr int WORLD_SIZE = 16384;
constexpr int RENDER_DISTANCE = 5;
constexpr int CACHE_DISTANCE = RENDER_DISTANCE + 2;

#endif //FT_VOX_DECLARATIONS_HPP