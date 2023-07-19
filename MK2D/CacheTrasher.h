#pragma once
#include "Component.h"
#include <chrono>

namespace dae
{
    class CacheTrasher
    {
    public:
        CacheTrasher() {};

        template <typename T>
        std::vector<float> MeasureMultiplication(std::vector<T>& objects, int scale, int nrMeasurements, int stepSize, int maxStep)
        {
            const int nrSteps = static_cast<int>(std::log(maxStep) / std::log(stepSize)) + 1;
            std::vector<std::vector<float>> measurements(nrMeasurements, std::vector<float>(nrSteps));

            for (int j = 0; j < nrMeasurements; ++j)
            {
                int idx{ 0 };

                for (int step = 1; step <= maxStep; step *= stepSize)
                {
                    auto start = std::chrono::high_resolution_clock::now();

                    for (int i = 0; i < static_cast<int>(objects.size()); i += step)
                    {
                        objects[i] *= scale;
                    }

                    auto end = std::chrono::high_resolution_clock::now();
                    auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

                    measurements[j][idx] = static_cast<float>(elapsedTime);

                    ++idx;
                }
            }

            return GetAverageOverTime(measurements, nrSteps);
        }
     

    private:

        std::vector<float> GetAverageOverTime(const std::vector<std::vector<float>>& measurements, int nrSteps)
        {
            std::vector<float> avgTimes(nrSteps);

            for (int i = 0; i < nrSteps; ++i)
            {
                float sum = 0;
                for (int j = 0; j < static_cast<int>(measurements.size()); ++j)
                {
                    sum += measurements[j][i];
                }
                avgTimes[i] = sum / measurements.size();
            }

            return avgTimes;
        }
    };
}
