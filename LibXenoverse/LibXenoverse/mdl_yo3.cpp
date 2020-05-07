#include "mdl_yo3.h"


#include <sstream>
#include <iomanip>

namespace LibXenoverse
{

























/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// wxHexEditor coloration ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////







/*-------------------------------------------------------------------------------\
|                             save_Coloration		                             |
\-------------------------------------------------------------------------------*/
void Mdl::save_Coloration(string filename, bool show_error)
{
	uint8_t *buf;
	size_t size;

	originefilename = filename;

	buf = EMO_BaseFile::ReadFile(filename, &size, show_error);
	if (!buf)
		return;


	BinColorTag binCt;

	write_Coloration(binCt, binCt.startColoration(filename), buf, size);

	binCt.endColoration(filename);

	return;
}

/*-------------------------------------------------------------------------------\
|                             write_Coloration				                     |
\-------------------------------------------------------------------------------*/
void Mdl::write_Coloration(BinColorTag &binCt, TiXmlElement *parent, const uint8_t *buf, size_t size)
{
	struct Vect4
	{
		float x, y, z, w;
		Vect4(float x = 0, float y = 0, float z = 0, float w = 1) { this->x = x; this->y = y; this->z = z; this->w = w;}
	};
	
	struct Kf
	{
		uint16_t t;
		Vect4 value;

		Kf(uint16_t t, Vect4 value) { this->t = t; this->value = value; }
	};

	struct KfAnim
	{
		uint8_t unk0;
		uint8_t transform;
		std::vector<Kf> listkf;

		KfAnim(uint8_t unk0, uint8_t transform) { this->unk0 = unk0; this->transform = transform; }
	};

	struct FaceInfo
	{
		uint8_t nbFaces;
		uint8_t type;						//MDL_TypeFace_Triangle, MDL_TypeFace_Quad, MDL_TypeFace_Unknow

		FaceInfo(uint8_t nbFaces, uint8_t type) { this->nbFaces = nbFaces; this->type = type; }
	};



	struct dataHeader
	{
		uint8_t type;
		string name;
		
		size_t startAdress;
		uint8_t* buf;
		size_t size;

		//animation
		std::vector<KfAnim> listkfAnim;
		//shape
		std::vector<EMDVertex> listVertex;
		std::vector<FaceInfo> listFaceInfos;
		std::vector<size_t> listFaces;
		string materialName;

		//material
		uint32_t ambientColor;
		uint32_t difuseColor;
		uint32_t specularColor;
		std::vector<string> listTextureNames;
		
		
		dataHeader(string name, uint8_t type, size_t startAdress, uint8_t* buf, size_t size)
		{
			this->name = name;
			this->startAdress = startAdress;
			this->buf = buf;
			this->size = size;
			this->type = type;
		}
	};
	std::vector<dataHeader> listBlock;


	
	Mdl_Header* hdr = (Mdl_Header*)buf;
	if (size < sizeof(Mdl_Header))
		return;
	
	if ((memcmp(hdr->signature, MDL_SIGNATURE, 4) != 0))
		return;

	std::vector<bool> listBytesAllreadyTagged;
	listBytesAllreadyTagged.resize(size, false);				//to check override of the same byte (overflow)
	size_t idTag = 0;
	size_t offset = 0;
	size_t incSection = 0;
	size_t incParam = 0;
	
	binCt.write_Coloration_Tag("signature", "string", "", offset, 4 * sizeof(char), "Mdl_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * sizeof(char);
	binCt.write_Coloration_Tag("unk0", "uint16_t", "", offset, sizeof(uint16_t), "Mdl_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("unk1", "uint16_t", "", offset, sizeof(uint16_t), "Mdl_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
	binCt.write_Coloration_Tag("unk2", "uint16_t", "", offset, sizeof(uint16_t), "Mdl_Header", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);

	uint8_t* listChar = (uint8_t*)GetOffsetPtr_native(buf, 0);

	offset++;
	Mdl_BlockHeader* bhdr = (Mdl_BlockHeader*)GetOffsetPtr_native(buf, offset);
	char str_char = ' ';
	size_t offset_testStart;
	size_t offset_testChar;
	while (offset + bhdr->nbNameChar < size)
	{
		bhdr = (Mdl_BlockHeader*)GetOffsetPtr_native(buf, offset);
		

		binCt.write_Coloration_Tag("type__" + UnsignedToString(bhdr->type, true), "uint8_t", "start new block. 2: material, 3: texture, A: model ", offset, sizeof(uint8_t), "Mdl_BlockHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		binCt.write_Coloration_Tag("nbNameChar", "uint8_t", "", offset, sizeof(uint8_t), "Mdl_BlockHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
		string name = "";
		for (size_t i = 0; i < bhdr->nbNameChar; i++)
			name += (char)listChar[offset + i];
		binCt.write_Coloration_Tag("name__"+ UnsignedToString(bhdr->type, true) +"__"+ name, "uint8_t x " + ToString((size_t)bhdr->nbNameChar), "", offset, sizeof(uint8_t) * bhdr->nbNameChar, "Mdl_BlockHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t) * bhdr->nbNameChar;
		
		listBlock.push_back(dataHeader(name, bhdr->type, offset, GetOffsetPtr_native(buf, offset), size - offset));

		while (offset + sizeof(Mdl_BlockHeader) < size)
		{
			offset_testStart = offset + sizeof(Mdl_BlockHeader);

			uint8_t aa = listChar[offset_testStart];						//todo remove

			if (!(
				((65 <= listChar[offset_testStart]) && (listChar[offset_testStart] <= 90)) ||				//A-Z
				((97 <= listChar[offset_testStart]) && (listChar[offset_testStart] <= 122)) ||				//a-z
				((48 <= listChar[offset_testStart]) && (listChar[offset_testStart] <= 57)) ||				//0-9
				((listChar[offset_testStart] == 46) || (listChar[offset_testStart] == 92) || (listChar[offset_testStart] == 95)) // ". \ _"
				))					//check if it's a possible normal char. 
			{
				offset++;					 //if not a normal char try next
				continue;
			}

			
			offset_testChar = offset_testStart;
			size_t inc = 0;
			while (offset_testChar + 1 < size)
			{
				offset_testChar++;
				inc++;

				uint8_t ab = listChar[offset_testChar];						//todo remove

				if (!(
					((65 <= listChar[offset_testChar]) && (listChar[offset_testChar] <= 90)) ||				//A-Z
					((97 <= listChar[offset_testChar]) && (listChar[offset_testChar] <= 122)) ||			//a-z
					((48 <= listChar[offset_testChar]) && (listChar[offset_testChar] <= 57)) ||				//0-9
					((listChar[offset_testChar] == 46) || (listChar[offset_testChar] == 92) || (listChar[offset_testChar] == 95)) // ". \ _"
					))					//check if it's a possible normal char. 
				{
					break;
				}
			}

			if ( ((inc <= 2) && ( (inc!=2) || (listChar[offset_testStart] != 115) || (listChar[offset_testStart+1] != 107))) || //minimum name is 3 (to avoid too much false results) BUT there is a bone named "sk"
				(inc > 0xFF))				
			{
				offset++;							//continue searching
				continue;
			}

			bhdr = (Mdl_BlockHeader*)GetOffsetPtr_native(buf, offset);
			uint8_t type = bhdr->type;
			if ((bhdr->nbNameChar >= 2)&&(bhdr->nbNameChar <= inc))
			{
				if((type == MDL_TYPE_Material) || (type == MDL_TYPE_Texture) || (type == MDL_TYPE_Model) || (type == MDL_TYPE_Shape) ||
					(type == MDL_TYPE_Submesh) || (type == MDL_TYPE_Animation) || (type == MDL_TYPE_Animation_Bone) || (type == MDL_TYPE_Bone) || (type == MDL_TYPE_Material_USE))
				{
					break;		//break to registre the new value
				}else {
					printf("warning on %s : unknow type %s\n", EMO_BaseFile::UnsignedToString(offset, true).c_str(), EMO_BaseFile::UnsignedToString(type, true).c_str() );
					notifyWarning();
				}
			}
			offset++;					 //if not a normal char try next
		}
	}










	EMD emd;
	emd.setName(originefilename);
	EMDModel* emdModel = 0;

	EMM emm;
	emm.setName(originefilename);

	EAN ean;
	ean.setName(originefilename);
	EANAnimation eanAnimation;

	ESK esk;
	esk.setName(originefilename);
	ean.setSkeleton(&esk);

	EMB emb;
	emb.load(folderFromFilename(originefilename) + "\\textures_yo3.emb");
	bool useTextures = false;

	
	size_t incSection_animH = incSection++;
	size_t incSection_animOrient = incSection++;
	size_t incSection_animScale = incSection++;
	size_t incSection_animPos = incSection++;
	size_t incSection_animUnk = incSection++;
	size_t incSection_animPos2 = incSection++;
	size_t incSection_animPos3 = incSection++;
	size_t incSection_animUnk2 = incSection++;
	size_t incSection_vertexPos = incSection++;
	size_t incSection_vertexUnk = incSection++;
	size_t incSection_vertexUv = incSection++;
	size_t incSection_model = incSection++;
	size_t incSection_bone = incSection++;
	size_t incSection_material = incSection++;
	size_t incSection_unkSplit = incSection++;
	

	for (size_t i = 0, nbBlock = listBlock.size(); i < nbBlock; i++)
	{
		dataHeader &dh = listBlock.at(i);

		if (dh.type == MDL_TYPE_Material)
		{
			offset = 0;

			incSection = incSection_material;
			incParam = 0;

			Mdl_Material* mat = (Mdl_Material*)GetOffsetPtr_native(dh.buf, offset);
			dh.ambientColor = mat[0].ambientColor;
			dh.difuseColor = mat[0].difuseColor;
			dh.specularColor = mat[0].specularColor;

			binCt.write_Coloration_Tag("ambientColor", "uint32_t", "", dh.startAdress + offset, sizeof(uint32_t), "Mdl_Material", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			binCt.write_Coloration_Tag("difuseColor", "uint32_t", "", dh.startAdress + offset, sizeof(uint32_t), "Mdl_Material", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);
			binCt.write_Coloration_Tag("specularColor", "uint32_t", "", dh.startAdress + offset, sizeof(uint32_t), "Mdl_Material", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

			binCt.write_Coloration_Tag("endColor", "uint8_t", "", dh.startAdress + offset, sizeof(uint8_t), "Mdl_Material", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
			binCt.write_Coloration_Tag("endMat", "uint8_t", "", dh.startAdress + offset, sizeof(uint8_t), "Mdl_Material", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);

			if (dh.buf[offset] == MDL_TYPE_Texture)
			{
				size_t nbNameChar = dh.buf[offset + 1];
				string name = "";
				for (size_t j = 0; j < nbNameChar; j++)
					name += (char)dh.buf[offset + 2 + j];

				dh.listTextureNames.push_back(name);
			}




			/////////////////////////////////// fill emm
			EMMMaterial* emmMat = new EMMMaterial(dh.name);
			emm.getMaterials().push_back(emmMat);
			emmMat->setShaderName("T1_L_PT");
			emmMat->setUnknow_0(65535);
			emmMat->getParameters().push_back(new EMMParameter("MatSpcR",	0x0, 0, 0, 0.683910012f));
			emmMat->getParameters().push_back(new EMMParameter("MatSpcG",	0x0, 0, 0, 0.879310012f));
			emmMat->getParameters().push_back(new EMMParameter("MatSpcB",	0x0, 0, 0, 0.971260011f));
			emmMat->getParameters().push_back(new EMMParameter("SpcCoeff",	0x0, 0, 0, 0.0f));
			emmMat->getParameters().push_back(new EMMParameter("SpcPower",	0x0, 0, 0, 15.0f));
			emmMat->getParameters().push_back(new EMMParameter("MipMapLod0",0x0, 1, 0, -0.0500000007f));
			emmMat->getParameters().push_back(new EMMParameter("AlphaTest",	0x1, 1, 100, 0.0f));
			emmMat->getParameters().push_back(new EMMParameter("BackFace",	0x1, 1, 1, 0.0f));



		}else if ((dh.type == MDL_TYPE_Animation) || (dh.type == MDL_TYPE_Animation_Bone)) {
			offset = 0;

			EANAnimationNode eanAnimationNode;
			eanAnimationNode.setBoneIndex((size_t)-1);

			std::vector<ESKBone*> &bones = esk.getBones();
			for (size_t j = 0, nbBones = bones.size(); j < nbBones; j++)
			{
				if (bones.at(j)->getName() == dh.name)
				{
					eanAnimationNode.setBoneIndex(j);
					break;
				}
			}


			if (dh.type == MDL_TYPE_Animation)
			{
				binCt.write_Coloration_Tag("unk0_0x28", "uint8_t", "", dh.startAdress + offset, sizeof(uint8_t), "Mdl_AnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
			}
			
			while (dh.buf[offset] != 0)
			{
				incSection = incSection_animH;
				incParam = 0;

				Mdl_AnimationHeader* animH = (Mdl_AnimationHeader*)GetOffsetPtr_native(dh.buf, offset);
				KfAnim kfAnim(animH->unk0, animH->transform);

				binCt.write_Coloration_Tag("unk0", "uint8_t", "", dh.startAdress + offset, sizeof(uint8_t), "Mdl_AnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
				binCt.write_Coloration_Tag("transform", "uint8_t", "", dh.startAdress + offset, sizeof(uint8_t), "Mdl_AnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
				binCt.write_Coloration_Tag("nbFrames", "uint16_t", "", dh.startAdress + offset, sizeof(uint16_t), "Mdl_AnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);

				uint16_t nbFrames = animH->nbFrames;

				if (dh.type == MDL_TYPE_Animation_Bone)
				{

					switch (kfAnim.transform)
					{
					case MDL_Animation_Tranform_Orientation:
					{
						incSection = incSection_animOrient;
						incParam = 0;

						Mdl_Animation_Kf_Orientation* kf = (Mdl_Animation_Kf_Orientation*)GetOffsetPtr_native(dh.buf, offset);

						EANKeyframedAnimation eanKfAnim;
						eanKfAnim.setFlag(LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION_or_TargetPosition);
						vector<EANKeyframe> &keyframes = eanKfAnim.getKeyframes();

						for (size_t j = 0; j < nbFrames; j++)
						{
							kfAnim.listkf.push_back(Kf(kf[j].frameIndex, Vect4(kf[j].quatW, kf[j].quatX, kf[j].quatY, kf[j].quatZ)));

							keyframes.push_back(EANKeyframe(kf[j].frameIndex, kf[j].quatW, kf[j].quatX, kf[j].quatY, kf[j].quatZ));
						}

						if ((eanAnimationNode.getBoneIndex() != (size_t)-1)&&(keyframes.size()!=0))
						{
							eanAnimationNode.getKeyframed_animations().push_back(eanKfAnim);

							if (keyframes.back().frame > eanAnimation.getDuration())
								eanAnimation.setDuration(keyframes.back().frame);
						}

						binCt.write_Coloration_Tag("keyframe Orientation", "(uint16_t + 4 x float) x " + ToString(nbFrames), "", dh.startAdress + offset, sizeof(Mdl_Animation_Kf_Orientation) * nbFrames, "Mdl_Animation", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(Mdl_Animation_Kf_Orientation) * nbFrames;
					}
					break;

					case MDL_Animation_Tranform_Scale:
					{
						incSection = incSection_animScale;
						incParam = 0;

						Mdl_Animation_Kf_Scale_or_Position* kf = (Mdl_Animation_Kf_Scale_or_Position*)GetOffsetPtr_native(dh.buf, offset);

						EANKeyframedAnimation eanKfAnim;
						eanKfAnim.setFlag(LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE_or_CAMERA);
						vector<EANKeyframe> &keyframes = eanKfAnim.getKeyframes();

						for (size_t j = 0; j < nbFrames; j++)
						{
							kfAnim.listkf.push_back(Kf(kf[j].frameIndex, Vect4(kf[j].PosX, kf[j].PosY, kf[j].PosZ)));

							keyframes.push_back(EANKeyframe((size_t)kf[j].frameIndex, kf[j].PosX, kf[j].PosY, kf[j].PosZ));
						}

						if ((eanAnimationNode.getBoneIndex() != (size_t)-1) && (keyframes.size() != 0))
						{
							eanAnimationNode.getKeyframed_animations().push_back(eanKfAnim);

							if (keyframes.back().frame > eanAnimation.getDuration())
								eanAnimation.setDuration(keyframes.back().frame);
						}

						binCt.write_Coloration_Tag("keyframe Scale", "(uint16_t + 3 x float) x " + ToString(nbFrames), "", dh.startAdress + offset, sizeof(Mdl_Animation_Kf_Scale_or_Position) * nbFrames, "Mdl_Animation", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(Mdl_Animation_Kf_Scale_or_Position) * nbFrames;
					}
					break;

					case MDL_Animation_Tranform_Position:
					{
						incSection = incSection_animPos;
						incParam = 0;

						Mdl_Animation_Kf_Scale_or_Position* kf = (Mdl_Animation_Kf_Scale_or_Position*)GetOffsetPtr_native(dh.buf, offset);


						EANKeyframedAnimation eanKfAnim;
						eanKfAnim.setFlag(LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE_or_CAMERA);
						vector<EANKeyframe> &keyframes = eanKfAnim.getKeyframes();

						for (size_t j = 0; j < nbFrames; j++)
						{
							kfAnim.listkf.push_back(Kf(kf[j].frameIndex, Vect4(kf[j].PosX, kf[j].PosY, kf[j].PosZ)));

							keyframes.push_back(EANKeyframe((size_t)kf[j].frameIndex, kf[j].PosX, kf[j].PosY, kf[j].PosZ));
						}

						if ((eanAnimationNode.getBoneIndex() != (size_t)-1) && (keyframes.size() != 0))
						{
							eanAnimationNode.getKeyframed_animations().push_back(eanKfAnim);

							if (keyframes.back().frame > eanAnimation.getDuration())
								eanAnimation.setDuration(keyframes.back().frame);
						}

						binCt.write_Coloration_Tag("keyframe Position", "(uint16_t + 3 x float) x " + ToString(nbFrames), "", dh.startAdress + offset, sizeof(Mdl_Animation_Kf_Scale_or_Position) * nbFrames, "Mdl_Animation", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(Mdl_Animation_Kf_Scale_or_Position) * nbFrames;
					}
					break;


					case MDL_Animation_Tranform_unknow:
					{
						incSection = incSection_animUnk;
						incParam = 0;

						Mdl_Animation_Kf_Unkow* kfOrient = (Mdl_Animation_Kf_Unkow*)GetOffsetPtr_native(dh.buf, offset);

						for (size_t j = 0; j < nbFrames; j++)
							kfAnim.listkf.push_back(Kf(kfOrient[j].frameIndex, Vect4((float)(size_t)kfOrient[j].value)));

						binCt.write_Coloration_Tag("keyframe Unkow transform", "(uint16_t + uint8_t) x " + ToString(nbFrames), "", dh.startAdress + offset, sizeof(Mdl_Animation_Kf_Unkow) * nbFrames, "Mdl_Animation", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(Mdl_Animation_Kf_Unkow) * nbFrames;
					}
					break;

					}


				}else if (dh.type == MDL_TYPE_Animation) {					//todo find this.
					switch (kfAnim.transform)
					{
					case MDL_Animation_Tranform_position2:
					{
						incSection = incSection_animPos2;
						incParam = 0;

						Mdl_Animation_Kf_Scale_or_Position* kf = (Mdl_Animation_Kf_Scale_or_Position*)GetOffsetPtr_native(dh.buf, offset);

						for (size_t j = 0; j < nbFrames; j++)
							kfAnim.listkf.push_back(Kf(kf[j].frameIndex, Vect4(kf[j].PosX, kf[j].PosY, kf[j].PosZ)));

						binCt.write_Coloration_Tag("keyframe 0x28_Pos2", "(uint16_t + 3 x float) x " + ToString(nbFrames), "", dh.startAdress + offset, sizeof(Mdl_Animation_Kf_Scale_or_Position) * nbFrames, "Mdl_Animation", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(Mdl_Animation_Kf_Scale_or_Position) * nbFrames;
					}
					break;

					case MDL_Animation_Tranform_position3:
					{
						incSection = incSection_animPos3;
						incParam = 0;

						Mdl_Animation_Kf_Scale_or_Position* kf = (Mdl_Animation_Kf_Scale_or_Position*)GetOffsetPtr_native(dh.buf, offset);

						for (size_t j = 0; j < nbFrames; j++)
							kfAnim.listkf.push_back(Kf(kf[j].frameIndex, Vect4(kf[j].PosX, kf[j].PosY, kf[j].PosZ)));

						binCt.write_Coloration_Tag("keyframe 0x28_Pos3", "(uint16_t + 3 x float) x " + ToString(nbFrames), "", dh.startAdress + offset, sizeof(Mdl_Animation_Kf_Scale_or_Position) * nbFrames, "Mdl_Animation", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(Mdl_Animation_Kf_Scale_or_Position) * nbFrames;
					}
					break;

					case MDL_Animation_Tranform_unknow2:
					{
						incSection = incSection_animUnk2;
						incParam = 0;

						Mdl_Animation_Kf_Unkow2* kfOrient = (Mdl_Animation_Kf_Unkow2*)GetOffsetPtr_native(dh.buf, offset);

						for (size_t j = 0; j < nbFrames; j++)
							kfAnim.listkf.push_back(Kf(kfOrient[j].frameIndex, Vect4(kfOrient[j].value)));

						binCt.write_Coloration_Tag("keyframe 0x28_Unkow2 transform", "(uint16_t + float) x " + ToString(nbFrames), "", dh.startAdress + offset, sizeof(Mdl_Animation_Kf_Unkow2) * nbFrames, "Mdl_Animation", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(Mdl_Animation_Kf_Unkow2) * nbFrames;
					}
					break;

					}
				}


				dh.listkfAnim.push_back(kfAnim);
			}

			binCt.write_Coloration_Tag("EndAnimationPart", "uint8_t", "", dh.startAdress + offset, sizeof(uint8_t), "Mdl_AnimationHeader", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);

			if ((eanAnimationNode.getBoneIndex() != (size_t)-1) && (eanAnimationNode.getKeyframed_animations().size() != 0))
				eanAnimation.getNodes().push_back(eanAnimationNode);








		}else if (dh.type == MDL_TYPE_Model) {

			offset = 0;

			incSection = incSection_model;
			incParam = 0;

			/*
			uint8_t* uint8s = (uint8_t*)GetOffsetPtr_native(dh.buf, offset);

			bool isABone = false;
			if (uint8s[offset] == 0x0C)
			{
				binCt.write_Coloration_Tag("flags", "uint8_t", "", dh.startAdress + offset, sizeof(uint8_t), "Mdl_Model", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);

				if ((uint8s[offset] == MDL_TYPE_Shape) || (uint8s[offset] == MDL_TYPE_Model) || (uint8s[offset] == 0x00))						// model with next bock a shape = geometry
				{
					emdModel = new EMDModel();
					emdModel->setName(dh.name);
					emd.getModel().push_back(emdModel);

				}else{													// model with next block bone = skeleton
					
					binCt.write_Coloration_Tag("unknow", "uint32_t", "", dh.startAdress + offset, sizeof(uint32_t), "Mdl_Model", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint32_t);

					if (uint8s[offset] == 0x0B)
						isABone = true;
				}
			}else if (uint8s[offset] == 0x0B) {
				isABone = true;
			}
			
			if ((isABone) && (dh.name == "body_sk"))
			{
				incSection = incSection_bone;
				incParam = 0;

				binCt.write_Coloration_Tag("bone_flags", "uint8_t", "", dh.startAdress + offset, sizeof(uint8_t), "Mdl_ModelBone", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);

				float* floats = (float*)GetOffsetPtr_native(dh.buf, offset);
				binCt.write_Coloration_Tag("bone_AbsMatrix4x4", "4 x 4 float", "", dh.startAdress + offset, 4 * 4 * sizeof(float), "Mdl_ModelBone", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * 4 * sizeof(float);
				
				ESKBone* bone = new ESKBone(dh.name);
				esk.getBones().push_back(bone);

				bone->haveAbsoluteMatrix = true;

				for (size_t j = 0; j < 16; j++)
					bone->absoluteMatrix[j] = floats[j];
			}
			*/


			uint8_t* uint8s = (uint8_t*)GetOffsetPtr_native(dh.buf, offset);

			bool isABone = false;
			if (uint8s[offset] == 0x0C)
			{
				binCt.write_Coloration_Tag("flags", "uint8_t", "", dh.startAdress + offset, sizeof(uint8_t), "Mdl_Model", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);

				if (dh.name != "body_sk")
				{
					emdModel = new EMDModel();
					emdModel->setName(dh.name);
					emd.getModel().push_back(emdModel);

				}else {													// it's a skeleton

					incSection = incSection_bone;
					incParam = 0;

					//node will be organize into hierarchy, to be more readable.
					EskTreeNode* treeRootNode = new EskTreeNode(nullptr, (size_t)-1, nullptr);
					std::vector<ESKBone*> bones = esk.getBones();
					std::vector<EskTreeNode*> lastOnEachLevel;
					lastOnEachLevel.push_back(treeRootNode);
					size_t currentLevel = 0;

					
					//add body_sk to be good with other sk , like cards
					ESKBone* bone = new ESKBone("body_sk");
					EskTreeNode* treeCurrentNode = new EskTreeNode(bone, bones.size(), treeRootNode);
					bones.push_back(bone);
					bone->haveAbsoluteMatrix = true;			//still have default values
					treeRootNode->mChildren.push_back(treeCurrentNode);
					lastOnEachLevel.push_back(treeCurrentNode);
					currentLevel++;


					Mdl_BlockHeader* boneH = (Mdl_BlockHeader*)GetOffsetPtr_native(dh.buf, offset);
					EskTreeNode* treeParentNode;

					while (boneH->type == MDL_TYPE_Model)
					{
						string boneName = "";
						for (size_t j = 0; j < boneH->nbNameChar; j++)
							boneName += (char)dh.buf[offset + sizeof(Mdl_BlockHeader) + j];

						

						offset += sizeof(Mdl_BlockHeader) + boneH->nbNameChar;
						bool haveMatrix4x4 = (dh.buf[offset] == 0x0B);
						if (haveMatrix4x4)
						{
							binCt.write_Coloration_Tag("bone_flags 0x0B for child with matrix. 0x0c for ends", "uint8_t", "", dh.startAdress + offset, sizeof(uint8_t), "Mdl_ModelBone", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
						}else {					//will be colored another time.
							offset += sizeof(uint8_t);
						}
						

						float* floats = (float*)GetOffsetPtr_native(dh.buf, offset);
						if (haveMatrix4x4)
						{
							binCt.write_Coloration_Tag("bone_AbsMatrix4x4", "4 x 4 float", "", dh.startAdress + offset, 4 * 4 * sizeof(float), "Mdl_ModelBone", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += 4 * 4 * sizeof(float);
						}

						treeParentNode = lastOnEachLevel.at(currentLevel);
						bone = new ESKBone(boneName);
						treeCurrentNode = new EskTreeNode(bone, bones.size(), treeParentNode);
						bones.push_back(bone);
						treeParentNode->mChildren.push_back(treeCurrentNode);
						
						
						currentLevel++;
						if (lastOnEachLevel.size() <= currentLevel)
							lastOnEachLevel.push_back(treeCurrentNode);
						lastOnEachLevel.at(currentLevel) = treeCurrentNode;


						bone->haveAbsoluteMatrix = true;			//still have default values

						if (haveMatrix4x4)
						{
							std::vector<double> relativeTransformMatrix;
							for (size_t j = 0; j < 16; j++)
								relativeTransformMatrix.push_back( (double)floats[j] );


							bone->calculRelativeTransformFromRelativeTransformMatrix(relativeTransformMatrix);
						}

						//if (dh.buf[offset] == 0)						//to cancel the current increase.
						//	currentLevel--;

						while ((dh.buf[offset] == 0) && (currentLevel > 0))
						{
							currentLevel--;
							binCt.write_Coloration_Tag("go back to parent's level "+ ToString(currentLevel), "uint8_t", "", dh.startAdress + offset, sizeof(uint8_t), "Mdl_ModelBone", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
						}

						boneH = (Mdl_BlockHeader*)GetOffsetPtr_native(dh.buf, offset);
					}

					esk.setTreeOrganisation(treeRootNode);					//read hieirarchy to make the list of bones.
					delete treeRootNode;					//delete the hierarchy organisation, but not the bones
				}
			}
			


			



		}else if (dh.type == MDL_TYPE_Shape) {
			
			offset = 0;

			incSection = incSection_vertexPos;
			incParam = 0;

			uint16_t* head = (uint16_t*)GetOffsetPtr_native(dh.buf, offset);
			binCt.write_Coloration_Tag("nbVertex", "uint16_t", "", dh.startAdress + offset, sizeof(uint16_t), "Mdl_Shape", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			uint16_t nbVertex = head[0];

			float* floats = (float*)GetOffsetPtr_native(dh.buf, offset);
			binCt.write_Coloration_Tag("vertex Positions", "(3 x float) x " + ToString(nbVertex), "", dh.startAdress + offset, sizeof(float) * 3 * nbVertex, "Mdl_Shape", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float) * 3 * nbVertex;

			
			for (size_t j = 0; j < nbVertex; j++)
			{
				EMDVertex v;

				v.pos_x = floats[j * 3 + 0];
				v.pos_y = floats[j * 3 + 1];
				v.pos_z = floats[j * 3 + 2];

				v.flags += EMD_VTX_FLAG_POS;

				dh.listVertex.push_back(v);
			}


			//todo get UVs.

			incSection = incSection_vertexUnk;
			incParam = 0;

			uint16_t nbFaces  = *(uint16_t*)GetOffsetPtr_native(dh.buf, offset);
			uint16_t endFacesUvsSection = val16(nbFaces);
			binCt.write_Coloration_Tag("nbFaces", "uint16_t", "", dh.startAdress + offset, sizeof(uint16_t), "Mdl_Shape", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);

			uint8_t* facesInfos = (uint8_t*)GetOffsetPtr_native(dh.buf, offset);
			uint8_t tmp;
			size_t inc = 0;
			size_t testNbFaces = 0;
			while (facesInfos[inc] != 0x16)
			{
				tmp = facesInfos[inc++];
				dh.listFaceInfos.push_back(FaceInfo( (tmp >> 3) + 1, tmp & 0x7));		//+1 because we get the last index, not the number of faces.
				testNbFaces += dh.listFaceInfos.back().nbFaces;
			}
			if (testNbFaces != nbFaces)							//todo remove.
				int aa = 42;
			
			binCt.write_Coloration_Tag("FaceInfos = on uint8 the 3 right bits is for type (triangles vs quad), the rest for numberofFaces", "uint8_t", "", dh.startAdress + offset, inc * sizeof(uint8_t), "Mdl_Shape", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += inc * sizeof(uint8_t);

			binCt.write_Coloration_Tag("tag 0x16 just before Uv", "uint8_t", "", dh.startAdress + offset, sizeof(uint8_t), "Mdl_Shape", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
			


			incSection = incSection_vertexUv;
			incParam = 0;

			binCt.write_Coloration_Tag("nbVertex again", "uint16_t", "", dh.startAdress + offset, sizeof(uint16_t), "Mdl_Shape", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);

			float* floats_uv = (float*)GetOffsetPtr_native(dh.buf, offset);

			binCt.write_Coloration_Tag("vertex Uv", "(2 x float) x " + ToString(nbVertex), "", dh.startAdress + offset, sizeof(float) * 2 * nbVertex, "Mdl_Shape", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(float) * 2 * nbVertex;

			for (size_t j = 0; j < nbVertex; j++)
			{
				EMDVertex &v = dh.listVertex.at(j);

				v.text_u = floats_uv[j * 2 + 0];
				v.text_v = floats_uv[j * 2 + 1];

				v.flags += EMD_VTX_FLAG_TEX;
			}

			incSection = incSection_vertexUnk;
			incParam = 0;

			binCt.write_Coloration_Tag("unknow after UVs", "uint16_t", "", dh.startAdress + offset, sizeof(uint16_t), "Mdl_Shape", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			binCt.write_Coloration_Tag("endFacesUvsSection (invere byte of startUnkowSection)", "uint16_t", "", dh.startAdress + offset, sizeof(uint16_t), "Mdl_Shape", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint16_t);
			
			if ((false) && ( (dh.buf[offset] == 0x8) || (dh.buf[offset] == 0x2) ) )
			{
				incSection = incSection_unkSplit;
				incParam = 0;

				binCt.write_Coloration_Tag("continuation for a unk 0 or 1 value by face", "uint8_t", "", dh.startAdress + offset, sizeof(uint8_t), "Mdl_Shape", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
				binCt.write_Coloration_Tag("unk 0 or 1 value by face", "uint8_t x " + ToString(nbFaces), "", dh.startAdress + offset, sizeof(uint8_t) * nbFaces, "Mdl_Shape", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t) * nbFaces;
			}else {
				binCt.write_Coloration_Tag("end", "uint8_t", "", dh.startAdress + offset, sizeof(uint8_t), "Mdl_Shape", parent, idTag++, incSection, incParam++, listBytesAllreadyTagged); offset += sizeof(uint8_t);
			}
			
			


			/*
			if (dh.buf[offset] == MDL_TYPE_Submesh)			//todo rename as MDL_TYPE_Material_Use2
			{
				size_t nbNameChar = dh.buf[offset + 1];
				string name = "";
				for (size_t j = 0; j < nbNameChar; j++)
					name += (char)dh.buf[offset + 2 + j];

				
			}
			*/

			if ((i + 1 < listBlock.size()) && (listBlock.at(i + 1).type == MDL_TYPE_Submesh))		//todo rename as MDL_TYPE_Material_Use2
				dh.materialName = listBlock.at(i + 1).name;
			
			
			

			



			
			// real faces
			inc = 0;
			for (size_t j = 0, nbFaceInfos = dh.listFaceInfos.size(); j < nbFaceInfos; j++)
			{
				FaceInfo &fi = dh.listFaceInfos.at(j);

				if (fi.type == MDL_TypeFace_Triangle)
				{
					for (size_t k = 0; k < fi.nbFaces; k++)
					{
						dh.listFaces.push_back(inc);
						dh.listFaces.push_back(inc + 2);
						dh.listFaces.push_back(inc + 1);
						inc += 3;
					}

				}else if (fi.type == MDL_TypeFace_Quad) {

					for (size_t k = 0; k < fi.nbFaces; k++)
					{
						dh.listFaces.push_back(inc);
						dh.listFaces.push_back(inc + 2);
						dh.listFaces.push_back(inc + 1);

						dh.listFaces.push_back(inc);
						dh.listFaces.push_back(inc + 3);
						dh.listFaces.push_back(inc + 2);
						inc += 4;
					}

				}else if (fi.type == MDL_TypeFace_Pentaogone) {

					for (size_t k = 0; k < fi.nbFaces; k++)
					{
						dh.listFaces.push_back(inc);
						dh.listFaces.push_back(inc + 2);
						dh.listFaces.push_back(inc + 1);

						dh.listFaces.push_back(inc);
						dh.listFaces.push_back(inc + 3);
						dh.listFaces.push_back(inc + 2);

						dh.listFaces.push_back(inc);
						dh.listFaces.push_back(inc + 4);
						dh.listFaces.push_back(inc + 3);
						inc += 5;
					}


				}else if (fi.type == MDL_TypeFace_Hexagone) {

					for (size_t k = 0; k < fi.nbFaces; k++)
					{
						dh.listFaces.push_back(inc);
						dh.listFaces.push_back(inc + 2);
						dh.listFaces.push_back(inc + 1);

						dh.listFaces.push_back(inc);
						dh.listFaces.push_back(inc + 3);
						dh.listFaces.push_back(inc + 2);

						dh.listFaces.push_back(inc);
						dh.listFaces.push_back(inc + 4);
						dh.listFaces.push_back(inc + 3);

						dh.listFaces.push_back(inc);
						dh.listFaces.push_back(inc + 5);
						dh.listFaces.push_back(inc + 4);
						inc += 6;
					}

					

				}else {
					int aa = 42;
				}
			}
			if (inc > nbVertex)
				int aa = 42;
			





			/////////////////////////////////// fill emd
			if (emdModel == 0)
			{
				emdModel = new EMDModel();
				emdModel->setName(dh.name);
				emd.getModel().push_back(emdModel);
			}

			EMDMesh* emdMesh = new EMDMesh();
			emdModel->getMeshes().push_back(emdMesh);
			emdMesh->setName(dh.name);
			
			EMDSubmesh* emdSubMesh = new EMDSubmesh();
			emdMesh->getSubmeshes().push_back(emdSubMesh);
			emdSubMesh->setMaterialName( dh.materialName.length() ? dh.materialName : dh.name);
			emdSubMesh->setVertexTypeFlags( (nbVertex) ? dh.listVertex.at(0).flags : 0);

			float minX, minY, minZ, maxX, maxY, maxZ;
			for (size_t j = 0; j < nbVertex; j++)
			{
				EMDVertex &v = dh.listVertex.at(j);
				emdSubMesh->getVertices().push_back(&dh.listVertex.at(j));

				if (j == 0){ minX = maxX = v.pos_x; minY = maxY = v.pos_y; minZ = maxZ = v.pos_z; }

				if (v.pos_x < minX) { minX = v.pos_x; } else if (v.pos_x > maxX) { maxX = v.pos_x; }
				if (v.pos_y < minY) { minY = v.pos_y; } else if (v.pos_y > maxY) { maxY = v.pos_y; }
				if (v.pos_z < minZ) { minZ = v.pos_z; } else if (v.pos_z > maxZ) { maxZ = v.pos_z; }
			}
			
			emdSubMesh->setAABB((minX + maxX) / 2.0f, (minY + maxY) / 2.0f, (minZ + maxZ) / 2.0f, 1.0f, minX, minY, minZ, 1.0f, maxX, maxY, maxZ, 1.0);
			emdMesh->setAABB((minX + maxX) / 2.0f, (minY + maxY) / 2.0f, (minZ + maxZ) / 2.0f, 1.0f, minX, minY, minZ, 1.0f, maxX, maxY, maxZ, 1.0);

			if (dh.materialName.length())		//search for texture utilisation
			{
				vector<EMBFile *> &embTextures = emb.getFiles();
				size_t nbEmbTextures = embTextures.size();
				
				for (size_t j = 0; j < i; j++)
				{
					dataHeader &dh_b = listBlock.at(j);

					if ((dh_b.type == MDL_TYPE_Material) && (dh.materialName == dh_b.name))
					{
						if (dh_b.listTextureNames.size() == 0)
							break;

						for (size_t k = 0; k < dh_b.listTextureNames.size(); k++)
						{
							string textureName = nameFromFilename(dh_b.listTextureNames.at(k));
							textureName = textureName.substr(0, textureName.length() - 4) + ".dds";			//temporary test because xenoviewer crash on png in emb.

							size_t textureIndex = (size_t)-1;
							for (size_t m = 0; m < nbEmbTextures; m++)
							{
								if (embTextures.at(m)->getName() == textureName)
								{
									textureIndex = m;
									break;
								}
							}

							if (textureIndex != (size_t)-1)
							{
								useTextures = true;
								emdSubMesh->getDefinitions().push_back(EMDTextureUnitState(textureIndex));
								if (dh_b.listTextureNames.size() == 1)						//double the texture if only one to match dbxv2 material/shaders behaviour
									emdSubMesh->getDefinitions().push_back(EMDTextureUnitState(textureIndex));
							}
						}

						break;
					}
				}
			}

			EMDTriangles triangle;
			triangle.faces = dh.listFaces;
			emdSubMesh->getTriangles().push_back(triangle);
		}


	}


	

	emd.save(filenameNoExtension(originefilename) + ".emd");
	emm.save(filenameNoExtension(originefilename) + ".emm");
	if (useTextures)
		emb.save(filenameNoExtension(originefilename) + ".emb");

	if (esk.getBones().size())
	{
		std::vector<ESKBone*> &bones = esk.getBones();
		for(size_t j=0, nbBones = bones.size();j<nbBones;j++)
			esk.getBones().at(j)->calculAbsoluteMatrixFromRelativeTransform(bones, true);
		
		esk.save(filenameNoExtension(originefilename) + ".esk");

		if (eanAnimation.getDuration() != 0)
		{
			ean.getAnimations().push_back(eanAnimation);
			ean.save(filenameNoExtension(originefilename) + ".ean");
		}
	}
	ean.setSkeleton(0);							//avoid destruction
}


}