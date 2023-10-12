#pragma once

#include "camera.h"
#include "ray.h"
#include "scene.h"
#include "sampler.h"

namespace Asuka {
    class Integrator {
    public:
        int max_depth = 5;
        virtual void Render() = 0;
        virtual Color Li(const Ray& ray, const Color& background, int depth) = 0;
    };

    class SamplerIntegrator : public Integrator {
    public:
        std::shared_ptr<Scene> scene;
        std::shared_ptr<Sampler> sampler;
    public:
        virtual Color Li(const Ray& ray, const Color& background, int depth) override;
        virtual void Render() override;
        void RenderOneTile(const FilmTile& tile);
        void RenderWithMultithreading(bool enable_gui = true);
    };
}