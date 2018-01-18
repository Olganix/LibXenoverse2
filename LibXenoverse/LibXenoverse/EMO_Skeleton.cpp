#include <stdlib.h>

#include <stdexcept>
#include <algorithm>

#include "EMO_Skeleton.h"
//#include "debug.h"


namespace LibXenoverse
{

/*
 * Structure of skeleton:
 *
 * Skeleton Header
 * node_count * SkeletonNode
 * node_count * UnkSkeletonData
 * names_table
 * (pad to next 0x10 boundary if necessary)
 * node_count * matrix_data
 * IKData
 *
*/

const static std::vector<std::string> matrix_suffix =
{
    "_X",
    "_Y",
    "_Z",
    "_O"
};



void EMO_Bone::DecompileTransformationMatrix(TiXmlElement *root, const char *name, const float *matrix)
{
    std::vector<float> row;

    row.resize(4);

#ifdef FBX_SUPPORT

    FbxMatrix fbx_matrix(matrix[0], matrix[1], matrix[2], matrix[3],
                         matrix[4], matrix[5], matrix[6], matrix[7],
                         matrix[8], matrix[9], matrix[10], matrix[11],
                         matrix[12], matrix[13], matrix[14], matrix[15]);


    FbxVector4 translation, scaling, shearing;
    FbxQuaternion rotation;
    double sign;

    fbx_matrix.GetElements(translation, rotation, shearing, scaling, sign);

    std::string comment = "Translation: " + EMO_BaseFile::FloatToString((float)translation[0]) + ", " +
                          EMO_BaseFile::FloatToString((float)translation[1]) + ", " + EMO_BaseFile::FloatToString((float)translation[2]);
    EMO_BaseFile::WriteComment(root, comment);

    comment = "Rotation: " + EMO_BaseFile::FloatToString((float)rotation[0]) + ", " + EMO_BaseFile::FloatToString((float)rotation[1]) + ", " +
                EMO_BaseFile::FloatToString((float)rotation[2]) + ", " + EMO_BaseFile::FloatToString((float)rotation[3]);
    EMO_BaseFile::WriteComment(root, comment);

    comment = "Scaling: " + EMO_BaseFile::FloatToString((float)scaling[0]) + ", " +
            EMO_BaseFile::FloatToString((float)scaling[1]) + ", " + EMO_BaseFile::FloatToString((float)scaling[2]);
    EMO_BaseFile::WriteComment(root, comment);

    comment = "Shearing: " + EMO_BaseFile::FloatToString((float)shearing[0]) + ", " +
            EMO_BaseFile::FloatToString((float)shearing[1]) + ", " + EMO_BaseFile::FloatToString((float)shearing[2]);
    EMO_BaseFile::WriteComment(root, comment);

    comment = "Sign: " + EMO_BaseFile::FloatToString((float)sign);
    EMO_BaseFile::WriteComment(root, comment);


#endif

    for (int i = 0; i < 4; i++)
    {
        std::string row_name = name + matrix_suffix[i];

        row[0] = matrix[i];
        row[1] = matrix[i+4];
        row[2] = matrix[i+8];
        row[3] = matrix[i+12];

        EMO_BaseFile::WriteParamMultipleFloats(root, row_name.c_str(), row);
    }
}

int EMO_Bone::CompileTransformationMatrix(const TiXmlElement *root, const char *name, float *matrix, bool must_exist)
{
    std::vector<float> row;

    for (int i = 0; i < 4; i++)
    {
        std::string row_name = name + matrix_suffix[i];

        if (i == 0 && !must_exist)
        {
            if (!EMO_BaseFile::ReadParamMultipleFloats(root, row_name.c_str(), row))
                return -1;
        }
        else
        {
            if (!EMO_BaseFile::GetParamMultipleFloats(root, row_name.c_str(), row))
                return false;
        }

        if (row.size() != 4)
        {
            LOG_DEBUG("%s: Invalid size for \"%s\"\n", FUNCNAME, row_name.c_str());
            return false;
        }

        matrix[i] = row[0];
        matrix[i+4] = row[1];
        matrix[i+8] = row[2];
        matrix[i+12] = row[3];
    }

    return true;
}

void EMO_Bone::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("EMO_Bone");
    entry_root->SetAttribute("name", name);

    if (parent)
    {
        EMO_BaseFile::WriteParamString(entry_root, "PARENT", parent->name);
    }
    else
    {
        EMO_BaseFile::WriteParamString(entry_root, "PARENT", "NULL");
    }

    if (child)
    {
        EMO_BaseFile::WriteParamString(entry_root, "CHILD", child->name);
    }
    else
    {
        EMO_BaseFile::WriteParamString(entry_root, "CHILD", "NULL");
    }

    if (sibling)
    {
        EMO_BaseFile::WriteParamString(entry_root, "SIBLING", sibling->name);
    }
    else
    {
        EMO_BaseFile::WriteParamString(entry_root, "SIBLING", "NULL");
    }


	if (emgIndex != 0xFFFF)
		EMO_BaseFile::WriteParamUnsigned(entry_root, "EmgIndex", emgIndex);
	else
		EMO_BaseFile::WriteParamString(entry_root, "EmgIndex", "NULL");



    if (index_4 != 0xffff)
    {
        EMO_BaseFile::WriteParamUnsigned(entry_root, "index_4", index_4);
    }
    else
    {
        EMO_BaseFile::WriteParamString(entry_root, "index_4", "NULL");
    }

    DecompileTransformationMatrix(entry_root, "M1", matrix1);

    if (has_matrix2)
    {
       DecompileTransformationMatrix(entry_root, "M2", matrix2);
    }

    EMO_BaseFile::WriteParamMultipleUnsigned(entry_root, "SN_U_0A", std::vector<uint16_t>(sn_u0A, sn_u0A+3), true);

    if (has_unk)
    {
        EMO_BaseFile::WriteParamMultipleUnsigned(entry_root, "USD_U_00", std::vector<uint16_t>(usd_u00, usd_u00+4), true);
    }

    root->LinkEndChild(entry_root);
}

