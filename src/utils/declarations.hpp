#ifndef FT_VOX_DECLARATIONS_HPP
#define FT_VOX_DECLARATIONS_HPP

/* SCREEN RESOLUTION */
struct Resolution {
    int width;
    int height;
    constexpr static float aspect = 16.0f / 9.0f;

    Resolution(const int w, const int h) : width(w), height(h) {}
};

Resolution qHD(960, 540);
Resolution HD(1280, 720);
Resolution HDP(1600, 900);
Resolution FHD(1920, 1080);
Resolution QHD(2560, 1440);


#endif //FT_VOX_DECLARATIONS_HPP