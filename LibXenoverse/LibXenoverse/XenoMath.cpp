namespace LibXenoverse
{


/*-------------------------------------------------------------------------------\
|                             reset					                             |
\-------------------------------------------------------------------------------*/
void AABB::reset(void)
{
	start_x = LIBXENOVERSE_AABB_MAX_START;
	start_y = LIBXENOVERSE_AABB_MAX_START;
	start_z = LIBXENOVERSE_AABB_MAX_START;

	end_x = LIBXENOVERSE_AABB_MIN_END;
	end_y = LIBXENOVERSE_AABB_MIN_END;
	end_z = LIBXENOVERSE_AABB_MIN_END;
}
/*-------------------------------------------------------------------------------\
|                             addPoint				                             |
\-------------------------------------------------------------------------------*/
void AABB::addPoint(float point_x, float point_y, float point_z)
{
	if (point_x < start_x) start_x = point_x;
	if (point_y < start_y) start_y = point_y;
	if (point_z < start_z) start_z = point_z;

	if (point_x > end_x) end_x = point_x;
	if (point_y > end_y) end_y = point_y;
	if (point_z > end_z) end_z = point_z;
}
/*-------------------------------------------------------------------------------\
|                             merge					                             |
\-------------------------------------------------------------------------------*/
void AABB::merge(const AABB& aabb)
{
	if (aabb.start_x < start_x) start_x = aabb.start_x;
	if (aabb.start_y < start_y) start_y = aabb.start_y;
	if (aabb.start_z < start_z) start_z = aabb.start_z;

	if (aabb.end_x > end_x) end_x = aabb.end_x;
	if (aabb.end_y > end_y) end_y = aabb.end_y;
	if (aabb.end_z > end_z) end_z = aabb.end_z;
}
/*-------------------------------------------------------------------------------\
|                             intersects			                             |
\-------------------------------------------------------------------------------*/
bool AABB::intersects(const AABB& aabb)
{
	if ((start_x > aabb.end_x) || (end_x < aabb.start_x))
		return false;
	if ((start_y > aabb.end_y) || (end_y < aabb.start_y))
		return false;
	if ((start_z > aabb.end_z) || (end_z < aabb.start_z))
		return false;

	return true;					//with previous cases, we assure there is a intersect
}
/*-------------------------------------------------------------------------------\
|                             operator=				                             |
\-------------------------------------------------------------------------------*/
inline AABB &AABB::operator=(const AABB& aabb)
{
	start_x = aabb.start_x;
	start_y = aabb.start_y;
	start_z = aabb.start_z;
	end_x = aabb.end_x;
	end_y = aabb.end_y;
	end_z = aabb.end_z;

	return *this;
}
/*-------------------------------------------------------------------------------\
|                             lerp					                             |
\-------------------------------------------------------------------------------*/
float lerp(float src, float dest, float factor)
{
	return src + factor * (dest - src);
}



/*-------------------------------------------------------------------------------\
|                             quadraticBezier		                             |
\-------------------------------------------------------------------------------*/
//
//interpolations with one control point. 
//from http ://altdevblog.com/2011/05/17/understanding-the-fourier-transform_mov/
//
double quadraticBezier(double factor, double startPoint, double controlPoint, double endPoint)
{
	return ( pow( 1.0 - factor, 2.0) * startPoint + 2.0 * (1.0 - factor) * factor * controlPoint + pow(factor, 2.0) * endPoint);
}
/*-------------------------------------------------------------------------------\
|                             cubicBezier										 |
\-------------------------------------------------------------------------------*/
double cubicBezier(double factor, double startPoint, double controlPoint1, double controlPoint2, double endPoint)
{
	return pow(1.0 - factor, 3.0) * startPoint + 3.0 * pow(1.0 - factor, 2.0) * factor * controlPoint1 + 3.0 * (1.0 - factor) * pow(factor, 2.0) * controlPoint2 + pow(factor, 3.0) * endPoint;
}






#ifdef LIBXENOVERSE_FBX_SUPPORT


/*-------------------------------------------------------------------------------\
|                             crossProduct			                             |
\-------------------------------------------------------------------------------*/
FbxDouble3 crossProduct(const FbxDouble3& vec, const FbxDouble3& rkVector)
{
	return FbxDouble3(
		vec[1] * rkVector[2] - vec[2] * rkVector[1],
		vec[2] * rkVector[0] - vec[0] * rkVector[2],
		vec[0] * rkVector[1] - vec[1] * rkVector[0]);
}
/*-------------------------------------------------------------------------------\
|                quatMulVec3									                 |
\-------------------------------------------------------------------------------*/
FbxDouble3 quatMulVec3(FbxVector4 quat, FbxDouble3 &v)						//from Ogre Vector3 Quaternion::operator* (const Vector3& v) const
{
	// nVidia SDK implementation
	FbxDouble3 uv, uuv;
	FbxDouble3 qvec(quat[0], quat[1], quat[2]);
	uv = crossProduct(qvec, v);
	uuv = crossProduct(qvec, uv);
	uv[0] = uv[0] * (2.0 * quat[3]);
	uv[1] = uv[1] * (2.0 * quat[3]);
	uv[2] = uv[2] * (2.0 * quat[3]);
	uuv[0] = uuv[0] * 2.0;
	uuv[1] = uuv[1] * 2.0;
	uuv[2] = uuv[2] * 2.0;

	return FbxDouble3(v[0] + uv[0] + uuv[0], v[1] + uv[1] + uuv[1], v[2] + uv[2] + uuv[2]);
}
/*-------------------------------------------------------------------------------\
|                quatMulQuat									                 |
\-------------------------------------------------------------------------------*/
FbxVector4 quatMulQuat(const FbxVector4 &quat, const FbxVector4 &rkQ)		//from Ogre Quaternion Quaternion::operator* (const Quaternion& rkQ) const
{
	// NOTE:  Multiplication is not generally commutative, so in most cases p*q != q*p.

	return FbxVector4
	(
		quat[3] * rkQ[0] + quat[0] * rkQ[3] + quat[1] * rkQ[2] - quat[2] * rkQ[1],
		quat[3] * rkQ[1] + quat[1] * rkQ[3] + quat[2] * rkQ[0] - quat[0] * rkQ[2],
		quat[3] * rkQ[2] + quat[2] * rkQ[3] + quat[0] * rkQ[1] - quat[1] * rkQ[0],
		quat[3] * rkQ[3] - quat[0] * rkQ[0] - quat[1] * rkQ[1] - quat[2] * rkQ[2]
	);
}
/*-------------------------------------------------------------------------------\
|                fromAngleAxis									                 |
\-------------------------------------------------------------------------------*/
FbxVector4 fromAngleAxis(const double& rfAngle, const FbxDouble3& rkAxis)			//from Ogre Quaternion::FromAngleAxis
{
	// assert:  axis[] is unit length
	//
	// The quaternion representing the rotation is
	//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

	double fHalfAngle(0.5*rfAngle);
	double fSin = FbxSin(fHalfAngle * FBXSDK_PI_DIV_180);
	return FbxVector4(fSin*rkAxis[0], fSin*rkAxis[1], fSin*rkAxis[2], FbxCos(fHalfAngle * FBXSDK_PI_DIV_180));			//X,y,z,w
}
/*-------------------------------------------------------------------------------\
|                giveAngleOrientationForThisOrientationTaitBryan                 |
\-------------------------------------------------------------------------------*/
FbxDouble3 giveAngleOrientationForThisOrientationTaitBryan(FbxVector4 orient)
{
	FbxDouble3 q(0, 0, 0);

	FbxDouble3 dir = quatMulVec3(orient, FbxDouble3(1, 0, 0));

	//1) calcul yaw
	FbxVector2 vectproj(dir[0], -dir[2]);		//projection of the result on (O,x,-z) plane
	if (vectproj.Length() > 0.000001)			//if undefined => by defaut 0;
	{
		vectproj.Normalize();

		q[0] = FbxACos(vectproj[0]) * FBXSDK_180_DIV_PI;
		if (vectproj[1] < 0)
			q[0] = -q[0];
	}

	//2) calcul pitch
	FbxVector4 rotationInv_Yrot = fromAngleAxis(-q[0], FbxDouble3(0, 1, 0));
	FbxDouble3 dir_tmp = quatMulVec3(quatMulQuat(rotationInv_Yrot, orient), FbxDouble3(1, 0, 0));		//we cancel yaw rotation, the point must be into (O,x,y) plane
	
	//just in case (ex Tapion (TPO) sword)
	double tmp = sqrt(dir_tmp[0] * dir_tmp[0] + dir_tmp[1] * dir_tmp[1] + dir_tmp[2] * dir_tmp[2]);
	dir_tmp[0] /= tmp; dir_tmp[1] /= tmp; dir_tmp[2] /= tmp;

	q[1] = FbxACos(dir_tmp[0]) * FBXSDK_180_DIV_PI;
	if (dir_tmp[1] < 0)
		q[1] = -q[1];




	//3) calcul roll
	FbxVector4 rotationInv_Zrot = fromAngleAxis(-q[1], FbxDouble3(0, 0, 1));
	dir_tmp = quatMulVec3(quatMulQuat(rotationInv_Zrot, quatMulQuat(rotationInv_Yrot, orient)), FbxDouble3(0, 0, 1));		//we cancel the yaw and pitch rotations, the point Vector3::UNIT_Y, after rotation, must be into (O,x,z) plane.
	
	//just in case (ex Tapion (TPO) sword)
	tmp = sqrt(dir_tmp[0] * dir_tmp[0] + dir_tmp[1] * dir_tmp[1] + dir_tmp[2] * dir_tmp[2]);
	dir_tmp[0] /= tmp; dir_tmp[1] /= tmp; dir_tmp[2] /= tmp;

	q[2] = FbxACos(dir_tmp[2]) * FBXSDK_180_DIV_PI;
	if (dir_tmp[1] > 0)		// the direct direction is from Oy to Oz
		q[2] = -q[2];

	return q;
}


/*-------------------------------------------------------------------------------\
|                giveAngleOrientationForThisOrientationTaitBryan_XYZ             |
\-------------------------------------------------------------------------------*
FbxDouble3 giveAngleOrientationForThisOrientationTaitBryan_XYZ(FbxVector4 orient)				//same version, but with this order of rotation, the yaw is on the diqs display by pitch rotation.
{
	FbxDouble3 q(0, 0, 0);

	FbxDouble3 dir = quatMulVec3(orient, FbxDouble3(1, 0, 0));

	//1) calcul pitch. for that, we ahve to find the rotation axis for yaw, witch is the normal of the disq of pitch rotation.
	if ((abs(dir[0]) < 0.000001) && (abs(abs(dir[1]) - 1.0) < 0.000001) && (abs(dir[2]) < 0.000001))
	{
		q[1] = ((dir[1] < 0) ? -90.0 : 90.0);
	}else {
		FbxDouble3 normalDisq = crossProduct( crossProduct(dir, FbxDouble3(0,1,0)), dir);
		q[1] = FbxACos(normalDisq[1]) * FBXSDK_180_DIV_PI;				//scalar produc with (0,1,0);
		if (normalDisq[0] > 0)
			q[1] = -q[1];
	}



	//2) calcul yaw
	FbxVector4 rotationInv_Zrot = fromAngleAxis(-q[1], FbxDouble3(0, 0, 1));

	FbxDouble3 dir_tmp = quatMulVec3(quatMulQuat(rotationInv_Zrot, orient), FbxDouble3(1, 0, 0));		//we cancel pitch rotation, the point must be into (O,x,z) plane
	
	FbxVector2 vectproj(dir[0], -dir[2]);		//projection of result on (O,x,-z) plane
	if (vectproj.Length() > 0.000001)					//if undefined => by defaut 0;
	{
		vectproj.Normalize();

		q[0] = FbxACos(vectproj[0]) * FBXSDK_180_DIV_PI;
		if (vectproj[1] < 0)
			q[0] = -q[0];
	}


	//3) calcul roll
	FbxVector4 rotationInv_Yrot = fromAngleAxis(-q[0], FbxDouble3(0, 1, 0));
	dir_tmp = quatMulVec3(quatMulQuat(rotationInv_Yrot, quatMulQuat(rotationInv_Zrot, orient)), FbxDouble3(0, 0, 1));		//we cancel the pitch and yaw rotation, the point Vector3::UNIT_Y, after rotation, must be into (O,x,z) plane.
	q[2] = FbxACos(dir_tmp[2]) * FBXSDK_180_DIV_PI;
	if (dir_tmp[1] > 0)		// the direct direction is from Oy to Oz
		q[2] = -q[2];

	return q;
}






/*-------------------------------------------------------------------------------\
|                giveAngleOrientationForThisOrientationTaitBryan_XYZ             |
\-------------------------------------------------------------------------------*/
FbxDouble3 giveAngleOrientationForThisOrientationTaitBryan_XYZ(FbxVector4 orient)				//same version, but with this order of rotation, the yaw is on the diqs display by pitch rotation.
{
	//convert into a matrix3x3
	FbxDouble3 m0, m1, m2;
	quadToRotationMatrix(orient, m0, m1, m2);
	
	//convert matrix3x3 into EulerAngle
	FbxDouble3 q(0, 0, 0);
	matrixToEulerAnglesZYX(m0, m1, m2, q);
	
	return q;
}



/*-------------------------------------------------------------------------------\
|                quadToRotationMatrix											 |
\-------------------------------------------------------------------------------*/
void quadToRotationMatrix(FbxVector4 orient, FbxDouble3 &m0, FbxDouble3 &m1, FbxDouble3 &m2)
{
	//normalize quaternion as in https://www.andre-gaschler.com/rotationconverter/ , else we could have infinite + weird result on matrixToEulerAnglesZYX, because of float precision on quaternion.
	double a = sqrt(orient[0] * orient[0] + orient[1] * orient[1] + orient[2] * orient[2] + orient[3] * orient[3]);
	if (0 == a)
	{
		orient[0] = orient[1] = orient[2] = 0;
		orient[3] = 1;
	}else{
		a = 1.0 / a;
		orient[0] *= a;
		orient[1] *= a;
		orient[2] *= a;
		orient[3] *= a;
	}


	
	double fTx = orient[0] + orient[0];
	double fTy = orient[1] + orient[1];
	double fTz = orient[2] + orient[2];
	double fTwx = fTx * orient[3];
	double fTwy = fTy * orient[3];
	double fTwz = fTz * orient[3];
	double fTxx = fTx * orient[0];
	double fTxy = fTy * orient[0];
	double fTxz = fTz * orient[0];
	double fTyy = fTy * orient[1];
	double fTyz = fTz * orient[1];
	double fTzz = fTz * orient[2];

	m0[0] = 1.0 - (fTyy + fTzz);
	m0[1] = fTxy - fTwz;
	m0[2] = fTxz + fTwy;
	m1[0] = fTxy + fTwz;
	m1[1] = 1.0 - (fTxx + fTzz);
	m1[2] = fTyz - fTwx;
	m2[0] = fTxz - fTwy;
	m2[1] = fTyz + fTwx;
	m2[2] = 1.0 - (fTxx + fTyy);
}



bool matrixToEulerAnglesZYX(FbxDouble3 m0, FbxDouble3 m1, FbxDouble3 m2, FbxDouble3 &YPR_angles)
{
	// rot =  cy*cz           cz*sx*sy-cx*sz  cx*cz*sy+sx*sz
	//        cy*sz           cx*cz+sx*sy*sz -cz*sx+cx*sy*sz
	//       -sy              cy*sx           cx*cy

	for (size_t i = 0; i < 3; i++)		//few corrections, due to the float precision on quaternion.
	{
		if ((m0[i] < -1) && (abs(m0[i] - (-1)) < 0.000001))
			m0[i] = -1;
		if ((m0[i] > 1) && (abs(m0[i] - 1) < 0.000001))
			m0[i] = 1;

		if ((m1[i] < -1) && (abs(m1[i] - (-1)) < 0.000001))
			m1[i] = -1;
		if ((m1[i] > 1) && (abs(m1[i] - 1) < 0.000001))
			m1[i] = 1;
		
		if ((m2[i] < -1) && (abs(m2[i] - (-1)) < 0.000001))
			m2[i] = -1;
		if ((m2[i] > 1) && (abs(m2[i] - 1) < 0.000001))
			m2[i] = 1;
	}



	YPR_angles[1] = FbxASin(-m2[0] ) * FBXSDK_180_DIV_PI;
	if (YPR_angles[1] < 90.0)
	{
		if (YPR_angles[1] > -90.0)
		{
			YPR_angles[0] = FbxATan(m1[0], m0[0]) * FBXSDK_180_DIV_PI;
			YPR_angles[2] = FbxATan(m2[1], m2[2]) * FBXSDK_180_DIV_PI;
			return true;
		}else{
			// WARNING.  Not a unique solution.
			double fRmY = FbxATan(-m0[1], m0[2]) * FBXSDK_180_DIV_PI;
			YPR_angles[2] = 0.0;  // any angle works
			//YPR_angles[0] = YPR_angles[2] - fRmY;
			YPR_angles[0] = fRmY - YPR_angles[2];
			return false;
		}
	}else{
		// WARNING.  Not a unique solution.
		double fRpY = FbxATan(-m0[1], m0[2]);
		YPR_angles[2] = 0.0;  // any angle works
		YPR_angles[0] = fRpY - YPR_angles[2];
		return false;
	}
}



//-----------------------------------------------------------------------






#endif

};