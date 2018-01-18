#include "Etr.h"

namespace LibXenoverse
{

/*-------------------------------------------------------------------------------\
|                             Etr												 |
\-------------------------------------------------------------------------------*/
Etr::Etr()
{
	//section 0 - ETRHeader : black/grey
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#232323");			//background color
	listTagColors.back().back().push_back("#FFFFFF");			//font color (just to possibility read from background color)
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#464646");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 1 - ETRUnk0AEntry : red
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0000");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC7777");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 2 - ETRUnk0BEntry : green
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 3 - ETRUnk1Entry : bleu
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#0000CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7777CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f




	//section 5 - ETRUnk2Entry : yellow
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC00");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCC77");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 6 - ETRUnk3Entry : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#00CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#77CCCC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 7 - ETRUnk3Entry indexes : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#009999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#339999");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 8 - ETRUnk4Entry : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC00CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC77CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f


	//section 9 - ETRUnk4Entry_Next : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#990099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#993399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



	//section 10 - ETRUnk4Entry_Next_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933CC");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 11 - ETRUnk4Entry_Next_c : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CC3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 12 - ETRUnk4Entry_Next_d : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933AA");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 13 - ETRUnk4Entry_Next_e : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA0099");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA3399");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f



	//section 14 - ETRUnk4Entry_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#770077");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#773377");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 15 - ETRUnk4Entry_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#550055");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#553355");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f





	//section 16 - ETRUnk4Entry_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA00FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AA33FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 17 - ETRUnk4Entry_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9900FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#9933FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 18 - ETRUnk4Entry_Next2 : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7700FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#7733FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f

	//section 19 - ETRUnk4Entry_Next2_b : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5500FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#5533FF");			//bg
	listTagColors.back().back().push_back("#FFFFFF");			//f




	//section 20 - ETRUnk5Entry : 
	listTagColors.push_back(std::vector<std::vector<string>>());
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#CCCCCC");			//bg
	listTagColors.back().back().push_back("#000000");			//f
	listTagColors.back().push_back(std::vector<string>());
	listTagColors.back().back().push_back("#AAAAAA");			//bg
	listTagColors.back().back().push_back("#000000");			//f


	//section 21 - ETRUnk5Entry_Box : 
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
void Etr::Reset()
{
	mListPart.clear();

	size_t nbText = mListTextureUnit.size();
	for (size_t i = 0; i < nbText; i++)
		delete mListTextureUnit.at(i);
	mListTextureUnit.clear();
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void Etr_TextureUnit::Reset()
{
	unk_0 = unk_2 = unk_3 = mEmbTextureId = unk_2_0 = unk_2_1 = unk_2_2 = 0;
	mTypeRepetition_U = mTypeRepetition_V = TR_Wrap;
	mMinFiltering = mMaxFiltering = TR_Linear;
	mUseSpeed = false;
	mScrollSpeed_U = mScrollSpeed_V = 0.0f;
	mEmbTextureId = (size_t)-1;
	mListKeyframe.clear();
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void Etr_Part::Reset()
{
	string mAttachedBoneName = "";
	unk_0_0 = unk_0_1 = unk_0_2 = unk_0_3 = unk_0_4 = unk_0_5 = unk_0_6 = unk_0_7 = unk_1 = mTimeStartExtrude = unk_2 = mNumberFrameForNextSegment = mDurationExtrude = unk_6 = mDurationHold = unk_13 = 0;
	mPositionOffset_X = mPositionOffset_Y = mPositionOffset_Z = mRotation_Pitch = mRotation_Yaw = mRotation_Roll = mPositionExtrude_Z = mScaleShape = mEndExtrude_WorldScale_add = mEndExtrude_WorldOffset_factor = mEndExtrude_WorldOffset_factorb = 0.0f;
	mColor0[0] = mColor0[1] = mColor0[2] = mColor0[3] = mColor1[0] = mColor1[1] = mColor1[2] = mColor1[3] = mEndExtrude_WorldScale_factor = 1.0;
	mEmmMaterialId = (size_t)-1;
	mAutoOrientation = mNoDegrade = mDoublePointOfMiddleSection = mDisplayMiddleSection = mUVPauseOnExtrude = mUVPauseOnHold = false;

	for (size_t i = 0; i < 8; i++)
		unk_4_b[i] = unk_4b_b[i] = unk_5_b[i] = false;

	mListTextureUnit.clear();
	mExtrudePath.clear();
	mExtrudeLineShape.clear();
	mListAnimationB.clear();
	mListAnimationA.clear();	
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void Etr_AnimationA::Reset()
{
	mStartAnimation = SA_Start;
	unk_0b = unk_1 = unk_1b = mComponent = unk_2 = 0;
	mListKeyframe.clear();
}
/*-------------------------------------------------------------------------------\
|                             Reset					                             |
\-------------------------------------------------------------------------------*/
void Etr_AnimationB::Reset()
{
	unk_0 = 0;
	mAnimations.clear();
}






/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool Etr::Load(const uint8_t *buf, size_t size)
{
	Reset();

	ETRHeader *hdr = (ETRHeader *)buf;

	if (size < sizeof(ETRHeader) || memcmp(hdr->signature, ETR_SIGNATURE, 4) != 0)
		return false;

	
	//Section1 make reference to Section2. so we begin by section2.
	size_t offset = 0;
	for (size_t i = 0; i < hdr->nbSection2; i++)
	{
		offset = hdr->offsetSection2 + i * sizeof(ETR_Section2);

		Etr_TextureUnit* textUnit = new Etr_TextureUnit();
		if( textUnit->Load( GetOffsetPtr(buf, offset), size - val32(offset) ) )
			mListTextureUnit.push_back(textUnit);
	}


	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		offset = hdr->offsetSection1 + i * sizeof(ETR_Section1);

		Etr_Part part;
		if (part.Load(GetOffsetPtr(buf, offset), size - val32(offset), hdr->offsetSection2 - offset, mListTextureUnit))
			mListPart.push_back(part);
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool Etr_TextureUnit::Load(const uint8_t *buf, size_t size)
{
	Reset();

	ETR_Section2* section2 = (ETR_Section2*)buf;

	unk_0 = section2[0].unk_0;
	mEmbTextureId = (section2[0].embTextureId!=0xFF) ? section2[0].embTextureId : (size_t)-1;
	mTypeRepetition_U = (TypeRepetition)((section2[0].unk_1 >> 4) & 0xF);
	mTypeRepetition_V = (TypeRepetition)((section2[0].unk_1) & 0xF);

	mMinFiltering = (TypeFiltering)((section2[0].unk_1b >> 4) & 0xF);
	mMaxFiltering = (TypeFiltering)((section2[0].unk_1b) & 0xF);
	unk_2 = section2[0].unk_2;
	unk_3 = section2[0].unk_3;

	size_t type = section2[0].typeSection2_b;

	switch (type)
	{
	case 0:
		{
			mUseSpeed = false;
			ETR_Section2_b_00 *section2b = (ETR_Section2_b_00*)((uint8_t*)buf + 0xC);
			mListKeyframe.push_back(Keyframe(0, section2b[0].color[0], section2b[0].color[1], section2b[0].color[2], section2b[0].color[3]));
		}
		break;

	case 1:
		{
			mUseSpeed = true;
			ETR_Section2_b_01 *section2b = (ETR_Section2_b_01*)((uint8_t*)buf + 0xC);
			mScrollSpeed_U = section2b[0].color[0];
			mScrollSpeed_V = section2b[0].color[1];
		}
		break;

	case 2:
		{
			mUseSpeed = false;
			ETR_Section2_b_02 *section2b = (ETR_Section2_b_02*)((uint8_t*)buf + 0xC);
			unk_2_0 = section2b->unk_0;
			unk_2_1 = section2b->unk_1;
			unk_2_2 = section2b->unk_2;

			if (section2b->offsetSection2_02_c != 0)
			{
				ETR_Section2_b_02_c *section2c = (ETR_Section2_b_02_c*)((uint8_t*)buf + section2b->offsetSection2_02_c + 0xC);
				for (size_t i = 0; i < section2b->numberSection2_02_c; i++)
					mListKeyframe.push_back(Keyframe(section2c[i].duration, section2c[i].color[0], section2c[i].color[1], section2c[i].color[2], section2c[i].color[3]));
			}
		}
		break;
	}
	
	return true;
}

/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool Etr_Part::Load(const uint8_t *buf, size_t size, size_t offsetStartTextureUnits, std::vector<Etr_TextureUnit*> &listTextureUnit)
{
	Reset();

	ETR_Section1* section1 = (ETR_Section1*)buf;

	mAttachedBoneName = string(section1->boneName);

	unk_0_0 = section1->unk_0_0;
	unk_0_1 = section1->unk_0_1;
	unk_0_2 = section1->unk_0_2;
	unk_0_3 = section1->unk_0_3;
	unk_0_4 = section1->unk_0_4;
	unk_0_5 = section1->unk_0_5;
	unk_0_6 = section1->unk_0_6;
	unk_0_7 = section1->unk_0_7;

	mPositionOffset_X = section1->unk_0_8;
	mPositionOffset_Y = section1->unk_0_9;
	mPositionOffset_Z = section1->unk_0_10;

	mRotation_Pitch = section1->unk_0_11 * 180.0f / 3.14159265358979f;
	mRotation_Yaw = section1->unk_0_12 * 180.0f / 3.14159265358979f;
	mRotation_Roll = section1->unk_0_13 * 180.0f / 3.14159265358979f;

	unk_1 = section1->unk_1;
	mTimeStartExtrude = section1->unk_1b;
	unk_2 = section1->unk_2;
	mNumberFrameForNextSegment = section1->unk_2b;
	mDurationExtrude = section1->unk_3;
	
	for (size_t i = 0; i < 8; i++)
	{
		unk_4_b[i] = ((section1->unk_4 >> i) & 1) == 1;				// => the left bit is the first bool
		unk_4b_b[i] = ((section1->unk_4b >> i) & 1) == 1;
		unk_5_b[i] = ((section1->unk_5 >> i) & 1) == 1;
	}

	mAutoOrientation = unk_4_b[0] == 1;
	mNoDegrade = unk_4_b[2] == 1;
	mDoublePointOfMiddleSection = unk_4_b[3] == 1;
	mDisplayMiddleSection = unk_4b_b[0] == 1;
	mUVPauseOnExtrude = unk_4b_b[1] == 1;
	mUVPauseOnHold = unk_5_b[0] == 1;


	unk_6 = section1->unk_6;
	mPositionExtrude_Z = section1->unk_7;
	mScaleShape = section1->unk_9;
	mDurationHold = section1->unk_11;
	unk_13 = section1->unk_13;

	mEmmMaterialId = (section1->emmMaterialId != 0xFFFF) ? section1->emmMaterialId : (size_t)-1;

	mColor0[0] = section1->colorStart[0];
	mColor0[1] = section1->colorStart[1];
	mColor0[2] = section1->colorStart[2];
	mColor0[3] = section1->colorStart[3];

	mColor1[0] = section1->colorEnd[0];
	mColor1[1] = section1->colorEnd[1];
	mColor1[2] = section1->colorEnd[2];
	mColor1[3] = section1->colorEnd[3];


	//TextureUnits / Section2
	if (section1->offsetSection2)
	{
		size_t nbTextureUnits = listTextureUnit.size();

		size_t offset = section1->offsetSection2;
		uint32_t* tmp = (uint32_t*)((uint8_t*)buf + offset);

		for (size_t j = 0; j < section1->numberSection2; j++)
		{
			size_t indexTextureUnit = (tmp[j] - offsetStartTextureUnits) / sizeof(ETR_Section2);
			if (indexTextureUnit < nbTextureUnits)
			{
				mListTextureUnit.push_back( listTextureUnit.at(indexTextureUnit) );
			}else {
				printf("Error : a TextureUnit (index %i from a Etr_part) is not found (adress matching). skipped.", j);
			}
		}
	}

	//Extrude / section3
	if (section1->offsetSection3)
	{
		size_t offset = section1->offsetSection3;
		float* tmp = (float*)((uint8_t*)buf + offset);
		size_t inc = 0;

		mEndExtrude_WorldScale_factor = tmp[inc++];
		mEndExtrude_WorldScale_add = tmp[inc++];
		mEndExtrude_WorldOffset_factor = tmp[inc++];
		mEndExtrude_WorldOffset_factorb = tmp[inc++];

		size_t index = 0;
		for (size_t j = 0; j < section1->numberPointsExtrudePathSection3; j++)
		{
			mExtrudePath.push_back(Point(tmp[inc], tmp[inc+1], tmp[inc + 2], tmp[inc + 3]));
			inc += 4;
		}

		if (section1->numberSegmentsExtrudeShapeSection3 != 0)
		{
			for (size_t j = 0; j <= section1->numberSegmentsExtrudeShapeSection3; j++)		//nbPoints = nbSegments + 1
			{
				mExtrudeLineShape.push_back(Point(tmp[inc], tmp[inc + 1]));
				inc += 2;
			}
		}
	}


	//AnimationB / Section4
	if (section1->offsetSection4)
	{
		size_t offset;
		for (size_t j = 0; j < section1->numberSection4; j++)
		{
			offset = section1->offsetSection4 + 0x78 + j * sizeof(ETR_Section4);

			Etr_AnimationB anim;
			anim.Load(((uint8_t*)buf + offset), size - offset);
			mListAnimationB.push_back(anim);
		}
	}


	//AnimationA / Section5
	if (section1->offsetSection5)
	{
		size_t offset;
		for (size_t j = 0; j < section1->numberSection5; j++)
		{
			offset = section1->offsetSection5 + 0xA8 + j * sizeof(ETR_Section5);

			Etr_AnimationA anim;
			anim.Load(( (uint8_t*)buf + offset), size - offset);
			mListAnimationA.push_back(anim);
		}
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool Etr_AnimationB::Load(const uint8_t *buf, size_t size)
{
	Reset();

	ETR_Section4* section4 = (ETR_Section4*)buf;

	unk_0 = section4->unk_0;
	unk_0b = section4->unk_0b;

	if (section4->offsetSection4_b)
	{
		size_t offset;
		for (size_t j = 0; j < section4->nbSection4_b; j++)
		{
			offset = section4->offsetSection4_b + j * sizeof(ETR_Section4b);

			Etr_AnimationA anim;
			anim.Load(((uint8_t*)buf + offset), size - offset);
			mAnimations.push_back(anim);
		}
	}
	
	return true;
}

/*-------------------------------------------------------------------------------\
|                             Load					                             |
\-------------------------------------------------------------------------------*/
bool Etr_AnimationA::Load(const uint8_t *buf, size_t size)
{
	Reset();

	ETR_Section5* section5 = (ETR_Section5*)buf;

	mStartAnimation = (StartAnimation)section5->unk_0;
	unk_0b = section5->unk_0b;

	mComponent = section5->component & 0xf;
	unk_1b = (section5->component >> 4) & 0xf;
	unk_1 = section5->unk_1;
	unk_2 = section5->unk_2;

	if (section5->numberKeyFrames == 0)
		return true;

	std::vector<uint16_t> listTimeIndex;
	std::vector<float> listValues;

	if (section5->offsetKeyFrames)
	{
		uint16_t* listUint16 = (uint16_t*)((uint8_t*)buf + section5->offsetKeyFrames);
		for (size_t i = 0; i < section5->numberKeyFrames; i++)
			listTimeIndex.push_back(listUint16[i]);
	}
	
	if (section5->offsetFloats)
	{
		float* listFloats = (float*)((uint8_t*)buf + section5->offsetFloats);
		for (size_t i = 0; i < section5->numberKeyFrames; i++)
			listValues.push_back(listFloats[i]);
	}

	size_t nbTimes = listTimeIndex.size();
	size_t nbValues = listValues.size();
	size_t inc = 1;
	for (size_t i = 0; i < section5->numberKeyFrames; i++)
		mListKeyframe.push_back(Keyframe( ((i<nbTimes) ? listTimeIndex.at(i) : (listTimeIndex.back() + inc++)), ((i<nbValues) ? listValues.at(i) : 0.0f)) );

	return true;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////











/*-------------------------------------------------------------------------------\
|                             calculeFilesize		                             |
\-------------------------------------------------------------------------------*/
size_t Etr::calculeFilesize(bool withTextureUnits)
{
	size_t filesize = 0x20;							//header, + filling to have proper alignement 16 bytes, taht help to have stable subDatas


	size_t nbParts = mListPart.size();
	for (size_t i = 0; i < nbParts; i++)
		filesize += mListPart.at(i).calculeFilesize();

	size_t nbText = (withTextureUnits) ? mListTextureUnit.size() : 0;
	for (size_t i = 0; i < nbText; i++)
		filesize += mListTextureUnit.at(i)->calculeFilesize();

	return filesize;
}
/*-------------------------------------------------------------------------------\
|                             calculeFilesize		                             |
\-------------------------------------------------------------------------------*/
size_t Etr_TextureUnit::calculeFilesize()
{
	size_t filesize = sizeof(ETR_Section2);							//header, + filling to have proper alignement 16 bytes, taht help to have stable subDatas

	if((!mUseSpeed) && (mListKeyframe.size()>1))
		filesize += sizeof(ETR_Section2_b_02_c) * mListKeyframe.size();

	return filesize;
}
/*-------------------------------------------------------------------------------\
|                             calculeFilesize		                             |
\-------------------------------------------------------------------------------*/
size_t Etr_Part::calculeFilesize()
{
	size_t filesize = sizeof(ETR_Section1);

	filesize += sizeof(uint32_t) * mListTextureUnit.size();			//pointer to Section2

	size_t nbAnim = mListAnimationA.size();
	for (size_t i = 0; i < nbAnim; i++)
		filesize += mListAnimationA.at(i).calculeFilesize();

	nbAnim = mListAnimationB.size();
	for (size_t i = 0; i < nbAnim; i++)
		filesize += mListAnimationB.at(i).calculeFilesize();


	filesize += 4 * sizeof(float) + 4 * sizeof(float) * mExtrudePath.size() + 2 * sizeof(float) * mExtrudeLineShape.size();
	
	return filesize;
}
/*-------------------------------------------------------------------------------\
|                             calculeFilesize		                             |
\-------------------------------------------------------------------------------*/
size_t Etr_AnimationA::calculeFilesize()
{
	size_t filesize = sizeof(ETR_Section5);

	filesize += sizeof(uint16_t) * mListKeyframe.size();
	if (filesize & 0x3)
		filesize += (0x4 - (filesize & 0x3));

	filesize += sizeof(float) * mListKeyframe.size();

	if (mListKeyframe.size() > 1)
		filesize += sizeof(uint16_t) * 102;					//index list witch do nothing. and we see nothing witch confirme the size , so we take the hypothese it's always the same for rebuild.

	return filesize;
}
/*-------------------------------------------------------------------------------\
|                             calculeFilesize		                             |
\-------------------------------------------------------------------------------*/
size_t Etr_AnimationB::calculeFilesize()
{
	size_t filesize = sizeof(ETR_Section4);

	size_t nbAnim = mAnimations.size();
	for (size_t i = 0; i < nbAnim; i++)
		filesize += mAnimations.at(i).calculeFilesize();

	return filesize;
}








/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*-------------------------------------------------------------------------------\
|                             CreateFile			                             |
\-------------------------------------------------------------------------------*/
uint8_t* Etr::CreateFile(unsigned int *psize)
{
	size_t filesize = calculeFilesize();

	uint8_t *buf = new uint8_t[filesize];
	if (!buf)
	{
		LOG_DEBUG("%s: Memory allocation error (0x%x)\n", FUNCNAME, filesize);
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
size_t Etr::write(uint8_t *buf, size_t size)
{
	ETRHeader* hdr = (ETRHeader*)buf;
	size_t offset = 0;

	strcpy(&(hdr->signature[0]), ETR_SIGNATURE);
	hdr->endian = val16(0xFFFE);
	hdr->version = 0x92c00018;
	hdr->unk_00 = 0;

	hdr->nbSection1 = mListPart.size();
	hdr->offsetSection1 = 0x20;										//we fix the start at 0x20 for better view
	hdr->nbSection2 = mListTextureUnit.size();
	hdr->offsetSection2 = calculeFilesize(false);


	offset = hdr->offsetSection1;
	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		offset = hdr->offsetSection1 + i * sizeof(ETR_Section1);
		mListPart.at(i).write_Main(buf + offset, size - offset);
	}

	size_t startSection1Offset = 0;
	offset = hdr->offsetSection1 + hdr->nbSection1 * sizeof(ETR_Section1);
	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		startSection1Offset = hdr->offsetSection1 + i * sizeof(ETR_Section1);

		*((uint32_t*)GetOffsetPtr(buf, startSection1Offset + 0xAC, true)) = (mListPart.at(i).mListAnimationA.size()) ? (offset - startSection1Offset - 0xA8) : 0;		//setup section1->offsetSection5;
		offset += mListPart.at(i).write_AnimationsA(buf + offset, size - offset);


		*((uint32_t*)GetOffsetPtr(buf, startSection1Offset + 0xA4, true)) = (mListPart.at(i).mListAnimationB.size()) ? (offset - startSection1Offset - 0x78) : 0;		//setup section1->offsetSection4;
		offset += mListPart.at(i).write_AnimationsB(buf + offset, size - offset);
	}

	
	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		startSection1Offset = hdr->offsetSection1 + i * sizeof(ETR_Section1);

		*((uint32_t*)GetOffsetPtr(buf, startSection1Offset + 0x70, true)) = (mListPart.at(i).mListTextureUnit.size()) ? (offset - startSection1Offset) : 0;		//setup section1->offsetSection2;
		offset += mListPart.at(i).write_TextureUnitPointers(buf + offset, size - offset, hdr->offsetSection2 - (hdr->offsetSection1 + i * sizeof(ETR_Section1)) , mListTextureUnit);
	}


	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		startSection1Offset = hdr->offsetSection1 + i * sizeof(ETR_Section1);

		*((uint32_t*)GetOffsetPtr(buf, startSection1Offset + 0x74, true)) = offset - startSection1Offset;		//setup section1->offsetSection3;
		offset += mListPart.at(i).write_Extrude(buf + offset, size - offset);
	}


	//TextureUnits
	offset = hdr->offsetSection2;
	for (size_t i = 0; i < hdr->nbSection2; i++)
	{
		offset = hdr->offsetSection2 + i * sizeof(ETR_Section2);
		mListTextureUnit.at(i)->write_Main(buf + offset, size - offset);
	}

	size_t startSection2Offset = 0;
	offset = hdr->offsetSection2 + hdr->nbSection2 * sizeof(ETR_Section2);
	for (size_t i = 0; i < hdr->nbSection2; i++)
	{
		if ((mListTextureUnit.at(i)->mUseSpeed) || (mListTextureUnit.at(i)->mListKeyframe.size() <=1 ))
			continue;

		startSection2Offset = hdr->offsetSection2 + i * sizeof(ETR_Section2);
		
		*((uint32_t*)GetOffsetPtr(buf, startSection2Offset + 0x18, true)) = (mListTextureUnit.at(i)->mListKeyframe.size()) ? (offset - startSection2Offset - 0xC) : 0;		//Section2_b_02->offsetSection2_02_c;
		offset += mListTextureUnit.at(i)->write_Animation(buf + offset, size - offset);
	}

	return offset;
}
/*-------------------------------------------------------------------------------\
|                             write_Main										 |
\-------------------------------------------------------------------------------*/
size_t Etr_Part::write_Main(uint8_t *buf, size_t size)
{
	ETR_Section1* section1 = (ETR_Section1*)buf;
	
	strcpy(&(section1->boneName[0]), mAttachedBoneName.c_str());

	section1->unk_0_0 = unk_0_0;
	section1->unk_0_1 = unk_0_1;
	section1->unk_0_2 = unk_0_2;
	section1->unk_0_3 = unk_0_3;
	section1->unk_0_4 = unk_0_4;
	section1->unk_0_5 = unk_0_5;
	section1->unk_0_6 = unk_0_6;
	section1->unk_0_7 = unk_0_7;
	section1->unk_0_8 = mPositionOffset_X;
	section1->unk_0_9 = mPositionOffset_Y;
	section1->unk_0_10 = mPositionOffset_Z;
	section1->unk_0_11 = mRotation_Pitch * 3.14159265358979f / 180.0f;
	section1->unk_0_12 = mRotation_Yaw * 3.14159265358979f / 180.0f;
	section1->unk_0_13 = mRotation_Roll * 3.14159265358979f / 180.0f;
	section1->unk_1 = unk_1;
	section1->unk_1b = mTimeStartExtrude;
	section1->unk_2 = unk_2;
	section1->unk_2b = mNumberFrameForNextSegment;
	section1->unk_3 = mDurationExtrude;
	
	unk_4_b[0] = mAutoOrientation;
	unk_4_b[2] = mNoDegrade;
	unk_4_b[3] = mDoublePointOfMiddleSection;
	unk_4b_b[0] = mDisplayMiddleSection;
	unk_4b_b[1] = mUVPauseOnExtrude;
	unk_5_b[0] = mUVPauseOnHold;

	section1->unk_4 = 0;
	section1->unk_4b = 0;
	section1->unk_5 = 0;
	for (size_t i = 0; i < 8; i++)
	{
		section1->unk_4 += 	((unk_4_b[i]) ? 1 : 0) << i;
		section1->unk_4b += ((unk_4b_b[i]) ? 1 : 0) << i;
		section1->unk_5 += ((unk_5_b[i]) ? 1 : 0) << i;
	}

	section1->unk_6 = unk_6;
	section1->unk_7 = mPositionExtrude_Z;
	section1->unk_9 = mScaleShape;
	section1->unk_11 = mDurationHold;
	section1->unk_13 = unk_13;
	section1->emmMaterialId = (mEmmMaterialId != (size_t)-1) ? mEmmMaterialId : 0xFFFF;
	section1->colorStart[0] = mColor0[0];
	section1->colorStart[1] = mColor0[1];
	section1->colorStart[2] = mColor0[2];
	section1->colorStart[3] = mColor0[3];
	section1->colorEnd[0] = mColor1[0];
	section1->colorEnd[1] = mColor1[1];
	section1->colorEnd[2] = mColor1[2];
	section1->colorEnd[3] = mColor1[3];

	section1->numberSection2 = mListTextureUnit.size();
	section1->numberPointsExtrudePathSection3 = mExtrudePath.size();
	section1->numberSegmentsExtrudeShapeSection3 = (mExtrudeLineShape.size()>0) ? mExtrudeLineShape.size()-1 : 0;
	section1->numberSection4 = mListAnimationB.size();
	section1->numberSection5 = mListAnimationA.size();

	return sizeof(ETR_Section1);
}
/*-------------------------------------------------------------------------------\
|                             write_AnimationsA									 |
\-------------------------------------------------------------------------------*/
size_t Etr_Part::write_AnimationsA(uint8_t *buf, size_t size)
{
	size_t offset = 0;
	
	size_t nbAnim = mListAnimationA.size();
	for (size_t i = 0; i < nbAnim; i++)
		offset += mListAnimationA.at(i).write_Main(buf + offset, size - offset);
	

	size_t startSection5Offset = 0;
	for (size_t i = 0; i < nbAnim; i++)
	{
		if (mListAnimationA.at(i).mListKeyframe.size() == 0)
			continue;

		startSection5Offset = i * sizeof(ETR_Section5);

		*((uint32_t*)((uint8_t *)buf + startSection5Offset + 0x8)) = offset - startSection5Offset;		//setup section5->offsetKeyFrames;
		offset += mListAnimationA.at(i).write_KeyframeTimeindex(buf + offset, size - offset);


		*((uint32_t*)((uint8_t *)buf + startSection5Offset + 0xC)) = offset - startSection5Offset;		//setup section5->offsetFloats;
		offset += mListAnimationA.at(i).write_KeyframeValues(buf + offset, size - offset);


		offset += mListAnimationA.at(i).write_EmptyIndexes(buf + offset, size - offset);
	}

	return offset;
}
/*-------------------------------------------------------------------------------\
|                             write_Main										 |
\-------------------------------------------------------------------------------*/
size_t Etr_AnimationA::write_Main(uint8_t *buf, size_t size)
{
	ETR_Section5* section5 = (ETR_Section5*)buf;
	section5->unk_0 = (size_t)mStartAnimation;
	section5->unk_0b = unk_0b;

	section5->component = mComponent + (unk_1b << 4);
	section5->unk_1 = unk_1;
	section5->unk_2 = unk_2;
	section5->numberKeyFrames = mListKeyframe.size();

	return sizeof(ETR_Section5);
}
/*-------------------------------------------------------------------------------\
|                             write_KeyframeTimeindex							 |
\-------------------------------------------------------------------------------*/
size_t Etr_AnimationA::write_KeyframeTimeindex(uint8_t *buf, size_t size)
{
	size_t offset = 0;

	uint16_t* timeIndex = (uint16_t*)buf;

	size_t nbKeyframes = mListKeyframe.size();
	for (size_t i = 0; i < nbKeyframes; i++)
		timeIndex[i] = mListKeyframe.at(i).mTime;

	offset += nbKeyframes * sizeof(uint16_t);
	if (offset & 0x3)
		offset += (0x4 - (offset & 0x3));

	return offset;
}
/*-------------------------------------------------------------------------------\
|                             write_KeyframeValues								 |
\-------------------------------------------------------------------------------*/
size_t Etr_AnimationA::write_KeyframeValues(uint8_t *buf, size_t size)
{
	size_t offset = 0;

	float* values = (float*)buf;

	size_t nbKeyframes = mListKeyframe.size();
	for (size_t i = 0; i < nbKeyframes; i++)
		values[i] = mListKeyframe.at(i).mValue;

	offset += nbKeyframes * sizeof(float);
	
	return offset;
}
/*-------------------------------------------------------------------------------\
|                             write_EmptyIndexes								 |
\-------------------------------------------------------------------------------*/
size_t Etr_AnimationA::write_EmptyIndexes(uint8_t *buf, size_t size)
{
	size_t offset = 0;
	if (mListKeyframe.size() < 2)
		return 0;


	uint16_t* indexes = (uint16_t*)buf;

	for (size_t i = 0; i < 102; i++)
		indexes[i] = 0;
	offset += 102 * sizeof(uint16_t);

	return offset;
}
/*-------------------------------------------------------------------------------\
|                             write_AnimationsB									 |
\-------------------------------------------------------------------------------*/
size_t Etr_Part::write_AnimationsB(uint8_t *buf, size_t size)
{
	size_t offset = 0;

	size_t nbAnim = mListAnimationB.size();
	for (size_t i = 0; i < nbAnim; i++)
		offset += mListAnimationB.at(i).write_Main(buf + offset, size - offset);

	size_t startSection4Offset = 0;
	nbAnim = mListAnimationB.size();
	for (size_t i = 0; i < nbAnim; i++)
	{
		if (mListAnimationB.at(i).mAnimations.size() == 0)
			continue;

		startSection4Offset = i * sizeof(ETR_Section4);

		*((uint32_t*)((uint8_t *)buf + startSection4Offset + 0x4)) = offset - startSection4Offset;		//setup section4->offsetSection4_b;
		offset += mListAnimationB.at(i).write_Animations(buf + offset, size - offset);
	}

	return offset;
}
/*-------------------------------------------------------------------------------\
|                             write_Main										 |
\-------------------------------------------------------------------------------*/
size_t Etr_AnimationB::write_Main(uint8_t *buf, size_t size)
{
	ETR_Section4* section4 = (ETR_Section4*)buf;
	section4->unk_0 = unk_0;
	section4->unk_0b = unk_0b;
	section4->nbSection4_b = mAnimations.size();

	return sizeof(ETR_Section4);
}
/*-------------------------------------------------------------------------------\
|                             write_Animations									 |
\-------------------------------------------------------------------------------*/
size_t Etr_AnimationB::write_Animations(uint8_t *buf, size_t size)
{
	size_t offset = 0;

	size_t nbAnim = mAnimations.size();
	for (size_t i = 0; i < nbAnim; i++)
		offset += mAnimations.at(i).write_Main(buf + offset, size - offset);

	size_t startSection5Offset = 0;
	nbAnim = mAnimations.size();
	for (size_t i = 0; i < nbAnim; i++)
	{
		if (mAnimations.at(i).mListKeyframe.size() == 0)
			continue;

		startSection5Offset = i * sizeof(ETR_Section5);

		*((uint32_t*)((uint8_t *)buf + startSection5Offset + 0x8)) = offset - startSection5Offset;		//setup section5->offsetKeyFrames;
		offset += mAnimations.at(i).write_KeyframeTimeindex(buf + offset, size - offset);


		*((uint32_t*)((uint8_t *)buf + startSection5Offset + 0xC)) = offset - startSection5Offset;		//setup section5->offsetFloats;
		offset += mAnimations.at(i).write_KeyframeValues(buf + offset, size - offset);

		offset += mAnimations.at(i).write_EmptyIndexes(buf + offset, size - offset);
	}

	return offset;
}
/*-------------------------------------------------------------------------------\
|                             write_TextureUnitPointers							 |
\-------------------------------------------------------------------------------*/
size_t Etr_Part::write_TextureUnitPointers(uint8_t *buf, size_t size, size_t startOffsetSection2, std::vector<Etr_TextureUnit*> &listTextureUnit)
{
	uint32_t* pointer = (uint32_t*)buf;

	size_t nbTextInEtr = listTextureUnit.size();
	size_t nbText = mListTextureUnit.size();
	for (size_t i = 0; i < nbText; i++)
	{
		Etr_TextureUnit* tu = mListTextureUnit.at(i);

		size_t isFound = (size_t)-1;
		for (size_t j = 0; j < nbTextInEtr; j++)
		{
			if (tu == listTextureUnit.at(j))
			{
				isFound = j;
				break;
			}
		}
		
		pointer[i] = (isFound != (size_t)-1) ? (startOffsetSection2 + isFound * sizeof(ETR_Section2)) : 0;
	}

	return nbText * sizeof(uint32_t);
}
/*-------------------------------------------------------------------------------\
|                             write_Extrude										 |
\-------------------------------------------------------------------------------*/
size_t Etr_Part::write_Extrude(uint8_t *buf, size_t size)
{
	float* floats = (float*)buf;

	size_t inc = 0;
	floats[inc++] = mEndExtrude_WorldScale_factor;
	floats[inc++] = mEndExtrude_WorldScale_add;
	floats[inc++] = mEndExtrude_WorldOffset_factor;
	floats[inc++] = mEndExtrude_WorldOffset_factorb;

	size_t nbPoints = mExtrudePath.size();
	for (size_t i = 0; i < nbPoints; i++)
	{
		Point &point = mExtrudePath.at(i);
		floats[inc++] = point.x;
		floats[inc++] = point.y;
		floats[inc++] = point.z;
		floats[inc++] = point.w;
	}

	nbPoints = mExtrudeLineShape.size();
	for (size_t i = 0; i < nbPoints; i++)
	{
		Point &point = mExtrudeLineShape.at(i);
		floats[inc++] = point.x;
		floats[inc++] = point.y;
	}

	return 4 * sizeof(float) + 4 * sizeof(float) * mExtrudePath.size() + 2 * sizeof(float) * mExtrudeLineShape.size();
}
/*-------------------------------------------------------------------------------\
|                             write_Main										 |
\-------------------------------------------------------------------------------*/
size_t Etr_TextureUnit::write_Main(uint8_t *buf, size_t size)
{
	ETR_Section2* section2 = (ETR_Section2*)buf;

	section2->unk_0 = unk_0;
	section2->embTextureId = (mEmbTextureId != (size_t)-1) ? mEmbTextureId : 0xFF;
	section2->unk_1 = (((size_t)mTypeRepetition_U) << 4) + ((size_t)mTypeRepetition_V);
	section2->unk_1b = (((size_t)mMinFiltering) << 4) + ((size_t)mMaxFiltering);;
	section2->unk_2 = unk_2;
	section2->unk_3 = unk_3;

	size_t type = (!mUseSpeed) ? ((mListKeyframe.size()>1) ? 2 : 0)  : 1;

	section2->typeSection2_b = type;

	switch (type)
	{
	case 0:
		{
			if (mListKeyframe.size())
			{
				ETR_Section2_b_00* section2b = (ETR_Section2_b_00*)((uint8_t*)buf + 0xC);

				Keyframe &kf = mListKeyframe.at(0);
				section2b->color[0] = kf.scroll_u;
				section2b->color[1] = kf.scroll_v;
				section2b->color[2] = kf.scale_u;
				section2b->color[3] = kf.scale_v;
			}
		}
		break;

	case 1:
		{
			ETR_Section2_b_01* section2b = (ETR_Section2_b_01*)((uint8_t*)buf + 0xC);
			section2b->color[0] = mScrollSpeed_U;
			section2b->color[1] = mScrollSpeed_V;
			section2b->color[2] = 0.0f;
			section2b->color[3] = 0.0f;
		}
		break;

	case 2:
		{
			ETR_Section2_b_02* section2b = (ETR_Section2_b_02*)((uint8_t*)buf + 0xC);
			section2b->unk_0 = unk_2_0;
			section2b->unk_1 = unk_2_1;
			section2b->unk_2 = unk_2_2;
			section2b->numberSection2_02_c = mListKeyframe.size();
		}
		break;
	}

	return sizeof(ETR_Section2);
}
/*-------------------------------------------------------------------------------\
|                             write_Animation									 |
\-------------------------------------------------------------------------------*/
size_t Etr_TextureUnit::write_Animation(uint8_t *buf, size_t size)
{
	size_t offset = 0;

	size_t nbKf = mListKeyframe.size();
	for (size_t i = 0; i < nbKf; i++)
	{
		Keyframe &kf = mListKeyframe.at(i);
		
		ETR_Section2_b_02_c* section2c = (ETR_Section2_b_02_c*)((uint8_t*)buf + offset);

		section2c->duration = kf.timeIndex;
		section2c->color[0] = kf.scroll_u;
		section2c->color[1] = kf.scroll_v;
		section2c->color[2] = kf.scale_u;
		section2c->color[3] = kf.scale_v;

		offset += sizeof(ETR_Section2_b_02_c);
	}

	return offset;
}




















/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/*-------------------------------------------------------------------------------\
|                             saveXml											 |
\-------------------------------------------------------------------------------*/
void Etr::saveXml(string filename)
{
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement* rootNode = export_Xml();
	doc.LinkEndChild(rootNode);

	doc.SaveFile(filename);
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* Etr::export_Xml()
{
	TiXmlElement* mainNode = new TiXmlElement("Etr");
	
	TiXmlElement* node = new TiXmlElement("Parts");
	size_t nbPart = mListPart.size();
	for (size_t i = 0; i < nbPart; i++)
		node->LinkEndChild(mListPart.at(i).export_Xml(mListTextureUnit));
	mainNode->LinkEndChild(node);


	node = new TiXmlElement("TextureUnits");
	size_t nbText = mListTextureUnit.size();
	for (size_t i = 0; i < nbText; i++)
		node->LinkEndChild(mListTextureUnit.at(i)->export_Xml());
	mainNode->LinkEndChild(node);


	return mainNode;
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* Etr_TextureUnit::export_Xml()
{
	TiXmlElement* mainNode = new TiXmlElement("TextureUnit");
	TiXmlElement* node;
	TiXmlComment* comment;
	string str;

	mainNode->SetAttribute("EmbTextureId", (mEmbTextureId!=(size_t)-1) ?  EMO_BaseFile::UnsignedToString(mEmbTextureId, false) : "None");

	
	node = new TiXmlElement("TypeRepetition");
	{
		switch (mTypeRepetition_U)
		{
		case TR_Wrap2: str = "Wrap2"; break;
		case TR_Wrap: str = "Wrap"; break;
		case TR_Mirror: str = "Mirror"; break;
		case TR_Border: str = "Border"; break;
		}
		node->SetAttribute("U", str);
		
		switch (mTypeRepetition_V)
		{
		case TR_Wrap2: str = "Wrap2"; break;
		case TR_Wrap: str = "Wrap"; break;
		case TR_Mirror: str = "Mirror"; break;
		case TR_Border: str = "Border"; break;
		}
		node->SetAttribute("V", str);
	}
	comment = new TiXmlComment("Wrap2, Wrap, Mirror, Border"); mainNode->LinkEndChild(comment);
	mainNode->LinkEndChild(node);


	node = new TiXmlElement("Filtering");
	{
		switch (mMinFiltering)
		{
		case TF_None: str = "None"; break;
		case TR_None2: str = "None2"; break;
		case TR_Linear: str = "Linear"; break;
		}
		node->SetAttribute("Min", str);

		switch (mMaxFiltering)
		{
		case TF_None: str = "None"; break;
		case TR_None2: str = "None2"; break;
		case TR_Linear: str = "Linear"; break;
		}
		node->SetAttribute("Max", str);
	}
	comment = new TiXmlComment("None, None2, Linear"); mainNode->LinkEndChild(comment);
	mainNode->LinkEndChild(node);


	node = new TiXmlElement("unk_0"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_0, true));  comment = new TiXmlComment("always 0"); mainNode->LinkEndChild(comment);  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_2"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_2, true));  comment = new TiXmlComment("always 0"); mainNode->LinkEndChild(comment);  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_3"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(unk_3, true));  comment = new TiXmlComment("only 0 (most) or 1"); mainNode->LinkEndChild(comment);  mainNode->LinkEndChild(node);
	



	
	TiXmlElement* node_anim = new TiXmlElement("ScrollAnimation");
	{
		node_anim->SetAttribute("UseSpeedInsteadKeyFrames", mUseSpeed ? "True" : "False");
		
		node = new TiXmlElement("ScrollSpeed"); node->SetAttribute("U", std::to_string(mScrollSpeed_U)); node->SetAttribute("V", std::to_string(mScrollSpeed_V)); node_anim->LinkEndChild(node);

		node = new TiXmlElement("unk_2_0"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_2_0, true)); comment = new TiXmlComment("only if there is many Keyframes. 0x0 : no interpolation, 0x8 : no animation, 0xa : no animation, 0xc : no animation ...., 0x18 :  the animation is done twice (with no interpolation)  ?, 0x20 :  same but still on the end of animation this time  (instead come back to no animation ...)  ? . values found 0x0 (most),0x8, 0xa, 0xc, 0x10, 0x12, 0x18, 0x20."); node_anim->LinkEndChild(comment); node_anim->LinkEndChild(node);
		node = new TiXmlElement("unk_2_1"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_2_1, true)); comment = new TiXmlComment("only if there is many Keyframes. always 0"); node_anim->LinkEndChild(comment); node_anim->LinkEndChild(node);
		node = new TiXmlElement("unk_2_2"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(unk_2_2, true)); comment = new TiXmlComment("only if there is many Keyframes. always 0"); node_anim->LinkEndChild(comment); node_anim->LinkEndChild(node);
		


		size_t type = (!mUseSpeed) ? ((mListKeyframe.size() > 1) ? 2 : 0) : 1;

		node = new TiXmlElement("Keyframes");
		TiXmlElement* node_tmp;

		size_t nbKeyframe = mListKeyframe.size();
		for (size_t i = 0; i < nbKeyframe; i++)
		{
			Keyframe &keyframe = mListKeyframe.at(i);

			node_tmp = new TiXmlElement("Keyframe");

			node_tmp->SetAttribute("timeIndex", EMO_BaseFile::UnsignedToString(keyframe.timeIndex, false));
			node_tmp->SetAttribute("scroll_u", std::to_string(keyframe.scroll_u));
			node_tmp->SetAttribute("scroll_v", std::to_string(keyframe.scroll_v));
			node_tmp->SetAttribute("scale_u", std::to_string(keyframe.scale_u));
			node_tmp->SetAttribute("scale_v", std::to_string(keyframe.scale_v));
			
			node->LinkEndChild(node_tmp);
		}
		node_anim->LinkEndChild(node);
	}
	mainNode->LinkEndChild(node_anim);

	return mainNode;
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* Etr_Part::export_Xml(std::vector<Etr_TextureUnit*> &listTextureUnit)
{
	TiXmlElement* mainNode = new TiXmlElement("Part");

	TiXmlElement* node;
	TiXmlComment* comment;
	node = new TiXmlElement("AttachedBone"); node->SetAttribute("Name", mAttachedBoneName);  mainNode->LinkEndChild(node);
	
	node = new TiXmlElement("TimeStartExtrude"); node->SetAttribute("value", EMO_BaseFile::UnsignedToString(mTimeStartExtrude, false));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("DurationExtrude"); node->SetAttribute("value", EMO_BaseFile::UnsignedToString(mDurationExtrude, false));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("UVPauseOnExtrude"); node->SetAttribute("value", mUVPauseOnExtrude ? "True" : "False"); mainNode->LinkEndChild(node);

	node = new TiXmlElement("NumberFrameForNextSegment"); node->SetAttribute("value", EMO_BaseFile::UnsignedToString(mNumberFrameForNextSegment, false)); comment = new TiXmlComment("127 maximum (because int8). becarefull, each segment have a Uv repetition (in V). So that could change a lot the visualisation."); mainNode->LinkEndChild(comment); mainNode->LinkEndChild(node);
	node = new TiXmlElement("DoublePointOfMiddleSection"); node->SetAttribute("value", mDoublePointOfMiddleSection ? "True" : "False"); mainNode->LinkEndChild(node);

	node = new TiXmlElement("DurationHold"); node->SetAttribute("value", EMO_BaseFile::UnsignedToString(mDurationHold, false)); mainNode->LinkEndChild(node);
	node = new TiXmlElement("UVPauseOnHold"); node->SetAttribute("value", mUVPauseOnHold ? "True" : "False"); mainNode->LinkEndChild(node);
	

	node = new TiXmlElement("PositionOffset"); node->SetAttribute("X", std::to_string(mPositionOffset_X)); node->SetAttribute("Y", std::to_string(mPositionOffset_Y)); node->SetAttribute("Z", std::to_string(mPositionOffset_Z)); mainNode->LinkEndChild(node);
	node = new TiXmlElement("Rotation"); node->SetAttribute("Pitch", std::to_string(mRotation_Pitch)); node->SetAttribute("Yaw", std::to_string(mRotation_Yaw));  node->SetAttribute("Roll", std::to_string(mRotation_Roll)); mainNode->LinkEndChild(node);
	node = new TiXmlElement("AutoOrientation"); node->SetAttribute("value", mAutoOrientation ? "True" : "False"); mainNode->LinkEndChild(node);
	node = new TiXmlElement("PositionExtrude_Z"); node->SetAttribute("value", std::to_string(mPositionExtrude_Z));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("ScaleShape"); node->SetAttribute("value", std::to_string(mScaleShape));  mainNode->LinkEndChild(node);


	node = new TiXmlElement("EmmMaterial"); node->SetAttribute("Id", (mEmmMaterialId != (size_t)-1) ? EMO_BaseFile::UnsignedToString(mEmmMaterialId, false) : "None");  mainNode->LinkEndChild(node);
	node = new TiXmlElement("Color0"); node->SetAttribute("R", std::to_string(mColor0[0])); node->SetAttribute("G", std::to_string(mColor0[1])); node->SetAttribute("B", std::to_string(mColor0[2])); node->SetAttribute("A", std::to_string(mColor0[3]));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("Color1"); node->SetAttribute("R", std::to_string(mColor1[0])); node->SetAttribute("G", std::to_string(mColor1[1])); node->SetAttribute("B", std::to_string(mColor1[2])); node->SetAttribute("A", std::to_string(mColor1[3]));  mainNode->LinkEndChild(node);
	node = new TiXmlElement("NoDegrade"); node->SetAttribute("value", mNoDegrade ? "True" : "False"); mainNode->LinkEndChild(node);
	node = new TiXmlElement("DisplayMiddleSection"); node->SetAttribute("value", mDisplayMiddleSection ? "True" : "False"); mainNode->LinkEndChild(node);




	TiXmlElement* node_tus = new TiXmlElement("TextureUnits");
	size_t nbTextInEtr = listTextureUnit.size();

	size_t nbText = mListTextureUnit.size();
	for (size_t i = 0; i < nbText; i++)
	{
		Etr_TextureUnit* tu = mListTextureUnit.at(i);

		size_t isFound = (size_t)-1;
		for (size_t j = 0; j < nbTextInEtr; j++)
		{
			if (tu == listTextureUnit.at(j))
			{
				isFound = j;
				break;
			}
		}
		
		if (isFound==(size_t)-1)
			continue;
		
		node = new TiXmlElement("TextureUnit"); node->SetAttribute("Index", std::to_string(isFound));  node_tus->LinkEndChild(node);
	}
	mainNode->LinkEndChild(node_tus);





	TiXmlElement* node_extr = new TiXmlElement("Extrude");

	{
		TiXmlElement* node_shape = new TiXmlElement("LineShape");

		size_t nbPoints = mExtrudeLineShape.size();
		for (size_t i = 0; i < nbPoints; i++)
		{
			Point &point = mExtrudeLineShape.at(i);
			node = new TiXmlElement("Point"); node->SetAttribute("X", std::to_string(point.x)); node->SetAttribute("Y", std::to_string(point.y)); node_shape->LinkEndChild(node);
		}


		comment = new TiXmlComment("you can draw a shape witch will be extrude.by add some 'Point' with a  X and Y coordinates(Uv coordinate is made by the game, by apply U on all along the shape.V is on a segment of the extrude, it's other parameters).\n   if AutoOrientation is True, X-Axis will be rotated go target the camera position. Else, X-Axis is left of Character (for positive values), and Y-Axis is bottom");
		node_extr->LinkEndChild(comment);

		node_extr->LinkEndChild(node_shape);
	}


	
	{
		TiXmlElement* node_path = new TiXmlElement("Path");

		size_t nbPoints = mExtrudePath.size();
		if (nbPoints > 0)
		{
			for (int i = nbPoints - 1; i >= 0; i--)		//order inversed to be more easier to understand. because it's allready inversed.
			{
				Point &point = mExtrudePath.at(i);
				node = new TiXmlElement("Point"); node->SetAttribute("WorldScaleFactor", std::to_string(point.x)); node->SetAttribute("WorldScaleAdd", std::to_string(point.y)); node->SetAttribute("WorldOffsetFactor", std::to_string(point.z)); node->SetAttribute("WorldOffsetFactorb", std::to_string(point.w));  node_path->LinkEndChild(node);
			}
		}

		comment = new TiXmlComment("Path and EndPoint are complex (some kind of scale from projection from center of the scene). The 'Point' in Path have the same definition as EndPoint. advice you to not touch on it.");
		node_extr->LinkEndChild(comment);

		node_extr->LinkEndChild(node_path);
	}
	node = new TiXmlElement("EndPoint"); node->SetAttribute("WorldScaleFactor", std::to_string(mEndExtrude_WorldScale_factor)); node->SetAttribute("WorldScaleAdd", std::to_string(mEndExtrude_WorldScale_add)); node->SetAttribute("WorldOffsetFactor", std::to_string(mEndExtrude_WorldOffset_factor)); node->SetAttribute("WorldOffsetFactorb", std::to_string(mEndExtrude_WorldOffset_factorb));  node_extr->LinkEndChild(node);

	mainNode->LinkEndChild(node_extr);

	


	TiXmlElement* node_anim = new TiXmlElement("AnimationAList");

	size_t nbAnim = mListAnimationA.size();
	for (size_t i = 0; i < nbAnim; i++)
		node_anim->LinkEndChild(mListAnimationA.at(i).export_Xml());

	comment = new TiXmlComment("In animations, parameter could be 'Color_Factor' with R, G, B or A components.\nStartAnimation : EndWithNoInterpolation, FirstMiddlePoint, Start");
	mainNode->LinkEndChild(comment);
	mainNode->LinkEndChild(node_anim);



	node_anim = new TiXmlElement("AnimationBList");
	nbAnim = mListAnimationB.size();
	for (size_t i = 0; i < nbAnim; i++)
		node_anim->LinkEndChild(mListAnimationB.at(i).export_Xml());
	mainNode->LinkEndChild(node_anim);



	


	//list unknow values.
	TiXmlElement* node_unknow = new TiXmlElement("Unknows");


	node = new TiXmlElement("unk_0_0"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_0_0, true));  comment = new TiXmlComment("values found 0x0 (most), 0x5f4c5f67, 0x535254, 0x5f4c5f62"); node_unknow->LinkEndChild(comment); node_unknow->LinkEndChild(node);
	node = new TiXmlElement("unk_0_1"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_0_1, true));  comment = new TiXmlComment("values found 0x0 (most), 0x646e6148"); node_unknow->LinkEndChild(comment); node_unknow->LinkEndChild(node);
	node = new TiXmlElement("unk_0_2"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_0_2, true));  comment = new TiXmlComment("always 0"); node_unknow->LinkEndChild(comment); node_unknow->LinkEndChild(node);
	node = new TiXmlElement("unk_0_3"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_0_3, true));  comment = new TiXmlComment("always 0"); node_unknow->LinkEndChild(comment); node_unknow->LinkEndChild(node);
	node = new TiXmlElement("unk_0_4"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_0_4, true));  comment = new TiXmlComment("always 0"); node_unknow->LinkEndChild(comment); node_unknow->LinkEndChild(node);
	node = new TiXmlElement("unk_0_5"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_0_5, true));  comment = new TiXmlComment("always 0"); node_unknow->LinkEndChild(comment); node_unknow->LinkEndChild(node);
	node = new TiXmlElement("unk_0_6"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_0_6, true));  comment = new TiXmlComment("always 0"); node_unknow->LinkEndChild(comment); node_unknow->LinkEndChild(node);
	node = new TiXmlElement("unk_0_7"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_0_7, true));  comment = new TiXmlComment("always 0"); node_unknow->LinkEndChild(comment); node_unknow->LinkEndChild(node);

	node = new TiXmlElement("unk_1"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_1, true));  comment = new TiXmlComment("values found 0, 1, 4"); node_unknow->LinkEndChild(comment); node_unknow->LinkEndChild(node);	
	node = new TiXmlElement("unk_2"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_2, true));  comment = new TiXmlComment("range found [0, 0x10], 1 at most, not have 7, and have 0x14, 0x1E and 0x80 (0x80 could be the unique negative value -1 if int, instead of uint)."); node_unknow->LinkEndChild(comment); node_unknow->LinkEndChild(node);
	
	TiXmlElement* node_unk4 = new TiXmlElement("unk_4");
	TiXmlElement* node_unk4_b = new TiXmlElement("unk_4b");
	TiXmlElement* node_unk5 = new TiXmlElement("unk_5");
	for (int i = 7; i >= 0; i--)					//we inverse the way to have the left bit at left
	{
		if((i!=0)&& (i != 2)&& (i != 3))
			node_unk4->SetAttribute("b"+ std::to_string(i), (unk_4_b[i]) ? 1 : 0);

		if(i>=2)
			node_unk4_b->SetAttribute("b" + std::to_string(i), (unk_4b_b[i]) ? 1 : 0);
		
		if(i>=1)
			node_unk5->SetAttribute("b" + std::to_string(i), (unk_5_b[i]) ? 1 : 0 );
	}
	comment = new TiXmlComment(" b7,b6,b5 always 0"); node_unknow->LinkEndChild(comment);
	node_unknow->LinkEndChild(node_unk4);
	comment = new TiXmlComment(" b7,b6,b5,b4,b3,b2 always 0"); node_unknow->LinkEndChild(comment);
	node_unknow->LinkEndChild(node_unk4_b);
	comment = new TiXmlComment(" b7,b6,b5,b4,b3 always 0"); node_unknow->LinkEndChild(comment);
	node_unknow->LinkEndChild(node_unk5);


	node = new TiXmlElement("unk_6"); node->SetAttribute("u32", EMO_BaseFile::UnsignedToString(unk_6, true));  comment = new TiXmlComment("0x45 (most), 0xc7, 0x47, 0x4f. When you use another values, we see something strange when we hit something. look like graphic bug or a Texture bad setup."); node_unknow->LinkEndChild(comment); node_unknow->LinkEndChild(node);
	node = new TiXmlElement("unk_13"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(unk_13, true));  comment = new TiXmlComment("always 1"); node_unknow->LinkEndChild(comment); node_unknow->LinkEndChild(node);

	mainNode->LinkEndChild(node_unknow);

	return mainNode;
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* Etr_AnimationA::export_Xml()
{
	TiXmlElement* mainNode = new TiXmlElement("Animation");
	TiXmlElement* node;
	TiXmlComment* comment;

	//						// finally all value on unk_1 is the same result , so no parameters here
	{
		mainNode->SetAttribute("Parameter", "Color_Factor");

		if ((mComponent >= 0) && (mComponent <= 3))
		{
			switch (mComponent)
			{
			case 0: mainNode->SetAttribute("Component", "R");  break;
			case 1: mainNode->SetAttribute("Component", "G");  break;
			case 2: mainNode->SetAttribute("Component", "B");  break;
			case 3: mainNode->SetAttribute("Component", "A");  break;
			}
		}else {
			mainNode->SetAttribute("Component", EMO_BaseFile::UnsignedToString(mComponent, false));			//by default, if we don't have the information, use the number.
		}

	}


	string str = "";
	switch (mStartAnimation)
	{
	case SA_EndWithNoInterpolation: str = "EndWithNoInterpolation"; break;
	case SA_FirstMiddlePoint: str = "FirstMiddlePoint"; break;
	case SA_Start: str = "Start"; break;
	}
	node = new TiXmlElement("StartAnimation"); node->SetAttribute("At", str);  mainNode->LinkEndChild(node);

	node = new TiXmlElement("unk_0b"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_0b, true));  comment = new TiXmlComment("0x0 (most) 0x1, 0x2. with 0x1 and 0x2 there is no noAnimations."); mainNode->LinkEndChild(comment);  mainNode->LinkEndChild(node);	
	node = new TiXmlElement("unk_1b"); node->SetAttribute("u4", EMO_BaseFile::UnsignedToString(unk_1b, true));  comment = new TiXmlComment("always 1."); mainNode->LinkEndChild(comment);  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_1"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_1, true));  comment = new TiXmlComment("always 9."); mainNode->LinkEndChild(comment); mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_2"); node->SetAttribute("u16", EMO_BaseFile::UnsignedToString(unk_2, true)); comment = new TiXmlComment("always 0."); mainNode->LinkEndChild(comment);  mainNode->LinkEndChild(node);

	TiXmlElement* node_kf = new TiXmlElement("Keyframes");

	size_t nbKeyframe = mListKeyframe.size();
	for (size_t i = 0; i < nbKeyframe; i++)
	{
		Keyframe &keyframe = mListKeyframe.at(i);

		node = new TiXmlElement("Keyframe");
		node->SetAttribute("TimeIndex", std::to_string(keyframe.mTime));
		node->SetAttribute("Value", std::to_string(keyframe.mValue));
		node_kf->LinkEndChild(node);
	}

	mainNode->LinkEndChild(node_kf);

	return mainNode;
}
/*-------------------------------------------------------------------------------\
|                             export_Xml					                     |
\-------------------------------------------------------------------------------*/
TiXmlElement* Etr_AnimationB::export_Xml()
{
	TiXmlElement* mainNode = new TiXmlElement("AnimationB");
	TiXmlElement* node;
	TiXmlComment* comment;

	node = new TiXmlElement("unk_0"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_0, true));  comment = new TiXmlComment("only 2 or 4 (most)."); mainNode->LinkEndChild(comment);  mainNode->LinkEndChild(node);
	node = new TiXmlElement("unk_0b"); node->SetAttribute("u8", EMO_BaseFile::UnsignedToString(unk_0b, true));  comment = new TiXmlComment("only 0 (most) or 1."); mainNode->LinkEndChild(comment);  mainNode->LinkEndChild(node);


	TiXmlElement* node_anim = new TiXmlElement("AnimationList");

	size_t nbAnim = mAnimations.size();
	for (size_t i = 0; i < nbAnim; i++)
		node_anim->LinkEndChild(mAnimations.at(i).export_Xml());

	mainNode->LinkEndChild(node_anim);

	return mainNode;
}








