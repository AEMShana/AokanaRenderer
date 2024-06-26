#pragma once

#include "vec.h"

namespace Aokana {
    class Perlin {
    public:
        Perlin() {
            ranvec = new Vector3[point_count];
            for (int i = 0;i < point_count;++i)
                ranvec[i] = Vector3::RandomUnitVector();
            perm_x = perlin_generate_perm();
            perm_y = perlin_generate_perm();
            perm_z = perlin_generate_perm();
        }

        ~Perlin() {
            delete[] ranvec;
            delete[] perm_x;
            delete[] perm_y;
            delete[] perm_z;
        }

        double noise(const Point3& p) const {
            auto u = p.x - std::floor(p.x);
            auto v = p.y - std::floor(p.y);
            auto w = p.z - std::floor(p.z);

            auto i = static_cast<int>(std::floor(p.x));
            auto j = static_cast<int>(std::floor(p.y));
            auto k = static_cast<int>(std::floor(p.z));
            Vector3 c[2][2][2];

            for (int di = 0;di < 2;++di) {
                for (int dj = 0;dj < 2;++dj) {
                    for (int dk = 0;dk < 2;++dk) {
                        c[di][dj][dk] = ranvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];
                    }
                }
            }
            return trilinear_interp(c, u, v, w);
        }

        double turb(const Point3& p, int depth = 7) const {
            auto accum = 0.0;
            auto temp_p = p;
            auto weight = 1.0;

            for (int i = 0;i < depth;++i) {
                accum += weight * noise(temp_p);
                weight *= 0.5;
                temp_p *= 2;
            }

            return fabs(accum);
        }

    private:
        static const int point_count = 256;
        Vector3* ranvec;
        int* perm_x;
        int* perm_y;
        int* perm_z;

        static int* perlin_generate_perm() {
            auto p = new int[point_count];
            for (int i = 0;i < point_count;++i)
                p[i] = i;
            permute(p, point_count);
            return p;
        }

        static void permute(int* p, int n) {
            for (int i = n - 1;i > 0;--i) {
                int target = RandomIntInRange(0, i);
                int temp = p[i];
                p[i] = p[target];
                p[target] = temp;
            }
        }

        static double trilinear_interp(Vector3 c[2][2][2], double u, double v, double w) {
            auto uu = u * u * (3 - 2 * u);
            auto vv = v * v * (3 - 2 * v);
            auto ww = w * w * (3 - 2 * w);
            auto accum = 0.0;

            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    for (int k = 0; k < 2; k++) {
                        Vector3 weight_v(u - i, v - j, w - k);
                        accum += (i * uu + (1 - i) * (1 - uu))
                            * (j * vv + (1 - j) * (1 - vv))
                            * (k * ww + (1 - k) * (1 - ww))
                            * Dot(c[i][j][k], weight_v);
                    }

            return accum;
        }
    };

}