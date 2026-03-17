#ifndef FT_VOX_UTILS_TPP
#define FT_VOX_UTILS_TPP

#include "declarations.hpp"

template<typename T>
T chunkToWorld(T pos) {
    return pos * CHUNK_SIZE;
}

template<typename T>
T worldToChunk(T pos) {
    return pos / CHUNK_SIZE;
}

#endif //FT_VOX_UTILS_TPP
