#include "sampler.h"

namespace Asuka {

    std::vector<Sample> SimpleSampler::sample() const {
        std::vector<Sample> samples(samples_per_pixel);
        for (int i = 0;i < samples_per_pixel;++i) {
            samples[i].u = random_double();
            samples[i].v = random_double();
        }
        return samples;
    }
}