#pragma once

#include "def.hpp"
#include <vector>

namespace engine {
namespace math {
	struct Vec3 {
		float x;
		float y;
		float z;
	} PACKED_STRUCT;

	struct Mat4 {
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;
	} PACKED_STRUCT;

	namespace soa {
		struct Vec3 {
			std::vector<float> x;
			std::vector<float> y;
			std::vector<float> z;

			inline void push_back(float x = 0.0f, float y = 0.0f, float z = 0.0f);
			inline void push_back(math::Vec3 vec);

			inline math::Vec3 get(std::size_t index) const;
			inline void set(std::size_t index, math::Vec3 vec);
		} PACKED_STRUCT;
	}
}
}

// Inline implementation.
namespace engine {
namespace math {
	namespace soa {
		void Vec3::push_back(float _x, float _y, float _z)
		{
			x.emplace_back(_x);
			y.emplace_back(_y);
			z.emplace_back(_z);
		}

		void Vec3::push_back(math::Vec3 vec)
		{
			x.emplace_back(vec.x);
			y.emplace_back(vec.y);
			z.emplace_back(vec.z);
		}

		math::Vec3 Vec3::get(std::size_t index) const
		{
			return math::Vec3{ x[index], y[index], z[index] };
		}

		void Vec3::set(std::size_t index, math::Vec3 vec)
		{
			x[index] = vec.x;
			y[index] = vec.y;
			z[index] = vec.z;
		}
	} // soa.
} // math.
} // engine.
