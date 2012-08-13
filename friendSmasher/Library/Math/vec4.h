#ifndef FRIENDSMASHER_VEC4
#define FRIENDSMASHER_VEC4

namespace FriendSmasher
{
namespace Math
{
	class vec4
	{
	public:
		vec4(void) : x(0.f), y(0.f), z(0.f), w(0.f) {} ;
		vec4(const float broadcast) : x(broadcast), y(broadcast), z(broadcast) {};
		vec4(const float xin, const float yin, const float zin, const float win) : x(xin), y(yin), z(zin), w(win) {} ;

		~vec4(void) {};

		vec4 operator +  (const vec4 &rhs) const;
		vec4 operator -  (const vec4 &rhs) const;
		vec4 operator *  (const vec4 &rhs) const; 
		vec4 operator /  (const vec4 &rhs) const;
		
		vec4 operator *  (const float &rhs) const; 
		vec4 operator /  (const float &rhs) const;
		
		vec4 operator += (const vec4 &rhs);
		vec4 operator -= (const vec4 &rhs);

		bool operator == (const vec4 &rhs) const;
		bool operator != (const vec4 &rhs) const;

		
		static vec4 allzero;
		static vec4 allone;

		union
		{
			struct  
			{
				float x, y, z, w;
			};
			float f[4];
		};
	};

}
}

#endif // namespace FriendSmasher_VEC4