#include "Fmp.h"

#include <sstream>
#include <iomanip>

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

namespace LibXenoverse
{


/*-------------------------------------------------------------------------------\
|                             FmpFile				                             |
\-------------------------------------------------------------------------------*/
FmpFile::FmpFile()
{
	originefilename = "";

	Reset();

	//section 0 - FMPHeader : black/grey
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#232323");			//background color
	listTagColors.back().back().push_back("#FFFFFF");			//font color (just to possibility read from background color)
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#464646");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 1 - Section0a : red
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0000");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC7777");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 2 - Section0b : green
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 3 - Section0b_names : green
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#009900");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#339933");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 4 - Section1 : bleu
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#0000CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7777CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 5 - Section2 : yellow
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 6 - Section3 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 7 - Section3 indexes : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#009999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#339999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 8 - Section4 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC00CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC77CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 9 - Section4_Next : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#990099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#993399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



	//section 10 - Section4_Next_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 11 - Section4_Next_c : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 12 - Section4_Next_d : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 13 - Section4_Next_e : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



	//section 14 - Section4_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#770077");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#773377");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 15 - Section4_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#550055");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#553355");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f





	//section 16 - Section4_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA00FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA33FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 17 - Section4_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 18 - Section4_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7700FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7733FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 19 - Section4_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5500FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5533FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f




	//section 20 - Section5 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCCCC");			//bg
	listTagColors.back().back().push_back("#000000");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AAAAAA");			//bg
	listTagColors.back().back().push_back("#000000");			//f


	//section 21 - Section5_Box : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#999999");			//bg
	listTagColors.back().back().push_back("#000000");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#777777");			//bg
	listTagColors.back().back().push_back("#000000");			//f
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FmpFile::Reset()
{
	//section Section0a:
	width_X = 1000.0f;
	width_Y = 1000.0f;
	width_Z = 1000.0f;
	s0a_unk_3 = 0.0f;
	s0a_unk_4 = 0.0f;
	s0a_unk_5 = 0.0f;
	s0a_unk_6 = -9.8f;
	s0a_unk_7 = 4;
	s0a_unk_8 = 0.577350f;
	s0a_unk_9 = -0.577350f;
	s0a_unk_10 = 0.577350f;
	s0a_unk_11 = 0.3f;
	s0a_unk_12 = 0.3f;
	s0a_unk_13 = 0.3f;
	s0a_unk_14 = 0.7f;
	s0a_unk_15 = 0.7f;
	s0a_unk_16 = 0.7f;
	s0a_unk_17 = 100.0f;
	s0a_unk_18 = 500.0f;
	s0a_unk_19 = 0.0f;
	s0a_unk_20 = 0.0f;
	s0a_unk_21 = 0.0f;
	s0a_unk_22 = 1.0f;
	s0a_unk_23 = 1.0f;
	s0a_unk_24 = 1.0f;
	s0a_unk_25 = 100.0f;
	s0a_unk_26 = 300.0f;
	s0a_unk_27 = 75.0f;
	s0a_unk_28 = 50.0f;
	s0a_unk_29 = 0.5f;
	s0a_unk_30 = 0.5f;
	s0a_unk_31 = 0.5f;
	nearDistance = 0.1f;
	farDistance = 10000.0f;
	s0a_unk_34 = 0.785398f;



	//section Section0b:
	s0b_unk_0 = 0x7cb96118;
	s0b_unk_1 = 0;
	s0b_unk_2 = 0x172e5846;
	s0b_unk_3 = 0;
	s0b_unk_4 = 0x204cb00;
	s0b_unk_5 = 0x84000000;
	s0b_unk_6 = 0x2e7cb0;
	s0b_unk_7 = 0x1000000;
	s0b_unk_8 = 0x4000;
	s0b_unk_9 = 0;
	s0b_unk_10 = 0x40;
	s0b_unk_11 = 0x932c0000;
	s0b_unk_12 = 0;
	s0b_unk_13 = 0x20800;
	s0b_unk_14 = 0;
	s0b_unk_15 = 0x5cf96;
	s0b_unk_16 = 0x800000;
	s0b_unk_17 = 0;
	s0b_unk_18 = 0x9000;
	s0b_unk_19 = 0;
	s0b_unk_20 = 0x205cb;
	s0b_unk_21 = 0x8000000;
	s0b_unk_22 = 0;
	s0b_unk_23 = 0x160;
	s0b_unk_23b = 0x2;
	s0b_unk_24 = 0x2800000;
	s0b_unk_25 = 0;
	s0b_unk_26 = 0x400;
	s0b_unk_27 = 0x1000000;
	s0b_unk_28 = 0;
	s0b_unk_29 = 0x100;
	s0b_unk_30 = 0x200000;
	s0b_unk_31 = 0;
	s0b_unk_32 = 0;
	s0b_unk_33 = 0;
	s0b_unk_34 = 0;
	s0b_unk_35 = 0;
	s0b_unk_36 = 0;
	s0b_unk_37 = 0x1000;
	s0b_unk_38 = 0;
	s0b_unk_39 = 0;
	s0b_unk_40 = 0;
	s0b_unk_41 = 0;
	s0b_unk_42 = 0;
	s0b_unk_43 = 0;
	s0b_unk_44 = 0;
	s0b_unk_45 = 0;
	s0b_unk_46 = 0x22c0000;
	s0b_unk_47 = 0x8a0000;
	s0b_unk_48 = 0x140000;
	s0b_unk_49 = 0x14000;
	s0b_unk_50 = 0xa000;
	s0b_unk_51 = 0x280;
	s0b_unk_52 = 0x148;
	s0b_unk_53 = 0x14000a8;
	s0b_unk_54 = 0x2a0000;

	mListTypeName.clear();
	mListTypeName.push_back("Ground");
	mListTypeName.push_back("Wall");
	mListTypeName.push_back("InvisibleWall");
	mListTypeName.push_back("BgObject");
	mListTypeName.push_back("BgObjectPiece");
	mListTypeName.push_back("WaterSurface");
	mListTypeName.push_back("Water");
	mListTypeName.push_back("Collide4Big");
	mListTypeName.push_back("BgObjectBig");
	mListTypeName.push_back("PlayerEvent");
	mListTypeName.push_back("PlayerNpcEvent");
	mListTypeName.push_back("InvisibleWallCamera");
	mListTypeName.push_back("CameraOnly");
	mListTypeName.push_back("Chara");
	mListTypeName.push_back("CharaBig");
	mListTypeName.push_back("Secondary");
	mListTypeName.push_back("NPC");
	mListTypeName.push_back("CharaBody");
	mListTypeName.push_back("SecondarySelf");
	mListTypeName.push_back("FieldOnly");
	mListTypeName.push_back("FieldOnlyBig");
	mListTypeName.push_back("Camera");
	mListTypeName.push_back("Breaker");
	mListTypeName.push_back("DragonBall");
	mListTypeName.push_back("None");
	mListTypeName.push_back("HenshinImpossible");
	mListTypeName.push_back("CheckWaterSurface");
	mListTypeName.push_back("CheckVisibleCollideBg");
	mListTypeName.push_back("CheckVisibleBg");
	mListTypeName.push_back("CheckGround");
	mListTypeName.push_back("CheckBg");
	mListTypeName.push_back("PlayerFriendAtari");
	mListTypeName.push_back("PlayerFriendKurai");
	mListTypeName.push_back("PlayerRivalAtari");
	mListTypeName.push_back("PlayerRivalKurai");
	mListTypeName.push_back("NpcAtari");
	mListTypeName.push_back("NpcKurai");
	mListTypeName.push_back("FriendShotAtari");
	mListTypeName.push_back("FriendShotKurai");
	mListTypeName.push_back("RivalShotAtari");
	mListTypeName.push_back("RivalShotKurai");
	mListTypeName.push_back("FriendEraseShot");
	mListTypeName.push_back("RivalEraseShot");
	mListTypeName.push_back("FriendAbsorbShot");
	mListTypeName.push_back("RivalAbsorbShot");
	mListTypeName.push_back("FriendReflectShot");
	mListTypeName.push_back("RivalReflectShot");
	mListTypeName.push_back("FriendBounceShot");
	mListTypeName.push_back("RivalBounceShot");
	mListTypeName.push_back("FriendAtemiShot");
	mListTypeName.push_back("RivalAtemiShot");
	
	size_t nbElements = mListObject.size();
	for (size_t i = 0; i < nbElements; i++)
		delete mListObject.at(i);
	mListObject.clear();

	mListHitboxGroup.clear();

	nbElements = mListHavokFile.size();
	for (size_t i = 0; i < nbElements; i++)
		delete mListHavokFile.at(i);
	mListHavokFile.clear();

	mListS1.clear();
	mListS2.clear();
	mListFragmentsGroups.clear();

	listNsk.size();
	listTextureEmb.size();
	listEmm.size();
	listEma.size();
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FMP_S1::Reset()
{
	name = "";
	unk_04 = 0;
	unk_08 = 0.5f;
	unk_0C = 0.5f;
	unk_10 = 0.0f;
	unk_14 = 999936.0f;
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FMP_S2::Reset()
{
	name = "";
	unk_04 = 1.0f;
	unk_08 = 1.0f;
	unk_0C = 200.0f;
	unk_10 = 100.0f;
	unk_14 = 0.0f;
	unk_18 = 0.0f;
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FMP_Object::Reset()
{
	name = "";
	hitboxGroupIndex = (size_t)-1;
	unk_04 = 1;
	unk_04b = 0;
	unk_0A = 1;
	unk_0B = 0;
	unk_20 = 2.0f;

	transformMatrix4x3[0] = 1; transformMatrix4x3[1] = 0; transformMatrix4x3[2] = 0;			//matrice identity.
	transformMatrix4x3[3] = 0; transformMatrix4x3[4] = 1; transformMatrix4x3[5] = 0;
	transformMatrix4x3[6] = 0; transformMatrix4x3[7] = 0; transformMatrix4x3[8] = 1;
	transformMatrix4x3[9] = 0; transformMatrix4x3[10] = 0; transformMatrix4x3[11] = 0;
	
	listEntity.clear();

	size_t nbElement = listVirtualSubParts.size();
	for (size_t i = 0; i < nbElement; i++)
		delete listVirtualSubParts.at(i);
	listVirtualSubParts.clear();

	if (action)
		delete action;
	action = 0;

	if (hierarchy)
		delete hierarchy;
	hierarchy = 0;

	debug_startOffset = 0;
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FMP_Hierarchy::Reset()
{
	listNode.clear();
	listTransform.clear();
	if (mRoot)
		delete mRoot;
	mRoot = 0;

	debug_startOffset = 0;
}
/*-------------------------------------------------------------------------------\
|                             operator==			                             |
\-------------------------------------------------------------------------------*/
bool FMP_Hierarchy::operator==(const FMP_Hierarchy& other)
{
	if ((listNode.size() != other.listNode.size()) ||
		(listTransform.size() != other.listTransform.size()) )
		return false;

	size_t nbElement = listNode.size();
	for (size_t i = 0; i < nbElement; i++)
		if (listNode.at(i) != other.listNode.at(i))
			return false;

	nbElement = listTransform.size();
	for (size_t i = 0; i < nbElement; i++)
		if (listTransform.at(i) != other.listTransform.at(i))
			return false;

	if (((mRoot == 0) && (other.mRoot != 0)) ||
		((mRoot != 0) && (other.mRoot == 0)) ||
		((mRoot != 0) && (*mRoot != *other.mRoot)))
		return false;

	return true;
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FMP_NodeTransform::Reset()
{
	position[0] = position[1] = position[2] = 0.0f;
	rotation[0] = rotation[1] = rotation[2] = 0.0f;
	scale[0] = scale[1] = scale[2] = 1.0f;
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FMP_Node::Reset()
{
	unk_0 = 0.0f;
	unk_1 = 0.0f;
	unk_2 = 0.0f;
	unk_3 = 1.0;
	unk_4 = 0.0f;
	unk_5 = 0.0f;
	unk_6 = 0.0f;
	unk_7 = 0.0f;
	unk_8 = 0.0f;
	unk_9 = 0.0f;
	unk_10 = 1;
	transformIndex = (size_t)-1;
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FMP_HierarchyNode::Reset()
{
	typeb = 0;
	unk_0 = 0.0f;
	unk_1 = 0.0f;
	unk_2 = 0.0f;

	size_t nbChilds = listChild.size();
	for (size_t i = 0; i < nbChilds; i++)
		delete listChild.at(i);
	listChild.clear();

	listIndex.clear();
}
/*-------------------------------------------------------------------------------\
|                             operator==					                     |
\-------------------------------------------------------------------------------*/
bool FMP_HierarchyNode::operator==(const FMP_HierarchyNode& other)
{
	if ((typeb != other.typeb) ||
		(unk_0 != other.unk_0) ||
		(unk_1 != other.unk_1) ||
		(unk_2 != other.unk_2) ||
		(listChild.size() != other.listChild.size()) ||
		(listIndex.size() != other.listIndex.size()))
		return false;

	size_t nbElement = listIndex.size();
	for (size_t i = 0; i < nbElement; i++)
		if (listIndex.at(i) != other.listIndex.at(i))
			return false;

	nbElement = listChild.size();
	for (size_t i = 0; i < nbElement; i++)
		if (*listChild.at(i) != *other.listChild.at(i))			//recursive.
			return false;

	return true;
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FMP_Entity::Reset()
{
	s4n2_unk_0 = 0;
	
	transformMatrix4x3[0] = 1; transformMatrix4x3[1] = 0; transformMatrix4x3[2] = 0;			//matrice identity.
	transformMatrix4x3[3] = 0; transformMatrix4x3[4] = 1; transformMatrix4x3[5] = 0;
	transformMatrix4x3[6] = 0; transformMatrix4x3[7] = 0; transformMatrix4x3[8] = 1;
	transformMatrix4x3[9] = 0; transformMatrix4x3[10] = 0; transformMatrix4x3[11] = 0;

	haveVisualPart = false;					//for consider or not the next parameters (Section4_Next2_b)
	name = "";

	listLod.clear();
	embTexture_filename = "";
	ema_filename = "";

	s4n2b_unk_0 = 0;
	s4n2b_unk_5 = 0xffffffff;
	s4n2b_unk_6 = 0xffffffff;
	s4n2b_unk_8 = 0xffffffff;
	s4n2b_unk_9 = 60.0f;
	s4n2b_unk_10 = 60.0f;

	debug_startOffset = 0;
}
/*-------------------------------------------------------------------------------\
|                             operator==			                             |
\-------------------------------------------------------------------------------*/
bool FMP_Entity::operator==(const FMP_Entity& other)
{
	if ((s4n2_unk_0 != other.s4n2_unk_0) ||
		(haveVisualPart != other.haveVisualPart) ||
		(name != other.name) ||
		(embTexture_filename != other.embTexture_filename) ||
		(ema_filename != other.ema_filename) ||
		(s4n2b_unk_0 != other.s4n2b_unk_0) ||
		(s4n2b_unk_5 != other.s4n2b_unk_5) ||
		(s4n2b_unk_6 != other.s4n2b_unk_6) ||
		(s4n2b_unk_8 != other.s4n2b_unk_8) ||
		(s4n2b_unk_9 != other.s4n2b_unk_9) ||
		(s4n2b_unk_10 != other.s4n2b_unk_10) ||
		(listLod.size() != other.listLod.size()))
		return false;
	
	for (size_t i = 0; i < 12; i++)
		if (transformMatrix4x3[i] != other.transformMatrix4x3[i])
			return false;

	for (size_t i = 0; i < listLod.size(); i++)
		if ((listLod.at(i).nsk_filename != other.listLod.at(i).nsk_filename) || (listLod.at(i).emm_filename != other.listLod.at(i).emm_filename) || (listLod.at(i).distance != other.listLod.at(i).distance))
			return false;

	return true;
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FMP_VirtualSubPart::Reset()
{
	listIndex.clear();

	if (sub0)
		delete sub0;
	sub0 = 0;

	if (sub1)
		delete sub1;
	sub1 = 0;
	
	if (action)
		delete action;
	action = 0;

	unk_2 = 0;
	unk_2b = 0xffff;
	unk_4 = 0.0f;
	unk_5 = 0.0f;

	transformMatrix4x3[0] = 1; transformMatrix4x3[1] = 0; transformMatrix4x3[2] = 0;			//matrice identity.
	transformMatrix4x3[3] = 0; transformMatrix4x3[4] = 1; transformMatrix4x3[5] = 0;
	transformMatrix4x3[6] = 0; transformMatrix4x3[7] = 0; transformMatrix4x3[8] = 1;
	transformMatrix4x3[9] = 0; transformMatrix4x3[10] = 0; transformMatrix4x3[11] = 0;

	debug_startOffset = 0;
}
/*-------------------------------------------------------------------------------\
|                             operator==			                             |
\-------------------------------------------------------------------------------*/
bool FMP_VirtualSubPart::operator==(const FMP_VirtualSubPart& other)
{
	if ((unk_2 != other.unk_2) ||
		(unk_2b != other.unk_2b) ||
		(unk_4 != other.unk_4) ||
		(unk_5 != other.unk_5) ||
		(listIndex.size() != other.listIndex.size()))
		return false;

	if (((sub0 == 0) && (other.sub0 != 0)) ||
		((sub0 != 0) && (other.sub0 == 0)) ||
		((sub0 != 0) && (*sub0 != *other.sub0)))
		return false;

	if (((sub1 == 0) && (other.sub1 != 0)) ||
		((sub1 != 0) && (other.sub1 == 0)) ||
		((sub1 != 0) && (*sub1 != *other.sub1)))
		return false;

	if (((action == 0) && (other.action != 0)) ||
		((action != 0) && (other.action == 0)) ||
		((action != 0) && (*action != *other.action)))
		return false;

	for (size_t i = 0; i < 12; i++)
		if (transformMatrix4x3[i] != other.transformMatrix4x3[i])
			return false;

	size_t nbElement = listIndex.size();
	for (size_t i = 0; i < nbElement; i++)
		if ((listIndex.at(i).at(0) != other.listIndex.at(i).at(0)) || (listIndex.at(i).at(1) != other.listIndex.at(i).at(1)))
			return false;

	return true;
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FMP_VirtualSubPart_sub::Reset()
{
	unk_0 = 4;
	unk_0b = 0xffff;
	unk_1 = 0;
	unk_1b = 2;
	unk_2 = 0xffffffff;
	unk_3 = 1.0f;
	unk_4 = 0.0f;
	unk_5 = 0.0f;
	unk_6 = 0.0f;
	unk_7 = 0.0f;
	unk_8 = 0.0f;
	unk_9 = 0.0f;
	unk_10 = 0.0f;
	unk_11 = 0.0f;
	unk_12 = 0.0f;
	unk_13 = 1.0f;
	widthX = 100.0f;
	widthY = 100.0f;
	widthZ = 100.0f;
	quaternionX = 0.0f;
	quaternionY = 0.0f;
	quaternionZ = 0.0f;
	quaternionW = 1.0f;
}
/*-------------------------------------------------------------------------------\
|                             operator==			                             |
\-------------------------------------------------------------------------------*/
bool FMP_VirtualSubPart_sub::operator==(const FMP_VirtualSubPart_sub& other)
{
	return ((unk_0 == other.unk_0) &&
		(unk_0b == other.unk_0b) &&
		(unk_1 == other.unk_1) &&
		(unk_1b == other.unk_1b) &&
		(unk_2 == other.unk_2) &&
		(unk_3 == other.unk_3) &&
		(unk_4 == other.unk_4) &&
		(unk_5 == other.unk_5) &&
		(unk_6 == other.unk_6) &&
		(unk_7 == other.unk_7) &&
		(unk_8 == other.unk_8) &&
		(unk_9 == other.unk_9) &&
		(unk_10 == other.unk_10) &&
		(unk_11 == other.unk_11) &&
		(unk_12 == other.unk_12) &&
		(unk_13 == other.unk_13) &&
		(widthX == other.widthX) &&
		(widthY == other.widthY) &&
		(widthZ == other.widthZ) &&
		(quaternionX == other.quaternionX) &&
		(quaternionY == other.quaternionY) &&
		(quaternionZ == other.quaternionZ) &&
		(quaternionW == other.quaternionW));
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FMP_HitboxGroup::Reset()
{
	listHitbox.clear();
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FMP_Hitbox::Reset()
{
	name = "";

	child = 0xffff;
	unk_a0 = 0xffff;
	sibling = 0xffff;
	parent = 0xffff;

	listOnDestruction.clear();
	listHavokFilesIndex.clear();
	listVertex.clear();
	listFaceIndex.clear();
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FMP_Destruction::Reset()
{
	unk_0 = 0;
	unk_1 = 0;
	unk_2 = 0;
	unk_5 = 0;
	unk_6 = 0;
	unk_7 = 0.01f;

	haveSub0 = false;
	sub0.Reset();

	haveSub1 = false;
	sub1.Reset();

	havokFileIndex = (size_t)-1;
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FMP_Destruction_sub::Reset()
{
	unk_0 = 1;
	unk_1 = 0.0f;
	unk_2 = 0.0f;
	unk_3 = 0.0f;
	yaw = 0.0f;
	pitch = 0.0f;
	roll = 0.0f;
	unk_7 = 0.5f;
	unk_8 = 0.5f;
	unk_9 = 0.5f;
	unk_10 = 0.0f;
	unk_11 = 0.0f;
	unk_12 = 0.0f;
	unk_13 = 0.0f;
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void Havok_File::Reset()
{
	if (buffer)
		free(buffer);
	buffer = 0;
	size = 0;
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void FMP_Action::Reset()
{
	unk_0 = 0;
	unk_1a = 2;
	unk_1b = 0;
	unk_1c = 0;
	unk_1d = 0;

	listCommands.clear();

	debug_startOffset = 0;
}
/*-------------------------------------------------------------------------------\
|                             operator==			                             |
\-------------------------------------------------------------------------------*/
bool FMP_Action::operator==(const FMP_Action& other)
{
	if ((this->unk_0 != other.unk_0) ||
		(this->unk_1a != other.unk_1a) ||
		(this->unk_1b != other.unk_1b) ||
		(this->unk_1c != other.unk_1c) ||
		(this->unk_1d != other.unk_1d) ||
		(this->listCommands.size() != other.listCommands.size()))
		return false;

	size_t nbElements = listCommands.size();
	for (size_t i = 0; i < nbElements; i++)
		if (listCommands.at(i) != other.listCommands.at(i))
			return false;

	return true;
}
/*-------------------------------------------------------------------------------\
|                             operator==			                             |
\-------------------------------------------------------------------------------*/
bool FMP_Action::Command::operator==(const Command& other)
{
	if ((functionName != other.functionName) ||
		(unk_1a != other.unk_1a) ||
		(unk_1b != other.unk_1b) ||
		(unk_1c != other.unk_1c) ||
		(unk_1d != other.unk_1d) ||
		(parameters.size() != other.parameters.size()))
		return false;

	size_t nbElements = parameters.size();
	for (size_t i = 0; i < nbElements; i++)
		if (parameters.at(i) != other.parameters.at(i))
			return false;
	return true;
}
/*-------------------------------------------------------------------------------\
|                             operator==			                             |
\-------------------------------------------------------------------------------*/
bool FMP_Parameter::operator==(const FMP_Parameter& other)
{
	if ((name != other.name) ||
		(typeValue != other.typeValue) ||
		((typeValue == TV_Bool) && (boolValue != other.boolValue)) ||
		((typeValue == TV_Int) && (intValue != other.intValue)) ||
		((typeValue == TV_Float) && (floatValue != other.floatValue)) ||
		((typeValue == TV_String) && (stringValue != other.stringValue)) ||
		((typeValue == TV_Uint) && (uintValue != other.uintValue))  )
		return false;

	if (typeValue == TV_Direction)
	{
		for (size_t i = 0; i < 4; i++)
			if (directionValue[i] != other.directionValue[i])
				return false;
	}
	if (typeValue == TV_Position)
	{
		for (size_t i = 0; i < 4; i++)
			if (positionValue[i] != other.positionValue[i])
				return false;
	}
	return true;
}












/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool FmpFile::Load(const uint8_t *buf, size_t size)
{
	Reset();

	
	FMP_Header *hdr = (FMP_Header *)buf;

	if (size < sizeof(FMP_Header) || memcmp(hdr->signature, FMP_SIGNATURE, 4) != 0)
		return false;


	uint32_t* name_offset;

	//section Section0a:
	if (hdr->offset_Section0a)
	{
		FMP_Section0a* section0a = (FMP_Section0a*)GetOffsetPtr(buf, hdr->offset_Section0a);

		width_X = section0a->width_X;
		width_Y = section0a->width_Y;
		width_Z = section0a->width_Z;

		s0a_unk_3 = section0a->unk_3;
		s0a_unk_4 = section0a->unk_4;
		s0a_unk_5 = section0a->unk_5;

		s0a_unk_6 = section0a->unk_6;
		s0a_unk_7 = section0a->unk_7;

		s0a_unk_8 = section0a->unk_8;
		s0a_unk_9 = section0a->unk_9;
		s0a_unk_10 = section0a->unk_10;

		s0a_unk_11 = section0a->unk_11;
		s0a_unk_12 = section0a->unk_12;
		s0a_unk_13 = section0a->unk_13;

		s0a_unk_14 = section0a->unk_14;
		s0a_unk_15 = section0a->unk_15;
		s0a_unk_16 = section0a->unk_16;

		s0a_unk_17 = section0a->unk_17;
		s0a_unk_18 = section0a->unk_18;

		s0a_unk_19 = section0a->unk_19;
		s0a_unk_20 = section0a->unk_20;
		s0a_unk_21 = section0a->unk_21;

		s0a_unk_22 = section0a->unk_22;
		s0a_unk_23 = section0a->unk_23;
		s0a_unk_24 = section0a->unk_24;

		s0a_unk_25 = section0a->unk_25;
		s0a_unk_26 = section0a->unk_26;
		s0a_unk_27 = section0a->unk_27;
		s0a_unk_28 = section0a->unk_28;

		s0a_unk_29 = section0a->unk_29;
		s0a_unk_30 = section0a->unk_30;
		s0a_unk_31 = section0a->unk_31;

		nearDistance = section0a->nearDistance;
		farDistance = section0a->farDistance;
		s0a_unk_34 = section0a->unk_34;
	}



	//section Section0b:
	if (hdr->offset_Section0b)
	{
		FMP_Section0b* section0b = (FMP_Section0b*)GetOffsetPtr(buf, hdr->offset_Section0b);

		s0b_unk_0 = section0b->unk_0;
		s0b_unk_1 = section0b->unk_1;
		s0b_unk_2 = section0b->unk_2;
		s0b_unk_3 = section0b->unk_3;
		s0b_unk_4 = section0b->unk_4;
		s0b_unk_5 = section0b->unk_5;
		s0b_unk_6 = section0b->unk_6;
		s0b_unk_7 = section0b->unk_7;
		s0b_unk_8 = section0b->unk_8;
		s0b_unk_9 = section0b->unk_9;
		s0b_unk_10 = section0b->unk_10;
		s0b_unk_11 = section0b->unk_11;
		s0b_unk_12 = section0b->unk_12;
		s0b_unk_13 = section0b->unk_13;
		s0b_unk_14 = section0b->unk_14;
		s0b_unk_15 = section0b->unk_15;
		s0b_unk_16 = section0b->unk_16;
		s0b_unk_17 = section0b->unk_17;
		s0b_unk_18 = section0b->unk_18;
		s0b_unk_19 = section0b->unk_19;
		s0b_unk_20 = section0b->unk_20;
		s0b_unk_21 = section0b->unk_21;
		s0b_unk_22 = section0b->unk_22;
		s0b_unk_23 = section0b->unk_23;
		s0b_unk_23b = section0b->unk_23b;
		s0b_unk_24 = section0b->unk_24;
		s0b_unk_25 = section0b->unk_25;
		s0b_unk_26 = section0b->unk_26;
		s0b_unk_27 = section0b->unk_27;
		s0b_unk_28 = section0b->unk_28;
		s0b_unk_29 = section0b->unk_29;
		s0b_unk_30 = section0b->unk_30;
		s0b_unk_31 = section0b->unk_31;
		s0b_unk_32 = section0b->unk_32;
		s0b_unk_33 = section0b->unk_33;
		s0b_unk_34 = section0b->unk_34;
		s0b_unk_35 = section0b->unk_35;
		s0b_unk_36 = section0b->unk_36;
		s0b_unk_37 = section0b->unk_37;
		s0b_unk_38 = section0b->unk_38;
		s0b_unk_39 = section0b->unk_39;
		s0b_unk_40 = section0b->unk_40;
		s0b_unk_41 = section0b->unk_41;
		s0b_unk_42 = section0b->unk_42;
		s0b_unk_43 = section0b->unk_43;
		s0b_unk_44 = section0b->unk_44;
		s0b_unk_45 = section0b->unk_45;
		s0b_unk_46 = section0b->unk_46;
		s0b_unk_47 = section0b->unk_47;
		s0b_unk_48 = section0b->unk_48;
		s0b_unk_49 = section0b->unk_49;
		s0b_unk_50 = section0b->unk_50;
		s0b_unk_51 = section0b->unk_51;
		s0b_unk_52 = section0b->unk_52;
		s0b_unk_53 = section0b->unk_53;
		s0b_unk_54 = section0b->unk_54;


		mListTypeName.clear();
		name_offset = (uint32_t*)GetOffsetPtr(buf, hdr->offset_Section0b + 0x100);
		for (size_t i = 0; i < 51; i++)
			mListTypeName.push_back(name_offset[i] ? std::string((char *)GetOffsetPtr(buf, name_offset, i)) : "");
	}



	//Section1
	mListS1.clear();
	if (hdr->offset_Section1)
	{
		FMP_Section1* section1 = (FMP_Section1*)GetOffsetPtr(buf, hdr->offset_Section1);
		for (size_t i = 0; i < hdr->numberSection1; i++)
		{
			mListS1.push_back(FMP_S1());
			FMP_S1 &s1 = mListS1.back();

			s1.name = std::string((char *)GetOffsetPtr(buf, section1[i].name_offset));
			s1.unk_04 = section1[i].unk_04;
			s1.unk_08 = section1[i].unk_08;
			s1.unk_0C = section1[i].unk_0C;
			s1.unk_10 = section1[i].unk_10;
			s1.unk_14 = section1[i].unk_14;
		}
	}


	//Section2
	mListS2.clear();
	if (hdr->offset_Section2)
	{
		FMP_Section2* section2 = (FMP_Section2*)GetOffsetPtr(buf, hdr->offset_Section2);
		for (size_t i = 0; i < hdr->numberSection2; i++)
		{
			mListS2.push_back(FMP_S2());
			FMP_S2 &s2 = mListS2.back();

			s2.name = std::string((char *)GetOffsetPtr(buf, section2[i].name_offset));
			s2.unk_04 = section2[i].unk_04;
			s2.unk_08 = section2[i].unk_08;
			s2.unk_0C = section2[i].unk_0C;
			s2.unk_10 = section2[i].unk_10;
			s2.unk_14 = section2[i].unk_14;
			s2.unk_18 = section2[i].unk_18;
		}
	}




	//Section3
	mListFragmentsGroups.clear();
	if (hdr->offset_Section3)
	{
		FMP_Section3* section3 = (FMP_Section3*)GetOffsetPtr(buf, hdr->offset_Section3);
		for (size_t i = 0; i < hdr->numberSection3; i++)
		{
			mListFragmentsGroups.push_back(FragmentsGroup(""));
			FragmentsGroup &group = mListFragmentsGroups.back();

			group.name = std::string((char *)GetOffsetPtr(buf, section3[i].name_offset));

			int16_t* index = (int16_t*)GetOffsetPtr(buf, section3[i].indexes_offset);
			size_t nbIndex = section3[i].num_indexes;
			for (size_t j = 0; j < nbIndex; j++)
				group.listObjects.push_back(index[j]);
		}
	}


	listNsk.clear();
	if (hdr->offset_Dependance_Nsk)
	{
		name_offset = (uint32_t*)GetOffsetPtr(buf, hdr->offset_Dependance_Nsk);
		for (size_t i = 0; i < hdr->numberFileInDependance_Nsk; i++)
			listNsk.push_back(std::string((char *)GetOffsetPtr(buf, name_offset[i])));
	}

	listTextureEmb.clear();
	if (hdr->offset_Dependance_TextureEmb)
	{
		name_offset = (uint32_t*)GetOffsetPtr(buf, hdr->offset_Dependance_TextureEmb);
		for (size_t i = 0; i < hdr->numberFileInDependance_TextureEmb; i++)
			listTextureEmb.push_back(std::string((char *)GetOffsetPtr(buf, name_offset[i])));
	}

	listEmm.clear();
	if (hdr->offset_Dependance_Emm)
	{
		name_offset = (uint32_t*)GetOffsetPtr(buf, hdr->offset_Dependance_Emm);
		for (size_t i = 0; i < hdr->numberFileInDependance_Emm; i++)
			listEmm.push_back(std::string((char *)GetOffsetPtr(buf, name_offset[i])));
	}

	listEma.clear();
	if (hdr->offset_Dependance_Ema)
	{
		name_offset = (uint32_t*)GetOffsetPtr(buf, hdr->offset_Dependance_Ema);
		for (size_t i = 0; i < hdr->numberFileInDependance_Ema; i++)
			listEma.push_back(std::string((char *)GetOffsetPtr(buf, name_offset[i])));
	}


	mListHitboxGroup.clear();
	if (hdr->offset_Section5)
	{
		for (size_t i = 0; i < hdr->numberSection5; i++)
		{
			FMP_HitboxGroup group;
			if (group.Load(buf, size, hdr->offset_Section5 + i * sizeof(FMP_Section5), hdr->version, mListHavokFile))
				mListHitboxGroup.push_back(group);
		}
	}


	mListObject.clear();
	if (hdr->offset_Section4)
	{
		for (size_t i = 0; i < hdr->numberSection4; i++)
		{
			FMP_Section4* section4 = (FMP_Section4*)(buf + hdr->offset_Section4 + i * sizeof(FMP_Section4));
			size_t nb_Section4_Next = 0;

			if ((section4->index_Section5 != 0xffff) && (section4->index_Section5 < mListHitboxGroup.size()))
			{
				FMP_Section5* section5 = (FMP_Section5*)(buf + hdr->offset_Section5 + section4->index_Section5 * sizeof(FMP_Section5));
				nb_Section4_Next = (section5->offset_Section5_Hitbox) ? section5->number_Section5_Hitbox : 0;
			}

			FMP_Object* obj = new FMP_Object();
			if (obj->Load(buf, size, hdr->offset_Section4 + i * sizeof(FMP_Section4), nb_Section4_Next, listNsk, listTextureEmb, listEmm, listEma))
				mListObject.push_back(obj);
			else
				delete obj;
		}
	}

	

	return true;
}

/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool FMP_Object::Load(const uint8_t *buf, size_t size, size_t offset_Section4, size_t nb_Section4_Next, std::vector<string> &listNskFile, std::vector<string> &listTextureEmbFile, std::vector<string> &listEmmFile, std::vector<string> &listEmaFile)
{
	Reset();

	debug_startOffset = offset_Section4;

	FMP_Section4* section4 = (FMP_Section4*)(buf + offset_Section4);
	name = std::string((char *)(buf + section4->name_offset));

	unk_04 = section4->unk_04;
	unk_04b = section4->unk_04b;
	unk_0A = section4->unk_0A;
	unk_0B = section4->unk_0B;
	unk_20 = section4->unk_20;

	hitboxGroupIndex = (section4->index_Section5 != 0xffff) ? section4->index_Section5 : (size_t)-1;
	

	for(size_t i=0;i<12;i++)
		transformMatrix4x3[i] = section4->transformMatrix4x3[i];
	

	//Section4_Next
	listVirtualSubParts.clear();
	if (section4->offset_Section4_Next)
	{
		for (size_t i = 0; i < nb_Section4_Next; i++)
		{
			FMP_VirtualSubPart* vsp = new FMP_VirtualSubPart();
			if (vsp->Load(buf, size, section4->offset_Section4_Next + i * sizeof(FMP_Section4_Next)))
				listVirtualSubParts.push_back(vsp);
			else
				delete vsp;
		}
	}

	//Section4_Next3
	if (section4->offset_Section4_Next3)
	{
		action = new FMP_Action();
		action->Load(buf, size, section4->offset_Section4_Next3);
	}


	//Section4_Next2
	listEntity.clear();
	if (section4->offset_Section4_Next2)
	{
		for (size_t i = 0; i < section4->number_Section4_Next2; i++)
		{
			FMP_Entity ent;
			if(ent.Load(buf, size, section4->offset_Section4_Next2 + i * sizeof(FMP_Section4_Next2), listNskFile, listTextureEmbFile, listEmmFile, listEmaFile))
				listEntity.push_back(ent);
		}
	}

	//Section4_Next4
	if (section4->offset_Section4_Next4)
	{
		hierarchy = new FMP_Hierarchy();
		hierarchy->Load(buf, size, section4->offset_Section4_Next4);
	}

	return true;
}












/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool FMP_Hierarchy::Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next4)
{
	Reset();

	debug_startOffset = offset_Section4_Next4;

	FMP_Section4_Next4* section4_Next4 = (FMP_Section4_Next4*)(buf + offset_Section4_Next4);


	listNode.clear();
	if (section4_Next4->offset_Section4_Next4_b)
	{
		for (size_t i = 0; i < section4_Next4->number_Section4_Next4_b; i++)
		{
			FMP_Node node;
			if (node.Load(buf, size, section4_Next4->offset_Section4_Next4_b + i * sizeof(FMP_Section4_Next4_b)))
				listNode.push_back(node);
		}
	}

	listTransform.clear();
	if (section4_Next4->offset_Section4_Next4_c)
	{
		for (size_t i = 0; i < section4_Next4->number_Section4_Next4_c; i++)
		{
			FMP_NodeTransform transf;
			if(transf.Load(buf, size, section4_Next4->offset_Section4_Next4_c + i * sizeof(FMP_Section4_Next4_c)))
				listTransform.push_back(transf);
		}
	}


	if (section4_Next4->offset_Section4_Next4_d)
	{
		mRoot = new FMP_HierarchyNode();
		mRoot->Load(buf, size, section4_Next4->offset_Section4_Next4_d);
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool FMP_NodeTransform::Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next4_c)
{
	Reset();

	FMP_Section4_Next4_c* section4_Next4_c = (FMP_Section4_Next4_c*)(buf + offset_Section4_Next4_c);

	position[0] = section4_Next4_c->positionX;
	position[1] = section4_Next4_c->positionY;
	position[2] = section4_Next4_c->positionZ;
	rotation[0] = section4_Next4_c->rotationX;
	rotation[1] = section4_Next4_c->rotationY;
	rotation[2] = section4_Next4_c->rotationZ;
	scale[0] = section4_Next4_c->scaleX;
	scale[1] = section4_Next4_c->scaleY;
	scale[2] = section4_Next4_c->scaleZ;

	return true;
}
/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool FMP_Node::Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next4_b)
{
	Reset();

	FMP_Section4_Next4_b* section4_Next4_b = (FMP_Section4_Next4_b*)(buf + offset_Section4_Next4_b);

	unk_0 = section4_Next4_b->unk_0;
	unk_1 = section4_Next4_b->unk_1;
	unk_2 = section4_Next4_b->unk_2;
	unk_3 = section4_Next4_b->unk_3;
	unk_4 = section4_Next4_b->unk_4;
	unk_5 = section4_Next4_b->unk_5;
	unk_6 = section4_Next4_b->unk_6;
	unk_7 = section4_Next4_b->unk_7;
	unk_8 = section4_Next4_b->unk_8;
	unk_9 = section4_Next4_b->unk_9;
	unk_10 = section4_Next4_b->unk_10;
	transformIndex = (section4_Next4_b->indexToNext4_c_Transform != 0xffffffff) ? section4_Next4_b->indexToNext4_c_Transform : (size_t)-1;

	return true;
}
/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool FMP_HierarchyNode::Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next4_d)
{
	Reset();

	FMP_Section4_Next4_d* section4_Next4_d = (FMP_Section4_Next4_d*)(buf + offset_Section4_Next4_d);

	typeb = section4_Next4_d->typeb;

	if (section4_Next4_d->type == 1)					//last child in hierarchy
	{
		uint16_t* index = (uint16_t*)(buf + offset_Section4_Next4_d + sizeof(uint32_t));

		listIndex.clear();
		for (size_t i = 0; i < section4_Next4_d->number_Index; i++)
			listIndex.push_back(index[i]);

	}else {

		FMP_Section4_Next4_e* section4_Next4_e = (FMP_Section4_Next4_e*)(buf + offset_Section4_Next4_d + sizeof(uint32_t));

		unk_0 = section4_Next4_e->unk_0;
		unk_1 = section4_Next4_e->unk_1;
		unk_2 = section4_Next4_e->unk_2;

		bool haveChildUp = false;										//because it's could have holes

		if (section4_Next4_e->unk7_offset)
		{
			FMP_HierarchyNode* node = new FMP_HierarchyNode();
			if (node->Load(buf, size, section4_Next4_e->unk7_offset))
			{
				listChild.insert(listChild.begin(), node);
				haveChildUp = true;
			}else {
				delete node;
			}
		}else if (haveChildUp)
			listChild.insert(listChild.begin(), 0);


		if (section4_Next4_e->unk6_offset)
		{
			FMP_HierarchyNode* node = new FMP_HierarchyNode();
			if (node->Load(buf, size, section4_Next4_e->unk6_offset))
			{
				listChild.insert(listChild.begin(), node);
				haveChildUp = true;
			}else {
				delete node;
			}
		}else if (haveChildUp)
			listChild.insert(listChild.begin(), 0);

		if (section4_Next4_e->unk5_offset)
		{
			FMP_HierarchyNode* node = new FMP_HierarchyNode();
			if (node->Load(buf, size, section4_Next4_e->unk5_offset))
			{
				listChild.insert(listChild.begin(), node);
				haveChildUp = true;
			}else {
				delete node;
			}
		}else if (haveChildUp)
			listChild.insert(listChild.begin(), 0);


		if (section4_Next4_e->unk4_offset)
		{
			FMP_HierarchyNode* node = new FMP_HierarchyNode();
			if (node->Load(buf, size, section4_Next4_e->unk4_offset))
			{
				listChild.insert(listChild.begin(), node);
				haveChildUp = true;
			}else {
				delete node;
			}
		}else if (haveChildUp)
			listChild.insert(listChild.begin(), 0);


		if (section4_Next4_e->unk3_offset)
		{
			FMP_HierarchyNode* node = new FMP_HierarchyNode();
			if (node->Load(buf, size, section4_Next4_e->unk3_offset))
			{
				listChild.insert(listChild.begin(), node);
				haveChildUp = true;
			}else {
				delete node;
			}
		}else if (haveChildUp)
			listChild.insert(listChild.begin(), 0);

		if (section4_Next4_e->unk2_offset)
		{
			FMP_HierarchyNode* node = new FMP_HierarchyNode();
			if (node->Load(buf, size, section4_Next4_e->unk2_offset))
			{
				listChild.insert(listChild.begin(), node);
				haveChildUp = true;
			}else {
				delete node;
			}
		}else if (haveChildUp)
			listChild.insert(listChild.begin(), 0);


		if (section4_Next4_e->unk1_offset)
		{
			FMP_HierarchyNode* node = new FMP_HierarchyNode();
			if (node->Load(buf, size, section4_Next4_e->unk1_offset))
			{
				listChild.insert(listChild.begin(), node);
				haveChildUp = true;
			}else {
				delete node;
			}
		}else if (haveChildUp)
			listChild.insert(listChild.begin(), 0);


		if (section4_Next4_e->unk0_offset)
		{
			FMP_HierarchyNode* node = new FMP_HierarchyNode();
			if (node->Load(buf, size, section4_Next4_e->unk0_offset))
			{
				listChild.insert(listChild.begin(), node);
				haveChildUp = true;
			}else {
				delete node;
			}
		}else if (haveChildUp)
			listChild.insert(listChild.begin(), 0);
	}

	return true;
}











/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool FMP_Action::Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next3)
{
	Reset();

	debug_startOffset = offset_Section4_Next3;

	FMP_Section4_Next3* section4_Next3 = (FMP_Section4_Next3*)(buf + offset_Section4_Next3);


	unk_0 = section4_Next3->unk_0;
	unk_1a = section4_Next3->unk_1a;
	unk_1b = section4_Next3->unk_1b;
	unk_1c = section4_Next3->unk_1c;
	unk_1d = section4_Next3->unk_1d;


	listCommands.clear();
	if (section4_Next3->offset_Section4_Next3_b)
	{
		FMP_Section4_Next3_b* section4_Next3_b = (FMP_Section4_Next3_b*)(buf + section4_Next3->offset_Section4_Next3_b);
		
		for (size_t i = 0; i < section4_Next3->number_Section4_Next3_b; i++)
		{
			Command command("");
			
			command.functionName = std::string((char *)(buf + section4_Next3_b[i].name_offset));
			command.unk_1a = section4_Next3_b[i].unk_1a;
			command.unk_1b = section4_Next3_b[i].unk_1b;
			command.unk_1c = section4_Next3_b[i].unk_1c;
			command.unk_1d = section4_Next3_b[i].unk_1d;

			if (section4_Next3_b[i].offset_Section4_Next_f)
			{
				FMP_Section4_Next_f* section4_Next_f = (FMP_Section4_Next_f*)(buf + section4_Next3_b[i].offset_Section4_Next_f);
				for (size_t j = 0; j < section4_Next3_b[i].number_Section4_Next_f; j++)
				{
					FMP_Parameter param;

					param.name = std::string((char *)(buf + section4_Next_f[j].name_offset));
					param.typeValue = (FMP_Parameter::TypeValue)(section4_Next_f[j].type_Data);
					
					if (section4_Next_f[j].offset_Data)
					{
						switch (param.typeValue)
						{
						case 0: param.boolValue = ( (*(uint32_t*)(buf + section4_Next_f[j].offset_Data)) != 0); break;
						case 1: param.intValue = *(int*)(buf + section4_Next_f[j].offset_Data); break;
						case 2: param.floatValue = *(float*)(buf + section4_Next_f[j].offset_Data); break;
						case 6: param.stringValue = std::string((char *)(buf + section4_Next_f[j].offset_Data)); break;
						case 8: param.uintValue = *(uint32_t*)(buf + section4_Next_f[j].offset_Data); break;
						
						case 3: 
						{
							for (size_t k = 0; k < 4; k++)
								param.directionValue[k] = *(float*)(buf + section4_Next_f[j].offset_Data + k * sizeof(uint32_t) );
						}
						break;

						case 4:
						{
							for (size_t k = 0; k < 4; k++)
								param.positionValue[k] = *(float*)(buf + section4_Next_f[j].offset_Data + k * sizeof(uint32_t));
						}
						break;
						
						}
					}

					command.parameters.push_back(param);
				}
			}

			listCommands.push_back(command);
		}
	}

	return true;
}











/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool FMP_VirtualSubPart::Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next)
{
	Reset();

	debug_startOffset = offset_Section4_Next;

	FMP_Section4_Next* section4_Next = (FMP_Section4_Next*)(buf + offset_Section4_Next);

	unk_2 = section4_Next->unk_2;
	unk_2b = section4_Next->unk_2b;
	unk_4 = section4_Next->unk_4;
	unk_5 = section4_Next->unk_5;

	for (size_t i = 0; i<12; i++)
		transformMatrix4x3[i] = section4_Next->transformMatrix4x3[i];


	listIndex.clear();
	if (section4_Next->offset_Section4_Next_b)
	{
		FMP_Section4_Next_b* section4_Next_b = (FMP_Section4_Next_b*)(buf + section4_Next->offset_Section4_Next_b);
		for (size_t i = 0; i < section4_Next->number_Section4_Next_b; i++)
		{
			listIndex.push_back(std::vector<size_t>());
			listIndex.back().push_back(section4_Next_b[i].unk_0);
			listIndex.back().push_back(section4_Next_b[i].unk_1);
		}
	}


	if (section4_Next->offset_Section4_Next_c)
	{
		sub0 = new FMP_VirtualSubPart_sub();
		sub0->Load(buf, size, section4_Next->offset_Section4_Next_c);
	}
	if (section4_Next->offset_Section4_Next_c_2)
	{
		sub1 = new FMP_VirtualSubPart_sub();
		sub1->Load(buf, size, section4_Next->offset_Section4_Next_c_2);
	}
	

	if (section4_Next->offset_Section4_Next_d)
	{
		action = new FMP_Action();
		action->Load(buf, size, section4_Next->offset_Section4_Next_d);
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool FMP_VirtualSubPart_sub::Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next_c)
{
	Reset();

	FMP_Section4_Next_c* section4_Next_c = (FMP_Section4_Next_c*)(buf + offset_Section4_Next_c);

	unk_0 = section4_Next_c->unk_0;
	unk_0b = section4_Next_c->unk_0b;
	unk_1 = section4_Next_c->unk_1;
	unk_1b = section4_Next_c->unk_1b;
	unk_2 = section4_Next_c->unk_2;
	unk_3 = section4_Next_c->unk_3;
	unk_4 = section4_Next_c->unk_4;
	unk_5 = section4_Next_c->unk_5;
	unk_6 = section4_Next_c->unk_6;
	unk_7 = section4_Next_c->unk_7;
	unk_8 = section4_Next_c->unk_8;
	unk_9 = section4_Next_c->unk_9;
	unk_10 = section4_Next_c->unk_10;
	unk_11 = section4_Next_c->unk_11;
	unk_12 = section4_Next_c->unk_12;
	unk_13 = section4_Next_c->unk_13;
	widthX = section4_Next_c->widthX;
	widthY = section4_Next_c->widthY;
	widthZ = section4_Next_c->widthZ;
	quaternionX = section4_Next_c->quaternionX;
	quaternionY = section4_Next_c->quaternionY;
	quaternionZ = section4_Next_c->quaternionZ;
	quaternionW = section4_Next_c->quaternionW;

	return true;
}












/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool FMP_Entity::Load(const uint8_t *buf, size_t size, size_t offset_Section4_Next2, std::vector<string> &listNskFile, std::vector<string> &listTextureEmbFile, std::vector<string> &listEmmFile, std::vector<string> &listEmaFile)
{
	Reset();

	debug_startOffset = offset_Section4_Next2;

	FMP_Section4_Next2* section4_Next2 = (FMP_Section4_Next2*)(buf + offset_Section4_Next2);



	s4n2_unk_0 = section4_Next2->unk_0;
	for (size_t i = 0; i<12; i++)
		transformMatrix4x3[i] = section4_Next2->transformMatrix4x3[i];



	if (section4_Next2->offset_Section4_Next2_b)
	{
		FMP_Section4_Next2_b* section4_Next2_b = (FMP_Section4_Next2_b*)(buf + section4_Next2->offset_Section4_Next2_b);

		haveVisualPart = true;
		name = std::string((char *)(buf + section4_Next2_b->name_offset));

		if ((section4_Next2_b->indexTextureEmb != 0xffffffff) && (section4_Next2_b->indexTextureEmb < listTextureEmbFile.size()))
			embTexture_filename = listTextureEmbFile.at(section4_Next2_b->indexTextureEmb);

		if ((section4_Next2_b->indexEma != 0xffffffff) && (section4_Next2_b->indexEma < listEmaFile.size()))
			ema_filename = listEmaFile.at(section4_Next2_b->indexEma);

		s4n2b_unk_0 = section4_Next2_b->unk_0;
		s4n2b_unk_5 = section4_Next2_b->unk_5;
		s4n2b_unk_6 = section4_Next2_b->unk_6;
		s4n2b_unk_8 = section4_Next2_b->unk_8;
		s4n2b_unk_9 = section4_Next2_b->unk_9;
		s4n2b_unk_10 = section4_Next2_b->unk_10;

		if (section4_Next2_b->number_Lod == 1)
		{
			Lod lod("", "", 0.0f);
			
			if ((section4_Next2_b->indexNsk != 0xffffffff) && (section4_Next2_b->indexNsk < listNskFile.size()))
				lod.nsk_filename = listNskFile.at(section4_Next2_b->indexNsk);
			if ((section4_Next2_b->indexEmm != 0xffffffff) && (section4_Next2_b->indexEmm < listEmmFile.size()))
				lod.emm_filename = listEmmFile.at(section4_Next2_b->indexEmm);

			lod.distance = section4_Next2_b->lodDistance;

			listLod.push_back(lod);

		}else {

			uint32_t* indexNsk = (section4_Next2_b->indexNsk) ?  (uint32_t*)(buf + section4_Next2_b->indexNsk) : 0;
			uint32_t* indexEmm = (section4_Next2_b->indexEmm) ? (uint32_t*)(buf + section4_Next2_b->indexEmm) : 0;
			float* lodDistance = (section4_Next2_b->lodDistance) ? (float*)(buf + (*((uint32_t*)&section4_Next2_b->lodDistance)) ) : 0;

			for (size_t i = 0; i < section4_Next2_b->number_Lod; i++)
			{
				Lod lod("", "", 0.0f);
				
				if ((indexNsk[i] != 0xffffffff) && (indexNsk[i] < listNskFile.size()))
					lod.nsk_filename = listNskFile.at(indexNsk[i]);
				if ((indexEmm[i] != 0xffffffff) && (indexEmm[i] < listEmmFile.size()))
					lod.emm_filename = listEmmFile.at(indexEmm[i]);
				lod.distance = lodDistance[i];

				listLod.push_back(lod);
			}

		}
	}

	return true;
}













/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool FMP_HitboxGroup::Load(const uint8_t *buf, size_t size, size_t offset_section5, size_t version, std::vector<Havok_File*> &mListHavokFile)
{
	Reset();

	FMP_Section5* section5 = (FMP_Section5*)(buf + offset_section5);
	name = std::string((char *)(buf + section5->name_offset));


	if (section5->offset_Section5_Hitbox)
	{
		bool isOldVersion = ((version & 0xFFF00) == 0x20800); //here there is 3 versions find in current files, but only 2 cases is to considere :
		FMP_Section5_Hitbox* section5_Hitbox = isOldVersion ? (new FMP_Section5_Hitbox()) : 0;
		size_t sectionSize = ((!isOldVersion) ? sizeof(FMP_Section5_Hitbox) : sizeof(FMP_Section5_Hitbox_Old_V0x0x208XX));

		for (size_t i = 0; i < section5->number_Section5_Hitbox; i++)
		{
			size_t start_Section5_HitBox = section5->offset_Section5_Hitbox + i * sectionSize;

			if (!isOldVersion)
			{
				section5_Hitbox = (FMP_Section5_Hitbox*)(buf + start_Section5_HitBox);
			}
			else {
				FMP_Section5_Hitbox_Old_V0x0x208XX* section5_Hitbox_Old = (FMP_Section5_Hitbox_Old_V0x0x208XX*)(buf + start_Section5_HitBox);

				section5_Hitbox->name_offset = section5_Hitbox_Old->name_offset;
				section5_Hitbox->child = section5_Hitbox_Old->child;
				section5_Hitbox->unk_a0 = section5_Hitbox_Old->unk_a0;
				section5_Hitbox->sibling = section5_Hitbox_Old->sibling;
				section5_Hitbox->parent = section5_Hitbox_Old->parent;
				section5_Hitbox->number_Section5_Hitbox_b = section5_Hitbox_Old->number_Section5_Hitbox_b;
				section5_Hitbox->offset_Section5_Hitbox_b = section5_Hitbox_Old->offset_Section5_Hitbox_b;
				section5_Hitbox->offset_Section5_Hitbox_f = 0;
				section5_Hitbox->number_Section5_Hitbox_e = section5_Hitbox_Old->number_Section5_Hitbox_e;
				section5_Hitbox->offset_Section5_Hitbox_e = section5_Hitbox_Old->offset_Section5_Hitbox_e;
				section5_Hitbox->number_Indexes = section5_Hitbox_Old->number_Indexes;
				section5_Hitbox->offset_Indexes = section5_Hitbox_Old->offset_Indexes;
			}

			FMP_Hitbox hitbox;
			if (hitbox.Load(buf, size, section5_Hitbox, mListHavokFile))
				listHitbox.push_back(hitbox);
		}
		if (isOldVersion)
			delete section5_Hitbox;
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool FMP_Hitbox::Load(const uint8_t *buf, size_t size, FMP_Section5_Hitbox* section5_hitbox, std::vector<Havok_File*> &mListHavokFile)
{
	Reset();

	name = std::string((char *)(buf + section5_hitbox->name_offset));

	child = section5_hitbox->child;
	unk_a0 = section5_hitbox->unk_a0;
	sibling = section5_hitbox->sibling;
	parent = section5_hitbox->parent;


	listOnDestruction.clear();
	if (section5_hitbox->offset_Section5_Hitbox_b)
	{
		FMP_Section5_Hitbox_b* section5_Hitbox_b = (FMP_Section5_Hitbox_b*)(buf + section5_hitbox->offset_Section5_Hitbox_b);
		
		for (size_t i = 0; i < section5_hitbox->number_Section5_Hitbox_b; i++)
		{
			listOnDestruction.push_back(std::vector<FMP_Destruction>());

			if (section5_Hitbox_b[i].offset_Section5_Hitbox_c)
			{
				for (size_t j = 0; j < section5_Hitbox_b[i].number_Section5_Hitbox_c; j++)
				{
					FMP_Destruction des;
					if (des.Load(buf, size, section5_Hitbox_b[i].offset_Section5_Hitbox_c + j * sizeof(FMP_Section5_Hitbox_c), mListHavokFile))
						listOnDestruction.back().push_back(des);
				}
			}
		}
	}


	listHavokFilesIndex.clear();
	if (section5_hitbox->offset_Section5_Hitbox_f)
	{
		FMP_Section5_Hitbox_f* section5_Hitbox_f = (FMP_Section5_Hitbox_f*)(buf + section5_hitbox->offset_Section5_Hitbox_f);
		if ((section5_Hitbox_f->offset_Section5_File) && (section5_Hitbox_f->size_Section5_File))
		{
			for (size_t i = 0; i < section5_Hitbox_f->unk_0; i++)
			{
				Havok_File* file = new Havok_File();
				if (file->Load(buf, size, section5_Hitbox_f->offset_Section5_File + i * section5_Hitbox_f->size_Section5_File, section5_Hitbox_f->size_Section5_File))
				{
					listHavokFilesIndex.push_back(mListHavokFile.size());
					mListHavokFile.push_back(file);
				}else {
					delete file;
				}
			}
		}
	}

	
	//definition of a geometry for colisions
	debug_startOffset_vertex = section5_hitbox->offset_Section5_Hitbox_e;
	debug_startOffset_faces = section5_hitbox->offset_Indexes;

	listVertex.clear();
	if(section5_hitbox->offset_Section5_Hitbox_e)
	{
		FMP_Section5_Hitbox_e* section5_Hitbox_e = (FMP_Section5_Hitbox_e*)(buf + section5_hitbox->offset_Section5_Hitbox_e);
		for (size_t i = 0; i < section5_hitbox->number_Section5_Hitbox_e; i++)
			listVertex.push_back(Vertex(section5_Hitbox_e[i].unk_0, section5_Hitbox_e[i].unk_1, section5_Hitbox_e[i].unk_2, section5_Hitbox_e[i].unk_3, section5_Hitbox_e[i].unk_4, section5_Hitbox_e[i].unk_5));
	}

	listFaceIndex.clear();
	if ((section5_hitbox->offset_Indexes)&&(section5_hitbox->number_Indexes))
	{
		uint16_t* index = (uint16_t*)(buf + section5_hitbox->offset_Indexes);
		for (size_t i = 0; i < section5_hitbox->number_Indexes; i++)
			listFaceIndex.push_back(index[i]);
	}


	return true;
}
/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool FMP_Destruction::Load(const uint8_t *buf, size_t size, size_t offset_Section5_Hitbox_c, std::vector<Havok_File*> &mListHavokFile)
{
	Reset();

	FMP_Section5_Hitbox_c* section5_Hitbox_c = (FMP_Section5_Hitbox_c*)(buf + offset_Section5_Hitbox_c);


	unk_0 = section5_Hitbox_c->unk_0;
	unk_1 = section5_Hitbox_c->unk_1;
	unk_2 = section5_Hitbox_c->unk_2;
	unk_5 = section5_Hitbox_c->unk_5;
	unk_6 = section5_Hitbox_c->unk_6;
	unk_7 = section5_Hitbox_c->unk_7;


	if (section5_Hitbox_c->offset_Section5_Hitbox_d)
	{
		if(sub0.Load(buf, size, section5_Hitbox_c->offset_Section5_Hitbox_d))
			haveSub0 = true;
	}
	if (section5_Hitbox_c->offset_Section5_Hitbox_d_2)
	{
		if (sub1.Load(buf, size, section5_Hitbox_c->offset_Section5_Hitbox_d_2))
			haveSub1 = true;
	}

	if ((section5_Hitbox_c->offset_Section5_File) && (section5_Hitbox_c->size_Section5_File))
	{

		havokFileIndex = mListHavokFile.size();
		Havok_File* havokFile = new Havok_File();
		havokFile->Load(buf, size, section5_Hitbox_c->offset_Section5_File, section5_Hitbox_c->size_Section5_File);
		mListHavokFile.push_back(havokFile);
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool FMP_Destruction_sub::Load(const uint8_t *buf, size_t size, size_t offset_Section5_Hitbox_d)
{
	Reset();

	FMP_Section5_Hitbox_d* section5_Hitbox_d = (FMP_Section5_Hitbox_d*)(buf + offset_Section5_Hitbox_d);


	unk_0 = section5_Hitbox_d->unk_0;
	unk_1 = section5_Hitbox_d->unk_1;
	unk_2 = section5_Hitbox_d->unk_2;
	unk_3 = section5_Hitbox_d->unk_3;
	yaw = section5_Hitbox_d->yaw * 180.0f / 3.1415926535897f;						//convert in degrees
	pitch = section5_Hitbox_d->pitch * 180.0f / 3.1415926535897f;
	roll = section5_Hitbox_d->roll * 180.0f / 3.1415926535897f;
	unk_7 = section5_Hitbox_d->unk_7;
	unk_8 = section5_Hitbox_d->unk_8;
	unk_9 = section5_Hitbox_d->unk_9;
	unk_10 = section5_Hitbox_d->unk_10;
	unk_11 = section5_Hitbox_d->unk_11;
	unk_12 = section5_Hitbox_d->unk_12;
	unk_13 = section5_Hitbox_d->unk_13;

	return true;
}
/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool Havok_File::Load(const uint8_t *buf, size_t size, size_t offset_Section5_File, size_t size_Section5_File)
{
	Reset();
	
	buffer = (uint8_t*)malloc(size_Section5_File);
	memcpy(buffer, buf + offset_Section5_File, size_Section5_File);
	this->size = size_Section5_File;

	return true;
}
















/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*-------------------------------------------------------------------------------\
|                             calculeFilesize		                             |
\-------------------------------------------------------------------------------*/
size_t FmpFile::calculeFilesize()
{
	struct NameLater								//for keep the information of the uint32 position in buf to update the offset of a name when is posed
	{
		string name;
		std::vector<size_t> listPosInBufffer;
		NameLater(string name, size_t posInBufffer) { this->name = name; listPosInBufffer.push_back(posInBufffer); }

		static void addNameLater(string name, size_t posInBufffer, std::vector<NameLater> &listAllNames)
		{
			size_t nbNames = listAllNames.size();
			for (size_t i = 0; i < nbNames; i++)
			{
				if (listAllNames.at(i).name == name)
				{
					listAllNames.at(i).listPosInBufffer.push_back(posInBufffer);
					return;
				}
			}
			listAllNames.push_back(NameLater(name, posInBufffer));
		};


		static size_t addUniqueString(string str, std::vector<string> &listString)
		{
			size_t nbNames = listString.size();
			for (size_t i = 0; i < nbNames; i++)
				if (listString.at(i) == str)
					return i;

			listString.push_back(str);
			return listString.size() - 1;
		};
	};


	

	

	
	
	size_t filesize = sizeof(FMP_Header) + sizeof(FMP_Section0a) + sizeof(FMP_Section0b) + 36 + mListTypeName.size() * sizeof(uint32_t) + 48 
					  + mListS1.size() * sizeof(FMP_Section1) + mListS2.size() * sizeof(FMP_Section2);

	std::vector<NameLater> listAllNames;

	size_t nbElement = mListTypeName.size();
	for (size_t i = 0; i < nbElement; i++)
		if(mListTypeName.at(i).length()!=0)
			NameLater::addNameLater(mListTypeName.at(i), 0, listAllNames);

	nbElement = mListS1.size();
	for (size_t i = 0; i < nbElement; i++)
		NameLater::addNameLater(mListS1.at(i).name, 0, listAllNames);

	nbElement = mListS2.size();
	for (size_t i = 0; i < nbElement; i++)
		NameLater::addNameLater(mListS2.at(i).name, 0, listAllNames);



	nbElement = mListFragmentsGroups.size();
	filesize += nbElement * sizeof(FMP_Section3);
	for (size_t i = 0; i < nbElement; i++)
	{
		NameLater::addNameLater(mListFragmentsGroups.at(i).name, 0, listAllNames);
		filesize += mListFragmentsGroups.at(i).listObjects.size() * sizeof(uint16_t);
	}
	if (filesize & 0x3)
		filesize += (0x4 - (filesize & 0x3));



	struct DebugTest
	{
		string text;
		size_t offset;
		DebugTest(string text, size_t offset) { this->text = text; this->offset = offset; }
	};
	std::vector<DebugTest> listDebug;



	//Section4
	size_t nbObject = mListObject.size();
	filesize += nbObject * sizeof(FMP_Section4);


	//if all subparts of a Section4 is the same as another, we could use the offset of the other begin suppart, to remove duplicates.
	std::vector<size_t> listOfDuplicate_Entity;
	listOfDuplicate_Entity.resize(nbObject, (size_t)-1);
	std::vector<size_t> listOfDuplicate_Vsp;
	listOfDuplicate_Vsp.resize(nbObject, (size_t)-1);
	std::vector<size_t> listOfDuplicate_Action;
	listOfDuplicate_Action.resize(nbObject, (size_t)-1);
	std::vector<size_t> listOfDuplicate_Hierarchy;
	listOfDuplicate_Hierarchy.resize(nbObject, (size_t)-1);

	for (size_t i = 1; i < nbObject; i++)					//here the check on duplicate. // I notice the "remove duplication" is applied if the 4 parts are the same in the same time. So here we will synchronize the 4 list
	{
		FMP_Object* obj = mListObject.at(i);
		if (obj->unk_0B != 0xffffff80)
			continue;


		size_t nbEnt = obj->listEntity.size();
		size_t nbVsp = obj->listVirtualSubParts.size();

		for (int j = (int)i - 1; j >= 0; j--)
		{
			FMP_Object* obj_b = mListObject.at(j);
			size_t nbEnt_b = obj_b->listEntity.size();
			size_t nbVsp_b = obj_b->listVirtualSubParts.size();

			bool transformEquals = true;
			/*
			for (size_t k = 0; k < 12; k++)
			{
			if (obj->transformMatrix4x3[k] != obj_b->transformMatrix4x3[k])
			{
			transformEquals = false;
			break;
			}
			}
			*/

			if ((!transformEquals) ||
				(nbVsp != nbVsp_b) ||
				(nbEnt != nbEnt_b) ||
				(obj->hitboxGroupIndex != obj_b->hitboxGroupIndex) ||
				((obj->action == 0) && (obj_b->action != 0)) ||
				((obj->action != 0) && (obj_b->action == 0)) ||
				((obj->hierarchy == 0) && (obj_b->hierarchy != 0)) ||
				((obj->hierarchy != 0) && (obj_b->hierarchy == 0)))
				continue;

			bool allEquals = true;
			for (size_t k = 0; k < nbEnt; k++)						//Entity
			{
				if (obj->listEntity.at(k) != obj_b->listEntity.at(k))
				{
					allEquals = false;
					break;
				}
			}
			if (!allEquals)
				continue;

			for (size_t k = 0; k < nbVsp; k++)						//VirtualSubPart
			{
				if (*obj->listVirtualSubParts.at(k) != *obj_b->listVirtualSubParts.at(k))
				{
					allEquals = false;
					break;
				}
			}
			if (!allEquals)
				continue;

			allEquals = ((obj->action == 0) || (*obj->action == *obj_b->action));			//Action
			if (!allEquals)
				continue;

			allEquals = ((obj->hierarchy == 0) || (*obj->hierarchy == *obj_b->hierarchy));
			if (!allEquals)
				continue;

			//we notify that is equal as a previous subpart.
			listOfDuplicate_Entity.at(i) = j;
			listOfDuplicate_Vsp.at(i) = j;
			listOfDuplicate_Action.at(i) = j;
			listOfDuplicate_Hierarchy.at(i) = j;
			break;
		}
	}



	listDebug.push_back(DebugTest("Start after Section4", filesize));

	for (size_t i = 0; i < nbObject; i++)
	{
		listDebug.push_back(DebugTest("*************** Section4["+ std::to_string(i) +"]", filesize));
		
		FMP_Object* obj = mListObject.at(i);
		NameLater::addNameLater(obj->name, 0, listAllNames);

		if (listOfDuplicate_Vsp.at(i) == (size_t)-1)													//it's not a duplicate
		{
			size_t nbVirtual = obj->listVirtualSubParts.size();
			filesize += nbVirtual * sizeof(FMP_Section4_Next);


			for (size_t j = 0; j < nbVirtual; j++)
			{
				FMP_VirtualSubPart* virt = obj->listVirtualSubParts.at(j);

				size_t nbIndex = virt->listIndex.size();
				if (nbIndex)
					filesize += nbIndex * sizeof(FMP_Section4_Next_b);

				filesize += ((virt->sub0) ? sizeof(FMP_Section4_Next_c) : 0) + ((virt->sub1) ? sizeof(FMP_Section4_Next_c) : 0);


				if (virt->action)
				{
					FMP_Action* action = virt->action;
					size_t nbCommand = action->listCommands.size();

					filesize += sizeof(FMP_Section4_Next3);
					filesize += nbCommand * sizeof(FMP_Section4_Next3_b);

					for (size_t j = 0; j < nbCommand; j++)
					{
						FMP_Action::Command &command = action->listCommands.at(j);
						NameLater::addNameLater(command.functionName, 0, listAllNames);

						size_t nbparams = command.parameters.size();
						filesize += nbparams * sizeof(FMP_Section4_Next_f);

						for (size_t k = 0; k < nbparams; k++)
						{
							FMP_Parameter &param = command.parameters.at(k);
							NameLater::addNameLater(param.name, 0, listAllNames);

							switch (param.typeValue)
							{
							case 0: filesize += sizeof(uint32_t); break;
							case 1: filesize += sizeof(uint32_t); break;
							case 2: filesize += sizeof(float); break;
							case 3: filesize += 4 * sizeof(uint32_t); break;
							case 4: filesize += 4 * sizeof(uint32_t); break;
							case 6:
							{
								if (param.stringValue != "")
								{
									NameLater::addNameLater(param.stringValue, 0, listAllNames);
								}
							}break;
							case 8: filesize += sizeof(uint32_t); break;
							}
						}
					}
				}
			}
		}

		listDebug.push_back(DebugTest("after Next", filesize));

		

		if ( (obj->action) && (listOfDuplicate_Action.at(i) == (size_t)-1))													//it's not a duplicate
		{
			FMP_Action* action = obj->action;
			size_t nbCommand = action->listCommands.size();

			filesize += sizeof(FMP_Section4_Next3);
			filesize += nbCommand * sizeof(FMP_Section4_Next3_b);

			for (size_t j = 0; j < nbCommand; j++)
			{
				FMP_Action::Command &command = action->listCommands.at(j);
				NameLater::addNameLater(command.functionName, 0, listAllNames);

				size_t nbparams = command.parameters.size();
				filesize += nbparams * sizeof(FMP_Section4_Next_f);

				for (size_t k = 0; k < nbparams; k++)
				{
					FMP_Parameter &param = command.parameters.at(k);
					NameLater::addNameLater(param.name, 0, listAllNames);

					switch (param.typeValue)
					{
					case 0: filesize += sizeof(uint32_t); break;
					case 1: filesize += sizeof(uint32_t); break;
					case 2: filesize += sizeof(float); break;
					case 3: filesize += 4 * sizeof(uint32_t); break;
					case 4: filesize += 4 * sizeof(uint32_t); break;
					case 6:
					{
						if (param.stringValue != "")
						{
							NameLater::addNameLater(param.stringValue, 0, listAllNames);
						}
					}break;
					case 8: filesize += sizeof(uint32_t); break;
					}
				}
			}
		}

		listDebug.push_back(DebugTest("after Next3", filesize));



		if (listOfDuplicate_Entity.at(i) == (size_t)-1)													//it's not a duplicate
		{
			size_t nbEntity = obj->listEntity.size();
			filesize += nbEntity * sizeof(FMP_Section4_Next2);

			for (size_t j = 0; j < nbEntity; j++)
			{
				FMP_Entity &ent = obj->listEntity.at(j);
				if (!ent.haveVisualPart)
					continue;

				filesize += sizeof(FMP_Section4_Next2_b);

				NameLater::addNameLater(ent.name, 0, listAllNames);
				if (ent.embTexture_filename.length())
				{
					bool isfound = false;
					size_t nbNames = listTextureEmb.size();
					for (size_t i = 0; i < nbNames; i++)
					{
						if (listTextureEmb.at(i) == ent.embTexture_filename)
						{
							isfound = true;
							break;
						}
					}
					if (!isfound)
						listTextureEmb.push_back(ent.embTexture_filename);

				}
				if (ent.ema_filename.length())
				{
					bool isfound = false;
					size_t nbNames = listEma.size();
					for (size_t i = 0; i < nbNames; i++)
					{
						if (listEma.at(i) == ent.ema_filename)
						{
							isfound = true;
							break;
						}
					}
					if (!isfound)
						listEma.push_back(ent.ema_filename);
				}

				size_t nbLod = ent.listLod.size();
				if (nbLod != 1)
					filesize += nbLod * (2 * sizeof(uint32_t) + sizeof(float));

				for (size_t k = 0; k < nbLod; k++)
				{
					if (ent.listLod.at(k).nsk_filename.length() != 0)
					{
						bool isfound = false;
						size_t nbNames = listNsk.size();
						for (size_t i = 0; i < nbNames; i++)
						{
							if (listNsk.at(i) == ent.listLod.at(k).nsk_filename)
							{
								isfound = true;
								break;
							}
						}
						if (!isfound)
							listNsk.push_back(ent.listLod.at(k).nsk_filename);
					}

					if (ent.listLod.at(k).emm_filename.length() != 0)
					{
						bool isfound = false;
						size_t nbNames = listEmm.size();
						for (size_t i = 0; i < nbNames; i++)
						{
							if (listEmm.at(i) == ent.listLod.at(k).emm_filename)
							{
								isfound = true;
								break;
							}
						}
						if (!isfound)
							listEmm.push_back(ent.listLod.at(k).emm_filename);
					}
				}
			}
		}

		listDebug.push_back(DebugTest("after Next2", filesize));


		if ((obj->hierarchy) && (listOfDuplicate_Hierarchy.at(i) == (size_t)-1))									//it's not a duplicate
		{
			filesize += sizeof(FMP_Section4_Next4) + obj->hierarchy->listNode.size() * sizeof(FMP_Section4_Next4_b) + obj->hierarchy->listTransform.size() * sizeof(FMP_Section4_Next4_c);

			if (obj->hierarchy->mRoot)
			{
				std::vector<FMP_HierarchyNode*> listToCheck;				//to deal with recursivity
				listToCheck.push_back(obj->hierarchy->mRoot);

				for (size_t j = 0; j < listToCheck.size(); j++)					//could increase
				{
					FMP_HierarchyNode* cur = listToCheck.at(j);

					filesize += sizeof(FMP_Section4_Next4_d);

					size_t nbChild = cur->listChild.size();
					if (nbChild != 0)
					{
						filesize += sizeof(FMP_Section4_Next4_e);

						for (size_t k = 0; k < nbChild; k++)
						{
							if (cur->listChild.at(k) == 0)
								continue;
							listToCheck.push_back(cur->listChild.at(k));
						}

					}
					else {
						size_t nbIndex = cur->listIndex.size();

						filesize += nbIndex * sizeof(uint16_t);
						if (filesize & 0x3)
							filesize += (0x4 - (filesize & 0x3));
					}
				}
			}
		}
		listDebug.push_back(DebugTest("after Next4", filesize));
	}

	listDebug.push_back(DebugTest("before Section5", filesize));


	//section5
	nbObject = mListHitboxGroup.size();
	filesize += nbObject * sizeof(FMP_Section5);
	for (size_t i = 0; i < nbObject; i++)
	{
		FMP_HitboxGroup &group = mListHitboxGroup.at(i);
		size_t nbhitbox = group.listHitbox.size();
		
		NameLater::addNameLater(group.name, 0, listAllNames);
		filesize += nbhitbox * sizeof(FMP_Section5_Hitbox);
		
		for (size_t j = 0; j < nbhitbox; j++)
		{
			FMP_Hitbox &hitbox = group.listHitbox.at(j);
			NameLater::addNameLater(hitbox.name, 0, listAllNames);

			size_t nbDes = hitbox.listOnDestruction.size();
			filesize += nbDes * sizeof(FMP_Section5_Hitbox_b);

			for (size_t k = 0; k < nbDes; k++)
			{
				size_t nbDessss = hitbox.listOnDestruction.at(k).size();
				filesize += nbDessss * sizeof(FMP_Section5_Hitbox_c);

				for (size_t m = 0; m < nbDessss; m++)
				{
					FMP_Destruction &des = hitbox.listOnDestruction.at(k).at(m);
					filesize += ((des.haveSub0) ? sizeof(FMP_Section5_Hitbox_d) : 0) + ((des.haveSub1) ? sizeof(FMP_Section5_Hitbox_d) : 0) + ((des.havokFileIndex !=(size_t)-1) ? mListHavokFile.at(des.havokFileIndex)->size : 0);
				}
			}

			size_t nbHavock = hitbox.listHavokFilesIndex.size();
			if (nbHavock)
			{
				filesize += sizeof(FMP_Section5_Hitbox_f);
				for (size_t k = 0; k < nbHavock; k++)
					filesize += mListHavokFile.at(hitbox.listHavokFilesIndex.at(k))->size;
			}

			size_t nbVertex = hitbox.listVertex.size();
			filesize += nbVertex * sizeof(FMP_Section5_Hitbox_e);

			size_t nbFaceIndex = hitbox.listFaceIndex.size();
			filesize += nbFaceIndex * sizeof(uint16_t);
			if (filesize & 0x3)
				filesize += (0x4 - (filesize & 0x3));
		}

	}





	//Dependances:
	filesize += listNsk.size() * sizeof(uint32_t);
	filesize += listTextureEmb.size() * sizeof(uint32_t);
	filesize += listEmm.size() * sizeof(uint32_t);
	filesize += listEma.size() * sizeof(uint32_t);

	nbElement = listNsk.size();
	for (size_t i = 0; i < nbElement; i++)
		NameLater::addNameLater(listNsk.at(i), 0, listAllNames);
	nbElement = listTextureEmb.size();
	for (size_t i = 0; i < nbElement; i++)
		NameLater::addNameLater(listTextureEmb.at(i), 0, listAllNames);
	nbElement = listEmm.size();
	for (size_t i = 0; i < nbElement; i++)
		NameLater::addNameLater(listEmm.at(i), 0, listAllNames);
	nbElement = listEma.size();
	for (size_t i = 0; i < nbElement; i++)
		NameLater::addNameLater(listEma.at(i), 0, listAllNames);


	//all names
	nbElement = listAllNames.size();
	for (size_t i = 0; i < nbElement; i++)
		filesize += listAllNames.at(i).name.length() + 1;
	
	
	return filesize;
}














/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*-------------------------------------------------------------------------------\
|                             CreateFile			                             |
\-------------------------------------------------------------------------------*/
uint8_t* FmpFile::CreateFile(unsigned int *psize)
{
	size_t filesize = calculeFilesize();

	uint8_t *buf = new uint8_t[filesize];
	if (!buf)
	{
		LOG_DEBUG("%s: Memory allocation error (0x%x)\n", FUNCNAME, filesize);
		LibXenoverse::notifyError();
		return nullptr;
	}
	memset(buf, 0, filesize);				//fill by 0 to secure, and not having random memory.


	write(buf, filesize);


	*psize = filesize;
	return buf;
}
/*-------------------------------------------------------------------------------\
|                             write												 |
\-------------------------------------------------------------------------------*/
size_t FmpFile::write(uint8_t *buf, size_t size)
{
	struct NameLater								//for keep the information of the uint32 position in buf to update the offset of a name when is posed
	{
		string name;
		std::vector<size_t> listPosInBufffer;
		NameLater(string name, size_t posInBufffer) { this->name = name; listPosInBufffer.push_back(posInBufffer); }

		static void addNameLater(string name, size_t posInBufffer, std::vector<NameLater> &listAllNames)
		{
			size_t nbNames = listAllNames.size();
			for (size_t i = 0; i < nbNames; i++)
			{
				if (listAllNames.at(i).name == name)
				{
					listAllNames.at(i).listPosInBufffer.push_back(posInBufffer);
					return;
				}
			}
			listAllNames.push_back(NameLater(name, posInBufffer));
		};


		static size_t addUniqueString(string str, std::vector<string> &listString)
		{
			size_t nbNames = listString.size();
			for (size_t i = 0; i < nbNames; i++)
				if (listString.at(i) == str)
					return i ;

			listString.push_back(str);
			return listString.size() - 1;
		};
	};
	

	std::vector<NameLater> listAllNames;
	size_t offset = 0;
	size_t offset_tmp = 0;

	
	FMP_Header* hdr = (FMP_Header*)buf;

	strcpy(&(hdr->signature[0]), FMP_SIGNATURE);
	hdr->version = 0x020900;
	hdr->unk_08 = 0x12;
	hdr->unk_0C = 0;
	hdr->unk_60 = 0;
	hdr->unk_64 = 0;
	hdr->unk_68 = 0;
	hdr->unk_6C = 0;


	hdr->offset_Section0a = sizeof(FMP_Header);
	hdr->offset_Section0b = hdr->offset_Section0a + sizeof(FMP_Section0a);
	offset = hdr->offset_Section0b + sizeof(FMP_Section0b) + 36 + mListTypeName.size() * sizeof(uint32_t) + 48;

	hdr->numberSection1 = mListS1.size();
	hdr->offset_Section1 = (hdr->numberSection1!=0) ? offset : 0;
	if (hdr->numberSection1 != 0)
		offset = hdr->offset_Section1 + hdr->numberSection1 * sizeof(FMP_Section1);


	hdr->numberSection2 = mListS2.size();
	hdr->offset_Section2 = (hdr->numberSection2!=0) ? offset : 0;
	if (hdr->numberSection2 != 0)
		offset = hdr->offset_Section2 + hdr->numberSection2 * sizeof(FMP_Section2);

	hdr->numberSection3 = mListFragmentsGroups.size();
	hdr->offset_Section3 = (hdr->numberSection3!=0) ? offset : 0;
	offset_tmp = offset;

	
	//Section0a
	FMP_Section0a* section0a = (FMP_Section0a*)(buf + hdr->offset_Section0a);
	section0a->width_X = width_X;
	section0a->width_Y = width_Y;
	section0a->width_Z = width_Z;
	section0a->unk_3 = s0a_unk_3;
	section0a->unk_4 = s0a_unk_4;
	section0a->unk_5 = s0a_unk_5;
	section0a->unk_6 = s0a_unk_6;
	section0a->unk_7 = s0a_unk_7;
	section0a->unk_8 = s0a_unk_8;
	section0a->unk_9 = s0a_unk_9;
	section0a->unk_10 = s0a_unk_10;
	section0a->unk_11 = s0a_unk_11;
	section0a->unk_12 = s0a_unk_12;
	section0a->unk_13 = s0a_unk_13;
	section0a->unk_14 = s0a_unk_14;
	section0a->unk_15 = s0a_unk_15;
	section0a->unk_16 = s0a_unk_16;
	section0a->unk_17 = s0a_unk_17;
	section0a->unk_18 = s0a_unk_18;
	section0a->unk_19 = s0a_unk_19;
	section0a->unk_20 = s0a_unk_20;
	section0a->unk_21 = s0a_unk_21;
	section0a->unk_22 = s0a_unk_22;
	section0a->unk_23 = s0a_unk_23;
	section0a->unk_24 = s0a_unk_24;
	section0a->unk_25 = s0a_unk_25;
	section0a->unk_26 = s0a_unk_26;
	section0a->unk_27 = s0a_unk_27;
	section0a->unk_28 = s0a_unk_28;
	section0a->unk_29 = s0a_unk_29;
	section0a->unk_30 = s0a_unk_30;
	section0a->unk_31 = s0a_unk_31;
	section0a->nearDistance = nearDistance;
	section0a->farDistance = farDistance;
	section0a->unk_34 = s0a_unk_34;

	//Section0b
	FMP_Section0b* section0b = (FMP_Section0b*)(buf + hdr->offset_Section0b);
	section0b->unk_0 = s0b_unk_0;
	section0b->unk_1 = s0b_unk_1;
	section0b->unk_2 = s0b_unk_2;
	section0b->unk_3 = s0b_unk_3;
	section0b->unk_4 = s0b_unk_4;
	section0b->unk_5 = s0b_unk_5;
	section0b->unk_6 = s0b_unk_6;
	section0b->unk_7 = s0b_unk_7;
	section0b->unk_8 = s0b_unk_8;
	section0b->unk_9 = s0b_unk_9;
	section0b->unk_10 = s0b_unk_10;
	section0b->unk_11 = s0b_unk_11;
	section0b->unk_12 = s0b_unk_12;
	section0b->unk_13 = s0b_unk_13;
	section0b->unk_14 = s0b_unk_14;
	section0b->unk_15 = s0b_unk_15;
	section0b->unk_16 = s0b_unk_16;
	section0b->unk_17 = s0b_unk_17;
	section0b->unk_18 = s0b_unk_18;
	section0b->unk_19 = s0b_unk_19;
	section0b->unk_20 = s0b_unk_20;
	section0b->unk_21 = s0b_unk_21;
	section0b->unk_22 = s0b_unk_22;
	section0b->unk_23 = s0b_unk_23;
	section0b->unk_23b = s0b_unk_23b;
	section0b->unk_24 = s0b_unk_24;
	section0b->unk_25 = s0b_unk_25;
	section0b->unk_26 = s0b_unk_26;
	section0b->unk_27 = s0b_unk_27;
	section0b->unk_28 = s0b_unk_28;
	section0b->unk_29 = s0b_unk_29;
	section0b->unk_30 = s0b_unk_30;
	section0b->unk_31 = s0b_unk_31;
	section0b->unk_32 = s0b_unk_32;
	section0b->unk_33 = s0b_unk_33;
	section0b->unk_34 = s0b_unk_34;
	section0b->unk_35 = s0b_unk_35;
	section0b->unk_36 = s0b_unk_36;
	section0b->unk_37 = s0b_unk_37;
	section0b->unk_38 = s0b_unk_38;
	section0b->unk_39 = s0b_unk_39;
	section0b->unk_40 = s0b_unk_40;
	section0b->unk_41 = s0b_unk_41;
	section0b->unk_42 = s0b_unk_42;
	section0b->unk_43 = s0b_unk_43;
	section0b->unk_44 = s0b_unk_44;
	section0b->unk_45 = s0b_unk_45;
	section0b->unk_46 = s0b_unk_46;
	section0b->unk_47 = s0b_unk_47;
	section0b->unk_48 = s0b_unk_48;
	section0b->unk_49 = s0b_unk_49;
	section0b->unk_50 = s0b_unk_50;
	section0b->unk_51 = s0b_unk_51;
	section0b->unk_52 = s0b_unk_52;
	section0b->unk_53 = s0b_unk_53;
	section0b->unk_54 = s0b_unk_54;

	
	offset = hdr->offset_Section0b + sizeof(FMP_Section0b) + 36;
	size_t nbElements = mListTypeName.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		if(mListTypeName.at(i).length()!=0)
			NameLater::addNameLater(mListTypeName.at(i), offset, listAllNames);			//(offset will be udpated later)
		offset += sizeof(uint32_t);
	}


	//Section1
	offset = hdr->offset_Section1;
	FMP_Section1* section1 = (FMP_Section1*)(buf + hdr->offset_Section1);
	for (size_t i = 0; i < hdr->numberSection1; i++)
	{
		FMP_S1 &s1 = mListS1.at(i);

		NameLater::addNameLater(s1.name, offset, listAllNames);		//for section1[i].name_offset (will be udpated later)
		section1[i].unk_04 = s1.unk_04;
		section1[i].unk_08 = s1.unk_08;
		section1[i].unk_0C = s1.unk_0C;
		section1[i].unk_10 = s1.unk_10;
		section1[i].unk_14 = s1.unk_14;

		offset += sizeof(FMP_Section1);
	}

	//Section2
	offset = hdr->offset_Section2;
	FMP_Section2* section2 = (FMP_Section2*)(buf + hdr->offset_Section2);
	for (size_t i = 0; i < hdr->numberSection2; i++)
	{
		FMP_S2 &s2 = mListS2.at(i);

		NameLater::addNameLater(s2.name, offset, listAllNames);		//for section1[i].name_offset (will be udpated later)
		section2[i].unk_04 = s2.unk_04;
		section2[i].unk_08 = s2.unk_08;
		section2[i].unk_0C = s2.unk_0C;
		section2[i].unk_10 = s2.unk_10;
		section2[i].unk_14 = s2.unk_14;
		section2[i].unk_18 = s2.unk_18;

		offset += sizeof(FMP_Section2);
	}



	//Section3
	FMP_Section3* section3 = (FMP_Section3*)GetOffsetPtr(buf, hdr->offset_Section3);
	offset = hdr->offset_Section3 + hdr->numberSection3 * sizeof(FMP_Section3);			//begin of list of index

	for (size_t i = 0; i < hdr->numberSection3; i++)
	{
		FragmentsGroup &group = mListFragmentsGroups.at(i);
		NameLater::addNameLater(group.name, hdr->offset_Section3 + i * sizeof(FMP_Section3), listAllNames);		//for section3[i].name_offset (will be udpated later)
		section3[i].num_indexes = group.listObjects.size();
		section3[i].indexes_offset = (section3[i].num_indexes!=0) ?  offset : 0;

		uint16_t* index = (uint16_t*)GetOffsetPtr(buf, offset);

		std::vector<size_t> &listIndex = group.listObjects;
		size_t nbIndex = section3[i].num_indexes;
		for (size_t j = 0; j < nbIndex; j++)
		{
			index[j] = listIndex.at(j);
			offset += sizeof(uint16_t);
		}
	}
	if (offset & 0x3)
		offset += (0x4 - (offset & 0x3));
	if (offset != 0)
		offset_tmp = offset;



	struct DebugTest
	{
		string text;
		size_t offset;
		DebugTest(string text, size_t offset) { this->text = text; this->offset = offset; }
	};
	std::vector<DebugTest> listDebug;


	//Section4
	hdr->numberSection4 = mListObject.size();
	hdr->offset_Section4 = (hdr->numberSection4 != 0) ? offset_tmp : 0;

	offset = hdr->offset_Section4 + hdr->numberSection4 * sizeof(FMP_Section4);			//for next Extra Data.
	
	listDebug.push_back(DebugTest("Start after Section4", offset));

	FMP_Section4* section4 = (FMP_Section4*)GetOffsetPtr(buf, hdr->offset_Section4);
	

	//if all subparts of a Section4 is the same as another, we could use the offset of the other begin suppart, to remove duplicates.
	std::vector<size_t> listOfDuplicate_Entity;
	listOfDuplicate_Entity.resize(hdr->numberSection4, (size_t)-1);
	std::vector<size_t> listOfDuplicate_Vsp;
	listOfDuplicate_Vsp.resize(hdr->numberSection4, (size_t)-1);
	std::vector<size_t> listOfDuplicate_Action;
	listOfDuplicate_Action.resize(hdr->numberSection4, (size_t)-1);
	std::vector<size_t> listOfDuplicate_Hierarchy;
	listOfDuplicate_Hierarchy.resize(hdr->numberSection4, (size_t)-1);



	for (size_t i = 1; i < hdr->numberSection4; i++)					//here the check on duplicate. // I notice the "remove duplication" is applied if the 4 parts are the same in the same time. So here we will synchronize the 4 list
	{
		FMP_Object* obj = mListObject.at(i);
		if (obj->unk_0B != 0xffffff80)
			continue;

		size_t nbEnt = obj->listEntity.size();
		size_t nbVsp = obj->listVirtualSubParts.size();
			
		for (int j = (int)i - 1; j >=0 ; j--)
		{
			FMP_Object* obj_b = mListObject.at(j);
			size_t nbEnt_b = obj_b->listEntity.size();
			size_t nbVsp_b = obj_b->listVirtualSubParts.size();
			
			bool transformEquals = true;
			/*
			for (size_t k = 0; k < 12; k++)
			{
				if (obj->transformMatrix4x3[k] != obj_b->transformMatrix4x3[k])
				{
					transformEquals = false;
					break;
				}
			}
			*/

			if ( (!transformEquals) || 
				(nbVsp != nbVsp_b) || 
				(nbEnt != nbEnt_b) || 
				(obj->hitboxGroupIndex != obj_b->hitboxGroupIndex) || 
				((obj->action == 0) && (obj_b->action != 0)) || 
				((obj->action != 0) && (obj_b->action == 0)) ||
				((obj->hierarchy == 0) && (obj_b->hierarchy != 0)) || 
				((obj->hierarchy != 0) && (obj_b->hierarchy == 0))       )
				continue;

			bool allEquals = true;
			for (size_t k = 0; k < nbEnt; k++)						//Entity
			{
				if (obj->listEntity.at(k) != obj_b->listEntity.at(k))
				{
					allEquals = false;
					break;
				}
			}
			if (!allEquals)
				continue;

			for (size_t k = 0; k < nbVsp; k++)						//VirtualSubPart
			{
				if (*obj->listVirtualSubParts.at(k) != *obj_b->listVirtualSubParts.at(k))
				{
					allEquals = false;
					break;
				}
			}
			if (!allEquals)
				continue;

			allEquals = ((obj->action == 0) || (*obj->action == *obj_b->action));			//Action
			if (!allEquals)
				continue;

			allEquals = ((obj->hierarchy == 0) || (*obj->hierarchy == *obj_b->hierarchy));
			if (!allEquals)
				continue;

			//we notify that is equal as a previous subpart.
			listOfDuplicate_Entity.at(i) = j;					
			listOfDuplicate_Vsp.at(i) = j;
			listOfDuplicate_Action.at(i) = j;
			listOfDuplicate_Hierarchy.at(i) = j;
			break;
		}
	}



	

	std::vector<size_t> listOffsetForBeginSubPart_Entity;
	std::vector<size_t> listOffsetForBeginSubPart_Vsp;
	std::vector<size_t> listOffsetForBeginSubPart_Action;
	std::vector<size_t> listOffsetForBeginSubPart_Hierarchy;


	for (size_t i = 0; i < hdr->numberSection4; i++)									//just to keep name order
	{
		FMP_Object* obj = mListObject.at(i);
		NameLater::addNameLater(obj->name, hdr->offset_Section4 + i * sizeof(FMP_Section4), listAllNames);		//for section4[i].name_offset (will be udpated later)
	}
	
	
	for (size_t i = 0; i < hdr->numberSection4; i++)
	{
		listDebug.push_back(DebugTest("*************** Section4[" + std::to_string(i) + "]", offset));

		FMP_Object* obj = mListObject.at(i);

		section4[i].unk_04 = obj->unk_04;
		section4[i].unk_04b = obj->unk_04b;
		section4[i].unk_0A = obj->unk_0A;
		section4[i].unk_0B = obj->unk_0B;
		section4[i].unk_20 = obj->unk_20;
		section4[i].index_Section5 = (obj->hitboxGroupIndex != (size_t)-1) ? obj->hitboxGroupIndex : 0xffff;

		for (size_t j = 0; j<12; j++)
			section4[i].transformMatrix4x3[j] = obj->transformMatrix4x3[j];



		//Section4_Next
		size_t nb_Section4_Next = obj->listVirtualSubParts.size();
		section4[i].offset_Section4_Next = (nb_Section4_Next != 0) ? offset : 0;

		bool isADuplicate_Vps = false;
		if (listOfDuplicate_Vsp.at(i) != (size_t)-1)													//it's a duplicate
		{
			isADuplicate_Vps = true;
			section4[i].offset_Section4_Next = listOffsetForBeginSubPart_Vsp.at(listOfDuplicate_Vsp.at(i));					//we take the offset of previous
		}
		listOffsetForBeginSubPart_Vsp.push_back(section4[i].offset_Section4_Next);


		FMP_Section4_Next* section4_Next = (FMP_Section4_Next*)GetOffsetPtr(buf, section4[i].offset_Section4_Next);
		if (!isADuplicate_Vps)
		{
			for (size_t j = 0; j < nb_Section4_Next; j++)
			{
				FMP_VirtualSubPart* vsp = obj->listVirtualSubParts.at(j);
				section4_Next[j].unk_2 = vsp->unk_2;
				section4_Next[j].unk_2b = vsp->unk_2b;
				section4_Next[j].unk_4 = vsp->unk_4;
				section4_Next[j].unk_5 = vsp->unk_5;

				for (size_t k = 0; k < 12; k++)
					section4_Next[j].transformMatrix4x3[k] = vsp->transformMatrix4x3[k];
			}
			offset += nb_Section4_Next * sizeof(FMP_Section4_Next);
		}



		//Section4_Next3				(apparently come after main part of Next and before the extra datas of Next)
		if (obj->action)
		{
			if (listOfDuplicate_Action.at(i) != (size_t)-1)													//it's a duplicate
			{
				section4[i].offset_Section4_Next3 = listOffsetForBeginSubPart_Action.at( listOfDuplicate_Action.at(i) );
				listOffsetForBeginSubPart_Action.push_back(section4[i].offset_Section4_Next3);
			}else{

				section4[i].offset_Section4_Next3 = offset;
				listOffsetForBeginSubPart_Action.push_back(section4[i].offset_Section4_Next3);

				FMP_Section4_Next3* section4_Next3 = (FMP_Section4_Next3*)GetOffsetPtr(buf, section4[i].offset_Section4_Next3);

				section4_Next3->unk_0 = obj->action->unk_0;
				section4_Next3->unk_1a = obj->action->unk_1a;
				section4_Next3->unk_1b = obj->action->unk_1b;
				section4_Next3->unk_1c = obj->action->unk_1c;
				section4_Next3->unk_1d = obj->action->unk_1d;

				offset += sizeof(FMP_Section4_Next3);

				//Section4_Next3_b
				section4_Next3->number_Section4_Next3_b = obj->action->listCommands.size();
				section4_Next3->offset_Section4_Next3_b = (section4_Next3->number_Section4_Next3_b != 0) ? offset : 0;

				offset += section4_Next3->number_Section4_Next3_b * sizeof(FMP_Section4_Next3_b);							//for ExtraData

				FMP_Section4_Next3_b* section4_Next3_b = (FMP_Section4_Next3_b*)GetOffsetPtr(buf, section4_Next3->offset_Section4_Next3_b);

				for (size_t k = 0; k < section4_Next3->number_Section4_Next3_b; k++)									//to keep order file from origianl game files.
				{
					FMP_Action::Command &command = obj->action->listCommands.at(k);
					NameLater::addNameLater(command.functionName, section4_Next3->offset_Section4_Next3_b + k * sizeof(FMP_Section4_Next3_b), listAllNames);		//for section4_Next3_b[i].name_offset (will be udpated later)
				}

				for (size_t k = 0; k < section4_Next3->number_Section4_Next3_b; k++)
				{
					FMP_Action::Command &command = obj->action->listCommands.at(k);

					section4_Next3_b[k].unk_1a = command.unk_1a;
					section4_Next3_b[k].unk_1b = command.unk_1b;
					section4_Next3_b[k].unk_1c = command.unk_1c;
					section4_Next3_b[k].unk_1d = command.unk_1d;

					//Section4_Next_f
					size_t nbParam = command.parameters.size();
					section4_Next3_b[k].number_Section4_Next_f = nbParam;
					section4_Next3_b[k].offset_Section4_Next_f = (nbParam != 0) ? offset : 0;

					FMP_Section4_Next_f* section4_Next_f = (FMP_Section4_Next_f*)GetOffsetPtr(buf, section4_Next3_b[k].offset_Section4_Next_f);
					offset += nbParam * sizeof(FMP_Section4_Next_f);										//for ExtraData

					for (size_t m = 0; m < nbParam; m++)
					{
						FMP_Parameter &param = command.parameters.at(m);
						NameLater::addNameLater(param.name, section4_Next3_b[k].offset_Section4_Next_f + m * sizeof(FMP_Section4_Next_f), listAllNames);		//for section4_Next_f[m].name_offset (will be udpated later)

						section4_Next_f[m].type_Data = (uint32_t)param.typeValue;
						section4_Next_f[m].offset_Data = offset;


						switch (param.typeValue)
						{
						case 0:
						{
							(*(uint32_t*)(buf + offset)) = param.boolValue ? 1 : 0;
							offset += sizeof(uint32_t);
						}break;

						case 1:
						{
							(*(int*)(buf + offset)) = param.intValue;
							offset += sizeof(uint32_t);
						}break;

						case 2:
						{
							(*(float*)(buf + offset)) = param.floatValue;
							offset += sizeof(float);
						}break;

						case 8:
						{
							(*(uint32_t*)(buf + offset)) = param.uintValue;
							offset += sizeof(uint32_t);
						}break;

						case 6:
						{
							if (param.stringValue != "")
								NameLater::addNameLater(param.stringValue, section4_Next3_b[k].offset_Section4_Next_f + m * sizeof(FMP_Section4_Next_f) + 0x8, listAllNames);		// (will be udpated later)
							else
								section4_Next_f[m].offset_Data = 0;
						}break;

						case 3:
						{
							for (size_t k = 0; k < 4; k++)
							{
								(*(float*)(buf + offset)) = param.directionValue[k];
								offset += sizeof(uint32_t);
							}
						}break;

						case 4:
						{
							for (size_t k = 0; k < 4; k++)
							{
								(*(float*)(buf + offset)) = param.positionValue[k];
								offset += sizeof(uint32_t);
							}
						}break;
						}
					}
				}
			}
		}
		listDebug.push_back(DebugTest("after Next3", offset));






		//Section4_Next Second part
		if (!isADuplicate_Vps)
		{
			for (size_t j = 0; j < nb_Section4_Next; j++)
			{
				FMP_VirtualSubPart* vsp = obj->listVirtualSubParts.at(j);




				//Section4_Next_c
				if (vsp->sub0)
				{
					section4_Next[j].offset_Section4_Next_c = offset;
					FMP_Section4_Next_c* section4_Next_c = (FMP_Section4_Next_c*)GetOffsetPtr(buf, section4_Next[j].offset_Section4_Next_c);

					section4_Next_c->unk_0 = vsp->sub0->unk_0;
					section4_Next_c->unk_0b = vsp->sub0->unk_0b;
					section4_Next_c->unk_1 = vsp->sub0->unk_1;
					section4_Next_c->unk_1b = vsp->sub0->unk_1b;
					section4_Next_c->unk_2 = vsp->sub0->unk_2;
					section4_Next_c->unk_3 = vsp->sub0->unk_3;
					section4_Next_c->unk_4 = vsp->sub0->unk_4;
					section4_Next_c->unk_5 = vsp->sub0->unk_5;
					section4_Next_c->unk_6 = vsp->sub0->unk_6;
					section4_Next_c->unk_7 = vsp->sub0->unk_7;
					section4_Next_c->unk_8 = vsp->sub0->unk_8;
					section4_Next_c->unk_9 = vsp->sub0->unk_9;
					section4_Next_c->unk_10 = vsp->sub0->unk_10;
					section4_Next_c->unk_11 = vsp->sub0->unk_11;
					section4_Next_c->unk_12 = vsp->sub0->unk_12;
					section4_Next_c->unk_13 = vsp->sub0->unk_13;
					section4_Next_c->widthX = vsp->sub0->widthX;
					section4_Next_c->widthY = vsp->sub0->widthY;
					section4_Next_c->widthZ = vsp->sub0->widthZ;
					section4_Next_c->quaternionX = vsp->sub0->quaternionX;
					section4_Next_c->quaternionY = vsp->sub0->quaternionY;
					section4_Next_c->quaternionZ = vsp->sub0->quaternionZ;
					section4_Next_c->quaternionW = vsp->sub0->quaternionW;

					offset += sizeof(FMP_Section4_Next_c);
				}


				//Section4_Next_c
				if (vsp->sub1)
				{
					section4_Next[j].offset_Section4_Next_c_2 = offset;
					FMP_Section4_Next_c* section4_Next_c = (FMP_Section4_Next_c*)GetOffsetPtr(buf, section4_Next[j].offset_Section4_Next_c_2);

					section4_Next_c->unk_0 = vsp->sub1->unk_0;
					section4_Next_c->unk_0b = vsp->sub1->unk_0b;
					section4_Next_c->unk_1 = vsp->sub1->unk_1;
					section4_Next_c->unk_1b = vsp->sub1->unk_1b;
					section4_Next_c->unk_2 = vsp->sub1->unk_2;
					section4_Next_c->unk_3 = vsp->sub1->unk_3;
					section4_Next_c->unk_4 = vsp->sub1->unk_4;
					section4_Next_c->unk_5 = vsp->sub1->unk_5;
					section4_Next_c->unk_6 = vsp->sub1->unk_6;
					section4_Next_c->unk_7 = vsp->sub1->unk_7;
					section4_Next_c->unk_8 = vsp->sub1->unk_8;
					section4_Next_c->unk_9 = vsp->sub1->unk_9;
					section4_Next_c->unk_10 = vsp->sub1->unk_10;
					section4_Next_c->unk_11 = vsp->sub1->unk_11;
					section4_Next_c->unk_12 = vsp->sub1->unk_12;
					section4_Next_c->unk_13 = vsp->sub1->unk_13;
					section4_Next_c->widthX = vsp->sub1->widthX;
					section4_Next_c->widthY = vsp->sub1->widthY;
					section4_Next_c->widthZ = vsp->sub1->widthZ;
					section4_Next_c->quaternionX = vsp->sub1->quaternionX;
					section4_Next_c->quaternionY = vsp->sub1->quaternionY;
					section4_Next_c->quaternionZ = vsp->sub1->quaternionZ;
					section4_Next_c->quaternionW = vsp->sub1->quaternionW;

					offset += sizeof(FMP_Section4_Next_c);
				}



				//Section4_Next_b
				size_t nbIndex = vsp->listIndex.size();
				section4_Next[j].number_Section4_Next_b = nbIndex;
				section4_Next[j].offset_Section4_Next_b = (nbIndex != 0) ? offset : 0;

				FMP_Section4_Next_b* section4_Next_b = (FMP_Section4_Next_b*)GetOffsetPtr(buf, section4_Next[j].offset_Section4_Next_b);
				offset += (nbIndex != 0) ? nbIndex * sizeof(FMP_Section4_Next_b) : 0;

				for (size_t k = 0; k < nbIndex; k++)
				{
					section4_Next_b[k].unk_0 = vsp->listIndex.at(k).at(0);
					section4_Next_b[k].unk_1 = vsp->listIndex.at(k).at(1);				//hyp: all manipulation of the file , there is 2 values
				}


				//Section4_Next_d = section4_Next3
				if (vsp->action)
				{
					section4_Next[j].offset_Section4_Next_d = offset;
					FMP_Section4_Next3* section4_Next3 = (FMP_Section4_Next3*)GetOffsetPtr(buf, section4_Next[j].offset_Section4_Next_d);

					section4_Next3->unk_0 = vsp->action->unk_0;
					section4_Next3->unk_1a = vsp->action->unk_1a;
					section4_Next3->unk_1b = vsp->action->unk_1b;
					section4_Next3->unk_1c = vsp->action->unk_1c;
					section4_Next3->unk_1d = vsp->action->unk_1d;

					offset += sizeof(FMP_Section4_Next3);

					//Section4_Next3_b
					section4_Next3->number_Section4_Next3_b = vsp->action->listCommands.size();
					section4_Next3->offset_Section4_Next3_b = (section4_Next3->number_Section4_Next3_b != 0) ? offset : 0;

					offset += section4_Next3->number_Section4_Next3_b * sizeof(FMP_Section4_Next3_b);							//for ExtraData

					FMP_Section4_Next3_b* section4_Next3_b = (FMP_Section4_Next3_b*)GetOffsetPtr(buf, section4_Next3->offset_Section4_Next3_b);

					for (size_t k = 0; k < section4_Next3->number_Section4_Next3_b; k++)			//keep name order from original game files
					{
						FMP_Action::Command &command = vsp->action->listCommands.at(k);
						NameLater::addNameLater(command.functionName, section4_Next3->offset_Section4_Next3_b + k * sizeof(FMP_Section4_Next3_b), listAllNames);		//for section4_Next3_b[i].name_offset (will be udpated later)
					}


					for (size_t k = 0; k < section4_Next3->number_Section4_Next3_b; k++)
					{
						FMP_Action::Command &command = vsp->action->listCommands.at(k);

						section4_Next3_b[k].unk_1a = command.unk_1a;
						section4_Next3_b[k].unk_1b = command.unk_1b;
						section4_Next3_b[k].unk_1c = command.unk_1c;
						section4_Next3_b[k].unk_1d = command.unk_1d;

						//Section4_Next_f
						size_t nbParam = command.parameters.size();
						section4_Next3_b[k].number_Section4_Next_f = nbParam;
						section4_Next3_b[k].offset_Section4_Next_f = (nbParam != 0) ? offset : 0;

						FMP_Section4_Next_f* section4_Next_f = (FMP_Section4_Next_f*)GetOffsetPtr(buf, section4_Next3_b[k].offset_Section4_Next_f);
						offset += nbParam * sizeof(FMP_Section4_Next_f);										//for ExtraData

						for (size_t m = 0; m < nbParam; m++)
						{
							FMP_Parameter &param = command.parameters.at(m);
							NameLater::addNameLater(param.name, section4_Next3_b[k].offset_Section4_Next_f + m * sizeof(FMP_Section4_Next_f), listAllNames);		//for section4_Next_f[m].name_offset (will be udpated later)
							section4_Next_f[m].type_Data = (uint32_t)param.typeValue;
							section4_Next_f[m].offset_Data = offset;


							switch (param.typeValue)
							{
							case 0:
							{
								(*(uint32_t*)(buf + offset)) = param.boolValue ? 1 : 0;
								offset += sizeof(uint32_t);
							}break;

							case 1:
							{
								(*(int*)(buf + offset)) = param.intValue;
								offset += sizeof(uint32_t);
							}break;

							case 2:
							{
								(*(float*)(buf + offset)) = param.floatValue;
								offset += sizeof(float);
							}break;

							case 8:
							{
								(*(uint32_t*)(buf + offset)) = param.uintValue;
								offset += sizeof(uint32_t);
							}break;

							case 6:
							{
								if (param.stringValue != "")
									NameLater::addNameLater(param.stringValue, section4_Next3_b[k].offset_Section4_Next_f + m * sizeof(FMP_Section4_Next_f) + 0x8, listAllNames);		// (will be udpated later)
								else
									section4_Next_f[m].offset_Data = 0;
							}break;

							case 3:
							{
								for (size_t k = 0; k < 4; k++)
								{
									(*(float*)(buf + offset)) = param.directionValue[k];
									offset += sizeof(uint32_t);
								}
							}break;

							case 4:
							{
								for (size_t k = 0; k < 4; k++)
								{
									(*(float*)(buf + offset)) = param.positionValue[k];
									offset += sizeof(uint32_t);
								}
							}break;
							}
						}
					}
				}
			}
		}
		listDebug.push_back(DebugTest("after Next", offset));



		

		//Section4_Next4
		if (obj->hierarchy)
		{
			if (listOfDuplicate_Hierarchy.at(i) != (size_t)-1)													//it's a duplicate
			{
				section4[i].offset_Section4_Next4 = listOffsetForBeginSubPart_Hierarchy.at(listOfDuplicate_Hierarchy.at(i));
				listOffsetForBeginSubPart_Hierarchy.push_back(section4[i].offset_Section4_Next4);
			}else{
				section4[i].offset_Section4_Next4 = offset;
				listOffsetForBeginSubPart_Hierarchy.push_back(section4[i].offset_Section4_Next4);

				FMP_Section4_Next4* section4_Next4 = (FMP_Section4_Next4*)(buf + section4[i].offset_Section4_Next4);
				offset += sizeof(FMP_Section4_Next4);


				//Section4_Next4_b
				section4_Next4->number_Section4_Next4_b = obj->hierarchy->listNode.size();
				section4_Next4->offset_Section4_Next4_b = (section4_Next4->number_Section4_Next4_b != 0) ? offset : 0;

				FMP_Section4_Next4_b* section4_Next4_b = (FMP_Section4_Next4_b*)(buf + section4_Next4->offset_Section4_Next4_b);
				for (size_t j = 0; j < section4_Next4->number_Section4_Next4_b; j++)
				{
					FMP_Node &node = obj->hierarchy->listNode.at(j);
					section4_Next4_b[j].unk_0 = node.unk_0;
					section4_Next4_b[j].unk_1 = node.unk_1;
					section4_Next4_b[j].unk_2 = node.unk_2;
					section4_Next4_b[j].unk_3 = node.unk_3;
					section4_Next4_b[j].unk_4 = node.unk_4;
					section4_Next4_b[j].unk_5 = node.unk_5;
					section4_Next4_b[j].unk_6 = node.unk_6;
					section4_Next4_b[j].unk_7 = node.unk_7;
					section4_Next4_b[j].unk_8 = node.unk_8;
					section4_Next4_b[j].unk_9 = node.unk_9;
					section4_Next4_b[j].unk_10 = node.unk_10;
					section4_Next4_b[j].indexToNext4_c_Transform = (node.transformIndex != (size_t)-1) ? node.transformIndex : 0xffffffff;
				}
				offset += section4_Next4->number_Section4_Next4_b * sizeof(FMP_Section4_Next4_b);


				//Section4_Next4_c
				section4_Next4->number_Section4_Next4_c = obj->hierarchy->listTransform.size();
				section4_Next4->offset_Section4_Next4_c = (section4_Next4->number_Section4_Next4_c != 0) ? offset : 0;

				FMP_Section4_Next4_c* section4_Next4_c = (FMP_Section4_Next4_c*)(buf + section4_Next4->offset_Section4_Next4_c);
				for (size_t j = 0; j < section4_Next4->number_Section4_Next4_c; j++)
				{
					FMP_NodeTransform &transf = obj->hierarchy->listTransform.at(j);
					section4_Next4_c[j].positionX = transf.position[0];
					section4_Next4_c[j].positionY = transf.position[1];
					section4_Next4_c[j].positionZ = transf.position[2];
					section4_Next4_c[j].rotationX = transf.rotation[0];
					section4_Next4_c[j].rotationY = transf.rotation[1];
					section4_Next4_c[j].rotationZ = transf.rotation[2];
					section4_Next4_c[j].scaleX = transf.scale[0];
					section4_Next4_c[j].scaleY = transf.scale[1];
					section4_Next4_c[j].scaleZ = transf.scale[2];
				}
				offset += section4_Next4->number_Section4_Next4_c * sizeof(FMP_Section4_Next4_c);


				if (obj->hierarchy->mRoot)
				{
					section4_Next4->offset_Section4_Next4_d = offset;

					std::vector<FMP_HierarchyNode*> listToCheck;				//to deal with recursivity
					std::vector<size_t> listToCheck_offsetLater;
					listToCheck.push_back(obj->hierarchy->mRoot);
					listToCheck_offsetLater.push_back(section4[i].offset_Section4_Next4 + 0xC);				//== offset for section4_Next4->offset_Section4_Next4_d 


					for (size_t j = 0; j < listToCheck.size(); j++)					//could increase
					{
						FMP_HierarchyNode* cur = listToCheck.at(j);

						FMP_Section4_Next4_d* section4_Next4_d = (FMP_Section4_Next4_d*)(buf + offset);
						section4_Next4_d->typeb = cur->typeb;
						*((uint32_t*)(buf + listToCheck_offsetLater.at(j))) = offset;						//update parent offset for this child.

						offset += sizeof(FMP_Section4_Next4_d);


						size_t nbChild = cur->listChild.size();
						if (nbChild != 0)
						{
							section4_Next4_d->type = 0;
							section4_Next4_d->number_Index = 0;

							FMP_Section4_Next4_e* section4_Next4_e = (FMP_Section4_Next4_e*)(buf + offset);

							section4_Next4_e->unk_0 = cur->unk_0;
							section4_Next4_e->unk_1 = cur->unk_1;
							section4_Next4_e->unk_2 = cur->unk_2;
							section4_Next4_e->unk0_offset = 0;
							section4_Next4_e->unk1_offset = 0;
							section4_Next4_e->unk2_offset = 0;
							section4_Next4_e->unk3_offset = 0;
							section4_Next4_e->unk4_offset = 0;
							section4_Next4_e->unk5_offset = 0;
							section4_Next4_e->unk6_offset = 0;
							section4_Next4_e->unk7_offset = 0;

							size_t inc = 0;
							for (size_t k = 0; k < nbChild; k++)
							{
								if (cur->listChild.at(k) == 0)
									continue;

								listToCheck.insert(listToCheck.begin() + j + 1 + inc, cur->listChild.at(k));
								listToCheck_offsetLater.insert(listToCheck_offsetLater.begin() + j + 1 + inc, offset + 0xC + k * sizeof(uint32_t));
								inc++;
							}

							offset += sizeof(FMP_Section4_Next4_e);

						}
						else {

							section4_Next4_d->type = 1;
							section4_Next4_d->number_Index = cur->listIndex.size();

							uint16_t* index = (uint16_t*)(buf + offset);
							for (size_t k = 0; k < section4_Next4_d->number_Index; k++)
								index[k] = cur->listIndex.at(k);

							offset += section4_Next4_d->number_Index * sizeof(uint16_t);
							if (offset & 0x3)
								offset += (0x4 - (offset & 0x3));
						}

						listDebug.push_back(DebugTest("after Next4_d", offset));
					}
				}
			}
		}
		listDebug.push_back(DebugTest("after Next4", offset));




		//Section4_Next2
		size_t nbEnt = obj->listEntity.size();
		section4[i].number_Section4_Next2 = nbEnt;
		section4[i].offset_Section4_Next2 = (nbEnt != 0) ? offset : 0;

		if (listOfDuplicate_Entity.at(i) != (size_t)-1)													//it's a duplicate
		{
			section4[i].offset_Section4_Next2 = listOffsetForBeginSubPart_Entity.at(listOfDuplicate_Entity.at(i));					//we take the offset of previous
			listOffsetForBeginSubPart_Entity.push_back(section4[i].offset_Section4_Next2);
		}else{

			listOffsetForBeginSubPart_Entity.push_back(section4[i].offset_Section4_Next2);

			FMP_Section4_Next2* section4_Next2 = (FMP_Section4_Next2*)GetOffsetPtr(buf, section4[i].offset_Section4_Next2);
			offset += nbEnt * sizeof(FMP_Section4_Next2);									//for Extra Data

			for (size_t j = 0; j < nbEnt; j++)
			{
				FMP_Entity &ent = obj->listEntity.at(j);

				section4_Next2[j].unk_0 = ent.s4n2_unk_0;
				for (size_t k = 0; k < 12; k++)
					section4_Next2[j].transformMatrix4x3[k] = ent.transformMatrix4x3[k];

				if (ent.haveVisualPart)
				{
					section4_Next2[j].offset_Section4_Next2_b = offset;
					FMP_Section4_Next2_b* section4_Next2_b = (FMP_Section4_Next2_b*)GetOffsetPtr(buf, section4_Next2[j].offset_Section4_Next2_b);

					NameLater::addNameLater(ent.name, offset, listAllNames);		//for section4_Next2_b[i].name_offset (will be udpated later)
					section4_Next2_b->unk_0 = ent.s4n2b_unk_0;
					section4_Next2_b->unk_5 = ent.s4n2b_unk_5;
					section4_Next2_b->unk_6 = ent.s4n2b_unk_6;
					section4_Next2_b->unk_8 = ent.s4n2b_unk_8;
					section4_Next2_b->unk_9 = ent.s4n2b_unk_9;
					section4_Next2_b->unk_10 = ent.s4n2b_unk_10;

					offset += sizeof(FMP_Section4_Next2_b);										//for Extra Data

					section4_Next2_b->indexTextureEmb = (ent.embTexture_filename.length() != 0) ? NameLater::addUniqueString(ent.embTexture_filename, listTextureEmb) : 0xffffffff;
					section4_Next2_b->indexEma = (ent.ema_filename.length() != 0) ? NameLater::addUniqueString(ent.ema_filename, listEma) : 0xffffffff;



					section4_Next2_b->number_Lod = ent.listLod.size();

					if (section4_Next2_b->number_Lod == 1)					//hyp : never 0
					{
						FMP_Entity::Lod &lod = ent.listLod.at(0);

						section4_Next2_b->indexNsk = (lod.nsk_filename.length() != 0) ? NameLater::addUniqueString(lod.nsk_filename, listNsk) : 0xffffffff;
						section4_Next2_b->indexEmm = (lod.emm_filename.length() != 0) ? NameLater::addUniqueString(lod.emm_filename, listEmm) : 0xffffffff;
						section4_Next2_b->lodDistance = lod.distance;

					}
					else {

						size_t offsetDistances = offset;
						*((uint32_t*)(&(section4_Next2_b->lodDistance))) = offset;							// original file have this order : distances first.
						section4_Next2_b->indexNsk = offsetDistances + section4_Next2_b->number_Lod * sizeof(float);
						section4_Next2_b->indexEmm = section4_Next2_b->indexNsk + section4_Next2_b->number_Lod * sizeof(uint32_t);
						offset = section4_Next2_b->indexEmm + section4_Next2_b->number_Lod * sizeof(uint32_t);

						uint32_t* indexNsk = (uint32_t*)(buf + section4_Next2_b->indexNsk);
						uint32_t* indexEmm = (uint32_t*)(buf + section4_Next2_b->indexEmm);
						float* lodDistance = (float*)(buf + (*((uint32_t*)&section4_Next2_b->lodDistance)));

						for (size_t k = 0; k < section4_Next2_b->number_Lod; k++)
						{
							FMP_Entity::Lod &lod = ent.listLod.at(k);

							indexNsk[k] = (lod.nsk_filename.length() != 0) ? NameLater::addUniqueString(lod.nsk_filename, listNsk) : 0xffffffff;
							indexEmm[k] = (lod.emm_filename.length() != 0) ? NameLater::addUniqueString(lod.emm_filename, listEmm) : 0xffffffff;
							lodDistance[k] = lod.distance;
						}
					}
				}
			}
		}
		listDebug.push_back(DebugTest("after Next2", offset));
	}
	if (offset != 0)
		offset_tmp = offset;

	listDebug.push_back(DebugTest("before Section5", offset));
	

	





	//section5	
	hdr->numberSection5 = mListHitboxGroup.size();
	hdr->offset_Section5 = (hdr->numberSection5 != 0) ? offset_tmp : 0;

	offset = hdr->offset_Section5 + hdr->numberSection5 * sizeof(FMP_Section5);			//for next Extra Data.

	FMP_Section5* section5 = (FMP_Section5*)GetOffsetPtr(buf, hdr->offset_Section5);

	for (size_t i = 0; i < hdr->numberSection5; i++)									//just to keep name order.
	{
		FMP_HitboxGroup &group = mListHitboxGroup.at(i);
		NameLater::addNameLater(group.name, hdr->offset_Section5 + i * sizeof(FMP_Section5), listAllNames);		//for section5[i].name_offset (will be udpated later)
	}

	for (size_t i = 0; i < hdr->numberSection5; i++)
	{
		FMP_HitboxGroup &group = mListHitboxGroup.at(i);
		size_t nbHitbox = group.listHitbox.size();

		//Section5_Hitbox
		section5[i].number_Section5_Hitbox = nbHitbox;
		section5[i].offset_Section5_Hitbox = (nbHitbox != 0) ? offset : 0;

		FMP_Section5_Hitbox* section5_hitbox = (FMP_Section5_Hitbox*)GetOffsetPtr(buf, section5[i].offset_Section5_Hitbox);
		offset += nbHitbox * sizeof(FMP_Section5_Hitbox);							//for ExtraData

		for (size_t j = 0; j < nbHitbox; j++)
		{
			FMP_Hitbox &hitbox = group.listHitbox.at(j);

			NameLater::addNameLater(hitbox.name, section5[i].offset_Section5_Hitbox + j * sizeof(FMP_Section5_Hitbox), listAllNames);		//for section5_hitbox[i].name_offset (will be udpated later)
			section5_hitbox[j].child = hitbox.child;
			section5_hitbox[j].unk_a0 = hitbox.unk_a0;
			section5_hitbox[j].sibling = hitbox.sibling;
			section5_hitbox[j].parent = hitbox.parent;


			//Section5_Hitbox_b
			size_t nbdesgroup = hitbox.listOnDestruction.size();
			section5_hitbox[j].number_Section5_Hitbox_b = nbdesgroup;
			section5_hitbox[j].offset_Section5_Hitbox_b = (nbdesgroup != 0) ? offset : 0;

			FMP_Section5_Hitbox_b* section5_hitbox_b = (FMP_Section5_Hitbox_b*)GetOffsetPtr(buf, section5_hitbox[j].offset_Section5_Hitbox_b);
			offset += nbdesgroup * sizeof(FMP_Section5_Hitbox_b);									//for extra Data

			for (size_t k = 0; k < nbdesgroup; k++)
			{
				//Section5_Hitbox_c
				size_t nbDes = hitbox.listOnDestruction.at(k).size();
				section5_hitbox_b[k].number_Section5_Hitbox_c = nbDes;
				section5_hitbox_b[k].offset_Section5_Hitbox_c = (nbDes != 0) ? offset : 0;

				FMP_Section5_Hitbox_c* section5_Hitbox_c = (FMP_Section5_Hitbox_c*)GetOffsetPtr(buf, section5_hitbox_b[k].offset_Section5_Hitbox_c);
				offset += nbDes * sizeof(FMP_Section5_Hitbox_c);									//for extra Data

				for (size_t m = 0; m < nbDes; m++)
				{
					FMP_Destruction &des = hitbox.listOnDestruction.at(k).at(m);
					section5_Hitbox_c[m].unk_0 = des.unk_0;
					section5_Hitbox_c[m].unk_1 = des.unk_1;
					section5_Hitbox_c[m].unk_2 = des.unk_2;
					section5_Hitbox_c[m].unk_5 = des.unk_5;
					section5_Hitbox_c[m].unk_6 = des.unk_6;
					section5_Hitbox_c[m].unk_7 = des.unk_7;


					//Section5_Hitbox_d
					if (des.haveSub0)
					{
						section5_Hitbox_c[m].offset_Section5_Hitbox_d = offset;
						FMP_Section5_Hitbox_d* section5_Hitbox_d = (FMP_Section5_Hitbox_d*)GetOffsetPtr(buf, section5_Hitbox_c[m].offset_Section5_Hitbox_d);
						offset += sizeof(FMP_Section5_Hitbox_d);

						section5_Hitbox_d->unk_0 = des.sub0.unk_0;
						section5_Hitbox_d->unk_1 = des.sub0.unk_1;
						section5_Hitbox_d->unk_2 = des.sub0.unk_2;
						section5_Hitbox_d->unk_3 = des.sub0.unk_3;
						section5_Hitbox_d->yaw = des.sub0.yaw * 3.1415926535897f / 180.0f;						//convert in radian
						section5_Hitbox_d->pitch = des.sub0.pitch * 3.1415926535897f / 180.0f;
						section5_Hitbox_d->roll = des.sub0.roll * 3.1415926535897f / 180.0f ;
						section5_Hitbox_d->unk_7 = des.sub0.unk_7;
						section5_Hitbox_d->unk_8 = des.sub0.unk_8;
						section5_Hitbox_d->unk_9 = des.sub0.unk_9;
						section5_Hitbox_d->unk_10 = des.sub0.unk_10;
						section5_Hitbox_d->unk_11 = des.sub0.unk_11;
						section5_Hitbox_d->unk_12 = des.sub0.unk_12;
						section5_Hitbox_d->unk_13 = des.sub0.unk_13;
					}

					if (des.haveSub1)
					{
						section5_Hitbox_c[m].offset_Section5_Hitbox_d_2 = offset;
						FMP_Section5_Hitbox_d* section5_Hitbox_d = (FMP_Section5_Hitbox_d*)GetOffsetPtr(buf, section5_Hitbox_c[m].offset_Section5_Hitbox_d_2);
						offset += sizeof(FMP_Section5_Hitbox_d);

						section5_Hitbox_d->unk_0 = des.sub1.unk_0;
						section5_Hitbox_d->unk_1 = des.sub1.unk_1;
						section5_Hitbox_d->unk_2 = des.sub1.unk_2;
						section5_Hitbox_d->unk_3 = des.sub1.unk_3;
						section5_Hitbox_d->yaw = des.sub1.yaw * 3.1415926535897f / 180.0f;						//convert in radian
						section5_Hitbox_d->pitch = des.sub1.pitch * 3.1415926535897f / 180.0f;
						section5_Hitbox_d->roll = des.sub1.roll * 3.1415926535897f / 180.0f;
						section5_Hitbox_d->unk_7 = des.sub1.unk_7;
						section5_Hitbox_d->unk_8 = des.sub1.unk_8;
						section5_Hitbox_d->unk_9 = des.sub1.unk_9;
						section5_Hitbox_d->unk_10 = des.sub1.unk_10;
						section5_Hitbox_d->unk_11 = des.sub1.unk_11;
						section5_Hitbox_d->unk_12 = des.sub1.unk_12;
						section5_Hitbox_d->unk_13 = des.sub1.unk_13;
					}


					//Section5_Hitbox_File
					if (des.havokFileIndex!=(size_t)-1)
					{
						section5_Hitbox_c[m].offset_Section5_File = offset;
						section5_Hitbox_c[m].size_Section5_File = mListHavokFile.at(des.havokFileIndex)->size;

						memcpy(buf + section5_Hitbox_c[m].offset_Section5_File, mListHavokFile.at(des.havokFileIndex)->buffer, mListHavokFile.at(des.havokFileIndex)->size);

						offset += mListHavokFile.at(des.havokFileIndex)->size;
					}
				}
			}



			//Section5_Hitbox_f
			size_t nbHavok = hitbox.listHavokFilesIndex.size();
			section5_hitbox[j].offset_Section5_Hitbox_f = (nbHavok != 0) ? offset : 0;
			if (nbHavok != 0)
			{
				FMP_Section5_Hitbox_f* section5_Hitbox_f = (FMP_Section5_Hitbox_f*)GetOffsetPtr(buf, section5_hitbox[j].offset_Section5_Hitbox_f);
				offset += sizeof(FMP_Section5_Hitbox_f);									//for extra Data

				section5_Hitbox_f->unk_0 = nbHavok;
				section5_Hitbox_f->offset_Section5_File = offset;
				section5_Hitbox_f->size_Section5_File = mListHavokFile.at(hitbox.listHavokFilesIndex.at(0))->size;

				offset += nbHavok * section5_Hitbox_f->size_Section5_File;

				for (size_t k = 0; k < nbHavok; k++)
					memcpy(buf + section5_Hitbox_f->offset_Section5_File + k * section5_Hitbox_f->size_Section5_File, mListHavokFile.at(hitbox.listHavokFilesIndex.at(k))->buffer, mListHavokFile.at(hitbox.listHavokFilesIndex.at(k))->size);
			}



			//Section5_Hitbox_e
			size_t nbVertex = hitbox.listVertex.size();
			section5_hitbox[j].number_Section5_Hitbox_e = nbVertex;
			section5_hitbox[j].offset_Section5_Hitbox_e = (nbVertex != 0) ? offset : 0;

			FMP_Section5_Hitbox_e* section5_Hitbox_e = (FMP_Section5_Hitbox_e*)GetOffsetPtr(buf, section5_hitbox[j].offset_Section5_Hitbox_e);
			offset += nbVertex * sizeof(FMP_Section5_Hitbox_e);							//for extra Data

			for (size_t k = 0; k < nbVertex; k++)
			{
				FMP_Hitbox::Vertex &vertex = hitbox.listVertex.at(k);
				section5_Hitbox_e[k].unk_0 = vertex.position[0];
				section5_Hitbox_e[k].unk_1 = vertex.position[1];
				section5_Hitbox_e[k].unk_2 = vertex.position[2];
				section5_Hitbox_e[k].unk_3 = vertex.normal[0];
				section5_Hitbox_e[k].unk_4 = vertex.normal[1];
				section5_Hitbox_e[k].unk_5 = vertex.normal[2];
			}



			//Section5_Hitbox_Face Index
			size_t nbIndex = hitbox.listFaceIndex.size();
			section5_hitbox[j].number_Indexes = nbIndex;
			section5_hitbox[j].offset_Indexes = (nbIndex != 0) ? offset : 0;

			uint16_t* index = (uint16_t*)GetOffsetPtr(buf, section5_hitbox[j].offset_Indexes);
			offset += nbIndex * sizeof(uint16_t);							//for extra Data
			if (offset & 0x3)
				offset += (0x4 - (offset & 0x3));

			for (size_t k = 0; k < nbIndex; k++)
				index[k] = hitbox.listFaceIndex.at(k);
		}
	}
	if (offset != 0)
		offset_tmp = offset;




	//Dependance
	hdr->numberFileInDependance_Nsk = listNsk.size();
	hdr->offset_Dependance_Nsk = (hdr->numberFileInDependance_Nsk != 0) ? offset_tmp : 0;
	if (hdr->numberFileInDependance_Nsk != 0)
		offset_tmp = hdr->offset_Dependance_Nsk + hdr->numberFileInDependance_Nsk * sizeof(uint32_t);

	hdr->numberFileInDependance_TextureEmb = listTextureEmb.size();
	hdr->offset_Dependance_TextureEmb = (hdr->numberFileInDependance_TextureEmb != 0) ? offset_tmp : 0;
	if (hdr->numberFileInDependance_TextureEmb != 0)
		offset_tmp = hdr->offset_Dependance_TextureEmb + hdr->numberFileInDependance_TextureEmb * sizeof(uint32_t);

	hdr->numberFileInDependance_Emm = listEmm.size();
	hdr->offset_Dependance_Emm = (hdr->numberFileInDependance_Emm != 0) ? offset_tmp : 0;
	if (hdr->numberFileInDependance_Emm != 0)
		offset_tmp = hdr->offset_Dependance_Emm + hdr->numberFileInDependance_Emm * sizeof(uint32_t);
	
	hdr->numberFileInDependance_Ema = listEma.size();
	hdr->offset_Dependance_Ema = (hdr->numberFileInDependance_Ema != 0) ? offset_tmp : 0;
	if (hdr->numberFileInDependance_Ema != 0)
		offset_tmp = hdr->offset_Dependance_Ema + hdr->numberFileInDependance_Ema * sizeof(uint32_t);
	

	//dependances
	offset = hdr->offset_Dependance_Nsk;
	for (size_t i = 0; i < hdr->numberFileInDependance_Nsk; i++)
	{
		NameLater::addNameLater(listNsk.at(i), offset, listAllNames);
		offset += sizeof(uint32_t);
	}
	offset = hdr->offset_Dependance_TextureEmb;
	for (size_t i = 0; i < hdr->numberFileInDependance_TextureEmb; i++)
	{
		NameLater::addNameLater(listTextureEmb.at(i), offset, listAllNames);
		offset += sizeof(uint32_t);
	}
	offset = hdr->offset_Dependance_Emm;
	for (size_t i = 0; i < hdr->numberFileInDependance_Emm; i++)
	{
		NameLater::addNameLater(listEmm.at(i), offset, listAllNames);
		offset += sizeof(uint32_t);
	}
	offset = hdr->offset_Dependance_Ema;
	for (size_t i = 0; i < hdr->numberFileInDependance_Ema; i++)
	{
		NameLater::addNameLater(listEma.at(i), offset, listAllNames);
		offset += sizeof(uint32_t);
	}

	
	
	// names
	offset = offset_tmp;
	size_t nbNames = listAllNames.size();
	for (size_t i = 0; i < nbNames; i++)
	{
		size_t nbPosOffset = listAllNames.at(i).listPosInBufffer.size();
		for(size_t j=0;j<nbPosOffset;j++)
			*(uint32_t*)(buf + listAllNames.at(i).listPosInBufffer.at(j) ) = offset;			//update of the part witch will referenced the name.

		strcpy((char*)buf + offset, listAllNames.at(i).name.c_str());
		offset += listAllNames.at(i).name.length() + 1;
	}


	return offset;
}












/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/*-------------------------------------------------------------------------------\
|                             saveXml											 |
\-------------------------------------------------------------------------------*/
void FmpFile::saveXml(string filename)
{
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement* rootNode = export_Xml(filename);
	doc.LinkEndChild(rootNode);

	doc.SaveFile(filename);
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FmpFile::export_Xml(string filename)
{
	TiXmlElement* mainNode = new TiXmlElement("Fmp");
	TiXmlElement* node;
	TiXmlComment* comment;


	TiXmlElement* node_Settings = new TiXmlElement("Settings");
	
	node = new TiXmlElement("width");
	node->SetAttribute("X", FloatToString(width_X));
	node->SetAttribute("Y", FloatToString(width_Y));
	node->SetAttribute("Z", FloatToString(width_Z));
	node_Settings->LinkEndChild(node);

	node = new TiXmlElement("nearDistance"); node->SetAttribute("float", FloatToString(nearDistance));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("farDistance"); node->SetAttribute("float" , FloatToString(farDistance));		node_Settings->LinkEndChild(node);

	node = new TiXmlElement("s0a_unk_3"); node->SetAttribute("float", FloatToString(s0a_unk_3));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_4"); node->SetAttribute("float", FloatToString(s0a_unk_4));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_5"); node->SetAttribute("float", FloatToString(s0a_unk_5));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_6"); node->SetAttribute("float", FloatToString(s0a_unk_6));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_7"); node->SetAttribute("u32", s0a_unk_7);									node_Settings->LinkEndChild(node);

	node = new TiXmlElement("s0a_unk_8"); node->SetAttribute("float", FloatToString(s0a_unk_8));	comment = new TiXmlComment("0.577350 (most)  or 0.324488 (only for BFgen.map.xml)"); node_Settings->LinkEndChild(comment); node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_9"); node->SetAttribute("float", FloatToString(s0a_unk_9));	comment = new TiXmlComment("-0.577350 (most)  or -0.920775 (only for BFgen.map.xml)"); node_Settings->LinkEndChild(comment); node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_10"); node->SetAttribute("float", FloatToString(s0a_unk_10)); comment = new TiXmlComment("0.577350 (most)  or -0.216519 (only for BFgen.map.xml)"); node_Settings->LinkEndChild(comment); node_Settings->LinkEndChild(node);
	
	node = new TiXmlElement("s0a_unk_11"); node->SetAttribute("float", FloatToString(s0a_unk_11));	comment = new TiXmlComment("s0a_unk_11 to 18  look like  s0a_unk_19 to 26"); node_Settings->LinkEndChild(comment); node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_12"); node->SetAttribute("float", FloatToString(s0a_unk_12));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_13"); node->SetAttribute("float", FloatToString(s0a_unk_13));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_14"); node->SetAttribute("float", FloatToString(s0a_unk_14));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_15"); node->SetAttribute("float", FloatToString(s0a_unk_15));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_16"); node->SetAttribute("float", FloatToString(s0a_unk_16));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_17"); node->SetAttribute("float", FloatToString(s0a_unk_17));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_18"); node->SetAttribute("float", FloatToString(s0a_unk_18));	node_Settings->LinkEndChild(node);

	node = new TiXmlElement("s0a_unk_19"); node->SetAttribute("float", FloatToString(s0a_unk_19));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_20"); node->SetAttribute("float", FloatToString(s0a_unk_20));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_21"); node->SetAttribute("float", FloatToString(s0a_unk_21));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_22"); node->SetAttribute("float", FloatToString(s0a_unk_22));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_23"); node->SetAttribute("float", FloatToString(s0a_unk_23));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_24"); node->SetAttribute("float", FloatToString(s0a_unk_24));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_25"); node->SetAttribute("float", FloatToString(s0a_unk_25));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_26"); node->SetAttribute("float", FloatToString(s0a_unk_26));	node_Settings->LinkEndChild(node);

	node = new TiXmlElement("s0a_unk_27"); node->SetAttribute("float", FloatToString(s0a_unk_27));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_28"); node->SetAttribute("float", FloatToString(s0a_unk_28));	node_Settings->LinkEndChild(node);

	node = new TiXmlElement("s0a_unk_29"); node->SetAttribute("float", FloatToString(s0a_unk_29));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_30"); node->SetAttribute("float", FloatToString(s0a_unk_30));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_31"); node->SetAttribute("float", FloatToString(s0a_unk_31));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0a_unk_34"); node->SetAttribute("float", FloatToString(s0a_unk_34));	comment = new TiXmlComment("0.785398 (most) or 0.698132 (only on 3 files)"); node_Settings->LinkEndChild(comment); node_Settings->LinkEndChild(node);




	node = new TiXmlElement("s0b_unk_0"); node->SetAttribute("u32", UnsignedToString(s0b_unk_0, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_1"); node->SetAttribute("u32", UnsignedToString(s0b_unk_1, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_2"); node->SetAttribute("u32", UnsignedToString(s0b_unk_2, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_3"); node->SetAttribute("u32", UnsignedToString(s0b_unk_3, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_4"); node->SetAttribute("u32", UnsignedToString(s0b_unk_4, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_5"); node->SetAttribute("u32", UnsignedToString(s0b_unk_5, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_6"); node->SetAttribute("u32", UnsignedToString(s0b_unk_6, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_7"); node->SetAttribute("u32", UnsignedToString(s0b_unk_7, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_8"); node->SetAttribute("u32", UnsignedToString(s0b_unk_8, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_9"); node->SetAttribute("u32", UnsignedToString(s0b_unk_9, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_10"); node->SetAttribute("u32", UnsignedToString(s0b_unk_10, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_11"); node->SetAttribute("u32", UnsignedToString(s0b_unk_11, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_12"); node->SetAttribute("u32", UnsignedToString(s0b_unk_12, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_13"); node->SetAttribute("u32", UnsignedToString(s0b_unk_13, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_14"); node->SetAttribute("u32", UnsignedToString(s0b_unk_14, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_15"); node->SetAttribute("u32", UnsignedToString(s0b_unk_15, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_16"); node->SetAttribute("u32", UnsignedToString(s0b_unk_16, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_17"); node->SetAttribute("u32", UnsignedToString(s0b_unk_17, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_18"); node->SetAttribute("u32", UnsignedToString(s0b_unk_18, true));	comment = new TiXmlComment("0x9000 (most) or 0, flags"); node_Settings->LinkEndChild(comment); node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_19"); node->SetAttribute("u32", UnsignedToString(s0b_unk_19, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_20"); node->SetAttribute("u32", UnsignedToString(s0b_unk_20, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_21"); node->SetAttribute("u32", UnsignedToString(s0b_unk_21, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_22"); node->SetAttribute("u32", UnsignedToString(s0b_unk_22, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_23"); node->SetAttribute("u16", UnsignedToString(s0b_unk_23, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_23b"); node->SetAttribute("u16", UnsignedToString(s0b_unk_23b, true));	comment = new TiXmlComment("0x2 (most) or 0"); node_Settings->LinkEndChild(comment); node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_24"); node->SetAttribute("u32", UnsignedToString(s0b_unk_24, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_25"); node->SetAttribute("u32", UnsignedToString(s0b_unk_25, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_26"); node->SetAttribute("u32", UnsignedToString(s0b_unk_26, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_27"); node->SetAttribute("u32", UnsignedToString(s0b_unk_27, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_28"); node->SetAttribute("u32", UnsignedToString(s0b_unk_28, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_29"); node->SetAttribute("u32", UnsignedToString(s0b_unk_29, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_30"); node->SetAttribute("u32", UnsignedToString(s0b_unk_30, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_31"); node->SetAttribute("u32", UnsignedToString(s0b_unk_31, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_32"); node->SetAttribute("u32", UnsignedToString(s0b_unk_32, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_33"); node->SetAttribute("u32", UnsignedToString(s0b_unk_33, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_34"); node->SetAttribute("u32", UnsignedToString(s0b_unk_34, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_35"); node->SetAttribute("u32", UnsignedToString(s0b_unk_35, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_36"); node->SetAttribute("u32", UnsignedToString(s0b_unk_36, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_37"); node->SetAttribute("u32", UnsignedToString(s0b_unk_37, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_38"); node->SetAttribute("u32", UnsignedToString(s0b_unk_38, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_39"); node->SetAttribute("u32", UnsignedToString(s0b_unk_39, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_40"); node->SetAttribute("u32", UnsignedToString(s0b_unk_40, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_41"); node->SetAttribute("u32", UnsignedToString(s0b_unk_41, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_42"); node->SetAttribute("u32", UnsignedToString(s0b_unk_42, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_43"); node->SetAttribute("u32", UnsignedToString(s0b_unk_43, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_44"); node->SetAttribute("u32", UnsignedToString(s0b_unk_44, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_45"); node->SetAttribute("u32", UnsignedToString(s0b_unk_45, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_46"); node->SetAttribute("u32", UnsignedToString(s0b_unk_46, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_47"); node->SetAttribute("u32", UnsignedToString(s0b_unk_47, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_48"); node->SetAttribute("u32", UnsignedToString(s0b_unk_48, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_49"); node->SetAttribute("u32", UnsignedToString(s0b_unk_49, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_50"); node->SetAttribute("u32", UnsignedToString(s0b_unk_50, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_51"); node->SetAttribute("u32", UnsignedToString(s0b_unk_51, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_52"); node->SetAttribute("u32", UnsignedToString(s0b_unk_52, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_53"); node->SetAttribute("u32", UnsignedToString(s0b_unk_53, true));	node_Settings->LinkEndChild(node);
	node = new TiXmlElement("s0b_unk_54"); node->SetAttribute("u32", UnsignedToString(s0b_unk_54, true));	node_Settings->LinkEndChild(node);
	


	TiXmlElement* node_Type_names = new TiXmlElement("ItemTypes");
	size_t nbType = mListTypeName.size();
	for (size_t i = 0; i < nbType; i++)
	{
		node = new TiXmlElement("ItemType"); node->SetAttribute("name", mListTypeName.at(i)); comment = new TiXmlComment( ("index : "+ std::to_string(i)).c_str() ); node_Type_names->LinkEndChild(comment); node_Type_names->LinkEndChild(node);
	}
	node_Settings->LinkEndChild(node_Type_names);
	

	mainNode->LinkEndChild(node_Settings);







	node = new TiXmlElement("ListSection1");
	size_t nbS1 = mListS1.size();
	for (size_t i = 0; i < nbS1; i++)
	{
		comment = new TiXmlComment(("index : " + std::to_string(i)).c_str()); node->LinkEndChild(comment);
		node->LinkEndChild(mListS1.at(i).export_Xml(i==0));
	}
	mainNode->LinkEndChild(node);


	node = new TiXmlElement("ListSection2");
	size_t nbS2 = mListS2.size();
	for (size_t i = 0; i < nbS2; i++)
	{
		comment = new TiXmlComment(("index : " + std::to_string(i)).c_str()); node->LinkEndChild(comment);
		node->LinkEndChild(mListS2.at(i).export_Xml(i == 0));
	}
	mainNode->LinkEndChild(node);




	TiXmlElement* node_groups = new TiXmlElement("ListFragmentGroup");
	TiXmlElement* node_group;
	size_t nbfg = mListFragmentsGroups.size();
	for (size_t i = 0; i < nbfg; i++)
	{
		comment = new TiXmlComment(("index : " + std::to_string(i)).c_str()); node_groups->LinkEndChild(comment);

		node_group = new TiXmlElement("Group"); 
		node_group->SetAttribute("name", mListFragmentsGroups.at(i).name);
		
		size_t nbObj = mListFragmentsGroups.at(i).listObjects.size();
		for (size_t j=0;j<nbObj;j++)
		{
			node = new TiXmlElement("ObjectIndex"); node->SetAttribute("value", mListFragmentsGroups.at(i).listObjects.at(j));	node_group->LinkEndChild(node);
		}

		node_groups->LinkEndChild(node_group);
	}
	mainNode->LinkEndChild(node_groups);






	node = new TiXmlElement("ListObject");
	size_t nbObj = mListObject.size();
	for (size_t i = 0; i < nbObj; i++)
	{
		comment = new TiXmlComment(("index : " + std::to_string(i)).c_str()); node->LinkEndChild(comment);
		node->LinkEndChild(mListObject.at(i)->export_Xml());
	}
	mainNode->LinkEndChild(node);



	node = new TiXmlElement("ListHitboxGroup");
	size_t nbHbg = mListHitboxGroup.size();
	for (size_t i = 0; i < nbHbg; i++)
	{
		comment = new TiXmlComment(("index : "+ std::to_string(i)).c_str()); node->LinkEndChild(comment);
		node->LinkEndChild(mListHitboxGroup.at(i).export_Xml(filename, mListHavokFile, i));
	}
	mainNode->LinkEndChild(node);












	TiXmlElement* node_listFile = new TiXmlElement("ListFiles");
	comment = new TiXmlComment("Notice: all this part is not necessary because the tool add the files used for Objects. it's more to keep filenames order for binary compare"); node_listFile->LinkEndChild(comment);

	TiXmlElement* node_listNsk = new TiXmlElement("ListNsk");
	size_t nbNsk = listNsk.size();
	for (size_t i = 0; i < nbNsk; i++)
	{
		node = new TiXmlElement("File"); node->SetAttribute("name", listNsk.at(i)); node_listNsk->LinkEndChild(node);
	}
	node_listFile->LinkEndChild(node_listNsk);


	TiXmlElement* node_listTextureEmb = new TiXmlElement("ListTextureEmb");
	size_t nbTE = listTextureEmb.size();
	for (size_t i = 0; i < nbTE; i++)
	{
		node = new TiXmlElement("File"); node->SetAttribute("name", listTextureEmb.at(i)); node_listTextureEmb->LinkEndChild(node);
	}
	node_listFile->LinkEndChild(node_listTextureEmb);


	TiXmlElement* node_listEmm = new TiXmlElement("ListEmm");
	size_t nbEmm = listEmm.size();
	for (size_t i = 0; i < nbEmm; i++)
	{
		node = new TiXmlElement("File"); node->SetAttribute("name", listEmm.at(i)); node_listEmm->LinkEndChild(node);
	}
	node_listFile->LinkEndChild(node_listEmm);



	TiXmlElement* node_listEma = new TiXmlElement("ListEma");
	size_t nbEma = listEma.size();
	for (size_t i = 0; i < nbEma; i++)
	{
		node = new TiXmlElement("File"); node->SetAttribute("name", listEma.at(i)); node_listEma->LinkEndChild(node);
	}
	node_listFile->LinkEndChild(node_listEma);

	mainNode->LinkEndChild(node_listFile);


	
	return mainNode;
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_S1::export_Xml(bool withComments)
{
	TiXmlElement* mainNode = new TiXmlElement("Section1");
	TiXmlElement* node;
	TiXmlComment* comment;

	node = new TiXmlElement("name"); node->SetAttribute("value", name);  mainNode->LinkEndChild(node);

	node = new TiXmlElement("unk_04"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_04, false));  if (withComments) { comment = new TiXmlComment("0 (most), 1, 2, 3. we have 0 for Default and '__X'"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_08"); node->SetAttribute("float", FloatToString(unk_08));  if (withComments) { comment = new TiXmlComment("values are in [0, 1.0], 0.5 is the most"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_0C"); node->SetAttribute("float", FloatToString(unk_0C));  if (withComments) { comment = new TiXmlComment("same as unk_08"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_10"); node->SetAttribute("float", FloatToString(unk_10));  if (withComments) { comment = new TiXmlComment("values are in [0, 1.0], 0.0 is the most"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_14"); node->SetAttribute("float", FloatToString(unk_14));  if (withComments) { comment = new TiXmlComment("values are 999936.0 (most), 0.0 or 0.104279."); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);

	return mainNode;
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_S2::export_Xml(bool withComments)
{
	TiXmlElement* mainNode = new TiXmlElement("Section2");
	TiXmlElement* node;
	TiXmlComment* comment;

	node = new TiXmlElement("name"); node->SetAttribute("value", name);  mainNode->LinkEndChild(node);

	node = new TiXmlElement("unk_04"); node->SetAttribute("float", FloatToString(unk_04));  if (withComments) { comment = new TiXmlComment("0.0, 1.0 (most), 2.0 or 3.0. we feel 0 and 1, 2 and 3, 4 and 5, are together."); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_08"); node->SetAttribute("float", FloatToString(unk_08));  if (withComments) { comment = new TiXmlComment("is always 1.0"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_0C"); node->SetAttribute("float", FloatToString(unk_0C));  if (withComments) { comment = new TiXmlComment("200.0 (most) or 300.0"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_10"); node->SetAttribute("float", FloatToString(unk_10));  if (withComments) { comment = new TiXmlComment("100.0 (most), or 300.0."); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_14"); node->SetAttribute("float", FloatToString(unk_14));  if (withComments) { comment = new TiXmlComment("is in [0,2.0], 0.0 at most"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_18"); node->SetAttribute("float", FloatToString(unk_18));  if (withComments) { comment = new TiXmlComment("is in [0, 10.0], 0.0 at most"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);

	return mainNode;
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_Object::export_Xml(bool withComments)
{
	TiXmlElement* mainNode = new TiXmlElement("Object");
	TiXmlElement* node;
	TiXmlComment* comment;

	comment = new TiXmlComment(("debug_startOffset :" + EMO_BaseFile::UnsignedToString(debug_startOffset, true)).c_str()); mainNode->LinkEndChild(comment);		//todo remove	
	
	node = new TiXmlElement("name"); node->SetAttribute("value", name);  mainNode->LinkEndChild(node);
	node = new TiXmlElement("hitboxGroupIndex"); node->SetAttribute("value", EMO_BaseFile::UnsignedToString(hitboxGroupIndex, false)); mainNode->LinkEndChild(node);


	node = new TiXmlElement("unk_04"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(unk_04, true));  if (withComments) { comment = new TiXmlComment("0(most), 1"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_04b"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(unk_04b, true));  if (withComments) { comment = new TiXmlComment("0, 1 (most)"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_0A"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_0A, true));  if (withComments) { comment = new TiXmlComment("0, 1 (most), 2, 3, 5, 7, 0x11, 0x13, 0x15, 0x2a, 0x49, 0x4b. may be 2 x uint4"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_0B"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_0B, true));  if (withComments) { comment = new TiXmlComment("always 0"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_20"); node->SetAttribute("float", FloatToString(unk_20));  if (withComments) { comment = new TiXmlComment("0.0, 0.000002, 2.0 (most)"); mainNode->LinkEndChild(comment);  }  mainNode->LinkEndChild(node);


	node = new TiXmlElement("ListEntity");
	size_t nbEnt = listEntity.size();
	for (size_t i = 0; i < nbEnt; i++)
	{
		comment = new TiXmlComment(("index : " + std::to_string(i)).c_str()); node->LinkEndChild(comment);
		node->LinkEndChild(listEntity.at(i).export_Xml(i==0));
	}
	mainNode->LinkEndChild(node);


	node = new TiXmlElement("ListVirtualSubPart");
	size_t nbVsp = listVirtualSubParts.size();
	for (size_t i = 0; i < nbVsp; i++)
	{
		comment = new TiXmlComment(("index : " + std::to_string(i)).c_str()); node->LinkEndChild(comment);
		node->LinkEndChild(listVirtualSubParts.at(i)->export_Xml(i==0));
	}
	mainNode->LinkEndChild(node);


	if(action)
		mainNode->LinkEndChild(action->export_Xml(true));

	if (hierarchy)
		mainNode->LinkEndChild(hierarchy->export_Xml());


	TiXmlElement* node_matrix = new TiXmlElement("TransformMatrix4x3");
	if (withComments) { comment = new TiXmlComment("Future version will have position, rotation (degree), and scale. Here the easy part (without knowing matrix) is last line is positionXYZ."); node_matrix->LinkEndChild(comment); }
	node = new TiXmlElement("L0"); node->SetAttribute("f32s", FloatToString(transformMatrix4x3[0]) + ", " + FloatToString(transformMatrix4x3[1]) + ", " + FloatToString(transformMatrix4x3[2])); node_matrix->LinkEndChild(node);
	node = new TiXmlElement("L1"); node->SetAttribute("f32s", FloatToString(transformMatrix4x3[3]) + ", " + FloatToString(transformMatrix4x3[4]) + ", " + FloatToString(transformMatrix4x3[5])); node_matrix->LinkEndChild(node);
	node = new TiXmlElement("L2"); node->SetAttribute("f32s", FloatToString(transformMatrix4x3[6]) + ", " + FloatToString(transformMatrix4x3[7]) + ", " + FloatToString(transformMatrix4x3[8])); node_matrix->LinkEndChild(node);
	node = new TiXmlElement("L3"); node->SetAttribute("f32s", FloatToString(transformMatrix4x3[9]) + ", " + FloatToString(transformMatrix4x3[10]) + ", " + FloatToString(transformMatrix4x3[11])); node_matrix->LinkEndChild(node);
	mainNode->LinkEndChild(node_matrix);

	return mainNode;
}




/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_Action::export_Xml(bool withComments)
{
	TiXmlElement* mainNode = new TiXmlElement("Action");
	TiXmlElement* node;
	TiXmlComment* comment;

	comment = new TiXmlComment(("debug_startOffset :" + EMO_BaseFile::UnsignedToString(debug_startOffset, true)).c_str()); mainNode->LinkEndChild(comment);		//todo remove

	node = new TiXmlElement("unk_0"); node->SetAttribute("u32", unk_0);  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_1a"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_1a, true)); mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_1b"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_1b, true));	if (withComments) { comment = new TiXmlComment("only 0 (most), 0x10, 0x20, 0x30, 0x40, 0x70"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_1c"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_1c, true));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_1d"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_1d, true));  if (withComments) { comment = new TiXmlComment("only 0 (most) or 0x80"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);


	node = new TiXmlElement("ListCommand");
	size_t nbCom = listCommands.size();
	for (size_t i = 0; i < nbCom; i++)
		node->LinkEndChild(listCommands.at(i).export_Xml(i==0));
	mainNode->LinkEndChild(node);

	return mainNode;
}

/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_Action::Command::export_Xml(bool withComments)
{
	TiXmlElement* mainNode = new TiXmlElement("Command");
	TiXmlElement* node;
	TiXmlComment* comment;

	node = new TiXmlElement("function"); node->SetAttribute("name", functionName);  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_1a"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_1a, true));	if (withComments) { comment = new TiXmlComment("0, 1, 2, 3 (most), 4"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_1b"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_1b, true));	mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_1c"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_1c, true));  if (withComments) { comment = new TiXmlComment("seam to be 2 x uint4 , with one is flags: 0, 4, 5, 7, 0x10, 0x64, 0x65, 0x66, 0x67, 0x68, 0x6a, 0x6b, 0x6c, 0x6e, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0xc8, 0xca, 0xcc, 0xcd, 0xd1, 0xd2, 0xd3, 0xf8, 0xfb"); mainNode->LinkEndChild(comment); }	mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_1d"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_1d, true));  if (withComments) { comment = new TiXmlComment("0 (most) or 0xff"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);


	node = new TiXmlElement("ListParameter");
	size_t nbParam = parameters.size();
	for (size_t i = 0; i < nbParam; i++)
		node->LinkEndChild(parameters.at(i).export_Xml());
	mainNode->LinkEndChild(node);


	return mainNode;
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_Parameter::export_Xml()
{
	TiXmlElement* mainNode = new TiXmlElement("Parameter");

	mainNode->SetAttribute("name", name);

	switch (typeValue)
	{
	case 0:
	{
		mainNode->SetAttribute("type", "Bool");
		mainNode->SetAttribute("value", boolValue ? "True": "False");
	}break;

	case 1:
	{
		mainNode->SetAttribute("type", "Int");
		mainNode->SetAttribute("value", intValue);
	}break;

	case 2:
	{
		mainNode->SetAttribute("type", "Float");
		mainNode->SetAttribute("value", FloatToString(floatValue));
	}break;

	case 3:
	{
		mainNode->SetAttribute("type", "Direction");
		mainNode->SetAttribute("X", FloatToString(directionValue[0]));
		mainNode->SetAttribute("Y", FloatToString(directionValue[1]));
		mainNode->SetAttribute("Z", FloatToString(directionValue[2]));
		mainNode->SetAttribute("W", FloatToString(directionValue[3]));
	}break;

	case 4:
	{
		mainNode->SetAttribute("type", "Position");
		mainNode->SetAttribute("X", FloatToString(positionValue[0]));
		mainNode->SetAttribute("Y", FloatToString(positionValue[1]));
		mainNode->SetAttribute("Z", FloatToString(positionValue[2]));
		mainNode->SetAttribute("W", FloatToString(positionValue[3]));
	}break;

	
	case 6:
	{
		mainNode->SetAttribute("type", "String");
		mainNode->SetAttribute("value", stringValue);
	}break;

	case 8:
	{
		mainNode->SetAttribute("type", "Uint");
		mainNode->SetAttribute("value", uintValue);
	}break;

	}

	return mainNode;
}




/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_Hierarchy::export_Xml()
{
	TiXmlElement* mainNode = new TiXmlElement("Hierarchy");
	TiXmlElement* node;
	TiXmlComment* comment;

	comment = new TiXmlComment(("debug_startOffset :" + EMO_BaseFile::UnsignedToString(debug_startOffset, true)).c_str()); mainNode->LinkEndChild(comment);	//todo remove

	node = new TiXmlElement("ListNode");
	size_t nbNode = listNode.size();
	for (size_t i = 0; i < nbNode; i++)
	{
		comment = new TiXmlComment(("index : " + std::to_string(i)).c_str()); node->LinkEndChild(comment);
		node->LinkEndChild(listNode.at(i).export_Xml(i == 0));
	}
	mainNode->LinkEndChild(node);

	node = new TiXmlElement("ListTransform");
	size_t nbTrans = listTransform.size();
	for (size_t i = 0; i < nbTrans; i++)
	{
		comment = new TiXmlComment(("index : " + std::to_string(i)).c_str()); node->LinkEndChild(comment);
		node->LinkEndChild(listTransform.at(i).export_Xml());
	}
	mainNode->LinkEndChild(node);

	if(mRoot)
		mainNode->LinkEndChild(mRoot->export_Xml(true));						//recursive

	return mainNode;
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_Node::export_Xml(bool withComments)
{
	TiXmlElement* mainNode = new TiXmlElement("Node");
	TiXmlElement* node;
	TiXmlComment* comment;

	node = new TiXmlElement("transformIndex"); node->SetAttribute("value", transformIndex);  mainNode->LinkEndChild(node);

	node = new TiXmlElement("unk_0"); node->SetAttribute("float", FloatToString(unk_0));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_1"); node->SetAttribute("float", FloatToString(unk_1));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_2"); node->SetAttribute("float", FloatToString(unk_2));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_3"); node->SetAttribute("float", FloatToString(unk_3));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_4"); node->SetAttribute("float", FloatToString(unk_4));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_5"); node->SetAttribute("float", FloatToString(unk_5));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_6"); node->SetAttribute("float", FloatToString(unk_6));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_7"); node->SetAttribute("float", FloatToString(unk_7));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_8"); node->SetAttribute("float", FloatToString(unk_8));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_9"); node->SetAttribute("float", FloatToString(unk_9));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_10"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_10, true));  if (withComments) { comment = new TiXmlComment("few hole in [1(most), 0x2f] go up to 0xef"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);

	return mainNode;
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_NodeTransform::export_Xml()
{
	TiXmlElement* mainNode = new TiXmlElement("Transform");
	TiXmlElement* node;
	
	node = new TiXmlElement("Position"); 
	node->SetAttribute("X", FloatToString(position[0]));
	node->SetAttribute("Y", FloatToString(position[1]));
	node->SetAttribute("Z", FloatToString(position[2]));
	mainNode->LinkEndChild(node);

	node = new TiXmlElement("Rotation");
	node->SetAttribute("X", FloatToString(rotation[0]));
	node->SetAttribute("Y", FloatToString(rotation[1]));
	node->SetAttribute("Z", FloatToString(rotation[2]));
	mainNode->LinkEndChild(node);

	node = new TiXmlElement("Scale");
	node->SetAttribute("X", FloatToString(scale[0]));
	node->SetAttribute("Y", FloatToString(scale[1]));
	node->SetAttribute("Z", FloatToString(scale[2]));
	mainNode->LinkEndChild(node);
	
	return mainNode;
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_HierarchyNode::export_Xml(bool withComments)
{
	TiXmlElement* mainNode = new TiXmlElement("HierarchyNode");
	TiXmlElement* node;
	TiXmlComment* comment;


	node = new TiXmlElement("typeb"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(typeb, true)); if (withComments) { comment = new TiXmlComment("only 0 (most), 0x20, 0x30 or 0x60 "); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_0"); node->SetAttribute("float", FloatToString(unk_0));  if(withComments) { comment = new TiXmlComment("unk_0 to 2 is only used when there is some Child, may be a position"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_1"); node->SetAttribute("float", FloatToString(unk_1));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_2"); node->SetAttribute("float", FloatToString(unk_2));  mainNode->LinkEndChild(node);
	

	if (listIndex.size())
	{
		TiXmlElement* node_listIndex = new TiXmlElement("ListIndex");
		if (withComments) { comment = new TiXmlComment("may be index for Node in Hierarchy"); mainNode->LinkEndChild(comment); }

		size_t nbIndex = listIndex.size();
		for (size_t i = 0; i < nbIndex; i++)
		{
			node = new TiXmlElement("Index"); node->SetAttribute("u16", listIndex.at(i));  node_listIndex->LinkEndChild(node);
		}
		mainNode->LinkEndChild(node_listIndex);
	}

	if (listChild.size())
	{
		node = new TiXmlElement("Child");
		size_t nbChild = listChild.size();
		for (size_t i = 0; i < nbChild; i++)
		{
			if (listChild.at(i))
				node->LinkEndChild(listChild.at(i)->export_Xml());
			else
				node->LinkEndChild(new TiXmlElement("HierarchyNode"));
		}
		mainNode->LinkEndChild(node);
	}

	return mainNode;
}









/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_Entity::export_Xml(bool withComments)
{
	TiXmlElement* mainNode = new TiXmlElement("Entity");
	TiXmlElement* node;
	TiXmlComment* comment;

	comment = new TiXmlComment(("debug_startOffset :" + EMO_BaseFile::UnsignedToString(debug_startOffset, true)).c_str()); mainNode->LinkEndChild(comment);	//todo remove.

	node = new TiXmlElement("s4n2_unk_0"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(s4n2_unk_0, true)); if (withComments) { comment = new TiXmlComment("few holes in [0 (most), 0x11], go up to 0x74"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);

	if (haveVisualPart)
	{
		TiXmlElement* node_Visual = new TiXmlElement("Visual");

		node = new TiXmlElement("name"); node->SetAttribute("value", name);  node_Visual->LinkEndChild(node);

		TiXmlElement* node_Lod = new TiXmlElement("ListLod");

		size_t nbLod = listLod.size();
		for (size_t i = 0; i < nbLod; i++)
		{
			node = new TiXmlElement("Lod");
			node->SetAttribute("distance", std::to_string(listLod.at(i).distance));
			node->SetAttribute("Nsk", listLod.at(i).nsk_filename);
			node->SetAttribute("Emm", listLod.at(i).emm_filename);
			node_Lod->LinkEndChild(node);
		}
		node_Visual->LinkEndChild(node_Lod);

		node = new TiXmlElement("Emb"); node->SetAttribute("filename", embTexture_filename);  node_Visual->LinkEndChild(node);
		node = new TiXmlElement("Ema"); node->SetAttribute("filename", ema_filename);  node_Visual->LinkEndChild(node);

		node = new TiXmlElement("s4n2b_unk_0"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(s4n2b_unk_0, true));  if(withComments) { comment = new TiXmlComment("0 (most), 4"); node_Visual->LinkEndChild(comment); }  node_Visual->LinkEndChild(node);
		node = new TiXmlElement("s4n2b_unk_5"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(s4n2b_unk_5, true));  node_Visual->LinkEndChild(node);
		node = new TiXmlElement("s4n2b_unk_6"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(s4n2b_unk_6, true));  node_Visual->LinkEndChild(node);
		node = new TiXmlElement("s4n2b_unk_8"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(s4n2b_unk_8, true));  if (withComments) { comment = new TiXmlComment("only 2, 3 or 0xffffffff (none and most). sound like another index to something, may be twice animation."); node_Visual->LinkEndChild(comment); }  node_Visual->LinkEndChild(node);
		node = new TiXmlElement("s4n2b_unk_9"); node->SetAttribute("float", FloatToString(s4n2b_unk_9));  node_Visual->LinkEndChild(node);
		node = new TiXmlElement("s4n2b_unk_10"); node->SetAttribute("float", FloatToString(s4n2b_unk_10));  node_Visual->LinkEndChild(node);

		mainNode->LinkEndChild(node_Visual);
	}


	TiXmlElement* node_matrix = new TiXmlElement("TransformMatrix4x3");
	if (withComments) { comment = new TiXmlComment("Future version will have position, rotation (degree), and scale. Here the easy part (without knowing matrix) is last line is positionXYZ."); node_matrix->LinkEndChild(comment); }
	node = new TiXmlElement("L0"); node->SetAttribute("f32s", FloatToString(transformMatrix4x3[0]) + ", " + FloatToString(transformMatrix4x3[1]) + ", " + FloatToString(transformMatrix4x3[2])); node_matrix->LinkEndChild(node);
	node = new TiXmlElement("L1"); node->SetAttribute("f32s", FloatToString(transformMatrix4x3[3]) + ", " + FloatToString(transformMatrix4x3[4]) + ", " + FloatToString(transformMatrix4x3[5])); node_matrix->LinkEndChild(node);
	node = new TiXmlElement("L2"); node->SetAttribute("f32s", FloatToString(transformMatrix4x3[6]) + ", " + FloatToString(transformMatrix4x3[7]) + ", " + FloatToString(transformMatrix4x3[8])); node_matrix->LinkEndChild(node);
	node = new TiXmlElement("L3"); node->SetAttribute("f32s", FloatToString(transformMatrix4x3[9]) + ", " + FloatToString(transformMatrix4x3[10]) + ", " + FloatToString(transformMatrix4x3[11])); node_matrix->LinkEndChild(node);
	mainNode->LinkEndChild(node_matrix);

	return mainNode;
}









/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_VirtualSubPart::export_Xml(bool withComments)
{
	TiXmlElement* mainNode = new TiXmlElement("VirtualSubPart");
	TiXmlElement* node;
	TiXmlComment* comment;

	comment = new TiXmlComment( ("debug_startOffset :" + EMO_BaseFile::UnsignedToString(debug_startOffset, true)).c_str() ); mainNode->LinkEndChild(comment);		//todo remove
	
	node = new TiXmlElement("unk_2"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(unk_2, true));  if (withComments) { comment = new TiXmlComment("only 0x0 (most), 0x1"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_2b"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(unk_2b, true));  if (withComments) { comment = new TiXmlComment("only 0, 0xffff (most)"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_4"); node->SetAttribute("float", FloatToString(unk_4));  if (withComments) { comment = new TiXmlComment("[0.0 (most), 1000000.0]"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_5"); node->SetAttribute("float", FloatToString(unk_5));  if (withComments) { comment = new TiXmlComment("[0.0 (most), 3.0]"); mainNode->LinkEndChild(comment);  } mainNode->LinkEndChild(node);


	if (listIndex.size())
	{
		TiXmlElement* node_listIndex = new TiXmlElement("ListIndexCouple");

		size_t nbIndex = listIndex.size();
		for (size_t i = 0; i < nbIndex; i++)
		{
			comment = new TiXmlComment(("index : " + std::to_string(i)).c_str()); node_listIndex->LinkEndChild(comment);
			node = new TiXmlElement("IndexCouple"); 
			node->SetAttribute("Index0", listIndex.at(i).at(0)); 
			node->SetAttribute("Index1", listIndex.at(i).at(1));
			node_listIndex->LinkEndChild(node);
		}
		mainNode->LinkEndChild(node_listIndex);
	}

	if (sub0)
		mainNode->LinkEndChild(sub0->export_Xml(true));
	else
		mainNode->LinkEndChild(new TiXmlElement("SubPart"));									//empty to make the difference with Sub1

	if (sub1)
		mainNode->LinkEndChild(sub1->export_Xml(true));

	if (action)
		mainNode->LinkEndChild(action->export_Xml());


	TiXmlElement* node_matrix = new TiXmlElement("TransformMatrix4x3");
	if (withComments) { comment = new TiXmlComment("Future version will have position, rotation (degree), and scale. Here the easy part (without knowing matrix) is last line is positionXYZ."); node_matrix->LinkEndChild(comment); }
	node = new TiXmlElement("L0"); node->SetAttribute("f32s", FloatToString(transformMatrix4x3[0]) + ", " + FloatToString(transformMatrix4x3[1]) + ", " + FloatToString(transformMatrix4x3[2])); node_matrix->LinkEndChild(node);
	node = new TiXmlElement("L1"); node->SetAttribute("f32s", FloatToString(transformMatrix4x3[3]) + ", " + FloatToString(transformMatrix4x3[4]) + ", " + FloatToString(transformMatrix4x3[5])); node_matrix->LinkEndChild(node);
	node = new TiXmlElement("L2"); node->SetAttribute("f32s", FloatToString(transformMatrix4x3[6]) + ", " + FloatToString(transformMatrix4x3[7]) + ", " + FloatToString(transformMatrix4x3[8])); node_matrix->LinkEndChild(node);
	node = new TiXmlElement("L3"); node->SetAttribute("f32s", FloatToString(transformMatrix4x3[9]) + ", " + FloatToString(transformMatrix4x3[10]) + ", " + FloatToString(transformMatrix4x3[11])); node_matrix->LinkEndChild(node);
	mainNode->LinkEndChild(node_matrix);

	return mainNode;
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_VirtualSubPart_sub::export_Xml(bool withComments)
{
	TiXmlElement* mainNode = new TiXmlElement("SubPart");
	TiXmlElement* node;
	TiXmlComment* comment;


	node = new TiXmlElement("Width");
	node->SetAttribute("X", FloatToString(widthX));
	node->SetAttribute("Y", FloatToString(widthY));
	node->SetAttribute("Z", FloatToString(widthZ));
	mainNode->LinkEndChild(node);

	if (withComments) { comment = new TiXmlComment("Future version will use Rotation degrees"); mainNode->LinkEndChild(comment); }
	node = new TiXmlElement("Quaternion");
	node->SetAttribute("X", FloatToString(quaternionX));
	node->SetAttribute("Y", FloatToString(quaternionY));
	node->SetAttribute("Z", FloatToString(quaternionZ));
	node->SetAttribute("W", FloatToString(quaternionW));
	mainNode->LinkEndChild(node);

	node = new TiXmlElement("unk_0"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(unk_0, true));  if (withComments) { comment = new TiXmlComment("few holes in [0, 0xe] , 4 at most"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_0b"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(unk_0b, true));  if (withComments) { comment = new TiXmlComment("[0,7], plus 0xffff (most) for none"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_1"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(unk_1, true));  if (withComments) { comment = new TiXmlComment("0 (most), 1, 3, 7, 0xff "); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_1b"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(unk_1b, true));  if (withComments) { comment = new TiXmlComment("0, 2 (most)"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_2"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_2, true));  if (withComments) { comment = new TiXmlComment("0, 2, 4, 5, 9, 0x19, 0xffffffff (most)"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);

	node = new TiXmlElement("unk_3"); node->SetAttribute("float", FloatToString(unk_3));  if (withComments) { comment = new TiXmlComment("[0.031900, 24098666.0], 1.0 at most. lots of uniques values, and progress, like a animation"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_4"); node->SetAttribute("float", FloatToString(unk_4));  if (withComments) { comment = new TiXmlComment("[-184.657913, 186.816162], 0.0 at most. could be some degrees ?"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_5"); node->SetAttribute("float", FloatToString(unk_5));  if (withComments) { comment = new TiXmlComment("[-175.524277, 188.422974], 0.000000 (most) "); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_6"); node->SetAttribute("float", FloatToString(unk_6));  if (withComments) { comment = new TiXmlComment("[-186.247391, 176.994080], 0.0 (most)"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_7"); node->SetAttribute("float", FloatToString(unk_7));  if (withComments) { comment = new TiXmlComment("[-192.804337, 140.889252], 0.0 (most)"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_8"); node->SetAttribute("float", FloatToString(unk_8));  if (withComments) { comment = new TiXmlComment("[-106.711281, 114.855453], 0.0 (most)"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_9"); node->SetAttribute("float", FloatToString(unk_9));  if (withComments) { comment = new TiXmlComment("[-185.846497, 198.041656] 0.0 at most"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_10"); node->SetAttribute("float", FloatToString(unk_10));  if (withComments) { comment = new TiXmlComment("[-279.467529, 259.390625], 0.0 (most). lots of values, look like for animation");  mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_11"); node->SetAttribute("float", FloatToString(unk_11));  if (withComments) { comment = new TiXmlComment("[-260.461700, 122.487022], 0.0 (most). lots of values"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_12"); node->SetAttribute("float", FloatToString(unk_12));  if (withComments) { comment = new TiXmlComment("[-463.797577, 387.593781], 0.0 (most). lots of values"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_13"); node->SetAttribute("float", FloatToString(unk_13));  if (withComments) { comment = new TiXmlComment("[0.0, 841637952.0], 1.0 (most). lots of values near 0. there also have strange too high values");  mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	
	return mainNode;
}


















/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_HitboxGroup::export_Xml(string filename, std::vector<Havok_File*> &listHavokFile, size_t indexGroup)
{
	
	TiXmlElement* mainNode = new TiXmlElement("HitboxGroup");
	TiXmlElement* node;
	TiXmlComment* comment;

	node = new TiXmlElement("name"); node->SetAttribute("value", name);  mainNode->LinkEndChild(node);

	node = new TiXmlElement("ListHitbox");
	size_t nbChild = listHitbox.size();
	for (size_t i = 0; i < nbChild; i++)
	{
		comment = new TiXmlComment(("index : "+ std::to_string(i)).c_str()); node->LinkEndChild(comment);
		node->LinkEndChild(listHitbox.at(i).export_Xml(filename, listHavokFile, indexGroup, i, i==0));
	}
	mainNode->LinkEndChild(node);

	return mainNode;
}

/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_Hitbox::export_Xml(string filename, std::vector<Havok_File*> &listHavokFile, size_t indexGroup, size_t indexHitbox, bool withComments)
{

	TiXmlElement* mainNode = new TiXmlElement("Hitbox");
	TiXmlElement* node;
	TiXmlComment* comment;


	node = new TiXmlElement("name"); node->SetAttribute("value", name);  mainNode->LinkEndChild(node);
	node = new TiXmlElement("child"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(child, true));  if(withComments) { comment = new TiXmlComment("holes in [1, 0xf7], and go to 0x43a. 0xffff (none and most) the holes are explain by sibling"); mainNode->LinkEndChild(comment);  }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_a0"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(unk_a0, true));  if (withComments) { comment = new TiXmlComment("only 0 or 0xffff (most)"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("sibling"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(sibling, true));  if (withComments) { comment = new TiXmlComment("[2, 0x43d], 0xffff (none and most). seam to confirm sibling, with relation of child"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("parent"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(parent, true));  if (withComments) { comment = new TiXmlComment("only 0x0 (most), 0xffff (none). So could be like unk_a0 or all have only 0 as parent ? "); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);


	TiXmlElement* node_ListOnDes = new TiXmlElement("ListOnDestruction");

	size_t nbdesGroup = listOnDestruction.size();
	for (size_t i = 0; i < nbdesGroup; i++)
	{
		comment = new TiXmlComment(("index : " + std::to_string(i)).c_str()); node_ListOnDes->LinkEndChild(comment);
		TiXmlElement* node_DesGroup = new TiXmlElement("DestructionGroup");
		size_t nbDes = listOnDestruction.at(i).size();
		for (size_t j = 0; j < nbDes; j++)
		{
			comment = new TiXmlComment(("index : " + std::to_string(j)).c_str()); node_DesGroup->LinkEndChild(comment);
			node_DesGroup->LinkEndChild(listOnDestruction.at(i).at(j).export_Xml(filename, indexGroup, indexHitbox, i, j, listHavokFile, ((i == 0) && (j == 0))));
		}
		node_ListOnDes->LinkEndChild(node_DesGroup);
	}
	mainNode->LinkEndChild(node_ListOnDes);
	




	TiXmlElement* node_ListHavok = new TiXmlElement("ListHavokFiles");
	size_t nbHk = listHavokFilesIndex.size();
	for (size_t i = 0; i < nbHk; i++)
	{
		Havok_File* havokFile = listHavokFile.at(listHavokFilesIndex.at(i));
		string filename_tmp = LibXenoverse::folderFromFilename(filename) + LibXenoverse::nameFromFilenameNoExtension(filename) +"\\"+ "Havok__" + std::to_string(indexGroup) + "_" + std::to_string(indexHitbox)  + "_" + std::to_string(i) + ".hkx";	//extract the file into a folder with the basename of the file.

		node_ListHavok->LinkEndChild(havokFile->export_Xml(filename_tmp));
	}
	mainNode->LinkEndChild(node_ListHavok);





	


	TiXmlElement* node_Mesh = new TiXmlElement("CollisionGeometry");
	comment = new TiXmlComment( ("debug_startOffset_vertex = " + EMO_BaseFile::UnsignedToString(debug_startOffset_vertex, true)).c_str() ); node_Mesh->LinkEndChild(comment);
	comment = new TiXmlComment( ("debug_startOffset_faces = " + EMO_BaseFile::UnsignedToString(debug_startOffset_faces, true)).c_str() ); node_Mesh->LinkEndChild(comment);	

	/*
	TiXmlElement* node_ListVertex = new TiXmlElement("ListVertex");
	size_t nbVertex = listVertex.size();
	for (size_t i = 0; i < nbVertex; i++)
	{
		Vertex &vertex = listVertex.at(i);
	
		/*
		comment = new TiXmlComment(("index : " + std::to_string(i)).c_str()); node_ListVertex->LinkEndChild(comment);
		TiXmlElement* node_Vertex = new TiXmlElement("Vertex");

		node = new TiXmlElement("Position");
		node->SetAttribute("X", std::to_string(vertex.position[0]));
		node->SetAttribute("Y", std::to_string(vertex.position[1]));
		node->SetAttribute("Z", std::to_string(vertex.position[2]));
		node_Vertex->LinkEndChild(node);

		node = new TiXmlElement("Normal");
		node->SetAttribute("X", std::to_string(vertex.normal[0]));
		node->SetAttribute("Y", std::to_string(vertex.normal[1]));
		node->SetAttribute("Z", std::to_string(vertex.normal[2]));
		node_Vertex->LinkEndChild(node);

		node_ListVertex->LinkEndChild(node_Vertex);
		*//*

		//because of the memory of tiXml with tag, NLBY.map crahs the tool. So We have to deal with large number of Vertex to reduce the memory print.
		TiXmlElement* node_Vertex = new TiXmlElement("Vertex");
		node_Vertex->SetAttribute("IdPosNor", std::to_string(i) +","+ FloatToString(vertex.position[0]) +","+ FloatToString(vertex.position[1]) + "," + FloatToString(vertex.position[2]) +","+ FloatToString(vertex.normal[0]) + "," + FloatToString(vertex.normal[1]) + "," + FloatToString(vertex.normal[2]));
		node_ListVertex->LinkEndChild(node_Vertex);
	}
	node_Mesh->LinkEndChild(node_ListVertex);


	TiXmlElement* node_ListFacesIndex = new TiXmlElement("ListFaceIndex");
	size_t nbFI = listFaceIndex.size();
	/*
	for (size_t i = 0; i < nbFI; i++)
	{
		node = new TiXmlElement("Index"); node->SetAttribute("u16", listFaceIndex.at(i));  node_ListFacesIndex->LinkEndChild(node);
	}
	*//*/

	if (nbFI!=0)
	{
		//because of the memory of tinyXml with tag, NLBY.map crahs the tool. So We have to deal with large number of index
		size_t nbElement = 1000;
		size_t nbFI_package = nbFI / nbElement;
		size_t nbFI_package_rest = nbFI - nbFI_package * nbElement;

		string str;
		size_t nbElem = nbElement;
		for (size_t i = 0; i <= nbFI_package; i++)
		{
			if (i == nbFI_package)								//for the last
				nbElem = nbFI_package_rest;					//we take the rest

			str = "";
			for (size_t j = 0; j < nbElem; j++)
				str += ((j != 0) ? "," : "") + std::to_string(listFaceIndex.at(i * nbElement + j));

			node = new TiXmlElement("Index");  node->SetAttribute("u16", str); node_ListFacesIndex->LinkEndChild(node);
		}
	}

	node_Mesh->LinkEndChild(node_ListFacesIndex);
	*/



	//with a test, all Vertex + faceIndex could be converted into emd without trouble. So here we will just extract (and repack after) a emd, that will make the map file lighter.
	size_t nbVertex = listVertex.size();
	size_t nbFI = listFaceIndex.size();

	if((nbVertex!=0)&&(nbFI!=0))
	{
		string meshFilename = "CollisionMesh_" + std::to_string(indexGroup) + "_" + std::to_string(indexHitbox) + ".emd";
		node_Mesh->SetAttribute("filename", LibXenoverse::nameFromFilename(meshFilename));
		
		EMD emd;
		emd.name = meshFilename;
		EMDModel* model = new EMDModel();
		emd.models.push_back(model);

		model->name = "unknowModel";
		EMDMesh* mesh = new EMDMesh();
		model->meshes.push_back(mesh);

		mesh->name = "unknowMesh";
		EMDSubmesh* submesh = new EMDSubmesh();
		mesh->submeshes.push_back(submesh);

		submesh->name = "White";
		submesh->vertex_type_flag = EMD_VTX_FLAG_POS | EMD_VTX_FLAG_NORM;
		submesh->vertex_size = EMDVertex::getSizeFromFlags(submesh->vertex_type_flag, false);


		mesh->aabb_min_w = mesh->aabb_max_w = mesh->aabb_center_w =  1;

		for (size_t i = 0; i < nbVertex; i++)
		{
			Vertex &vertex = listVertex.at(i);

			EMDVertex vert;
			
			vert.flags = submesh->vertex_type_flag;
			vert.pos_x = vertex.position[0];
			vert.pos_y = vertex.position[1];
			vert.pos_z = vertex.position[2];

			vert.norm_x = vertex.normal[0];
			vert.norm_y = vertex.normal[1];
			vert.norm_z = vertex.normal[2];

			submesh->vertices.push_back(vert);

			if (vertex.position[0] < mesh->aabb_min_x)
				mesh->aabb_min_x = vertex.position[0];
			if (vertex.position[0] > mesh->aabb_max_x)
				mesh->aabb_max_x = vertex.position[0];

			if (vertex.position[1] < mesh->aabb_min_y)
				mesh->aabb_min_y = vertex.position[1];
			if (vertex.position[1] > mesh->aabb_max_y)
				mesh->aabb_max_y = vertex.position[1];

			if (vertex.position[2] < mesh->aabb_min_z)
				mesh->aabb_min_z = vertex.position[2];
			if (vertex.position[2] > mesh->aabb_max_z)
				mesh->aabb_max_z = vertex.position[2];
		}

		submesh->aabb_center_x = mesh->aabb_center_x;
		submesh->aabb_center_y = mesh->aabb_center_y;
		submesh->aabb_center_z = mesh->aabb_center_z;
		submesh->aabb_center_w = mesh->aabb_center_w;
		submesh->aabb_min_x = mesh->aabb_min_x;
		submesh->aabb_min_y = mesh->aabb_min_y;
		submesh->aabb_min_z = mesh->aabb_min_z;
		submesh->aabb_min_w = mesh->aabb_min_w;
		submesh->aabb_max_x = mesh->aabb_max_x;
		submesh->aabb_max_y = mesh->aabb_max_y;
		submesh->aabb_max_z = mesh->aabb_max_z;
		submesh->aabb_max_w = mesh->aabb_max_w;

		EMDTriangles triangle;
		for (size_t i = 0; i < nbFI; i++)
		{
			triangle.faces.push_back(listFaceIndex.at(i));
		}
		submesh->triangles.push_back(triangle);

		emd.save( LibXenoverse::folderFromFilename(filename) + LibXenoverse::nameFromFilenameNoExtension(filename) + "\\" + emd.name );
	}


	mainNode->LinkEndChild(node_Mesh);

	return mainNode;
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* Havok_File::export_Xml(string filename)
{
	TiXmlElement* mainNode = new TiXmlElement("HavokFile");
	mainNode->SetAttribute("filename", LibXenoverse::nameFromFilename(filename));

	EMO_BaseFile::WriteFile(filename, buffer, size, true, true);							//extract the file, create folder if needed.

	return mainNode;
}












/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_Destruction::export_Xml(string filename, size_t indexGroup, size_t indexHitbox, size_t indexDestructionGroup, size_t indexDestruction, std::vector<Havok_File*> &listHavokFile, bool withComments)
{

	TiXmlElement* mainNode = new TiXmlElement("Destruction");
	TiXmlElement* node;
	TiXmlComment* comment;

	node = new TiXmlElement("unk_0"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_0, true));  if (withComments) { comment = new TiXmlComment("few hole in [0 (most), 0x11]"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_1"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_1, true));  if(withComments) { comment = new TiXmlComment("from 0x0 (most) to 0x1240b"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_2"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_2, true));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_5"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_5, true));  if (withComments) { comment = new TiXmlComment("look pretty the same as unk_1, but until 0x112aa"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_6"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_6, true));  if (withComments) { comment = new TiXmlComment("from 0 (most) to 0x6159"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_7"); node->SetAttribute("float", FloatToString(unk_7));  if (withComments) { comment = new TiXmlComment("only 0.0, 0.01 (most)"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);


	if(haveSub0)
		mainNode->LinkEndChild(sub0.export_Xml(true));
	else
		mainNode->LinkEndChild(new TiXmlElement("DestructionSubPart"));									//empty to make the difference with Sub1

	if (haveSub1)
		mainNode->LinkEndChild(sub1.export_Xml(true));

	if ((havokFileIndex != (size_t)-1)&&(havokFileIndex<listHavokFile.size()))
	{
		Havok_File* havokFile = listHavokFile.at(havokFileIndex);
		string filename_tmp = LibXenoverse::folderFromFilename(filename) + LibXenoverse::nameFromFilenameNoExtension(filename) + "\\"  + "Havok__" + std::to_string(indexGroup) + "_" + std::to_string(indexHitbox) + "_" + std::to_string(indexDestructionGroup) + "_" + std::to_string(indexDestruction) + ".hkx";	//extract the file into a folder with the basename of the file.

		mainNode->LinkEndChild(havokFile->export_Xml(filename_tmp));
	}
	
	return mainNode;
}

/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* FMP_Destruction_sub::export_Xml(bool withComments)
{

	TiXmlElement* mainNode = new TiXmlElement("DestructionSubPart");
	TiXmlElement* node;
	TiXmlComment* comment;

	node = new TiXmlElement("yaw"); node->SetAttribute("float", FloatToString(yaw));   mainNode->LinkEndChild(node);
	node = new TiXmlElement("pitch"); node->SetAttribute("float", FloatToString(pitch));   mainNode->LinkEndChild(node);
	node = new TiXmlElement("roll"); node->SetAttribute("float", FloatToString(roll));   mainNode->LinkEndChild(node);

	node = new TiXmlElement("unk_0"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_0, false));  if (withComments) { comment = new TiXmlComment("1 (most), 2, 3"); mainNode->LinkEndChild(comment); }  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_1"); node->SetAttribute("float", FloatToString(unk_1));  if (withComments) { comment = new TiXmlComment("[-30.776886 , 1.893005] + 55.799999, 0.0 at most"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_2"); node->SetAttribute("float", FloatToString(unk_2));  if (withComments) { comment = new TiXmlComment("[-58.922211, 35.0], 0.0 (most)"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_3"); node->SetAttribute("float", FloatToString(unk_3));  if (withComments) { comment = new TiXmlComment("[-0.359985, 1.434509] plus -463.797607, -5.0, 20.0. 0.0 at most");  mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_7"); node->SetAttribute("float", FloatToString(unk_7));  if (withComments) { comment = new TiXmlComment("[0.05, 707.106812], 0.5 (most)"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_8"); node->SetAttribute("float", FloatToString(unk_8));  if (withComments) { comment = new TiXmlComment("[0.0, 210.0], 0.5 (most)"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_9"); node->SetAttribute("float", FloatToString(unk_9));  if (withComments) { comment = new TiXmlComment("[0.0, 700.0], 0.5 (most)"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_10"); node->SetAttribute("float", FloatToString(unk_10));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_11"); node->SetAttribute("float", FloatToString(unk_11));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_12"); node->SetAttribute("float", FloatToString(unk_12));  if (withComments) { comment = new TiXmlComment("-210.0, 0.0 (most)]"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_13"); node->SetAttribute("float", FloatToString(unk_13));  if(withComments) { comment = new TiXmlComment("only 0 (most), or 7.721795e-10"); mainNode->LinkEndChild(comment); } mainNode->LinkEndChild(node);
	
	return mainNode;
}













//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------\
|                             Compile				                             |
\-------------------------------------------------------------------------------*/
bool FmpFile::Compile(TiXmlDocument *doc, bool big_endian)
{
	this->big_endian = big_endian;

	TiXmlHandle handle(doc);
	TiXmlElement* rootNode = EMO_BaseFile::FindRoot(&handle, "Fmp");

	if (!rootNode)
	{
		LOG_DEBUG("Cannot find\"Fmp\" in xml.\n");
		return false;
	}

	return import_Xml(rootNode);
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FmpFile::import_Xml(TiXmlElement* rootNode)
{
	Reset();

	
	TiXmlElement* node_Settings = rootNode->FirstChildElement("Settings");
	if (!node_Settings)
	{
		LOG_DEBUG("Cannot find\"Settings\" in xml.\n");
		return false;
	}


	TiXmlElement* node = node_Settings->FirstChildElement("width");
	if (node)
	{
		node->QueryFloatAttribute_floatPrecision("X", &width_X);
		node->QueryFloatAttribute_floatPrecision("Y", &width_Y);
		node->QueryFloatAttribute_floatPrecision("Z", &width_Z);
	}
	string str = "";
	

	node = node_Settings->FirstChildElement("nearDistance"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &nearDistance); }
	node = node_Settings->FirstChildElement("farDistance"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &farDistance); }

	node = node_Settings->FirstChildElement("s0a_unk_3"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_3); }
	node = node_Settings->FirstChildElement("s0a_unk_4"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_4); }
	node = node_Settings->FirstChildElement("s0a_unk_5"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_5); }
	node = node_Settings->FirstChildElement("s0a_unk_6"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_6); }
	node = node_Settings->FirstChildElement("s0a_unk_7"); if (node) node->QueryStringAttribute("u32", &str); s0a_unk_7 = EMO_BaseFile::GetUnsigned(str, s0a_unk_7);

	node = node_Settings->FirstChildElement("s0a_unk_8"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_8); }
	node = node_Settings->FirstChildElement("s0a_unk_9"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_9); }
	node = node_Settings->FirstChildElement("s0a_unk_10"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_10); }

	node = node_Settings->FirstChildElement("s0a_unk_11"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_11); }
	node = node_Settings->FirstChildElement("s0a_unk_12"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_12); }
	node = node_Settings->FirstChildElement("s0a_unk_13"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_13); }
	node = node_Settings->FirstChildElement("s0a_unk_14"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_14); }
	node = node_Settings->FirstChildElement("s0a_unk_15"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_15); }
	node = node_Settings->FirstChildElement("s0a_unk_16"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_16); }
	node = node_Settings->FirstChildElement("s0a_unk_17"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_17); }
	node = node_Settings->FirstChildElement("s0a_unk_18"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_18); }

	node = node_Settings->FirstChildElement("s0a_unk_19"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_19); }
	node = node_Settings->FirstChildElement("s0a_unk_20"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_20); }
	node = node_Settings->FirstChildElement("s0a_unk_21"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_21); }
	node = node_Settings->FirstChildElement("s0a_unk_22"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_22); }
	node = node_Settings->FirstChildElement("s0a_unk_23"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_23); }
	node = node_Settings->FirstChildElement("s0a_unk_24"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_24); }
	node = node_Settings->FirstChildElement("s0a_unk_25"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_25); }
	node = node_Settings->FirstChildElement("s0a_unk_26"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_26); }

	node = node_Settings->FirstChildElement("s0a_unk_27"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_27); }
	node = node_Settings->FirstChildElement("s0a_unk_28"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_28); }

	node = node_Settings->FirstChildElement("s0a_unk_29"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_29); }
	node = node_Settings->FirstChildElement("s0a_unk_30"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_30); }
	node = node_Settings->FirstChildElement("s0a_unk_31"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_31); }
	node = node_Settings->FirstChildElement("s0a_unk_34"); if (node) { node->QueryFloatAttribute_floatPrecision("float", &s0a_unk_34); }
	


	node = node_Settings->FirstChildElement("s0b_unk_0"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_0 = EMO_BaseFile::GetUnsigned(str, s0b_unk_0);
	node = node_Settings->FirstChildElement("s0b_unk_1"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_1 = EMO_BaseFile::GetUnsigned(str, s0b_unk_1);
	node = node_Settings->FirstChildElement("s0b_unk_2"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_2 = EMO_BaseFile::GetUnsigned(str, s0b_unk_2);
	node = node_Settings->FirstChildElement("s0b_unk_3"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_3 = EMO_BaseFile::GetUnsigned(str, s0b_unk_3);
	node = node_Settings->FirstChildElement("s0b_unk_4"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_4 = EMO_BaseFile::GetUnsigned(str, s0b_unk_4);
	node = node_Settings->FirstChildElement("s0b_unk_5"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_5 = EMO_BaseFile::GetUnsigned(str, s0b_unk_5);
	node = node_Settings->FirstChildElement("s0b_unk_6"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_6 = EMO_BaseFile::GetUnsigned(str, s0b_unk_6);
	node = node_Settings->FirstChildElement("s0b_unk_7"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_7 = EMO_BaseFile::GetUnsigned(str, s0b_unk_7);
	node = node_Settings->FirstChildElement("s0b_unk_8"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_8 = EMO_BaseFile::GetUnsigned(str, s0b_unk_8);
	node = node_Settings->FirstChildElement("s0b_unk_9"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_9 = EMO_BaseFile::GetUnsigned(str, s0b_unk_9);
	node = node_Settings->FirstChildElement("s0b_unk_10"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_10 = EMO_BaseFile::GetUnsigned(str, s0b_unk_10);
	node = node_Settings->FirstChildElement("s0b_unk_11"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_11 = EMO_BaseFile::GetUnsigned(str, s0b_unk_11);
	node = node_Settings->FirstChildElement("s0b_unk_12"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_12 = EMO_BaseFile::GetUnsigned(str, s0b_unk_12);
	node = node_Settings->FirstChildElement("s0b_unk_13"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_13 = EMO_BaseFile::GetUnsigned(str, s0b_unk_13);
	node = node_Settings->FirstChildElement("s0b_unk_14"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_14 = EMO_BaseFile::GetUnsigned(str, s0b_unk_14);
	node = node_Settings->FirstChildElement("s0b_unk_15"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_15 = EMO_BaseFile::GetUnsigned(str, s0b_unk_15);
	node = node_Settings->FirstChildElement("s0b_unk_16"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_16 = EMO_BaseFile::GetUnsigned(str, s0b_unk_16);
	node = node_Settings->FirstChildElement("s0b_unk_17"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_17 = EMO_BaseFile::GetUnsigned(str, s0b_unk_17);
	node = node_Settings->FirstChildElement("s0b_unk_18"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_18 = EMO_BaseFile::GetUnsigned(str, s0b_unk_18);
	node = node_Settings->FirstChildElement("s0b_unk_19"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_19 = EMO_BaseFile::GetUnsigned(str, s0b_unk_19);
	node = node_Settings->FirstChildElement("s0b_unk_20"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_20 = EMO_BaseFile::GetUnsigned(str, s0b_unk_20);
	node = node_Settings->FirstChildElement("s0b_unk_21"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_21 = EMO_BaseFile::GetUnsigned(str, s0b_unk_21);
	node = node_Settings->FirstChildElement("s0b_unk_22"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_22 = EMO_BaseFile::GetUnsigned(str, s0b_unk_22);
	node = node_Settings->FirstChildElement("s0b_unk_23"); if (node) node->QueryStringAttribute("u16", &str); s0b_unk_23 = EMO_BaseFile::GetUnsigned(str, s0b_unk_23);
	node = node_Settings->FirstChildElement("s0b_unk_23b"); if (node) node->QueryStringAttribute("u16", &str); s0b_unk_23b = EMO_BaseFile::GetUnsigned(str, s0b_unk_23b);
	node = node_Settings->FirstChildElement("s0b_unk_24"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_24 = EMO_BaseFile::GetUnsigned(str, s0b_unk_24);
	node = node_Settings->FirstChildElement("s0b_unk_25"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_25 = EMO_BaseFile::GetUnsigned(str, s0b_unk_25);
	node = node_Settings->FirstChildElement("s0b_unk_26"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_26 = EMO_BaseFile::GetUnsigned(str, s0b_unk_26);
	node = node_Settings->FirstChildElement("s0b_unk_27"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_27 = EMO_BaseFile::GetUnsigned(str, s0b_unk_27);
	node = node_Settings->FirstChildElement("s0b_unk_28"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_28 = EMO_BaseFile::GetUnsigned(str, s0b_unk_28);
	node = node_Settings->FirstChildElement("s0b_unk_29"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_29 = EMO_BaseFile::GetUnsigned(str, s0b_unk_29);
	node = node_Settings->FirstChildElement("s0b_unk_30"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_30 = EMO_BaseFile::GetUnsigned(str, s0b_unk_30);
	node = node_Settings->FirstChildElement("s0b_unk_31"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_31 = EMO_BaseFile::GetUnsigned(str, s0b_unk_31);
	node = node_Settings->FirstChildElement("s0b_unk_32"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_32 = EMO_BaseFile::GetUnsigned(str, s0b_unk_32);
	node = node_Settings->FirstChildElement("s0b_unk_33"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_33 = EMO_BaseFile::GetUnsigned(str, s0b_unk_33);
	node = node_Settings->FirstChildElement("s0b_unk_34"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_34 = EMO_BaseFile::GetUnsigned(str, s0b_unk_34);
	node = node_Settings->FirstChildElement("s0b_unk_35"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_35 = EMO_BaseFile::GetUnsigned(str, s0b_unk_35);
	node = node_Settings->FirstChildElement("s0b_unk_36"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_36 = EMO_BaseFile::GetUnsigned(str, s0b_unk_36);
	node = node_Settings->FirstChildElement("s0b_unk_37"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_37 = EMO_BaseFile::GetUnsigned(str, s0b_unk_37);
	node = node_Settings->FirstChildElement("s0b_unk_38"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_38 = EMO_BaseFile::GetUnsigned(str, s0b_unk_38);
	node = node_Settings->FirstChildElement("s0b_unk_39"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_39 = EMO_BaseFile::GetUnsigned(str, s0b_unk_39);
	node = node_Settings->FirstChildElement("s0b_unk_40"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_40 = EMO_BaseFile::GetUnsigned(str, s0b_unk_40);
	node = node_Settings->FirstChildElement("s0b_unk_41"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_41 = EMO_BaseFile::GetUnsigned(str, s0b_unk_41);
	node = node_Settings->FirstChildElement("s0b_unk_42"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_42 = EMO_BaseFile::GetUnsigned(str, s0b_unk_42);
	node = node_Settings->FirstChildElement("s0b_unk_43"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_43 = EMO_BaseFile::GetUnsigned(str, s0b_unk_43);
	node = node_Settings->FirstChildElement("s0b_unk_44"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_44 = EMO_BaseFile::GetUnsigned(str, s0b_unk_44);
	node = node_Settings->FirstChildElement("s0b_unk_45"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_45 = EMO_BaseFile::GetUnsigned(str, s0b_unk_45);
	node = node_Settings->FirstChildElement("s0b_unk_46"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_46 = EMO_BaseFile::GetUnsigned(str, s0b_unk_46);
	node = node_Settings->FirstChildElement("s0b_unk_47"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_47 = EMO_BaseFile::GetUnsigned(str, s0b_unk_47);
	node = node_Settings->FirstChildElement("s0b_unk_48"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_48 = EMO_BaseFile::GetUnsigned(str, s0b_unk_48);
	node = node_Settings->FirstChildElement("s0b_unk_49"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_49 = EMO_BaseFile::GetUnsigned(str, s0b_unk_49);
	node = node_Settings->FirstChildElement("s0b_unk_50"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_50 = EMO_BaseFile::GetUnsigned(str, s0b_unk_50);
	node = node_Settings->FirstChildElement("s0b_unk_51"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_51 = EMO_BaseFile::GetUnsigned(str, s0b_unk_51);
	node = node_Settings->FirstChildElement("s0b_unk_52"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_52 = EMO_BaseFile::GetUnsigned(str, s0b_unk_52);
	node = node_Settings->FirstChildElement("s0b_unk_53"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_53 = EMO_BaseFile::GetUnsigned(str, s0b_unk_53);
	node = node_Settings->FirstChildElement("s0b_unk_54"); if (node) node->QueryStringAttribute("u32", &str); s0b_unk_54 = EMO_BaseFile::GetUnsigned(str, s0b_unk_54);
	

	TiXmlElement* node_Type_names = node_Settings->FirstChildElement("ItemTypes");
	if (node_Type_names)
		mListTypeName.clear();
	for (TiXmlElement* node = (node_Type_names) ? node_Type_names->FirstChildElement("ItemType") : 0; node; node = node->NextSiblingElement("ItemType"))
	{
		str = "";
		node->QueryStringAttribute("name", &str);
		//if (str.length() == 0)
		//	continue;

		mListTypeName.push_back(str);
	}



	TiXmlElement* node_ListSection1 = rootNode->FirstChildElement("ListSection1");
	for (TiXmlElement* node = (node_ListSection1) ? node_ListSection1->FirstChildElement("Section1") : 0; node; node = node->NextSiblingElement("Section1"))
	{
		mListS1.push_back(FMP_S1());
		if (!mListS1.back().import_Xml(node))
			mListS1.pop_back();
	}

	TiXmlElement* node_ListSection2 = rootNode->FirstChildElement("ListSection2");
	for (TiXmlElement* node = (node_ListSection2) ? node_ListSection2->FirstChildElement("Section2") : 0; node; node = node->NextSiblingElement("Section2"))
	{
		mListS2.push_back(FMP_S2());
		if (!mListS2.back().import_Xml(node))
			mListS2.pop_back();
	}



	TiXmlElement* node_groups = rootNode->FirstChildElement("ListFragmentGroup");
	size_t tmp = 0;
	for (TiXmlElement* node_group = (node_groups) ? node_groups->FirstChildElement("Group") : 0; node_group; node_group = node_group->NextSiblingElement("Group"))
	{
		str = "";
		node_group->QueryStringAttribute("name", &str);
		
		mListFragmentsGroups.push_back(FragmentsGroup(str));
		for (TiXmlElement* node = node_group->FirstChildElement("ObjectIndex"); node; node = node->NextSiblingElement("ObjectIndex"))
		{
			tmp = (size_t)-1;
			node->QueryUnsignedAttribute("value", &tmp);

			mListFragmentsGroups.back().listObjects.push_back(tmp);
		}
	}







	TiXmlElement* node_ListObj = rootNode->FirstChildElement("ListObject");
	for (TiXmlElement* node = (node_ListObj) ? node_ListObj->FirstChildElement("Object") : 0; node; node = node->NextSiblingElement("Object"))
	{
		mListObject.push_back(new FMP_Object());
		if (!mListObject.back()->import_Xml(node))
		{
			delete mListObject.back();
			mListObject.pop_back();
		}
	}




	TiXmlElement* node_ListHG = rootNode->FirstChildElement("ListHitboxGroup");
	for (TiXmlElement* node = (node_ListHG) ? node_ListHG->FirstChildElement("HitboxGroup") : 0; node; node = node->NextSiblingElement("HitboxGroup"))
	{
		mListHitboxGroup.push_back(FMP_HitboxGroup());
		if (!mListHitboxGroup.back().import_Xml(node, mListHavokFile, originefilename))
			mListHitboxGroup.pop_back();
	}









	TiXmlElement* node_listFile = rootNode->FirstChildElement("ListFiles");
	if (node_listFile)
	{
		TiXmlElement* node_Files = node_listFile->FirstChildElement("ListNsk");
		for (TiXmlElement* node = (node_Files) ? node_Files->FirstChildElement("File") : 0; node; node = node->NextSiblingElement("File"))
		{
			str = "";
			node->QueryStringAttribute("name", &str);
			if (str.length() != 0)
				listNsk.push_back(str);
		}


		node_Files = node_listFile->FirstChildElement("ListTextureEmb");
		for (TiXmlElement* node = (node_Files) ? node_Files->FirstChildElement("File") : 0; node; node = node->NextSiblingElement("File"))
		{
			str = "";
			node->QueryStringAttribute("name", &str);
			if (str.length() != 0)
				listTextureEmb.push_back(str);
		}


		node_Files = node_listFile->FirstChildElement("ListEmm");
		for (TiXmlElement* node = (node_Files) ? node_Files->FirstChildElement("File") : 0; node; node = node->NextSiblingElement("File"))
		{
			str = "";
			node->QueryStringAttribute("name", &str);
			if (str.length() != 0)
				listEmm.push_back(str);
		}

		node_Files = node_listFile->FirstChildElement("ListEma");
		for (TiXmlElement* node = (node_Files) ? node_Files->FirstChildElement("File") : 0; node; node = node->NextSiblingElement("File"))
		{
			str = "";
			node->QueryStringAttribute("name", &str);
			if (str.length() != 0)
				listEma.push_back(str);
		}
	}





	return true;
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_S1::import_Xml(TiXmlElement* node)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";

	node_tmp = node->FirstChildElement("name"); if (node_tmp) node_tmp->QueryStringAttribute("value", &name);
	node_tmp = node->FirstChildElement("unk_04"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_04 = EMO_BaseFile::GetUnsigned(str, unk_04);
	node_tmp = node->FirstChildElement("unk_08"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_08); }
	node_tmp = node->FirstChildElement("unk_0C"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_0C); }
	node_tmp = node->FirstChildElement("unk_10"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_10); }
	node_tmp = node->FirstChildElement("unk_14"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_14); }

	return true;
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_S2::import_Xml(TiXmlElement* node)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";

	node_tmp = node->FirstChildElement("name"); if (node_tmp) node_tmp->QueryStringAttribute("value", &name);
	node_tmp = node->FirstChildElement("unk_04"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_04); }
	node_tmp = node->FirstChildElement("unk_08"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_08); }
	node_tmp = node->FirstChildElement("unk_0C"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_0C); }
	node_tmp = node->FirstChildElement("unk_10"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_10); }
	node_tmp = node->FirstChildElement("unk_14"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_14); }
	node_tmp = node->FirstChildElement("unk_18"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_18); }

	return true;
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_Object::import_Xml(TiXmlElement* node)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";

	node_tmp = node->FirstChildElement("name"); if (node_tmp) node_tmp->QueryStringAttribute("value", &name);
	node_tmp = node->FirstChildElement("hitboxGroupIndex"); if (node_tmp) { node_tmp->QueryUnsignedAttribute("value", &hitboxGroupIndex); }

	//todo check hitboxGroupIndex is good, and also the number of hitbox == number VisualSubPart. (check all hyp in code).

	node_tmp = node->FirstChildElement("unk_04"); if (node_tmp) node_tmp->QueryStringAttribute("u16", &str); unk_04 = EMO_BaseFile::GetUnsigned(str, unk_04);
	node_tmp = node->FirstChildElement("unk_04b"); if (node_tmp) node_tmp->QueryStringAttribute("u16", &str); unk_04b = EMO_BaseFile::GetUnsigned(str, unk_04b);
	node_tmp = node->FirstChildElement("unk_0A"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_0A = EMO_BaseFile::GetUnsigned(str, unk_0A);
	node_tmp = node->FirstChildElement("unk_0B"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_0B = EMO_BaseFile::GetUnsigned(str, unk_0B);
	node_tmp = node->FirstChildElement("unk_20"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_20); }



	TiXmlElement* node_ListEnt = node->FirstChildElement("ListEntity");
	for (TiXmlElement* node_tmp = (node_ListEnt) ? node_ListEnt->FirstChildElement("Entity") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("Entity"))
	{
		listEntity.push_back(FMP_Entity());
		if (!listEntity.back().import_Xml(node_tmp))
			listEntity.pop_back();
	}



	TiXmlElement* node_ListVsp = node->FirstChildElement("ListVirtualSubPart");
	for (TiXmlElement* node_tmp = (node_ListVsp) ? node_ListVsp->FirstChildElement("VirtualSubPart") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("VirtualSubPart"))
	{
		listVirtualSubParts.push_back(new FMP_VirtualSubPart());
		if (!listVirtualSubParts.back()->import_Xml(node_tmp))
		{
			delete listVirtualSubParts.back();
			listVirtualSubParts.pop_back();
		}
	}

	TiXmlElement* node_Action = node->FirstChildElement("Action");
	if (node_Action)
	{
		action = new FMP_Action();
		if (!action->import_Xml(node_Action))
		{
			delete action;
			action = 0;
		}
	}
	

	TiXmlElement* node_Hierarchy = node->FirstChildElement("Hierarchy");
	if (node_Hierarchy)
	{
		hierarchy = new FMP_Hierarchy();
		if (!hierarchy->import_Xml(node_Hierarchy))
		{
			delete hierarchy;
			hierarchy = 0;
		}
	}



	TiXmlElement* node_matrix = node->FirstChildElement("TransformMatrix4x3");
	if (node_matrix)
	{
		TiXmlElement* node_matrix_Lx;

		for (size_t i = 0; i < 4; i++)
		{
			node_matrix_Lx = node_matrix->FirstChildElement("L" + std::to_string(i));
			if (node_matrix_Lx)
			{
				str = "";
				node_matrix_Lx->QueryStringAttribute("f32s", &str);
				std::vector<string> vector = split(str, ',');
				if (vector.size() == 3)
				{
					for(size_t j=0;j<3;j++)
						transformMatrix4x3[i * 3 + j] = StringToFloat(vector.at(j));
				}
			}
		}
	}
	
	return true;
}













/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_Action::import_Xml(TiXmlElement* node)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";

	node_tmp = node->FirstChildElement("unk_0"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_0 = EMO_BaseFile::GetUnsigned(str, unk_0);
	node_tmp = node->FirstChildElement("unk_1a"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_1a = EMO_BaseFile::GetUnsigned(str, unk_1a);
	node_tmp = node->FirstChildElement("unk_1b"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_1b = EMO_BaseFile::GetUnsigned(str, unk_1b);
	node_tmp = node->FirstChildElement("unk_1c"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_1c = EMO_BaseFile::GetUnsigned(str, unk_1c);
	node_tmp = node->FirstChildElement("unk_1d"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_1d = EMO_BaseFile::GetUnsigned(str, unk_1d);



	TiXmlElement* node_ListCommand = node->FirstChildElement("ListCommand");
	for (TiXmlElement* node_tmp = (node_ListCommand) ? node_ListCommand->FirstChildElement("Command") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("Command"))
	{
		listCommands.push_back(FMP_Action::Command(""));
		if (!listCommands.back().import_Xml(node_tmp))
			listCommands.pop_back();
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_Action::Command::import_Xml(TiXmlElement* node)
{
	//Reset();

	TiXmlElement* node_tmp;
	string str = "";

	node_tmp = node->FirstChildElement("function"); if (node_tmp) node_tmp->QueryStringAttribute("name", &functionName);
	node_tmp = node->FirstChildElement("unk_1a"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_1a = EMO_BaseFile::GetUnsigned(str, unk_1a);
	node_tmp = node->FirstChildElement("unk_1b"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_1b = EMO_BaseFile::GetUnsigned(str, unk_1b);
	node_tmp = node->FirstChildElement("unk_1c"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_1c = EMO_BaseFile::GetUnsigned(str, unk_1c);
	node_tmp = node->FirstChildElement("unk_1d"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_1d = EMO_BaseFile::GetUnsigned(str, unk_1d);



	TiXmlElement* node_ListParam = node->FirstChildElement("ListParameter");
	for (TiXmlElement* node_tmp = (node_ListParam) ? node_ListParam->FirstChildElement("Parameter") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("Parameter"))
	{
		parameters.push_back(FMP_Parameter());
		if (!parameters.back().import_Xml(node_tmp))
			parameters.pop_back();
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_Parameter::import_Xml(TiXmlElement* node)
{
	//Reset();

	string str = "";

	node->QueryStringAttribute("name", &name);
	string typeValue_str = "";
	node->QueryStringAttribute("type", &typeValue_str);
	

	if (typeValue_str == "Bool")
	{
		typeValue = TV_Bool;
		node->QueryBoolAttribute("value", &boolValue);

	}else if(typeValue_str == "Int") {
		
		typeValue = TV_Int;
		node->QueryIntAttribute("value", &intValue);

	}else if (typeValue_str == "Float") {

		typeValue = TV_Float;
		node->QueryFloatAttribute_floatPrecision("value", &floatValue);

	}else if (typeValue_str == "String") {

		typeValue = TV_String;
		node->QueryStringAttribute("value", &stringValue);

	}else if (typeValue_str == "Uint") {

		typeValue = TV_Uint;
		node->QueryUnsignedAttribute("value", &uintValue);


	}else if (typeValue_str == "Direction") {

		typeValue = TV_Direction;
		node->QueryFloatAttribute_floatPrecision("X", &directionValue[0]);
		node->QueryFloatAttribute_floatPrecision("Y", &directionValue[1]);
		node->QueryFloatAttribute_floatPrecision("Z", &directionValue[2]);
		node->QueryFloatAttribute_floatPrecision("W", &directionValue[3]);

	}else if (typeValue_str == "Position") {

		typeValue = TV_Position;
		node->QueryFloatAttribute_floatPrecision("X", &positionValue[0]);
		node->QueryFloatAttribute_floatPrecision("Y", &positionValue[1]);
		node->QueryFloatAttribute_floatPrecision("Z", &positionValue[2]);
		node->QueryFloatAttribute_floatPrecision("W", &positionValue[3]);

	}else {
		printf("unknow Type Data %s for parameters. skipped.\n", typeValue_str.c_str());
		return false;
	}

	return true;
}













/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_Hierarchy::import_Xml(TiXmlElement* node)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";


	TiXmlElement* node_ListNode = node->FirstChildElement("ListNode");
	for (TiXmlElement* node_tmp = (node_ListNode) ? node_ListNode->FirstChildElement("Node") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("Node"))
	{
		listNode.push_back(FMP_Node());
		if (!listNode.back().import_Xml(node_tmp))
			listNode.pop_back();
	}

	
	TiXmlElement* node_ListTransf = node->FirstChildElement("ListTransform");
	for (TiXmlElement* node_tmp = (node_ListTransf) ? node_ListTransf->FirstChildElement("Transform") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("Transform"))
	{
		listTransform.push_back(FMP_NodeTransform());
		if (!listTransform.back().import_Xml(node_tmp))
			listTransform.pop_back();
	}

	node_tmp = node->FirstChildElement("HierarchyNode");
	if (node_tmp)
	{
		mRoot = new FMP_HierarchyNode();
		if (!mRoot->import_Xml(node_tmp))
		{
			delete mRoot;
			mRoot = 0;
		}
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_Node::import_Xml(TiXmlElement* node)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";

	node_tmp = node->FirstChildElement("transformIndex"); if (node_tmp) node_tmp->QueryUnsignedAttribute("value", &transformIndex);

	node_tmp = node->FirstChildElement("unk_0"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_0); }
	node_tmp = node->FirstChildElement("unk_1"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_1); }
	node_tmp = node->FirstChildElement("unk_2"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_2); }
	node_tmp = node->FirstChildElement("unk_3"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_3); }
	node_tmp = node->FirstChildElement("unk_4"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_4); }
	node_tmp = node->FirstChildElement("unk_5"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_5); }
	node_tmp = node->FirstChildElement("unk_6"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_6); }
	node_tmp = node->FirstChildElement("unk_7"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_7); }
	node_tmp = node->FirstChildElement("unk_8"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_8); }
	node_tmp = node->FirstChildElement("unk_9"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_9); }
	node_tmp = node->FirstChildElement("unk_10"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_10 = EMO_BaseFile::GetUnsigned(str, unk_10);

	return true;
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_NodeTransform::import_Xml(TiXmlElement* node)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";

	node_tmp = node->FirstChildElement("Position"); 
	if (node_tmp)
	{
		node_tmp->QueryFloatAttribute_floatPrecision("X", &position[0]);
		node_tmp->QueryFloatAttribute_floatPrecision("Y", &position[1]);
		node_tmp->QueryFloatAttribute_floatPrecision("Z", &position[2]);
	}

	node_tmp = node->FirstChildElement("Rotation");
	if (node_tmp)
	{
		node_tmp->QueryFloatAttribute_floatPrecision("X", &rotation[0]);
		node_tmp->QueryFloatAttribute_floatPrecision("Y", &rotation[1]);
		node_tmp->QueryFloatAttribute_floatPrecision("Z", &rotation[2]);
	}

	node_tmp = node->FirstChildElement("Scale");
	if (node_tmp)
	{
		node_tmp->QueryFloatAttribute_floatPrecision("X", &scale[0]);
		node_tmp->QueryFloatAttribute_floatPrecision("Y", &scale[1]);
		node_tmp->QueryFloatAttribute_floatPrecision("Z", &scale[2]);
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_HierarchyNode::import_Xml(TiXmlElement* node)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";


	node_tmp = node->FirstChildElement("typeb"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); typeb = EMO_BaseFile::GetUnsigned(str, typeb);
	node_tmp = node->FirstChildElement("unk_0"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_0); }
	node_tmp = node->FirstChildElement("unk_1"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_1); }
	node_tmp = node->FirstChildElement("unk_2"); if (node_tmp) { node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_2); }


	TiXmlElement* node_listIndex = node->FirstChildElement("ListIndex");
	size_t index = 0;
	for (TiXmlElement* node_tmp = (node_listIndex) ? node_listIndex->FirstChildElement("Index") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("Index"))
	{
		node_tmp->QueryUnsignedAttribute("u16", &index);
		listIndex.push_back(index);
	}


	TiXmlElement* node_Child = node->FirstChildElement("Child");
	for (TiXmlElement* node_tmp = (node_Child) ? node_Child->FirstChildElement("HierarchyNode") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("HierarchyNode"))
	{
		bool isEmpty = (node_tmp->FirstChildElement("typeb")==0);
		if (isEmpty)
		{
			listChild.push_back(0);
			continue;
		}

		listChild.push_back(new FMP_HierarchyNode());
		if (!listChild.back()->import_Xml(node_tmp))											//recursive
		{
			delete listChild.back();
			listChild.pop_back();
		}
	}

	return true;
}









/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_Entity::import_Xml(TiXmlElement* node)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";



	node_tmp = node->FirstChildElement("s4n2_unk_0"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); s4n2_unk_0 = EMO_BaseFile::GetUnsigned(str, s4n2_unk_0);


	TiXmlElement* node_Visual = node->FirstChildElement("Visual");
	if (node_Visual)
	{
		haveVisualPart = true;
		node_tmp = node_Visual->FirstChildElement("name"); if (node_tmp) node_tmp->QueryStringAttribute("value", &name);


		TiXmlElement* node_ListLod = node_Visual->FirstChildElement("ListLod");
		float distance; 
		string nskFilename, emmFilename;
		for (TiXmlElement* node_tmp_b = (node_ListLod) ? node_ListLod->FirstChildElement("Lod") : 0; node_tmp_b; node_tmp_b = node_tmp_b->NextSiblingElement("Lod"))
		{
			node_tmp_b->QueryFloatAttribute_floatPrecision("distance", &distance);
			node_tmp_b->QueryStringAttribute("Nsk", &nskFilename);
			node_tmp_b->QueryStringAttribute("Emm", &emmFilename);

			listLod.push_back(Lod(nskFilename, emmFilename, distance));
		}


		node_tmp = node_Visual->FirstChildElement("Emb"); if (node_tmp) node_tmp->QueryStringAttribute("filename", &embTexture_filename);
		node_tmp = node_Visual->FirstChildElement("Ema"); if (node_tmp) node_tmp->QueryStringAttribute("filename", &ema_filename);

		node_tmp = node_Visual->FirstChildElement("s4n2b_unk_0"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); s4n2b_unk_0 = EMO_BaseFile::GetUnsigned(str, s4n2b_unk_0);
		node_tmp = node_Visual->FirstChildElement("s4n2b_unk_5"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); s4n2b_unk_5 = EMO_BaseFile::GetUnsigned(str, s4n2b_unk_5);
		node_tmp = node_Visual->FirstChildElement("s4n2b_unk_6"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); s4n2b_unk_6 = EMO_BaseFile::GetUnsigned(str, s4n2b_unk_6);
		node_tmp = node_Visual->FirstChildElement("s4n2b_unk_8"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); s4n2b_unk_8 = EMO_BaseFile::GetUnsigned(str, s4n2b_unk_8);
		node_tmp = node_Visual->FirstChildElement("s4n2b_unk_9"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &s4n2b_unk_9);
		node_tmp = node_Visual->FirstChildElement("s4n2b_unk_10"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &s4n2b_unk_10);
	}

	
	TiXmlElement* node_matrix = node->FirstChildElement("TransformMatrix4x3");
	if (node_matrix)
	{
		TiXmlElement* node_matrix_Lx;

		for (size_t i = 0; i < 4; i++)
		{
			node_matrix_Lx = node_matrix->FirstChildElement("L" + std::to_string(i));
			if (node_matrix_Lx)
			{
				str = "";
				node_matrix_Lx->QueryStringAttribute("f32s", &str);
				std::vector<string> vector = split(str, ',');
				if (vector.size() == 3)
				{
					for (size_t j = 0; j<3; j++)
						transformMatrix4x3[i * 3 + j] = StringToFloat(vector.at(j));
				}
			}
		}
	}

	return true;
}













/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_VirtualSubPart::import_Xml(TiXmlElement* node)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";


	node_tmp = node->FirstChildElement("unk_2"); if (node_tmp) node_tmp->QueryStringAttribute("u16", &str); unk_2 = EMO_BaseFile::GetUnsigned(str, unk_2);
	node_tmp = node->FirstChildElement("unk_2b"); if (node_tmp) node_tmp->QueryStringAttribute("u16", &str); unk_2b = EMO_BaseFile::GetUnsigned(str, unk_2b);
	node_tmp = node->FirstChildElement("unk_4"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_4);
	node_tmp = node->FirstChildElement("unk_5"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_5);


	TiXmlElement* node_listIndex = node->FirstChildElement("ListIndexCouple");
	size_t index = 0;
	for (TiXmlElement* node_tmp = (node_listIndex) ? node_listIndex->FirstChildElement("IndexCouple") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("IndexCouple"))
	{
		listIndex.push_back(std::vector<size_t>());

		node_tmp->QueryUnsignedAttribute("Index0", &index);
		listIndex.back().push_back(index);
		node_tmp->QueryUnsignedAttribute("Index1", &index);
		listIndex.back().push_back(index);
	}


	TiXmlElement* node_sub0 = node->FirstChildElement("SubPart");
	if ((node_sub0) && (node_sub0->FirstChildElement("Width")))
	{
		sub0 = new FMP_VirtualSubPart_sub();
		if (!sub0->import_Xml(node_sub0))
		{
			delete sub0;
			sub0 = 0;
		}
	}

	TiXmlElement* node_sub1 = (node_sub0) ? node_sub0->NextSiblingElement("SubPart") : 0;
	if ((node_sub1) && (node_sub1->FirstChildElement("Width")))
	{
		sub1 = new FMP_VirtualSubPart_sub();
		if (!sub1->import_Xml(node_sub1))
		{
			delete sub1;
			sub1 = 0;
		}
	}

	TiXmlElement* node_Action = node->FirstChildElement("Action");
	if (node_Action)
	{
		action = new FMP_Action();
		if (!action->import_Xml(node_Action))
		{
			delete action;
			action = 0;
		}
	}




	TiXmlElement* node_matrix = node->FirstChildElement("TransformMatrix4x3");
	if (node_matrix)
	{
		TiXmlElement* node_matrix_Lx;

		for (size_t i = 0; i < 4; i++)
		{
			node_matrix_Lx = node_matrix->FirstChildElement("L" + std::to_string(i));
			if (node_matrix_Lx)
			{
				str = "";
				node_matrix_Lx->QueryStringAttribute("f32s", &str);
				std::vector<string> vector = split(str, ',');
				if (vector.size() == 3)
				{
					for (size_t j = 0; j<3; j++)
						transformMatrix4x3[i * 3 + j] = StringToFloat(vector.at(j));
				}
			}
		}
	}

	return true;
}

/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_VirtualSubPart_sub::import_Xml(TiXmlElement* node)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";


	node_tmp = node->FirstChildElement("Width"); 
	if (node_tmp)
	{
		node_tmp->QueryFloatAttribute_floatPrecision("X", &widthX);
		node_tmp->QueryFloatAttribute_floatPrecision("Y", &widthY);
		node_tmp->QueryFloatAttribute_floatPrecision("Z", &widthZ);
	}

	node_tmp = node->FirstChildElement("Quaternion");
	if (node_tmp)
	{
		node_tmp->QueryFloatAttribute_floatPrecision("X", &quaternionX);
		node_tmp->QueryFloatAttribute_floatPrecision("Y", &quaternionY);
		node_tmp->QueryFloatAttribute_floatPrecision("Z", &quaternionZ);
		node_tmp->QueryFloatAttribute_floatPrecision("W", &quaternionW);
	}

	node_tmp = node->FirstChildElement("unk_0"); if (node_tmp) node_tmp->QueryStringAttribute("u16", &str); unk_0 = EMO_BaseFile::GetUnsigned(str, unk_0);
	node_tmp = node->FirstChildElement("unk_0b"); if (node_tmp) node_tmp->QueryStringAttribute("u16", &str); unk_0b = EMO_BaseFile::GetUnsigned(str, unk_0b);
	node_tmp = node->FirstChildElement("unk_1"); if (node_tmp) node_tmp->QueryStringAttribute("u16", &str); unk_1 = EMO_BaseFile::GetUnsigned(str, unk_1);
	node_tmp = node->FirstChildElement("unk_1b"); if (node_tmp) node_tmp->QueryStringAttribute("u16", &str); unk_1b = EMO_BaseFile::GetUnsigned(str, unk_1b);
	node_tmp = node->FirstChildElement("unk_2"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_2 = EMO_BaseFile::GetUnsigned(str, unk_2);
	node_tmp = node->FirstChildElement("unk_3"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_3);
	node_tmp = node->FirstChildElement("unk_4"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_4);
	node_tmp = node->FirstChildElement("unk_5"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_5);
	node_tmp = node->FirstChildElement("unk_6"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_6);
	node_tmp = node->FirstChildElement("unk_7"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_7);
	node_tmp = node->FirstChildElement("unk_8"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_8);
	node_tmp = node->FirstChildElement("unk_9"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_9);
	node_tmp = node->FirstChildElement("unk_10"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_10);
	node_tmp = node->FirstChildElement("unk_11"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_11);
	node_tmp = node->FirstChildElement("unk_12"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_12);
	node_tmp = node->FirstChildElement("unk_13"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_13);
	
	return true;
}















/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_HitboxGroup::import_Xml(TiXmlElement* node, std::vector<Havok_File*> &listHavokFile, string filename)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";

	node_tmp = node->FirstChildElement("name"); if (node_tmp) node_tmp->QueryStringAttribute("value", &name);

	TiXmlElement* node_ListH = node->FirstChildElement("ListHitbox");
	for (TiXmlElement* node_tmp = (node_ListH) ? node_ListH->FirstChildElement("Hitbox") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("Hitbox"))
	{
		listHitbox.push_back(FMP_Hitbox());
		if (!listHitbox.back().import_Xml(node_tmp, listHavokFile, filename))
			listHitbox.pop_back();
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_Hitbox::import_Xml(TiXmlElement* node, std::vector<Havok_File*> &listHavokFile, string filename)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";

	node_tmp = node->FirstChildElement("name"); if (node_tmp) node_tmp->QueryStringAttribute("value", &name);
	node_tmp = node->FirstChildElement("child"); if (node_tmp) node_tmp->QueryStringAttribute("u16", &str); child = EMO_BaseFile::GetUnsigned(str, child);
	node_tmp = node->FirstChildElement("unk_a0"); if (node_tmp) node_tmp->QueryStringAttribute("u16", &str); unk_a0 = EMO_BaseFile::GetUnsigned(str, unk_a0);
	node_tmp = node->FirstChildElement("sibling"); if (node_tmp) node_tmp->QueryStringAttribute("u16", &str); sibling = EMO_BaseFile::GetUnsigned(str, sibling);
	node_tmp = node->FirstChildElement("parent"); if (node_tmp) node_tmp->QueryStringAttribute("u16", &str); parent = EMO_BaseFile::GetUnsigned(str, parent);


	TiXmlElement* node_ListOnDes = node->FirstChildElement("ListOnDestruction");
	for (TiXmlElement* node_DesGroup = (node_ListOnDes) ? node_ListOnDes->FirstChildElement("DestructionGroup") : 0; node_DesGroup; node_DesGroup = node_DesGroup->NextSiblingElement("DestructionGroup"))
	{
		listOnDestruction.push_back(std::vector<FMP_Destruction>());
		
		for (TiXmlElement* node_tmp = (node_DesGroup) ? node_DesGroup->FirstChildElement("Destruction") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("Destruction"))
		{
			listOnDestruction.back().push_back(FMP_Destruction());
			if (!listOnDestruction.back().back().import_Xml(node_tmp, listHavokFile, filename))
				listOnDestruction.back().pop_back();
		}
	}



	TiXmlElement* node_ListHavok = node->FirstChildElement("ListHavokFiles");
	for (TiXmlElement* node_tmp = (node_ListHavok) ? node_ListHavok->FirstChildElement("HavokFile") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("HavokFile"))
	{
		listHavokFilesIndex.push_back(listHavokFile.size());
		Havok_File* havokFile = new Havok_File();
		listHavokFile.push_back(havokFile);

		if (!havokFile->import_Xml(node_tmp, filename))
		{
			listHavokFilesIndex.pop_back();
			listHavokFile.pop_back();
			delete havokFile;
		}
	}


	TiXmlElement* node_Mesh = node->FirstChildElement("CollisionGeometry");
	if(node_Mesh)
	{
		string meshFilename = "";
		node_Mesh->QueryStringAttribute("filename", &meshFilename);
		
		if ((meshFilename.length()>4)&&(meshFilename.substr(meshFilename.length() - 4)==".emd"))
		{
			meshFilename = LibXenoverse::folderFromFilename(filename) + LibXenoverse::nameFromFilenameNoExtension(filename) + "\\" + meshFilename;

			EMD emd;
			if ((emd.load(meshFilename))&&(emd.models.size()!=0) && (emd.models.at(0)->meshes.size() != 0) && (emd.models.at(0)->meshes.at(0)->submeshes.size() != 0))
			{
				EMDSubmesh* submesh = emd.models.at(0)->meshes.at(0)->submeshes.at(0);
				if (((submesh->vertex_type_flag & (EMD_VTX_FLAG_POS | EMD_VTX_FLAG_NORM)) != 0) && (submesh->vertices.size() != 0) && (submesh->triangles.size() != 0) && (submesh->triangles.at(0).faces.size() != 0))
				{
					vector<EMDVertex> &vertices = submesh->vertices;
					vector<unsigned short> &faces = submesh->triangles.at(0).faces;

					size_t nbVertex = vertices.size();
					for (size_t i = 0; i < nbVertex; i++)
					{
						EMDVertex &vert = vertices.at(i);
						Vertex vertex(vert.pos_x, vert.pos_y, vert.pos_z, vert.norm_x, vert.norm_y, vert.norm_z);
						listVertex.push_back(vertex);
					}

					size_t nbFaceIndex = faces.size();;
					for (size_t i = 0; i < nbFaceIndex; i++)
						listFaceIndex.push_back(faces.at(i));
				}
			}
		}
		
		
		/*
		TiXmlElement* node_ListVertex = node_Mesh->FirstChildElement("ListVertex");
		for (TiXmlElement* node_Vertex = (node_ListVertex) ? node_ListVertex->FirstChildElement("Vertex") : 0; node_Vertex; node_Vertex = node_Vertex->NextSiblingElement("Vertex"))
		{
			Vertex vertex(0,0,0,0,0,0);
			
			/*
			node_tmp = node_Vertex->FirstChildElement("Position");
			if (node_tmp)
			{
				node_tmp->QueryFloatAttribute_floatPrecision("X", &vertex.position[0]);
				node_tmp->QueryFloatAttribute_floatPrecision("Y", &vertex.position[1]);
				node_tmp->QueryFloatAttribute_floatPrecision("Z", &vertex.position[2]);
			}
			node_tmp = node_Vertex->FirstChildElement("Normal");
			if (node_tmp)
			{
				node_tmp->QueryFloatAttribute_floatPrecision("X", &vertex.normal[0]);
				node_tmp->QueryFloatAttribute_floatPrecision("Y", &vertex.normal[1]);
				node_tmp->QueryFloatAttribute_floatPrecision("Z", &vertex.normal[2]);
			}
			*//*

			//v2 because of memory of tinyXml
			node_Vertex->QueryStringAttribute("IdPosNor", &str);
			
			std::vector<string> sp = split(str, ',');
			if (sp.size() != 7)
				continue;

			vertex.position[0] = StringToFloat(sp.at(1));
			vertex.position[1] = StringToFloat(sp.at(2));
			vertex.position[2] = StringToFloat(sp.at(3));
			vertex.normal[0] = StringToFloat(sp.at(4));
			vertex.normal[1] = StringToFloat(sp.at(5));
			vertex.normal[2] = StringToFloat(sp.at(6));

			listVertex.push_back(vertex);
		}


		TiXmlElement* node_ListFacesIndex = node_Mesh->FirstChildElement("ListFaceIndex");
		size_t index = 0;
		for (TiXmlElement* node_tmp = (node_ListFacesIndex) ? node_ListFacesIndex->FirstChildElement("Index") : 0; node_tmp; node_tmp = node_tmp->NextSiblingElement("Index"))
		{
			/*
			node_tmp->QueryUnsignedAttribute("u16", &index);
			listFaceIndex.push_back(index);
			*//*

			//v2 because of memory of tinyXml
			node_tmp->QueryStringAttribute("u16", &str);
			if (str.length() == 0)
				continue;

			std::vector<string> sp = split(str, ',');

			size_t nbIndex = sp.size();
			for(size_t j=0;j<nbIndex;j++)
				listFaceIndex.push_back( stoul(sp.at(j)) );
		}
		*/
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool Havok_File::import_Xml(TiXmlElement* node, string originefilename)
{
	Reset();

	string filename = "";
	node->QueryStringAttribute("filename", &filename);

	filename = LibXenoverse::folderFromFilename(originefilename) + LibXenoverse::nameFromFilenameNoExtension(originefilename) + "\\" + filename;

	buffer = EMO_BaseFile::ReadFile(filename, &size, true);

	return ((buffer!=0)&&(size!=0));
}







/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_Destruction::import_Xml(TiXmlElement* node, std::vector<Havok_File*> &listHavokFile, string filename)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";

	node_tmp = node->FirstChildElement("unk_0"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_0 = EMO_BaseFile::GetUnsigned(str, unk_0);
	node_tmp = node->FirstChildElement("unk_1"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_1 = EMO_BaseFile::GetUnsigned(str, unk_1);
	node_tmp = node->FirstChildElement("unk_2"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_2 = EMO_BaseFile::GetUnsigned(str, unk_2);
	node_tmp = node->FirstChildElement("unk_5"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_5 = EMO_BaseFile::GetUnsigned(str, unk_5);
	node_tmp = node->FirstChildElement("unk_6"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_6 = EMO_BaseFile::GetUnsigned(str, unk_6);
	node_tmp = node->FirstChildElement("unk_7"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_7);
	

	TiXmlElement* node_sub0 = node->FirstChildElement("DestructionSubPart");
	if ((node_sub0) && (node_sub0->FirstChildElement("yaw")))
	{
		haveSub0 = true;
		if (!sub0.import_Xml(node_sub0))
		{
			haveSub0 = false;
			sub0.Reset();
		}
	}

	TiXmlElement* node_sub1 = (node_sub0) ? node_sub0->NextSiblingElement("DestructionSubPart") : 0;
	if ((node_sub1) && (node_sub1->FirstChildElement("yaw")))
	{
		haveSub1 = true;
		if (!sub1.import_Xml(node_sub1))
		{
			haveSub1 = false;
			sub1.Reset();
		}
	}


	node_tmp = node->FirstChildElement("HavokFile");
	if (node_tmp)
	{
		havokFileIndex = listHavokFile.size();
		Havok_File* havokFile = new Havok_File();
		listHavokFile.push_back(havokFile);

		if (!havokFile->import_Xml(node_tmp, filename))
		{
			havokFileIndex = (size_t)-1;
			listHavokFile.pop_back();
			delete havokFile;
		}
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool FMP_Destruction_sub::import_Xml(TiXmlElement* node)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";

	node_tmp = node->FirstChildElement("yaw"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &yaw);
	node_tmp = node->FirstChildElement("pitch"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &pitch);
	node_tmp = node->FirstChildElement("roll"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &roll);


	node_tmp = node->FirstChildElement("unk_0"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_0 = EMO_BaseFile::GetUnsigned(str, unk_0);
	node_tmp = node->FirstChildElement("unk_1"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_1);
	node_tmp = node->FirstChildElement("unk_2"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_2);
	node_tmp = node->FirstChildElement("unk_3"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_3);
	node_tmp = node->FirstChildElement("unk_7"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_7);
	node_tmp = node->FirstChildElement("unk_8"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_8);
	node_tmp = node->FirstChildElement("unk_9"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_9);
	node_tmp = node->FirstChildElement("unk_10"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_10);
	node_tmp = node->FirstChildElement("unk_11"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_11);
	node_tmp = node->FirstChildElement("unk_12"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_12);
	node_tmp = node->FirstChildElement("unk_13"); if (node_tmp) node_tmp->QueryFloatAttribute_floatPrecision("float", &unk_13);
	
	return true;
}



























/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Xml First Version for Debug ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*-------------------------------------------------------------------------------\
|                             save_Xml				                             |
\-------------------------------------------------------------------------------*/
void FmpFile::save_Xml(string filename, bool show_error)
{
	uint8_t *buf;
	size_t size;

	buf = ReadFile(filename, &size, show_error);
	if (!buf)
		return;

	TiXmlDocument *doc = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc->LinkEndChild(decl);

	TiXmlElement* root = new TiXmlElement("Fmp");
	EMO_BaseFile::WriteParamString(root, "path", filename);
	
	write_Xml(root, buf, size, filename);

	doc->LinkEndChild(root);



	delete[] buf;

	doc->SaveFile(filename + ".xml");

	return;
}




/*-------------------------------------------------------------------------------\
|                             write_Xml						                     |
\-------------------------------------------------------------------------------*/
void FmpFile::write_Xml(TiXmlElement *parent, const uint8_t *buf, size_t size, string filename)			//string filename is for extract inside files
{
	FMP_Header* hdr = (FMP_Header*)buf;
	if (size < sizeof(FMP_Header) || memcmp(hdr->signature, FMP_SIGNATURE, 4) != 0)
		return;


	std::vector<bool> listBytesAllreadyTagged;
	listBytesAllreadyTagged.resize(size, false);				//to check override of the same byte (overflow)


	//node->SetAttribute("value", UnsignedToString(value, hexadecimal));
	
	TiXmlElement* node_FMPHeader = new TiXmlElement("Header");
	TiXmlElement* node;
	node = new TiXmlElement("Version"); node->SetAttribute("u32", UnsignedToString(hdr->version, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("unk_08"); node->SetAttribute("u32", UnsignedToString(hdr->unk_08, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("unk_0C"); node->SetAttribute("u32", UnsignedToString(hdr->unk_0C, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Section0a"); node->SetAttribute("startOffset", UnsignedToString(hdr->offset_Section0a, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Section0b"); node->SetAttribute("startOffset", UnsignedToString(hdr->offset_Section0b, true)); node_FMPHeader->LinkEndChild(node);

	node = new TiXmlElement("Section1"); node->SetAttribute("count", hdr->numberSection1);  node->SetAttribute("startOffset", UnsignedToString(hdr->offset_Section1, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Section2"); node->SetAttribute("count", hdr->numberSection2);  node->SetAttribute("startOffset", UnsignedToString(hdr->offset_Section2, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Section3"); node->SetAttribute("count", hdr->numberSection3);  node->SetAttribute("startOffset", UnsignedToString(hdr->offset_Section3, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Section4"); node->SetAttribute("count", hdr->numberSection4);  node->SetAttribute("startOffset", UnsignedToString(hdr->offset_Section4, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Section5"); node->SetAttribute("count", hdr->numberSection5);  node->SetAttribute("startOffset", UnsignedToString(hdr->offset_Section5, true)); node_FMPHeader->LinkEndChild(node);

	node = new TiXmlElement("Nsk_Files"); node->SetAttribute("numberFile", hdr->numberFileInDependance_Nsk);  node->SetAttribute("startOffset", UnsignedToString(hdr->offset_Dependance_Nsk, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("TextureEmb_Files"); node->SetAttribute("numberFile", hdr->numberFileInDependance_TextureEmb);  node->SetAttribute("startOffset", UnsignedToString(hdr->offset_Dependance_TextureEmb, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Emm_Files"); node->SetAttribute("numberFile", hdr->numberFileInDependance_Emm);  node->SetAttribute("startOffset", UnsignedToString(hdr->offset_Dependance_Emm, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("Ema_Files"); node->SetAttribute("numberFile", hdr->numberFileInDependance_Ema);  node->SetAttribute("startOffset", UnsignedToString(hdr->offset_Dependance_Ema, true)); node_FMPHeader->LinkEndChild(node);
	
	node = new TiXmlElement("unk_60"); node->SetAttribute("u32", UnsignedToString(hdr->unk_60, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("unk_64"); node->SetAttribute("u32", UnsignedToString(hdr->unk_64, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("unk_68"); node->SetAttribute("u32", UnsignedToString(hdr->unk_68, true)); node_FMPHeader->LinkEndChild(node);
	node = new TiXmlElement("unk_6C"); node->SetAttribute("u32", UnsignedToString(hdr->unk_6C, true)); node_FMPHeader->LinkEndChild(node);
	parent->LinkEndChild(node_FMPHeader);
	



	FMP_Section0a* section0a = (FMP_Section0a*)GetOffsetPtr(buf, hdr->offset_Section0a);
	TiXmlElement* node_section0a = new TiXmlElement("Section0a");
	node = new TiXmlElement("Width"); 
	node->SetAttribute("X", std::to_string(section0a->width_X));
	node->SetAttribute("Y", std::to_string(section0a->width_Y));
	node->SetAttribute("Z", std::to_string(section0a->width_Z));
	node_section0a->LinkEndChild(node);

	node = new TiXmlElement("unk_3"); node->SetAttribute("float", std::to_string(section0a->unk_3)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_4"); node->SetAttribute("float", std::to_string(section0a->unk_4)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_5"); node->SetAttribute("float", std::to_string(section0a->unk_5)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_6"); node->SetAttribute("float", std::to_string(section0a->unk_6)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_7"); node->SetAttribute("u32", UnsignedToString(section0a->unk_7, true)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_8"); node->SetAttribute("float", std::to_string(section0a->unk_8)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_9"); node->SetAttribute("float", std::to_string(section0a->unk_9)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_10"); node->SetAttribute("float", std::to_string(section0a->unk_10)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_11"); node->SetAttribute("float", std::to_string(section0a->unk_11)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_12"); node->SetAttribute("float", std::to_string(section0a->unk_12)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_13"); node->SetAttribute("float", std::to_string(section0a->unk_13)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_14"); node->SetAttribute("float", std::to_string(section0a->unk_14)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_15"); node->SetAttribute("float", std::to_string(section0a->unk_15)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_16"); node->SetAttribute("float", std::to_string(section0a->unk_16)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_17"); node->SetAttribute("float", std::to_string(section0a->unk_17)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_18"); node->SetAttribute("float", std::to_string(section0a->unk_18)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_19"); node->SetAttribute("float", std::to_string(section0a->unk_19)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_20"); node->SetAttribute("float", std::to_string(section0a->unk_20)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_21"); node->SetAttribute("float", std::to_string(section0a->unk_21)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_22"); node->SetAttribute("float", std::to_string(section0a->unk_22)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_23"); node->SetAttribute("float", std::to_string(section0a->unk_23)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_24"); node->SetAttribute("float", std::to_string(section0a->unk_24)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_25"); node->SetAttribute("float", std::to_string(section0a->unk_25)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_26"); node->SetAttribute("float", std::to_string(section0a->unk_26)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_27"); node->SetAttribute("float", std::to_string(section0a->unk_27)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_28"); node->SetAttribute("float", std::to_string(section0a->unk_28)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_29"); node->SetAttribute("float", std::to_string(section0a->unk_29)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_30"); node->SetAttribute("float", std::to_string(section0a->unk_30)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_31"); node->SetAttribute("float", std::to_string(section0a->unk_31)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("nearDistance"); node->SetAttribute("float", std::to_string(section0a->nearDistance)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("farDistance"); node->SetAttribute("float", std::to_string(section0a->farDistance)); node_section0a->LinkEndChild(node);
	node = new TiXmlElement("unk_34"); node->SetAttribute("float", std::to_string(section0a->unk_34)); node_section0a->LinkEndChild(node);
	
	parent->LinkEndChild(node_section0a);


	
	FMP_Section0b* section0b = (FMP_Section0b*)GetOffsetPtr(buf, hdr->offset_Section0b);

	TiXmlElement* node_Section0b = new TiXmlElement("Section0b");
	node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(section0b->unk_0, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(section0b->unk_1, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_2"); node->SetAttribute("u32", UnsignedToString(section0b->unk_2, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_3"); node->SetAttribute("u32", UnsignedToString(section0b->unk_3, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_4"); node->SetAttribute("u32", UnsignedToString(section0b->unk_4, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_5"); node->SetAttribute("u32", UnsignedToString(section0b->unk_5, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_6"); node->SetAttribute("u32", UnsignedToString(section0b->unk_6, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_7"); node->SetAttribute("u32", UnsignedToString(section0b->unk_7, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_8"); node->SetAttribute("u32", UnsignedToString(section0b->unk_8, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_9"); node->SetAttribute("u32", UnsignedToString(section0b->unk_9, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_10"); node->SetAttribute("u32", UnsignedToString(section0b->unk_10, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_11"); node->SetAttribute("u32", UnsignedToString(section0b->unk_11, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_12"); node->SetAttribute("u32", UnsignedToString(section0b->unk_12, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_13"); node->SetAttribute("u32", UnsignedToString(section0b->unk_13, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_14"); node->SetAttribute("u32", UnsignedToString(section0b->unk_14, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_15"); node->SetAttribute("u32", UnsignedToString(section0b->unk_15, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_16"); node->SetAttribute("u32", UnsignedToString(section0b->unk_16, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_17"); node->SetAttribute("u32", UnsignedToString(section0b->unk_17, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_18"); node->SetAttribute("u32", UnsignedToString(section0b->unk_18, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_19"); node->SetAttribute("u32", UnsignedToString(section0b->unk_19, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_20"); node->SetAttribute("u32", UnsignedToString(section0b->unk_20, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_21"); node->SetAttribute("u32", UnsignedToString(section0b->unk_21, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_22"); node->SetAttribute("u32", UnsignedToString(section0b->unk_22, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_23"); node->SetAttribute("u16", UnsignedToString(section0b->unk_23, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_23b"); node->SetAttribute("u16", UnsignedToString(section0b->unk_23b, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_24"); node->SetAttribute("u32", UnsignedToString(section0b->unk_24, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_25"); node->SetAttribute("u32", UnsignedToString(section0b->unk_25, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_26"); node->SetAttribute("u32", UnsignedToString(section0b->unk_26, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_27"); node->SetAttribute("u32", UnsignedToString(section0b->unk_27, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_28"); node->SetAttribute("u32", UnsignedToString(section0b->unk_28, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_29"); node->SetAttribute("u32", UnsignedToString(section0b->unk_29, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_30"); node->SetAttribute("u32", UnsignedToString(section0b->unk_30, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_31"); node->SetAttribute("u32", UnsignedToString(section0b->unk_31, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_32"); node->SetAttribute("u32", UnsignedToString(section0b->unk_32, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_33"); node->SetAttribute("u32", UnsignedToString(section0b->unk_33, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_34"); node->SetAttribute("u32", UnsignedToString(section0b->unk_34, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_35"); node->SetAttribute("u32", UnsignedToString(section0b->unk_35, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_36"); node->SetAttribute("u32", UnsignedToString(section0b->unk_36, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_37"); node->SetAttribute("u32", UnsignedToString(section0b->unk_37, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_38"); node->SetAttribute("u32", UnsignedToString(section0b->unk_38, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_39"); node->SetAttribute("u32", UnsignedToString(section0b->unk_39, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_40"); node->SetAttribute("u32", UnsignedToString(section0b->unk_40, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_41"); node->SetAttribute("u32", UnsignedToString(section0b->unk_41, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_42"); node->SetAttribute("u32", UnsignedToString(section0b->unk_42, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_43"); node->SetAttribute("u32", UnsignedToString(section0b->unk_43, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_44"); node->SetAttribute("u32", UnsignedToString(section0b->unk_44, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_45"); node->SetAttribute("u32", UnsignedToString(section0b->unk_45, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_46"); node->SetAttribute("u32", UnsignedToString(section0b->unk_46, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_47"); node->SetAttribute("u32", UnsignedToString(section0b->unk_47, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_48"); node->SetAttribute("u32", UnsignedToString(section0b->unk_48, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_49"); node->SetAttribute("u32", UnsignedToString(section0b->unk_49, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_50"); node->SetAttribute("u32", UnsignedToString(section0b->unk_50, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_51"); node->SetAttribute("u32", UnsignedToString(section0b->unk_51, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_52"); node->SetAttribute("u32", UnsignedToString(section0b->unk_52, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_53"); node->SetAttribute("u32", UnsignedToString(section0b->unk_53, true)); node_Section0b->LinkEndChild(node);
	node = new TiXmlElement("unk_54"); node->SetAttribute("u32", UnsignedToString(section0b->unk_54, true)); node_Section0b->LinkEndChild(node);


	TiXmlElement* node_Section0b_names = new TiXmlElement("ItemTypes");
	uint32_t* name_offset;
	string name;
	for (size_t i = 0; i < 51; i++)
	{
		name_offset = (uint32_t*)GetOffsetPtr(buf, hdr->offset_Section0b + 0x100);
		name = name_offset[i] ? std::string((char *)GetOffsetPtr(buf, name_offset, i)) : "";

		node = new TiXmlElement("ItemType"); node->SetAttribute("id", UnsignedToString(i, true)); node->SetAttribute("name", name); node_Section0b_names->LinkEndChild(node);
	}
	node_Section0b->LinkEndChild(node_Section0b_names);

	parent->LinkEndChild(node_Section0b);


	



	FMP_Section1* section1 = (FMP_Section1*)GetOffsetPtr(buf, hdr->offset_Section1);
	TiXmlElement* node_ListSection1 = new TiXmlElement("ListSection1");
	node_ListSection1->SetAttribute("count", hdr->numberSection1);
	TiXmlElement* node_Section1;
	for (size_t i = 0; i < hdr->numberSection1; i++)
	{
		node_Section1 = new TiXmlElement("Section1");
		node_Section1->SetAttribute("index", i);
		
		name = std::string((char *)GetOffsetPtr(buf, section1[i].name_offset));
		node = new TiXmlElement("name"); node->SetAttribute("name", name); node_Section1->LinkEndChild(node);

		node = new TiXmlElement("unk_04"); node->SetAttribute("u32", UnsignedToString(section1[i].unk_04, true)); node_Section1->LinkEndChild(node);
		node = new TiXmlElement("unk_08"); node->SetAttribute("float", std::to_string(section1[i].unk_08)); node_Section1->LinkEndChild(node);
		node = new TiXmlElement("unk_0C"); node->SetAttribute("float", std::to_string(section1[i].unk_0C)); node_Section1->LinkEndChild(node);
		node = new TiXmlElement("unk_10"); node->SetAttribute("float", std::to_string(section1[i].unk_10)); node_Section1->LinkEndChild(node);
		node = new TiXmlElement("unk_14"); node->SetAttribute("float", std::to_string(section1[i].unk_14)); node_Section1->LinkEndChild(node);

		node_ListSection1->LinkEndChild(node_Section1);
	}
	parent->LinkEndChild(node_ListSection1);






	FMP_Section2* section2 = (FMP_Section2*)GetOffsetPtr(buf, hdr->offset_Section2);
	TiXmlElement* node_ListSection2 = new TiXmlElement("ListSection2");
	node_ListSection2->SetAttribute("count", hdr->numberSection2);
	TiXmlElement* node_Section2;
	for (size_t i = 0; i < hdr->numberSection2; i++)
	{
		node_Section2 = new TiXmlElement("Section2");
		node_Section2->SetAttribute("index", i);

		name = std::string((char *)GetOffsetPtr(buf, section2[i].name_offset));
		node = new TiXmlElement("name"); node->SetAttribute("name", name); node_Section2->LinkEndChild(node);

		node = new TiXmlElement("unk_04"); node->SetAttribute("float", std::to_string(section2[i].unk_04)); node_Section2->LinkEndChild(node);
		node = new TiXmlElement("unk_08"); node->SetAttribute("float", std::to_string(section2[i].unk_08)); node_Section2->LinkEndChild(node);
		node = new TiXmlElement("unk_0C"); node->SetAttribute("float", std::to_string(section2[i].unk_0C)); node_Section2->LinkEndChild(node);
		node = new TiXmlElement("unk_10"); node->SetAttribute("float", std::to_string(section2[i].unk_10)); node_Section2->LinkEndChild(node);
		node = new TiXmlElement("unk_14"); node->SetAttribute("float", std::to_string(section2[i].unk_14)); node_Section2->LinkEndChild(node);
		node = new TiXmlElement("unk_18"); node->SetAttribute("float", std::to_string(section2[i].unk_18)); node_Section2->LinkEndChild(node);

		node_ListSection2->LinkEndChild(node_Section2);
	}
	parent->LinkEndChild(node_ListSection2);




	FMP_Section3* section3 = (FMP_Section3*)GetOffsetPtr(buf, hdr->offset_Section3);
	TiXmlElement* node_ListSection3 = new TiXmlElement("ListSection3");
	node_ListSection3->SetAttribute("count", hdr->numberSection3);
	TiXmlElement* node_Section3;
	for (size_t i = 0; i < hdr->numberSection3; i++)
	{
		node_Section3 = new TiXmlElement("Section3");
		node_Section3->SetAttribute("index", i);

		name = std::string((char *)GetOffsetPtr(buf, section3[i].name_offset));
		node = new TiXmlElement("name"); node->SetAttribute("name", name); node_Section3->LinkEndChild(node);


		TiXmlElement* node_Indexes = new TiXmlElement("Indexes");
		size_t nbIndex = section3[i].num_indexes;
		node_Indexes->SetAttribute("count", nbIndex);

		for (size_t j = 0; j < nbIndex; j++)
		{
			size_t offset_b = section3[i].indexes_offset + j * sizeof(int16_t);
			int16_t value = *((int16_t*)GetOffsetPtr(buf, offset_b));

			node = new TiXmlElement("Index_ListSection4"); node->SetAttribute("value", value); node_Indexes->LinkEndChild(node);
		}
		node_Section3->LinkEndChild(node_Indexes);

		node_ListSection3->LinkEndChild(node_Section3);
	}
	parent->LinkEndChild(node_ListSection3);





	FMP_Section4* section4 = (FMP_Section4*)GetOffsetPtr(buf, hdr->offset_Section4);
	
	TiXmlElement* node_ListSection4 = new TiXmlElement("ListSection4");

	node_ListSection4->SetAttribute("count", hdr->numberSection4);
	TiXmlElement* node_Section4;


	for (size_t i = 0; i < hdr->numberSection4; i++)
	{
		node_Section4 = new TiXmlElement("Section4");
		node_Section4->SetAttribute("index", i);

		name = std::string((char *)GetOffsetPtr(buf, section4[i].name_offset));
		node = new TiXmlElement("name"); node->SetAttribute("name", name); node_Section4->LinkEndChild(node);

		node = new TiXmlElement("unk_04"); node->SetAttribute("u16", UnsignedToString(section4[i].unk_04, true)); node_Section4->LinkEndChild(node);
		node = new TiXmlElement("unk_04b"); node->SetAttribute("u16", UnsignedToString(section4[i].unk_04b, true)); node_Section4->LinkEndChild(node);
		node = new TiXmlElement("index_Section5"); node->SetAttribute("u16", UnsignedToString(section4[i].index_Section5, true)); node_Section4->LinkEndChild(node);
		node = new TiXmlElement("unk_0A"); node->SetAttribute("u8", UnsignedToString(section4[i].unk_0A, true)); node_Section4->LinkEndChild(node);
		node = new TiXmlElement("unk_0B"); node->SetAttribute("u8", UnsignedToString(section4[i].unk_0B, true)); node_Section4->LinkEndChild(node);
		

		size_t nb_Section4_Next = 0;					//nb_Section4_Next is the number of Hitbox on Section5 targeted.
		if (section4[i].index_Section5 < hdr->numberSection5)
		{
			FMP_Section5* section5 = (FMP_Section5*)GetOffsetPtr(buf, hdr->offset_Section5 + section4[i].index_Section5 * sizeof(FMP_Section5));
			nb_Section4_Next = section5->number_Section5_Hitbox;
		}



		
		if (section4[i].offset_Section4_Next)
		{
			TiXmlElement* node_Section4_NextList = new TiXmlElement("ListNext");

			for (size_t j = 0; j < nb_Section4_Next; j++)
			{
				size_t start_Section4_Next = section4[i].offset_Section4_Next + j * sizeof(FMP_Section4_Next);

				TiXmlElement* node_Next = new TiXmlElement("Next");
				node_Next->SetAttribute("index", j);
				FMP_Section4_Next* next = (FMP_Section4_Next*)GetOffsetPtr(buf, start_Section4_Next);


				node_Next->SetAttribute("startOffset", UnsignedToString(start_Section4_Next, true));
				node = new TiXmlElement("number_Section4_Next_b"); node->SetAttribute("u32", UnsignedToString(next->number_Section4_Next_b, true)); node_Next->LinkEndChild(node);
				node = new TiXmlElement("offset_Section4_Next_b"); node->SetAttribute("u32", UnsignedToString(next->offset_Section4_Next_b, true)); node_Next->LinkEndChild(node);



				if (next->offset_Section4_Next_b)
				{
					FMP_Section4_Next_b* next_b = (FMP_Section4_Next_b*)GetOffsetPtr(buf, next->offset_Section4_Next_b);
					TiXmlElement* node_ListNext_b = new TiXmlElement("ListNext_b");

					for (size_t k = 0; k < next->number_Section4_Next_b; k++)
					{
						TiXmlElement* node_Next_b = new TiXmlElement("Next_b");
						node_Next_b->SetAttribute("index", k);
						node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(next_b[k].unk_0, true)); node_Next_b->LinkEndChild(node);
						node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(next_b[k].unk_1, true)); node_Next_b->LinkEndChild(node);
						node_ListNext_b->LinkEndChild(node_Next_b);
					}
					node_Next->LinkEndChild(node_ListNext_b);
				}


				node = new TiXmlElement("offset_Section4_Next_c"); node->SetAttribute("u32", UnsignedToString(next->offset_Section4_Next_c, true)); node_Next->LinkEndChild(node);


				if (next->offset_Section4_Next_c)
				{
					FMP_Section4_Next_c* next_c = (FMP_Section4_Next_c*)GetOffsetPtr(buf, next->offset_Section4_Next_c);
					TiXmlElement* node_Next_c = new TiXmlElement("Next_c");
					node = new TiXmlElement("unk_0"); node->SetAttribute("u16", UnsignedToString(next_c->unk_0, true)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_0b"); node->SetAttribute("u16", UnsignedToString(next_c->unk_0b, true)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_1"); node->SetAttribute("u16", UnsignedToString(next_c->unk_1, true)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_1b"); node->SetAttribute("u16", UnsignedToString(next_c->unk_1b, true)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_2"); node->SetAttribute("u32", UnsignedToString(next_c->unk_2, true)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_3"); node->SetAttribute("float", std::to_string(next_c->unk_3)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_4"); node->SetAttribute("float", std::to_string(next_c->unk_4)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_5"); node->SetAttribute("float", std::to_string(next_c->unk_5)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_6"); node->SetAttribute("float", std::to_string(next_c->unk_6)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_7"); node->SetAttribute("float", std::to_string(next_c->unk_7)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_8"); node->SetAttribute("float", std::to_string(next_c->unk_8)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_9"); node->SetAttribute("float", std::to_string(next_c->unk_9)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_10"); node->SetAttribute("float", std::to_string(next_c->unk_10)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_11"); node->SetAttribute("float", std::to_string(next_c->unk_11)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_12"); node->SetAttribute("float", std::to_string(next_c->unk_12)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_13"); node->SetAttribute("float", std::to_string(next_c->unk_13)); node_Next_c->LinkEndChild(node);
					
					node = new TiXmlElement("width"); 
					node->SetAttribute("X", std::to_string(next_c->widthX));
					node->SetAttribute("Y", std::to_string(next_c->widthY));
					node->SetAttribute("Z", std::to_string(next_c->widthZ));
					node_Next_c->LinkEndChild(node);
					
					node = new TiXmlElement("Quaternion");
					node->SetAttribute("X", std::to_string(next_c->quaternionX));
					node->SetAttribute("Y", std::to_string(next_c->quaternionY));
					node->SetAttribute("Z", std::to_string(next_c->quaternionZ));
					node->SetAttribute("W", std::to_string(next_c->quaternionW));
					node_Next_c->LinkEndChild(node);

					node_Next->LinkEndChild(node_Next_c);
				}


				node = new TiXmlElement("offset_Section4_Next_c_2"); node->SetAttribute("u32", UnsignedToString(next->offset_Section4_Next_c_2, true)); node_Next->LinkEndChild(node);


				if (next->offset_Section4_Next_c_2)
				{
					FMP_Section4_Next_c* next_c = (FMP_Section4_Next_c*)GetOffsetPtr(buf, next->offset_Section4_Next_c_2);
					TiXmlElement* node_Next_c = new TiXmlElement("Next_c");
					node = new TiXmlElement("unk_0"); node->SetAttribute("u16", UnsignedToString(next_c->unk_0, true)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_0b"); node->SetAttribute("u16", UnsignedToString(next_c->unk_0b, true)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_1"); node->SetAttribute("u16", UnsignedToString(next_c->unk_1, true)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_1b"); node->SetAttribute("u16", UnsignedToString(next_c->unk_1b, true)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_2"); node->SetAttribute("u32", UnsignedToString(next_c->unk_2, true)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_3"); node->SetAttribute("float", std::to_string(next_c->unk_3)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_4"); node->SetAttribute("float", std::to_string(next_c->unk_4)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_5"); node->SetAttribute("float", std::to_string(next_c->unk_5)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_6"); node->SetAttribute("float", std::to_string(next_c->unk_6)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_7"); node->SetAttribute("float", std::to_string(next_c->unk_7)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_8"); node->SetAttribute("float", std::to_string(next_c->unk_8)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_9"); node->SetAttribute("float", std::to_string(next_c->unk_9)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_10"); node->SetAttribute("float", std::to_string(next_c->unk_10)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_11"); node->SetAttribute("float", std::to_string(next_c->unk_11)); node_Next_c->LinkEndChild(node);
					node = new TiXmlElement("unk_12"); node->SetAttribute("float", std::to_string(next_c->unk_12)); node_Next_c->LinkEndChild(node);

					node = new TiXmlElement("width");
					node->SetAttribute("X", std::to_string(next_c->widthX));
					node->SetAttribute("Y", std::to_string(next_c->widthY));
					node->SetAttribute("Z", std::to_string(next_c->widthZ));
					node_Next_c->LinkEndChild(node);

					node = new TiXmlElement("Quaternion");
					node->SetAttribute("X", std::to_string(next_c->quaternionX));
					node->SetAttribute("Y", std::to_string(next_c->quaternionY));
					node->SetAttribute("Z", std::to_string(next_c->quaternionZ));
					node->SetAttribute("W", std::to_string(next_c->quaternionW));
					node_Next_c->LinkEndChild(node);

					node_Next->LinkEndChild(node_Next_c);
				}


				node = new TiXmlElement("offset_Section4_Next_d"); node->SetAttribute("u32", UnsignedToString(next->offset_Section4_Next_d, true)); node_Next->LinkEndChild(node);


				if (next->offset_Section4_Next_d)
				{
					FMP_Section4_Next_d* next_d = (FMP_Section4_Next_d*)GetOffsetPtr(buf, next->offset_Section4_Next_d);
					TiXmlElement* node_Next_d = new TiXmlElement("Next_d");
					node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(next_d->unk_0, true)); node_Next_d->LinkEndChild(node);
					node = new TiXmlElement("unk_1a"); node->SetAttribute("u8", UnsignedToString(next_d->unk_1a, true)); node_Next_d->LinkEndChild(node);
					node = new TiXmlElement("unk_1b"); node->SetAttribute("u8", UnsignedToString(next_d->unk_1b, true)); node_Next_d->LinkEndChild(node);
					node = new TiXmlElement("unk_1c"); node->SetAttribute("u8", UnsignedToString(next_d->unk_1c, true)); node_Next_d->LinkEndChild(node);
					node = new TiXmlElement("unk_1d"); node->SetAttribute("u8", UnsignedToString(next_d->unk_1d, true)); node_Next_d->LinkEndChild(node);
					node = new TiXmlElement("number_Section4_Next_e"); node->SetAttribute("u32", UnsignedToString(next_d->number_Section4_Next_e, false)); node_Next_d->LinkEndChild(node);
					node = new TiXmlElement("offset_Section4_Next_e"); node->SetAttribute("u32", UnsignedToString(next_d->offset_Section4_Next_e, true)); node_Next_d->LinkEndChild(node);
					

					if (next_d->offset_Section4_Next_e)
					{
						FMP_Section4_Next_e* next_e = (FMP_Section4_Next_e*)GetOffsetPtr(buf, next_d->offset_Section4_Next_e);
						TiXmlElement* node_ListNext_e = new TiXmlElement("ListNext_e");

						for (size_t k = 0; k < next->number_Section4_Next_b; k++)
						{
							TiXmlElement* node_Next_e = new TiXmlElement("Next_e");
							node_Next_e->SetAttribute("index", k);
							name = std::string((char *)GetOffsetPtr(buf, next_e[k].name_offset));
							node = new TiXmlElement("name"); node->SetAttribute("name", name); node_Next_e->LinkEndChild(node);


							node = new TiXmlElement("unk_1a"); node->SetAttribute("u8", UnsignedToString(next_e[k].unk_1a, true)); node_Next_e->LinkEndChild(node);
							node = new TiXmlElement("unk_1b"); node->SetAttribute("u8", UnsignedToString(next_e[k].unk_1b, true)); node_Next_e->LinkEndChild(node);
							node = new TiXmlElement("unk_1c"); node->SetAttribute("u8", UnsignedToString(next_e[k].unk_1c, true)); node_Next_e->LinkEndChild(node);
							node = new TiXmlElement("unk_1d"); node->SetAttribute("u8", UnsignedToString(next_e[k].unk_1d, true)); node_Next_e->LinkEndChild(node);
							node = new TiXmlElement("number_Section4_Next_f"); node->SetAttribute("u32", UnsignedToString(next_e[k].number_Section4_Next_f, true)); node_Next_e->LinkEndChild(node);
							node = new TiXmlElement("offset_Section4_Next_f"); node->SetAttribute("u32", UnsignedToString(next_e[k].number_Section4_Next_f, true)); node_Next_e->LinkEndChild(node);





							if (next_e[k].offset_Section4_Next_f)
							{
								FMP_Section4_Next_f* next_f = (FMP_Section4_Next_f*)GetOffsetPtr(buf, next_e[k].offset_Section4_Next_f);
								TiXmlElement* node_ListNext_f = new TiXmlElement("ListNext_f");

								for (size_t m = 0; m < next_e[k].number_Section4_Next_f; m++)
								{
									TiXmlElement* node_Next_f = new TiXmlElement("Next_f");
									node_Next_f->SetAttribute("index", m);
									name = std::string((char *)GetOffsetPtr(buf, next_f[m].name_offset));
									node = new TiXmlElement("name"); node->SetAttribute("name", name); node_Next_f->LinkEndChild(node);
									node = new TiXmlElement("type_Data"); node->SetAttribute("u32", UnsignedToString(next_f[m].type_Data, false)); node_Next_f->LinkEndChild(node);
									node = new TiXmlElement("offset_Data"); node->SetAttribute("u32", UnsignedToString(next_f[m].offset_Data, true)); node_Next_f->LinkEndChild(node);

									

									switch (next_f[m].type_Data)					//notice all the file only have 0,1,2,3,4, 6, 8 
									{
									case 0:						//Bool
									{
										if (next_f[m].offset_Data)
										{
											bool test = ((*(uint32_t*)GetOffsetPtr(buf, next_f[m].offset_Data)) != 0);
											node = new TiXmlElement("Data"); node->SetAttribute("bool", test ? "True" : "False"); node_Next_f->LinkEndChild(node);
										}
										else {
											node = new TiXmlElement("Data"); node->SetAttribute("bool", ""); node_Next_f->LinkEndChild(node);
										}

									}
									break;

									case 1:						//int
									{
										if (next_f[m].offset_Data)
										{
											int test = (*(int*)GetOffsetPtr(buf, next_f[m].offset_Data));
											node = new TiXmlElement("Data"); node->SetAttribute("i32", test); node_Next_f->LinkEndChild(node);
										}
										else {
											node = new TiXmlElement("Data"); node->SetAttribute("i32", ""); node_Next_f->LinkEndChild(node);
										}
									}
									break;

									case 2:						//float
									{
										if (next_f[m].offset_Data)
										{
											float test = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data));
											node = new TiXmlElement("Data"); node->SetAttribute("float", std::to_string(test)); node_Next_f->LinkEndChild(node);
										}
										else {
											node = new TiXmlElement("Data"); node->SetAttribute("float", ""); node_Next_f->LinkEndChild(node);
										}
									}
									break;


									case 3:						//Direction Vector
									{
										if (next_f[m].offset_Data)
										{
											float x = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data));
											float y = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data + sizeof(float)));
											float z = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data + 2 * sizeof(float)));
											float w = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data + 3 * sizeof(float)));
											node = new TiXmlElement("Data");
											node->SetAttribute("X", std::to_string(x));
											node->SetAttribute("Y", std::to_string(x));
											node->SetAttribute("Z", std::to_string(x));
											node->SetAttribute("W", std::to_string(x));
											node_Next_f->LinkEndChild(node);
										}
										else {
											node = new TiXmlElement("Data"); node->SetAttribute("float", ""); node_Next_f->LinkEndChild(node);
										}
									}
									break;

									case 4:						//Position
									{
										if (next_f[m].offset_Data)
										{
											float x = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data));
											float y = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data + sizeof(float)));
											float z = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data + 2 * sizeof(float)));
											float w = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data + 3 * sizeof(float)));
											node = new TiXmlElement("Data");
											node->SetAttribute("X", std::to_string(x));
											node->SetAttribute("Y", std::to_string(x));
											node->SetAttribute("Z", std::to_string(x));
											node->SetAttribute("W", std::to_string(x));
											node_Next_f->LinkEndChild(node);
										}
										else {
											node = new TiXmlElement("Data"); node->SetAttribute("float", ""); node_Next_f->LinkEndChild(node);
										}
									}
									break;

									case 6:
									{
										if (next_f[m].offset_Data)
										{
											name = std::string((char *)GetOffsetPtr(buf, next_f[m].offset_Data));
											node = new TiXmlElement("Data"); node->SetAttribute("string", name); node_Next_f->LinkEndChild(node);
										}
										else {
											node = new TiXmlElement("Data"); node->SetAttribute("string", ""); node_Next_f->LinkEndChild(node);
										}
									}
									break;


									case 8:						//uint
									{
										if (next_f[m].offset_Data)
										{
											uint32_t test = (*(uint32_t*)GetOffsetPtr(buf, next_f[m].offset_Data));
											node = new TiXmlElement("Data"); node->SetAttribute("u32", test); node_Next_f->LinkEndChild(node);
										}
										else {
											node = new TiXmlElement("Data"); node->SetAttribute("u32", ""); node_Next_f->LinkEndChild(node);
										}
									}
									break;

									}

									node_ListNext_f->LinkEndChild(node_Next_f);
								}
								node_Next_e->LinkEndChild(node_ListNext_f);
							}

							node_ListNext_e->LinkEndChild(node_Next_e);
						}
						node_Next_d->LinkEndChild(node_ListNext_e);
					}


					node_Next->LinkEndChild(node_Next_d);
				}


				node = new TiXmlElement("unk_2"); node->SetAttribute("u16", UnsignedToString(next->unk_2, true)); node_Next->LinkEndChild(node);
				node = new TiXmlElement("unk_2b"); node->SetAttribute("u16", UnsignedToString(next->unk_2b, true)); node_Next->LinkEndChild(node);
				node = new TiXmlElement("unk_4"); node->SetAttribute("float", std::to_string(next->unk_4)); node_Next->LinkEndChild(node);
				node = new TiXmlElement("unk_5"); node->SetAttribute("float", std::to_string(next->unk_5)); node_Next->LinkEndChild(node);
				
				TiXmlElement* node_matrix = new TiXmlElement("TransformMatrix4x3");
				node = new TiXmlElement("L0"); node->SetAttribute("f32s", std::to_string(next->transformMatrix4x3[0]) + ", " + std::to_string(next->transformMatrix4x3[1]) + ", " + std::to_string(next->transformMatrix4x3[2])); node_matrix->LinkEndChild(node);
				node = new TiXmlElement("L1"); node->SetAttribute("f32s", std::to_string(next->transformMatrix4x3[3]) + ", " + std::to_string(next->transformMatrix4x3[4]) + ", " + std::to_string(next->transformMatrix4x3[5])); node_matrix->LinkEndChild(node);
				node = new TiXmlElement("L2"); node->SetAttribute("f32s", std::to_string(next->transformMatrix4x3[6]) + ", " + std::to_string(next->transformMatrix4x3[7]) + ", " + std::to_string(next->transformMatrix4x3[8])); node_matrix->LinkEndChild(node);
				node = new TiXmlElement("L3"); node->SetAttribute("f32s", std::to_string(next->transformMatrix4x3[9]) + ", " + std::to_string(next->transformMatrix4x3[10]) + ", " + std::to_string(next->transformMatrix4x3[11])); node_matrix->LinkEndChild(node);
				node_Next->LinkEndChild(node_matrix);


				node_Section4_NextList->LinkEndChild(node_Next);
			}			

			node_Section4->LinkEndChild(node_Section4_NextList);
		}
		

		
		


		


		
		if (section4[i].offset_Section4_Next3)
		{
			TiXmlElement* node_Next3 = new TiXmlElement("Next3");
			FMP_Section4_Next3* next3 = (FMP_Section4_Next3*)GetOffsetPtr(buf, section4[i].offset_Section4_Next3);


			node_Next3->SetAttribute("startOffset", UnsignedToString(section4[i].offset_Section4_Next3, true));
			node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(next3->unk_0, true)); node_Next3->LinkEndChild(node);
			node = new TiXmlElement("unk_1a"); node->SetAttribute("u8", UnsignedToString(next3->unk_1a, true)); node_Next3->LinkEndChild(node);
			node = new TiXmlElement("unk_1b"); node->SetAttribute("u8", UnsignedToString(next3->unk_1b, true)); node_Next3->LinkEndChild(node);
			node = new TiXmlElement("unk_1c"); node->SetAttribute("u8", UnsignedToString(next3->unk_1c, true)); node_Next3->LinkEndChild(node);
			node = new TiXmlElement("unk_1d"); node->SetAttribute("u8", UnsignedToString(next3->unk_1d, true)); node_Next3->LinkEndChild(node);
			node = new TiXmlElement("number_Section4_Next3_b"); node->SetAttribute("u32", UnsignedToString(next3->number_Section4_Next3_b, false)); node_Next3->LinkEndChild(node);
			node = new TiXmlElement("offset_Section4_Next3_b"); node->SetAttribute("u32", UnsignedToString(next3->offset_Section4_Next3_b, true)); node_Next3->LinkEndChild(node);



			if (next3->offset_Section4_Next3_b)
			{
				FMP_Section4_Next3_b* next3_b = (FMP_Section4_Next3_b*)GetOffsetPtr(buf, next3->offset_Section4_Next3_b);
				TiXmlElement* node_ListNext3_b = new TiXmlElement("ListNext3_b");

				for (size_t k = 0; k < next3->number_Section4_Next3_b; k++)
				{
					TiXmlElement* node_Next3_b = new TiXmlElement("Next3_b");

					name = std::string((char *)GetOffsetPtr(buf, next3_b[k].name_offset));
					node = new TiXmlElement("name"); node->SetAttribute("name", name); node_Next3_b->LinkEndChild(node);
					node = new TiXmlElement("unk_1a"); node->SetAttribute("u8", UnsignedToString(next3_b[k].unk_1a, true)); node_Next3_b->LinkEndChild(node);
					node = new TiXmlElement("unk_1b"); node->SetAttribute("u8", UnsignedToString(next3_b[k].unk_1b, true)); node_Next3_b->LinkEndChild(node);
					node = new TiXmlElement("unk_1c"); node->SetAttribute("u8", UnsignedToString(next3_b[k].unk_1c, true)); node_Next3_b->LinkEndChild(node);
					node = new TiXmlElement("unk_1d"); node->SetAttribute("u8", UnsignedToString(next3_b[k].unk_1d, true)); node_Next3_b->LinkEndChild(node);
					node = new TiXmlElement("number_Section4_Next3_c"); node->SetAttribute("u32", UnsignedToString(next3_b[k].number_Section4_Next_f, false)); node_Next3_b->LinkEndChild(node);
					node = new TiXmlElement("offset_Section4_Next3_c"); node->SetAttribute("u32", UnsignedToString(next3_b[k].offset_Section4_Next_f, true)); node_Next3_b->LinkEndChild(node);



					//notice  old FMP_Section4_Next3_c is exactly like FMP_Section4_Next_f
					if (next3_b[k].offset_Section4_Next_f)
					{
						FMP_Section4_Next_f* next_f = (FMP_Section4_Next_f*)GetOffsetPtr(buf, next3_b[k].offset_Section4_Next_f);
						TiXmlElement* node_ListNext_f = new TiXmlElement("ListNext_f");

						for (size_t m = 0; m < next3_b[k].number_Section4_Next_f; m++)
						{
							TiXmlElement* node_Next_f = new TiXmlElement("Next_f");
							node_Next_f->SetAttribute("index", m);
							name = std::string((char *)GetOffsetPtr(buf, next_f[m].name_offset));
							node = new TiXmlElement("name"); node->SetAttribute("name", name); node_Next_f->LinkEndChild(node);
							node = new TiXmlElement("type_Data"); node->SetAttribute("u32", UnsignedToString(next_f[m].type_Data, false)); node_Next_f->LinkEndChild(node);
							node = new TiXmlElement("offset_Data"); node->SetAttribute("u32", UnsignedToString(next_f[m].offset_Data, true)); node_Next_f->LinkEndChild(node);

							switch (next_f[m].type_Data)					//notice all the file only have 0,1,2,3,4, 6, 8 
							{
							case 0:						//Bool
							{
								if (next_f[m].offset_Data)
								{
									bool test = ((*(uint32_t*)GetOffsetPtr(buf, next_f[m].offset_Data)) != 0);
									node = new TiXmlElement("Data"); node->SetAttribute("bool", test ? "True" : "False"); node_Next_f->LinkEndChild(node);
								}else {
									node = new TiXmlElement("Data"); node->SetAttribute("bool", ""); node_Next_f->LinkEndChild(node);
								}
								
							}
							break;

							case 1:						//int
							{
								if (next_f[m].offset_Data)
								{
									int test = (*(int*)GetOffsetPtr(buf, next_f[m].offset_Data));
									node = new TiXmlElement("Data"); node->SetAttribute("i32", test); node_Next_f->LinkEndChild(node);
								}else {
									node = new TiXmlElement("Data"); node->SetAttribute("i32", ""); node_Next_f->LinkEndChild(node);
								}
							}
							break;

							case 2:						//float
							{
								if (next_f[m].offset_Data)
								{
									float test = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data));
									node = new TiXmlElement("Data"); node->SetAttribute("float", std::to_string(test)); node_Next_f->LinkEndChild(node);
								}else {
									node = new TiXmlElement("Data"); node->SetAttribute("float", ""); node_Next_f->LinkEndChild(node);
								}
							}
							break;


							case 3:						//Direction Vector
							{
								if (next_f[m].offset_Data)
								{
									float x = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data));
									float y = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data + sizeof(float)));
									float z = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data + 2 *sizeof(float)));
									float w = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data + 3 * sizeof(float)));
									node = new TiXmlElement("Data"); 
									node->SetAttribute("X", std::to_string(x)); 
									node->SetAttribute("Y", std::to_string(x));
									node->SetAttribute("Z", std::to_string(x));
									node->SetAttribute("W", std::to_string(x));
									node_Next_f->LinkEndChild(node);
								}
								else {
									node = new TiXmlElement("Data"); node->SetAttribute("float", ""); node_Next_f->LinkEndChild(node);
								}
							}
							break;

							case 4:						//Position
							{
								if (next_f[m].offset_Data)
								{
									float x = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data));
									float y = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data + sizeof(float)));
									float z = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data + 2 * sizeof(float)));
									float w = (*(float*)GetOffsetPtr(buf, next_f[m].offset_Data + 3 * sizeof(float)));
									node = new TiXmlElement("Data");
									node->SetAttribute("X", std::to_string(x));
									node->SetAttribute("Y", std::to_string(x));
									node->SetAttribute("Z", std::to_string(x));
									node->SetAttribute("W", std::to_string(x));
									node_Next_f->LinkEndChild(node);
								}
								else {
									node = new TiXmlElement("Data"); node->SetAttribute("float", ""); node_Next_f->LinkEndChild(node);
								}
							}
							break;

							case 6:
							{
								if (next_f[m].offset_Data)
								{
									name = std::string((char *)GetOffsetPtr(buf, next_f[m].offset_Data));
									node = new TiXmlElement("Data"); node->SetAttribute("string", name); node_Next_f->LinkEndChild(node);
								}else {
									node = new TiXmlElement("Data"); node->SetAttribute("string", ""); node_Next_f->LinkEndChild(node);
								}
							}
							break;


							case 8:						//uint
							{
								if (next_f[m].offset_Data)
								{
									uint32_t test = (*(uint32_t*)GetOffsetPtr(buf, next_f[m].offset_Data));
									node = new TiXmlElement("Data"); node->SetAttribute("u32", test); node_Next_f->LinkEndChild(node);
								}else {
									node = new TiXmlElement("Data"); node->SetAttribute("u32", ""); node_Next_f->LinkEndChild(node);
								}
							}
							break;

							}

							node_ListNext_f->LinkEndChild(node_Next_f);
						}
						node_Next3_b->LinkEndChild(node_ListNext_f);
					}


					node_ListNext3_b->LinkEndChild(node_Next3_b);
				}
				node_Next3->LinkEndChild(node_ListNext3_b);
			}

			node_Section4->LinkEndChild(node_Next3);
		}
		




		

		node = new TiXmlElement("number_Section4_Next2"); node->SetAttribute("u32", UnsignedToString(section4[i].number_Section4_Next2, true)); node_Section4->LinkEndChild(node);





		if (section4[i].offset_Section4_Next2)
		{			
			TiXmlElement* node_List_Next2 = new TiXmlElement("List_Next2");

			

			for (size_t k = 0; k < section4[i].number_Section4_Next2; k++)
			{
				size_t start_Section4_Next2 = section4[i].offset_Section4_Next2 + k * sizeof(FMP_Section4_Next2);


				TiXmlElement* node_Next2 = new TiXmlElement("Next2");
				node_Next2->SetAttribute("index", k);
				node_Next2->SetAttribute("startOffset", UnsignedToString(start_Section4_Next2, true));
				FMP_Section4_Next2* next2 = (FMP_Section4_Next2*)GetOffsetPtr(buf, start_Section4_Next2);



				TiXmlElement* node_Next2_b = new TiXmlElement("Next2_b");
				node_Next2_b->SetAttribute("startOffset", UnsignedToString(next2->offset_Section4_Next2_b, true));
				if (next2->offset_Section4_Next2_b)
				{
					FMP_Section4_Next2_b* next2_b = (FMP_Section4_Next2_b*)GetOffsetPtr(buf, next2->offset_Section4_Next2_b);

					name = std::string((char *)GetOffsetPtr(buf, next2_b->name_offset));
					node = new TiXmlElement("name"); node->SetAttribute("name", name); node_Next2_b->LinkEndChild(node);

					node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(next2_b->unk_0, true)); node_Next2_b->LinkEndChild(node);
					node = new TiXmlElement("number_Lod"); node->SetAttribute("u32", UnsignedToString(next2_b->number_Lod, false)); node_Next2_b->LinkEndChild(node);
					node = new TiXmlElement("indexTextureEmb"); node->SetAttribute("u32", UnsignedToString(next2_b->indexTextureEmb, false)); node_Next2_b->LinkEndChild(node);
					node = new TiXmlElement("unk_5"); node->SetAttribute("u32", UnsignedToString(next2_b->unk_5, true)); node_Next2_b->LinkEndChild(node);
					node = new TiXmlElement("unk_6"); node->SetAttribute("u32", UnsignedToString(next2_b->unk_6, true)); node_Next2_b->LinkEndChild(node);
					node = new TiXmlElement("indexEma"); node->SetAttribute("u32", UnsignedToString(next2_b->indexEma, next2_b->indexEma == 0xffffffff)); node_Next2_b->LinkEndChild(node);
					node = new TiXmlElement("unk_8"); node->SetAttribute("u32", UnsignedToString(next2_b->unk_8, true)); node_Next2_b->LinkEndChild(node);
					node = new TiXmlElement("unk_9"); node->SetAttribute("float", std::to_string(next2_b->unk_9)); node_Next2_b->LinkEndChild(node);
					node = new TiXmlElement("unk_10"); node->SetAttribute("float", std::to_string(next2_b->unk_10)); node_Next2_b->LinkEndChild(node);


					TiXmlElement* node_List_IndexNsk = new TiXmlElement("ListLod_IndexNsk");
					if (next2_b->number_Lod == 1)
					{
						node = new TiXmlElement("indexNsk"); node->SetAttribute("u32", UnsignedToString(next2_b->indexNsk, false)); node_List_IndexNsk->LinkEndChild(node);
					}else {

						if (next2_b->indexNsk)
						{
							uint32_t* indexes = (uint32_t*)GetOffsetPtr(buf, next2_b->indexNsk);

							for (size_t k = 0; k < next2_b->number_Lod; k++)
							{
								node = new TiXmlElement("indexNsk"); node->SetAttribute("u32", UnsignedToString(indexes[k], false)); node_List_IndexNsk->LinkEndChild(node);
							}
						}
					}
					node_Next2_b->LinkEndChild(node_List_IndexNsk);




					TiXmlElement* node_List_IndexEmm = new TiXmlElement("ListLod_IndexEmm");
					if (next2_b->number_Lod == 1)
					{
						node = new TiXmlElement("indexEmm"); node->SetAttribute("u32", UnsignedToString(next2_b->indexEmm, false)); node_List_IndexEmm->LinkEndChild(node);
					}
					else {

						if (next2_b->indexEmm)
						{
							uint32_t* indexes = (uint32_t*)GetOffsetPtr(buf, next2_b->indexEmm);

							for (size_t k = 0; k < next2_b->number_Lod; k++)
							{
								node = new TiXmlElement("indexEmm"); node->SetAttribute("u32", UnsignedToString(indexes[k], false)); node_List_IndexEmm->LinkEndChild(node);
							}
						}
					}
					node_Next2_b->LinkEndChild(node_List_IndexEmm);



					

					TiXmlElement* node_List_Distance = new TiXmlElement("ListLod_Distance");
					if (next2_b->number_Lod == 1)
					{
						node = new TiXmlElement("Distance"); node->SetAttribute("float", std::to_string(next2_b->lodDistance)); node_List_Distance->LinkEndChild(node);
					}
					else {

						uint32_t offset_lodDistance = *((uint32_t*)&(next2_b->lodDistance));
						if (offset_lodDistance)
						{
							float* distances = (float*)GetOffsetPtr(buf, offset_lodDistance);

							for (size_t k = 0; k < next2_b->number_Lod; k++)
							{
								node = new TiXmlElement("Distance"); node->SetAttribute("float", std::to_string(distances[k])); node_List_Distance->LinkEndChild(node);
							}
						}
					}
					node_Next2_b->LinkEndChild(node_List_Distance);


				}
				node_Next2->LinkEndChild(node_Next2_b);



				node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(next2->unk_0, true)); node_Next2->LinkEndChild(node);
				TiXmlElement* node_matrix = new TiXmlElement("TransformMatrix4x3");
				node = new TiXmlElement("L0"); node->SetAttribute("f32s", std::to_string(next2->transformMatrix4x3[0]) + ", " + std::to_string(next2->transformMatrix4x3[1]) + ", " + std::to_string(next2->transformMatrix4x3[2])); node_matrix->LinkEndChild(node);
				node = new TiXmlElement("L1"); node->SetAttribute("f32s", std::to_string(next2->transformMatrix4x3[3]) + ", " + std::to_string(next2->transformMatrix4x3[4]) + ", " + std::to_string(next2->transformMatrix4x3[5])); node_matrix->LinkEndChild(node);
				node = new TiXmlElement("L2"); node->SetAttribute("f32s", std::to_string(next2->transformMatrix4x3[6]) + ", " + std::to_string(next2->transformMatrix4x3[7]) + ", " + std::to_string(next2->transformMatrix4x3[8])); node_matrix->LinkEndChild(node);
				node = new TiXmlElement("L3"); node->SetAttribute("f32s", std::to_string(next2->transformMatrix4x3[9]) + ", " + std::to_string(next2->transformMatrix4x3[10]) + ", " + std::to_string(next2->transformMatrix4x3[11])); node_matrix->LinkEndChild(node);
				node_Next2->LinkEndChild(node_matrix);

				node_List_Next2->LinkEndChild(node_Next2);
			}

			node_Section4->LinkEndChild(node_List_Next2);
		}
		




		





		
		
		if (section4[i].offset_Section4_Next4)
		{
			TiXmlElement* node_Next4 = new TiXmlElement("Next4");
			node_Next4->SetAttribute("startOffset", UnsignedToString(section4[i].offset_Section4_Next4, true));
			FMP_Section4_Next4* next4 = (FMP_Section4_Next4*)GetOffsetPtr(buf, section4[i].offset_Section4_Next4);


			if (next4->offset_Section4_Next4_b)
			{
				FMP_Section4_Next4_b* next4_b = (FMP_Section4_Next4_b*)GetOffsetPtr(buf, next4->offset_Section4_Next4_b);

				TiXmlElement* node_ListNext4_b = new TiXmlElement("ListNext4_b");
				node_ListNext4_b->SetAttribute("startOffset", UnsignedToString(next4->offset_Section4_Next4_b, true));

				for(size_t j=0;j<next4->number_Section4_Next4_b;j++)
				{
					TiXmlElement* node_Next4_b = new TiXmlElement("Next4_b");
					node_Next4_b->SetAttribute("index", j);
					node = new TiXmlElement("unk_0"); node->SetAttribute("float", std::to_string(next4_b[j].unk_0)); node_Next4_b->LinkEndChild(node);
					node = new TiXmlElement("unk_1"); node->SetAttribute("float", std::to_string(next4_b[j].unk_1)); node_Next4_b->LinkEndChild(node);
					node = new TiXmlElement("unk_2"); node->SetAttribute("float", std::to_string(next4_b[j].unk_2)); node_Next4_b->LinkEndChild(node);
					node = new TiXmlElement("unk_3"); node->SetAttribute("float", std::to_string(next4_b[j].unk_3)); node_Next4_b->LinkEndChild(node);
					node = new TiXmlElement("unk_4"); node->SetAttribute("float", std::to_string(next4_b[j].unk_4)); node_Next4_b->LinkEndChild(node);
					node = new TiXmlElement("unk_5"); node->SetAttribute("float", std::to_string(next4_b[j].unk_5)); node_Next4_b->LinkEndChild(node);
					node = new TiXmlElement("unk_6"); node->SetAttribute("float", std::to_string(next4_b[j].unk_6)); node_Next4_b->LinkEndChild(node);
					node = new TiXmlElement("unk_7"); node->SetAttribute("float", std::to_string(next4_b[j].unk_7)); node_Next4_b->LinkEndChild(node);
					node = new TiXmlElement("unk_8"); node->SetAttribute("float", std::to_string(next4_b[j].unk_8)); node_Next4_b->LinkEndChild(node);
					node = new TiXmlElement("unk_9"); node->SetAttribute("float", std::to_string(next4_b[j].unk_9)); node_Next4_b->LinkEndChild(node);
					node = new TiXmlElement("unk_10"); node->SetAttribute("u32", UnsignedToString(next4_b[j].unk_10, true)); node_Next4_b->LinkEndChild(node);
					node = new TiXmlElement("indexToNext4_c_Transform"); node->SetAttribute("u32", UnsignedToString(next4_b[j].indexToNext4_c_Transform, false)); node_Next4_b->LinkEndChild(node);
					node_ListNext4_b->LinkEndChild(node_Next4_b);
				}
				node_Next4->LinkEndChild(node_ListNext4_b);
			}

			if (next4->offset_Section4_Next4_c)
			{
				FMP_Section4_Next4_c* next4_c = (FMP_Section4_Next4_c*)GetOffsetPtr(buf, next4->offset_Section4_Next4_c);

				TiXmlElement* node_ListNext4_c = new TiXmlElement("ListNext4_c");
				node_ListNext4_c->SetAttribute("startOffset", UnsignedToString(next4->offset_Section4_Next4_c, true));

				for (size_t j = 0; j<next4->number_Section4_Next4_c; j++)
				{
					TiXmlElement* node_Next4_c = new TiXmlElement("Next4_c");
					node_Next4_c->SetAttribute("index", j);

					node = new TiXmlElement("Position"); 
					node->SetAttribute("X", std::to_string(next4_c[j].positionX));
					node->SetAttribute("Y", std::to_string(next4_c[j].positionY));
					node->SetAttribute("Z", std::to_string(next4_c[j].positionZ));
					node_Next4_c->LinkEndChild(node);

					node = new TiXmlElement("Rotation_Radian");
					node->SetAttribute("X", std::to_string(next4_c[j].rotationX));
					node->SetAttribute("Y", std::to_string(next4_c[j].rotationY));
					node->SetAttribute("Z", std::to_string(next4_c[j].rotationZ));
					node_Next4_c->LinkEndChild(node);

					node = new TiXmlElement("Scale");
					node->SetAttribute("X", std::to_string(next4_c[j].scaleX));
					node->SetAttribute("Y", std::to_string(next4_c[j].scaleY));
					node->SetAttribute("Z", std::to_string(next4_c[j].scaleZ));
					node_Next4_c->LinkEndChild(node);
					
					node_ListNext4_c->LinkEndChild(node_Next4_c);
				}
				node_Next4->LinkEndChild(node_ListNext4_c);
			}



			if (next4->offset_Section4_Next4_d)
			{
				std::vector<uint32_t> adressesToCheck;
				std::vector<TiXmlElement*> parents;
				adressesToCheck.push_back(next4->offset_Section4_Next4_d);				//could be recursive
				parents.push_back(node_Next4);


				for (size_t j = 0; j < adressesToCheck.size(); j++)		//the list will grow in process
				{
					FMP_Section4_Next4_d* next4_d = (FMP_Section4_Next4_d*)GetOffsetPtr(buf, adressesToCheck.at(j));

					TiXmlElement* node_Next4_d = new TiXmlElement("Next4_d");
					node_Next4_d->SetAttribute("startOffset", UnsignedToString(adressesToCheck.at(j), true));
					node = new TiXmlElement("type"); node->SetAttribute("u8", UnsignedToString(next4_d->type, true)); node_Next4_d->LinkEndChild(node);
					node = new TiXmlElement("typeb"); node->SetAttribute("u8", UnsignedToString(next4_d->typeb, true)); node_Next4_d->LinkEndChild(node);
					node = new TiXmlElement("number_Index"); node->SetAttribute("u16", UnsignedToString(next4_d->number_Index, false)); node_Next4_d->LinkEndChild(node);
					parents.at(j)->LinkEndChild(node_Next4_d);


					if (next4_d->type == 1)				// isLastChild in hierarchy ? (== no child, just reference of something)
					{
						uint16_t* indexes = (uint16_t*)GetOffsetPtr(buf, adressesToCheck.at(j) + 2 * sizeof(uint32_t));

						TiXmlElement* node_IndexList = new TiXmlElement("IndexList");

						for (size_t k = 0; k < next4_d->number_Index; k++)
						{
							node = new TiXmlElement("Index"); node->SetAttribute("u16", UnsignedToString(indexes[k], true)); node_IndexList->LinkEndChild(node);
						}
						node_Next4_d->LinkEndChild(node_IndexList);

					}else{

						FMP_Section4_Next4_e* next4_e = (FMP_Section4_Next4_e*)GetOffsetPtr(buf, adressesToCheck.at(j) + 2 * sizeof(uint16_t));

						TiXmlElement* node_Next4_e = new TiXmlElement("Next4_e");
						node = new TiXmlElement("unk_0"); node->SetAttribute("float", std::to_string(next4_e->unk_0)); node_Next4_e->LinkEndChild(node);
						node = new TiXmlElement("unk_1"); node->SetAttribute("float", std::to_string(next4_e->unk_1)); node_Next4_e->LinkEndChild(node);
						node = new TiXmlElement("unk_2"); node->SetAttribute("float", std::to_string(next4_e->unk_2)); node_Next4_e->LinkEndChild(node);

						node = new TiXmlElement("unk0_offset"); node->SetAttribute("u32", UnsignedToString(next4_e->unk0_offset, true)); node_Next4_e->LinkEndChild(node);
						if (next4_e->unk0_offset)
						{
							adressesToCheck.push_back(next4_e->unk0_offset);
							parents.push_back(node_Next4_e);
						}
						node = new TiXmlElement("unk1_offset"); node->SetAttribute("u32", UnsignedToString(next4_e->unk1_offset, true)); node_Next4_e->LinkEndChild(node);
						if (next4_e->unk1_offset)
						{
							adressesToCheck.push_back(next4_e->unk1_offset);
							parents.push_back(node_Next4_e);
						}
						node = new TiXmlElement("unk2_offset"); node->SetAttribute("u32", UnsignedToString(next4_e->unk2_offset, true)); node_Next4_e->LinkEndChild(node);
						if (next4_e->unk2_offset)
						{
							adressesToCheck.push_back(next4_e->unk2_offset);
							parents.push_back(node_Next4_e);
						}
						node = new TiXmlElement("unk3_offset"); node->SetAttribute("u32", UnsignedToString(next4_e->unk3_offset, true)); node_Next4_e->LinkEndChild(node);
						if (next4_e->unk3_offset)
						{
							adressesToCheck.push_back(next4_e->unk3_offset);
							parents.push_back(node_Next4_e);
						}
						node = new TiXmlElement("unk4_offset"); node->SetAttribute("u32", UnsignedToString(next4_e->unk4_offset, true)); node_Next4_e->LinkEndChild(node);
						if (next4_e->unk4_offset)
						{
							adressesToCheck.push_back(next4_e->unk4_offset);
							parents.push_back(node_Next4_e);
						}
						node = new TiXmlElement("unk5_offset"); node->SetAttribute("u32", UnsignedToString(next4_e->unk5_offset, true)); node_Next4_e->LinkEndChild(node);
						if (next4_e->unk5_offset)
						{
							adressesToCheck.push_back(next4_e->unk5_offset);
							parents.push_back(node_Next4_e);
						}
						node = new TiXmlElement("unk6_offset"); node->SetAttribute("u32", UnsignedToString(next4_e->unk6_offset, true)); node_Next4_e->LinkEndChild(node);
						if (next4_e->unk6_offset)
						{
							adressesToCheck.push_back(next4_e->unk6_offset);
							parents.push_back(node_Next4_e);
						}
						node = new TiXmlElement("unk7_offset"); node->SetAttribute("u32", UnsignedToString(next4_e->unk7_offset, true)); node_Next4_e->LinkEndChild(node);
						if (next4_e->unk7_offset)
						{
							adressesToCheck.push_back(next4_e->unk7_offset);
							parents.push_back(node_Next4_e);
						}

						node_Next4_d->LinkEndChild(node_Next4_e);
					}
				}
			}
			node_Section4->LinkEndChild(node_Next4);
		}
		





		node = new TiXmlElement("unk_20"); node->SetAttribute("float", std::to_string(section4[i].unk_20)); node_Section4->LinkEndChild(node);


		TiXmlElement* node_matrix = new TiXmlElement("TransformMatrix4x3");
		node = new TiXmlElement("L0"); node->SetAttribute("f32s", std::to_string(section4[i].transformMatrix4x3[0]) +", "+ std::to_string(section4[i].transformMatrix4x3[1]) + ", " + std::to_string(section4[i].transformMatrix4x3[2])); node_matrix->LinkEndChild(node);
		node = new TiXmlElement("L1"); node->SetAttribute("f32s", std::to_string(section4[i].transformMatrix4x3[3]) + ", " + std::to_string(section4[i].transformMatrix4x3[4]) + ", " + std::to_string(section4[i].transformMatrix4x3[5])); node_matrix->LinkEndChild(node);
		node = new TiXmlElement("L2"); node->SetAttribute("f32s", std::to_string(section4[i].transformMatrix4x3[6]) + ", " + std::to_string(section4[i].transformMatrix4x3[7]) + ", " + std::to_string(section4[i].transformMatrix4x3[8])); node_matrix->LinkEndChild(node);
		node = new TiXmlElement("L3"); node->SetAttribute("f32s", std::to_string(section4[i].transformMatrix4x3[9]) + ", " + std::to_string(section4[i].transformMatrix4x3[10]) + ", " + std::to_string(section4[i].transformMatrix4x3[11])); node_matrix->LinkEndChild(node);
		node_Section4->LinkEndChild(node_matrix);

		node_ListSection4->LinkEndChild(node_Section4);
	}
	parent->LinkEndChild(node_ListSection4);










	FMP_Section5* unk5 = (FMP_Section5*)GetOffsetPtr(buf, hdr->offset_Section5);
	TiXmlElement* node_Boxes;
	TiXmlElement* node_Box;
	TiXmlElement* node_Section5;
	size_t offset_tmp;

	TiXmlElement* node_ListSection5 = new TiXmlElement("ListSection5");
	node_ListSection5->SetAttribute("count", hdr->numberSection5);


	for (size_t i = 0; i < hdr->numberSection5; i++)
	{
		node_Section5 = new TiXmlElement("Section5");
		node_Section5->SetAttribute("index", i);
		node_Section5->SetAttribute("startOffset", hdr->offset_Section5 + i * sizeof(FMP_Section5));


		name = std::string((char *)GetOffsetPtr(buf, unk5[i].name_offset));
		node = new TiXmlElement("name"); node->SetAttribute("name", name); node_Section5->LinkEndChild(node);


		node_Boxes = new TiXmlElement("HitBoxes");
		size_t nbBox = unk5[i].number_Section5_Hitbox;
		node_Boxes->SetAttribute("count", nbBox);

		if (unk5[i].offset_Section5_Hitbox)
		{
			uint32_t offset = unk5[i].offset_Section5_Hitbox;
			size_t start_Section5_HitBox_list = unk5[i].offset_Section5_Hitbox;
			
			for (size_t j = 0; j < nbBox; j++)
			{
				bool isOldVersion = ((hdr->version & 0xFFF00) == 0x20800);

				size_t start_Section5_HitBox = start_Section5_HitBox_list + j * ((!isOldVersion) ? sizeof(FMP_Section5_Hitbox) : sizeof(FMP_Section5_Hitbox_Old_V0x0x208XX));


				FMP_Section5_Hitbox* section5_Hitbox = 0;

				if (!isOldVersion)
				{
					section5_Hitbox = (FMP_Section5_Hitbox*)GetOffsetPtr(buf, start_Section5_HitBox);
				}
				else {
					FMP_Section5_Hitbox_Old_V0x0x208XX* section5_Hitbox_Old = (FMP_Section5_Hitbox_Old_V0x0x208XX*)GetOffsetPtr(buf, start_Section5_HitBox);

					section5_Hitbox = new FMP_Section5_Hitbox();

					section5_Hitbox->name_offset = section5_Hitbox_Old->name_offset;
					section5_Hitbox->child = section5_Hitbox_Old->child;
					section5_Hitbox->unk_a0 = section5_Hitbox_Old->unk_a0;
					section5_Hitbox->sibling = section5_Hitbox_Old->sibling;
					section5_Hitbox->parent = section5_Hitbox_Old->parent;
					section5_Hitbox->number_Section5_Hitbox_b = section5_Hitbox_Old->number_Section5_Hitbox_b;
					section5_Hitbox->offset_Section5_Hitbox_b = section5_Hitbox_Old->offset_Section5_Hitbox_b;
					section5_Hitbox->number_Section5_Hitbox_e = section5_Hitbox_Old->number_Section5_Hitbox_e;
					section5_Hitbox->offset_Section5_Hitbox_e = section5_Hitbox_Old->offset_Section5_Hitbox_e;
					section5_Hitbox->number_Indexes = section5_Hitbox_Old->number_Indexes;
					section5_Hitbox->offset_Indexes = section5_Hitbox_Old->offset_Indexes;
				}


				
				
				uint32_t* nameOffset_tmp = (uint32_t*)GetOffsetPtr(buf, start_Section5_HitBox);
				if ((*nameOffset_tmp >= (uint32_t)size) || (*nameOffset_tmp <= start_Section5_HitBox))
				{
					printf("Error : overflow on HitBox name [offset=%s]. skipped all next hitbox from Section5.\n", UnsignedToString(start_Section5_HitBox, true).c_str());
					LibXenoverse::notifyError();
					break;						//we can't know witch size in bytes.
				}
				
				node_Box = new TiXmlElement("HitBox");
				node_Box->SetAttribute("index", j);
				node_Box->SetAttribute("startOffset", start_Section5_HitBox);
				node = new TiXmlElement("name"); node->SetAttribute("name", name); node_Box->LinkEndChild(node);
				
				node = new TiXmlElement("child"); node->SetAttribute("u16", UnsignedToString(section5_Hitbox->child, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("unk_a0"); node->SetAttribute("u16", UnsignedToString(section5_Hitbox->unk_a0, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("sibling"); node->SetAttribute("u16", UnsignedToString(section5_Hitbox->sibling, true)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("parent"); node->SetAttribute("u16", UnsignedToString(section5_Hitbox->parent, true)); node_Box->LinkEndChild(node);

				node = new TiXmlElement("number_Section5_Hitbox_b"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox->number_Section5_Hitbox_b, false)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("offset_Section5_Hitbox_b"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox->offset_Section5_Hitbox_b, true)); node_Box->LinkEndChild(node);


				offset_tmp = section5_Hitbox->offset_Section5_Hitbox_b;
				if (offset_tmp)
				{
					size_t start_Section5_Hitbox_b_list = offset_tmp;

					TiXmlElement* node_List_HitBox_b = new TiXmlElement("List_HitBox_b");

					for (size_t k = 0; k < section5_Hitbox->number_Section5_Hitbox_b; k++)
					{
						size_t start_Section5_Hitbox_b = start_Section5_Hitbox_b_list + k * sizeof(FMP_Section5_Hitbox_b);
						offset = start_Section5_Hitbox_b;


						FMP_Section5_Hitbox_b* section5_Hitbox_b = (FMP_Section5_Hitbox_b*)GetOffsetPtr(buf, start_Section5_Hitbox_b);


						TiXmlElement* node_HitBox_b = new TiXmlElement("HitBox_b");
						node_HitBox_b->SetAttribute("index", k);
						node_HitBox_b->SetAttribute("startOffset", start_Section5_Hitbox_b);
						
						node = new TiXmlElement("number_Section5_Hitbox_c"); node->SetAttribute("u32", section5_Hitbox_b->number_Section5_Hitbox_c); node_HitBox_b->LinkEndChild(node);
						node = new TiXmlElement("offset_Section5_Hitbox_c"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox_b->offset_Section5_Hitbox_c, true)); node_HitBox_b->LinkEndChild(node);

						offset_tmp = section5_Hitbox_b->offset_Section5_Hitbox_c;
						if (offset_tmp)
						{
							size_t start_Section5_Hitbox_c_list = offset_tmp;;

							TiXmlElement* node_List_HitBox_c = new TiXmlElement("List_HitBox_c");

							for (size_t m = 0; m<section5_Hitbox_b->number_Section5_Hitbox_c; m++)
							{
								size_t start_Section5_Hitbox_c = start_Section5_Hitbox_c_list + m * sizeof(FMP_Section5_Hitbox_c);
								offset = start_Section5_Hitbox_c;

								FMP_Section5_Hitbox_c* section5_Hitbox_c = (FMP_Section5_Hitbox_c*)GetOffsetPtr(buf, start_Section5_Hitbox_c);

								TiXmlElement* node_HitBox_c = new TiXmlElement("HitBox_c");
								node_HitBox_c->SetAttribute("index", m);
								node_HitBox_c->SetAttribute("startOffset", start_Section5_Hitbox_c);

								node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox_c->unk_0, true)); node_HitBox_c->LinkEndChild(node);
								node = new TiXmlElement("offset_Section5_Hitbox_d"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox_c->offset_Section5_Hitbox_d, true)); node_HitBox_c->LinkEndChild(node);


								offset_tmp = section5_Hitbox_c->offset_Section5_Hitbox_d;
								if (offset_tmp)
								{
									size_t start_Section5_Hitbox_d = offset_tmp;
									offset = start_Section5_Hitbox_d;

									FMP_Section5_Hitbox_d* section5_Hitbox_d = (FMP_Section5_Hitbox_d*)GetOffsetPtr(buf, start_Section5_Hitbox_d);

									TiXmlElement* node_HitBox_d = new TiXmlElement("HitBox_d");

									node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox_d->unk_0, true)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_1"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_1)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_2"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_2)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_3"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_3)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("yaw"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->yaw)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("pitch"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->pitch)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("roll"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->roll)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_7"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_7)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_8"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_8)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_9"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_9)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_10"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_10)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_11"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_11)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_12"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_12)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_13"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_13)); node_HitBox_d->LinkEndChild(node);

									node_HitBox_c->LinkEndChild(node_HitBox_d);
								}


								node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox_c->unk_1, true)); node_HitBox_c->LinkEndChild(node);
								node = new TiXmlElement("unk_2"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox_c->unk_2, true)); node_HitBox_c->LinkEndChild(node);

								node = new TiXmlElement("size_Section5_File"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox_c->size_Section5_File, true)); node_HitBox_c->LinkEndChild(node);
								node = new TiXmlElement("offset_Section5_File"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox_c->offset_Section5_File, true)); node_HitBox_c->LinkEndChild(node);


								//extract the files
								if ((section5_Hitbox_c->offset_Section5_File) && (section5_Hitbox_c->size_Section5_File))
								{
									string name_tmp = filename + "_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k) + "_" + std::to_string(m) + ".unknow";
									TiXmlElement* node_File = new TiXmlElement("File");
									node->SetAttribute("Extract_Filename", name_tmp);
									node_HitBox_c->LinkEndChild(node_File);
									
									EMO_BaseFile::WriteFile(name_tmp, buf + section5_Hitbox_c->offset_Section5_File, section5_Hitbox_c->size_Section5_File, true, false);
								}





								node = new TiXmlElement("offset_Section5_Hitbox_d_2"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox_c->offset_Section5_File, true)); node_HitBox_c->LinkEndChild(node);


								offset_tmp = section5_Hitbox_c->offset_Section5_Hitbox_d_2;
								if (offset_tmp)
								{
									size_t start_Section5_Hitbox_d = offset_tmp;
									offset = start_Section5_Hitbox_d;

									FMP_Section5_Hitbox_d* section5_Hitbox_d = (FMP_Section5_Hitbox_d*)GetOffsetPtr(buf, start_Section5_Hitbox_d);

									TiXmlElement* node_HitBox_d = new TiXmlElement("HitBox_d");

									node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox_d->unk_0, true)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_1"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_1)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_2"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_2)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_3"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_3)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("yaw"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->yaw)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("pitch"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->pitch)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("roll"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->roll)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_7"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_7)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_8"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_8)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_9"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_9)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_10"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_10)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_11"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_11)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_12"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_12)); node_HitBox_d->LinkEndChild(node);
									node = new TiXmlElement("unk_13"); node->SetAttribute("float", std::to_string(section5_Hitbox_d->unk_13)); node_HitBox_d->LinkEndChild(node);

									node_HitBox_c->LinkEndChild(node_HitBox_d);
								}

								node = new TiXmlElement("unk_5"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox_c->unk_5, true)); node_HitBox_c->LinkEndChild(node);
								node = new TiXmlElement("unk_6"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox_c->unk_6, true)); node_HitBox_c->LinkEndChild(node);
								node = new TiXmlElement("unk_7"); node->SetAttribute("float", std::to_string(section5_Hitbox_c->unk_7)); node_HitBox_c->LinkEndChild(node);
								

								node_List_HitBox_c->LinkEndChild(node_HitBox_c);
							}
							node_HitBox_b->LinkEndChild(node_List_HitBox_c);
						}

						node_List_HitBox_b->LinkEndChild(node_HitBox_b);
					}
					node_Box->LinkEndChild(node_List_HitBox_b);
				}



				node = new TiXmlElement("offset_Section5_Hitbox_f"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox->offset_Section5_Hitbox_f, true)); node_Box->LinkEndChild(node);



				offset_tmp = section5_Hitbox->offset_Section5_Hitbox_f;
				if ((offset_tmp) && (!isOldVersion))
				{
					size_t start_Section5_Hitbox_f = offset_tmp;
					offset = start_Section5_Hitbox_f;


					FMP_Section5_Hitbox_f* section5_Hitbox_f = (FMP_Section5_Hitbox_f*)GetOffsetPtr(buf, start_Section5_Hitbox_f);

					TiXmlElement* node_HitBox_f = new TiXmlElement("HitBox_f");
					
					node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox_f->unk_0, true)); node_HitBox_f->LinkEndChild(node);
					node = new TiXmlElement("size_Section5_File"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox_f->size_Section5_File, true)); node_HitBox_f->LinkEndChild(node);
					node = new TiXmlElement("offset_Section5_File"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox_f->offset_Section5_File, true)); node_HitBox_f->LinkEndChild(node);

					//extract the files
					if ((section5_Hitbox_f->offset_Section5_File) && (section5_Hitbox_f->size_Section5_File))
					{
						string name_tmp = filename + "_" + std::to_string(i) + "_" + std::to_string(j) + ".unknow";
						TiXmlElement* node_File = new TiXmlElement("File");
						node->SetAttribute("Extract_Filename", name_tmp);
						node_HitBox_f->LinkEndChild(node_File);

						EMO_BaseFile::WriteFile(name_tmp, buf + section5_Hitbox_f->offset_Section5_File, section5_Hitbox_f->size_Section5_File, true, false);
					}

					node_Box->LinkEndChild(node_HitBox_f);
				}






				node = new TiXmlElement("number_Section5_Hitbox_e"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox->number_Section5_Hitbox_e, false)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("offset_Section5_Hitbox_e"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox->offset_Section5_Hitbox_e, true)); node_Box->LinkEndChild(node);

				offset_tmp = section5_Hitbox->offset_Section5_Hitbox_e;
				if (offset_tmp)
				{
					size_t start_Section5_Hitbox_e = offset_tmp;
					offset = start_Section5_Hitbox_e;

					FMP_Section5_Hitbox_e* section5_Hitbox_e = (FMP_Section5_Hitbox_e*)GetOffsetPtr(buf, start_Section5_Hitbox_e);

					TiXmlElement* node_List_HitBox_e = new TiXmlElement("List_HitBox_e");

					for (size_t k = 0; k < section5_Hitbox->number_Section5_Hitbox_e; k++)
					{
						TiXmlElement* node_HitBox_e = new TiXmlElement("HitBox_e");

						node = new TiXmlElement("unk_0"); node->SetAttribute("float", std::to_string(section5_Hitbox_e[k].unk_0)); node_HitBox_e->LinkEndChild(node);
						node = new TiXmlElement("unk_1"); node->SetAttribute("float", std::to_string(section5_Hitbox_e[k].unk_1)); node_HitBox_e->LinkEndChild(node);
						node = new TiXmlElement("unk_2"); node->SetAttribute("float", std::to_string(section5_Hitbox_e[k].unk_2)); node_HitBox_e->LinkEndChild(node);
						node = new TiXmlElement("unk_3"); node->SetAttribute("float", std::to_string(section5_Hitbox_e[k].unk_3)); node_HitBox_e->LinkEndChild(node);
						node = new TiXmlElement("unk_4"); node->SetAttribute("float", std::to_string(section5_Hitbox_e[k].unk_4)); node_HitBox_e->LinkEndChild(node);
						node = new TiXmlElement("unk_5"); node->SetAttribute("float", std::to_string(section5_Hitbox_e[k].unk_5)); node_HitBox_e->LinkEndChild(node);

						node_List_HitBox_e->LinkEndChild(node_HitBox_e);
					}
					node_Box->LinkEndChild(node_List_HitBox_e);
				}







				node = new TiXmlElement("number_Indexes"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox->number_Indexes, false)); node_Box->LinkEndChild(node);
				node = new TiXmlElement("offset_Indexes"); node->SetAttribute("u32", UnsignedToString(section5_Hitbox->offset_Indexes, true)); node_Box->LinkEndChild(node);


				offset_tmp = section5_Hitbox->offset_Indexes;
				if ((offset_tmp) && (section5_Hitbox->number_Indexes))
				{
					offset = offset_tmp;

					TiXmlElement* node_List_Index = new TiXmlElement("List_Index");
					uint16_t* index = (uint16_t*)GetOffsetPtr(buf, section5_Hitbox->offset_Indexes);

					for (size_t k = 0; k < section5_Hitbox->number_Indexes; k++)
					{
						TiXmlElement* node_Index = new TiXmlElement("Index");
						node_Index->SetAttribute("value", index[k]);
						node_List_Index->LinkEndChild(node_Index);
					}

					node_Box->LinkEndChild(node_List_Index);
				}

				
				

				if (isOldVersion)
					delete section5_Hitbox;

				node_Boxes->LinkEndChild(node_Box);
			}
		}

		node_Section5->LinkEndChild(node_Boxes);


		node_ListSection5->LinkEndChild(node_Section5);
	}
	parent->LinkEndChild(node_ListSection5);


	






	TiXmlElement* node_Dependance_Nsk = new TiXmlElement("Nsk_Files");
	node_Dependance_Nsk->SetAttribute("count", hdr->numberFileInDependance_Nsk);
	name_offset = (uint32_t*)GetOffsetPtr(buf, hdr->offset_Dependance_Nsk);
	for (size_t i = 0; i < hdr->numberFileInDependance_Nsk; i++)
	{
		name = std::string((char *)GetOffsetPtr(buf, name_offset[i]));
		node = new TiXmlElement("File"); node->SetAttribute("index", i); node->SetAttribute("filename", name); node_Dependance_Nsk->LinkEndChild(node);
	}
	parent->LinkEndChild(node_Dependance_Nsk);

	TiXmlElement* node_Dependance_TextureEmb = new TiXmlElement("TextureEmb_Files");
	node_Dependance_TextureEmb->SetAttribute("count", hdr->numberFileInDependance_TextureEmb);
	name_offset = (uint32_t*)GetOffsetPtr(buf, hdr->offset_Dependance_TextureEmb);
	for (size_t i = 0; i < hdr->numberFileInDependance_TextureEmb; i++)
	{
		name = std::string((char *)GetOffsetPtr(buf, name_offset[i]));
		node = new TiXmlElement("File"); node->SetAttribute("index", i); node->SetAttribute("filename", name); node_Dependance_TextureEmb->LinkEndChild(node);
	}
	parent->LinkEndChild(node_Dependance_TextureEmb);

	TiXmlElement* node_Dependance_Emm = new TiXmlElement("Emm_Files");
	node_Dependance_Emm->SetAttribute("count", hdr->numberFileInDependance_Emm);
	name_offset = (uint32_t*)GetOffsetPtr(buf, hdr->offset_Dependance_Emm);
	for (size_t i = 0; i < hdr->numberFileInDependance_Emm; i++)
	{
		name = std::string((char *)GetOffsetPtr(buf, name_offset[i]));
		node = new TiXmlElement("File"); node->SetAttribute("index", i); node->SetAttribute("filename", name); node_Dependance_Emm->LinkEndChild(node);
	}
	parent->LinkEndChild(node_Dependance_Emm);

	TiXmlElement* node_Dependance_Ema = new TiXmlElement("Ema_Files");
	node_Dependance_Ema->SetAttribute("count", hdr->numberFileInDependance_Ema);
	name_offset = (uint32_t*)GetOffsetPtr(buf, hdr->offset_Dependance_Ema);
	for (size_t i = 0; i < hdr->numberFileInDependance_Ema; i++)
	{
		name = std::string((char *)GetOffsetPtr(buf, name_offset[i]));
		node = new TiXmlElement("File"); node->SetAttribute("index", i); node->SetAttribute("filename", name); node_Dependance_Ema->LinkEndChild(node);
	}
	parent->LinkEndChild(node_Dependance_Ema);
}








/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// wxHexEditor coloration ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*-------------------------------------------------------------------------------\
|                             save_Coloration		                             |
\-------------------------------------------------------------------------------*/
void FmpFile::save_Coloration(string filename, bool show_error)
{
	uint8_t *buf;
	size_t size;

	buf = ReadFile(filename, &size, show_error);
	if (!buf)
		return;

	TiXmlDocument *doc = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc->LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("wxHexEditor_XML_TAG");


	TiXmlElement* filename_node = new TiXmlElement("filename");
	EMO_BaseFile::WriteParamString(filename_node, "path", filename);



	write_Coloration(filename_node, buf, size);



	root->LinkEndChild(filename_node);
	doc->LinkEndChild(root);



	delete[] buf;

	doc->SaveFile(filename + ".tags");

	return;
}




/*-------------------------------------------------------------------------------\
|                             write_Coloration				                     |
\-------------------------------------------------------------------------------*/
void FmpFile::write_Coloration(TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	FMP_Header* hdr = (FMP_Header*)buf;
	if (size < sizeof(FMP_Header) || memcmp(hdr->signature, FMP_SIGNATURE, 4) != 0)
		return;


	std::vector<bool> listBytesAllreadyTagged;
	listBytesAllreadyTagged.resize(size, false);				//to check override of the same byte (overflow)


	size_t idTag = 0;
	size_t offset = 0;
	size_t incSection = 0;
	size_t incParam = 0;

	write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
	offset += 4 * sizeof(char);
	write_Coloration_Tag("version", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);
	offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_08", "uint32_t", "must be an even number", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_0C", "uint32_t", "Always 0", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);	offset += sizeof(uint32_t);
	write_Coloration_Tag("offset_Section0a", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);	offset += sizeof(uint32_t);
	write_Coloration_Tag("offset_Section0b", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("numberSection1", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("offset_Section1", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("numberSection2", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("offset_Section2", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("numberSection3", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("offset_Section3", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("numberSection4", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("offset_Section4", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("numberSection5", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("offset_Section5", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("numberFileInDependance_Nsk", "uint32_t", "=> always 0 in Qst et EVe, but not in BFten.map: 'Start Frame' 'Speed Rate', 'PLAY SOUND 3D' , etc ...", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("offset_Dependance_Nsk", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("numberFileInDependance_TextureEmb", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("offset_Dependance_TextureEmb", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("numberFileInDependance3", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("offset_Dependance_Emm", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("numberFileInDependance_Ema", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("offset_Dependance_Ema", "uint32_t", "", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_60", "uint32_t", "Always 0 (but game code references this)", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_64", "uint32_t", "Always 0 (but game code references this)", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_68", "uint32_t", "Always 0", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_6C", "uint32_t", "Always 0", offset, sizeof(uint32_t), "FMPHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	offset += 4 * sizeof(uint32_t);


	FMP_Section0a* section0a = (FMP_Section0a*)GetOffsetPtr(buf, hdr->offset_Section0a);
	offset = hdr->offset_Section0a;
	incSection++;
	incParam = 0;

	write_Coloration_Tag("width_X", "float", "1000.0 is most", offset, sizeof(uint32_t), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("width_Y", "float", "always as width_X", offset, sizeof(uint32_t), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("width_Z", "float", "always as width_X", offset, sizeof(uint32_t), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	
	write_Coloration_Tag("unk_3", "float", "always 0", offset, sizeof(uint32_t), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_4", "float", "always 0", offset, sizeof(uint32_t), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_5", "float", "always 0", offset, sizeof(uint32_t), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_6", "float", "always -9.8", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_7", "uint32_t", "always 4", offset, sizeof(uint32_t), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	
	write_Coloration_Tag("unk_8", "float", "0.577350 (most)  or 0.324488 (only for BFgen.map.xml)", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_9", "float", "-0.577350 (most) or -0.920775 (only for BFgen.map.xml)", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_10", "float", "0.577350 (most) or -0.216519 (only for BFgen.map.xml)", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	
	write_Coloration_Tag("unk_11", "float", "always 0.3", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_12", "float", "always as unk_11", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_13", "float", "always as unk_11", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_14", "float", "always 0.7", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_15", "float", "always as unk_14", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_16", "float", "always as unk_14", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_17", "float", "always 100.0", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_18", "float", "always 500.0", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	
	write_Coloration_Tag("unk_19", "float", "always 0 (until unk_26 there is the same block look like as unk_11 to 18)", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_20", "float", "always as unk_19", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_21", "float", "always  as unk_19", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_22", "float", "always 1", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_23", "float", "always as unk_22", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_24", "float", "always as unk_22", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_25", "float", "always 100.0", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_26", "float", "always 300.0", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);

	write_Coloration_Tag("unk_27", "float", "always 75.0", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_28", "float", "always 50.0", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);

	write_Coloration_Tag("unk_29", "float", "always 0.5", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_30", "float", "always as unk_29", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
	write_Coloration_Tag("unk_31", "float", "always as unk_29", offset, sizeof(float), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);

	write_Coloration_Tag("nearDistance", "uint32_t", "0.1 (most) or 1.0 ", offset, sizeof(uint32_t), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("farDistance", "uint32_t", " 1000.0 to 100000.0, 10000.0 (most)", offset, sizeof(uint32_t), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	
	write_Coloration_Tag("unk_34", "uint32_t", "0.785398 (most) or 0.698132 (only on 3 files)", offset, sizeof(uint32_t), "Section0a", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);



	incSection++;
	
	FMP_Section0b* section0b = (FMP_Section0b*)GetOffsetPtr(buf, hdr->offset_Section0b);
	offset = hdr->offset_Section0b;

	incParam = 0;
	write_Coloration_Tag("unk_0", "uint32_t", "0x7cb96118, not a float", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_1", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_2", "uint32_t", "0x172e5846", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_3", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_4", "uint32_t", "0x204cb00", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_5", "uint32_t", "0x84000000, look at flags", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_6", "uint32_t", "0x2e7cb0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_7", "uint32_t", "0x1000000", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_8", "uint32_t", "0x4000", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_9", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_10", "uint32_t", "0x40", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_11", "uint32_t", "0x932c0000", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_12", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_13", "uint32_t", "0x20800 flags", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_14", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_15", "uint32_t", "0x5cf96", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_16", "uint32_t", "0x800000, flags", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_17", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_18", "uint32_t", "0x9000 (most) or 0, flags", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_19", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_20", "uint32_t", "0x205cb", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_21", "uint32_t", "0x8000000, flags", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_22", "uint32_t", "0x0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_23", "uint16_t", "always 0x160", offset, sizeof(uint16_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("unk_23b", "uint16_t", "0x2 (most) or 0", offset, sizeof(uint16_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("unk_24", "uint32_t", "0x2800000 flags", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_25", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_26", "uint32_t", "0x400", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_27", "uint32_t", "0x1000000", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_28", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_29", "uint32_t", "0x100", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_30", "uint32_t", "0x200000", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_31", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_32", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_33", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_34", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_35", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_36", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_37", "uint32_t", "0x1000", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_38", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_39", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_40", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_41", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_42", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_43", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_44", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_45", "uint32_t", "0", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_46", "uint32_t", "0x22c0000", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_47", "uint32_t", "0x8a0000", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_48", "uint32_t", "0x140000", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_49", "uint32_t", "0x14000", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_50", "uint32_t", "0xa000", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_51", "uint32_t", "0x280", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_52", "uint32_t", "0x148", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_53", "uint32_t", "0x14000a8", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_54", "uint32_t", "0x2a0000", offset, sizeof(uint32_t), "Section0b", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	
	

	//start of list of name_offset.
	incSection++;
	offset = hdr->offset_Section0b + 0x100;
	uint32_t* name_offset;
	for (size_t i = 0; i < 51; i++)
	{
		name_offset = (uint32_t*)GetOffsetPtr(buf, offset);

		write_Coloration_Tag("name_offset", "uint32_t", "", offset, sizeof(uint32_t), "Section0b_nameOffsets", parent, idTag++, incSection, i, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		if(name_offset)
			write_Coloration_Tag("name", "string", "", *name_offset, 4 * sizeof(char), "Section0b_names", parent, idTag++, incSection + 1, i, listBytesAllreadyTagged, (size_t)-1, false);
	}





	FMP_Section1* section1 = (FMP_Section1*)GetOffsetPtr(buf, hdr->offset_Section1);
	offset = hdr->offset_Section1;
	incSection++;
	for (size_t i = 0; i < hdr->numberSection1; i++)
	{
		incParam = 0;
		write_Coloration_Tag("name_offset", "uint32_t", "TRIGGER_BROAD_PHASE, TRIGGER_NARROW_PHASE, TRIGGER_CONTACT_SOLVER, Default, or some '__X'", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_04", "uint32_t", "0 (most), 1, 2, 3. we have 0 for Default and '__X'", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_08", "float", "values are in [0, 1.0], 0.5 is the most", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		write_Coloration_Tag("unk_0C", "float", "same as unk_08", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		write_Coloration_Tag("unk_10", "float", "values are in [0, 1.0], 0.0 is the most", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		write_Coloration_Tag("unk_14", "float", "values are 999936.0 (most), 0.0 or 0.104279.", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		//associate name on last block.
		write_Coloration_Tag("name", "string", "", section1[i].name_offset, 4 * sizeof(char), "Section1", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}




	FMP_Section2* section2 = (FMP_Section2*)GetOffsetPtr(buf, hdr->offset_Section2);
	offset = hdr->offset_Section2;
	incSection++;
	for (size_t i = 0; i < hdr->numberSection2; i++)
	{
		incParam = 0;
		write_Coloration_Tag("name_offset", "uint32_t", "Default or some '__X'", offset, sizeof(uint32_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_04", "float", "0.0, 1.0 (most), 2.0 or 3.0. we feel 0 and 1, 2 and 3, 4 and 5, are together.", offset, sizeof(float), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		write_Coloration_Tag("unk_08", "float", "is always 1.0", offset, sizeof(float), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		write_Coloration_Tag("unk_0C", "float", "200.0 (most) or 300.0", offset, sizeof(float), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		write_Coloration_Tag("unk_10", "float", "100.0 (most), or 300.0. ", offset, sizeof(float), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		write_Coloration_Tag("unk_14", "float", "is in [0,2.0], 0.0 at most", offset, sizeof(float), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		write_Coloration_Tag("unk_18", "float", "is in [0, 10.0], 0.0 at most", offset, sizeof(float), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		//associate name on last block.
		write_Coloration_Tag("name", "string", "", section2[i].name_offset, 4 * sizeof(char), "Section2", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}



	FMP_Section3* section3 = (FMP_Section3*)GetOffsetPtr(buf, hdr->offset_Section3);
	offset = hdr->offset_Section3;
	incSection++;
	for (size_t i = 0; i < hdr->numberSection3; i++)
	{
		incParam = 0;
		write_Coloration_Tag("name_offset", "uint32_t", "Some interesting Names : ACT, EFCT,  Break_OFF, Break, , EXP_OFF, Fragments_0, Gate, InitPos, InstanceObject, Master, Master_G18_00, Palm, Quest_0100, BG, Other, AREA_BAMBOO, AREA_CITY, AREA_SHOP, ITEM_FIND_BAMBOO_FOREST, BATTLE, CLOSED_AREA, CLPC_POS02, CONFLICT, EMOTION, EVE, EVE_ITEM_FOOD, Env Sound, GAG, Group10, Main, MainScenario, NLBY_ACT40palm, NMC, NPC_CC, ObjIns_Palm, Respawn_Point, SubArea_Arena, TELEPORT, TEACHER, ", offset, sizeof(uint32_t), "Section3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("num_indexes", "uint32_t", "", offset, sizeof(uint32_t), "Section3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("indexes_offset", "uint32_t", "Points to table of 16 bits integers. Indexes are relative to Section4", offset, sizeof(uint32_t), "Section3", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

		size_t nbIndex = section3[i].num_indexes;
		for (size_t j = 0; j < nbIndex; j++)
		{
			incParam = 0;
			size_t offset_b = section3[i].indexes_offset + j * sizeof(int16_t);

			write_Coloration_Tag("index", "int16_t", "index into Section4. maximum found 1374", offset_b, sizeof(int16_t), "Section3_Indexes" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j);
		}

		//associate name on last block.
		write_Coloration_Tag("name", "string", "", section3[i].name_offset, 4 * sizeof(char), "Section3", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}







	FMP_Section4* section4 = (FMP_Section4*)GetOffsetPtr(buf, hdr->offset_Section4);
	FMP_Section4_Next* unk4_next;
	FMP_Section4_Next_b* unk4_next_b;
	FMP_Section4_Next_c* unk4_next_c;
	FMP_Section4_Next_d* unk4_next_d;
	FMP_Section4_Next_e* unk4_next_e;
	FMP_Section4_Next_f* unk4_next_f;

	FMP_Section4_Next3* unk4_next3;
	FMP_Section4_Next3_b* unk4_next3_b;

	FMP_Section4_Next2* unk4_next2;
	FMP_Section4_Next2_b* unk4_next2_b;
	
	FMP_Section4_Next4* unk4_next4;
	FMP_Section4_Next4_b* unk4_next4_b;
	FMP_Section4_Next4_c* unk4_next4_c;
	FMP_Section4_Next4_d* unk4_next4_d;
	FMP_Section4_Next4_e* unk4_next4_e;

	std::vector<uint32_t> listAllreadyUsedOffset_byUnk4Entrys;		//many Unk4Entry could use the same unk1_offset, unk2_offset, unk3_offset or unk4_offset. So it's break the tools witch look after rewriten tags. So we have to skip if allready see it.

	offset = hdr->offset_Section4;
	incSection += 2;
	
	for (size_t i = 0; i < hdr->numberSection4; i++)
	{
		incParam = 0;
		write_Coloration_Tag("name_offset", "uint32_t", "interresting names : 00_ivent_005, ACT06_Event, AI_MOVE_08, AREA_IN_FLBY03, A_Env_Sound_32_0, BACK_KH_00, BAQ_SCR_01_POS_02, CAMERA_POS_01, DB3, DM2_ULT_084, ENVTEX, EPISODE_03_02_POS_01, EVE_ITEM_FIND_93, GATE_to_BFlnd, GPUParticle, H49_meteoC00, MOVEPOS_NPC_EVE_CC_TRS_03, MST_VGT_03_POS_02, NPC_TEACHER_BRL, QUEST_0102_POS_01, RESPAWNPOS_01, SKY, SUN, SetShadowArea, SmallScene, TRESPASS_7, ULC_END_HIT_0, VS_INIT_POS_B_4, WARPPOS_06, ", offset, sizeof(uint32_t), "Section4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_04", "uint16_t", "0, 1 (most) or 0x10001 =>  sort of flag. for the right bit, may be 0 for instanced objects and particules. 1 for the rest (so could be a bool 'instanced')", offset, sizeof(uint16_t), "Section4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("unk_04b", "uint16_t", "0, 1 (most) or 0x10001 =>  sort of flag. for the right bit, may be 0 for instanced objects and particules. 1 for the rest (so could be a bool 'instanced')", offset, sizeof(uint16_t), "Section4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint16_t);
		write_Coloration_Tag("index_Section5", "int16_t", "0xffff for none.  with the targeted Section5, the number of Next is the number of Section5_Hitbox", offset, sizeof(int16_t), "Section4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(int16_t);

		write_Coloration_Tag("unk_0A", "int8_t", "0, 1 (most), 2, 3, 5, 7, 0x11, 0x13, 0x15, 0x2a, 0x49, 0x4b. may be 2 x uint4", offset, sizeof(int8_t), "Section4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(int8_t);
		write_Coloration_Tag("unk_0B", "int8_t", "always 0", offset, sizeof(int8_t), "Section4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(int8_t);
		write_Coloration_Tag("offset_Section4_Next", "uint32_t", " => "+ UnsignedToString(section4[i].offset_Section4_Next,true) +" To Section4_Next (0x50*n, n is get somehow from Section5) (Notice many Section4 could use the same offset)", offset, sizeof(uint32_t), "Section4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("offset_Section4_Next3", "uint32_t", " => " + UnsignedToString(section4[i].offset_Section4_Next3, true) + " To Section4_Next3 (Notice many Section4 could use the same offset)", offset, sizeof(uint32_t), "Section4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		
		write_Coloration_Tag("number_Section4_Next2", "uint32_t", "0 1 or 2, so it's could be a type", offset, sizeof(uint32_t), "Section4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("offset_Section4_Next2", "uint32_t", " => " + UnsignedToString(section4[i].offset_Section4_Next2, true) + " Ref to Section4_Next2 (Notice many Section4 could use the same offset)", offset, sizeof(uint32_t), "Section4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("offset_Section4_Next4", "uint32_t", " =>  " + UnsignedToString(section4[i].offset_Section4_Next4, true) + "Ref to Section4_Next4 (Notice many Section4 could use the same offset)", offset, sizeof(uint32_t), "Section4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

		write_Coloration_Tag("unk_20", "float", "0.0, 0.000002, 2.0 (most)", offset, sizeof(float), "Section4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		write_Coloration_Tag("transformMatrix4x3", "float[12]", "local matrix transform", offset, 12 * sizeof(float), "Section4", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += 12 * sizeof(float);


		size_t nb_Section4_Next = 0;					//nb_Section4_Next is the number of Hitbox on Section5 targeted.
		if (section4[i].index_Section5 < hdr->numberSection5)
		{
			FMP_Section5* section5 = (FMP_Section5*)GetOffsetPtr(buf, hdr->offset_Section5 + section4[i].index_Section5 * sizeof(FMP_Section5));
			nb_Section4_Next = section5->number_Section5_Hitbox;
		}







		bool isfound = true;
		if (section4[i].offset_Section4_Next)					//to remove Section4_Next allready saw
		{
			uint32_t offset_tmp = section4[i].offset_Section4_Next;
			isfound = false;
			size_t nbOffset = listAllreadyUsedOffset_byUnk4Entrys.size();
			for (size_t j = 0; j < nbOffset; j++)
			{
				if (listAllreadyUsedOffset_byUnk4Entrys.at(j) == offset_tmp)
				{
					isfound = true;
					break;
				}
			}
			if (!isfound)
				listAllreadyUsedOffset_byUnk4Entrys.push_back(offset_tmp);
		}
		
		if((section4[i].offset_Section4_Next)&&(!isfound))
		{
			for (size_t m = 0; m < nb_Section4_Next; m++)			//nb_Section4_Next is the number of Hitbox on Section5 targeted.
			{
				size_t offset_b = section4[i].offset_Section4_Next + m * sizeof(FMP_Section4_Next);
				unk4_next = (FMP_Section4_Next*)GetOffsetPtr(buf, offset_b);

				incParam = 0;

				write_Coloration_Tag("number_Section4_Next_b", "uint32_t", "[A] 0, 1 or 2 (may be a type)", offset_b, sizeof(uint32_t), "Section4_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("offset_Section4_Next_b", "uint32_t", "=> ref to Section4_Next_b", offset_b, sizeof(uint32_t), "Section4_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				
				write_Coloration_Tag("offset_Section4_Next_c", "uint32_t", "=> ref to Section4_Next_c", offset_b, sizeof(uint32_t), "Section4_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("offset_Section4_Next_c_2", "uint32_t", "another to Section4_Next_c", offset_b, sizeof(uint32_t), "Section4_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
				write_Coloration_Tag("offset_Section4_Next_d", "uint32_t", "=> Ref to Section4_Next_d", offset_b, sizeof(uint32_t), "Section4_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);

				write_Coloration_Tag("unk_2", "uint16_t", "only 0x0 (most), 0x1", offset_b, sizeof(uint16_t), "Section4_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint16_t);
				write_Coloration_Tag("unk_2b", "uint16_t", "only 0, 0xffff (most)", offset_b, sizeof(uint16_t), "Section4_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint16_t);
				write_Coloration_Tag("unk_4", "float", "[0.0 (most), 1000000.0] ", offset_b, sizeof(float), "Section4_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
				write_Coloration_Tag("unk_5", "float", "[0.0 (most), 3.0]", offset_b, sizeof(float), "Section4_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
				write_Coloration_Tag("transformMatrix4x3", "float[12]", "", offset_b, 12 * sizeof(float), "Section4_Next", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, i); offset_b += 12 * sizeof(float);

				if (unk4_next->offset_Section4_Next_b)
				{
					unk4_next_b = (FMP_Section4_Next_b*)GetOffsetPtr(buf, unk4_next->offset_Section4_Next_b);
					offset_b = unk4_next->offset_Section4_Next_b;
					for (size_t j = 0; j < unk4_next->number_Section4_Next_b; j++)
					{
						incParam = 0;
						write_Coloration_Tag("unk_0", "uint32_t", "few holes in [0, 0xe] go up to 0x1d. 5 at most. ", offset_b, sizeof(uint32_t), "Section4_Next_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_1", "uint32_t", "few holes in [0, 0xe] , 3 at most", offset_b, sizeof(uint32_t), "Section4_Next_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					}
				}

				if (unk4_next->offset_Section4_Next_c)
				{
					unk4_next_c = (FMP_Section4_Next_c*)GetOffsetPtr(buf, unk4_next->offset_Section4_Next_c);
					offset_b = unk4_next->offset_Section4_Next_c;
					{
						write_Coloration_Tag("unk_0", "uint16_t", "few holes in [0, 0xe] , 4 at most", offset_b, sizeof(uint16_t), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint16_t);
						write_Coloration_Tag("unk_0b", "uint16_t", " [0,7], plus 0xffff (most) for none", offset_b, sizeof(uint16_t), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint16_t);
						write_Coloration_Tag("unk_1", "uint16_t", "0 (most), 1, 3, 7, 0xff ", offset_b, sizeof(uint16_t), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint16_t);
						write_Coloration_Tag("unk_1b", "uint16_t", "0, 2 (most)", offset_b, sizeof(uint16_t), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint16_t);
						write_Coloration_Tag("unk_2", "uint32_t", "0, 2, 4, 5, 9, 0x19, 0xffffffff (most)", offset_b, sizeof(uint32_t), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						
						write_Coloration_Tag("unk_3", "float", "[0.031900, 24098666.0], 1.0 at most. lots of uniques values, and progress, like a animation", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_4", "float", "[-184.657913, 186.816162], 0.0 at most. could be some degrees ?", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_5", "float", "[-175.524277, 188.422974], 0.000000 (most) ", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_6", "float", "[-186.247391, 176.994080], 0.0 (most)", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_7", "float", "[-192.804337, 140.889252], 0.0 (most)", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_8", "float", "[-106.711281, 114.855453], 0.0 (most)", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_9", "float", "[-185.846497, 198.041656] 0.0 at most", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_10", "float", "[-279.467529, 259.390625], 0.0 (most). lots of values, look like for animation", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_11", "float", "[-260.461700, 122.487022], 0.0 (most). lots of values", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_12", "float", "[-463.797577, 387.593781], 0.0 (most). lots of values", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_13", "float", "[0.0, 841637952.0], 1.0 (most). lots of values near 0. there also have strange too high values", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("widthX", "float", "[0.0, 245005.984375]. lot of values. may be widthX of the Area witch the gate is targetable/usable (in case of gate)", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("widthY", "float", "[0.000035, 489994.437500]. lot of values", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("widthZ", "float", "[0.000000, 245005.984375]. lot of values", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("quaternionX", "float", "", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("quaternionY", "float", "", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("quaternionZ", "float", "", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("quaternionW", "float", "", offset_b, sizeof(float), "Section4_Next_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
					}
				}

				if (unk4_next->offset_Section4_Next_c_2)
				{
					unk4_next_c = (FMP_Section4_Next_c*)GetOffsetPtr(buf, unk4_next->offset_Section4_Next_c_2);
					offset_b = unk4_next->offset_Section4_Next_c_2;
					{
						write_Coloration_Tag("unk_0", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_1", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_2", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_3", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_4", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_5", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_6", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_7", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_8", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_9", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_10", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_11", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_12", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("unk_13", "float", "lot of high values ... strange", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("widthX", "float", "may be widthX of the Area witch the gate is targetable/usable (in case of gate)", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("widthY", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("widthZ", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("quaternionX", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("quaternionY", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("quaternionZ", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
						write_Coloration_Tag("quaternionW", "float", "", offset_b, sizeof(float), "Section4_Next_c_2[" + std::to_string(i) + "]", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);
					}
				}
				

				if (unk4_next->offset_Section4_Next_d)
				{
					unk4_next_d = (FMP_Section4_Next_d*)GetOffsetPtr(buf, unk4_next->offset_Section4_Next_d);
					offset_b = unk4_next->offset_Section4_Next_d;
					{
						write_Coloration_Tag("unk_0", "uint32_t", "always 0", offset_b, sizeof(uint32_t), "Section4_Next_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_1a", "uint8_t", "always 2", offset_b, sizeof(uint8_t), "Section4_Next_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint8_t);
						write_Coloration_Tag("unk_1b", "uint8_t", "only 0 (most), 1, 3, 4, 8, 0xc. sound like flags", offset_b, sizeof(uint8_t), "Section4_Next_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint8_t);
						write_Coloration_Tag("unk_1c", "uint8_t", "0, 2 (most)", offset_b, sizeof(uint8_t), "Section4_Next_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint8_t);
						write_Coloration_Tag("unk_1d", "uint8_t", "always 0", offset_b, sizeof(uint8_t), "Section4_Next_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint8_t);
						write_Coloration_Tag("number_Section4_Next_e", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("offset_Section4_Next_e", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);


						if (unk4_next_d->offset_Section4_Next_e)
						{
							unk4_next_e = (FMP_Section4_Next_e*)GetOffsetPtr(buf, unk4_next_d->offset_Section4_Next_e);
							offset_b = unk4_next_d->offset_Section4_Next_e;
							for (size_t j = 0; j < unk4_next_d->number_Section4_Next_e; j++)
							{
								incParam = 0;

								write_Coloration_Tag("name_offset", "uint32_t", "Chase Start, Henshin Impossible, MOTION SET, PLAY EFFECT, PLAY SOUND 3D, [NO PARAMETER]. ", offset_b, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
								write_Coloration_Tag("unk_1a", "uint8_t", "only 0 (most) or 0x2", offset_b, sizeof(uint8_t), "Section4_Next_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint8_t);
								write_Coloration_Tag("unk_1b", "uint8_t", "only 0, 1, 3 (most), 4", offset_b, sizeof(uint8_t), "Section4_Next_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint8_t);
								write_Coloration_Tag("unk_1c", "uint8_t", "only 0, 0x1 (most), 0x7, 0xc, 0xf", offset_b, sizeof(uint8_t), "Section4_Next_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint8_t);
								write_Coloration_Tag("unk_1d", "uint8_t", "only 0 (most), 7, 4, 0xa, 8", offset_b, sizeof(uint8_t), "Section4_Next_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint8_t);
								write_Coloration_Tag("number_Section4_Next_f", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
								write_Coloration_Tag("offset_Section4_Next_f", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);

								write_Coloration_Tag("name", "string", "", unk4_next_e[j].name_offset, 4 * sizeof(char), "Section4_Next3_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 5, i, listBytesAllreadyTagged, j, false);

								if (unk4_next_e[j].offset_Section4_Next_f)
								{
									unk4_next_f = (FMP_Section4_Next_f*)GetOffsetPtr(buf, unk4_next_e[j].offset_Section4_Next_f);
									size_t offset_c = unk4_next_e[j].offset_Section4_Next_f;
									for (size_t k = 0; k < unk4_next_e[j].number_Section4_Next_f; k++)
									{
										incParam = 0;

										write_Coloration_Tag("name_offset", "uint32_t", "Category ID, Effect ID, Enable Dir, Group ID, Lerp Time, Motion ID, Motion Index, Movable, Next ID, Object Name, Radius Far, Radius Near, Scale, Sound ID, Speed Rate, Start Frame, Volume", offset_c, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); offset_c += sizeof(uint32_t);
										write_Coloration_Tag("type_Data", "uint32_t", "0 = Bool, 1 = int (most), 2 = float, 3 = Direction/Vector, 4 = Position, 6 = string , 8 = uint", offset_c, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); offset_c += sizeof(uint32_t);
										write_Coloration_Tag("offset_Data", "uint32_t", "depend of typeData", offset_c, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); offset_c += sizeof(uint32_t);

										write_Coloration_Tag("name", "string", "", unk4_next_f[k].name_offset, 4 * sizeof(char), "Section4_Next3_b[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, i, listBytesAllreadyTagged, k, false);

										if (unk4_next_f[k].offset_Data)
										{
											switch (unk4_next_f[k].type_Data)					//notice all the file only have 0,1,2,3,4, 6, 8 
											{
											case 0: write_Coloration_Tag("Data_Bool", "uint32_t", "for parameters", unk4_next_f[k].offset_Data, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); break;
											case 1: write_Coloration_Tag("Data_Int", "uint32_t", "for parameters", unk4_next_f[k].offset_Data, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); break;
											case 2: write_Coloration_Tag("Data_Float", "uint32_t", "for parameters", unk4_next_f[k].offset_Data, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); break;
											case 3: write_Coloration_Tag("Data_Direction_vector", "uint32_t", "for parameters", unk4_next_f[k].offset_Data, 4 * sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); break;
											case 4: write_Coloration_Tag("Data_Position_vector", "uint32_t", "for parameters", unk4_next_f[k].offset_Data, 4 * sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); break;
											case 6: write_Coloration_Tag("Data_String", "string", "@self", unk4_next_f[k].offset_Data, 4 * sizeof(char), "Section4_Next3_b[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, i, listBytesAllreadyTagged, k, false); break;
											case 8: write_Coloration_Tag("Data_uint32", "uint32_t", "for parameters", unk4_next_f[k].offset_Data, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}







		isfound = true;
		if (section4[i].offset_Section4_Next3)					//to remove Section4_Next allready saw
		{
			uint32_t offset_tmp = section4[i].offset_Section4_Next3;
			isfound = false;
			size_t nbOffset = listAllreadyUsedOffset_byUnk4Entrys.size();
			for (size_t j = 0; j < nbOffset; j++)
			{
				if (listAllreadyUsedOffset_byUnk4Entrys.at(j) == offset_tmp)
				{
					isfound = true;
					break;
				}
			}
			if (!isfound)
				listAllreadyUsedOffset_byUnk4Entrys.push_back(offset_tmp);
		}

		if ((section4[i].offset_Section4_Next3)&&(!isfound))
		{
			unk4_next3 = (FMP_Section4_Next3*)GetOffsetPtr(buf, section4[i].offset_Section4_Next3);
			size_t offset_b = section4[i].offset_Section4_Next3;
			incParam = 0;

			write_Coloration_Tag("unk_0", "uint32_t", "always 0", offset_b, sizeof(uint32_t), "Section4_Next3", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("unk_1a", "uint8_t", "always 2", offset_b, sizeof(uint8_t), "Section4_Next3", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint8_t);
			write_Coloration_Tag("unk_1b", "uint8_t", "only 0 (most), 0x10, 0x20, 0x30, 0x40, 0x70", offset_b, sizeof(uint8_t), "Section4_Next3", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint8_t);
			write_Coloration_Tag("unk_1c", "uint8_t", "always 0", offset_b, sizeof(uint8_t), "Section4_Next3", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint8_t);
			write_Coloration_Tag("unk_1d", "uint8_t", "only 0 (most) or 0x80", offset_b, sizeof(uint8_t), "Section4_Next3", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint8_t);
			write_Coloration_Tag("number_Section4_Next3_b", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next3", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("offset_Section4_Next3_b", "uint32_t", "=> ref to Section4_Next3_b", offset_b, sizeof(uint32_t), "Section4_Next3", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);

			if (unk4_next3->offset_Section4_Next3_b)
			{
				unk4_next3_b = (FMP_Section4_Next3_b*)GetOffsetPtr(buf, unk4_next3->offset_Section4_Next3_b);
				offset_b = unk4_next3->offset_Section4_Next3_b;
				for (size_t j = 0; j < unk4_next3->number_Section4_Next3_b; j++)
				{
					incParam = 0;

					write_Coloration_Tag("name_offset", "uint32_t", "interresting names : ADD SOUND, Add Plane Sound, CHANGE AREA, Camera, Change Env Sound, Change Interaction, EVENT CALL, Henshin Impossible,  LIMIT MOVE INVISIBLE, HideObject, HitCapsule, Lock Camera, LockonEnable, MOTION SET, PLAY EFFECT, PLAY SOUND 3D, Play Massive Particles, SET BREAK PARTS INVISIBLE, SET FLAG USE CMN BREAK PARTS EFFECT, SET LCT, SET MOTION, SET SHADOW AREA, SET SUN, Small Scene, [NO PARAMETER]. it's look like orders, or a name of what it will do.", offset_b, sizeof(uint32_t), "Section4_Next3_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_1a", "uint8_t", "0, 1, 2, 3 (most), 4", offset_b, sizeof(uint8_t), "Section4_Next3_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint8_t);
					write_Coloration_Tag("unk_1b", "uint8_t", "always 0", offset_b, sizeof(uint8_t), "Section4_Next3_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint8_t);
					write_Coloration_Tag("unk_1c", "uint8_t", "seam to be 2 x uint4 , with one is flags: 0, 4, 5, 7, 0x10, 0x64, 0x65, 0x66, 0x67, 0x68, 0x6a, 0x6b, 0x6c, 0x6e, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0xc8, 0xca, 0xcc, 0xcd, 0xd1, 0xd2, 0xd3, 0xf8, 0xfb", offset_b, sizeof(uint8_t), "Section4_Next3_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint8_t);
					write_Coloration_Tag("unk_1d", "uint8_t", "0 (most) or 0xff", offset_b, sizeof(uint8_t), "Section4_Next3_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint8_t);
					write_Coloration_Tag("number_Section4_Next_f", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next3_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("offset_Section4_Next_f", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next3_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);

					write_Coloration_Tag("name", "string", "", unk4_next3_b[j].name_offset, 4 * sizeof(char), "Section4_Next3_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 4, i, listBytesAllreadyTagged, j, false);

					if (unk4_next3_b[j].offset_Section4_Next_f)
					{
						unk4_next_f = (FMP_Section4_Next_f*)GetOffsetPtr(buf, unk4_next3_b[j].offset_Section4_Next_f);
						size_t offset_c = unk4_next3_b[j].offset_Section4_Next_f;
						for (size_t k = 0; k < unk4_next3_b[j].number_Section4_Next_f; k++)
						{
							incParam = 0;

							write_Coloration_Tag("name_offset", "uint32_t", "interresting names: Camera Object, CapsuleHeight, CapsuleRadius, Category ID, Effect ID, Effect Wait, FovY, Group ID, Invisible Time, LCT Node Index, Lerp Time, Motion ID, Motion Index, Movie Name, ObjectName, Offset Y, Position, Plane Type, Preset Name, Sound ID, Speed Rate, Start Frame, Target Object Name, Type, Volume, Wait Frame, flag, time", offset_c, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); offset_c += sizeof(uint32_t);
							write_Coloration_Tag("type_Data", "uint32_t", "0 = Bool, 1 = int (most), 2 = float, 3 = Direction/Vector, 4 = Position, 6 = string , 8 = uint", offset_c, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); offset_c += sizeof(uint32_t);
							write_Coloration_Tag("offset_Data", "uint32_t", "depend of the type_Data", offset_c, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); offset_c += sizeof(uint32_t);

							write_Coloration_Tag("name", "string", "", unk4_next_f[k].name_offset, 4 * sizeof(char), "Section4_Next3_b[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, i, listBytesAllreadyTagged, k, false);

							if (unk4_next_f[k].offset_Data)
							{
								switch (unk4_next_f[k].type_Data)					//notice all the file only have 0,1,2,3,4, 6, 8 
								{
								case 0: write_Coloration_Tag("Data_Bool", "uint32_t", "for parameters", unk4_next_f[k].offset_Data, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); break;
								case 1: write_Coloration_Tag("Data_Int", "uint32_t", "for parameters", unk4_next_f[k].offset_Data, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); break;
								case 2: write_Coloration_Tag("Data_Float", "uint32_t", "for parameters", unk4_next_f[k].offset_Data, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); break;
								case 3: write_Coloration_Tag("Data_Direction_vector", "uint32_t", "for parameters", unk4_next_f[k].offset_Data, 4 * sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); break;
								case 4: write_Coloration_Tag("Data_Position_vector", "uint32_t", "for parameters", unk4_next_f[k].offset_Data, 4 * sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); break;
								case 6: write_Coloration_Tag("Data_String", "string", "interresting names : @self[, Snowfall, COMa10_006, CAMERA_POS_01, ", unk4_next_f[k].offset_Data, 4 * sizeof(char), "Section4_Next3_b[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, i, listBytesAllreadyTagged, k, false); break;
								case 8: write_Coloration_Tag("Data_uint32", "uint32_t", "for parameters", unk4_next_f[k].offset_Data, sizeof(uint32_t), "Section4_Next_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); break;
								}
							}
						}
					}
				}
			}
		}









		if (section4[i].offset_Section4_Next2)
		{
			for(size_t k=0;k<section4[i].number_Section4_Next2;k++)
			{
				size_t start_Section4_Next2 = section4[i].offset_Section4_Next2 + k * sizeof(FMP_Section4_Next2);


				isfound = false;
				size_t nbOffset = listAllreadyUsedOffset_byUnk4Entrys.size();
				for (size_t j = 0; j < nbOffset; j++)
				{
					if (listAllreadyUsedOffset_byUnk4Entrys.at(j) == start_Section4_Next2)
					{
						isfound = true;
						break;
					}
				}
				if (!isfound)
					listAllreadyUsedOffset_byUnk4Entrys.push_back(start_Section4_Next2);


				if (!isfound)
				{
					unk4_next2 = (FMP_Section4_Next2*)GetOffsetPtr(buf, start_Section4_Next2);
					size_t offset_b = start_Section4_Next2;
					incParam = 0;

					write_Coloration_Tag("offset_Section4_Next2_b", "uint32_t", "=> ref to Section4_Next2_b", offset_b, sizeof(uint32_t), "Section4_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("unk_0", "uint32_t", "few holes in [0 (most), 0x11], go up to 0x74", offset_b, sizeof(uint32_t), "Section4_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("transformMatrix4x3", "float[12]", "", offset_b, 12 * sizeof(float), "Section4_Next2", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, i); offset_b += 12 * sizeof(float);

					if (unk4_next2[0].offset_Section4_Next2_b)
					{
						unk4_next2_b = (FMP_Section4_Next2_b*)GetOffsetPtr(buf, unk4_next2[0].offset_Section4_Next2_b);
						offset_b = unk4_next2[0].offset_Section4_Next2_b;
						incParam = 0;

						write_Coloration_Tag("name_offset", "uint32_t", "interresting names : ACT02_gate, BFiceh76, BFnmb_RockA, BFseiENV,  BFsky00, COMa12, DB5, ENVTEX, FragmentsF02_rock00, New Model, REFTEX, TreeA, TreeSet, sky_Model", offset_b, sizeof(uint32_t), "Section4_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_0", "uint32_t", "0 (most), 4", offset_b, sizeof(uint32_t), "Section4_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("number_Lod", "uint32_t", "only 1 (most), 2, 3 or 4 (3 and 4 only on NLBY). => type witch influence unk_2, unk_4 unk_11 is a ref offset (if ==2).", offset_b, sizeof(uint32_t), "Section4_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("indexNsk", "uint32_t", "0xffffffff for none => if number_Lod==2, Ref to a series of index", offset_b, sizeof(uint32_t), "Section4_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("indexTextureEmb", "uint32_t", "0xffffffff for none.", offset_b, sizeof(uint32_t), "Section4_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("indexEmm", "uint32_t", "0xffffffff for none. => if number_Lod==2, Ref to a series of index", offset_b, sizeof(uint32_t), "Section4_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_5", "uint32_t", "always 0xffffffff", offset_b, sizeof(uint32_t), "Section4_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_6", "uint32_t", "always 0xffffffff", offset_b, sizeof(uint32_t), "Section4_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("indexEma", "uint32_t", "0xffffffff for none (most) ", offset_b, sizeof(uint32_t), "Section4_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_8", "uint32_t", "only 2, 3 or 0xffffffff (none and most). sound like another index to something, may be twice animation. Todo check", offset_b, sizeof(uint32_t), "Section4_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_9", "uint32_t", "always 60.0", offset_b, sizeof(uint32_t), "Section4_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk_10", "uint32_t", "always 60.0", offset_b, sizeof(uint32_t), "Section4_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("lodDistance", "float", "if number_Lod=1 is a float, else it's ref to s aseries of floats . only values are 70.0, 100.0, 500.0, ...., 100000.0 sound about far distance again, strange.", offset_b, sizeof(float), "Section4_Next2_b", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(float);

						//associate name on last block.
						write_Coloration_Tag("name", "string", "", unk4_next2_b[0].name_offset, 4 * sizeof(char), "Section4_Next2_b", parent, idTag++, incSection + 7, i, listBytesAllreadyTagged, i, false);

						if (unk4_next2_b[0].number_Lod != 1)
						{
							if (unk4_next2_b[0].indexNsk)
							{
								offset_b = unk4_next2_b[0].indexNsk;

								incParam = 0;
								for (size_t k = 0; k < unk4_next2_b[0].number_Lod; k++)
								{
									write_Coloration_Tag("index", "uint32_t", string("index to Nsk (LOD_+") + std::to_string(k) + ")", offset_b, sizeof(uint32_t), "Section4_Next2_b_ext", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); offset_b += sizeof(uint32_t);
								}
							}

							if (unk4_next2_b[0].indexEmm)
							{
								offset_b = unk4_next2_b[0].indexEmm;

								incParam = 0;
								for (size_t k = 0; k < unk4_next2_b[0].number_Lod; k++)
								{
									write_Coloration_Tag("index", "uint32_t", string("index to Emm (LOD_+") + std::to_string(k) +")", offset_b, sizeof(uint32_t), "Section4_Next2_b_ext", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); offset_b += sizeof(uint32_t);
								}
							}

							if (unk4_next2_b[0].lodDistance)
							{
								offset_b = *((uint32_t*)&(unk4_next2_b[0].lodDistance));

								incParam = 0;
								for (size_t k = 0; k < unk4_next2_b[0].number_Lod; k++)
								{
									write_Coloration_Tag("lodDistance", "float", string("LodDistance to (LOD_+") + std::to_string(k) + ")", offset_b, sizeof(float), "Section4_Next2_b_ext", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, k); offset_b += sizeof(float);
								}
							}
						}
					}
				}
			}
		}









		isfound = true;
		if (section4[i].offset_Section4_Next4)					//to remove Section4_Next allready saw
		{
			uint32_t offset_tmp = section4[i].offset_Section4_Next4;
			isfound = false;
			size_t nbOffset = listAllreadyUsedOffset_byUnk4Entrys.size();
			for (size_t j = 0; j < nbOffset; j++)
			{
				if (listAllreadyUsedOffset_byUnk4Entrys.at(j) == offset_tmp)
				{
					isfound = true;
					break;
				}
			}
			if (!isfound)
				listAllreadyUsedOffset_byUnk4Entrys.push_back(offset_tmp);
		}

		if ((section4[i].offset_Section4_Next4)&&(!isfound))
		{
			unk4_next4 = (FMP_Section4_Next4*)GetOffsetPtr(buf, section4[i].offset_Section4_Next4);
			size_t offset_b = section4[i].offset_Section4_Next4;
			incParam = 0;

			write_Coloration_Tag("number_Section4_Next4_c", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next4", parent, idTag++, incSection + 8, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("number_Section4_Next4_b", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next4", parent, idTag++, incSection + 8, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);

			write_Coloration_Tag("offset_Section4_Next4_b", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next4", parent, idTag++, incSection + 8, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("offset_Section4_Next4_d", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next4", parent, idTag++, incSection + 8, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			write_Coloration_Tag("offset_Section4_Next4_c", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next4", parent, idTag++, incSection + 8, incParam++, listBytesAllreadyTagged, i); offset_b += sizeof(uint32_t);
			

			if (unk4_next4->offset_Section4_Next4_b)
			{
				unk4_next4_b = (FMP_Section4_Next4_b*)GetOffsetPtr(buf, unk4_next4->offset_Section4_Next4_b);
				offset_b = unk4_next4->offset_Section4_Next4_b;
				for (size_t j = 0; j < unk4_next4->number_Section4_Next4_b; j++)
				{
					incParam = 0;

					write_Coloration_Tag("unk_0", "float", "[-276.801208, 523.696960]", offset_b, sizeof(float), "Section4_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("unk_1", "float", "[-60.752632, 293.368286]", offset_b, sizeof(float), "Section4_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("unk_2", "float", "[-425.796051, 286.322693]", offset_b, sizeof(float), "Section4_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("unk_3", "float", "[0.309734, 10034.649414]", offset_b, sizeof(float), "Section4_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("unk_4", "float", "[-9816.500000, 509.546631]", offset_b, sizeof(float), "Section4_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("unk_5", "float", "[-9998.799805, 272.971954]", offset_b, sizeof(float), "Section4_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("unk_6", "float", "[-10251.099609, 283.456329]", offset_b, sizeof(float), "Section4_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("unk_7", "float", "[-266.516388, 10218.0]", offset_b, sizeof(float), "Section4_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("unk_8", "float", "[-46.602291, 10001.200195]", offset_b, sizeof(float), "Section4_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("unk_9", "float", "[-411.645721, 9809.0]", offset_b, sizeof(float), "Section4_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("unk_10", "uint32_t", "few hole in [1(most), 0x2f] go up to 0xef", offset_b, sizeof(uint32_t), "Section4_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
					write_Coloration_Tag("indexToNext4_c_Transform", "uint32_t", "", offset_b, sizeof(uint32_t), "Section4_Next4_b[" + std::to_string(i) + "]", parent, idTag++, incSection + 9, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
				}
			}

			if (unk4_next4->offset_Section4_Next4_c)
			{
				unk4_next4_c = (FMP_Section4_Next4_c*)GetOffsetPtr(buf, unk4_next4->offset_Section4_Next4_c);
				offset_b = unk4_next4->offset_Section4_Next4_c;
				for (size_t j = 0; j < unk4_next4->number_Section4_Next4_c; j++)
				{
					incParam = 0;
					write_Coloration_Tag("positionX", "float", "lots of values . look like for a animation", offset_b, sizeof(float), "Section4_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("positionY", "float", "", offset_b, sizeof(float), "Section4_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("positionZ", "float", "", offset_b, sizeof(float), "Section4_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("rotationX", "float", "radians, [-pi, pi], so could be Yaw", offset_b, sizeof(float), "Section4_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("rotationY", "float", "radians, [-pi/2, pi/2] so could be Pitch", offset_b, sizeof(float), "Section4_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("rotationZ", "float", "radiansn, [-pi, pi] so could be Roll", offset_b, sizeof(float), "Section4_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("scaleX", "float", "", offset_b, sizeof(float), "Section4_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("scaleY", "float", "", offset_b, sizeof(float), "Section4_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
					write_Coloration_Tag("scaleZ", "float", "", offset_b, sizeof(float), "Section4_Next4_c[" + std::to_string(i) + "]", parent, idTag++, incSection + 10, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
				}
			}

			if (unk4_next4->offset_Section4_Next4_d)
			{
				
				std::vector<uint32_t> adressesToCheck;
				adressesToCheck.push_back(unk4_next4->offset_Section4_Next4_d);

				
				for (size_t j = 0; j < adressesToCheck.size(); j++)		//the list will grow in process
				{
					offset_b = adressesToCheck.at(j);
					unk4_next4_d = (FMP_Section4_Next4_d*)GetOffsetPtr(buf, offset_b);
					incParam = 0;

					write_Coloration_Tag("type", "uint8_t", "only 0, 1 (most)", offset_b, sizeof(uint8_t), "Section4_Next4_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint8_t);
					write_Coloration_Tag("typeb", "uint8_t", "only 0 (most), 0x20, 0x30 or 0x60 (if type==1, typeb is always 0). if 2 -> followed by Section4_Next4_e else is Indexes, Todo check other values", offset_b, sizeof(uint8_t), "Section4_Next4_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint8_t);
					write_Coloration_Tag("number_Index", "uint16_t", "number of followed Index (with padding to match uint32_t) => could be the same number as Next4_b ? todo check. there is also another strange things, the type is on analyzed values, So may be it's the 'last index', instead of 'number'. Todo check", offset_b, sizeof(uint16_t), "Section4_Next4_d[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint16_t);


					if (unk4_next4_d->type == 1)				// isLastChild in hierarchy ? (== no child, just reference of something)
					{	
						for (size_t k = 0; k < unk4_next4_d->number_Index; k++)
						{
							write_Coloration_Tag("Index", "uint16_t", "all value in [0, 0x50] with few holes until 0x382, and after it look like offset, or float16, or may be couple shape a float32. there is also a strange/specific 0x2000,0x3000, or 0x6000 that the bigeining of a another Next4_d. 1 at most. (TODO check 0x1494[48,0,7,42,9,2,2,16,2,2] ).  Indexes with padding to Uint32_t", offset_b, sizeof(uint16_t), "Section4_Next4_d[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, k); offset_b += sizeof(uint16_t);
						}
					}else{

						unk4_next4_e = (FMP_Section4_Next4_e*)GetOffsetPtr(buf, offset_b);
						incParam = 0;

						write_Coloration_Tag("unk_0", "float", "[-130.691345, -35.299774], -117.454727 (~most)", offset_b, sizeof(float), "Section4_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
						write_Coloration_Tag("unk_1", "float", "[11.680498, 14.232130], 13.294364 (~most)", offset_b, sizeof(float), "Section4_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);
						write_Coloration_Tag("unk_2", "float", "[-28.578398, 53.643196], -15.570686 (~most)", offset_b, sizeof(float), "Section4_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(float);

						write_Coloration_Tag("unk0_offset", "uint32_t", "for recursive Next4_d", offset_b, sizeof(uint32_t), "Section4_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk1_offset", "uint32_t", "for recursive Next4_d", offset_b, sizeof(uint32_t), "Section4_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk2_offset", "uint32_t", "for recursive Next4_d", offset_b, sizeof(uint32_t), "Section4_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk3_offset", "uint32_t", "for recursive Next4_d", offset_b, sizeof(uint32_t), "Section4_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk4_offset", "uint32_t", "for recursive Next4_d", offset_b, sizeof(uint32_t), "Section4_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk5_offset", "uint32_t", "for recursive Next4_d", offset_b, sizeof(uint32_t), "Section4_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk6_offset", "uint32_t", "for recursive Next4_d", offset_b, sizeof(uint32_t), "Section4_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);
						write_Coloration_Tag("unk7_offset", "uint32_t", "for recursive Next4_d", offset_b, sizeof(uint32_t), "Section4_Next4_e[" + std::to_string(i) + "]", parent, idTag++, incSection + 11, incParam++, listBytesAllreadyTagged, j); offset_b += sizeof(uint32_t);

						
						if (unk4_next4_e->unk0_offset) adressesToCheck.push_back(unk4_next4_e->unk0_offset);
						if (unk4_next4_e->unk1_offset) adressesToCheck.push_back(unk4_next4_e->unk1_offset);
						if (unk4_next4_e->unk2_offset) adressesToCheck.push_back(unk4_next4_e->unk2_offset);
						if (unk4_next4_e->unk3_offset) adressesToCheck.push_back(unk4_next4_e->unk3_offset);
						if (unk4_next4_e->unk4_offset) adressesToCheck.push_back(unk4_next4_e->unk4_offset);
						if (unk4_next4_e->unk5_offset) adressesToCheck.push_back(unk4_next4_e->unk5_offset);
						if (unk4_next4_e->unk6_offset) adressesToCheck.push_back(unk4_next4_e->unk6_offset);
						if (unk4_next4_e->unk7_offset) adressesToCheck.push_back(unk4_next4_e->unk7_offset);
					}
				}
				


			}

		}



		//associate name on last block.
		write_Coloration_Tag("name", "string", "", section4[i].name_offset, 4 * sizeof(char), "Section4", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}







	FMP_Section5* section5 = (FMP_Section5*)GetOffsetPtr(buf, hdr->offset_Section5);
	size_t offset_tmp = 0;
	incSection += 12;

	for (size_t i = 0; i < hdr->numberSection5; i++)
	{
		size_t start_Section5 = hdr->offset_Section5 + i * sizeof(FMP_Section5);
		offset = start_Section5;
		
		uint32_t* nameOffset_tmp = (uint32_t*)GetOffsetPtr(buf, offset);
		if (*nameOffset_tmp >= (uint32_t)size)
		{
			printf("Error : overflow on HitBox name. skipped all next hitbox from Unk5.\n");
			LibXenoverse::notifyError();
			break;						//we can't know witch size in bytes.
		}
		string name_Section5 = std::string((char *)GetOffsetPtr(buf, *nameOffset_tmp));



		incParam = 0;
		write_Coloration_Tag("name_offset", "uint32_t", "["+ name_Section5+ "] sound like Section4's names, witch do the link between. notice there is more names on Section4. interresting names: 00_ivent_005, ACT04_06, AI_MOVE_03, A_Env_Sound_32_1,  BFice_StoneA_col, BFund04_smoke04,  CAMERA_AREA_IN_01, COMa10_005, C_Env_Sound_33_3, DB4, DM2_ULT_088, EFFECT_WaterFall03, ENVTEX, GATE_to_BFlnc, GROUP09_WATCH_POINT, Hologram, LBYa23_20, LOCAL_VS_INIT_POS_1P, NPC_TALK_name_4051, QUEST_0101_POS_03, RESPAWNPOS_04, SKY, SUN, SetShadowArea, WARPPOS_05, sound_ENV  ", offset, sizeof(uint32_t), "Section5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("number_Section5_Hitbox", "uint32_t", "", offset, sizeof(uint32_t), "Section5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("offset_Section5_Hitbox", "uint32_t", "", offset, sizeof(uint32_t), "Section5", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);


		

		offset_tmp = section5[i].offset_Section5_Hitbox;
		if (offset_tmp)
		{
			size_t start_Section5_HitBox_list = offset_tmp;

			size_t incLastOffset = 0;
			for (size_t j = 0; j < section5[i].number_Section5_Hitbox; j++)
			{
				bool isOldVersion = ((hdr->version & 0xFFF00) == 0x20800);
				
				size_t start_Section5_HitBox = start_Section5_HitBox_list + j * ( (!isOldVersion) ? sizeof(FMP_Section5_Hitbox) : sizeof(FMP_Section5_Hitbox_Old_V0x0x208XX) );
			

				FMP_Section5_Hitbox* section5_Hitbox = 0;

				if (!isOldVersion)
				{
					section5_Hitbox = (FMP_Section5_Hitbox*)GetOffsetPtr(buf, start_Section5_HitBox);
				}else{
					FMP_Section5_Hitbox_Old_V0x0x208XX* section5_Hitbox_Old = (FMP_Section5_Hitbox_Old_V0x0x208XX*)GetOffsetPtr(buf, start_Section5_HitBox);
					
					section5_Hitbox = new FMP_Section5_Hitbox();

					section5_Hitbox->name_offset = section5_Hitbox_Old->name_offset;
					section5_Hitbox->child = section5_Hitbox_Old->child;
					section5_Hitbox->unk_a0 = section5_Hitbox_Old->unk_a0;
					section5_Hitbox->sibling = section5_Hitbox_Old->sibling;
					section5_Hitbox->parent = section5_Hitbox_Old->parent;
					section5_Hitbox->number_Section5_Hitbox_b = section5_Hitbox_Old->number_Section5_Hitbox_b;
					section5_Hitbox->offset_Section5_Hitbox_b = section5_Hitbox_Old->offset_Section5_Hitbox_b;
					section5_Hitbox->offset_Section5_Hitbox_f = 0;
					section5_Hitbox->number_Section5_Hitbox_e = section5_Hitbox_Old->number_Section5_Hitbox_e;
					section5_Hitbox->offset_Section5_Hitbox_e = section5_Hitbox_Old->offset_Section5_Hitbox_e;
					section5_Hitbox->number_Indexes = section5_Hitbox_Old->number_Indexes;
					section5_Hitbox->offset_Indexes = section5_Hitbox_Old->offset_Indexes;
				}
				

				
				offset = start_Section5_HitBox;

				incParam = 0;
				uint32_t* nameOffset_tmp = (uint32_t*)GetOffsetPtr(buf, offset);
				if ((*nameOffset_tmp >= (uint32_t)size) || (*nameOffset_tmp <= offset))
				{
					printf("Error : overflow on HitBox name [offset=%s]. skipped all next hitbox from Section5.\n", UnsignedToString(offset, true).c_str());
					LibXenoverse::notifyError();
					break;						//we can't know witch size in bytes.
				}

				name = std::string((char *)GetOffsetPtr(buf, *nameOffset_tmp));
				write_Coloration_Tag("name", "string", "interresting names : 00_ivent_010, ACT19_04, AI_MOVE_03, A_Env_Sound_32_1, BFice_StoneA_col, QUEST_1001_POS_00", *nameOffset_tmp, 4 * sizeof(char), "Section5_Box", parent, idTag++, incSection + 1, j, listBytesAllreadyTagged, j, false);
				
				write_Coloration_Tag("name_offset", "uint32_t", "[" + name + "] BOX, SPHERE, and a lot of names (sometime the same as Section5's names, but there is more name in this list).", offset, sizeof(uint32_t), "Section5_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
				write_Coloration_Tag("child", "uint16_t", "holes in [1, 0xf7], and go to 0x43a. 0xffff (none and most) the holes are explain by sibling", offset, sizeof(uint16_t), "Section5_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
				write_Coloration_Tag("unk_a0", "uint16_t", "only 0 or 0xffff (most)", offset, sizeof(uint16_t), "Section5_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
				write_Coloration_Tag("sibling", "uint16_t", "[2, 0x43d], 0xffff (none and most). seam to confirm sibling, with relation of child", offset, sizeof(uint16_t), "Section5_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
				write_Coloration_Tag("parent", "uint16_t", "only 0x0 (most), 0xffff (none). So could be like unk_a0 or all have only 0 as parent ? ", offset, sizeof(uint16_t), "Section5_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);

				write_Coloration_Tag("number_Section5_Hitbox_b", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section5_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
				write_Coloration_Tag("offset_Section5_Hitbox_b", "uint32_t", "=> ref to Section5_Box_b", offset, sizeof(uint32_t), "Section5_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
				
				if (!isOldVersion)
				{
					write_Coloration_Tag("offset_Section5_Hitbox_f", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
				}
				
				write_Coloration_Tag("number_Section5_Hitbox_e", "uint32_t", " value in [0, 0x53da], number of Section5_Box_e", offset, sizeof(uint32_t), "Section5_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
				write_Coloration_Tag("offset_Section5_Hitbox_e", "uint32_t", "=> Ref to Section5_Box_e", offset, sizeof(uint32_t), "Section5_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
				write_Coloration_Tag("number_Indexes", "uint32_t", "count of series of uint16_t values (list of Id, may the one of each Section5_Box_e (same number)). if not null (and other unknowed cases), the next value is the unk3_offset", offset, sizeof(uint32_t), "Section5_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
				write_Coloration_Tag("offset_Indexes", "uint32_t", "offset of series of uint16_t values (list of Id, may the one of each Section5_Box_e (same number)). if not null (and other unknowed cases), the next value is the unk3_offset", offset, sizeof(uint32_t), "Section5_Box" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
				

				
				offset_tmp = section5_Hitbox->offset_Section5_Hitbox_b;
				if (offset_tmp)
				{
					size_t start_Section5_Hitbox_b_list = offset_tmp;
					
					for (size_t k = 0; k < section5_Hitbox->number_Section5_Hitbox_b; k++)
					{
						size_t start_Section5_Hitbox_b = start_Section5_Hitbox_b_list + k * sizeof(FMP_Section5_Hitbox_b);
						offset = start_Section5_Hitbox_b;


						FMP_Section5_Hitbox_b* section5_Hitbox_b = (FMP_Section5_Hitbox_b*)GetOffsetPtr(buf, start_Section5_Hitbox_b);


						write_Coloration_Tag("number_Section5_Hitbox_c", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_b" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
						write_Coloration_Tag("offset_Section5_Hitbox_c", "uint32_t", "=> ref to Section5_Box_c ", offset, sizeof(uint32_t), "Section5_Box_b" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);


						offset_tmp = section5_Hitbox_b->offset_Section5_Hitbox_c;
						if (offset_tmp)
						{
							size_t start_Section5_Hitbox_c_list = offset_tmp;;

							for (size_t m = 0; m<section5_Hitbox_b->number_Section5_Hitbox_c;m++)
							{
								size_t start_Section5_Hitbox_c = start_Section5_Hitbox_c_list + m * sizeof(FMP_Section5_Hitbox_c);
								offset = start_Section5_Hitbox_c;

								FMP_Section5_Hitbox_c* section5_Hitbox_c = (FMP_Section5_Hitbox_c*)GetOffsetPtr(buf, start_Section5_Hitbox_c);

								write_Coloration_Tag("unk_0", "uint32_t", "few hole in [0 (most), 0x11]", offset, sizeof(uint32_t), "Section5_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
								write_Coloration_Tag("offset_Section5_Hitbox_d", "uint32_t", "=> ref to a Section5_Box_d", offset, sizeof(uint32_t), "Section5_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);

								write_Coloration_Tag("unk_1", "uint32_t", "from 0x0 (most) to 0x1240b", offset, sizeof(uint32_t), "Section5_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
								write_Coloration_Tag("unk_2", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section5_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
								write_Coloration_Tag("size_Section5_File", "uint32_t", "size in octets of part started by unk3_offset (seam to be a file inserted)", offset, sizeof(uint32_t), "Section5_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
								write_Coloration_Tag("offset_Section5_File", "uint32_t", "0 start of a part (seam to be a file inserted). Look it's about Havok Engine : https://github.com/nitaigao/engine-showcase/blob/master/etc/vendor/havok/Source/Common/Base/Object/hkReferencedObject.h", offset, sizeof(uint32_t), "Section5_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
								write_Coloration_Tag("offset_Section5_Hitbox_d_2", "uint32_t", "=> ref to a Section5_Box_d", offset, sizeof(uint32_t), "Section5_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);

								write_Coloration_Tag("unk_5", "uint32_t", "look pretty the same as unk_1, but until 0x112aa", offset, sizeof(uint32_t), "Section5_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
								write_Coloration_Tag("unk_6", "uint32_t", "from 0 (most) to 0x6159", offset, sizeof(uint32_t), "Section5_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
								write_Coloration_Tag("unk_7", "float", "only 0.0, 0.01 (most)", offset, sizeof(float), "Section5_Box_c" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);



								offset_tmp = section5_Hitbox_c->offset_Section5_Hitbox_d;
								if (offset_tmp)
								{
									size_t start_Section5_Hitbox_d = offset_tmp;
									offset = start_Section5_Hitbox_d;

									FMP_Section5_Hitbox_d* section5_Hitbox_d = (FMP_Section5_Hitbox_d*)GetOffsetPtr(buf, start_Section5_Hitbox_d);

									write_Coloration_Tag("unk_0", "uint32_t", "1 (most), 2, 3", offset, sizeof(uint32_t), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
									write_Coloration_Tag("unk_1", "float", "[-30.776886 , 1.893005] + 55.799999, 0.0 at most", offset, sizeof(float), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
									write_Coloration_Tag("unk_2", "float", "[-58.922211, 35.0], 0.0 (most)", offset, sizeof(float), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
									write_Coloration_Tag("unk_3", "float", "[-0.359985, 1.434509] plus -463.797607, -5.0, 20.0. 0.0 at most", offset, sizeof(float), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
									write_Coloration_Tag("yaw", "float", "[-3.141592, 1.570796], 0.0 (most), sound radian, so a [-pi, pi/2] could be Yaw.", offset, sizeof(float), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
									write_Coloration_Tag("pitch", "float", "[-0.012218, 0.523601], 0.0 (most), could be pitch", offset, sizeof(float), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
									write_Coloration_Tag("roll", "float", "[-3.141592, 1.570797], 0.0 (most) could be Roll", offset, sizeof(float), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
									write_Coloration_Tag("unk_7", "float", "[0.05, 707.106812], 0.5 (most)", offset, sizeof(float), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
									write_Coloration_Tag("unk_8", "float", "[0.0, 210.0], 0.5 (most)", offset, sizeof(float), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
									write_Coloration_Tag("unk_9", "float", "[0.0, 700.0], 0.5 (most)", offset, sizeof(float), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
									write_Coloration_Tag("unk_10", "float", "always 0", offset, sizeof(float), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
									write_Coloration_Tag("unk_11", "float", "always 0", offset, sizeof(float), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
									write_Coloration_Tag("unk_12", "float", "[-210.0, 0.0 (most)]", offset, sizeof(float), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
									write_Coloration_Tag("unk_13", "float", "only 0 (most), or 7.721795e-10", offset, sizeof(float), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);

								}

								offset_tmp = section5_Hitbox_c->offset_Section5_Hitbox_d_2;
								if (offset_tmp)
								{
									size_t start_Section5_Hitbox_d = offset_tmp;
									offset = start_Section5_Hitbox_d;


									FMP_Section5_Hitbox_d* section5_Hitbox_d = (FMP_Section5_Hitbox_d*)GetOffsetPtr(buf, start_Section5_Hitbox_d);

									write_Coloration_Tag("unk_0", "uint32_t", "See previous Hitbox_d", offset, sizeof(uint32_t), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
									write_Coloration_Tag("unk_1", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
									write_Coloration_Tag("unk_2", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
									write_Coloration_Tag("unk_3", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
									write_Coloration_Tag("unk_4", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
									write_Coloration_Tag("unk_5", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
									write_Coloration_Tag("unk_6", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
									write_Coloration_Tag("unk_7", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
									write_Coloration_Tag("unk_8", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
									write_Coloration_Tag("unk_9", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
									write_Coloration_Tag("unk_10", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
									write_Coloration_Tag("unk_11", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
									write_Coloration_Tag("unk_12", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
									write_Coloration_Tag("unk_13", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_d" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
								}

								if ((section5_Hitbox_c->offset_Section5_File) && (section5_Hitbox_c->size_Section5_File))
								{
									write_Coloration_Tag("sort of inserted Files", "uint32_t", "Look it's about Havok Engine : https://github.com/nitaigao/engine-showcase/blob/master/etc/vendor/havok/Source/Common/Base/Object/hkReferencedObject.h", section5_Hitbox_c->offset_Section5_File, section5_Hitbox_c->size_Section5_File, "Section5_Box_c_FILE_" + std::to_string(i), parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j);
								}
							}
						}
					}
				}


				offset_tmp = section5_Hitbox->offset_Section5_Hitbox_f;
				if ((offset_tmp)&&(!isOldVersion))
				{
					size_t start_Section5_Hitbox_f = offset_tmp;
					offset = start_Section5_Hitbox_f;

					FMP_Section5_Hitbox_f* section5_Hitbox_f = (FMP_Section5_Hitbox_f*)GetOffsetPtr(buf, start_Section5_Hitbox_f);
					
					write_Coloration_Tag("unk_0", "uint32_t", "always 1, that could be the number of file. the game crash if I use 0 or 2 instead.", offset, sizeof(uint32_t), "Section5_Box_f" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
					write_Coloration_Tag("size_Section5_File", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_f" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
					write_Coloration_Tag("offset_Section5_File", "uint32_t", "", offset, sizeof(uint32_t), "Section5_Box_f" + std::to_string(i), parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);

					if ((section5_Hitbox_f->offset_Section5_File) && (section5_Hitbox_f->size_Section5_File))
					{
						write_Coloration_Tag("sort of inserted Files", "uint32_t", "", section5_Hitbox_f->offset_Section5_File, section5_Hitbox_f->size_Section5_File, "Section5_Box_c_FILE_" + std::to_string(i), parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, j);
					}

				}





				offset_tmp = section5_Hitbox->offset_Section5_Hitbox_e;
				if (offset_tmp)
				{
					size_t start_Section5_Hitbox_e = offset_tmp;
					offset = start_Section5_Hitbox_e;

					FMP_Section5_Hitbox_e* section5_Hitbox_e = (FMP_Section5_Hitbox_e*)GetOffsetPtr(buf, start_Section5_Hitbox_e);

					/*
					for (size_t k = 0; k < section5_Hitbox->number_Section5_Hitbox_e; k++)
					{
						incParam = 0;

						write_Coloration_Tag("unk_0", "float", "[-312.058716, 332.57], 0.0 most. there is lot of progressive values, sound like a value for keyframes animations. there is very lots of very small values.", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset += sizeof(float);
						write_Coloration_Tag("unk_1", "float", "[-124.419319, 75.189041] average same as unk_0", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset += sizeof(float);
						write_Coloration_Tag("unk_2", "float", "[-250.000015, 312.366180] average same as unk_0 ", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset += sizeof(float);
						write_Coloration_Tag("unk_3", "float", "[-1.0, 1.000000], 0.0 is the most.", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset += sizeof(float);
						write_Coloration_Tag("unk_4", "float", "same as unk_3", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset += sizeof(float);
						write_Coloration_Tag("unk_5", "float", "same as unk_3", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset += sizeof(float);
					}
					*/


					
					/*
					//for less memory size of string
					incParam = 0;
					for (size_t k = 0; k < section5_Hitbox->number_Section5_Hitbox_e; k++)
					{
						if (k==0)
						{
							write_Coloration_Tag("unk_0", "float", "[-312.058716, 332.57], 0.0 most. there is lot of progressive values, sound like a value for keyframes animations. there is very lots of very small values.", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset += sizeof(float);
							write_Coloration_Tag("unk_1", "float", "[-124.419319, 75.189041] average same as unk_0", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset += sizeof(float);
							write_Coloration_Tag("unk_2", "float", "[-250.000015, 312.366180] average same as unk_0 ", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset += sizeof(float);
							write_Coloration_Tag("unk_3", "float", "[-1.0, 1.000000], 0.0 is the most.", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset += sizeof(float);
							write_Coloration_Tag("unk_4", "float", "same as unk_3", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset += sizeof(float);
							write_Coloration_Tag("unk_5", "float", "same as unk_3", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset += sizeof(float);

						}else {
							write_Coloration_Tag("HitBox_e", "FMP_Section5_Hitbox_e", "", offset, sizeof(FMP_Section5_Hitbox_e), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, k); offset += sizeof(FMP_Section5_Hitbox_e);
						}
					}
					*/

					//for less memory size of string (again)
					incParam = 0;					
					if (section5_Hitbox->number_Section5_Hitbox_e > 0)
					{
						write_Coloration_Tag("unk_0", "float", "[-312.058716, 332.57], 0.0 most. there is lot of progressive values, sound like a value for keyframes animations. there is very lots of very small values.", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, 0); offset += sizeof(float);
						write_Coloration_Tag("unk_1", "float", "[-124.419319, 75.189041] average same as unk_0", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, 0); offset += sizeof(float);
						write_Coloration_Tag("unk_2", "float", "[-250.000015, 312.366180] average same as unk_0 ", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, 0); offset += sizeof(float);
						write_Coloration_Tag("unk_3", "float", "[-1.0, 1.000000], 0.0 is the most.", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, 0); offset += sizeof(float);
						write_Coloration_Tag("unk_4", "float", "same as unk_3", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, 0); offset += sizeof(float);
						write_Coloration_Tag("unk_5", "float", "same as unk_3", offset, sizeof(float), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, 0); offset += sizeof(float);

						write_Coloration_Tag("HitBox_e", "section5_Hitbox->number_Section5_Hitbox_e * FMP_Section5_Hitbox_e", "", offset, (section5_Hitbox->number_Section5_Hitbox_e -1) * sizeof(FMP_Section5_Hitbox_e), "Section5_Box_e[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged, 1); offset += (section5_Hitbox->number_Section5_Hitbox_e - 1) * sizeof(FMP_Section5_Hitbox_e);
					}
				}

				offset_tmp = section5_Hitbox->offset_Indexes;
				if ((offset_tmp) && (section5_Hitbox->number_Indexes))
				{
					offset = offset_tmp;


					/*
					incParam = 0;
					for (size_t k = 0; k < section5_Hitbox->number_Indexes; k++)
					{
						write_Coloration_Tag("Index", "uint16_t", "seam to have ALL values in [0 (most), 61626]. wait what ??? too much values, what could be indexed", offset, sizeof(uint16_t), "Section5_Box__[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, k); offset += sizeof(uint16_t);
					}
					*/
					//for less memory size of string
					write_Coloration_Tag("List Index", std::to_string(section5_Hitbox->number_Indexes)+ " x uint16_t", "seam to have ALL values in [0 (most), 61626]. wait what ??? too much values, what could be indexed", offset, section5_Hitbox->number_Indexes * sizeof(uint16_t), "Section5_Box__[" + std::to_string(i) + "][" + std::to_string(j) + "]", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += section5_Hitbox->number_Indexes * sizeof(uint16_t);
				}


				if (isOldVersion)
					delete section5_Hitbox;
			}
		}


		//associate name on last block.
		write_Coloration_Tag("name", "string", "", section5[i].name_offset, 4 * sizeof(char), "Section5", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}







	offset = hdr->offset_Dependance_Nsk;
	incSection += 2;
	incParam = 0;
	for (size_t i = 0; i < hdr->numberFileInDependance_Nsk; i++)
	{
		uint32_t name_offset = *(uint32_t*)GetOffsetPtr(buf, offset);
		write_Coloration_Tag("name_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMP_Dep1 (geometry/meshs)", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);

		write_Coloration_Tag("name", "string", "", name_offset, 4 * sizeof(char), "FMP_Dep1", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}

	
	offset = hdr->offset_Dependance_TextureEmb;
	incSection++;
	incParam = 0;
	for (size_t i = 0; i < hdr->numberFileInDependance_TextureEmb; i++)
	{
		uint32_t name_offset = *(uint32_t*)GetOffsetPtr(buf, offset);
		write_Coloration_Tag("name_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMP_Dep2 (textures)", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("name", "string", "", name_offset, 4 * sizeof(char), "FMP_Dep2", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}

	offset = hdr->offset_Dependance_Emm;
	incSection++;
	incParam = 0;
	for (size_t i = 0; i < hdr->numberFileInDependance_Emm; i++)
	{
		uint32_t name_offset = *(uint32_t*)GetOffsetPtr(buf, offset);
		write_Coloration_Tag("name_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMP_Dep3 (Materials)", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("name", "string", "", name_offset, 4 * sizeof(char), "FMP_Dep3", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}

	offset = hdr->offset_Dependance_Ema;
	incSection++;
	incParam = 0;
	for (size_t i = 0; i < hdr->numberFileInDependance_Ema; i++)
	{
		uint32_t name_offset = *(uint32_t*)GetOffsetPtr(buf, offset);
		write_Coloration_Tag("name_offset", "uint32_t", "", offset, sizeof(uint32_t), "FMP_Dep4 (Animations)", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("name", "string", "", name_offset, 4 * sizeof(char), "FMP_Dep4", parent, idTag++, incSection, i, listBytesAllreadyTagged, i, false);
	}




	write_Coloration_Gaps(0x2F8, hdr->offset_Dependance_Nsk, sizeof(uint16_t) + sizeof(uint8_t), idTag, "Empty", parent, listBytesAllreadyTagged);		//test Empty/gaps/holes.
}





/*-------------------------------------------------------------------------------\
|                             write_Coloration_Tag			                     |
\-------------------------------------------------------------------------------*/
void FmpFile::write_Coloration_Tag(string paramName, string paramType, string paramComment, size_t startOffset, size_t size, string sectionName, TiXmlElement* parent, size_t idTag, size_t sectionIndex, size_t paramIndex, std::vector<bool> &listBytesAllreadyTagged, size_t sectionIndexInList, bool checkAllreadyTaggued)
{
	TiXmlElement* tag_node = new TiXmlElement("TAG");

	tag_node->SetAttribute("id", UnsignedToString(idTag, false));


	TiXmlElement* start_offset_node = new TiXmlElement("start_offset");
	TiXmlText* text = new TiXmlText(std::to_string(startOffset));
	start_offset_node->LinkEndChild(text);
	tag_node->LinkEndChild(start_offset_node);

	TiXmlElement* end_offset_node = new TiXmlElement("end_offset");
	text = new TiXmlText(std::to_string(startOffset + size - 1));
	end_offset_node->LinkEndChild(text);
	tag_node->LinkEndChild(end_offset_node);

	TiXmlElement* text_node = new TiXmlElement("tag_text");
	text = new TiXmlText(sectionName + ((sectionIndexInList != (size_t)-1) ? "[" + std::to_string(sectionIndexInList) + "]" : "") + "." + paramName + " (" + paramType + ") : " + paramComment);
	text_node->LinkEndChild(text);
	tag_node->LinkEndChild(text_node);





	if (listTagColors.size() == 0)
	{
		listTagColors.push_back(std::vector<std::vector<string>>());
		listTagColors.back().push_back(std::vector<string>());
		listTagColors.back().back().push_back("#000000");			//background color
		listTagColors.back().back().push_back("#FFFFFF");			//font color (just to possibility read from background color)
	}

	size_t sectionIndex_tmp = sectionIndex % listTagColors.size();
	std::vector<std::vector<string>> &sectionColorlist = listTagColors.at(sectionIndex_tmp);
	size_t paramIndex_tmp = paramIndex % sectionColorlist.size();
	std::vector<string> &paramColors = sectionColorlist.at(paramIndex_tmp);

	TiXmlElement* font_colour_node = new TiXmlElement("font_colour");
	text = new TiXmlText(paramColors.at(1));
	font_colour_node->LinkEndChild(text);
	tag_node->LinkEndChild(font_colour_node);

	TiXmlElement* bg_colour_node = new TiXmlElement("note_colour");
	text = new TiXmlText(paramColors.at(0));
	bg_colour_node->LinkEndChild(text);
	tag_node->LinkEndChild(bg_colour_node);

	parent->LinkEndChild(tag_node);




	//un check d'overide , pour savoir si des blocks se chevauche.
	size_t index;
	size_t limit = listBytesAllreadyTagged.size();
	for (size_t i = 0; i < size; i++)
	{
		index = startOffset + i;
		if (index >= limit)
		{
			printf("Error on tagID %i : overflow %i >= %i.\n", idTag, index, limit);
			LibXenoverse::notifyError();
			continue;
		}

		if (index == 0x11ac)			//for test
			int aa = 42;

		if ((checkAllreadyTaggued) && (listBytesAllreadyTagged.at(index)))
		{
			printf("warning on tagID %i : the byte %i allready taggued, may be a overflow between blocks. Infos : %s. \n", idTag, index, (sectionName + ((sectionIndexInList != (size_t)-1) ? "[" + std::to_string(sectionIndexInList) + "]" : "") + "." + paramName + " (" + paramType + ") : " + paramComment).c_str());
			LibXenoverse::notifyError();
		}

		listBytesAllreadyTagged.at(index) = true;
	}
}




/*-------------------------------------------------------------------------------\
|                             write_Coloration_Gaps			                     |
\-------------------------------------------------------------------------------*/
void FmpFile::write_Coloration_Gaps(size_t startOffsetToConsidere, size_t endOffsetToConsidere, size_t sizeFilter, size_t start_idTag, string sectionName, TiXmlElement* parent, std::vector<bool> &listBytesAllreadyTagged)
{
	size_t nbBytes = listBytesAllreadyTagged.size();
	if ((nbBytes==0)||(startOffsetToConsidere >= nbBytes)|| (startOffsetToConsidere > endOffsetToConsidere))
		return;

	if (endOffsetToConsidere >= nbBytes)
		endOffsetToConsidere = nbBytes - 1;


	size_t sectionStart = (size_t)-1;
	size_t inc = 0;
	bool isByteTagged = false;
	for (size_t i = startOffsetToConsidere;i<=endOffsetToConsidere;i++)			//detection of the Hole
	{
		isByteTagged = listBytesAllreadyTagged.at(i);

		if ((!isByteTagged) && (sectionStart==(size_t)-1))
		{
			sectionStart = i;

		}else if (( (isByteTagged) || (i== endOffsetToConsidere) )&&(sectionStart != (size_t)-1)) {		//if end of Section, or end of range.

			size_t size = i - sectionStart;
			
			if(size > sizeFilter)
			{
				TiXmlElement* tag_node = new TiXmlElement("TAG");

				tag_node->SetAttribute("id", UnsignedToString(start_idTag++, false));


				TiXmlElement* start_offset_node = new TiXmlElement("start_offset");
				TiXmlText* text = new TiXmlText(std::to_string(sectionStart));
				start_offset_node->LinkEndChild(text);
				tag_node->LinkEndChild(start_offset_node);

				start_offset_node = new TiXmlElement("start_offset_Hexa");
				text = new TiXmlText(UnsignedToString(sectionStart, true));
				start_offset_node->LinkEndChild(text);
				tag_node->LinkEndChild(start_offset_node);

				TiXmlElement* end_offset_node = new TiXmlElement("end_offset");
				text = new TiXmlText(std::to_string(sectionStart + size - 1));
				end_offset_node->LinkEndChild(text);
				tag_node->LinkEndChild(end_offset_node);

				TiXmlElement* text_node = new TiXmlElement("tag_text");
				text = new TiXmlText(sectionName + "[" + std::to_string(inc++) + "]. Size="+ std::to_string(size));
				text_node->LinkEndChild(text);
				tag_node->LinkEndChild(text_node);

				TiXmlElement* font_colour_node = new TiXmlElement("font_colour");
				text = new TiXmlText("#000000");
				font_colour_node->LinkEndChild(text);
				tag_node->LinkEndChild(font_colour_node);

				TiXmlElement* bg_colour_node = new TiXmlElement("note_colour");
				text = new TiXmlText("#FF0000");
				bg_colour_node->LinkEndChild(text);
				tag_node->LinkEndChild(bg_colour_node);

				parent->LinkEndChild(tag_node);
			}

			sectionStart = (size_t)-1;
		}
	}
}


}