//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------\
|                             Compile				                             |
\-------------------------------------------------------------------------------*/
bool Etr::Compile(TiXmlDocument *doc, bool big_endian)
{
	this->big_endian = big_endian;

	TiXmlHandle handle(doc);
	TiXmlElement* rootNode = EMO_BaseFile::FindRoot(&handle, "Etr");

	if (!rootNode)
	{
		LOG_DEBUG("Cannot find\"Etr\" in xml.\n");
		return false;
	}

	return import_Xml(rootNode);
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool Etr::import_Xml(TiXmlElement* rootNode)
{
	Reset();

	TiXmlElement* partsNode = rootNode->FirstChildElement("Parts");
	if (!partsNode)
	{
		LOG_DEBUG("Cannot find\"Parts\" in xml.\n");
		return false;
	}

	TiXmlElement* textureUnitsNode = rootNode->FirstChildElement("TextureUnits");
	if (!textureUnitsNode)
	{
		LOG_DEBUG("Cannot find\"TextureUnits\" in xml.\n");
		return false;
	}


	//Parts make some reference to the TextureUnits, So we have to deal with TextureUnits first.
	for (TiXmlElement* node = textureUnitsNode->FirstChildElement("TextureUnit"); node; node = node->NextSiblingElement("TextureUnit"))
	{
		mListTextureUnit.push_back(new Etr_TextureUnit());
		if(!mListTextureUnit.back()->import_Xml(node))
			mListTextureUnit.pop_back();
	}

	for (TiXmlElement* node = partsNode->FirstChildElement("Part"); node; node = node->NextSiblingElement("Part"))
	{
		mListPart.push_back(Etr_Part());
		if (!mListPart.back().import_Xml(node, mListTextureUnit))
			mListPart.pop_back();
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool Etr_TextureUnit::import_Xml(TiXmlElement* node)
{
	Reset();
	
	node->QueryUnsignedAttribute("EmbTextureId", &mEmbTextureId);

	TiXmlElement* node_tmp;
	string str = "";
	

	node_tmp = node->FirstChildElement("unk_0"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_0 = EMO_BaseFile::GetUnsigned(str, unk_0);
	node_tmp = node->FirstChildElement("unk_2"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_2 = EMO_BaseFile::GetUnsigned(str, unk_2);
	node_tmp = node->FirstChildElement("unk_3"); if (node_tmp) node_tmp->QueryStringAttribute("u16", &str); unk_3 = EMO_BaseFile::GetUnsigned(str, unk_3);


	node_tmp = node->FirstChildElement("TypeRepetition");
	if (node_tmp)
	{
		node_tmp->QueryStringAttribute("U", &str);
		if (str == "Wrap2")
			mTypeRepetition_U = TR_Wrap2;
		else if (str == "Wrap")
			mTypeRepetition_U = TR_Wrap;
		else if (str == "Mirror")
			mTypeRepetition_U = TR_Mirror;
		else if (str == "Border")
			mTypeRepetition_U = TR_Border;

		node_tmp->QueryStringAttribute("V", &str);
		if (str == "Wrap2")
			mTypeRepetition_V = TR_Wrap2;
		else if (str == "Wrap")
			mTypeRepetition_V = TR_Wrap;
		else if (str == "Mirror")
			mTypeRepetition_V = TR_Mirror;
		else if (str == "Border")
			mTypeRepetition_V = TR_Border;
	}

	node_tmp = node->FirstChildElement("Filtering");
	if (node_tmp)
	{
		node_tmp->QueryStringAttribute("Min", &str);
		if (str == "None")
			mMinFiltering = TF_None;
		else if (str == "None2")
			mMinFiltering = TR_None2;
		else if (str == "Linear")
			mMinFiltering = TR_Linear;
		
		node_tmp->QueryStringAttribute("Max", &str);
		if (str == "None")
			mMaxFiltering = TF_None;
		else if (str == "None2")
			mMaxFiltering = TR_None2;
		else if (str == "Linear")
			mMaxFiltering = TR_Linear;
	}


	TiXmlElement* node_anim = node->FirstChildElement("ScrollAnimation");
	if (node_anim)
	{
		node_anim->QueryBoolAttribute("UseSpeedInsteadKeyFrames", &mUseSpeed);

		node_tmp = node_anim->FirstChildElement("ScrollSpeed"); 
		if (node_tmp)
		{
			node_tmp->QueryFloatAttribute("U", &mScrollSpeed_U);
			node_tmp->QueryFloatAttribute("V", &mScrollSpeed_V);
		}

		node_tmp = node_anim->FirstChildElement("unk_2_0"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_2_0 = EMO_BaseFile::GetUnsigned(str, unk_2_0);
		node_tmp = node_anim->FirstChildElement("unk_2_1"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_2_1 = EMO_BaseFile::GetUnsigned(str, unk_2_1);
		node_tmp = node_anim->FirstChildElement("unk_2_2"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_2_2 = EMO_BaseFile::GetUnsigned(str, unk_2_2);

		TiXmlElement* node_kf = node_anim->FirstChildElement("Keyframes");
		if (!node_kf)
			return true;

		size_t timeIndex = 0;
		float scroll_u = 0.0f;
		float scroll_v = 0.0f;
		float scale_u = 0.0f;
		float scale_v = 0.0f;
		for (node_tmp = node_kf->FirstChildElement("Keyframe"); node_tmp; node_tmp = node_tmp->NextSiblingElement("Keyframe"))
		{
			timeIndex = 0;
			node_tmp->QueryStringAttribute("timeIndex", &str); timeIndex = EMO_BaseFile::GetUnsigned(str, 0);
			node_tmp->QueryFloatAttribute("scroll_u", &scroll_u);
			node_tmp->QueryFloatAttribute("scroll_v", &scroll_v);
			node_tmp->QueryFloatAttribute("scale_u", &scale_u);
			node_tmp->QueryFloatAttribute("scale_v", &scale_v);

			mListKeyframe.push_back(Keyframe(timeIndex, scroll_u, scroll_v, scale_u, scale_v));
		}
	}
	
	return true;
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool Etr_Part::import_Xml(TiXmlElement* node, std::vector<Etr_TextureUnit*> &listTextureUnit)
{
	Reset();


	TiXmlElement* node_tmp;
	string str = "";
	node_tmp = node->FirstChildElement("AttachedBone"); if (node_tmp) node_tmp->QueryStringAttribute("Name", &mAttachedBoneName);
	node_tmp = node->FirstChildElement("EmmMaterial"); if (node_tmp) node_tmp->QueryStringAttribute("Id", &str); mEmmMaterialId = EMO_BaseFile::GetUnsigned(str, mEmmMaterialId);
	node_tmp = node->FirstChildElement("TimeStartExtrude"); if (node_tmp) node_tmp->QueryStringAttribute("value", &str); mTimeStartExtrude = EMO_BaseFile::GetUnsigned(str, mTimeStartExtrude);
	node_tmp = node->FirstChildElement("DurationExtrude"); if (node_tmp) node_tmp->QueryStringAttribute("value", &str); mDurationExtrude = EMO_BaseFile::GetUnsigned(str, mDurationExtrude);
	node_tmp = node->FirstChildElement("PositionExtrude_Z"); if (node_tmp) node_tmp->QueryFloatAttribute("value", &mPositionExtrude_Z);
	node_tmp = node->FirstChildElement("ScaleShape"); if (node_tmp) node_tmp->QueryFloatAttribute("value", &mScaleShape);
	node_tmp = node->FirstChildElement("DurationHold"); if (node_tmp) node_tmp->QueryStringAttribute("value", &str); mDurationHold = EMO_BaseFile::GetUnsigned(str, mDurationHold);
	node_tmp = node->FirstChildElement("NumberFrameForNextSegment"); if (node_tmp) node_tmp->QueryStringAttribute("value", &str); mNumberFrameForNextSegment = EMO_BaseFile::GetUnsigned(str, mNumberFrameForNextSegment);
	
	node_tmp = node->FirstChildElement("AutoOrientation"); if (node_tmp) node_tmp->QueryBoolAttribute("value", &mAutoOrientation);
	node_tmp = node->FirstChildElement("NoDegrade"); if (node_tmp) node_tmp->QueryBoolAttribute("value", &mNoDegrade);
	node_tmp = node->FirstChildElement("DoublePointOfMiddleSection"); if (node_tmp) node_tmp->QueryBoolAttribute("value", &mDoublePointOfMiddleSection);
	node_tmp = node->FirstChildElement("DisplayMiddleSection"); if (node_tmp) node_tmp->QueryBoolAttribute("value", &mDisplayMiddleSection);
	node_tmp = node->FirstChildElement("UVPauseOnExtrude"); if (node_tmp) node_tmp->QueryBoolAttribute("value", &mUVPauseOnExtrude);
	node_tmp = node->FirstChildElement("UVPauseOnHold"); if (node_tmp) node_tmp->QueryBoolAttribute("value", &mUVPauseOnHold);
	
	node_tmp = node->FirstChildElement("Color0");
	if (node_tmp)
	{
		node_tmp->QueryFloatAttribute("R", &mColor0[0]);
		node_tmp->QueryFloatAttribute("G", &mColor0[1]);
		node_tmp->QueryFloatAttribute("B", &mColor0[2]);
		node_tmp->QueryFloatAttribute("A", &mColor0[3]);
	}
	node_tmp = node->FirstChildElement("Color1");
	if (node_tmp)
	{
		node_tmp->QueryFloatAttribute("R", &mColor1[0]);
		node_tmp->QueryFloatAttribute("G", &mColor1[1]);
		node_tmp->QueryFloatAttribute("B", &mColor1[2]);
		node_tmp->QueryFloatAttribute("A", &mColor1[3]);
	}


	node_tmp = node->FirstChildElement("PositionOffset");
	if (node_tmp)
	{
		node_tmp->QueryFloatAttribute("X", &mPositionOffset_X);
		node_tmp->QueryFloatAttribute("Y", &mPositionOffset_Y);
		node_tmp->QueryFloatAttribute("Z", &mPositionOffset_Z);
	}
	node_tmp = node->FirstChildElement("Rotation");
	if (node_tmp)
	{
		node_tmp->QueryFloatAttribute("Pitch", &mRotation_Pitch);
		node_tmp->QueryFloatAttribute("Yaw", &mRotation_Yaw);
		node_tmp->QueryFloatAttribute("Roll", &mRotation_Roll);
	}

	
	


	TiXmlElement* node_tus = node->FirstChildElement("TextureUnits"); 
	if (node_tus)
	{
		size_t nbTextInEtr = listTextureUnit.size();

		for (node_tmp = node_tus->FirstChildElement("TextureUnit"); node_tmp; node_tmp = node_tmp->NextSiblingElement("TextureUnit"))
		{
			size_t index = (size_t)-1;
			node_tmp->QueryStringAttribute("Index", &str); index = EMO_BaseFile::GetUnsigned(str, index);

			if (index < nbTextInEtr)
				mListTextureUnit.push_back(listTextureUnit.at(index));
		}
	}






	TiXmlElement* node_extr = node->FirstChildElement("Extrude");
	if (node_extr)
	{
		string str;
		node_extr->QueryStringAttribute("Enable", &str);
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);


		TiXmlElement* node_shape = node_extr->FirstChildElement("LineShape");
		if (node_shape)
		{
			float x = 0.0f;
			float y = 0.0f;
			for (node_tmp = node_shape->FirstChildElement("Point"); node_tmp; node_tmp = node_tmp->NextSiblingElement("Point"))
			{
				node_tmp->QueryFloatAttribute("X", &x);
				node_tmp->QueryFloatAttribute("Y", &y);

				mExtrudeLineShape.push_back(Point(x, y));		//order reinversed as the file.
			}
		}

		TiXmlElement* node_path = node_extr->FirstChildElement("Path");
		if (node_path)
		{
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
			float w = 0.0f;
			for (node_tmp = node_path->FirstChildElement("Point"); node_tmp; node_tmp = node_tmp->NextSiblingElement("Point"))
			{
				node_tmp->QueryFloatAttribute("WorldScaleFactor", &x);
				node_tmp->QueryFloatAttribute("WorldScaleAdd", &y);
				node_tmp->QueryFloatAttribute("WorldOffsetFactor", &z);
				node_tmp->QueryFloatAttribute("WorldOffsetFactorb", &w);
				
				mExtrudePath.insert(mExtrudePath.begin(), Point(x, y, z, w));
			}
		}

		node_tmp = node_extr->FirstChildElement("EndPoint");
		if (node_tmp)
		{
			node_tmp->QueryFloatAttribute("WorldScaleFactor", &mEndExtrude_WorldScale_factor);
			node_tmp->QueryFloatAttribute("WorldScaleAdd", &mEndExtrude_WorldScale_add);
			node_tmp->QueryFloatAttribute("WorldOffsetFactor", &mEndExtrude_WorldOffset_factor);
			node_tmp->QueryFloatAttribute("WorldOffsetFactorb", &mEndExtrude_WorldOffset_factorb);
		}

	}



	TiXmlElement* node_anim = node->FirstChildElement("AnimationAList");
	if (node_anim)
	{
		for (node_tmp = node_anim->FirstChildElement("Animation"); node_tmp; node_tmp = node_tmp->NextSiblingElement("Animation"))
		{
			mListAnimationA.push_back(Etr_AnimationA());
			if (!mListAnimationA.back().import_Xml(node_tmp))
				mListAnimationA.pop_back();
		}
	}

	node_anim = node->FirstChildElement("AnimationBList");
	if (node_anim)
	{
		for (node_tmp = node_anim->FirstChildElement("AnimationB"); node_tmp; node_tmp = node_tmp->NextSiblingElement("AnimationB"))
		{
			mListAnimationB.push_back(Etr_AnimationB());
			if (!mListAnimationB.back().import_Xml(node_tmp))
				mListAnimationB.pop_back();
		}
	}



	TiXmlElement* node_unknow = node->FirstChildElement("Unknows");
	if (node_unknow)
	{
		node_tmp = node_unknow->FirstChildElement("unk_0_0"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_0_0 = EMO_BaseFile::GetUnsigned(str, unk_0_0);
		node_tmp = node_unknow->FirstChildElement("unk_0_1"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_0_1 = EMO_BaseFile::GetUnsigned(str, unk_0_1);
		node_tmp = node_unknow->FirstChildElement("unk_0_2"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_0_2 = EMO_BaseFile::GetUnsigned(str, unk_0_2);
		node_tmp = node_unknow->FirstChildElement("unk_0_3"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_0_3 = EMO_BaseFile::GetUnsigned(str, unk_0_3);
		node_tmp = node_unknow->FirstChildElement("unk_0_4"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_0_4 = EMO_BaseFile::GetUnsigned(str, unk_0_4);
		node_tmp = node_unknow->FirstChildElement("unk_0_5"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_0_5 = EMO_BaseFile::GetUnsigned(str, unk_0_5);
		node_tmp = node_unknow->FirstChildElement("unk_0_6"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_0_6 = EMO_BaseFile::GetUnsigned(str, unk_0_6);
		node_tmp = node_unknow->FirstChildElement("unk_0_7"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_0_7 = EMO_BaseFile::GetUnsigned(str, unk_0_7);
		
		node_tmp = node_unknow->FirstChildElement("unk_1"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_1 = EMO_BaseFile::GetUnsigned(str, unk_1);
		node_tmp = node_unknow->FirstChildElement("unk_2"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_2 = EMO_BaseFile::GetUnsigned(str, unk_2);
		
		
		TiXmlElement* node_unk4 = node_unknow->FirstChildElement("unk_4");
		TiXmlElement* node_unk4_b = node_unknow->FirstChildElement("unk_4b");
		TiXmlElement* node_unk5 = node_unknow->FirstChildElement("unk_5");
		for (size_t i = 0; i < 8; i++)
		{
			if (node_unk4)
			{
				node_unk4->QueryStringAttribute(("b" + std::to_string(i)).c_str(), &str); unk_4_b[i] = (EMO_BaseFile::GetUnsigned(str, unk_4_b[i] ? 1 : 0) == 1);
			}

			if (node_unk4_b)
			{
				node_unk4_b->QueryStringAttribute(("b" + std::to_string(i)).c_str(), &str); unk_4b_b[i] = (EMO_BaseFile::GetUnsigned(str, unk_4b_b[i] ? 1 : 0) == 1);
			}

			if (node_unk5)
			{
				node_unk5->QueryStringAttribute(("b" + std::to_string(i)).c_str(), &str); unk_5_b[i] = (EMO_BaseFile::GetUnsigned(str, unk_5_b[i] ? 1 : 0) == 1);
			}
		}

		node_tmp = node_unknow->FirstChildElement("unk_6"); if (node_tmp) node_tmp->QueryStringAttribute("u32", &str); unk_6 = EMO_BaseFile::GetUnsigned(str, unk_6);
		node_tmp = node_unknow->FirstChildElement("unk_13"); if (node_tmp) node_tmp->QueryStringAttribute("u16", &str); unk_13 = EMO_BaseFile::GetUnsigned(str, unk_13);
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool Etr_AnimationA::import_Xml(TiXmlElement* node)
{
	Reset();

	string str = "";
	node->QueryStringAttribute("Parameter", &str);
	node->QueryStringAttribute("Component", &str);
	if (str == "R")
		mComponent = 0;
	else if (str == "G")
		mComponent = 1;
	else if (str == "B")
		mComponent = 2;
	else if (str == "A")
		mComponent = 3;
	else
		mComponent = EMO_BaseFile::GetUnsigned(str, mComponent);


	TiXmlElement* node_tmp;
	node_tmp = node->FirstChildElement("StartAnimation"); if (node_tmp) node_tmp->QueryStringAttribute("At", &str);
	if (str == "EndWithNoInterpolation")
		mStartAnimation = SA_EndWithNoInterpolation;
	else if (str == "FirstMiddlePoint")
		mStartAnimation = SA_FirstMiddlePoint;
	else if (str == "Start")
		mStartAnimation = SA_Start;

	node_tmp = node->FirstChildElement("unk_0b"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_0b = EMO_BaseFile::GetUnsigned(str, unk_0b);
	node_tmp = node->FirstChildElement("unk_1"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_1 = EMO_BaseFile::GetUnsigned(str, unk_1);
	node_tmp = node->FirstChildElement("unk_1b"); if (node_tmp) node_tmp->QueryStringAttribute("u4", &str); unk_1b = EMO_BaseFile::GetUnsigned(str, unk_1b);
	node_tmp = node->FirstChildElement("unk_2"); if (node_tmp) node_tmp->QueryStringAttribute("u16", &str); unk_2 = EMO_BaseFile::GetUnsigned(str, unk_2);


	TiXmlElement* node_kf = node->FirstChildElement("Keyframes");
	if (!node_kf)
		return true;

	size_t timeIndex = 0;
	float value = 0.0f;
	for (node_tmp = node_kf->FirstChildElement("Keyframe"); node_tmp; node_tmp = node_tmp->NextSiblingElement("Keyframe"))
	{
		node_tmp->QueryStringAttribute("TimeIndex", &str); timeIndex = EMO_BaseFile::GetUnsigned(str, timeIndex);
		node_tmp->QueryFloatAttribute("Value", &value);

		mListKeyframe.push_back(Keyframe(timeIndex, value));
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             import_Xml			                             |
\-------------------------------------------------------------------------------*/
bool Etr_AnimationB::import_Xml(TiXmlElement* node)
{
	Reset();

	TiXmlElement* node_tmp;
	string str = "";
	node_tmp = node->FirstChildElement("unk_0"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_0 = EMO_BaseFile::GetUnsigned(str, unk_0);
	node_tmp = node->FirstChildElement("unk_0b"); if (node_tmp) node_tmp->QueryStringAttribute("u8", &str); unk_0b = EMO_BaseFile::GetUnsigned(str, unk_0b);


	TiXmlElement* node_anim = node->FirstChildElement("AnimationList");
	if (!node_anim)
		return true;

	for (node_tmp = node_anim->FirstChildElement("Animation"); node_tmp; node_tmp = node_tmp->NextSiblingElement("Animation"))
	{
		mAnimations.push_back(Etr_AnimationA());
		if (!mAnimations.back().import_Xml(node_tmp))
			mAnimations.pop_back();
	}

	return true;
}









/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Xml First Version for Debug ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*-------------------------------------------------------------------------------\
|                             save_Xml				                             |
\-------------------------------------------------------------------------------*
void Etr::save_Xml(string filename, bool show_error)
{
	uint8_t *buf;
	size_t size;

	buf = ReadFile(filename, &size, show_error);
	if (!buf)
		return;

	TiXmlDocument *doc = new TiXmlDocument();
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "");
	doc->LinkEndChild(decl);

	TiXmlElement* root = new TiXmlElement("Etr");
	EMO_BaseFile::WriteParamString(root, "path", filename);
	
	write_Xml(root, buf, size);

	doc->LinkEndChild(root);



	delete[] buf;

	doc->SaveFile(filename + ".xml");

	return;
}




/*-------------------------------------------------------------------------------\
|                             write_Xml						                     |
\-------------------------------------------------------------------------------*
void Etr::write_Xml(TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	ETRHeader *hdr = (ETRHeader *)buf;
	if (size < sizeof(ETRHeader) || memcmp(hdr->signature, ETR_SIGNATURE, 4) != 0)
		return;


	std::vector<bool> listBytesAllreadyTagged;
	listBytesAllreadyTagged.resize(size, false);				//to check override of the same byte (overflow)


	//node->SetAttribute("value", UnsignedToString(value, hexadecimal));
	
	TiXmlElement* node_ETRHeader = new TiXmlElement("Header");
	TiXmlElement* node;
	node = new TiXmlElement("nbSection1"); node->SetAttribute("u16", UnsignedToString(hdr->nbSection1, false)); node_ETRHeader->LinkEndChild(node);
	node = new TiXmlElement("offsetSection1"); node->SetAttribute("startOffset", UnsignedToString(hdr->offsetSection1, true)); node_ETRHeader->LinkEndChild(node);
	node = new TiXmlElement("nbSection2"); node->SetAttribute("u16", UnsignedToString(hdr->nbSection2, false)); node_ETRHeader->LinkEndChild(node);
	node = new TiXmlElement("offsetSection2"); node->SetAttribute("startOffset", UnsignedToString(hdr->offsetSection2, true)); node_ETRHeader->LinkEndChild(node);
	parent->LinkEndChild(node_ETRHeader);
	


	struct forTestSection3
	{
		size_t startOffset;
		TiXmlElement* node;

		forTestSection3(size_t startOffset, TiXmlElement* node=0){ this->startOffset = startOffset; this->node = node; }
	};
	struct forTestSection3_Sorter
	{
		bool operator()(forTestSection3 &a, forTestSection3 &b) { return (a.startOffset < b.startOffset); }
	};
	std::vector<forTestSection3> listSection3;





	std::vector<uint32_t> listOffsetfromSection1ToSection2;

	ETR_Section1* section1 = (ETR_Section1*)GetOffsetPtr(buf, hdr->offsetSection1);

	size_t offset = 0;
	size_t offset_tmp = 0;
	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		size_t startOffsetSection1 = hdr->offsetSection1 + i * sizeof(ETR_Section1);

		TiXmlElement* node_ETRSection1 = new TiXmlElement("Section1");

		node = new TiXmlElement("bone"); node->SetAttribute("string", string(section1[i].boneName)); node_ETRSection1->LinkEndChild(node);
		
		node = new TiXmlElement("unk_0_0"); node->SetAttribute("u32", UnsignedToString(section1[i].unk_0_0, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_0_1"); node->SetAttribute("u32", UnsignedToString(section1[i].unk_0_1, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_0_2"); node->SetAttribute("u32", UnsignedToString(section1[i].unk_0_2, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_0_3"); node->SetAttribute("u32", UnsignedToString(section1[i].unk_0_3, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_0_4"); node->SetAttribute("u32", UnsignedToString(section1[i].unk_0_4, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_0_5"); node->SetAttribute("u32", UnsignedToString(section1[i].unk_0_5, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_0_6"); node->SetAttribute("u32", UnsignedToString(section1[i].unk_0_6, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_0_7"); node->SetAttribute("u32", UnsignedToString(section1[i].unk_0_7, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_0_8"); node->SetAttribute("float", std::to_string(section1[i].unk_0_8)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_0_9"); node->SetAttribute("float", std::to_string(section1[i].unk_0_9)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_0_10"); node->SetAttribute("float", std::to_string(section1[i].unk_0_10)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_0_11"); node->SetAttribute("float", std::to_string(section1[i].unk_0_11)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_0_12"); node->SetAttribute("float", std::to_string(section1[i].unk_0_12)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_0_13"); node->SetAttribute("float", std::to_string(section1[i].unk_0_13)); node_ETRSection1->LinkEndChild(node);
		
		node = new TiXmlElement("unk_1"); node->SetAttribute("u8", UnsignedToString(section1[i].unk_1, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("numberPointsExtrudePathSection3"); node->SetAttribute("u8", UnsignedToString(section1[i].numberPointsExtrudePathSection3, false)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_1c"); node->SetAttribute("u8", UnsignedToString(section1[i].unk_1c, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_1d"); node->SetAttribute("u8", UnsignedToString(section1[i].unk_1d, true)); node_ETRSection1->LinkEndChild(node);

		node = new TiXmlElement("unk_2"); node->SetAttribute("u8", UnsignedToString(section1[i].unk_2, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_2b"); node->SetAttribute("u8", UnsignedToString(section1[i].unk_2b, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_3"); node->SetAttribute("u16", UnsignedToString(section1[i].unk_3, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_4"); node->SetAttribute("u16", UnsignedToString(section1[i].unk_4, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_5"); node->SetAttribute("u16", UnsignedToString(section1[i].unk_5, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_6"); node->SetAttribute("u32", UnsignedToString(section1[i].unk_6, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_7"); node->SetAttribute("float", std::to_string(section1[i].unk_7)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("emmMaterialId"); node->SetAttribute("u16", UnsignedToString(section1[i].emmMaterialId, false)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("numberSection2"); node->SetAttribute("u16", UnsignedToString(section1[i].numberSection2, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("offsetSection2"); node->SetAttribute("startOffset", UnsignedToString(section1[i].offsetSection2, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("offsetSection3"); node->SetAttribute("startOffset", UnsignedToString(section1[i].offsetSection3, true)); node_ETRSection1->LinkEndChild(node);
		
		node = new TiXmlElement("colorStart"); node->SetAttribute("R", FloatToString(section1[i].colorStart[0])); node->SetAttribute("G", FloatToString(section1[i].colorStart[1])); node->SetAttribute("B", FloatToString(section1[i].colorStart[2])); node->SetAttribute("A", FloatToString(section1[i].colorStart[3]));  node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("colorEnd"); node->SetAttribute("R", FloatToString(section1[i].colorEnd[0])); node->SetAttribute("G", FloatToString(section1[i].colorEnd[1])); node->SetAttribute("B", FloatToString(section1[i].colorEnd[2])); node->SetAttribute("A", FloatToString(section1[i].colorEnd[3]));  node_ETRSection1->LinkEndChild(node);

		node = new TiXmlElement("unk_9"); node->SetAttribute("float", std::to_string(section1[i].unk_9)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("numberSegmentsExtrudeShapeSection3"); node->SetAttribute("u32", UnsignedToString(section1[i].numberSegmentsExtrudeShapeSection3, false)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("unk_11"); node->SetAttribute("u16", UnsignedToString(section1[i].unk_11, true)); node_ETRSection1->LinkEndChild(node);

		node = new TiXmlElement("numberSection4"); node->SetAttribute("u16", UnsignedToString(section1[i].unk_11, false)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("offsetSection4"); node->SetAttribute("startOffset", UnsignedToString(section1[i].offsetSection4, true)); node_ETRSection1->LinkEndChild(node);

		node = new TiXmlElement("unk_13"); node->SetAttribute("u16", UnsignedToString(section1[i].unk_13, true)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("numberSection5"); node->SetAttribute("u16", UnsignedToString(section1[i].numberSection5, false)); node_ETRSection1->LinkEndChild(node);
		node = new TiXmlElement("offsetSection5"); node->SetAttribute("startOffset", UnsignedToString(section1[i].offsetSection5, true)); node_ETRSection1->LinkEndChild(node);

		parent->LinkEndChild(node_ETRSection1);

		listOffsetfromSection1ToSection2.push_back((*((uint32_t*)GetOffsetPtr(buf, hdr->offsetSection1 + i * sizeof(ETR_Section1) + section1[i].offsetSection2))) + hdr->offsetSection1 + i * sizeof(ETR_Section1));			// we will made the link on header of Section2




		offset_tmp = section1[i].offsetSection3;
		if (offset_tmp)
		{
			offset_tmp += startOffsetSection1;

			TiXmlElement* node_ETRSection3 = new TiXmlElement("Section3");
			node_ETRSection1->LinkEndChild(node_ETRSection3);

			listSection3.push_back(forTestSection3(offset_tmp, node_ETRSection3));
		}
	}




	//Section3, list of floats
	{
		//we check the order to have all startOffset ordered
		std::sort(listSection3.begin(), listSection3.end(), forTestSection3_Sorter());


		size_t sizeSection3 = 0;

		size_t nbSection3 = listSection3.size();
		for (size_t i = 0; i < nbSection3; i++)
		{
			forTestSection3 &section3 = listSection3.at(i);

			sizeSection3 = 0;
			if (i + 1 != nbSection3)					//if not the last
			{
				for (size_t j = i+1; j < nbSection3; j++)							//prevent from 2 Section1 target the same Section3, so the same offset
				{
					if (listSection3.at(j).startOffset != section3.startOffset)
					{
						sizeSection3 = listSection3.at(j).startOffset - section3.startOffset;
						break;
					}
				}
				if (sizeSection3 == 0)					//not next Section3 found,
					sizeSection3 = hdr->offsetSection2 - section3.startOffset;			//so we use the start of Section2

			}else {
				sizeSection3 = hdr->offsetSection2 - section3.startOffset;				//for the last Section3, we use the start of Section2.
			}

			if (sizeSection3)
			{
				float* section3_floats = (float*)GetOffsetPtr(buf, section3.startOffset);
				
				size_t nbFloats = sizeSection3 / 4;
				for (size_t j = 0; j < nbFloats; j++)
				{
					node = new TiXmlElement("Float_" + std::to_string(j)); node->SetAttribute("float", std::to_string(section3_floats[j])); section3.node->LinkEndChild(node);
				}
			}
		}
	}





	ETR_Section2* section2 = (ETR_Section2*)GetOffsetPtr(buf, hdr->offsetSection2);
	for (size_t i = 0; i < hdr->nbSection2; i++)
	{
		uint32_t offset = hdr->offsetSection2 + i * sizeof(ETR_Section2);
		size_t startSection2 = offset;

		string linkFromSection1 = "";
		size_t nbLink = listOffsetfromSection1ToSection2.size();
		for (size_t j = 0; j < nbLink; j++)
		{
			if (listOffsetfromSection1ToSection2.at(j) == offset)
				linkFromSection1 += ((linkFromSection1.size() != 0) ? "," : "") + std::to_string(j);
		}
		
		
		TiXmlElement* node_ETRSection2 = new TiXmlElement("Section2");
		node_ETRSection2->SetAttribute("linkToSection1", string(linkFromSection1));
		
		node = new TiXmlElement("unk_0"); node->SetAttribute("u8", UnsignedToString(section2[i].unk_0, true)); node_ETRSection2->LinkEndChild(node);
		node = new TiXmlElement("EmbTextureId"); node->SetAttribute("u8", UnsignedToString(section2[i].embTextureId, false)); node_ETRSection2->LinkEndChild(node);

		node = new TiXmlElement("unk_1"); node->SetAttribute("u8", UnsignedToString(section2[i].unk_1, true)); node_ETRSection2->LinkEndChild(node);
		node = new TiXmlElement("unk_1b"); node->SetAttribute("u8", UnsignedToString(section2[i].unk_1b, true)); node_ETRSection2->LinkEndChild(node);
		node = new TiXmlElement("unk_2"); node->SetAttribute("u32", UnsignedToString(section2[i].unk_2, true)); node_ETRSection2->LinkEndChild(node);
		node = new TiXmlElement("unk_3"); node->SetAttribute("u16", UnsignedToString(section2[i].unk_3, false)); node_ETRSection2->LinkEndChild(node);
		node = new TiXmlElement("typeSection2_b"); node->SetAttribute("u16", UnsignedToString(section2[i].typeSection2_b, false)); node_ETRSection2->LinkEndChild(node);


		if (section2[i].typeSection2_b == 0)
		{
			ETR_Section2_b_00* section2b = (ETR_Section2_b_00*)GetOffsetPtr(buf, startSection2 + 0xC);
			TiXmlElement* node_ETRSection2b = new TiXmlElement("Section2b_00");
			node = new TiXmlElement("color"); node->SetAttribute("R", FloatToString(section2b[0].color[0])); node->SetAttribute("G", FloatToString(section2b[0].color[1])); node->SetAttribute("B", FloatToString(section2b[0].color[2])); node->SetAttribute("A", FloatToString(section2b[0].color[3]));  node_ETRSection2b->LinkEndChild(node);
			node_ETRSection2->LinkEndChild(node_ETRSection2b);

		}
		else if (section2[i].typeSection2_b == 1) {

			ETR_Section2_b_01* section2b = (ETR_Section2_b_01*)GetOffsetPtr(buf, startSection2 + 0xC);
			TiXmlElement* node_ETRSection2b = new TiXmlElement("Section2b_01");
			node = new TiXmlElement("color"); node->SetAttribute("R", FloatToString(section2b[0].color[0])); node->SetAttribute("G", FloatToString(section2b[0].color[1])); node->SetAttribute("B", FloatToString(section2b[0].color[2])); node->SetAttribute("A", FloatToString(section2b[0].color[3]));  node_ETRSection2b->LinkEndChild(node);
			node_ETRSection2->LinkEndChild(node_ETRSection2b);


		}
		else if (section2[i].typeSection2_b == 2) {

			ETR_Section2_b_02* section2b = (ETR_Section2_b_02*)GetOffsetPtr(buf, startSection2 + 0xC);
			TiXmlElement* node_ETRSection2b = new TiXmlElement("Section2b_02");

			node = new TiXmlElement("unk_0"); node->SetAttribute("u32", UnsignedToString(section2b[0].unk_0, true)); node_ETRSection2b->LinkEndChild(node);
			node = new TiXmlElement("unk_1"); node->SetAttribute("u32", UnsignedToString(section2b[0].unk_1, true)); node_ETRSection2b->LinkEndChild(node);
			node = new TiXmlElement("unk_2"); node->SetAttribute("u16", UnsignedToString(section2b[0].unk_2, true)); node_ETRSection2b->LinkEndChild(node);
			node = new TiXmlElement("numberSection2_02_c"); node->SetAttribute("u16", UnsignedToString(section2b[0].numberSection2_02_c, false)); node_ETRSection2b->LinkEndChild(node);
			node = new TiXmlElement("offsetSection2_02_c"); node->SetAttribute("offset", UnsignedToString(section2b[0].offsetSection2_02_c, true)); node_ETRSection2b->LinkEndChild(node);


			offset_tmp = section2b[0].offsetSection2_02_c;
			if (offset_tmp)
			{
				offset_tmp += startSection2 + 0xC;
				size_t startSection2c = offset_tmp;
				ETR_Section2_b_02_c* section2c = (ETR_Section2_b_02_c*)GetOffsetPtr(buf, startSection2c);


				TiXmlElement* node_ETRSection2c_list = new TiXmlElement("Section2b_02_c_list");

				for (size_t j = 0; j < section2b[0].numberSection2_02_c; j++)
				{
					TiXmlElement* node_ETRSection2c = new TiXmlElement("Section2b_02_c");

					node = new TiXmlElement("unk_0"); node->SetAttribute("u16", UnsignedToString(section2c[j].unk_0, true)); node_ETRSection2c->LinkEndChild(node);
					node = new TiXmlElement("unk_1"); node->SetAttribute("u16", UnsignedToString(section2c[j].unk_1, true)); node_ETRSection2c->LinkEndChild(node);
					node = new TiXmlElement("color"); node->SetAttribute("R", FloatToString(section2c[j].color[0])); node->SetAttribute("G", FloatToString(section2c[j].color[1])); node->SetAttribute("B", FloatToString(section2c[j].color[2])); node->SetAttribute("A", FloatToString(section2c[j].color[3]));  node_ETRSection2c->LinkEndChild(node);

					node_ETRSection2c_list->LinkEndChild(node_ETRSection2c);
				}

				node_ETRSection2b->LinkEndChild(node_ETRSection2c_list);
			}

			node_ETRSection2->LinkEndChild(node_ETRSection2b);
		}


		parent->LinkEndChild(node_ETRSection2);
	}

}

*/






/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// wxHexEditor coloration ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*-------------------------------------------------------------------------------\
|                             save_Coloration		                             |
\-------------------------------------------------------------------------------*/
void Etr::save_Coloration(string filename, bool show_error)
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
void Etr::write_Coloration(TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	ETRHeader *hdr = (ETRHeader *)buf;
	if (size < sizeof(ETRHeader) || memcmp(hdr->signature, ETR_SIGNATURE, 4) != 0)
		return;


	std::vector<bool> listBytesAllreadyTagged;
	listBytesAllreadyTagged.resize(size, false);				//to check override of the same byte (overflow)


	size_t idTag = 0;
	size_t offset = 0;
	size_t incSection = 0;
	size_t incParam = 0;

	write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "ETRHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);	offset += 4 * sizeof(char);
	write_Coloration_Tag("endian", "uint16_t", "", offset, sizeof(uint16_t), "ETRHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("version", "uint32_t", "", offset, sizeof(uint32_t), "ETRHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
	write_Coloration_Tag("unk_00", "uint16_t", "always 0", offset, sizeof(uint16_t), "ETRHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);

	write_Coloration_Tag("nbSection1", "uint16_t", "", offset, sizeof(uint16_t), "ETRHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	write_Coloration_Tag("nbSection2", "uint16_t", "", offset, sizeof(uint16_t), "ETRHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);

	write_Coloration_Tag("offsetSection1", "uint32_t", "Offset (always 0x18 because of header Size)", offset, sizeof(uint32_t), "ETRHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);	offset += sizeof(uint32_t);
	write_Coloration_Tag("offsetSection2", "uint32_t", "Offset ", offset, sizeof(uint32_t), "ETRHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged);	offset += sizeof(uint32_t);



	std::vector<uint32_t> listOffsetfromSection1ToSection2;

	ETR_Section1 *section1 = (ETR_Section1*)GetOffsetPtr(buf, hdr->offsetSection1);
	offset = hdr->offsetSection1;
	size_t offset_tmp = 0;
	incSection++;
	for (size_t i = 0; i < hdr->nbSection1; i++)
	{
		size_t offsetSection1 = hdr->offsetSection1 + i * sizeof(ETR_Section1);
		offset = offsetSection1;

		incParam = 0;
		write_Coloration_Tag("boneName", "string", "boneName to attach to. hyp : the name are on 2 line of 16 bytes as it's clear in emp (but not here because of lot of 0 after)", offset, 32 * sizeof(char), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += 32 * sizeof(char);

		write_Coloration_Tag("unk_0_0", "uint32_t", "values found 0x0 (most), 0x5f4c5f67, 0x535254, 0x5f4c5f62", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_0_1", "uint32_t", "values found 0x0 (most), 0x646e6148", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_0_2", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_0_3", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_0_4", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_0_5", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_0_6", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_0_7", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_0_8", "float", "[-1.1, 1.2], 0.0 at most", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		write_Coloration_Tag("unk_0_9", "float", "[-3.0, 2.0], 0.0 at most", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		write_Coloration_Tag("unk_0_10", "float", "[-3.0, 1.5], 0.0 at most", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		write_Coloration_Tag("unk_0_11", "float", "[-2.09439516, 3.14159274], 0.0 at most", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		write_Coloration_Tag("unk_0_12", "float", "[-2.792527, 1.04719758], 0.0 at most", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);
		write_Coloration_Tag("unk_0_13", "float", "[-2.792527, 2.44346118], 0.0 at most", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += sizeof(float);

		write_Coloration_Tag("unk_1", "uint8_t", " values found 0, 1, 4", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("numberPointsExtrudePathSection3", "uint8_t", " seam to be a number for a path (2D position + UV) in section3, may be a path for extrude. values found 0 (most), 1, 2, 3, 4, 5, 6, 8, 0xa, 0xc, 0xf, 0x10", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_1b", "uint16_t", " few hole in range [0, 0x14], and after go to 0xae. 0 at most", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);

		write_Coloration_Tag("unk_2", "uint8_t", " range found [0, 0x10], 1 at most, not have 7, and have 0x14, 0x1E and 0x80.", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_2b", "uint8_t", " values found 0x0 (most), 0x3c, 0x40, 0x42, 0x44, 0x45, 0x46, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4f, 0x51. seam to be a bit composition", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_3", "uint16_t", " 0xffff for none (most). ", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);

		write_Coloration_Tag("unk_4", "uint8_t", " ", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_4b", "uint8_t", " ", offset, sizeof(uint8_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_5", "uint16_t", "range found [0 (most), 7]", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);

		write_Coloration_Tag("unk_6", "uint32_t", "0x45 (most), 0xc7, 0x47, 0x4f", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_7", "float", "range found [-3.0, 1.0]. 0.0 at most.", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);

		write_Coloration_Tag("emmMaterialId", "uint16_t", "maximum found 13. 65535 for none.", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		
		write_Coloration_Tag("numberSection2", "uint16_t", "count of pointers to the Section2. maximum found 2 (never 0)", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		offset_tmp = section1[i].offsetSection2;
		if (offset_tmp)
			offset_tmp += offsetSection1;
		write_Coloration_Tag("offsetSection2", "uint32_t", "Offset (add with the start of the current Section1) => "+ UnsignedToString(offset_tmp, true) + " , to another offset witch target one of section2.", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged);	offset += sizeof(uint32_t);

		offset_tmp = section1[i].offsetSection3;
		if (offset_tmp)
			offset_tmp += offsetSection1;
		write_Coloration_Tag("offsetSection3", "uint32_t", "Offset (add start Section1) to target a Section3 => " + UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged);	offset += sizeof(uint32_t);

		write_Coloration_Tag("colorStart", "float[4]", "1, 1, 1, 0 at most ", offset, 4 * sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += 4 * sizeof(float);
		write_Coloration_Tag("colorEnd", "float[4]", "0, 0, 0, 0 at most", offset, 4 * sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged, i); offset += 4 * sizeof(float);

		write_Coloration_Tag("unk_9", "float", "range found [-0.15, 5.2]", offset, sizeof(float), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
		write_Coloration_Tag("numberSegmentsExtrudeShapeSection3", "uint32_t", "seam to be a number of segment (give a number of point) in Section3, may be for a shape to extrude. values found 0, 4, 6, 8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x18, 0x20. seam to be subdivisions in width", offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
		write_Coloration_Tag("unk_11", "uint16_t", "few holes in range [2, 0x23], and after go to 0x96.", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		
		write_Coloration_Tag("numberSection4", "uint16_t", "maximum found 150", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		offset_tmp = section1[i].offsetSection4;
		if (offset_tmp)
			offset_tmp += offsetSection1 + 0x70;
		write_Coloration_Tag("offsetSection4", "uint32_t", "Offset (add 0x70 from start Section1) => "+ UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		write_Coloration_Tag("unk_13", "uint16_t", "always 1", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		write_Coloration_Tag("numberSection5", "uint16_t", " maximum found 6", offset, sizeof(uint16_t), "Section1", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		offset_tmp = section1[i].offsetSection5;
		if (offset_tmp)
			offset_tmp += offsetSection1 + 0xA8;
		write_Coloration_Tag("offsetSection5", "uint32_t", "Offset (add 0xA8 from start Section1 => " + UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section1", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);






		//offsets
		offset_tmp = section1[i].offsetSection2;
		if (offset_tmp)
		{
			offset_tmp += offsetSection1;
			offset = offset_tmp;
			
			uint32_t* tmp = (uint32_t*)GetOffsetPtr(buf, offset);

			for (size_t j = 0; j < section1[i].numberSection2; j++)
			{
				write_Coloration_Tag("offsetSection2b", "uint32_t", "Offset (add with the start of Section1[" + std::to_string(i) + "] ), target a section2 => " + UnsignedToString(tmp[j] + offsetSection1, true), offset, sizeof(uint32_t), "Section1b", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
				listOffsetfromSection1ToSection2.push_back(tmp[j] + offsetSection1);			// we will made the link on header of Section2
			}
		}

		offset_tmp = section1[i].offsetSection3;
		if (offset_tmp)
		{
			offset_tmp += offsetSection1;
			
			size_t offsetSection3 = offset_tmp;
			offset = offsetSection3;

			incParam = 0;
			write_Coloration_Tag("directionX", "float", "seam to be a direction_X (for extrude ?), startSection3 from Section1[" + std::to_string(i) + "] ", offset, sizeof(float), "Section3", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("directionY", "float", "seam to be a direction_Y (for extrude ?)", offset, sizeof(float), "Section3", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("directionZ", "float", "seam to be a direction_Z (for extrude ?)", offset, sizeof(float), "Section3", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged); offset += sizeof(float);
			write_Coloration_Tag("directionW", "float", "seam to be a direction_W (for extrude ?)", offset, sizeof(float), "Section3", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged); offset += sizeof(float);


			for (size_t j = 0; j < section1[i].numberPointsExtrudePathSection3; j++)				//as unk_1b is a number of Points
			{
				incParam = 0;
				write_Coloration_Tag("Point_X", "float", "seam to be a X position in a plane (extrude path ?)", offset, sizeof(float), "Section3", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
				write_Coloration_Tag("Point_Y", "float", "seam to be a Y position in a plane (extrude path ?)", offset, sizeof(float), "Section3", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
				write_Coloration_Tag("Point_U", "float", "seam to be a U of Uv (extrude path ?)", offset, sizeof(float), "Section3", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
				write_Coloration_Tag("Point_V", "float", "seam to be a V of Uv (extrude path ?)", offset, sizeof(float), "Section3", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
			}
			
			if (section1[i].numberSegmentsExtrudeShapeSection3 != 0)
			{
				for (size_t j = 0; j <= section1[i].numberSegmentsExtrudeShapeSection3; j++)				//as unk_10 is a number of segments, so we have unk_10+1 points
				{
					incParam = 0;
					write_Coloration_Tag("Point_X", "float", "seam to be a X position in a plane (extrude shape?)", offset, sizeof(float), "Section3", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
					write_Coloration_Tag("Point_Y", "float", "seam to be a Y position in a plane (extrude shape?)", offset, sizeof(float), "Section3", parent, idTag++, incSection + 7, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
				}
			}
		}


		offset_tmp = section1[i].offsetSection4;
		if (offset_tmp)
		{
			offset_tmp += offsetSection1 + 0x78;
			size_t offsetSection4 = offset_tmp;
			offset = offsetSection4;
			ETR_Section4 *section4 = (ETR_Section4*)GetOffsetPtr(buf, offsetSection4);

			//TODO section1[i].numberSection4;

			incParam = 0;
			write_Coloration_Tag("unk_0", "uint16_t", "", offset, sizeof(uint16_t), "Section4", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			write_Coloration_Tag("nbSection4_b", "uint16_t", "", offset, sizeof(uint16_t), "Section4", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			
			offset_tmp = section4[0].offsetSection4_b;
			if (offset_tmp)
				offset_tmp += offsetSection4;
			write_Coloration_Tag("offsetSection4_b", "uint32_t", " offset To Section4b (add StartSection4) => "+ UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section4", parent, idTag++, incSection + 3, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

			if (offset_tmp)
			{
				size_t startSection4b_list = offset_tmp;
				offset = startSection4b_list;

				for (size_t j = 0; j < section4[0].nbSection4_b; j++)
				{
					size_t offsetSection4b = startSection4b_list + j * sizeof(ETR_Section4b);
					offset = offsetSection4b;
					ETR_Section4b *section4b = (ETR_Section4b*)GetOffsetPtr(buf, offsetSection4b);


					incParam = 0;
					write_Coloration_Tag("unk_0", "uint16_t", "", offset, sizeof(uint16_t), "Section4b", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
					write_Coloration_Tag("unk_1", "uint32_t", "", offset, sizeof(uint32_t), "Section4b", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
					write_Coloration_Tag("numberKeyFrames", "uint16_t", "number of KeyframesTimeIndex and Floats", offset, sizeof(uint16_t), "Section4b", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
					
					offset_tmp = section4b[0].offsetKeyFrames;
					if (offset_tmp)
						offset_tmp += offsetSection4b;
					write_Coloration_Tag("offsetKeyFrames", "uint32_t", "list Time (in frames) of keyframes => "+ UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section4b", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);

					offset_tmp = section4b[0].offsetFloats;
					if (offset_tmp)
						offset_tmp += offsetSection4b;
					write_Coloration_Tag("offsetFloats", "uint32_t", "list of values of keyframes => " + UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section4b", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);


					offset_tmp = section4b[0].offsetKeyFrames;
					if (offset_tmp)
					{
						offset_tmp += offsetSection4b;
						offset = offset_tmp;

						incParam = 0;
						for (size_t k = 0; k < section4b[0].numberKeyFrames; k++)
						{
							write_Coloration_Tag("TimeIndexOfKeyframe["+ std::to_string(k) +"]", "uint16_t", " time in frames ", offset, sizeof(uint16_t), "Section4b", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
						}
					}

					offset_tmp = section4b[0].offsetFloats;
					if (offset_tmp)
					{
						offset_tmp += offsetSection4b;
						offset = offset_tmp;

						incParam = 0;
						for (size_t k = 0; k < section4b[0].numberKeyFrames; k++)
						{
							write_Coloration_Tag("ValueOfKeyFrames[" + std::to_string(k) + "]", "float", " time in frames ", offset, sizeof(float), "Section4b", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
						}
					}

					/*
					//seam the game don't care about indexlist (redondantes informations). so in rebuild , we will just put 202 bytes (+ 0 filling).

					//for Indexes, lazybone say there is always the same size : so 102 uint16. So this is the test.
					if (section4b[0].numberKeyFrames > 1)				//if there isn't any keyframe or there is only one, we don't have any index list.
					{
						uint16_t *listindex = (uint16_t*)GetOffsetPtr(buf, offset);

						incParam = 0;
						for (size_t k = 0; k < 102; k++)
						{
							write_Coloration_Tag("Index[" + std::to_string(k) + "]", "uint16_t", "", offset, sizeof(uint16_t), "Section4b", parent, idTag++, incSection + 4, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
						}
					}
					*/
				}
			}
		}



		offset_tmp = section1[i].offsetSection5;
		if (offset_tmp)
		{
			offset_tmp += offsetSection1 + 0xA8;
			size_t startSection5_list = offset_tmp;

			for (size_t j = 0; j < section1[i].numberSection5; j++)
			{
				size_t offsetSection5 = startSection5_list + j * sizeof(ETR_Section5);
				offset = offsetSection5;
				ETR_Section5 *section5 = (ETR_Section5*)GetOffsetPtr(buf, offsetSection5);

				incParam = 0;
				write_Coloration_Tag("unk_0", "uint16_t", "", offset, sizeof(uint16_t), "Section5", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
				write_Coloration_Tag("unk_1", "uint32_t", "", offset, sizeof(uint32_t), "Section5", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
				write_Coloration_Tag("numberKeyFrames", "uint16_t", "number of KeyframesTimeIndex and Floats", offset, sizeof(uint16_t), "Section5", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
				
				offset_tmp = section5[0].offsetKeyFrames;
				if (offset_tmp)
					offset_tmp += offsetSection5;
				write_Coloration_Tag("offsetKeyFrames", "uint32_t", "list Time (in frames) of keyframes => "+ UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section5", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);
				
				offset_tmp = section5[0].offsetFloats;
				if (offset_tmp)
					offset_tmp += offsetSection5;
				write_Coloration_Tag("offsetFloats", "uint32_t", "list of values of keyframes => " + UnsignedToString(offset_tmp, true), offset, sizeof(uint32_t), "Section5", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint32_t);



				offset_tmp = section5[0].offsetKeyFrames;
				if (offset_tmp)
				{
					offset_tmp += offsetSection5;
					offset = offset_tmp;

					incParam = 0;
					for (size_t k = 0; k < section5[0].numberKeyFrames; k++)
					{
						write_Coloration_Tag("TimeIndexOfKeyframe[" + std::to_string(k) + "]", "uint16_t", " time in frames ", offset, sizeof(uint16_t), "Section5", parent, idTag++, incSection + 6, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
					}
				}

				offset_tmp = section5[0].offsetFloats;
				if (offset_tmp)
				{
					offset_tmp += offsetSection5;
					offset = offset_tmp;

					incParam = 0;
					for (size_t k = 0; k < section5[0].numberKeyFrames; k++)
					{
						write_Coloration_Tag("ValueOfKeyFrames[" + std::to_string(k) + "]", "float", " time in frames ", offset, sizeof(float), "Section5", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset += sizeof(float);
					}
				}


				/*
				//seam the game don't care about indexlist (redondantes informations). so in rebuild , we will just put 202 bytes (+ 0 filling).

				//for Indexes, lazybone say there is always the same size : so 102 uint16. So this is the test.
				if (section5[0].numberKeyFrames > 1)				//if there isn't any keyframe or there is only one, we don't have any index list.
				{
					uint16_t *listindex = (uint16_t*)GetOffsetPtr(buf, offset);

					incParam = 0;
					for (size_t k = 0; k < 102; k++)
					{
						write_Coloration_Tag("Index[" + std::to_string(k) + "]", "uint16_t", "", offset, sizeof(uint16_t), "Section5", parent, idTag++, incSection + 5, incParam++, listBytesAllreadyTagged, j); offset += sizeof(uint16_t);
					}
				}
				*/
			}

		}

	}




	ETR_Section2 *section2 = (ETR_Section2*)GetOffsetPtr(buf, hdr->offsetSection2);
	offset = hdr->offsetSection2;
	incSection++;
	for (size_t i = 0; i < hdr->nbSection2; i++)
	{
		size_t offsetSection2 = hdr->offsetSection2 + i * sizeof(ETR_Section2);
		offset = offsetSection2;

		string linkFromSection1 = "";
		size_t nbLink = listOffsetfromSection1ToSection2.size();
		for (size_t j = 0; j < nbLink; j++)
		{
			if (listOffsetfromSection1ToSection2.at(j)==offset)
				linkFromSection1 += ((linkFromSection1.size()!=0) ? "," : "") + string("Section1[") + std::to_string(j) +"]";
		}

		incParam = 0;
		write_Coloration_Tag("unk_0", "uint8_t", "always 0, Start of Section2["+ std::to_string(i) +"]" + ((linkFromSection1.size()) ? (" with link to "+ linkFromSection1) : ""), offset, sizeof(uint8_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("EmbTextureId", "uint8_t", " 255  for no texture.", offset, sizeof(uint8_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);

		write_Coloration_Tag("unk_1", "uint8_t", "value found 0x11 (most), 0x21, 0x31, 0x33. ", offset, sizeof(uint8_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_1b", "uint8_t", "always 0x22. could be min and maxFiltering as emp.", offset, sizeof(uint8_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		write_Coloration_Tag("unk_2", "uint32_t", "always 0", offset, sizeof(uint32_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

		write_Coloration_Tag("unk_3", "uint16_t", "only 0 (most) or 1", offset, sizeof(uint16_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
		write_Coloration_Tag("typeSection2_b", "uint16_t", "influence the next 16 bytes, only 0, 1 (most) or 2", offset, sizeof(uint16_t), "Section2", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);


		if (section2[i].typeSection2_b == 0)
		{
			size_t startOffsetSection2b = offset;
			ETR_Section2_b_00 *section2b = (ETR_Section2_b_00*)GetOffsetPtr(buf, startOffsetSection2b);

			write_Coloration_Tag("Color", "(from EMP:) float[4]", "R: [0.0, 1.0] 0.0 most of Time. G: [0.0, 1.0] 0.0 most of Time. B : [0.00100000005, 40.0] 1.0 most of Time. A : [0.00999999978, 50.0] 1.0 most of Time. it's TextureScrollScale", offset, 4 * sizeof(float), "Section2b_00", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);

		}
		else if (section2[i].typeSection2_b == 1) {

			size_t startOffsetSection2b = offset;
			ETR_Section2_b_01 *section2b = (ETR_Section2_b_01*)GetOffsetPtr(buf, startOffsetSection2b);

			write_Coloration_Tag("Color", "(from EMP:) float[4]", "R is in [-0.0500000007, 2.0], 0 most of time. G is in [-0.0799999982, 1.0], 0 most of Time. B and A alwaus at 0. may be it's not a color ?", offset, 4 * sizeof(float), "Section2b_01", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);


		}
		else if (section2[i].typeSection2_b == 2) {

			size_t startOffsetSection2b = offset;
			ETR_Section2_b_02 *section2b = (ETR_Section2_b_02*)GetOffsetPtr(buf, startOffsetSection2b);

			incParam = 0;
			write_Coloration_Tag("unk_0", "uint32_t", "(from EMP:) all value in [1, 0x10], go no continue to 0x7d0, 2 is the most", offset, sizeof(uint32_t), "Section2b_02", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_1", "uint32_t", "(from EMP:) always 0", offset, sizeof(uint32_t), "Section2b_02", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			write_Coloration_Tag("unk_2", "uint16_t", "(from EMP:) always 0", offset, sizeof(uint16_t), "Section2b_02", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			write_Coloration_Tag("numberSection2_02_c", "uint16_t", "(from EMP:) maximum found 64", offset, sizeof(uint16_t), "Section2b_02", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			write_Coloration_Tag("offsetSection2_02_c", "uint32_t", "", offset, sizeof(uint32_t), "Section2b_02", parent, idTag++, incSection + 1, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

			offset_tmp = (*section2b).offsetSection2_02_c;
			if (offset_tmp)
			{
				offset_tmp += startOffsetSection2b;
				size_t startOffsetSection2c = offset_tmp;
				offset = offset_tmp;

				ETR_Section2_b_02_c *section2c = (ETR_Section2_b_02_c*)GetOffsetPtr(buf, startOffsetSection2c);
				for (size_t j = 0; j < (*section2b).numberSection2_02_c; j++)
				{
					incParam = 0;
					write_Coloration_Tag("unk_0", "uint16_t", "(from EMP:) continue from 1 to 0x10, and after could go until 0x7d0", offset, sizeof(uint16_t), "Section2b_02_c", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
					write_Coloration_Tag("unk_1", "uint16_t", "(from EMP:)always 0", offset, sizeof(uint16_t), "Section2b_02_c", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
					write_Coloration_Tag("Color", "float[4]", "(from EMP:)R : [-0.8, 5.0] 0.0 is the most. G : [-2.0, 1.0] 0.0 is the most. B : [-1.0, 8.0] 0.125 is the most. A : [-1.0, 4.0] 0.5 and 0.25 are the most. it's TextureScrollScale", offset, 4 * sizeof(float), "Section2b_02_c", parent, idTag++, incSection + 2, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(float);
				}
			}

		}
	}
}





/*-------------------------------------------------------------------------------\
|                             write_Coloration_Tag			                     |
\-------------------------------------------------------------------------------*/
void Etr::write_Coloration_Tag(string paramName, string paramType, string paramComment, size_t startOffset, size_t size, string sectionName, TiXmlElement* parent, size_t idTag, size_t sectionIndex, size_t paramIndex, std::vector<bool> &listBytesAllreadyTagged, size_t sectionIndexInList, bool checkAllreadyTaggued)
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
			continue;
		}

		if (index == 0x11ac)			//for test
			int aa = 42;

		if ((checkAllreadyTaggued) && (listBytesAllreadyTagged.at(index)))
			printf("warning on tagID %i : the byte %i allready taggued, may be a overflow between blocks. Infos : %s. \n", idTag, index, (sectionName + ((sectionIndexInList != (size_t)-1) ? "[" + std::to_string(sectionIndexInList) + "]" : "") + "." + paramName + " (" + paramType + ") : " + paramComment).c_str() );

		listBytesAllreadyTagged.at(index) = true;
	}
}


}