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
        namespace internal
        {
            static std::random_device rd;
            static std::mt19937 gen { rd() };
        };

        template<typename T, typename U>
        static T genNum(T base, U variance) noexcept
        {
            std::uniform_real_distribution<U> dis(-variance, variance);
            return base + dis(internal::gen);
        }

        template<typename T>
        static tvec3<T> genVec(tvec3<T> base, T variance) noexcept
        {
            std::uniform_real_distribution<T> dis(-variance, variance);

            return tvec3<T> (
                base.x + dis(internal::gen),
                base.y + dis(internal::gen),
                base.z + dis(internal::gen)
            );
        }

        template<typename T>
        static tvec4<T> genVec(tvec4<T> base, T variance) noexcept
        {
            std::uniform_real_distribution<T> dis(-variance, variance);

            return tvec4<T> (
                base.x + dis(internal::gen),
                base.y + dis(internal::gen),
                base.z + dis(internal::gen),
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
                base.x + disX(internal::gen),
                base.y + disY(internal::gen),
                base.z + disZ(internal::gen)
            );
        }

        template<typename T>
        static tvec4<T> genVec(tvec4<T> base, tvec3<T> variance) noexcept
        {
            std::uniform_real_distribution<T> disX(-variance.x, variance.x);
            std::uniform_real_distribution<T> disY(-variance.y, variance.y);
            std::uniform_real_distribution<T> disZ(-variance.z, variance.z);

            return tvec4<T> (
                base.x + disX(internal::gen),
                base.y + disY(internal::gen),
                base.z + disZ(internal::gen),
                base.w
            );
        }
    };
} // namespace glb

#endif