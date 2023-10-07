#pragma once

#include "camera.h"
#include "ray.h"
#include "scene.h"
#include "sampler.h"

namespace Asuka {
    class Integrator {
    public:
        int max_depth = 5;
        virtual void Render(const Camera& camera) = 0;
        virtual color Li(const Ray& ray, const color& background, int depth) = 0;
    };

    class SamplerIntegrator : public Integrator {
    public:
        std::shared_ptr<Scene> scene;
        std::shared_ptr<Sampler> sampler;
    public:
        virtual color Li(const Ray& ray, const color& background, int depth) override;
        virtual void Render(const Camera& camera) override;
        void RenderOneTile(const Camera& camera, const FilmTile& tile);
        void RenderWithMultithreading(const Camera& camera, bool enable_gui = true);
    };
}