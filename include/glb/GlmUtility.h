#pragma once
#ifndef GLMUTILITY_H
#define GLMUTILITY_H

#include <random>

#include <glm/glm.hpp>
using namespace glm;

namespace glb
{
    namespace glm_util
    {
        namespace
        {
            static std::random_device rd;
            static std::mt19937 gen { rd() };
        } // anonymous namespace

        template<typename T, typename U>
        static T genNum(T base, U variance) noexcept
        {
            std::uniform_real_distribution<U> dis(-variance, variance);
            return base + dis(gen);
        }

        template<typename T>
        static tvec3<T> genVec(tvec3<T> base, T variance) noexcept
        {
            std::uniform_real_distribution<T> dis(-variance, variance);

            return tvec3<T> (
                base.x + dis(gen),
                base.y + dis(gen),
                base.z + dis(gen)
            );
        }

        template<typename T>
        static tvec4<T> genVec(tvec4<T> base, T variance) noexcept
        {
            std::uniform_real_distribution<T> dis(-variance, variance);

            return tvec4<T> (
                base.x + dis(gen),
                base.y + dis(gen),
                base.z + dis(gen),
                base.w
            );
        }

        template<typename T>
        static tvec3<T> genVec(tvec3<T> base, tvec3<T> variance) noexcept
        {
            std::uniform_real_distribution<T> disX(-variance.x, variance.x);
            std::uniform_real_distribution<T> disY(-variance.y, variance.y);
            std::uniform_real_distribution<T> disZ(-variance.z, variance.z);

            return tvec3<T> (
                base.x + disX(gen),
                base.y + disY(gen),
                base.z + disZ(gen)
            );
        }

        template<typename T>
        static tvec4<T> genVec(tvec4<T> base, tvec3<T> variance) noexcept
        {
            std::uniform_real_distribution<T> disX(-variance.x, variance.x);
            std::uniform_real_distribution<T> disY(-variance.y, variance.y);
            std::uniform_real_distribution<T> disZ(-variance.z, variance.z);

            return tvec4<T> (
                base.x + disX(gen),
                base.y + disY(gen),
                base.z + disZ(gen),
                base.w
            );
        }
    } // namespace glm_util
} // namespace glb

#endif
