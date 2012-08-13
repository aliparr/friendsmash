#include "math_lib.h"

namespace FriendSmasher
{
namespace Math
{
	vec4 vec4::allzero	= vec4(0.0f,0.0f,0.f,0.f);
	vec4 vec4::allone	= vec4(1.0f,1.0f,1.f,1.f);
	
	vec4 vec4::operator + (const vec4 &rhs) const
	{
		return vec4(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z, this->w + rhs.w);
	}

	vec4 vec4::operator - (const vec4 &rhs) const
	{
		return vec4(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z, this->w - rhs.w);
	}
	
	vec4 vec4::operator * (const vec4 &rhs) const
	{
		return vec4(this->x * rhs.x, this->y * rhs.y, this->z * rhs.z, this->w * rhs.w);
	}
		
	
	vec4 vec4::operator / (const vec4 &rhs) const
	{
		return vec4(this->x / rhs.x, this->y / rhs.y, this->z / rhs.z, this->w / rhs.w);
	}
	
	vec4 vec4::operator * (const float &rhs) const
	{
		vec4 vResult;
		vec4Scale(vResult, *this, rhs);
		return vResult;
	}
	
	vec4 vec4::operator / (const float &rhs) const
	{
		vec4 vResult;
		vec4Scale(vResult, *this, 1.f/rhs);
		return vResult;
	}
	
	

	vec4 vec4::operator += (const vec4 &rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		this->w += rhs.w;
		return *this;
	}

	vec4 vec4::operator -= (const vec4 &rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->w -= rhs.w;
		this->z -= rhs.z;
		return *this;
	}

	bool vec4::operator == (const vec4 &rhs) const
	{
		return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w);
	}


	bool vec4::operator != (const vec4 &rhs) const
	{
		return !(this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w);
	}
}

}