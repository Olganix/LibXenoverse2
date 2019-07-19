

#include <iomanip>

namespace LibXenoverse
{

/*-------------------------------------------------------------------------------\
|                             to_string_with_precision                           |
\-------------------------------------------------------------------------------*/
template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out << std::setprecision(n) << a_value;
	return out.str();
}



/*-------------------------------------------------------------------------------\
|                             ESKBone				                             |
\-------------------------------------------------------------------------------*/
ESKBone::ESKBone(string name)
{
	this->name = name;
	
	parent_index = 65535;
	child_index = 65535;
	sibling_index = 65535;
	index_4 = 0;
	
	unk_extraInfo_0 = unk_extraInfo_1 = unk_extraInfo_3 = 0;
	unk_extraInfo_2 = 0xffff;

	//do the iddentity matrix by default for transform_matrix, to not used a strange matrix with random values.
	transform_matrix[0] = 1; transform_matrix[1] = 0; transform_matrix[2] = 0; transform_matrix[3] = 0;
	transform_matrix[4] = 0; transform_matrix[5] = 1; transform_matrix[6] = 0; transform_matrix[7] = 0;
	transform_matrix[8] = 0; transform_matrix[9] = 0; transform_matrix[10] = 1; transform_matrix[11] = 0;
	transform_matrix[12] = 0; transform_matrix[13] = 0; transform_matrix[14] = 0; transform_matrix[15] = 1;

	skinning_matrix[0] = 0; skinning_matrix[1] = 0; skinning_matrix[2] = 0; skinning_matrix[3] = 1;		//pos
	skinning_matrix[4] = 0; skinning_matrix[5] = 0; skinning_matrix[6] = 0; skinning_matrix[7] = 1;		//Orientation quaternion x y z w
	skinning_matrix[8] = 1; skinning_matrix[9] = 1; skinning_matrix[10] = 1; skinning_matrix[11] = 1;	//scale

	this->haveTransformMatrix = false;
	mVisible = true;
}
/*-------------------------------------------------------------------------------\
|                             ESKBone				                             |
\-------------------------------------------------------------------------------*/
ESKBone::ESKBone(ESKBone *eskBone)
{
	this->name = eskBone->name;
	this->mListFilenameOrigin = eskBone->mListFilenameOrigin;
	this->parent_index = eskBone->parent_index;
	this->child_index = eskBone->child_index;
	this->sibling_index = eskBone->sibling_index;
	this->index_4 = eskBone->index_4;
	unk_extraInfo_0 = unk_extraInfo_1 = unk_extraInfo_3 = 0;
	unk_extraInfo_2 = 0xffff;
	memcpy(&this->transform_matrix, &eskBone->transform_matrix, 16 * sizeof(float));
	memcpy(&this->skinning_matrix, &eskBone->skinning_matrix, 12 * sizeof(float));
	this->haveTransformMatrix = eskBone->haveTransformMatrix;
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::readName(File *file)
{
	string filename = nameFromFilename(file->getPath());
	mListFilenameOrigin.push_back(filename);
		
	file->readString(&name);

	//LOG_DEBUG("Bone - %s\n", name.c_str());
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::writeName(File *file)
{
	file->writeString(&name);
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::readIndices(File *file)
{
	file->readInt16E(&parent_index);
	file->readInt16E(&child_index);
	file->readInt16E(&sibling_index);
	file->readInt16E(&index_4);
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::writeIndices(File *file)
{
	file->writeInt16E(&parent_index);
	file->writeInt16E(&child_index);
	file->writeInt16E(&sibling_index);
	file->writeInt16E(&index_4);
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::readMatrix(File *file)
{
	this->haveTransformMatrix = true;
		
	for (size_t i = 0; i < 16; i++)
		file->readFloat32E(&transform_matrix[i]);
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::writeMatrix(File *file)
{
	for (size_t i = 0; i < 16; i++)
		file->writeFloat32E(&transform_matrix[i]);
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::readSkinningMatrix(File *file)
{
	for (size_t i = 0; i < 12; i++)
		file->readFloat32E(&skinning_matrix[i]);
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::writeSkinningMatrix(File *file)
{
	for (size_t i = 0; i < 12; i++)
		file->writeFloat32E(&skinning_matrix[i]);
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::mergeMatrix(ESKBone *eskBone)
{
	if ((this->haveTransformMatrix) || (!eskBone->haveTransformMatrix))				//update transform matrix only if it isn't allready define
		return;

	memcpy(&this->transform_matrix, &eskBone->transform_matrix, 16 * sizeof(float));
	this->haveTransformMatrix = true;
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::mergeFilenameOrigin(ESKBone *eskBone)
{
	std::vector<string>	&list = eskBone->getFilenameOriginList();
	size_t nbElement = list.size();
	for (size_t i = 0; i < nbElement; i++)
		mListFilenameOrigin.push_back(list.at(i));
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::test_calculTransformMatrixFromSkinningMatrix(std::vector<ESKBone *> &listBones, bool recursive)
{
		
	//1st test: calcul TransformMatrix From SkinningMatrix and compare with the original on esk. => still have precision probleme on the Thumb bones
	double resultTransformMatrix[16];
		
	double skinning_matrix_a[12];						//special tranformation from observation between skinningMatrix and transformMatrix
	skinning_matrix_a[0] = skinning_matrix[0];
	skinning_matrix_a[1] = skinning_matrix[1];
	skinning_matrix_a[2] = skinning_matrix[2];
	skinning_matrix_a[3] = skinning_matrix[3];
	skinning_matrix_a[4] = skinning_matrix[4];
	skinning_matrix_a[5] = skinning_matrix[5];
	skinning_matrix_a[6] = skinning_matrix[6];
	skinning_matrix_a[7] = skinning_matrix[7];
	skinning_matrix_a[8] = skinning_matrix[8];
	skinning_matrix_a[9] = skinning_matrix[9];
	skinning_matrix_a[10] = skinning_matrix[10];
	skinning_matrix_a[11] = skinning_matrix[11];

	makeTransform4x4(&skinning_matrix_a[0], &resultTransformMatrix[0]);

	printf("************ Bone : %s\nRelative information: : \n%s\n=> result makeTransform4x4 : \n", this->getName().c_str(), this->getSkinningMatrixDebug().c_str());
		
	transpose4x4(&resultTransformMatrix[0]); //in Ogre, position is in last colone, but in Esk, position is on the last row, so we will use Transpose transformation.
	displayMatrix4x4(&resultTransformMatrix[0]);

	double tmpTransformMatrix[16];
	inverse4x4(&resultTransformMatrix[0], &tmpTransformMatrix[0]);
	for (size_t i = 0; i < 16; i++)
		resultTransformMatrix[i] = tmpTransformMatrix[i];


	//take car of parent matrix (and parent , and parent, ...) to have absolute transformation.
	if ((this->parent_index) && (this->parent_index<listBones.size()))
	{
		ESKBone *parent = listBones.at(this->parent_index);
		if (recursive)
			parent->calculTransformMatrixFromSkinningMatrix(listBones, recursive);

		double tmpTransformMatrix_b[16];
		for (size_t i = 0; i < 16; i++)
			tmpTransformMatrix_b[i] = parent->transform_matrix[i];

		printf("Parent (%s) Relative information: : \n%s\n=> tranform : \n", parent->getName().c_str(), parent->getSkinningMatrixDebug().c_str());
		displayMatrix4x4(&tmpTransformMatrix_b[0]);

		printf("result makeTransform4x4 inversed: \n");
		displayMatrix4x4(&tmpTransformMatrix[0]);

		concatenate4x4(&tmpTransformMatrix_b[0], &tmpTransformMatrix[0], &resultTransformMatrix[0]);
	}


	//this->haveTransformMatrix = true;
	bool alreadyMakeTheCompareson = false;
	float compare_resultTransformMatrix[16];
	for (size_t i = 0; i < 16; i++)
	{
		compare_resultTransformMatrix[i] = (float)resultTransformMatrix[i];

		if (abs(compare_resultTransformMatrix[i] - transform_matrix[i]) > 0.000001)
		{
			float test_a = (float)resultTransformMatrix[i];
			float test_b = transform_matrix[i];

			if (!alreadyMakeTheCompareson)
			{
				alreadyMakeTheCompareson = true;

				printf("there is some differences between real transform matrix: \n%s\nand result :\n", this->getTransformMatrixDebug().c_str() );
				displayMatrix4x4(&resultTransformMatrix[0]);
			}
		}
	}

		
	//2nd test, we make the inverse, to see if we come back on skinning matrix. => not one is good, it's a failer.
	double skinning_matrix_b[12];
		
		
	if ((this->parent_index) && (this->parent_index<listBones.size()))
	{
		ESKBone *parent = listBones.at(this->parent_index);
		if (recursive)
			parent->calculTransformMatrixFromSkinningMatrix(listBones, recursive);

		double tmpTransformMatrix[16];
		double tmpTransformMatrix2[16];
		double tmpTransformMatrix_b[16];
		for (size_t i = 0; i < 16; i++)
			tmpTransformMatrix_b[i] = parent->transform_matrix[i];

		inverse4x4(&tmpTransformMatrix_b[0], &tmpTransformMatrix2[0]);				//inverse of parent transformation

		concatenate4x4(&tmpTransformMatrix2[0], &resultTransformMatrix[0], &tmpTransformMatrix[0]);

		inverse4x4(&tmpTransformMatrix[0], &resultTransformMatrix[0]);
	}


	transpose4x4(&resultTransformMatrix[0]);		//come back to Ogre matrice way
	decomposition4x4(&resultTransformMatrix[0], &skinning_matrix_b[0]);

	float compare_skinning_matrix_b[12];

	for (size_t i = 0; i < 12; i++)
	{
		compare_skinning_matrix_b[i] = (float)skinning_matrix_b[i];
			
		/*
		//test to compare precision problem.
		if (abs(compare_skinning_matrix_b[i] - skinning_matrix[i]) > 0.000001)
			assert(false);
		*/
	}




	//3nd test, we go from transformMatrix to build skining matrix (we have to look if precision problems on generated transformMatrix from skining amtrix could be ignored.)
	for (size_t i = 0; i < 16; i++)
		resultTransformMatrix[i] = this->transform_matrix[i];

	if ((this->parent_index) && (this->parent_index<listBones.size()))
	{
		ESKBone *parent = listBones.at(this->parent_index);
		if (recursive)
			parent->calculTransformMatrixFromSkinningMatrix(listBones, recursive);

		double tmpTransformMatrix[16];
		double tmpTransformMatrix2[16];
		double tmpTransformMatrix_b[16];
		for (size_t i = 0; i < 16; i++)
			tmpTransformMatrix_b[i] = parent->transform_matrix[i];

		inverse4x4(&tmpTransformMatrix_b[0], &tmpTransformMatrix2[0]);				//inverse of parent transformation

		concatenate4x4(&tmpTransformMatrix2[0], &resultTransformMatrix[0], &tmpTransformMatrix[0]);

		inverse4x4(&tmpTransformMatrix[0], &resultTransformMatrix[0]);
	}


	transpose4x4(&resultTransformMatrix[0]);		//come back to Ogre matrice way
	decomposition4x4(&resultTransformMatrix[0], &skinning_matrix_b[0]);

	for (size_t i = 0; i < 12; i++)
	{
		compare_skinning_matrix_b[i] = (float)skinning_matrix_b[i];

		/*
		//test to compare precision problem.
		if (abs(compare_skinning_matrix_b[i] - skinning_matrix[i]) > 0.000001)
			assert(false)
		*/
	}
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::calculTransformMatrixFromSkinningMatrix(std::vector<ESKBone *> &listBones, bool recursive)
{
	double resultTransformMatrix[16];

	double skinning_matrix_a[12];						//special tranformation from observation between skinningMatrix and transformMatrix
	skinning_matrix_a[0] = skinning_matrix[0];
	skinning_matrix_a[1] = skinning_matrix[1];
	skinning_matrix_a[2] = skinning_matrix[2];
	skinning_matrix_a[3] = skinning_matrix[3];
	skinning_matrix_a[4] = skinning_matrix[4];
	skinning_matrix_a[5] = skinning_matrix[5];
	skinning_matrix_a[6] = skinning_matrix[6];
	skinning_matrix_a[7] = skinning_matrix[7];
	skinning_matrix_a[8] = skinning_matrix[8];
	skinning_matrix_a[9] = skinning_matrix[9];
	skinning_matrix_a[10] = skinning_matrix[10];
	skinning_matrix_a[11] = skinning_matrix[11];

	makeTransform4x4(&skinning_matrix_a[0], &resultTransformMatrix[0]);
	transpose4x4(&resultTransformMatrix[0]); //in Ogre, position is in last colone, but in Esk, position is on the last row, so we will use Transpose transformation.

	double tmpTransformMatrix[16];
	inverse4x4(&resultTransformMatrix[0], &tmpTransformMatrix[0]);
	for (size_t i = 0; i < 16; i++)
		resultTransformMatrix[i] = tmpTransformMatrix[i];

	//take car of parent matrix (and parent , and parent, ...) to have absolute transformation.
	if ((this->parent_index) && (this->parent_index<listBones.size()))
	{
		ESKBone *parent = listBones.at(this->parent_index);
		if (recursive)
			parent->calculTransformMatrixFromSkinningMatrix(listBones, recursive);

		double tmpTransformMatrix_b[16];
		for (size_t i = 0; i < 16; i++)
			tmpTransformMatrix_b[i] = parent->transform_matrix[i];

		concatenate4x4(&tmpTransformMatrix_b[0], &tmpTransformMatrix[0], &resultTransformMatrix[0]);
	}


	for (size_t i = 0; i < 16; i++)
		transform_matrix[i] = (float)(resultTransformMatrix[i]);
	this->haveTransformMatrix = true;
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::calculSkinningMatrixFromTransformMatrix(std::vector<ESKBone *> &listBones, bool recursive)
{
	double resultTransformMatrix[16];
	double tmpTransformMatrix[16];

	for (size_t i = 0; i < 16; i++)
	{
		resultTransformMatrix[i] = this->transform_matrix[i];
		tmpTransformMatrix[i] = this->transform_matrix[i];
	}



	if ((this->parent_index) && (this->parent_index<listBones.size()))
	{
		ESKBone *parent = listBones.at(this->parent_index);
		if (recursive)
			parent->calculTransformMatrixFromSkinningMatrix(listBones, recursive);

			
		double tmpTransformMatrix2[16];
		double tmpTransformMatrix_b[16];
		for (size_t i = 0; i < 16; i++)
			tmpTransformMatrix_b[i] = parent->transform_matrix[i];

		inverse4x4(&tmpTransformMatrix_b[0], &tmpTransformMatrix2[0]);				//inverse of parent transformation
		concatenate4x4(&tmpTransformMatrix2[0], &resultTransformMatrix[0], &tmpTransformMatrix[0]);	
	}

	inverse4x4(&tmpTransformMatrix[0], &resultTransformMatrix[0]);

	transpose4x4(&resultTransformMatrix[0]);		//come back to Ogre matrice way
	double skinning_matrix_b[12];						//special tranformation from observation between skinningMatrix and transformMatrix
	decomposition4x4(&resultTransformMatrix[0], &skinning_matrix_b[0]);

	for (size_t i = 0; i < 12; i++)
		skinning_matrix[i] = (float)skinning_matrix_b[i];
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
string ESKBone::getSkinningMatrixDebug(void)
{
	return ("\"Pos\" : [" + to_string_with_precision(skinning_matrix[0], 12) + ",\t" + to_string_with_precision(skinning_matrix[1], 12) + ",\t" + to_string_with_precision(skinning_matrix[2], 12) + ",\t" + to_string_with_precision(skinning_matrix[3], 12) + "],\n\"Quaternion\": [" + to_string_with_precision(skinning_matrix[4], 12) + ",\t" + to_string_with_precision(skinning_matrix[5], 12) + ",\t" + to_string_with_precision(skinning_matrix[6], 12) + ",\t" + to_string_with_precision(skinning_matrix[7], 12) + "],\n\"Scale\": [" + to_string_with_precision(skinning_matrix[8], 12) + ",\t" + to_string_with_precision(skinning_matrix[9], 12) + ",\t" + to_string_with_precision(skinning_matrix[10], 12) + ",\t" + to_string_with_precision(skinning_matrix[11], 12) + "]\n");
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
string ESKBone::getTransformMatrixDebug(void)
{
	return ("[" + to_string_with_precision(transform_matrix[0], 12) + ",\t" + to_string_with_precision(transform_matrix[1], 12) + ",\t" + to_string_with_precision(transform_matrix[2], 12) + ",\t" + to_string_with_precision(transform_matrix[3], 12) + ",\n" + to_string_with_precision(transform_matrix[4], 12) + ",\t" + to_string_with_precision(transform_matrix[5], 12) + ",\t" + to_string_with_precision(transform_matrix[6], 12) + ",\t" + to_string_with_precision(transform_matrix[7], 12) + ",\n" + to_string_with_precision(transform_matrix[8], 12) + ",\t" + to_string_with_precision(transform_matrix[9], 12) + ",\t" + to_string_with_precision(transform_matrix[10], 12) + ",\t" + to_string_with_precision(transform_matrix[11], 12) + ",\n" + to_string_with_precision(transform_matrix[12], 12) + ",\t" + to_string_with_precision(transform_matrix[13], 12) + ",\t" + to_string_with_precision(transform_matrix[14], 12) + ",\t" + to_string_with_precision(transform_matrix[15], 12) + "]\n");
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::displayMatrix4x4(double* transformMatrix)
{
	printf(("[" + std::to_string(transformMatrix[0]) + ",\t" + std::to_string(transformMatrix[1]) + ",\t" + std::to_string(transformMatrix[2]) + ",\t" + std::to_string(transformMatrix[3]) + "]\n[" + std::to_string(transformMatrix[4]) + ",\t" + std::to_string(transformMatrix[5]) + ",\t" + std::to_string(transformMatrix[6]) + ",\t" + std::to_string(transformMatrix[7]) + "]\n[" + std::to_string(transformMatrix[8]) + ",\t" + std::to_string(transformMatrix[9]) + ",\t" + std::to_string(transformMatrix[10]) + ",\t" + std::to_string(transformMatrix[11]) + "]\n[" + std::to_string(transformMatrix[12]) + ",\t" + std::to_string(transformMatrix[13]) + ",\t" + std::to_string(transformMatrix[14]) + ",\t" + std::to_string(transformMatrix[15]) + "]\n").c_str());
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::transpose4x4(double* transformMatrix)
{
	double m00 = transformMatrix[0], m01 = transformMatrix[1], m02 = transformMatrix[2], m03 = transformMatrix[3];
	double m10 = transformMatrix[4], m11 = transformMatrix[5], m12 = transformMatrix[6], m13 = transformMatrix[7];
	double m20 = transformMatrix[8], m21 = transformMatrix[9], m22 = transformMatrix[10], m23 = transformMatrix[11];
	double m30 = transformMatrix[12], m31 = transformMatrix[13], m32 = transformMatrix[14], m33 = transformMatrix[15];
		
	transformMatrix[0] = m00;
	transformMatrix[1] = m10;
	transformMatrix[2] = m20;
	transformMatrix[3] = m30;

	transformMatrix[4] = m01;
	transformMatrix[5] = m11;
	transformMatrix[6] = m21;
	transformMatrix[7] = m31;

	transformMatrix[8] = m02;
	transformMatrix[9] = m12;
	transformMatrix[10] = m22;
	transformMatrix[11] = m32;

	transformMatrix[12] = m03;
	transformMatrix[13] = m13;
	transformMatrix[14] = m23;
	transformMatrix[15] = m33;
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::makeTransform4x4(double* posOrientScaleMatrix, double* resultTransformMatrix)	//posOrientScaleMatrix is 3x4, orient is a quaternion informations, resultTransformMatrix is 4x4
{
	// Ordering:
	//    1. Scale
	//    2. Rotate
	//    3. Translate

	//Matrix3 rot3x3;
	//orientation.ToRotationMatrix(rot3x3);
	double fTx = posOrientScaleMatrix[4] + posOrientScaleMatrix[4];		// x + x
	double fTy = posOrientScaleMatrix[5] + posOrientScaleMatrix[5];		//y + y
	double fTz = posOrientScaleMatrix[6] + posOrientScaleMatrix[6];		//z + z
	double fTwx = fTx* posOrientScaleMatrix[7];		// * w
	double fTwy = fTy* posOrientScaleMatrix[7];
	double fTwz = fTz* posOrientScaleMatrix[7];
	double fTxx = fTx* posOrientScaleMatrix[4];		// * x
	double fTxy = fTy* posOrientScaleMatrix[4];
	double fTxz = fTz* posOrientScaleMatrix[4];
	double fTyy = fTy* posOrientScaleMatrix[5];		// * y
	double fTyz = fTz* posOrientScaleMatrix[5];
	double fTzz = fTz* posOrientScaleMatrix[6];		// * z
	double rot3x3_00 = 1.0f - (fTyy + fTzz);
	double rot3x3_01 = fTxy - fTwz;
	double rot3x3_02 = fTxz + fTwy;
	double rot3x3_10 = fTxy + fTwz;
	double rot3x3_11 = 1.0f - (fTxx + fTzz);
	double rot3x3_12 = fTyz - fTwx;
	double rot3x3_20 = fTxz - fTwy;
	double rot3x3_21 = fTyz + fTwx;
	double rot3x3_22 = 1.0f - (fTxx + fTyy);



	// Set up final matrix with scale, rotation and translation
	resultTransformMatrix[0] = posOrientScaleMatrix[8] * rot3x3_00;	//m00 = scale.x * 
	resultTransformMatrix[1] = posOrientScaleMatrix[9] * rot3x3_01;	//m01 = scale.y *
	resultTransformMatrix[2] = posOrientScaleMatrix[10] * rot3x3_02;	//m02 = scale.z * 
	resultTransformMatrix[3] = posOrientScaleMatrix[0];					//m03 = pos.x
		
	resultTransformMatrix[4] = posOrientScaleMatrix[8] * rot3x3_10;
	resultTransformMatrix[5] = posOrientScaleMatrix[9] * rot3x3_11;
	resultTransformMatrix[6] = posOrientScaleMatrix[10] * rot3x3_12;
	resultTransformMatrix[7] = posOrientScaleMatrix[1];					//m13 = pos.y

	resultTransformMatrix[8] = posOrientScaleMatrix[8] * rot3x3_20;
	resultTransformMatrix[9] = posOrientScaleMatrix[9] * rot3x3_21;
	resultTransformMatrix[10] = posOrientScaleMatrix[10] * rot3x3_22;
	resultTransformMatrix[11] = posOrientScaleMatrix[2];				//m23 = pos.z

	// No projection term
	resultTransformMatrix[12] = 0;
	resultTransformMatrix[13] = 0;
	resultTransformMatrix[14] = 0;
	resultTransformMatrix[15] = posOrientScaleMatrix[3];				//m33 = pos.w
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::decomposition4x4(double* transformMatrix, double* resultPosOrientScaleMatrix)//posOrientScaleMatrix is 3x4, orient is a quaternion informations, TransformMatrix is 4x4
{
	double m00 = transformMatrix[0], m01 = transformMatrix[1], m02 = transformMatrix[2], m03 = transformMatrix[3];
	double m10 = transformMatrix[4], m11 = transformMatrix[5], m12 = transformMatrix[6], m13 = transformMatrix[7];
	double m20 = transformMatrix[8], m21 = transformMatrix[9], m22 = transformMatrix[10], m23 = transformMatrix[11];
	double m30 = transformMatrix[12], m31 = transformMatrix[13], m32 = transformMatrix[14], m33 = transformMatrix[15];

	if (!((abs(m30) <= 0.000001) && (abs(m31) <= 0.000001) && (abs(m32) <= 0.000001) && (abs(m33 - 1) <= 0.000001)))		//assert(isAffine());
		return;

	//position
	resultPosOrientScaleMatrix[0] = transformMatrix[3];
	resultPosOrientScaleMatrix[1] = transformMatrix[7];
	resultPosOrientScaleMatrix[2] = transformMatrix[11];
	resultPosOrientScaleMatrix[3] = transformMatrix[15];


	//Matrix3 matQ;
	//Vector3 vecU;
	//m3x3.QDUDecomposition(matQ, scale, vecU);
		

		
	// Factor M = QR = QDU where Q is orthogonal, D is diagonal,
	// and U is upper triangular with ones on its diagonal.  Algorithm uses
	// Gram-Schmidt orthogonalization (the QR algorithm).
	//
	// If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
	//
	//   q0 = m0/|m0|
	//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
	//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
	//
	// where |V| indicates length of vector V and A*B indicates dot
	// product of vectors A and B.  The matrix R has entries
	//
	//   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
	//   r10 = 0      r11 = q1*m1  r12 = q1*m2
	//   r20 = 0      r21 = 0      r22 = q2*m2
	//
	// so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
	// u02 = r02/r00, and u12 = r12/r11.

	// Q = rotation
	// D = scaling
	// U = shear

	// D stores the three diagonal entries r00, r11, r22
	// U stores the entries U[0] = u01, U[1] = u02, U[2] = u12


	// build orthogonal matrix Q
	double fInvLength = 1.0 / sqrt( m00 * m00 + m10 * m10 + m20 * m20);
	double kQ_00 = m00 * fInvLength;
	double kQ_10 = m10 * fInvLength;
	double kQ_20 = m20 * fInvLength;

	double fDot = kQ_00 * m01 + kQ_10 * m11 + kQ_20 * m21;
	double kQ_01 = m01 - fDot * kQ_00;
	double kQ_11 = m11 - fDot * kQ_10;
	double kQ_21 = m21 - fDot * kQ_20;
	fInvLength = 1.0 / sqrt( kQ_01 * kQ_01 + kQ_11 * kQ_11 + kQ_21 * kQ_21);
	kQ_01 *= fInvLength;
	kQ_11 *= fInvLength;
	kQ_21 *= fInvLength;

	fDot = kQ_00 * m02 + kQ_10 * m12 + kQ_20 * m22;
	double kQ_02 = m02 - fDot*kQ_00;
	double kQ_12 = m12 - fDot*kQ_10;
	double kQ_22 = m22 - fDot*kQ_20;
	fDot = kQ_01 * m02 + kQ_11 * m12 + kQ_21 * m22;
	kQ_02 -= fDot*kQ_01;
	kQ_12 -= fDot*kQ_11;
	kQ_22 -= fDot*kQ_21;
	fInvLength = 1.0 / sqrt( kQ_02 * kQ_02 + kQ_12 * kQ_12 + kQ_22 * kQ_22);
	kQ_02 *= fInvLength;
	kQ_12 *= fInvLength;
	kQ_22 *= fInvLength;

	// guarantee that orthogonal matrix has determinant 1 (no reflections)
	double fDet = kQ_00 * kQ_11 * kQ_22 + kQ_01 * kQ_12 * kQ_20 + kQ_02 * kQ_10 * kQ_21 - kQ_02 * kQ_11 * kQ_20 - kQ_01 * kQ_10 * kQ_22 - kQ_00 * kQ_12 * kQ_21;

	if (fDet < 0.0)
	{
		kQ_00 = -kQ_00;
		kQ_01 = -kQ_01;
		kQ_02 = -kQ_02;
		kQ_10 = -kQ_10;
		kQ_11 = -kQ_11;
		kQ_12 = -kQ_12;
		kQ_20 = -kQ_20;
		kQ_21 = -kQ_21;
		kQ_22 = -kQ_22;
	}

	// build "right" matrix R
	double kR_00 = kQ_00 * m00 + kQ_10 * m10 + kQ_20 * m20;
	double kR_01 = kQ_00 * m01 + kQ_10 * m11 + kQ_20 * m21;
	double kR_11 = kQ_01 * m01 + kQ_11 * m11 + kQ_21 * m21;
	double kR_02 = kQ_00 * m02 + kQ_10 * m12 + kQ_20 * m22;
	double kR_12 = kQ_01 * m02 + kQ_11 * m12 + kQ_21 * m22;
	double kR_22 = kQ_02 * m02 + kQ_12 * m12 + kQ_22 * m22;

	// the scaling component
	double kD_0 = kR_00;
	double kD_1 = kR_11;
	double kD_2 = kR_22;

	// the shear component
	double fInvD0 = 1.0 / kD_0;
	double kU_0 = kR_01 * fInvD0;
	double kU_1 = kR_02 * fInvD0;
	double kU_2 = kR_12 / kD_1;



	resultPosOrientScaleMatrix[8] = kD_0;
	resultPosOrientScaleMatrix[9] = kD_1;
	resultPosOrientScaleMatrix[10] = kD_2;
	resultPosOrientScaleMatrix[11] = 1.0;




	//orientation = Quaternion(matQ);		//this->FromRotationMatrix(rot);
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternion Calculus and Fast Animation".

	double fTrace = kQ_00 + kQ_11 + kQ_22;
	double fRoot;

	if (fTrace > 0.0)
	{
		// |w| > 1/2, may as well choose w > 1/2
		fRoot = sqrt(fTrace + 1.0f);  // 2w
		resultPosOrientScaleMatrix[7] = 0.5f*fRoot;					//w
		fRoot = 0.5f / fRoot;  // 1/(4w)
		resultPosOrientScaleMatrix[4] = (kQ_21 - kQ_12)*fRoot;
		resultPosOrientScaleMatrix[5] = (kQ_02 - kQ_20)*fRoot;
		resultPosOrientScaleMatrix[6] = (kQ_10 - kQ_01)*fRoot;
	}
	else
	{
		/*
		// |w| <= 1/2
		static size_t s_iNext[3] = { 1, 2, 0 };
		size_t i = 0;
		if (kQ_11 > kQ_00)
			i = 1;
		if (kQ_22 > kQ_[i][i])
			i = 2;
		size_t j = s_iNext[i];
		size_t k = s_iNext[j];

		fRoot = 1.0 / sqrt(kQ_[i][i] - kQ_[j][j] - kQ_[k][k] + 1.0f);
		double* apkQuat[3] = { &x, &y, &z };
		*apkQuat[i] = 0.5f*fRoot;
		fRoot = 0.5f / fRoot;
		resultPosOrientScaleMatrix[4] = (kQ_[k][j] - kQ_[j][k])*fRoot;		//w
		*apkQuat[j] = (kQ_[j][i] + kQ_[i][j])*fRoot;
		*apkQuat[k] = (kQ_[k][i] + kQ_[i][k])*fRoot;
		*/

		// |w| <= 1/2
		static size_t s_iNext[3] = { 1, 2, 0 };

		if (kQ_11 > kQ_00)
		{
			if (kQ_22 > kQ_11)
			{
				//i = 2;
				//size_t j = 0;
				//size_t k = 1;

				fRoot = sqrt(kQ_22 - kQ_00 - kQ_11 + 1.0);
					
				resultPosOrientScaleMatrix[6] = 0.5f*fRoot;					//z
				fRoot = 0.5f / fRoot;
				resultPosOrientScaleMatrix[7] = (kQ_10 - kQ_01)*fRoot;		//w
				resultPosOrientScaleMatrix[4] = (kQ_02 + kQ_20)*fRoot;		//x
				resultPosOrientScaleMatrix[5] = (kQ_12 + kQ_21)*fRoot;		//y
			}
			else{
				//i = 1
				//size_t j = 2;
				//size_t k = 0;

				fRoot = sqrt(kQ_11 - kQ_22 - kQ_00 + 1.0);
				resultPosOrientScaleMatrix[5] = 0.5f*fRoot;							//y
				fRoot = 0.5f / fRoot;
				resultPosOrientScaleMatrix[7] = (kQ_02 - kQ_20)*fRoot;		//w
				resultPosOrientScaleMatrix[6] = (kQ_21 + kQ_12)*fRoot;		//z
				resultPosOrientScaleMatrix[4] = (kQ_01 + kQ_10)*fRoot;		//x

			}
		}
		else{

			if (kQ_22 > kQ_00)
			{
				//i = 2;
				//size_t j = 0;
				//size_t k = 1;

				fRoot = sqrt(kQ_22 - kQ_00 - kQ_11 + 1.0);

				resultPosOrientScaleMatrix[6] = 0.5f*fRoot;					//z
				fRoot = 0.5f / fRoot;
				resultPosOrientScaleMatrix[7] = (kQ_10 - kQ_01)*fRoot;		//w
				resultPosOrientScaleMatrix[4] = (kQ_02 + kQ_20)*fRoot;		//x
				resultPosOrientScaleMatrix[5] = (kQ_12 + kQ_21)*fRoot;		//y
			}
			else{
				//i = 0
				//size_t j = 1;
				//size_t k = 2;

				fRoot = sqrt(kQ_00 - kQ_11 - kQ_22 + 1.0f);

				resultPosOrientScaleMatrix[4] = 0.5f*fRoot;					//x
				fRoot = 0.5f / fRoot;
				resultPosOrientScaleMatrix[7] = (kQ_21 - kQ_12)*fRoot;		//w
				resultPosOrientScaleMatrix[5] = (kQ_10 + kQ_01)*fRoot;	//y
				resultPosOrientScaleMatrix[6] = (kQ_20 + kQ_02)*fRoot;	//z
			}

		}
	}

}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::inverse4x4(double* transformMatrix, double* inversedTransformMatrix)
{
	/*
	//from Ogre Matrix4x4
	m[0][0] = m00;	0
	m[0][1] = m01;	1
	m[0][2] = m02;	2
	m[0][3] = m03;	3
	m[1][0] = m10;	4
	m[1][1] = m11;	5
	m[1][2] = m12;	6
	m[1][3] = m13;	7
	m[2][0] = m20;	8
	m[2][1] = m21;	9
	m[2][2] = m22;	10
	m[2][3] = m23;	11
	m[3][0] = m30;	12
	m[3][1] = m31;	13
	m[3][2] = m32;	14
	m[3][3] = m33;	15
	*/

	double m00 = transformMatrix[0], m01 = transformMatrix[1], m02 = transformMatrix[2], m03 = transformMatrix[3];
	double m10 = transformMatrix[4], m11 = transformMatrix[5], m12 = transformMatrix[6], m13 = transformMatrix[7];
	double m20 = transformMatrix[8], m21 = transformMatrix[9], m22 = transformMatrix[10], m23 = transformMatrix[11];
	double m30 = transformMatrix[12], m31 = transformMatrix[13], m32 = transformMatrix[14], m33 = transformMatrix[15];

	double v0 = m20 * m31 - m21 * m30;
	double v1 = m20 * m32 - m22 * m30;
	double v2 = m20 * m33 - m23 * m30;
	double v3 = m21 * m32 - m22 * m31;
	double v4 = m21 * m33 - m23 * m31;
	double v5 = m22 * m33 - m23 * m32;

	double t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
	double t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
	double t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
	double t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

	double invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

	double d00 = t00 * invDet;
	double d10 = t10 * invDet;
	double d20 = t20 * invDet;
	double d30 = t30 * invDet;

	double d01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	double d11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	double d21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	double d31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m10 * m31 - m11 * m30;
	v1 = m10 * m32 - m12 * m30;
	v2 = m10 * m33 - m13 * m30;
	v3 = m11 * m32 - m12 * m31;
	v4 = m11 * m33 - m13 * m31;
	v5 = m12 * m33 - m13 * m32;

	double d02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	double d12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	double d22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	double d32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m21 * m10 - m20 * m11;
	v1 = m22 * m10 - m20 * m12;
	v2 = m23 * m10 - m20 * m13;
	v3 = m22 * m11 - m21 * m12;
	v4 = m23 * m11 - m21 * m13;
	v5 = m23 * m12 - m22 * m13;

	double d03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	double d13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	double d23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	double d33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;


	inversedTransformMatrix[0] = d00;
	inversedTransformMatrix[1] = d01;
	inversedTransformMatrix[2] = d02;
	inversedTransformMatrix[3] = d03;
	inversedTransformMatrix[4] = d10;
	inversedTransformMatrix[5] = d11;
	inversedTransformMatrix[6] = d12;
	inversedTransformMatrix[7] = d13;
	inversedTransformMatrix[8] = d20;
	inversedTransformMatrix[9] = d21;
	inversedTransformMatrix[10] = d22;
	inversedTransformMatrix[11] = d23;
	inversedTransformMatrix[12] = d30;
	inversedTransformMatrix[13] = d31;
	inversedTransformMatrix[14] = d32;
	inversedTransformMatrix[15] = d33;
}
/*-------------------------------------------------------------------------------\
|                             EMD					                             |
\-------------------------------------------------------------------------------*/
void ESKBone::concatenate4x4(double* transformMatrix_left, double* transformMatrix_right, double* resultTransformMatrix)
{
	/*
	//from Ogre Matrix4x4, equivalent at the multiplication of matrix.
	m[0][0] = m00;	0
	m[0][1] = m01;	1
	m[0][2] = m02;	2
	m[0][3] = m03;	3
	m[1][0] = m10;	4
	m[1][1] = m11;	5
	m[1][2] = m12;	6
	m[1][3] = m13;	7
	m[2][0] = m20;	8
	m[2][1] = m21;	9
	m[2][2] = m22;	10
	m[2][3] = m23;	11
	m[3][0] = m30;	12
	m[3][1] = m31;	13
	m[3][2] = m32;	14
	m[3][3] = m33;	15
	*/

	double m00 = transformMatrix_left[0], m01 = transformMatrix_left[1], m02 = transformMatrix_left[2], m03 = transformMatrix_left[3];
	double m10 = transformMatrix_left[4], m11 = transformMatrix_left[5], m12 = transformMatrix_left[6], m13 = transformMatrix_left[7];
	double m20 = transformMatrix_left[8], m21 = transformMatrix_left[9], m22 = transformMatrix_left[10], m23 = transformMatrix_left[11];
	double m30 = transformMatrix_left[12], m31 = transformMatrix_left[13], m32 = transformMatrix_left[14], m33 = transformMatrix_left[15];
		
	double m00_b = transformMatrix_right[0], m01_b = transformMatrix_right[1], m02_b = transformMatrix_right[2], m03_b = transformMatrix_right[3];
	double m10_b = transformMatrix_right[4], m11_b = transformMatrix_right[5], m12_b = transformMatrix_right[6], m13_b = transformMatrix_right[7];
	double m20_b = transformMatrix_right[8], m21_b = transformMatrix_right[9], m22_b = transformMatrix_right[10], m23_b = transformMatrix_right[11];
	double m30_b = transformMatrix_right[12], m31_b = transformMatrix_right[13], m32_b = transformMatrix_right[14], m33_b = transformMatrix_right[15];


	double m00_r = 0.0, m01_r = 0.0, m02_r = 0.0, m03_r = 0.0;
	double m10_r = 0.0, m11_r = 0.0, m12_r = 0.0, m13_r = 0.0;
	double m20_r = 0.0, m21_r = 0.0, m22_r = 0.0, m23_r = 0.0;
	double m30_r = 0.0, m31_r = 0.0, m32_r = 0.0, m33_r = 0.0;

		

	m00_r = m00 * m00_b + m01 * m10_b + m02 * m20_b + m03 * m30_b;
	m01_r = m00 * m01_b + m01 * m11_b + m02 * m21_b + m03 * m31_b;
	m02_r = m00 * m02_b + m01 * m12_b + m02 * m22_b + m03 * m32_b;
	m03_r = m00 * m03_b + m01 * m13_b + m02 * m23_b + m03 * m33_b;

	m10_r = m10 * m00_b + m11 * m10_b + m12 * m20_b + m13 * m30_b;
	m11_r = m10 * m01_b + m11 * m11_b + m12 * m21_b + m13 * m31_b;
	m12_r = m10 * m02_b + m11 * m12_b + m12 * m22_b + m13 * m32_b;
	m13_r = m10 * m03_b + m11 * m13_b + m12 * m23_b + m13 * m33_b;

	m20_r = m20 * m00_b + m21 * m10_b + m22 * m20_b + m23 * m30_b;
	m21_r = m20 * m01_b + m21 * m11_b + m22 * m21_b + m23 * m31_b;
	m22_r = m20 * m02_b + m21 * m12_b + m22 * m22_b + m23 * m32_b;
	m23_r = m20 * m03_b + m21 * m13_b + m22 * m23_b + m23 * m33_b;

	m30_r = m30 * m00_b + m31 * m10_b + m32 * m20_b + m33 * m30_b;
	m31_r = m30 * m01_b + m31 * m11_b + m32 * m21_b + m33 * m31_b;
	m32_r = m30 * m02_b + m31 * m12_b + m32 * m22_b + m33 * m32_b;
	m33_r = m30 * m03_b + m31 * m13_b + m32 * m23_b + m33 * m33_b;

	resultTransformMatrix[0] = m00_r;
	resultTransformMatrix[1] = m01_r;
	resultTransformMatrix[2] = m02_r;
	resultTransformMatrix[3] = m03_r;
	resultTransformMatrix[4] = m10_r;
	resultTransformMatrix[5] = m11_r;
	resultTransformMatrix[6] = m12_r;
	resultTransformMatrix[7] = m13_r;
	resultTransformMatrix[8] = m20_r;
	resultTransformMatrix[9] = m21_r;
	resultTransformMatrix[10] = m22_r;
	resultTransformMatrix[11] = m23_r;
	resultTransformMatrix[12] = m30_r;
	resultTransformMatrix[13] = m31_r;
	resultTransformMatrix[14] = m32_r;
	resultTransformMatrix[15] = m33_r;
}



}