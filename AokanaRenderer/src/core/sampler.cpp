#include "sampler.h"

namespace Aokana {

    std::vector<Sample> SimpleSampler::sample() const {
        std::vector<Sample> samples(samples_per_PIxel);
        for (int i = 0;i < samples_per_PIxel;++i) {
            samples[i].u = Random::RandomDouble();
            samples[i].v = Random::RandomDouble();
        }
        return samples;
    }
}