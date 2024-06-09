#include "sampler.h"

namespace Aokana {

    std::vector<Sample> SimpleSampler::Sampling() const {
        std::vector<Sample> samples(samples_per_pixel);
        for (int i = 0;i < samples_per_pixel;++i) {
            samples[i].u = RandomDoubleIn01();
            samples[i].v = RandomDoubleIn01();
        }
        return samples;
    }
}