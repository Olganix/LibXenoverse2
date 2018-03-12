#ifndef LIBXENOVERSE_MATH_H_INCLUDED
#define LIBXENOVERSE_MATH_H_INCLUDED

#define LIBXENOVERSE_AABB_MAX_START  FLT_MAX 
#define LIBXENOVERSE_AABB_MIN_END    -FLT_MAX

namespace LibXenoverse
{

	/*-------------------------------------------------------------------------------\
	|                             AABB					                             |
	\-------------------------------------------------------------------------------*/
	class AABB
	{
	public:
		float start_x;
		float start_y;
		float start_z;
		float end_x;
		float end_y;
		float end_z;

		AABB(void) { reset(); }

		void	reset(void);
		void	addPoint(float point_x, float point_y, float point_z);
		void	merge(const AABB& aabb);
		bool	intersects(const AABB& aabb);
		//AABB	half(int axis, int side);

		void	expandX(float v) { start_x -= v; end_x += v; }
		void	expandY(float v) { start_y -= v; end_y += v; }
		void	expandZ(float v) { start_z -= v; end_z += v; }
		void	expand(float v) { expandX(v); expandY(v); expandZ(v); }

		float	centerX(void) { return (start_x + end_x) / 2.0f; }
		float	centerY(void) { return (start_y + end_y) / 2.0f; }
		float	centerZ(void) { return (start_z + end_z) / 2.0f; }
		float	sizeX(void) { return end_x - start_x; }
		float	sizeY(void) { return end_y - start_y; }
		float	sizeZ(void) { return end_z - start_z; }
		float	size(void) { return sizeX() * sizeY() * sizeZ(); }
		float	sizeMax(void) { return max(sizeX(), max(sizeY(), sizeZ())); }



		AABB&	operator=(const AABB& aabb);

	};

	/*-------------------------------------------------------------------------------\
	|                             Math					                             |
	\-------------------------------------------------------------------------------*/
	float lerp(float src, float dest, float factor);
	double quadraticBezier(double factor, double startPoint, double controlPoint, double endPoint);
	double cubicBezier(double factor, double startPoint, double controlPoint1, double controlPoint2, double endPoint);







#ifdef LIBXENOVERSE_FBX_SUPPORT

	//math with Fbx class

	FbxDouble3 crossProduct(const FbxDouble3& vec, const FbxDouble3& rkVector);
	FbxDouble3 quatMulVec3(FbxVector4 quat, FbxDouble3 &v);								//from Ogre Vector3 Quaternion::operator* (const Vector3& v) const
	FbxVector4 quatMulQuat(const FbxVector4 &quat, const FbxVector4 &rkQ);				//from Ogre Quaternion Quaternion::operator* (const Quaternion& rkQ) const
	FbxVector4 fromAngleAxis(const double& rfAngle, const FbxDouble3& rkAxis);			//from Ogre Quaternion::FromAngleAxis
	FbxDouble3 giveAngleOrientationForThisOrientationTaitBryan(FbxVector4 orient);
	FbxDouble3 giveAngleOrientationForThisOrientationTaitBryan_XYZ(FbxVector4 orient);
	void quadToRotationMatrix(FbxVector4 orient, FbxDouble3 &m0, FbxDouble3 &m1, FbxDouble3 &m2);		//from Ogre 
	bool matrixToEulerAnglesZYX(FbxDouble3 m0, FbxDouble3 m1, FbxDouble3 m2, FbxDouble3 &YPR_angles);	//from Ogre 

#endif


};

#endif