bool EMO_Bone::Compile(const TiXmlElement *root, EMO_Skeleton *skl)
{
    if (root->QueryStringAttribute("name", &name) != 0)
    {
        LOG_DEBUG("%s: Cannot get name of EMO_Bone.\n", FUNCNAME);
        return false;
    }

    if (skl)
    {
        std::string bone_name;

        if (!EMO_BaseFile::GetParamString(root, "PARENT", bone_name))
            return false;

        if (bone_name == "NULL")
        {
            parent = nullptr;
        }
        else
        {
            parent = skl->GetBone(bone_name);
            if (!parent)
            {
                LOG_DEBUG("%s: PARENT bone %s doesn't exist.\n", FUNCNAME, bone_name.c_str());
                return false;
            }
        }

        if (!EMO_BaseFile::GetParamString(root, "CHILD", bone_name))
            return false;

        if (bone_name == "NULL")
        {
            child = nullptr;
        }
        else
        {
            child = skl->GetBone(bone_name);
            if (!child)
            {
                LOG_DEBUG("%s: CHILD bone %s doesn't exist.\n", FUNCNAME, bone_name.c_str());
                return false;
            }
        }

        if (!EMO_BaseFile::GetParamString(root, "SIBLING", bone_name))
            return false;

        if (bone_name == "NULL")
        {
			sibling = nullptr;
        }
        else
        {
			sibling = skl->GetBone(bone_name);
            if (!sibling)
            {
                LOG_DEBUG("%s: SIBLING bone %s doesn't exist.\n", FUNCNAME, bone_name.c_str());
                return false;
            }
        }

		emgIndex = 0xffff;

        if (!EMO_BaseFile::GetParamString(root, "index_4", bone_name))
            return false;

        if (bone_name == "NULL")
        {
			index_4 = 0xffff;
        }
        else
        {
			uint32_t tmp;
			if (!EMO_BaseFile::GetParamUnsigned(root, "index_4", &tmp))
				return false;

			index_4 = tmp;
        }
    }

    if (!CompileTransformationMatrix(root, "M1", matrix1, true))
        return false;

    int troolean = CompileTransformationMatrix(root, "M2", matrix2, false);

    if (!troolean)
        return false;

    has_matrix2 = (troolean > 0);

    std::vector<uint16_t> sn_u0A;

    if (!EMO_BaseFile::GetParamMultipleUnsigned(root, "SN_U_0A", sn_u0A))
        return false;

    if (sn_u0A.size() != 3)
    {
        LOG_DEBUG("%s: Invalid size for \"SN_U_0A\"\n", FUNCNAME);
        return false;
    }

    memcpy(this->sn_u0A, sn_u0A.data(), sizeof(this->sn_u0A));

    std::vector<uint16_t> usd_u00;

    if (EMO_BaseFile::ReadParamMultipleUnsigned(root, "USD_U_00", usd_u00))
    {
        has_unk = true;

        if (usd_u00.size() != 4)
        {
            LOG_DEBUG("%s: Invalid size for \"USD_U_00\"\n", FUNCNAME);
            return false;
        }

        memcpy(this->usd_u00, usd_u00.data(), sizeof(this->usd_u00));
    }
    else
    {
        has_unk = false;
    }

    meta_original_offset = 0xFFFFFFFF;
    return true;
}




/*-------------------------------------------------------------------------------\
|                             readESKBone			                             |
\-------------------------------------------------------------------------------*/
void EMO_Bone::readESKBone(ESKBone* esk)
{
	name = esk->name;
	index_4 = esk->index_4;

	has_matrix2 = esk->haveTransformMatrix;

	if (has_matrix2)
	{
		for (size_t i = 0; i < 16; i++)
			matrix2[i] = esk->transform_matrix[i];
	}

	double skinning_matrix_a[12];									//special tranformation from observation between skinningMatrix and transformMatrix
	for (size_t i = 0; i < 12; i++)
		skinning_matrix_a[i] = esk->skinning_matrix[i];

	double resultTransformMatrix[16];
	ESKBone::makeTransform4x4(&skinning_matrix_a[0], &resultTransformMatrix[0]);
	ESKBone::transpose4x4(&resultTransformMatrix[0]);				//in Ogre, position is in last colone, but in Esk, position is on the last row, so we will use Transpose transformation.

	for (size_t i = 0; i < 16; i++)
		matrix1[i] = (float)(resultTransformMatrix[i]);
}
/*-------------------------------------------------------------------------------\
|                             writeESKBone			                             |
\-------------------------------------------------------------------------------*/
void EMO_Bone::writeESKBone(ESKBone* esk)
{
	esk->name = name;
	esk->index_4 = index_4;

	esk->haveTransformMatrix = has_matrix2;

	if (esk->haveTransformMatrix)
	{
		for (size_t i = 0; i < 16; i++)
			esk->transform_matrix[i] = matrix2[i];
	}

	


	double resultTransformMatrix[16];
	double tmpTransformMatrix[16];

	for (size_t i = 0; i < 16; i++)
	{
		tmpTransformMatrix[i] = matrix1[i];
		resultTransformMatrix[i] = matrix1[i];
	}

	ESKBone::transpose4x4(&resultTransformMatrix[0]);		//come back to Ogre matrice way
	double skinning_matrix_b[12];						//special tranformation from observation between skinningMatrix and transformMatrix
	ESKBone::decomposition4x4(&resultTransformMatrix[0], &skinning_matrix_b[0]);

	for (size_t i = 0; i < 12; i++)
		esk->skinning_matrix[i] = (float)skinning_matrix_b[i];
}





#ifdef FBX_SUPPORT

FbxAMatrix EMO_Bone::GetGlobalTransform() const
{
	FbxAMatrix bone_matrix = EMO_Bone::ArrayToFbxAMatrix(matrix1);

    if (parent)
    {
        bone_matrix = parent->GetGlobalTransform() * bone_matrix;
    }

    return bone_matrix;
}

void EMO_Bone::FbxMatrixToArray(float *mo, const FbxMatrix *mi)
{
	for (int i = 0; i < 4; i++)
	{
		FbxVector4 row = mi->GetRow(i);

		mo[i] = (float)row[0];
		mo[i + 4] = (float)row[1];
		mo[i + 8] = (float)row[2];
		mo[i + 12] = (float)row[3];
	}
}

void EMO_Bone::FbxMatrixToArray(double *mo, const FbxMatrix *mi)
{
	for (int i = 0; i < 4; i++)
	{
		FbxVector4 row = mi->GetColumn(i);

		mo[i] = row[0];
		mo[i + 4] = row[1];
		mo[i + 8] = row[2];
		mo[i + 12] = row[3];
	}
}

FbxMatrix EMO_Bone::ArrayToFbxMatrix(const float *mi)
{
	return FbxMatrix(mi[0], mi[1], mi[2], mi[3],
		mi[4], mi[5], mi[6], mi[7],
		mi[8], mi[9], mi[10], mi[11],
		mi[12], mi[13], mi[14], mi[15]);
}

FbxAMatrix EMO_Bone::ArrayToFbxAMatrix(const float *mi)
{
	FbxMatrix m = ArrayToFbxMatrix(mi);
	FbxVector4 translation, rotation, scaling, shearing;
	double sign;

	m.GetElements(translation, rotation, shearing, scaling, sign);
	return FbxAMatrix(translation, rotation, scaling);
}

