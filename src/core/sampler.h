#pragma once

#include <vector>

#include "utils.h"

namespace Asuka {
    struct Sample {
        double u;
        double v;
    };

    class Sampler {
    public:
        Sampler() = default;
        Sampler(int spp) : samples_per_pixel(spp) {}

        int samples_per_pixel = 20;
        virtual std::vector<Sample> sample() const = 0;
    };

    class SimpleSampler : public Sampler {
    public:
        SimpleSampler() = default;
        SimpleSampler(int spp) : Sampler(spp) {}

        virtual std::vector<Sample> sample() const override;
    };

}