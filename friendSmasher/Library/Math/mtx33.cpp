#include "mtx33.h"
#include "mtx33_op.h"

namespace FriendSmasher
{
namespace Math
{
	mtx33 mtx33::identity( 1.0f, 0.0f, 0.0f,
						   0.0f, 1.0f, 0.0f,
						   0.0f, 0.0f, 1.0f );

	mtx33::mtx33( const float aa, const float ab, const float ac, const float ba, const float bb, const float bc, const float ca, const float cb, const float cc )
	{
		SET_MATRIX_ROW(0, aa, ab, ac);
		SET_MATRIX_ROW(1, ba, bb, bc);
		SET_MATRIX_ROW(2, ca, cb, cc);
	}

}
}