FbxAMatrix EMO_Bone::GetGlobalDefaultPosition(FbxNode* node)
{
	FbxAMatrix local_position;
	FbxAMatrix global_position;
	FbxAMatrix parent_global_position;

	local_position.SetT(node->LclTranslation.Get());
	local_position.SetR(node->LclRotation.Get());
	local_position.SetS(node->LclScaling.Get());

	if (node->GetParent())
	{
		parent_global_position = GetGlobalDefaultPosition(node->GetParent());
		global_position = parent_global_position * local_position;
	}
	else
	{
		global_position = local_position;
	}

	return global_position;
}

void EMO_Bone::SetGlobalDefaultPosition(FbxNode* node, FbxAMatrix global_position)
{
	FbxAMatrix local_position;
	FbxAMatrix parent_global_position;

	if (node->GetParent())
	{
		parent_global_position = GetGlobalDefaultPosition(node->GetParent());
		local_position = parent_global_position.Inverse() * global_position;
	}
	else
	{
		local_position = global_position;
	}

	node->LclTranslation.Set(local_position.GetT());
	node->LclRotation.Set(local_position.GetR());
	node->LclScaling.Set(local_position.GetS());
}

#endif

bool EMO_Bone::PartialCompare(const EMO_Bone *b1, const EMO_Bone *b2)
{
    if (b1 == b2)
    {
        return true;
    }
    else if (!b1 || !b2)
    {
        return false;
    }

    if (b1->name != b2->name)
        return false;

    if (b1->has_unk != b2->has_unk)
        return false;

    if (b1->has_matrix2 != b2->has_matrix2)
        return false;

    if (memcmp(b1->matrix1, b2->matrix1, sizeof(b1->matrix1)) != 0)
        return false;

    if (b1->has_matrix2)
    {
        if (memcmp(b1->matrix2, b2->matrix2, sizeof(b1->matrix2)) != 0)
            return false;
    }

    if (memcmp(b1->sn_u0A, b2->sn_u0A, sizeof(b1->sn_u0A)) != 0)
        return false;

    if (b1->has_unk)
    {
        if (memcmp(b1->usd_u00, b2->usd_u00, sizeof(b1->usd_u00)) != 0)
            return false;
    }

    if (b1->parent != b2->parent)
    {
        if (!b1->parent || !b2->parent)
            return false;

        if (b1->parent->name != b2->parent->name)
            return false;
    }

    if (b1->child != b2->child)
    {
        if (!b1->child || !b2->child)
            return false;

        if (b1->child->name != b2->child->name)
            return false;
    }

    if (b1->sibling != b2->sibling)
    {
        if (!b1->sibling || !b2->sibling)
            return false;

        if (b1->sibling->name != b2->sibling->name)
            return false;
    }

	if (b1->emgIndex != b2->emgIndex)
		return false;

	if (b1->index_4 != b2->index_4)
		return false;
    
    return true;
}

bool EMO_Bone::operator==(const EMO_Bone &rhs) const
{
    if (!PartialCompare(this, &rhs))
        return false;

    if (!PartialCompare(this->parent, rhs.parent))
        return false;

    if (!PartialCompare(this->child, rhs.child))
        return false;

    if (!PartialCompare(this->sibling, rhs.sibling))
        return false;

	if (this->emgIndex != rhs.emgIndex)
		return false;

	if (this->index_4 != rhs.index_4)
		return false;

    return true;
}

EMO_Skeleton::EMO_Skeleton(void)
{
    ik_data = nullptr;
    big_endian = false;
    Reset();
}

EMO_Skeleton::EMO_Skeleton(uint8_t *buf, unsigned int size)
{
	ik_data = nullptr;
    big_endian = false;
    Load(buf, size);
}

EMO_Skeleton::~EMO_Skeleton()
{
    Reset();
}

void EMO_Skeleton::Copy(const EMO_Skeleton &other)
{
    if (other.ik_data)
    {
        this->ik_data = new uint8_t[other.ik_size];
		memcpy(this->ik_data, other.ik_data, other.ik_size);
		
        this->ik_size = other.ik_size;
    }
    else
    {
        this->ik_data = nullptr;
        this->ik_size = 0;
    }

    this->unk_02 = other.unk_02;
    this->unk_06 = other.unk_06;

    memcpy(this->unk_10, other.unk_10, sizeof(this->unk_10));
    memcpy(this->unk_34, other.unk_34, sizeof(this->unk_34));
    memcpy(this->unk_38, other.unk_38, sizeof(this->unk_38));

    this->bones = other.bones;

    for (EMO_Bone &b : bones)
    {
        if (b.parent)
        {
            b.parent = GetBone(b.parent->name);
			assert(b.parent != nullptr);
        }
		
		if (b.child)
		{
			b.child = GetBone(b.child->name);
			assert(b.child != nullptr);
		}
		
		if (b.sibling)
		{
			b.sibling = GetBone(b.sibling->name);
			assert(b.sibling != nullptr);
		}
    }

    this->big_endian = other.big_endian;
}

void EMO_Skeleton::Reset()
{
    if (ik_data)
    {
        delete[] ik_data;
        ik_data = nullptr;
    }

    ik_size = unk_02 = unk_06 = unk_10[0] = unk_10[1] = unk_34[0] = unk_34[1] = 0;
	unk_38[0] = unk_38[1] = 0.0f;

    bones.clear();
}

uint16_t EMO_Skeleton::FindBone(const std::vector<EMO_Bone *> &bones, EMO_Bone *bone, bool assert_if_not_found)
{
    assert(bone != nullptr);

    for (size_t i = 0; i < bones.size(); i++)
    {
        if (bones[i] == bone)
            return i;
    }

    if (assert_if_not_found)
    {
        assert(0);
    }

    return 0xFFFF;
}

void EMO_Skeleton::RebuildSkeleton(const std::vector<EMO_Bone *> &old_bones_ptr)
{
    for (EMO_Bone &b : bones)
    {
        if (b.parent != nullptr)
        {
            b.parent = &bones[FindBone(old_bones_ptr, b.parent, true)];
        }

        if (b.child != nullptr)
        {
            b.child = &bones[FindBone(old_bones_ptr, b.child, true)];
        }

        if (b.sibling != nullptr)
        {
            b.sibling = &bones[FindBone(old_bones_ptr, b.sibling, true)];
        }
    }
}

uint16_t EMO_Skeleton::AppendBone(const EMO_Skeleton &other, const std::string &name)
{
    const EMO_Bone *bone = other.GetBone(name);
    if (!bone)
        return (uint16_t)-1;

    return AppendBone(*bone);
}

