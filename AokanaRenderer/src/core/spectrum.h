#pragma once
#include <vector>
#include <cmath>
#include <span>
#include <array>
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

    /// <summary>
    /// 计算黑体辐射
    /// </summary>
    /// <param name="lambda">波长, 以纳米为单位</param>
    /// <param name="temperature">温度, 以开尔文为单位</param>
    /// <returns></returns>
    inline double Blackbody(double lambda, double temperature) {
        if (temperature < 0) {
            return 0;
        }
        const double c = 299792458.f;       // 光速
        const double h = 6.62606957e-34f;   // 普朗克常数
        const double kb = 1.3806488e-23f;   // 玻尔兹曼常数
        double l = lambda * 1e-9; // 上面3个常数中用到了以米为单位, 要先将波长转化为米来计算
        double le = (2 * h * c * c) / (std::pow(l, 5) * (std::exp((h * c) / (l * kb * temperature)) - 1)); // 普朗克定律
        return le;
    }

    class BlackbodySpectrum {
    public:
        BlackbodySpectrum(double temperature) : temperature{ temperature } {
            double lambda_max = 2.8977721e-3f / temperature; // 维恩位移定律, 计算当前温度下, 最大光谱分布值所对应的波长
            normalization_factor = 1 / Blackbody(lambda_max * 1e9, temperature);
            // 因为随着黑体的温度升高, 黑体辐射出的能量增长的非常快, 我们使用当前温度下最大的光谱分布值作为归一化系数
        }

        double operator()(double lambda) const {
            return Blackbody(lambda, temperature) * normalization_factor;
        }

    private:
        double temperature;
        double normalization_factor;
    };

    static constexpr int N_SPECTRUM_SAMPLES = 4; // 对于每个光谱采样4个样本

    class SampledSpectrum {
    public:
        SampledSpectrum() { values.fill(0); }
        explicit SampledSpectrum(double c) { values.fill(c); }
        SampledSpectrum(std::span<const double> v) {
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
                values[i] = v[i];
            }
        }
        double operator[](int i) const { return values[i]; }
        double& operator[](int i) { return values[i]; }
        bool operator==(const SampledSpectrum& s) const { return values == s.values; }
        bool operator!=(const SampledSpectrum& s) const { return values != s.values; }

        bool HasNaNs() const {
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
                if (std::isnan(values[i])) {
                    return true;
                }
            }
            return false;
        }

        // 判断是否全零
        explicit operator bool() const {
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
                if (values[i] != 0) {
                    return true;
                }
            }
            return false;
        }

        SampledSpectrum operator+(const SampledSpectrum& s) const {
            SampledSpectrum ret = *this;
            return ret += s;
        }

        SampledSpectrum& operator+=(const SampledSpectrum& s) {
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
                values[i] += s.values[i];
            }
            return *this;
        }

        SampledSpectrum operator-(const SampledSpectrum& s) const {
            SampledSpectrum ret = *this;
            return ret -= s;
        }

        SampledSpectrum& operator-=(const SampledSpectrum& s) {
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
                values[i] -= s.values[i];
            }
            return *this;
        }

        friend SampledSpectrum operator-(double a, const SampledSpectrum& s) {
            //DCHECK(!IsNaN(a));
            assert(std::isnan(a) == false);
            SampledSpectrum ret;
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
                ret.values[i] = a - s.values[i];
            }
            return ret;
        }

        SampledSpectrum operator*(const SampledSpectrum& s) const {
            SampledSpectrum ret = *this;
            return ret *= s;
        }

        SampledSpectrum& operator*=(const SampledSpectrum& s) {
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
                values[i] *= s.values[i];
            }
            return *this;
        }

        SampledSpectrum operator*(double a) const {
            //DCHECK(!IsNaN(a));
            assert(std::isnan(a) == false);
            SampledSpectrum ret = *this;
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
                ret.values[i] *= a;
            }
            return ret;
        }

        SampledSpectrum& operator*=(double a) {
            //DCHECK(!IsNaN(a));
            assert(std::isnan(a) == false);
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
                values[i] *= a;
            }
            return *this;
        }

        friend SampledSpectrum operator*(double a, const SampledSpectrum& s) { return s * a; }

        SampledSpectrum operator/(const SampledSpectrum& s) const {
            SampledSpectrum ret = *this;
            return ret /= s;
        }

        SampledSpectrum& operator/=(const SampledSpectrum& s) {
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
                //DCHECK_NE(0, s.values[i]);
                assert(s.values[i] != 0);
                values[i] /= s.values[i];
            }
            return *this;
        }

        SampledSpectrum operator/(double a) const {
            SampledSpectrum ret = *this;
            return ret /= a;
        }

        SampledSpectrum& operator/=(double a) {
            //DCHECK_NE(a, 0);
            //DCHECK(!IsNaN(a));
            assert(a != 0);
            assert(std::isnan(a) == false);
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
                values[i] /= a;
            }
            return *this;
        }

        SampledSpectrum operator-() const {
            SampledSpectrum ret;
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
                ret.values[i] = -values[i];
            }
            return ret;
        }

        SampledSpectrum SafeDiv(SampledSpectrum a, SampledSpectrum b) {
            SampledSpectrum r{};
            for (int i = 0; i < N_SPECTRUM_SAMPLES; ++i) {
                r[i] = (b[i] != 0) ? a[i] / b[i] : 0.;
            }
            return r;
        }

        double MinComponentValue() const {
            double m = values[0];
            for (int i = 1; i < N_SPECTRUM_SAMPLES; ++i) {
                m = std::min(m, values[i]);
            }
            return m;
        }

        double MaxComponentValue() const {
            double m = values[0];
            for (int i = 1; i < N_SPECTRUM_SAMPLES; ++i) {
                m = std::max(m, values[i]);
            }
            return m;
        }

        double Average() const {
            double sum = values[0];
            for (int i = 1; i < N_SPECTRUM_SAMPLES; ++i) {
                sum += values[i];
            }
            return sum / N_SPECTRUM_SAMPLES;
        }

    private:
        std::array<double, N_SPECTRUM_SAMPLES> values;
    };

}
