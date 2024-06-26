#pragma once
#include <vector>
#include <cmath>
#include <span>
#include <cassert>
#include "utils.h"
#include "vec.h"

namespace Aokana {
    constexpr double WAVE_LENGTH_MIN = 360; // 定义可见光波长范围是 360(nm) ~ 830(nm)
    constexpr double WAVE_LENGTH_MAX = 830;

    class Spectrum {
    public:
        // 返回对应波长处的光谱分布值
        virtual double operator()(double lambda) const = 0;
        // 返回该光谱分布值在其波长范围内的最大值上限
        virtual double GetMaxValue() const = 0;
    };


    // 常数光谱，在各个波长上的光谱分布值都是常数
    class ConstantSpectrum : Spectrum {
    public:
        ConstantSpectrum(double c) : c{ c } {}
        double operator()(double lambda) const override { return c; }
        double GetMaxValue() const override { return c; }
    private:
        double c;
    };


    // 稠密光谱
    // 在 [lambda_min, lambda_max] 中以 1nm 的间隔采样的光谱
    class DenselySampledSpectrum : Spectrum {
    public:
        DenselySampledSpectrum(Spectrum* spectrum, int lambda_min = WAVE_LENGTH_MIN, int lambda_max = WAVE_LENGTH_MAX) : lambda_min{ lambda_min }, lambda_max{ lambda_max }, values(lambda_max - lambda_min + 1) {
            // 如果传入的光谱不为 nullptr，那么从该光谱采样创建 DenselySampledSpectrum
            if (spectrum != nullptr) {
                for (int lambda = lambda_min; lambda <= lambda_max; ++lambda) {
                    int index = lambda - lambda_min;
                    values[index] = spectrum->operator()(lambda);
                }
            }
        }

        double operator()(double lambda) const override {
            int offset = std::round(lambda) - lambda_min;
            // 对于超过波长范围的波长，直接返回0
            if (offset < 0 || offset >= values.size()) return 0;
            return values[offset];
        }

        double GetMaxValue() const override {
            if (values.empty()) return 0;
            return *std::max_element(values.begin(), values.end());
        }

    private:
        int lambda_min; // 波长范围
        int lambda_max;
        std::vector<double> values;
    };


    // 分段光谱
    // 用一系列 (lambda_i, v_i) 数据点表示的光谱，落在 lambda_i 和 lambda_{i + 1} 之间的波长所对应的光谱分布值通过 v_i 和 v_{i + 1} 的线性插值产生
    class PiecewiseLinearSpectrum : Spectrum {
    public:
        PiecewiseLinearSpectrum(std::span<const double> lambdas, std::span<const double> values)
            : lambdas(lambdas.begin(), lambdas.end()), values(values.begin(), values.end()) {
            assert(this->lambdas.size() == this->values.size());
            // 确保波长按升序排列
            for (size_t i = 1; i < this->lambdas.size(); ++i) {
                assert(this->lambdas[i - 1] < this->lambdas[i]);
            }
        }

        double operator()(double lambda) const override {
            if (lambdas.empty() || lambda < lambdas.front() || lambda > lambdas.back()) {
                return 0;
            }
            // 寻找该 lambda 落在的区间
            int o = FindInterval(lambdas.size(), [&](int i) {return lambdas[i] <= lambda; });
            // 线性插值
            double t = (lambda - lambdas[o]) / (lambdas[o + 1] - lambdas[o]);
            return Lerp(t, values[o], values[o + 1]);
        }

        double GetMaxValue() const override {
            if (values.empty()) return 0;
            return *std::max_element(values.begin(), values.end());
        }

    private:
        std::vector<double> lambdas;
        std::vector<double> values;
    };


}