uint16_t EMO_Skeleton::AppendBone(const EMO_Bone &bone)
{
    std::vector<EMO_Bone *> old_bones_ptr;
    EMO_Bone new_bone = bone;

    new_bone.parent = nullptr;
    new_bone.child = nullptr;
    new_bone.sibling = nullptr;
    new_bone.emgIndex = 0xfff;
    new_bone.index_4 = 0;

    for (EMO_Bone &b : bones)
    {
        old_bones_ptr.push_back(&b);
    }

    bones.push_back(new_bone);
    RebuildSkeleton(old_bones_ptr);

    return (bones.size()-1);
}

bool EMO_Skeleton::CloneEMO_BoneParentChild(const EMO_Skeleton &other, const std::string &bone_name, EMO_Bone **not_found)
{
    EMO_Bone *bone_me;
    const EMO_Bone *bone_other;

    //LOG_DEBUG("Clone: %s\n", bone_name.c_str());

    if (not_found)
        *not_found = nullptr;

    bone_me = GetBone(bone_name);
    if (!bone_me)
        return false;

    bone_other = other.GetBone(bone_name);
    if (!bone_other)
        return false;

    EMO_Bone bone_temp = EMO_Bone(*bone_me);

    if (!bone_other->parent)
    {
        bone_temp.parent = nullptr;
    }
    else
    {
        EMO_Bone *parent = GetBone(bone_other->parent->name);

        if (!parent)
        {
            *not_found = bone_other->parent;
            return false;
        }

        bone_temp.parent = parent;
    }

    if (!bone_other->child)
    {
        bone_temp.child = nullptr;
    }
    else
    {
        EMO_Bone *child = GetBone(bone_other->child->name);

        if (!child)
        {
            *not_found = bone_other->child;
            return false;
        }

        bone_temp.child = child;
    }

    if (!bone_other->sibling)
    {
        bone_temp.sibling = nullptr;
    }
    else
    {
        EMO_Bone *sibling = GetBone(bone_other->sibling->name);

        if (!sibling)
        {
            *not_found = bone_other->sibling;
            return false;
        }

        bone_temp.sibling = sibling;
    }

	bone_temp.emgIndex = bone_other->emgIndex;
	bone_temp.index_4 = bone_other->index_4;

    //bones[BoneToIndex(bone_me)] = bone_temp;
    *bone_me = bone_temp;
    return true;
}

uint16_t EMO_Skeleton::BoneToIndex(EMO_Bone *bone) const
{
    if (!bone)
        return 0xFFFF;

    for (size_t i = 0; i < bones.size(); i++)
    {
        if (bone == &bones[i])
            return i;
    }

    LOG_DEBUG("%s: We are about to crash or abort.\n", FUNCNAME);
    LOG_DEBUG("Cannot find bone \"%s\" in this skeleton\n", bone->name.c_str());

    //LOG_DEBUG("%s: coding error somewhere. This bone is not in this object (%s)\n", FUNCNAME, bone->name.c_str());
    throw std::runtime_error("Aborting");
    return 0xFFFF;
}

size_t EMO_Skeleton::CalculateIKSize(const uint8_t *ik_data, uint16_t count)
{
    size_t size = 0;

    for (uint16_t i = 0; i < count; i++)
    {
        uint16_t data_size = val16(*(uint16_t *)(ik_data + 2));

        size += data_size;
        ik_data += data_size;
    }

    return size;
}

uint16_t EMO_Skeleton::CalculateIKCount(const uint8_t *ik_data, size_t size)
{
    const uint8_t *end = ik_data + size;
    unsigned int count = 0;

    while (ik_data < end)
    {
        uint16_t data_size = *(uint16_t *)(ik_data + 2);

        count++;
        ik_data += data_size;
    }    

    return count;
}

void EMO_Skeleton::TranslateIKData(uint8_t *dst, const uint8_t *src, size_t size, bool import)
{
    if (this->big_endian == false)
    {
        memcpy(dst, src, size);
        return;
    }

    const IKEntry *ike_src = (const IKEntry *)src;
    IKEntry *ike_dst = (IKEntry *)dst;
    IKEntry *top = (IKEntry *)(dst + size);

    while (ike_dst < top)
    {
       ike_dst->unk_00 = val16(ike_src->unk_00);
       ike_dst->entry_size = val16(ike_src->entry_size);

       if (import)
       {
           assert (ike_dst->entry_size == 0x18);
       }

       ike_dst->unk_03 = ike_src->unk_03;
       ike_dst->unk_04 = ike_src->unk_04;
       ike_dst->unk_06 = val16(ike_src->unk_06);

       ike_dst->unk_08[0] = val16(ike_src->unk_08[0]);
       ike_dst->unk_08[1] = val16(ike_src->unk_08[1]);

       assert(ike_src->unk_08[2] == 0 && ike_src->unk_08[3] == 0);

       ike_dst->unk_08[2] = 0;
       ike_dst->unk_08[3] = 0;

       ike_dst->unk_10[0] = val32(ike_src->unk_10[0]);
       ike_dst->unk_10[1] = val32(ike_src->unk_10[1]);

       if (import)
       {
           assert(ike_dst->unk_10[0] == 0x3F000000 && ike_dst->unk_10[1] == 0);
       }

       ike_src++;
       ike_dst++;
    }

    //LOG_DEBUG("Comparison after translation: %d\n", (memcmp(dst, src, size) == 0));
}

