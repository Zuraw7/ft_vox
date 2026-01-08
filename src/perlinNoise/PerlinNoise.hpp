#ifndef FT_VOX_PERLINNOISE_HPP
#define FT_VOX_PERLINNOISE_HPP
#include <vector>

#define PERLIN_DEFAULT_Y 0.12345
#define PERLIN_DEFAULT_Z 0.34567

class PerlinNoise {
public:
    PerlinNoise(int seed);

    float noise2D(float x, float y);
    float noise3D(float x, float y, float z);

    float fbm2D(float x, float y, int octaves = 4, float persistence = 0.5f);
    float fbm3D(float x, float y, float z, int octaves = 4, float persistence = 0.5f);

private:
    std::vector<int> m_permutation;

    float fade(float t);
    float lerp(float a, float b, float t);
    float grad(int hash, float x, float y, float z);
};


#endif //FT_VOX_PERLINNOISE_HPP