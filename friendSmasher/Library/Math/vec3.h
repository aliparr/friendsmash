#ifndef FRIENDSMASHER_VEC3
#define FRIENDSMASHER_VEC3
#if _MSC_VER >= 1200
#pragma once
#endif

namespace FriendSmasher
{
namespace Math
{
	class vec3
	{
	public:
		vec3(void) {};
		vec3(const float broadcast) : x(broadcast), y(broadcast), z(broadcast) { };
		vec3(const float xin, const float yin, const float zin) : x(xin), y(yin), z(zin) {} ;
		~vec3(void) {};

		// Element wise
		vec3 operator +  (const vec3 &rhs) const;
		vec3 operator -  (const vec3 &rhs) const;
		vec3 operator *  (const vec3 &rhs) const; 
		vec3 operator /  (const vec3 &rhs) const;

		vec3 operator *  (const float &rhs) const; 
		vec3 operator /  (const float &rhs) const;

		vec3 operator += (const vec3 &rhs);
		vec3 operator -= (const vec3 &rhs);

		bool operator == (const vec3 &rhs) const;
		bool operator != (const vec3 &rhs) const;

		vec3 operator -() const;

		static vec3 allzero;
		static vec3 allone;

		union
		{
			struct  
			{
				float x, y, z;
			};
			float f[3];
		};
	};
}
}

#endif // namespace FriendSmasher_VEC3