bool EMO_Skeleton::Load(const uint8_t *buf, unsigned int size)
{
	Reset();

	UNUSED(size);

	SkeletonHeader *hdr = (SkeletonHeader *)buf;
	uint16_t node_count = val16(hdr->node_count);

	if (hdr->ik_data_offset)
	{
		ik_size = CalculateIKSize(GetOffsetPtr(buf, hdr->ik_data_offset), val16(hdr->ik_count));
		if (ik_size != 0x108)
		{
			// Temporal message, it will be supressed in future
			// Update: supressed now
			//LOG_DEBUG("Warning: ik_size not 0x108. Ignore this if this is not a SSSS CHARACTER file.\n");
		}

		ik_data = new uint8_t[ik_size];

		if (!ik_data)
		{
			LOG_DEBUG("%s: Memory allocation error.\n", FUNCNAME);
			return false;
		}

		TranslateIKData(ik_data, GetOffsetPtr(buf, hdr->ik_data_offset), ik_size, true);
	}
	else
	{
		ik_data = nullptr;
		ik_size = 0;
	}

	unk_02 = val16(hdr->unk_02);
	unk_06 = val16(hdr->unk_06);

	unk_10[0] = val32(hdr->unk_10[0]);
	unk_10[1] = val32(hdr->unk_10[1]);

	if (hdr->unk_24[0] != 0 || hdr->unk_24[1] != 0 || hdr->unk_24[2] != 0 || hdr->unk_24[3] != 0)
	{
		LOG_DEBUG("%s: unk_24 not zero as expected.\n", FUNCNAME);
		return false;
	}

	unk_34[0] = val16(hdr->unk_34[0]);
	unk_34[1] = val16(hdr->unk_34[1]);

	
	unk_38[0] = val_float(hdr->unk_38[0]);
	unk_38[1] = val_float(hdr->unk_38[1]);
	
	/*
	//test essaye de trouver a quoi sert unk_38 qui a l'air d'etre important , et qui est le meme dans le xxxx.obj.ema.
	{
		uint32_t *p = (uint32_t *)&(hdr->unk_38[0]);
		uint32_t *p_2 = (uint32_t *)&(hdr->unk_38[1]);

		//inversion du val32() pour les avoir a nouveau dans l'ordre
#ifdef __BIG_ENDIAN__
		*p = (big_endian) ? LE32(*p) : *p;
		*p_2 = (big_endian) ? LE32(*p_2) : *p_2;
#else
		*p = (big_endian) ? *p : BE32(*p);
		*p_2 = (big_endian) ? *p_2 : BE32(*p_2);
#endif

		uint16_t *p16 = (uint16_t*)p;
		uint16_t *p16_2 = (uint16_t*)p_2;

		float a38_a = float16ToFloat(val16(p16[1]));
		float a38_b = float16ToFloat(val16(p16[0]));
		float a38_c = float16ToFloat(val16(p16_2[1]));
		float a38_d = float16ToFloat(val16(p16_2[0]));
		

		int aa = 42;
	}
	*/

    uint32_t *names_table = (uint32_t *)GetOffsetPtr(buf, hdr->names_offset);
    SkeletonNode *nodes = (SkeletonNode *)GetOffsetPtr(buf, hdr->start_offset);
    UnkSkeletonData *unks = nullptr;
    MatrixData *matrixes = nullptr;

    if (hdr->unk_skd_offset)
    {
        unks = (UnkSkeletonData *)GetOffsetPtr(buf, hdr->unk_skd_offset);
    }

    if (hdr->matrix_offset)
    {
        matrixes = (MatrixData *)GetOffsetPtr(buf, hdr->matrix_offset);
    }

    for (uint16_t i = 0; i < node_count; i++)
    {
        EMO_Bone bone;

        bone.meta_original_offset = EMO_BaseFile::DifPointer(&nodes[i], buf);
        bone.name = std::string((char *)GetOffsetPtr(buf, names_table, i));

        for (int j = 0; j < 16; j++)
        {
            bone.matrix1[j] = val_float(nodes[i].matrix[j]);
        }

        bone.sn_u0A[0] = val16(nodes[i].unk_0A[0]);
        bone.sn_u0A[1] = val16(nodes[i].unk_0A[1]);
        bone.sn_u0A[2] = val16(nodes[i].unk_0A[2]);

        if (unks)
        {
            bone.has_unk = true;
            bone.usd_u00[0] = val16(unks[i].unk_00[0]);
            bone.usd_u00[1] = val16(unks[i].unk_00[1]);
            bone.usd_u00[2] = val16(unks[i].unk_00[2]);
            bone.usd_u00[3] = val16(unks[i].unk_00[3]);
        }
        else
        {
            bone.has_unk = false;
        }

        if (matrixes)
        {
            bone.has_matrix2 = true;

            for (int j = 0; j < 16; j++)
            {
                bone.matrix2[j] = val_float(matrixes[i].matrix_00[j]);
            }
        }
        else
        {
            bone.has_matrix2 = false;
        }

        bones.push_back(bone);
    }

    assert(bones.size() == node_count);

    for (uint16_t i = 0; i < node_count; i++)
    {
        uint16_t parent, child, sibling, emgIndex, index_4;

        parent = val16(nodes[i].parent_id);
		child = val16(nodes[i].child_id);
		sibling = val16(nodes[i].sibling_id);
		emgIndex = val16(nodes[i].emgIndex);
		index_4 = val16(nodes[i].index_4);

        if (parent == 0xFFFF)
        {
            bones[i].parent = nullptr;
        }
        else if (parent < node_count)
        {
            bones[i].parent = &bones[parent];
        }
        else
        {
            LOG_DEBUG("%s: parent is out of range (on EMO_Bone %d %s)\n", FUNCNAME, i, bones[i].name.c_str());
        }

        if (child == 0xFFFF)
        {
            bones[i].child = nullptr;
        }
        else if (child < node_count)
        {
            bones[i].child = &bones[child];
        }
        else
        {
            LOG_DEBUG("%s: child1 is out of range (on EMO_Bone %d %s)\n", FUNCNAME, i, bones[i].name.c_str());
        }

        if (sibling == 0xFFFF)
        {
            bones[i].sibling = nullptr;
        }
        else if (sibling < node_count)
        {
            bones[i].sibling = &bones[sibling];
        }
        else
        {
            LOG_DEBUG("%s: child2 is out of range (on EMO_Bone %d %s)\n", FUNCNAME, i, bones[i].name.c_str());
        }

		bones[i].emgIndex = emgIndex;
		bones[i].index_4 = index_4;
    }

    return true;
}

unsigned int EMO_Skeleton::CalculateFileSize() const
{
    if (bones.size() == 0)
        return 0;

    unsigned int file_size = sizeof(SkeletonHeader);
    file_size += (bones.size() * sizeof(SkeletonNode));

    if (bones[0].has_unk)
    {
        file_size += (bones.size() * sizeof(UnkSkeletonData));
    }

    file_size += (bones.size() * sizeof(uint32_t)); // Names table

    for (const EMO_Bone &b : bones)
    {
        file_size += b.name.length() + 1;
    }

    // Padding
    if (file_size & 0xF)
    {
        file_size += (0x10 - (file_size & 0xF));
    }

    if (bones[0].has_matrix2)
    {
        file_size += (bones.size() * sizeof(MatrixData));
    }

    if (ik_data)
    {
        file_size += ik_size;
    }

    return file_size;
}

uint8_t *EMO_Skeleton::CreateFile(unsigned int *psize)
{
	std::vector<string> listEMG;
	return CreateFile(psize, listEMG);
}

