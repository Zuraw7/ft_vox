#include "PerlinNoise.hpp"

#include <algorithm>
#include <random>

PerlinNoise::PerlinNoise(int seed) {
    for (int i = 0; i < 256; i++) {
        m_permutation.push_back(i);
    }

    std::mt19937 g(seed);
    std::shuffle(m_permutation.begin(), m_permutation.end(), g);
    m_permutation.insert(m_permutation.end(), m_permutation.begin(), m_permutation.end());

}

float PerlinNoise::noise2D(float x, float y) {
    return noise3D(x, y, PERLIN_DEFAULT_Z);
}

float PerlinNoise::noise3D(float x, float y, float z) {
    // Find unit cube that contains point
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;
    int Z = static_cast<int>(std::floor(z)) & 255;

    // Find relative X, Y, Z of point in cube
    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    // Compute fade curves for each of X, Y, Z
    float u = fade(x);
    float v = fade(y);
    float w = fade(z);

    // Hash coordinates of the 8 cube corners
    int A = m_permutation[X] + Y;
    int AA = m_permutation[A] + Z;
    int AB = m_permutation[A + 1] + Z;
    int B = m_permutation[X + 1] + Y;
    int BA = m_permutation[B] + Z;
    int BB = m_permutation[B + 1] + Z;

    // Add blended results from 8 corners of cube
    return lerp(w,
            lerp(v,
                lerp(u,
                    grad(m_permutation[AA  ], x, y, z),
                    grad(m_permutation[BA  ], x - 1, y, z)),
                lerp(u,
                    grad(m_permutation[AB  ], x, y - 1, z),
                    grad(m_permutation[BB  ], x - 1, y - 1, z))),
            lerp(v,
                lerp(u,
                    grad(m_permutation[AA+1], x, y, z - 1),
                    grad(m_permutation[BA+1], x - 1, y, z - 1)),
                lerp(u,
                    grad(m_permutation[AB+1], x, y - 1, z - 1),
                    grad(m_permutation[BB+1], x - 1, y - 1, z - 1))));
}

float PerlinNoise::fbm2D(float x, float y, int octaves, float persistence) {
    float total = 0.0f;
    float amplitude = 1.0f;

    for (int i = 0; i < octaves; i++) {
        total += noise2D(x, y) * amplitude;

        x *= 2.0f;
        y *= 2.0f;
        amplitude *= persistence;
    }
    return total;
}

float PerlinNoise::fbm3D(float x, float y, float z, int octaves, float persistence) {
    float total = 0.0f;
    float amplitude = 1.0f;

    for (int i = 0; i < octaves; i++) {
        total += noise3D(x, y, z) * amplitude;

        x *= 2.0f;
        y *= 2.0f;
        z *= 2.0f;
        amplitude *= persistence;
    }

    return total;
}

float PerlinNoise::fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinNoise::lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float PerlinNoise::grad(int hash, float x, float y, float z) {
    // Convert low 4 bits of hash code into 12 gradient directions
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}