uint8_t *EMO_Skeleton::CreateFile(unsigned int *psize, std::vector<string> listEMG)
{
    if (bones.size() == 0)
        return nullptr;

    uint8_t *buf;
    unsigned int file_size;
    uint32_t offset;

    file_size = CalculateFileSize();
    buf = new uint8_t[file_size];
    if (!buf)
    {
        LOG_DEBUG("%s: Memory allocation error (0x%x)\n", FUNCNAME, file_size);
        return nullptr;
    }

    memset(buf, 0, file_size);

    SkeletonHeader *hdr = (SkeletonHeader *)buf;

    hdr->node_count = val16(bones.size());
    hdr->unk_02 = val16(unk_02);
    hdr->unk_06 = val16(unk_06);
    hdr->unk_10[0] = val32(unk_10[0]);
    hdr->unk_10[1] = val32(unk_10[1]);
    hdr->unk_34[0] = val16(unk_34[0]);
    hdr->unk_34[1] = val16(unk_34[1]);
    copy_float(&hdr->unk_38[0], unk_38[0]);
    copy_float(&hdr->unk_38[1], unk_38[1]);

    offset = sizeof(SkeletonHeader);
    hdr->start_offset = val32(offset);

    SkeletonNode *nodes = (SkeletonNode *)GetOffsetPtr(buf, offset, true);
	
	size_t nbEMG = listEMG.size();

    for (size_t i = 0; i < bones.size(); i++)
    {
        nodes[i].parent_id = val16(BoneToIndex(bones[i].parent));
        nodes[i].child_id = val16(BoneToIndex(bones[i].child));
        nodes[i].sibling_id = val16(BoneToIndex(bones[i].sibling));

		nodes[i].emgIndex = val16(0xFFFF);
		string boneName = bones[i].GetName();
		for (size_t j = 0; j < nbEMG; j++)
		{
			if (listEMG.at(j) == boneName)
			{
				nodes[i].emgIndex = val16(j);
				break;
			}
		}

        nodes[i].index_4 = val16(bones[i].index_4);

        nodes[i].unk_0A[0] = val16(bones[i].sn_u0A[0]);
        nodes[i].unk_0A[1] = val16(bones[i].sn_u0A[1]);
        nodes[i].unk_0A[2] = val16(bones[i].sn_u0A[2]);

        for (int j = 0; j < 16; j++)
            copy_float(&nodes[i].matrix[j], bones[i].matrix1[j]);

        offset += sizeof(SkeletonNode);
    }

    if (bones[0].has_unk)
    {
        hdr->unk_skd_offset = val32(offset);

        UnkSkeletonData *unks = (UnkSkeletonData *)GetOffsetPtr(buf, offset, true);

        for (size_t i = 0; i < bones.size(); i++)
        {
            unks[i].unk_00[0] = val16(bones[i].usd_u00[0]);
            unks[i].unk_00[1] = val16(bones[i].usd_u00[1]);
            unks[i].unk_00[2] = val16(bones[i].usd_u00[2]);
            unks[i].unk_00[3] = val16(bones[i].usd_u00[3]);

            offset += sizeof(UnkSkeletonData);
        }
    }
    else
    {
        hdr->unk_skd_offset = 0;
    }

    hdr->names_offset = val32(offset);

    uint32_t *names_table = (uint32_t *)GetOffsetPtr(buf, offset, true);
    offset += bones.size() * sizeof(uint32_t);

    for (size_t i = 0; i < bones.size(); i++)
    {
        names_table[i] = val32(offset);
        strcpy((char *)buf+offset, bones[i].name.c_str());
        offset += bones[i].name.length() + 1;
    }

    // Padding
    if (offset & 0xF)
    {
        offset += (0x10 - (offset & 0xF));
    }

    if (bones[0].has_matrix2)
    {
        hdr->matrix_offset = val32(offset);

        MatrixData *matrixes = (MatrixData *)GetOffsetPtr(buf, offset, true);

        for (size_t i = 0; i < bones.size(); i++)
        {
            for (int j = 0; j < 16; j++)
            {
                copy_float(&matrixes[i].matrix_00[j], bones[i].matrix2[j]);
            }

            offset += sizeof(MatrixData);
        }
    }
    else
    {
        hdr->matrix_offset = 0;
    }

    if (ik_data)
    {
       hdr->ik_data_offset = val32(offset);

       TranslateIKData(buf+offset, ik_data, ik_size, false);
       offset += ik_size;

       hdr->ik_count = val16(CalculateIKCount(ik_data, ik_size));
    }
    else
    {
        hdr->ik_data_offset = 0;
        hdr->ik_count = 0;
    }

    assert(offset == file_size);

    *psize = file_size;
    return buf;
}

void EMO_Skeleton::Decompile(TiXmlNode *root) const
{
    TiXmlElement *entry_root = new TiXmlElement("Skeleton");

    EMO_BaseFile::WriteParamUnsigned(entry_root, "U_02", unk_02, true);
    EMO_BaseFile::WriteParamUnsigned(entry_root, "U_06", unk_06, true);
    EMO_BaseFile::WriteParamMultipleUnsigned(entry_root, "U_10", std::vector<uint32_t>(unk_10, unk_10+2), true);
    EMO_BaseFile::WriteParamMultipleUnsigned(entry_root, "U_34", std::vector<uint16_t>(unk_34, unk_34+2), true);
    EMO_BaseFile::WriteParamMultipleFloats(entry_root, "U_38", std::vector<float>(unk_38, unk_38+2));

    for (const EMO_Bone &b : bones)
    {
        b.Decompile(entry_root);
    }

    if (ik_data)
    {
        EMO_BaseFile::WriteParamBlob(entry_root, "IK_DATA", ik_data, ik_size);
    }

    root->LinkEndChild(entry_root);
}

TiXmlDocument *EMO_Skeleton::Decompile() const
{
    TiXmlDocument *doc = new TiXmlDocument();
    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "" );
    doc->LinkEndChild(decl);

    Decompile(doc);

    return doc;
}

bool EMO_Skeleton::Compile(const TiXmlElement *root)
{
    unsigned int unk_02, unk_06;
    std::vector<uint32_t> unk_10;
    std::vector<uint16_t> unk_34;
    std::vector<float> unk_38;

    if (!EMO_BaseFile::GetParamUnsigned(root, "U_02", &unk_02))
        return false;

    if (unk_02 > 0xFFFF)
    {
        LOG_DEBUG("%s: \"U_02\" must be a 16 bits value.\n", FUNCNAME);
        return false;
    }

    this->unk_02 = unk_02;

    if (!EMO_BaseFile::GetParamUnsigned(root, "U_06", &unk_06))
        return false;

    if (unk_06 > 0xFFFF)
    {
        LOG_DEBUG("%s: \"U_06\" must be a 16 bits value.\n", FUNCNAME);
        return false;
    }

    this->unk_06 = unk_06;

    if (!EMO_BaseFile::GetParamMultipleUnsigned(root, "U_10", unk_10))
        return false;

    if (!EMO_BaseFile::GetParamMultipleUnsigned(root, "U_34", unk_34))
        return false;

    if (!EMO_BaseFile::GetParamMultipleFloats(root, "U_38", unk_38))
        return false;

    if (unk_10.size() != 2)
    {
        LOG_DEBUG("%s: Invalid size for \"U_10\"\n", FUNCNAME);
        return false;
    }

    if (unk_34.size() != 2)
    {
        LOG_DEBUG("%s: Invalid size for \"U_34\"\n", FUNCNAME);
        return false;
    }

    if (unk_38.size() != 2)
    {
        LOG_DEBUG("%s: Invalid size for \"U_38\"\n", FUNCNAME);
        return false;
    }

    memcpy(this->unk_10, unk_10.data(), sizeof(this->unk_10));
    memcpy(this->unk_34, unk_34.data(), sizeof(this->unk_34));
    memcpy(this->unk_38, unk_38.data(), sizeof(this->unk_38));

    // Skeleton, first pass
    for (const TiXmlElement *elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
    {
        const std::string &str = elem->ValueStr();

        if (str == "EMO_Bone")
        {
            EMO_Bone b;

            if (!b.Compile(elem, nullptr))
            {
                LOG_DEBUG("%s: EMO_Bone compilation failed.\n", FUNCNAME);
                return false;
            }

            bones.push_back(b);
        }
    }

    // Skeleton, second pass
    size_t i = 0;
    for (const TiXmlElement *elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
    {
        const std::string &str = elem->ValueStr();

        if (str == "EMO_Bone")
        {
            if (!bones[i++].Compile(elem, this))
            {
                LOG_DEBUG("%s: EMO_Bone compilation failed.\n", FUNCNAME);
                return false;
            }
        }
    }
	
	ik_data = EMO_BaseFile::ReadParamBlob(root, "IK_DATA", &ik_size);
	if (!ik_data)
		ik_size = 0;

    return true;
}

bool EMO_Skeleton::Compile(TiXmlDocument *doc, bool big_endian)
{
    Reset();
    this->big_endian = big_endian;

    TiXmlHandle handle(doc);
    const TiXmlElement *root = EMO_BaseFile::FindRoot(&handle, "Skeleton");

    if (!root)
    {
        LOG_DEBUG("Cannot find\"Skeleton\" in xml.\n");
        return false;
    }

    return Compile(root);
}

bool EMO_Skeleton::SaveSkeletonToFile(const std::string &path, bool show_error, bool build_path)
{
    unsigned int size;

    uint8_t *buf = EMO_Skeleton::CreateFile(&size);

    if (!buf)
        return false;

    bool ret = EMO_BaseFile::WriteFileBool(path, buf, size, show_error, build_path);
    delete[] buf;

    return ret;
}

bool EMO_Skeleton::DecompileSkeletonToFile(const std::string &path, bool show_error, bool build_path)
{
    TiXmlDocument *doc = EMO_Skeleton::Decompile();

    if (!doc)
    {
        if (show_error)
        {
             LOG_DEBUG("Decompilation of file \"%s\" failed.\n", path.c_str());
        }

        return false;
    }

    if (build_path)
    {
        if (!EMO_BaseFile::CreatePath(path))
        {
            if (show_error)
            {
                LOG_DEBUG("Cannot create path for file \"%s\"\n", path.c_str());
            }

            return false;
        }
    }

    bool ret = doc->SaveFile(path);
    delete doc;

    if (!ret && show_error)
    {
        LOG_DEBUG("Cannot create/write file \"%s\"\n", path.c_str());
    }

    return ret;
}

#ifdef FBX_SUPPORT

bool EMO_Skeleton::ExportFbxBone(const EMO_Bone *parent, FbxNode *root_node, FbxScene *scene, std::vector<FbxNode *> &fbx_bones) const
{
    for (size_t i = 0; i < bones.size(); i++)
    {
        const EMO_Bone &bone = bones[i];

        if (bone.parent != parent)
            continue;

        FbxSkeleton* skeleton_root_attribute = FbxSkeleton::Create(scene, bone.name.c_str());

        if (!parent)
        {
            skeleton_root_attribute->SetSkeletonType(FbxSkeleton::eRoot);
        }
        else
        {
            skeleton_root_attribute->SetSkeletonType(FbxSkeleton::eLimbNode);
        }

        FbxNode* fbx_bone_node = FbxNode::Create(scene, bone.name.c_str());
        fbx_bone_node->SetNodeAttribute(skeleton_root_attribute);
        root_node->AddChild(fbx_bone_node);

        FbxAMatrix matrix = bone.GetGlobalTransform();

        /*bool invert_yz = false;
        bool change_y = false;

        if (invert_yz)
        {
            FbxVector4 translation, rotation, scaling;
            double temp;

            translation = matrix.GetT();
            rotation = matrix.GetR();
            scaling = matrix.GetS();

            temp = translation[1];
            translation[1] = translation[2];
            translation[2] = temp;

            temp = rotation[1];
            rotation[1] = rotation[2];
            rotation[2] = temp;

            temp = scaling[1];
            scaling[1] = scaling[2];
            scaling[2] = temp;

            if (change_y)
            {
                translation[1] = -translation[1];
                rotation[1] = -rotation[1];
            }

            matrix.SetT(translation);
            matrix.SetR(rotation);
            matrix.SetS(scaling);
        }*/

		EMO_Bone::SetGlobalDefaultPosition(fbx_bone_node, matrix);

        fbx_bones[i] = fbx_bone_node;
        ExportFbxBone(&bone, fbx_bone_node, scene, fbx_bones);
    }

    return true;
}

bool EMO_Skeleton::ExportFbx(FbxScene *scene, std::vector<FbxNode *> &fbx_bones) const
{
    fbx_bones.resize(bones.size());
    return ExportFbxBone(nullptr, scene->GetRootNode(), scene, fbx_bones);
}

#endif

bool EMO_Skeleton::operator==(const EMO_Skeleton &rhs) const
{
    if (this->ik_size != rhs.ik_size)
        return false;

    if (this->ik_size != 0)
    {
        if (memcmp(this->ik_data, rhs.ik_data, this->ik_size) != 0)
            return false;
    }

    if (this->unk_02 != rhs.unk_02)
        return false;

    if (this->unk_06 != rhs.unk_06)
        return false;

    if (memcmp(this->unk_10, rhs.unk_10, sizeof(rhs.unk_10)) != 0)
        return false;

    if (memcmp(this->unk_34, rhs.unk_34, sizeof(rhs.unk_34)) != 0)
        return false;

    if (memcmp(this->unk_38, rhs.unk_38, sizeof(rhs.unk_38)) != 0)
        return false;

    if (this->bones != rhs.bones)
        return false;

    return true;
}





/*-------------------------------------------------------------------------------\
|                             writeEsk				                             |
\-------------------------------------------------------------------------------*/
void EMO_Skeleton::writeEsk(ESK* esk)
{
	esk->name = name;


	esk->bones.clear();
	EskTreeNode* rootNode = esk->getTreeOrganisation();




	//to secure, we begin to build bones, and after we will made the hierarchy.
	std::vector<EskTreeNode*> listTreeNode;
	ESKBone* bone;
	EskTreeNode* node;
	size_t nbElements = bones.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		bone = new ESKBone();
		bones.at(i).writeESKBone(bone);
		esk->bones.push_back(bone);

		node = new EskTreeNode(esk->bones.at(i), i, rootNode);			//by default, rootnode is the parent of all.
		listTreeNode.push_back(node);
	}


	for (size_t i = 0; i < nbElements; i++)
	{
		EMO_Bone &emoBone = bones.at(i);

		EMO_Bone* emoBone_ptr = &emoBone;
		if (!emoBone_ptr->parent)									//we take care only bone witch have rootNode as parent.
		{
			rootNode->mChildren.push_back(listTreeNode.at(i));
			listTreeNode.at(i)->mParent = rootNode;

			writeEsk__recursive(emoBone_ptr, bones, listTreeNode.at(i), listTreeNode);		//after, we take care of children recursively.
		}
	}


	//all the recursive try to take care of child and sibling informations. saddly, there could have wrong information about child information.
	//saddly, by adding security on it, it could break the link witch could have with the parent information.
	//SO, we have to look after missing bone. if the parent informaiton is good, we could add it in hierarchy, else, it will still on rootNode.
	size_t nbNode = listTreeNode.size();
	for (size_t i = 0; i < nbNode; i++)											//we look at all bones
	{
		if (rootNode->getBoneWithIndex(i))
			continue;

		//if we miss someone in hierarchy.
		EMO_Bone &emoBone = bones.at(i);
		if (emoBone.parent)										//try with parent information (instead of child and sibling)
		{
			bool isGood = true;
			EMO_Bone* emoBone_tmp = emoBone.parent;
			while (emoBone_tmp)
			{
				if (emoBone_tmp == &emoBone)						//try to avoid bone be the parent of the parent, or him self directly.
				{
					isGood = false;
					break;
				}
				emoBone_tmp = emoBone_tmp->parent;
			}

			if (isGood)
			{
				size_t isFound = (size_t)-1;
				size_t nbElements = bones.size();
				for (size_t j = 0; j < nbElements; j++)
				{
					if (&bones.at(j) == emoBone.parent)
					{
						isFound = j;
						break;
					}
				}

				if (isFound != (size_t)-1)					//if we find it, we update the treeNode.
				{
					//another check, if there isn't a fail on sibling, to avoid infinite loop
					size_t nbchildren_tmp = listTreeNode.at(isFound)->mChildren.size();
					for (size_t m = 0; m < nbchildren_tmp; m++)
					{
						if (listTreeNode.at(isFound)->mChildren.at(m) == listTreeNode.at(i))
						{
							printf("warning: There is a strange thing about hierarchy on %s. skipped for avoid infinite loops.\n", listTreeNode.at(i)->mBone->getName().c_str());
							isFound = (size_t)-1;
							break;
						}
					}

					if (isFound != (size_t)-1)
					{
						listTreeNode.at(i)->mParent = listTreeNode.at(isFound);
						listTreeNode.at(isFound)->mChildren.push_back(listTreeNode.at(i));
					}
					else {
						rootNode->mChildren.push_back(listTreeNode.at(i));
						listTreeNode.at(i)->mParent = rootNode;
					}

				}
				else {
					rootNode->mChildren.push_back(listTreeNode.at(i));
					listTreeNode.at(i)->mParent = rootNode;
				}

			}
			else {
				rootNode->mChildren.push_back(listTreeNode.at(i));
				listTreeNode.at(i)->mParent = rootNode;
			}

		}
		else {
			rootNode->mChildren.push_back(listTreeNode.at(i));
			listTreeNode.at(i)->mParent = rootNode;
		}
	}


	esk->setTreeOrganisation(rootNode);
}
/*-------------------------------------------------------------------------------\
|                             writeEsk__recursive				                 |
\-------------------------------------------------------------------------------*/
void EMO_Skeleton::writeEsk__recursive(EMO_Bone* emoBone, std::vector<EMO_Bone> &bones, EskTreeNode* treeNode, std::vector<EskTreeNode*> &listTreeNode)
{


	//we look after all children (child and sibling), to make the hierarchy.
	EMO_Bone* emoBone_child = emoBone->child;
	size_t inc = 0;

	while (emoBone_child != nullptr)
	{
		//to secure on wrong informations about hierarchy (in some emo), we have to break possible infinite loops.
		if (emoBone_child == emoBone)				//I see a case witch the bone was its parent himself, and the child ...
		{
			printf("warning: There is a strange thing about hierarchy on %s. skipped for avoid infinite loops.\n", emoBone_child->GetName().c_str());
			return;
		}

		EMO_Bone* emoBone_tmp = emoBone->parent;
		while (emoBone_tmp)										//case of a bad link between parent/child. lets check if parents are not used as a child of a child.
		{
			if (emoBone_child == emoBone_tmp)
			{
				printf("warning: There is a strange thing about hierarchy on %s. skipped for avoid infinite loops.\n", emoBone_child->GetName().c_str());
				return;
			}
			emoBone_tmp = emoBone_tmp->parent;
		}


		//normal case:

		size_t isFound = (size_t)-1;
		size_t nbElements = bones.size();
		for (size_t j = 0; j < nbElements; j++)
		{
			if (&bones.at(j) == emoBone_child)
			{
				isFound = j;
				break;
			}
		}

		if (isFound != (size_t)-1)					//if we find it, we update the treeNode.
		{
			listTreeNode.at(isFound)->mParent = treeNode;


			//another check, if there isn't a fail on sibling, to avoid infinite loop
			size_t nbchildren_tmp = treeNode->mChildren.size();
			for (size_t j = 0; j < nbchildren_tmp; j++)
			{
				if (treeNode->mChildren.at(j) == listTreeNode.at(isFound))
				{
					printf("warning: There is a strange thing about hierarchy on %s. skipped for avoid infinite loops.\n", emoBone_child->GetName().c_str());
					return;
				}
			}

			treeNode->mChildren.push_back(listTreeNode.at(isFound));

			writeEsk__recursive(emoBone_child, bones, listTreeNode.at(isFound), listTreeNode);		//same for children
		}

		emoBone_child = emoBone_child->sibling;
	}
}





}