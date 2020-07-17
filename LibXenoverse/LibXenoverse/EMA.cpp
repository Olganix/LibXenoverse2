#include "EMA.h"


namespace LibXenoverse
{




bool EMA::Load(const uint8_t *buf, unsigned int size)
{
	while (name.find('.') != string::npos)
		name = nameFromFilenameNoExtension(name, true);

	Reset();

	EMAHeader *hdr = (EMAHeader *)buf;
	if (size < sizeof(EMAHeader) || hdr->signature != EMA_SIGNATURE)
		return false;

	this->big_endian = (buf[4] != 0xFE);

	if (hdr->skeleton_offset == 0)
	{
		// No skeleton
	}else {
		if (!EMO_Skeleton::Load(GetOffsetPtr(buf, hdr->skeleton_offset), size - val32(hdr->skeleton_offset)))
			return false;
	}

	version = ToString((uint32_t)hdr->version[0]) + "." + ToString((uint32_t)hdr->version[1]) + "." + ToString((uint32_t)hdr->version[2]) + "." + ToString((uint32_t)hdr->version[3]);
	type = val16(hdr->type);
	unknow_0 = val32(hdr->unknow_0);
	unknow_1 = val32(hdr->unknow_1);
	unknow_2 = val32(hdr->unknow_2);

	context = (type == EMA_TYPE_ANIM_Material) ? ContextUse::MaterialAnimation : ContextUse::ObjectAnimation;
	contextPtr = this;


	uint32_t *anim_offsets = (uint32_t *)GetOffsetPtr(buf, val16(hdr->header_size), true);
	animations.resize(val16(hdr->anim_count));

	for (size_t i = 0; i < animations.size(); i++)
	{
		EmaAnimation &animation = animations[i];
		EMAAnimationHeader *ahdr = (EMAAnimationHeader *)GetOffsetPtr(buf, anim_offsets, i);

		animation.duration = val16(ahdr->lastframeTime) + 1;
		
		animation.type = ahdr->type;
		animation.light_unknow = ahdr->light_unknow;
		animation.frame_float_size = val16(ahdr->frame_float_size);
		
		
		EMAAnimationName* emaAnimationName = (EMAAnimationName*)GetOffsetPtr(ahdr, ahdr->name_offset);
		animation.name_unknow_0 = val32(emaAnimationName->unknow_0);
		animation.name_unknow_1 = val32(emaAnimationName->unknow_1);
		animation.name_unknow_2 = val16(emaAnimationName->unknow_2);

		animation.name = (char*)GetOffsetPtr(ahdr, ahdr->name_offset + sizeof(EMAAnimationName));
		

		animation.nodes.resize(val16(ahdr->cmd_count));

		for (size_t j = 0; j < animation.nodes.size(); j++)
		{
			EMAAnimationNode &node = animation.nodes[j];
			EMAAnimationAnimNodeHeader *chdr = (EMAAnimationAnimNodeHeader *)GetOffsetPtr(ahdr, ahdr->animNode_offsets, j);

			if (HasSkeleton())
			{
				if (val16(chdr->bone_idx) >= GetNumBones())					//ex: some case on SSSS mat.ema
				{
					node.bone = nullptr;
					printf("Bone idx 0x%x out of bounds, in animation \"%s\", in node 0x%x. that could happen on material animation (we don't know why yet).\n", chdr->bone_idx, animation.name.c_str(), j);
					notifyWarning();
				}else{
					node.bone = &bones[val16(chdr->bone_idx)];
				}
			}else{
				node.bone = nullptr;
			}

			node.bone_idx = val16(chdr->bone_idx);
			node.transformComponent = (chdr->transformComponent & 0x3);
			node.noInterpolation = ((chdr->transformComponent & 0x4)!=0);
			node.unknow_0 = (chdr->transformComponent & 0x8);
			node.unknow_1 = (chdr->transformComponent & 0x10);

			node.transform = chdr->transform;
			node.keyframes.resize(val16(chdr->keyframe_count));



			for (size_t k = 0; k < node.keyframes.size(); k++)
			{
				EMAKeyframe &keyframe = node.keyframes[k];

				node.timesByteSize = chdr->transformComponent & 0x20;

				if (!(chdr->transformComponent & 0x20))				// timesByteSize
				{
					uint8_t *timing = (uint8_t *)GetOffsetPtr(chdr, sizeof(EMAAnimationAnimNodeHeader), true);
					keyframe.time = timing[k];
				}else {
					uint16_t *timing = (uint16_t *)GetOffsetPtr(chdr, sizeof(EMAAnimationAnimNodeHeader), true);
					keyframe.time = val16(timing[k]);
				}
				
				if (!(chdr->transformComponent & 0x40))				// indexesByteSize
				{
					uint16_t *indices = (uint16_t *)GetOffsetPtr(chdr, val16(chdr->indices_offset), true);
					uint16_t tmp = val16(indices[k]);
					keyframe.index = tmp & 0x3FFF;
					keyframe.interpolation = (tmp & 0xC000) >> 12;

				}else {
					uint32_t *indices = (uint32_t *)GetOffsetPtr(chdr, val16(chdr->indices_offset), true);
					uint32_t tmp = val32(indices[k]);
					keyframe.index = tmp & 0x3FFFFFFF;
					keyframe.interpolation = (tmp & 0xC0000000) >> 28;
				}
			}
		}

		animation.values.resize(val32(ahdr->value_count));
		float* values = (float *)GetOffsetPtr(ahdr, ahdr->values_offset);
		uint16_t* values_uint6 = (uint16_t*)values;

		

		

		for (size_t j = 0; j < animation.values.size(); j++)
		{
			if (animation.frame_float_size == 0)
				animation.values[j] = val_float(values[j]);
			else
				animation.values[j] = float16ToFloat(values_uint6[j]);
		}

	}

	return true;
}






uint8_t *EMA::CreateFile(unsigned int *psize)
{
	unsigned int file_size;
	uint32_t offset;


	cleanAnimations();

	file_size = CalculateFileSize();
	uint8_t *buf = new uint8_t[file_size];

	if (!buf)
	{
		printf("%s: Memory allocation error (0x%x)\n", FUNCNAME, file_size);
		LibXenoverse::notifyError();
		return nullptr;
	}

	memset(buf, 0, file_size);


	uint16_t nbAnimations = animations.size();
	if (nbAnimations > 65535)
	{
		printf("Error: the number of animation is up to 65535, witch is not possible. skipped after the limit.\n");
		notifyError();

		nbAnimations = 65535;
	}

	EMAHeader *hdr = (EMAHeader *)buf;
	hdr->signature = EMA_SIGNATURE;
	hdr->endianess_check = val16(0xFFFE);
	hdr->header_size = val16(sizeof(EMAHeader));
	hdr->anim_count = val16(nbAnimations);
	hdr->type = val16(type);
	hdr->unknow_0 = val32(unknow_0);
	hdr->unknow_1 = val32(unknow_1);
	hdr->unknow_2 = val32(unknow_2);
	
	if (version.length() == 0)
		version = "0";
	std::vector<string> sv = split(version, '.');
	for (size_t i = 0; i < 4; i++)
	{
		if (i < sv.size())
			hdr->version[i] = (uint8_t)std::stoi(sv.at(i));
		else
			hdr->version[i] = 0;
	}
	


	offset = sizeof(EMAHeader);
	uint32_t *anim_offsets = (uint32_t *)GetOffsetPtr(buf, offset, true);

	offset += nbAnimations * sizeof(uint32_t);

	for (uint16_t i = 0; i < nbAnimations; i++)
	{
		if (offset & 3)
			offset += (4 - (offset & 3));

		const EmaAnimation &animation = animations[i];
		EMAAnimationHeader *ahdr = (EMAAnimationHeader *)GetOffsetPtr(buf, offset, true);
		anim_offsets[i] = val32(offset);

		

		
		int16_t nbAnimationNodes = animation.nodes.size();
		if (nbAnimationNodes > 65535)
		{
			printf("Error: the number of animationNodes is up to 65535, witch is not possible. skipped after the limit.\n");
			notifyError();

			nbAnimationNodes = (int16_t)65535;
		}

		ahdr->lastframeTime = val16( (animation.duration > 0) ? (animation.duration - 1) : 0 );
		ahdr->cmd_count = val16(nbAnimationNodes);
		ahdr->value_count = val32(animation.values.size());
		ahdr->type = animation.type;
		ahdr->light_unknow = animation.light_unknow;
		ahdr->frame_float_size = val16(animation.frame_float_size);

		size_t timesByteSize = (animation.duration > 0x100) ? 0x20 : 0x0;							// (animation.duration > 0x100) == (lastKeyFrame > 0xFF) => have to change nbBytes for code frame index.
		size_t indexesByteSize = (animation.values.size() > 0x3FFF) ? 0x40 : 0x0;


		offset += sizeof(EMAAnimationHeader) - sizeof(uint32_t);
		offset += nbAnimationNodes * sizeof(uint32_t);

		for (uint16_t j = 0; j < nbAnimationNodes; j++)
		{
			if (offset & 3)
				offset += (4 - (offset & 3));

			const EMAAnimationNode &node = animation.nodes.at(j);
			EMAAnimationAnimNodeHeader *chdr = (EMAAnimationAnimNodeHeader *)GetOffsetPtr(buf, offset, true);
			ahdr->animNode_offsets[j] = val32(EMO_BaseFile::DifPointer(buf + offset, ahdr));

			
			int16_t nbKfs = node.keyframes.size();
			if (nbKfs > 65535)
			{
				printf("Error: the number of keyframes is up to 65535, witch is not possible. skipped after the limit (except we take the lastIndex, because is needed).\n");
				notifyError();

				nbKfs = (uint16_t)65535;
			}

			if ((HasSkeleton())&&(node.bone != nullptr))
				chdr->bone_idx = val16(BoneToIndex(node.bone));
			else
				chdr->bone_idx = val16(node.bone_idx);

			timesByteSize = (animation.duration > 0x100) ? 0x20 : node.timesByteSize;			// (animation.duration > 0x100) == (lastKeyFrame > 0xFF) => have to change nbBytes for code frame index.
			
			chdr->transform = node.transform;
			chdr->transformComponent = (node.transformComponent & 0x3) | ((node.noInterpolation) ? 0x4 : 0) | node.unknow_0 | node.unknow_1 | timesByteSize | indexesByteSize;
			chdr->keyframe_count = val16(nbKfs);
			offset += sizeof(EMAAnimationAnimNodeHeader);

			if (!timesByteSize)
			{
				uint8_t *timing = GetOffsetPtr(buf, offset, true);
				for (uint16_t k = 0; k < nbKfs; k++)
				{
					if ((k + 1 == nbKfs) && (nbKfs != node.keyframes.size()))
						k = node.keyframes.size() - 1;										//the lastIndex is neeeded, to not carsh the game (need duration -1)

					timing[k] = (uint8_t)node.keyframes[k].time;
					offset += sizeof(uint8_t);
				}

			}else {

				uint16_t *timing = (uint16_t *)GetOffsetPtr(buf, offset, true);
				for (uint16_t k = 0; k < nbKfs; k++)
				{
					if ((k + 1 == nbKfs) && (nbKfs != node.keyframes.size()))
						k = node.keyframes.size() - 1;										//the lastIndex is neeeded, to not carsh the game (need duration -1)

					timing[k] = val16(node.keyframes[k].time);
					offset += sizeof(uint16_t);
				}
			}


			if (offset & 3)
				offset += (4 - (offset & 3));

			uint32_t dif = EMO_BaseFile::DifPointer(buf + offset, chdr);
			assert(dif < 65536);

			chdr->indices_offset = val16(dif);

			if (!indexesByteSize)
			{
				uint16_t *indices = (uint16_t *)GetOffsetPtr(buf, offset, true);
				for (uint16_t k = 0; k < nbKfs; k++)
				{
					if ((k + 1 == nbKfs) && (nbKfs != node.keyframes.size()))
						k = node.keyframes.size() - 1;										//the lastIndex is neeeded, to not carsh the game (need duration -1)

					indices[k] = val16(node.keyframes[k].index | ( ((uint16_t)node.keyframes[k].interpolation) << 12) );
					offset += sizeof(uint16_t);
				}

			}else {
				uint32_t *indices = (uint32_t *)GetOffsetPtr(buf, offset, true);

				for (uint16_t k = 0; k < nbKfs; k++)
				{
					if ((k + 1 == nbKfs) && (nbKfs != node.keyframes.size()))
						k = node.keyframes.size() - 1;										//the lastIndex is neeeded, to not carsh the game (need duration -1)

					indices[k] = val32(node.keyframes[k].index | (((uint32_t)node.keyframes[k].interpolation) << 28));
					offset += sizeof(uint32_t);
				}
			}
		}

		if (offset & 3)
			offset += (4 - (offset & 3));


		if (animation.frame_float_size == 0)
		{
			float *values = (float *)GetOffsetPtr(buf, offset, true);
			ahdr->values_offset = val32(EMO_BaseFile::DifPointer(buf + offset, ahdr));

			for (size_t j = 0; j < animation.values.size(); j++)
			{
				values[j] = val_float(animation.values[j]);
				copy_float(values + j, animation.values[j]);
				offset += sizeof(float);
			}

		}else {

			uint16_t *values = (uint16_t *)GetOffsetPtr(buf, offset, true);
			ahdr->values_offset = val32(EMO_BaseFile::DifPointer(buf + offset, ahdr));

			for (size_t j = 0; j < animation.values.size(); j++)
			{
				copy_float16(values + j, animation.values[j]);
				offset += sizeof(uint16_t);
			}
		}
	}

	if ((offset & 0x3F) && (type != EMA_TYPE_ANIM_Light))
		offset += (0x40 - (offset & 0x3F));

	if (HasSkeleton())
	{
		unsigned int skl_size;
		uint8_t *skl = EMO_Skeleton::CreateFile(&skl_size, type);
		if (!skl)
		{
			delete[] buf;
			return nullptr;
		}

		hdr->skeleton_offset = val32(offset);
		memcpy(buf + offset, skl, skl_size);
		delete[] skl;

		offset += skl_size;

	}

	string name;
	for (uint16_t i = 0; i < nbAnimations; i++)
	{
		const EmaAnimation &animation = animations[i];
		EMAAnimationHeader *ahdr = (EMAAnimationHeader *)GetOffsetPtr(buf, anim_offsets, i);

		if (offset & 3)
			offset += (4 - (offset & 3));

		ahdr->name_offset = val32(EMO_BaseFile::DifPointer(buf + offset, ahdr));

		EMAAnimationName* emaAnimationName = (EMAAnimationName*)GetOffsetPtr(buf, offset);
		emaAnimationName->unknow_0 = val32(animation.name_unknow_0);
		emaAnimationName->unknow_1 = val32(animation.name_unknow_1);
		emaAnimationName->unknow_2 = val16(animation.name_unknow_2);
		emaAnimationName->nbChar = animation.name.length();
		offset += sizeof(EMAAnimationName);


		name = animation.name;
		if (name.length() >= 256)
		{
			printf("Error: a animation's name is too long (limit to 255). it will be cropped : %s\n", name.c_str());
			notifyError();

			name = name.substr(0, 255);
		}

		strcpy((char *)buf + offset, name.c_str());
		offset += name.length() + 1;
	}

	if (offset != file_size)
	{
		printf("Error: the size of the final file is not the same as allocated memory. so there certainly is a mistake. please contact the dev team and send them yours current sample.\n");
		notifyError();
	}

	*psize = file_size;
	return buf;
}


unsigned int EMA::CalculateFileSize() const
{
	unsigned int file_size = sizeof(EMAHeader);


	uint16_t nbAnimations = animations.size();
	if ((nbAnimations > 65535))
		nbAnimations = (uint16_t)65535;

	file_size += nbAnimations * sizeof(uint32_t);

	for (uint16_t i=0;i< nbAnimations;i++)
	{
		const EmaAnimation &a = animations.at(i);

		int16_t nbAnimationNodes = a.nodes.size();
		if ((nbAnimationNodes > 65535))
			nbAnimationNodes = (uint16_t)65535;

		if (file_size & 3)
			file_size += (4 - (file_size & 3));

		file_size += sizeof(EMAAnimationHeader) - sizeof(uint32_t);
		file_size += nbAnimationNodes * sizeof(uint32_t);

		size_t timesByteSize = (a.duration > 0x100) ? 0x20 : 0x0;							// (animation.duration > 0x100) == (lastKeyFrame > 0xFF) => have to change nbBytes for code frame index.
		size_t indexesByteSize = (a.values.size() > 0x3FFF) ? 0x40 : 0x0;

		for (uint16_t j = 0; j < nbAnimationNodes; j++)
		{
			const EMAAnimationNode &c = a.nodes.at(j);

			int16_t nbKfs = c.keyframes.size();
			if (nbKfs > 65535)
				nbKfs = (uint16_t)65535;


			if (file_size & 3)
				file_size += (4 - (file_size & 3));

			file_size += sizeof(EMAAnimationAnimNodeHeader);

			timesByteSize = (a.duration > 0x100) ? 0x20 : c.timesByteSize;						// (animation.duration > 0x100) == (lastKeyFrame > 0xFF) => have to change nbBytes for code frame index.
			if (!timesByteSize)
				file_size += nbKfs;					//uint8
			else
				file_size += nbKfs * sizeof(uint16_t);

			if (file_size & 3)
				file_size += (4 - (file_size & 3));

			if (!indexesByteSize)
				file_size += nbKfs * sizeof(uint16_t);
			else
				file_size += nbKfs * sizeof(uint32_t);
		}

		if (file_size & 3)
			file_size += (4 - (file_size & 3));

		if (a.frame_float_size == 0)
			file_size += a.values.size() * sizeof(float);
		else
			file_size += a.values.size() * sizeof(uint16_t);
	}


	if ((file_size & 0x3F) && (type != EMA_TYPE_ANIM_Light))
		file_size += (0x40 - (file_size & 0x3F));

	if (HasSkeleton())
		file_size += EMO_Skeleton::CalculateFileSize();

	string name;
	for (uint16_t i = 0; i < nbAnimations; i++)
	{
		const EmaAnimation &a = animations.at(i);

		if (file_size & 3)
			file_size += (4 - (file_size & 3));

		name = a.name;
		if (name.length() >= 256)
			name = name.substr(0, 255);

		file_size += 11 + name.length() + 1;
	}

	return file_size;
}




float EMAAnimationNode::getDefaultValue(size_t anim_type)
{
	switch (anim_type)
	{
	case EMA_ANIM_TARGET_Object:
	{
		if((transform == 0) || (transform == 1))		// "Position" || "Rotation"
		{
			switch (transformComponent)
			{
			case 0: return 0.0f; // "x"
			case 1: return 0.0f; // "y"
			case 2: return 0.0f; // "z"
			case 3: return 1.0f; // "w"
			}
		} else {										// "Scale"
			return 1.0f;
		}
	}
	break;

	case EMA_ANIM_TARGET_Camera:
	{
		if (transform != 2)								// "Position"  "TargetPosition"
		{
			switch (transformComponent)
			{
			case 0: return 0.0f; // "x";
			case 1: return 0.0f; // "y";
			case 2: return 0.0f; // "z";
			case 3: return 1.0f; // "w";
			}
		}else {
			switch (transformComponent)					// "Camera"
			{
			case 0: return 0.0f; // "roll";
			case 1: return 40.0f; // "focale";
			}
		}
	}
	break;


	case EMA_ANIM_TARGET_Light:
	{
		if (transform == 0)						//"Position"
		{
			switch (transformComponent)
			{
			case 0: return 0.0f; // "x"
			case 1: return 0.0f; // "y"
			case 2: return 0.0f; // "z"
			case 3: return 1.0f; // "w"
			}

		}else if (transform == 2) {				// "Color"
			return 1.0f;
			
		}else if (transform == 3) {				// "DegradeDistance"
			switch (transformComponent)
			{
			case 0: return 0.0f; // "start";
			case 1: return 1.0f; // "end";
			}
		}
	}
	break;


	case EMA_ANIM_TARGET_Material:
	{
		if (transform < 4) {								// "MatColX"
			return 1.0f;

		} else {											// "TexScrlX"
			return 0.0f;
		}
	}
	break;
	}
	return 0.0f;
}





















/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// load/save the Xml version	/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////



bool EMA::DecompileToFile(const std::string &path, bool show_error, bool build_path)
{
	printf("Ema is being saved to .xml file. This process may take a while.\n");
	bool ret = EMO_BaseFile::DecompileToFile(path, show_error, build_path);

	if (ret) { printf("Ema has been saved to .xml.\n"); }

	return ret;
}
TiXmlDocument *EMA::Decompile() const
{
	TiXmlDocument *doc = new TiXmlDocument();

	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "utf-8", "");
	doc->LinkEndChild(decl);

	TiXmlElement *root = new TiXmlElement("EMA");

	root->SetAttribute("version", version);
	root->SetAttribute("type", ((type == EMA_TYPE_ANIM_Object_or_Camera) ? "Object" : ((type == EMA_TYPE_ANIM_Light) ? "Light" : ((type == EMA_TYPE_ANIM_Material) ? "Material" : EMO_BaseFile::UnsignedToString(type, true)))));
	root->SetAttribute("unk_0", unknow_0);
	root->SetAttribute("unk_1", unknow_1);
	root->SetAttribute("unk_2", unknow_2);

	if (HasSkeleton())
		EMO_Skeleton::Decompile(root);

	TiXmlElement* emaAnimsNode = new TiXmlElement("Animations");
	emaAnimsNode->LinkEndChild(new TiXmlComment("f_id = floatIndex (in float_values list), interp = interpolation, QuadB = QuadraticBezier, CubicB = CubicBezier, dv = debugValues (can't be changed) and inside t=tang (value to be added to previous of segment/current point), tp= tangentPrevious (same as t), tn: tangentNext (at end of segment, to be sub to next value) "));

	for (size_t i = 0; i < animations.size(); i++)
	{
		emaAnimsNode->LinkEndChild(new TiXmlComment((string(" Index :") + ToString(i) +" nbVertex: "+ ToString(animations[i].values.size()) ).c_str()));
		animations.at(i).Decompile(emaAnimsNode);
	}
	root->LinkEndChild(emaAnimsNode);



	{						//test todo remove
		bool hyp_haveFirstKeyframe = true;
		bool hyp_haveLastKeyframe = true;
		bool hyp_haveKeyframeOrdered = true;

		for (size_t i = 0; i < animations.size(); i++)
		{
			const EmaAnimation &anim = animations.at(i);
			for (size_t j = 0; j < anim.nodes.size(); j++)
			{
				const EMAAnimationNode &animNode = anim.nodes.at(j);

				size_t lastIndex = (size_t)-1;
				for (size_t k = 0; k < animNode.keyframes.size(); k++)
				{
					const EMAKeyframe &kf = animNode.keyframes.at(k);
					
					if (k == 0)
					{
						if (kf.time != 0)
							hyp_haveFirstKeyframe = false;

						lastIndex = kf.time;
					}else {

						if(kf.time < lastIndex)
							hyp_haveKeyframeOrdered = false;
					}

					if((k + 1 == animNode.keyframes.size()) && (kf.time + 1 != anim.duration))			//the last
						hyp_haveLastKeyframe = false;
				}
			}
		}
		root->SetAttribute("hyp_haveFirstKeyframe", hyp_haveFirstKeyframe ? "true" : "false");
		root->SetAttribute("hyp_haveLastKeyframeAsDuration", hyp_haveLastKeyframe ? "true" : "false");
		root->SetAttribute("hyp_haveKeyframeOrdered", hyp_haveKeyframeOrdered ? "true" : "false");
	}



	doc->LinkEndChild(root);
	return doc;
}

void EmaAnimation::Decompile(TiXmlNode *root) const
{
	TiXmlElement *entry_root = new TiXmlElement("Animation");
	entry_root->SetAttribute("name", name);
	entry_root->SetAttribute("duration", duration);
	entry_root->SetAttribute("target", ((type == EMA_ANIM_TARGET_Object) ? "Object" : ((type == EMA_ANIM_TARGET_Camera) ? "Camera" : ((type == EMA_ANIM_TARGET_Light) ? "Light" : ((type == EMA_ANIM_TARGET_Material) ? "Material" : EMO_BaseFile::UnsignedToString(type, true) )))) );
	entry_root->SetAttribute("light_unk", light_unknow);
	entry_root->SetAttribute("floatSize", ((frame_float_size==0) ? "32bits" : ((frame_float_size == 1) ? "16bits" : ToString(frame_float_size))) );
	
	entry_root->SetAttribute("name_unk_0", name_unknow_0);
	entry_root->SetAttribute("name_unk_1", name_unknow_1);
	entry_root->SetAttribute("name_unk_2", name_unknow_2);

	for (size_t i = 0; i < nodes.size(); i++)
		nodes[i].Decompile(entry_root, values, type, duration);


	//EMO_BaseFile::WriteParamMultipleFloats(entry_root, "float_values", values);				//not perfect Float rpresentation
	string str = "";
	for (size_t i = 0; i < values.size(); i++)
		str += ((i!=0) ? ", " : "") + FloatToString(values.at(i));
	TiXmlElement *float_values_node = new TiXmlElement("float_values"); 
	entry_root->LinkEndChild(float_values_node);
	float_values_node->SetAttribute("value", str);

	

	root->LinkEndChild(entry_root);
}

void EMAAnimationNode::Decompile(TiXmlNode *root, const std::vector<float> &values, size_t anim_type, size_t duration) const
{
	TiXmlElement *entry_root = new TiXmlElement("AnimationNode");
	entry_root->SetAttribute("name", ((bone) ? bone->GetName() : ""));
	entry_root->SetAttribute("refIndex", bone_idx);

	entry_root->SetAttribute("timesByteSize", ((timesByteSize == 0) ? "8bits" : "16bits"));						//oblige because some of StIv files don't respect the rule with duration.

	string transformStr = EMO_BaseFile::UnsignedToString(transform, false);
	string transformComponentStr = EMO_BaseFile::UnsignedToString(transformComponent, false);
	switch (anim_type)
	{
	case EMA_ANIM_TARGET_Object:
		{
			switch (transform)
			{
			case 0: transformStr = "Position"; break;
			case 1: transformStr = "Rotation"; break;
			case 2: transformStr = "Scale"; break;
			}

			switch (transformComponent)
			{
			case 0: {transformComponentStr = "x"; } break;
			case 1: {transformComponentStr = "y"; } break;
			case 2: {transformComponentStr = "z"; } break;
			case 3: {transformComponentStr = "w"; } break;
			}
		}
		break;

	case EMA_ANIM_TARGET_Camera:
		{
			switch (transform)
			{
			case 0: transformStr = "Position"; break;
			case 1: transformStr = "TargetPosition"; break;
			case 2: transformStr = "Camera"; break;
			}

			if (transform != 2)
			{
				switch (transformComponent)
				{
				case 0: {transformComponentStr = "x"; } break;
				case 1: {transformComponentStr = "y"; } break;
				case 2: {transformComponentStr = "z"; } break;
				case 3: {transformComponentStr = "w"; } break;
				}
			}else {
				switch (transformComponent)
				{
				case 0: {transformComponentStr = "roll"; } break;
				case 1: {transformComponentStr = "focale"; } break;
				}
			}
		}
		break;


	case EMA_ANIM_TARGET_Light:
		{
			switch (transform)
			{
			case 0: transformStr = "Position"; break;
			case 2: transformStr = "Color"; break;
			case 3: transformStr = "DegradeDistance"; break;
			}

			if (transform == 0)
			{
				switch (transformComponent)
				{
				case 0: {transformComponentStr = "x"; } break;
				case 1: {transformComponentStr = "y"; } break;
				case 2: {transformComponentStr = "z"; } break;
				case 3: {transformComponentStr = "w"; } break;
				}
			}else if (transform == 2) {
				switch (transformComponent)
				{
				case 0: {transformComponentStr = "r"; } break;
				case 1: {transformComponentStr = "g"; } break;
				case 2: {transformComponentStr = "b"; } break;
				case 3: {transformComponentStr = "a"; } break;
				}
			}else if (transform == 3) {				//degrade
				switch (transformComponent)
				{
				case 0: {transformComponentStr = "start"; } break;
				case 1: {transformComponentStr = "end"; } break;
				}
			}
		}
		break;


	case EMA_ANIM_TARGET_Material:
		{
			switch (transform)
			{
			case 0: transformStr = "MatCol0"; break;
			case 1: transformStr = "MatCol1"; break;
			case 2: transformStr = "MatCol2"; break;
			case 3: transformStr = "MatCol3"; break;
			case 4: transformStr = "TexScrl0"; break;
			case 5: transformStr = "TexScrl1"; break;
			case 6: transformStr = "TexScrl2"; break;				//logical interpolation from shader entries, but none of Dbvx2 and SSSS use it
			case 7: transformStr = "TexScrl3"; break;				//logical interpolation from shader entries, but none of Dbvx2 and SSSS use it
			}

			if (transform < 4) {
				switch (transformComponent)
				{
				case 0: {transformComponentStr = "r"; } break;
				case 1: {transformComponentStr = "g"; } break;
				case 2: {transformComponentStr = "b"; } break;
				case 3: {transformComponentStr = "a"; } break;
				}
			}else{
				switch (transformComponent)
				{
				case 0: {transformComponentStr = "u"; } break;
				case 1: {transformComponentStr = "v"; } break;
				}
			}
		}
		break;
	}


	entry_root->SetAttribute("transform", transformStr);
	entry_root->SetAttribute("component", transformComponentStr);
	entry_root->SetAttribute("noInterpolation", noInterpolation ? "true" : "false");
	entry_root->SetAttribute("unk_0", unknow_0);
	entry_root->SetAttribute("unk_1", unknow_1);
	
	for(size_t i=0,nb= keyframes.size();i<nb;i++)
		keyframes.at(i).Decompile(entry_root, values, (i+1<nb) ? &keyframes.at(i+1) : 0);

	root->LinkEndChild(entry_root);
}
void EMAKeyframe::Decompile(TiXmlNode *root, const std::vector<float> &values, const EMAKeyframe* next_keyframe) const
{
	TiXmlElement *entry_root = new TiXmlElement("Keyframe");

	bool isQuadraticBezier = ((interpolation & 0x4)!=0);
	bool isCubicBezier = ((interpolation & 0x8)!=0);

	entry_root->SetAttribute("frame", time);
	entry_root->SetAttribute("f_id", EMO_BaseFile::UnsignedToString(index, false));
	entry_root->SetAttribute("interp", ((isQuadraticBezier) ? "QuadB" : ((isCubicBezier) ? "CubicB" : "Linear")) );

	if ((isQuadraticBezier)&&(next_keyframe))
	{
		float value_p = values.at(index);
		float tangent_p = values.at(index + 1);
		//float value_n = values.at(next_keyframe->index);
		entry_root->SetAttribute("dv", (FloatToString(value_p) +", t:"+ FloatToString(tangent_p)).c_str() );

	}else if ((isCubicBezier) && (next_keyframe)) {

		float value_p = values.at(index);
		float tangent_p = values.at(index + 1);
		float tangent_n = values.at(index + 2);
		//float value_n = values.at(next_keyframe->index);
		entry_root->SetAttribute("dv", (FloatToString(value_p) + ", tp:" + FloatToString(tangent_p) + ", tn:" + FloatToString(tangent_n)).c_str());
	}else {
		float value = values.at(index);
		entry_root->SetAttribute("dv", (FloatToString(value)).c_str());
	}

	

	root->LinkEndChild(entry_root);
}













bool EMA::CompileFromFile(const std::string &path, bool show_error, bool big_endian)
{
	printf("Ema is being loaded from .xml file. This process may take a while.\n");
	bool ret = EMO_BaseFile::CompileFromFile(path, show_error, big_endian);

	if (ret){ printf("Ema has been loaded from .xml.\n"); }

	return ret;
}
bool EMA::Compile(TiXmlDocument *doc, bool big_endian)
{
	Reset();
	this->big_endian = big_endian;

	TiXmlHandle handle(doc);

	const TiXmlElement *root = EMO_BaseFile::FindRoot(&handle, "EMA");
	if (!root)
	{
		printf("Cannot find\"EMA\" in xml.\n");
		notifyError();
		return false;
	}

	uint32_t tmp = 0;
	string str = "";
	root->QueryStringAttribute("version", &version);
	root->QueryUnsignedAttribute("type", &tmp); type = (uint16_t)tmp;

	root->QueryStringAttribute("type", &str);
	type = ((str == "Object") ? EMA_TYPE_ANIM_Object_or_Camera : ((str == "Light") ? EMA_TYPE_ANIM_Light : ((str == "Material") ? EMA_TYPE_ANIM_Material : EMO_BaseFile::GetUnsigned(str))));

	context = (type == EMA_TYPE_ANIM_Material) ? ContextUse::MaterialAnimation : ContextUse::ObjectAnimation;


	root->QueryUnsignedAttribute("unk_0", &unknow_0);
	root->QueryUnsignedAttribute("unk_1", &unknow_1);
	root->QueryUnsignedAttribute("unk_2", &unknow_2);


	mHaveExtraBytesOnEachBone = (type == EMA_TYPE_ANIM_Object_or_Camera);

	EMO_Skeleton::Compile(root->FirstChildElement("Skeleton"));

	const TiXmlElement* animations_node = root->FirstChildElement("Animations");
	for (const TiXmlElement *elem = (animations_node) ? animations_node->FirstChildElement("Animation") : 0; elem; elem = elem->NextSiblingElement("Animation"))
	{
		EmaAnimation emaAnim;
		if (!emaAnim.Compile(elem, *this))
		{
			printf("%s: Compilation of Animation failed.\n", FUNCNAME);
			notifyError();
			continue;
		}

		animations.push_back(emaAnim);
	}

	if ((animations.size() != 0) && (animations.at(0).type == EMA_ANIM_TARGET_Camera))
		mHaveExtraBytesOnEachBone = false;

	return true;
}

bool EmaAnimation::Compile(const TiXmlElement *root, EMO_Skeleton &skl)
{
	uint32_t tmp;
	string str;

	root->QueryStringAttribute("name", &name);
	root->QueryUnsignedAttribute("duration", &tmp); duration = (uint16_t)tmp;
	if (duration <= 1)
		duration = 1;
	
	root->QueryStringAttribute("target", &str);
	type = ((str == "Object") ? EMA_ANIM_TARGET_Object : ((str == "Camera") ? EMA_ANIM_TARGET_Camera : ((str == "Light") ? EMA_ANIM_TARGET_Light : ((str == "Material") ? EMA_ANIM_TARGET_Material : EMO_BaseFile::GetUnsigned(str) ))));

	root->QueryUnsignedAttribute("light_unk", &tmp); light_unknow = (uint8_t)tmp;

	root->QueryStringAttribute("floatSize", &str);
	frame_float_size = ((str == "32bits") ? 0 : ((str == "16bits") ? 1 : std::stoi(str)));

	root->QueryUnsignedAttribute("name_unk_0", &name_unknow_0);
	root->QueryUnsignedAttribute("name_unk_1", &name_unknow_1);
	root->QueryUnsignedAttribute("name_unk_2", &tmp); name_unknow_2 = (uint16_t)tmp;


	for (const TiXmlElement *elem = root->FirstChildElement("AnimationNode"); elem; elem = elem->NextSiblingElement("AnimationNode"))
	{
		EMAAnimationNode emaAnimNode;
		if (!emaAnimNode.Compile(elem, skl))
		{
			printf("%s: Compilation of node failed.\n", FUNCNAME);
			notifyError();
			continue;
		}

		nodes.push_back(emaAnimNode);
	}

	



	//if (!EMO_BaseFile::GetParamMultipleFloats(root, "float_values", values))		//not perfect Float rpresentation
	//	return false;

	const TiXmlElement* node = root->FirstChildElement("float_values");
	if (!node)
		return false;

	values.clear();
	str = "";
	node->QueryStringAttribute("value", &str);
	std::vector<string> sv = split(str, ',');
	for (size_t i = 0; i < sv.size(); i++)
		values.push_back( StringToFloat( rtrim( ltrim(sv.at(i) ) ) ));

	return true;
}
bool EMAAnimationNode::Compile(const TiXmlElement *root, EMO_Skeleton &skl)
{
	std::string str;
	size_t tmp = 0;
	root->QueryStringAttribute("name", &str); 
	this->bone = skl.GetBone(str);

	root->QueryUnsignedAttribute("refIndex", &tmp);
	bone_idx = (uint16_t)tmp;

	string transformStr = "";
	string transformComponentStr = "";
	root->QueryStringAttribute("transform", &transformStr);
	root->QueryStringAttribute("component", &transformComponentStr); 

	root->QueryStringAttribute("timesByteSize", &str); timesByteSize = ((str == "8bits") ? 0 : 0x20);


	if		((transformStr == "Position")															 || (transformStr == "MatCol0")) transform = 0;
	else if ((transformStr == "Rotation") || (transformStr == "TargetPosition")						 || (transformStr == "MatCol1")) transform = 1;
	else if ((transformStr == "Scale")	  || (transformStr == "Camera") || (transformStr == "Color") || (transformStr == "MatCol2")) transform = 2;
	else if														((transformStr == "DegradeDistance") || (transformStr == "MatCol3")) transform = 3;
	else if (transformStr == "TexScrl0") transform = 4;
	else if (transformStr == "TexScrl1") transform = 5;
	else if (transformStr == "TexScrl2") transform = 6;
	else if (transformStr == "TexScrl3") transform = 7;
	else transform = std::stoi(transformStr);


	if		((transformComponentStr == "x") || (transformComponentStr == "r") || (transformComponentStr == "roll") || (transformComponentStr == "start") || (transformComponentStr == "u")) transformComponent = 0;
	else if ((transformComponentStr == "y") || (transformComponentStr == "g") || (transformComponentStr == "focale") || (transformComponentStr == "end") || (transformComponentStr == "v")) transformComponent = 1;
	else if ((transformComponentStr == "z") || (transformComponentStr == "b")) transformComponent = 2;
	else if ((transformComponentStr == "w") || (transformComponentStr == "a")) transformComponent = 3;
	else transformComponent = std::stoi(transformComponentStr);


	root->QueryStringAttribute("noInterpolation", &str); noInterpolation = (str == "true");
	root->QueryUnsignedAttribute("unk_0", &tmp); unknow_0 = (uint8_t)tmp;
	root->QueryUnsignedAttribute("unk_1", &tmp); unknow_1 = (uint8_t)tmp;


	for (const TiXmlElement *elem = root->FirstChildElement("Keyframe"); elem; elem = elem->NextSiblingElement("Keyframe"))
	{
		EMAKeyframe keyframe;
		if (!keyframe.Compile(elem))
			continue;

		keyframes.push_back(keyframe);
	}

	return true;
}

bool EMAKeyframe::Compile(const TiXmlElement *root)
{
	uint32_t tmp;
	root->QueryUnsignedAttribute("frame", &tmp); time = (uint16_t)tmp;
	root->QueryUnsignedAttribute("f_id", &index);

	string str = "Linear";
	root->QueryStringAttribute("interp", &str);
	interpolation = ((str == "QuadB") ? 0x4 : ((str == "CubicB") ? 0x8 : 0x0));

	return true;
}





























/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// EAN conversions	/////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


/*-------------------------------------------------------------------------------\
|                             readEAN				                             |
\-------------------------------------------------------------------------------*/
void EMA::readEAN(EAN* ean, bool forceOrientationInterpolation)
{
	name = ean->name;
	version = ean->version;
	
	type = EMA_TYPE_ANIM_Object_or_Camera;
	switch (ean->type)
	{
	case LIBXENOVERSE_EAN_ANIMATION_TYPE_NORMAL: type = EMA_TYPE_ANIM_Object_or_Camera; break;
	case LIBXENOVERSE_EAN_ANIMATION_TYPE_CAMERA: type = EMA_TYPE_ANIM_Object_or_Camera; break;
	case EMA_ANIM_TARGET_Light: type = EMA_TYPE_ANIM_Light; break;
	case EMA_ANIM_TARGET_Material: type = EMA_TYPE_ANIM_Material; break;
	}

	readEsk(ean->getSkeleton());
	
	size_t nbElements = ean->animations.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		animations.push_back(EmaAnimation());
		animations.back().readEANAnimation(&ean->animations.at(i), ean->getSkeleton(), this, forceOrientationInterpolation);
	}
}

/*-------------------------------------------------------------------------------\
|                             readEsk				                             |
\-------------------------------------------------------------------------------*/
void EMA::readEsk(ESK* esk)
{
	name = esk->name;
	skeletonUniqueId = esk->skeletonUniqueId;

	EskTreeNode* rootNode = esk->getTreeOrganisation();

	vector<EskTreeNode*> newListBonesOrdered;
	rootNode->getTreeBonesList(newListBonesOrdered);					//we get the list by following the hierarchy. it's for not having miss of a parent.

	bones.clear();

	EskTreeNode* node;
	size_t nbBones = newListBonesOrdered.size();
	for (size_t i = 1; i < nbBones; i++)					//we begin by 1 because the first isn't a bone.
	{
		node = newListBonesOrdered.at(i);

		bones.push_back(EMO_Bone());
		bones.back().readESKBone(node->mBone);
	}


	for (size_t i = 1; i < nbBones; i++)					//same
	{
		node = newListBonesOrdered.at(i);

		if ((node->mParent) && (node->mParent->mBone))
		{
			string parentName = node->mParent->mBone->getName();
			for (size_t j = i - 1; j < i; --j)
			{
				if (bones.at(j).GetName() == parentName)
				{
					bones.at(i - 1).parent = &(bones.at(j));

					if (bones.at(j).child == nullptr)
					{
						bones.at(j).child = &bones.at(i - 1);

					}else{

						size_t inc = 0;
						EMO_Bone* childBone = bones.at(j).child;

						while (childBone->sibling != nullptr)
						{
							childBone = childBone->sibling;
							++inc;
						}
						++inc;

						childBone->sibling = &bones.at(i - 1);
						//bones.at(i - 1).indexInChildren = inc;					// apparently with Ema , is all Null = 0xffff.
					}

					break;
				}
			}
		}
	}




	//Ik datas
	listInverseKinematic.clear();
	size_t nbBones_all = bones.size();
	size_t nbGroup = esk->listInverseKinematic.size();
	for (size_t i = 0; i < nbGroup; i++)
	{
		Esk_IK_Group &group = esk->listInverseKinematic.at(i);
		listInverseKinematic.push_back(Emo_IK_Group());

		size_t nbIk = group.mListIK.size();
		for (size_t j = 0; j < nbIk; j++)
		{
			Esk_IK_Relation &ik = group.mListIK.at(j);
			listInverseKinematic.back().mListIK.push_back(Emo_IK_Relation());
			Emo_IK_Relation &ik_b = listInverseKinematic.back().mListIK.back();

			size_t nbBones = ik.mListBones.size();
			for (size_t k = 0; k < nbBones; k++)
			{
				ESKBone* bone = ik.mListBones.at(k).bone;
				EMO_Bone* bone_b = 0;

				for (size_t m = 0; m < nbBones_all; m++)
				{
					if (bones.at(m).name == bone->getName())
					{
						bone_b = &bones.at(m);
						break;
					}
				}
				if (!bone_b)
					continue;

				ik_b.mListBones.push_back(Emo_IK_Relation::IKR_Bone(bone_b, ik.mListBones.at(k).value));
			}
		}
	}
}

/*-------------------------------------------------------------------------------\
|                             readEANAnimation		                             |
\-------------------------------------------------------------------------------*/
void EmaAnimation::readEANAnimation(EANAnimation* ean, ESK* esk, EMO_Skeleton* emoSkeleton, bool forceOrientationInterpolation)
{
	name = ean->name;
	duration = ean->duration;
	frame_float_size = ((ean->frame_float_size == 1) ? 1 : 0);

	size_t ean_type = ean->getParent()->getType();
	type = ean_type;
	

	//Todo deal with the rest depend of the type.

	ESKBone* eskBone;
	EMO_Bone* emo_Bone;
	size_t nbElements = ean->nodes.size();
	size_t currentTransform = 0;

	for (size_t m = 0; m < 3; m++)								//we pass from position to Orientation to scale.
	{
		switch (m)
		{
		case 0: currentTransform = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION; break;
		case 1: currentTransform = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION_or_TargetPosition; break;
		case 2: currentTransform = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE_or_CAMERA; break;
		}



		for (size_t i = 0; i < nbElements; i++)						//node is about a couple Bone+Transform+transformComponent, but EanAnimationNode is only bone, and under, EanKeyframeAnimation is about transform (position, rotation, scale).
		{
			emo_Bone = 0;
			eskBone = esk->getBone(ean->nodes.at(i).bone_index);
			if (eskBone)
				emo_Bone = emoSkeleton->GetBone(eskBone->getName());

			size_t nbKeyFramAnim = ean->nodes.at(i).keyframed_animations.size();
			for (size_t j = 0; j < nbKeyFramAnim; j++)
			{
				if (currentTransform != ean->nodes.at(i).keyframed_animations.at(j).getFlag())			//it will do the transform's order.
					continue;

				for (size_t k = 0; k < 3; k++)					//so the part for each component of the transform component.
				{
					nodes.push_back(EMAAnimationNode());
					nodes.back().bone = emo_Bone;
					//nodes.back().transformComponent = k;			//0 for X, 1 one Y, 2 for Z.
					nodes.back().transformComponent = k + 0x8;			//0 for X, 1 one Y, 2 for Z. TODO find what is the first left bit for. and also the component 3. it's W for light and material animations.

					nodes.back().readEANKeyframedAnimation(&ean->nodes.at(i).keyframed_animations.at(j), values, k, duration, forceOrientationInterpolation);
				}
			}
		}
	}
}

/*-------------------------------------------------------------------------------\
|                             readANKeyframedAnimation                           |
\-------------------------------------------------------------------------------*/
void EMAAnimationNode::readEANKeyframedAnimation(EANKeyframedAnimation* ean, std::vector<float> &values, size_t currComponent, size_t duration, bool forceOrientationInterpolation)
{
	size_t flag_tmp = 0;
	switch (ean->flag)
	{
	case LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION: flag_tmp = 0; break;
	case LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION_or_TargetPosition: flag_tmp = 1; break;
	case LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE_or_CAMERA: flag_tmp = 2; break;
	default:
	{
		//todo ajouter un warning.
		//assert(false);
	}
	}

	transform = flag_tmp;


	size_t nbElements = ean->keyframes.size();
	for (size_t i = 0; i < nbElements; i++)						//node is about a couple Bone+Transform, but EanAnimationNode is only bone, and under, EanKeyframeAnimation is about transform (position, rotation, scale).
	{
		EANKeyframe &kf = ean->keyframes.at(i);


		float value = 0.0;
		if (ean->flag != LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION_or_TargetPosition)
		{
			keyframes.push_back(EMAKeyframe());
			keyframes.back().time = kf.frame;

			if (currComponent == 0)
				value = kf.x;
			else if (currComponent == 1)
				value = kf.y;
			else if (currComponent == 2)
				value = kf.z;
			else if (currComponent == 3)
				value = kf.w;

		}else{

			if ((!forceOrientationInterpolation) || (i + 1 == nbElements))
			{
				keyframes.push_back(EMAKeyframe());
				keyframes.back().time = kf.frame;

				//Quaternion -> Euler Angle (in real, is TaitBryan angles)
				FbxDouble3 angles = giveAngleOrientationForThisOrientationTaitBryan_XYZ(FbxVector4(kf.x, kf.y, kf.z, kf.w));		// yaws, pitch, roll

				//Xenoverse data is on XYZ order.
				if (currComponent == 0)					//rotation for X axis
					value = (float)angles[2];					//roll
				else if (currComponent == 1)			//for Y axis
					value = (float)angles[1];					//yaw on disc from pitch.
				else if (currComponent == 2)			//for Z axis
					value = (float)angles[0];					//pitch

				/*
				//test TODO REMOVE ( this test <=>remove if (ean->flag != LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION) in certain case )
				if (currComponent == 0)					//rotation for X axis
					value = kf.x;					//roll
				else if (currComponent == 1)			//for Y axis
					value = kf.y;					//yaw on disc from pitch.
				else if (currComponent == 2)			//for Z axis
					value = kf.z;					//pitch
				*/

			}
			else {									// force to have all keyframe for orientation, to avoid weird interpolation between result of the conversion from quaternion to EulerAngles XYZ.

				EANKeyframe &kf_next = ean->keyframes.at(i + 1);
				size_t prevFrame = kf.frame;
				size_t nextFrame = kf_next.frame;
				size_t nbFrames = nextFrame - prevFrame;

				if (nbFrames != 0)
				{
					double factor = 0.0;
					for (size_t j = 0; j < nbFrames; j++)
					{
						factor = (double)j / (double)nbFrames;

						keyframes.push_back(EMAKeyframe());
						keyframes.back().time = prevFrame + j;


						//Todo slerp between kf, kf_next, factor => x y z w
						FbxQuaternion prevQuat = FbxQuaternion(kf.x, kf.y, kf.z, kf.w);
						FbxQuaternion nextQuat = FbxQuaternion(kf_next.x, kf_next.y, kf_next.z, kf_next.w);
						FbxQuaternion currQuat = prevQuat.Slerp(nextQuat, factor);


						FbxDouble3 angles = giveAngleOrientationForThisOrientationTaitBryan_XYZ(FbxVector4(currQuat[0], currQuat[1], currQuat[2], currQuat[3]));		// yaws, pitch, roll

						//Xenoverse data is on XYZ order.
						if (currComponent == 0)					//rotation for X axis
							value = (float)angles[2];					//roll
						else if (currComponent == 1)			//for Y axis
							value = (float)angles[1];					//yaw on disc from pitch.
						else if (currComponent == 2)			//for Z axis
							value = (float)angles[0];					//pitch


						//to reduce the number of Values (goal of this format)
						size_t isfound = (size_t)-1;
						size_t nbValues = values.size();
						for (size_t j = 0; j < nbValues; j++)
						{
							if (values.at(j) == value)
							{
								isfound = j;
								break;
							}
						}
						if (isfound == (size_t)-1)
						{
							isfound = nbValues;
							values.push_back(value);
						}

						keyframes.back().index = isfound;
					}
					continue;



				}
				else {				//secu

					keyframes.push_back(EMAKeyframe());
					keyframes.back().time = kf.frame;

					FbxDouble3 angles = giveAngleOrientationForThisOrientationTaitBryan_XYZ(FbxVector4(kf.x, kf.y, kf.z, kf.w));		// yaws, pitch, roll
					if (currComponent == 0)					//rotation for X axis
						value = (float)angles[2];					//roll
					else if (currComponent == 1)			//for Y axis
						value = (float)angles[1];					//yaw on disc from pitch.
					else if (currComponent == 2)			//for Z axis
						value = (float)angles[0];					//pitch
				}
			}
		}

		//to reduce the number of Values (goal of this format)
		size_t isfound = (size_t)-1;
		size_t nbValues = values.size();
		for (size_t j = 0; j < nbValues; j++)
		{
			if (values.at(j) == value)
			{
				isfound = j;
				break;
			}
		}
		if (isfound == (size_t)-1)
		{
			isfound = nbValues;
			values.push_back(value);
		}

		keyframes.back().index = isfound;
	}


	//apparently, not having the last keyframe make the game in infinte loop, so we will check for having frame 0 and last from duration
	if (keyframes.size())
	{
		if (keyframes.at(0).time != 0)
		{
			keyframes.insert(keyframes.begin(), keyframes.at(0));					//clone the first index
			keyframes.at(0).time = 0;
		}

		if ((duration > 0) && (keyframes.back().time + 1 != duration))
		{
			keyframes.push_back(keyframes.back());					//clone the first index
			keyframes.back().time = duration - 1;
		}
	}

}














/*-------------------------------------------------------------------------------\
|                             writeEAN				                             |
\-------------------------------------------------------------------------------*/
void EMA::writeEAN(EAN* ean)
{
	ean->name = name;
	ean->version = version;
	ean->type = (animations.size()) ? animations.at(0).type : LIBXENOVERSE_EAN_ANIMATION_TYPE_NORMAL;
	
	ESK* skeleton = new ESK();
	writeEsk(skeleton);
	ean->setSkeleton(skeleton);

	size_t nbElements = animations.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		ean->animations.push_back(EANAnimation());
		ean->animations.back().setParent(ean);
		animations.at(i).writeEANAnimation(&ean->animations.back(), skeleton);
	}
}
/*-------------------------------------------------------------------------------\
|                             writeEANAnimation		                             |
\-------------------------------------------------------------------------------*/
void EmaAnimation::writeEANAnimation(EANAnimation* ean, ESK* esk)
{
	ean->name = name;
	ean->duration = duration;
	ean->frame_float_size = (unsigned char)((frame_float_size == 1) ? 1 : 2);


	//node is about a couple Bone+Transform+TransformComponent,
	//but EanAnimationNode is only bone, and under, EanKeyframeAnimation is about transform (position, rotation, scale).
	//so the first we have to do is to regroupe node by Bone, transform and transformComponent. withc will easiers to use.
	struct EmaBoneTransformComponent
	{
		size_t transformComponentId;								//0:X, 1:Y, 2:Z 
		EMAAnimationNode* node;
	};
	struct EmaBoneTransform
	{
		size_t transformId;								//0:position, 1:rotation, 2:scale
		std::vector<EmaBoneTransformComponent> list;
	};
	struct EmaBone
	{
		EMO_Bone* emo_Bone;
		std::vector<EmaBoneTransform> list;
	};
	std::vector<EmaBone> listByBone;



	std::vector<bool> isValuesReallyUsed;  //test Todo remove
	isValuesReallyUsed.resize(values.size(), false);



	EMAAnimationNode* ema_node;
	EmaBone* emaBone;
	EmaBoneTransform* emaBoneTransform;
	EmaBoneTransformComponent* emaBoneTransformComponent;
	EMO_Bone* emo_Bone;
	EANAnimationNode* node;
	size_t boneIndex;
	size_t nbBones;
	size_t nbTransform;
	size_t nbTransformComponent;
	size_t nbElements = nodes.size();
	for (size_t i = 0; i < nbElements; i++)
	{
		ema_node = &(nodes.at(i));
		emo_Bone = ema_node->bone;

		emaBone = 0;
		nbBones = listByBone.size();
		for (size_t j = 0; j < nbBones; j++)
		{
			if (listByBone.at(j).emo_Bone == emo_Bone)
			{
				emaBone = &(listByBone.at(j));
				break;
			}
		}
		if (!emaBone)
		{
			listByBone.push_back(EmaBone());
			emaBone = &listByBone.back();
			emaBone->emo_Bone = emo_Bone;

			emaBone->list.push_back(EmaBoneTransform());
			emaBoneTransform = &emaBone->list.back();
			emaBoneTransform->transformId = ema_node->transform;

			emaBoneTransform->list.push_back(EmaBoneTransformComponent());
			emaBoneTransformComponent = &emaBoneTransform->list.back();
			emaBoneTransformComponent->transformComponentId = ema_node->transformComponent & 0x7;		//0x7 is limiting about 3 component (auras have a 0x9 for 0x1 + a flag at 0x8)
			emaBoneTransformComponent->node = ema_node;

			continue;
		}


		emaBoneTransform = 0;
		nbTransform = emaBone->list.size();
		for (size_t j = 0; j < nbTransform; j++)
		{
			if (emaBone->list.at(j).transformId == ema_node->transform)
			{
				emaBoneTransform = &(emaBone->list.at(j));
				break;
			}
		}
		if (!emaBoneTransform)
		{
			emaBone->list.push_back(EmaBoneTransform());
			emaBoneTransform = &emaBone->list.back();
			emaBoneTransform->transformId = ema_node->transform;

			emaBoneTransform->list.push_back(EmaBoneTransformComponent());
			emaBoneTransformComponent = &emaBoneTransform->list.back();
			emaBoneTransformComponent->transformComponentId = ema_node->transformComponent;
			emaBoneTransformComponent->node = ema_node;
			continue;
		}


		emaBoneTransformComponent = 0;
		nbTransformComponent = emaBoneTransform->list.size();
		for (size_t j = 0; j < nbTransformComponent; j++)
		{
			if (emaBoneTransform->list.at(j).transformComponentId == ema_node->transformComponent)
			{
				emaBoneTransformComponent = &(emaBoneTransform->list.at(j));
				break;
			}
		}
		if (!emaBoneTransformComponent)
		{
			emaBoneTransform->list.push_back(EmaBoneTransformComponent());
			emaBoneTransformComponent = &emaBoneTransform->list.back();
			emaBoneTransformComponent->transformComponentId = ema_node->transformComponent;
			emaBoneTransformComponent->node = ema_node;
			continue;
		}

		//not a good case because there is a duplicate component. TODO add a warning.
		//assert(false);
	}



	//now we could make ean.
	nbBones = listByBone.size();
	for (size_t i = 0; i < nbBones; i++)
	{
		boneIndex = -1;
		emaBone = &listByBone.at(i);
		emo_Bone = emaBone->emo_Bone;
		if(emo_Bone)
			boneIndex = esk->getBoneIndex(emo_Bone->GetName());		//Note: if boneIndex = -1 because there is no bone, it still work (ex : for material animation)

		node = 0;
		size_t nbNodes = ean->nodes.size();
		for (size_t j = 0; j < nbNodes; j++)
		{
			if (ean->nodes.at(j).getBoneIndex()== boneIndex)
			{
				node = &ean->nodes.at(j);
				break;
			}
		}
		if (!node)
		{
			ean->nodes.push_back(EANAnimationNode());
			node = &ean->nodes.back();
			node->setBoneIndex(boneIndex);							//not -1 , it's a kind of root.
		}

		nbTransform = emaBone->list.size();
		for (size_t j = 0; j < nbTransform; j++)
		{
			emaBoneTransform = &emaBone->list.at(j);

			size_t flag_tmp = 0;
			switch (emaBoneTransform->transformId)
			{
				case 0: flag_tmp = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION; break;
				case 1: flag_tmp = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION_or_TargetPosition; break;
				case 2: flag_tmp = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE_or_CAMERA; break;
				default:
				{
					printf("Warning: unknow flag %s (different position/rotation/scale)\n", EMO_BaseFile::UnsignedToString(flag_tmp, true));
					LibXenoverse::notifyWarning();
				}
			}


			node->keyframed_animations.push_back(EANKeyframedAnimation());
			EANKeyframedAnimation* eanKFAnim = &node->keyframed_animations.back();
			eanKFAnim->setFlag(flag_tmp);
			

			//here, the trouble is about one animation by channel, so you could not have the same keyframe on all channel.
			//so we have to merge list of frameIndex on each components.
			std::vector<size_t> listIndexMerged;
			nbTransformComponent = emaBoneTransform->list.size();
			for (size_t k=0; k < nbTransformComponent; k++)
			{
				ema_node = emaBoneTransform->list.at(k).node;

				size_t nbFrames = ema_node->keyframes.size();
				for (size_t m=0; m < nbFrames; m++)
				{
					size_t time = ema_node->keyframes.at(m).time;
					
					size_t nbIndex = listIndexMerged.size();
					bool isfound = false;
					for (size_t n=0; n < nbIndex; n++)
					{
						if (listIndexMerged.at(n)== time)
						{
							isfound = true;
							break;
						}
					}
					if (!isfound)
						listIndexMerged.push_back(time);
				}
			}

			struct TestToOrder { bool operator() (const size_t & a, const size_t & b) const { return ((a == b)||(a < b)); } };
			std::sort(listIndexMerged.begin(), listIndexMerged.end(), TestToOrder());



			size_t nbIndex = listIndexMerged.size();
			for (size_t n = 0; n < nbIndex; n++)
			{
				eanKFAnim->keyframes.push_back(EANKeyframe());
				EANKeyframe* kf = &eanKFAnim->keyframes.back();

				size_t time = listIndexMerged.at(n);
				kf->frame = time;

				for (size_t k=0; k < nbTransformComponent; k++)
				{
					emaBoneTransformComponent = &emaBoneTransform->list.at(k);
					emaBoneTransformComponent->node->writeEANKeyframe(kf, values, time, isValuesReallyUsed);
				}

				//need to convert Euler Angles to Quaternion
				if (flag_tmp == LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION_or_TargetPosition)
				{
					float rotX = kf->x;
					float rotY = kf->y;
					float rotZ = kf->z;
					
					kf->x = kf->y = kf->z = 0.0;
					kf->w = 1.0;
					
					FbxVector4 quatX = fromAngleAxis(rotX, FbxDouble3(1, 0, 0) );
					FbxVector4 quatY = fromAngleAxis(rotY, FbxDouble3(0, 1, 0));
					FbxVector4 quatZ = fromAngleAxis(rotZ, FbxDouble3(0, 0, 1));

					FbxVector4 quat = quatMulQuat(quatZ, quatMulQuat(quatY, quatX)); //XYZ		=> not like the XYZ order in ean, strange. (but it's the only order solution, others give weird)

					kf->x = (float)quat[0];
					kf->y = (float)quat[1];
					kf->z = (float)quat[2];
					kf->w = (float)quat[3];
				}
			}


			
		}
	}


	
	//test Todo remove
	std::vector<size_t> listUnUsed;

	size_t nbValues = isValuesReallyUsed.size();
	for (size_t i = 0; i < nbValues; i++)
		if (!isValuesReallyUsed.at(i))
			listUnUsed.push_back(i);

	if(listUnUsed.size() != 0)
	{
		int aa = 42;
	}
}
/*-------------------------------------------------------------------------------\
|                             writeEANKeyframedAnimation                         |
\-------------------------------------------------------------------------------*/
void EMAAnimationNode::writeEANKeyframe(EANKeyframe* ean, std::vector<float> &values, size_t frame, std::vector<bool> &isValuesReallyUsed)
{
	uint8_t transformComponent_tmp = transformComponent;
	
	size_t nbElements = keyframes.size();
	if (nbElements == 0)
	{
		if (transform == 2)				//default for scale
		{
			if (transformComponent_tmp == 0)
				ean->x = 1.0;
			else if (transformComponent_tmp == 1)
				ean->y = 1.0;
			else if (transformComponent_tmp == 2)
				ean->z = 1.0;
			else if (transformComponent_tmp == 3)
				ean->w = 1.0;
			//else
				//assert(false);
		}

		return;
	}

	
	size_t index_p = (size_t)-1;					//prev
	size_t index_n = (size_t)-1;					//next
	float factor = 0.0;								//between prev and next.
	if (frame <= keyframes.at(0).time)
	{
		index_p = 0;
		index_n = 0;
	}
	if (frame >= keyframes.back().time)
	{
		index_p = keyframes.size() - 1;
		index_n = index_p;
	}

	if (index_p == (size_t)-1)
	{
		for (size_t i = 1; i < nbElements; i++)						//Command is about a couple Bone+Transform, but EanAnimationNode is only bone, and under, EanKeyframeAnimation is about transform (position, rotation, scale).
		{
			if (frame == keyframes.at(i).time)
			{
				index_p = i;
				index_n = i;
				factor = 0.0;
				break;

			} else if (frame < keyframes.at(i).time) {
				index_p = i-1;
				index_n = i;
				factor = ((float)frame - (float)keyframes.at(i-1).time) /((float)keyframes.at(i).time - (float)keyframes.at(i-1).time);
				break;
			}
		}
	}
	
	float result = 0.0;

	size_t indexValue_p = keyframes.at(index_p).index;
	size_t indexValue_n = keyframes.at(index_n).index;
	size_t interpolation_p = keyframes.at(index_p).interpolation;
	size_t interpolation_n = keyframes.at(index_n).interpolation;
	bool isQuadraticBezier_p = ((interpolation_p & 0x4)!=0);
	bool isCubicBezier_p = ((interpolation_p & 0x8) != 0);
	bool isQuadraticBezier_n = ((interpolation_n & 0x4) != 0);
	bool isCubicBezier_n = ((interpolation_n & 0x8) != 0);
	size_t nbValues = values.size();

	isValuesReallyUsed.at(indexValue_p) = true;
	if((isQuadraticBezier_p) ||(isCubicBezier_p))
		isValuesReallyUsed.at(indexValue_p + 1) = true;
	if (isCubicBezier_p)
		isValuesReallyUsed.at(indexValue_p + 2) = true;

	isValuesReallyUsed.at(indexValue_n) = true;
	if ((isQuadraticBezier_n)|| (isCubicBezier_n))
		isValuesReallyUsed.at(indexValue_n + 1) = true;
	if (isCubicBezier_n)
		isValuesReallyUsed.at(indexValue_n + 2) = true;


	if (indexValue_p == indexValue_n)
	{
		if (indexValue_p >= nbValues)								//saddly , so index are not into array range. normally it's good from the discover of interpolation. keep it for security
		{
			if (transform == 2)				//default for scale
			{
				if (transformComponent_tmp == 0)
					ean->x = 1.0;
				else if (transformComponent_tmp == 1)
					ean->y = 1.0;
				else if (transformComponent_tmp == 2)
					ean->z = 1.0;
				else if (transformComponent_tmp == 3)
					ean->w = 1.0;
				//else
					//assert(false);
			}

			return;
		}

		result = values.at(indexValue_p);

	} else {

		if (indexValue_n >= nbValues)
		{
			indexValue_n = indexValue_p;
			factor = 0.0;
		}

		if (indexValue_p >= nbValues)
		{
			if (transform == 2)				//default for scale
			{
				if (transformComponent_tmp == 0)
					ean->x = 1.0;
				else if (transformComponent_tmp == 1)
					ean->y = 1.0;
				else if (transformComponent_tmp == 2)
					ean->z = 1.0;
				else if (transformComponent_tmp == 3)
					ean->w = 1.0;
				//else
					//assert(false);
			}
			
			return;
		}

		double value_p = (double)values.at(indexValue_p);
		double value_n = (double)values.at(indexValue_n);


		result = (float)(value_p + factor * (value_n - value_p));
		
		if (isQuadraticBezier_p)
		{
			double tangent_p = (double)values.at(indexValue_p + 1);

			result = (float)quadraticBezier(factor, value_p, value_p + tangent_p, value_n);
		}

		if (isCubicBezier_p)
		{
			double tangent_p = (double)values.at(indexValue_p + 1);
			double tangent_n = (double)values.at(indexValue_p + 2);

			result = (float)cubicBezier(factor, value_p, value_p + tangent_p, value_n - tangent_n, value_n);
		}
	}


	//now, it still to have find the component for 
	if (transformComponent_tmp == 0)
		ean->x = result;
	else if (transformComponent_tmp == 1)
		ean->y = result;
	else if (transformComponent_tmp == 2)
		ean->z = result;
	else if (transformComponent_tmp == 3)
		ean->w = result;
	//else
		//assert(false);
}


































/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////









EMA::EMA()
{
	this->big_endian = false;
	version = "0.0.0.0";
	type = 3;
	unknow_0 = unknow_1 = unknow_2 = 0;
}

EMA::~EMA()
{
}

bool EMA::LinkAnimation(EmaAnimation &anim, EMO_Bone **not_found)
{
	if (!HasSkeleton())
		return true;

	EmaAnimation temp = anim;

	for (EMAAnimationNode &c : temp)
	{
		EMO_Bone *b = GetBone(c.bone->GetName());

		if (!b)
		{
			if (not_found)
				*not_found = c.bone;

			return false;
		}

		c.bone = b;
	}

	anim = temp;
	return true;
}

uint16_t EMA::AppendAnimation(const EmaAnimation &animation)
{
	if (HasSkeleton())
	{
		for (const EMAAnimationNode &c : animation.nodes)
			if (!BoneExists(c.bone->GetName()))
				return 0xFFFF;
	}

	uint16_t id = animations.size();
	animations.push_back(animation);

	if (HasSkeleton())
	{
		for (EMAAnimationNode &c : animations[id].nodes)
			c.bone = GetBone(c.bone->GetName());
	}

	return id;
}

uint16_t EMA::AppendAnimation(const EMA &other, const std::string &name)
{
	const EmaAnimation *animation = other.GetAnimation(name);
	if (!animation)
		return (uint16_t)-1;

	return AppendAnimation(*animation);
}

void EMA::Copy(const EMA &other)
{
	EMO_Skeleton::Copy(other);

	this->animations = other.animations;
	this->version = other.version;
	this->type = other.type;


	if (HasSkeleton())
	{
		for (EmaAnimation &a : animations)
			for (EMAAnimationNode &c : a.nodes)
				c.bone = GetBone(c.bone->GetName());
	}
}

void EMA::Reset()
{
	EMO_Skeleton::Reset();
	animations.clear();
}

void EMA::RebuildSkeleton(const std::vector<EMO_Bone *> &old_bones_ptr)
{
	if (!HasSkeleton())
		return;

	EMO_Skeleton::RebuildSkeleton(old_bones_ptr);

	for (EmaAnimation &a : animations)
	{
		for (EMAAnimationNode &c : a.nodes)
		{
			uint16_t id = FindBone(old_bones_ptr, c.bone, false);

			if (id == 0xFFFF)
			{
				LibXenoverse::notifyError();
				throw std::runtime_error(std::string(FUNCNAME) + ": bone " + c.bone->GetName() + " is not resolved.\n");
			}

			c.bone = &bones[id];
		}
	}
}



bool EMA::operator==(const EMA &rhs) const
{
	if( (this->animations != rhs.animations) ||
		(this->version != rhs.version) ||
		(this->type != rhs.type) )
		return false;

	if (!HasSkeleton())
	{
		if (rhs.HasSkeleton())
			return false;
	}else if (!rhs.HasSkeleton()){
		return false;
	}

	if (HasSkeleton())
	{
		if (EMO_Skeleton::operator ==(rhs) != true)
			return false;
	}

	return true;
}












/*-------------------------------------------------------------------------------\
|                             buildOrganizedNodes		                         |
\-------------------------------------------------------------------------------*/
void EMA::buildOrganizedNodes()
{
	for (size_t i = 0, nb = animations.size(); i < nb; i++)
		animations.at(i).buildOrganizedNodes();
}
/*-------------------------------------------------------------------------------\
|                             clearOrganizedNodes		                         |
\-------------------------------------------------------------------------------*/
void EMA::clearOrganizedNodes()
{
	for (size_t i = 0, nb = animations.size(); i < nb; i++)
		animations.at(i).clearOrganizedNodes();
}





/*-------------------------------------------------------------------------------\
|                             buildOrganizedNodes						          |
\-------------------------------------------------------------------------------*/
void EmaAnimation::buildOrganizedNodes()				//the goal is to split the nodes for organize by Nodes, byTranform and by component.
{
	organizedNodes.clear();

	struct TestToOrder { bool operator() (const EMAAnimationNode *a, const EMAAnimationNode *b) const { return (a->transformComponent <= b->transformComponent); } };

	
	for (size_t i = 0, nb = nodes.size(); i < nb; i++)
	{
		EMAAnimationNode &node = nodes.at(i);

		bool isfound = false;
		for (size_t j = 0, nb2 = organizedNodes.size(); j < nb2; j++)
		{
			if (node.bone_idx != organizedNodes.at(j).bone_idx)
				continue;


			isfound = true;
			EMAAnimation_ByNode &organizedNode = organizedNodes.at(j);

			bool isfound2 = false;
			for (size_t k = 0, nb3 = organizedNode.transforms.size(); k < nb3; k++)
			{
				if (node.transform != organizedNode.transforms.at(k).transform)
					continue;

				isfound2 = true;
				EMAAnimation_ByNode_ByTransform &transform = organizedNode.transforms.at(k);

				transform.components.push_back(&(nodes.at(i)));
				std::sort(transform.components.begin(), transform.components.end(), TestToOrder());
			}
			if(!isfound2)
			{
				organizedNode.transforms.push_back(EMAAnimation_ByNode_ByTransform(node.transform));
				organizedNode.transforms.back().components.push_back(&(nodes.at(i)));
			}
		}
		if (!isfound)
		{
			organizedNodes.push_back(EMAAnimation_ByNode(node.bone_idx, node.bone));
			organizedNodes.back().transforms.push_back(EMAAnimation_ByNode_ByTransform(node.transform));
			organizedNodes.back().transforms.back().components.push_back(&(nodes.at(i)));
		}
	}
}

/*-------------------------------------------------------------------------------\
|                             getInterpolatedKeyframe						     |
\-------------------------------------------------------------------------------*/
EANKeyframe EMAAnimation_ByNode_ByTransform::getInterpolatedKeyframe(float time, std::vector<float> &values, float default_x, float default_y, float default_z, float default_w)
{
	EANKeyframe kf((int)time, default_x, default_y, default_z, default_w);

	float default_value = 0;
	

	float value = 0;
	for (size_t i = 0, nb = components.size(); i < nb; i++)
	{
		switch (components.at(i)->getTransformComponent())
		{
		case 0: default_value = default_x;  break;
		case 1: default_value = default_y;  break;
		case 2: default_value = default_z;  break;
		case 3: default_value = default_w;  break;
		};

		value = components.at(i)->getInterpolatedKeyframeComponent(time, values, default_value);

		switch (components.at(i)->getTransformComponent())
		{
		case 0: kf.x = value;  break;
		case 1: kf.y = value;  break;
		case 2: kf.z = value;  break;
		case 3: kf.w = value;  break;
		};
	}

	return kf;
}

float EMAAnimationNode::getInterpolatedKeyframeComponent(float time, std::vector<float> &values, float default_value)
{
	float value = default_value;
	
	size_t nbElements = keyframes.size();
	if (nbElements == 0)
		return value;

	//todo add case	this->noInterpolation.


	size_t index_p = (size_t)-1;					//prev
	size_t index_n = (size_t)-1;					//next
	float factor = 0.0;								//between prev and next.
	if (time <= keyframes.at(0).time)
	{
		index_p = 0;
		index_n = 0;
	}
	if (time >= keyframes.back().time)
	{
		index_p = keyframes.size() - 1;
		index_n = index_p;
	}

	if (index_p == (size_t)-1)
	{
		for (size_t i = 1; i < nbElements; i++)						//Command is about a couple Bone+Transform, but EanAnimationNode is only bone, and under, EanKeyframeAnimation is about transform (position, rotation, scale).
		{
			if (time == keyframes.at(i).time)
			{
				index_p = i;
				index_n = i;
				factor = 0.0;
				break;

			}else if (time < keyframes.at(i).time) {
				index_p = i - 1;
				index_n = i;
				factor = (time - (float)keyframes.at(i - 1).time) / ((float)keyframes.at(i).time - (float)keyframes.at(i - 1).time);
				break;
			}
		}
	}

	size_t indexValue_p = keyframes.at(index_p).index;
	size_t indexValue_n = keyframes.at(index_n).index;
	size_t interpolation_p = keyframes.at(index_p).interpolation;
	size_t interpolation_n = keyframes.at(index_n).interpolation;
	bool isQuadraticBezier_p = ((interpolation_p & 0x4) != 0);
	bool isCubicBezier_p = ((interpolation_p & 0x8) != 0);
	bool isQuadraticBezier_n = ((interpolation_n & 0x4) != 0);
	bool isCubicBezier_n = ((interpolation_n & 0x8) != 0);
	size_t nbValues = values.size();

	if ((factor < 0) || (factor > 1))
		int aa = 42;

	if (indexValue_p == indexValue_n)
	{
		if (indexValue_p >= nbValues)								//saddly , so index are not into array range. normally it's good from the discover of interpolation. keep it for security
			return value;

		value = values.at(indexValue_p);

	}else {

		if (indexValue_n >= nbValues)
		{
			indexValue_n = indexValue_p;
			factor = 0.0;
		}

		if (indexValue_p >= nbValues)
			return value;

		double value_p = (double)values.at(indexValue_p);
		double value_n = (double)values.at(indexValue_n);


		value = (float)(value_p + factor * (value_n - value_p));

		if (isQuadraticBezier_p)
		{
			double tangent_p = (double)values.at(indexValue_p + 1);
			value = (float)quadraticBezier(factor, value_p, value_p + tangent_p, value_n);
		}

		if (isCubicBezier_p)
		{
			double tangent_p = (double)values.at(indexValue_p + 1);
			double tangent_n = (double)values.at(indexValue_p + 2);
			value = (float)cubicBezier(factor, value_p, value_p + tangent_p, value_n - tangent_n, value_n);
		}
	}

	return value;
}


/*-------------------------------------------------------------------------------\
|                             sort						                         |
\-------------------------------------------------------------------------------*/
void EMAAnimationNode::sort()
{
	std::sort(keyframes.begin(), keyframes.end(), &EMAAnimationNode::timeOrder);


	/*
	size_t nbKeyFrames = keyframes.size();
	size_t lastTime = (size_t)-1;
	for (size_t i = 0; i < nbKeyFrames; i++)					//remove duplicate frames times. => apparently in ema you have duplicate frame. strange.
	{
		if ((i != 0) && (keyframes.at(i).time == lastTime))
		{
			keyframes.erase(keyframes.begin() + i);
			nbKeyFrames--;
			i--;
			continue;
		}
		lastTime = keyframes.at(i).time;
	}
	*/
}
/*-------------------------------------------------------------------------------\
|                             addKeyFrameAtTime			                         |
\-------------------------------------------------------------------------------*/
void EMAAnimationNode::addKeyFrameAtTime(size_t frame)
{
	size_t nbKf = keyframes.size();
	if (nbKf == 0)										//can't add, there is not values to duplicate.
		return;


	if (frame < keyframes.at(0).time)
	{
		keyframes.insert(keyframes.begin(), keyframes.at(0));
		keyframes.at(0).time = frame;
		return;
	}
	if (frame > keyframes.back().time)
	{
		keyframes.push_back(keyframes.back());
		keyframes.back().time = frame;
		return;
	}


	size_t isFoundForInsert = (size_t)-1;
	for (size_t i = 0; i < nbKf; i++)
	{
		if (keyframes.at(i).time == frame)
		{
			return;
		}
		else if (keyframes.at(i).time > frame) {
			isFoundForInsert = i - 1;
			break;
		}
	}

	if (isFoundForInsert != (size_t)-1)
	{
		keyframes.insert(keyframes.begin() + isFoundForInsert, keyframes.at(isFoundForInsert + 1));
		keyframes.at(isFoundForInsert).time = frame;
	}	//normaly else is not possible.
}

/*-------------------------------------------------------------------------------\
|                             cut						                         |
\-------------------------------------------------------------------------------*/
void EMAAnimationNode::cut(size_t indexKfStart, size_t indexKfEnd, std::vector<float> &values, float default_value, bool pushTo0)
{
	if ((keyframes.size() == 0) || (indexKfEnd < keyframes.at(0).time) || (indexKfStart > keyframes.back().time))		//exclusion
	{
		keyframes.clear();
		return;
	}

	if (indexKfStart < keyframes.at(0).time)				//to avoid add strange values
		indexKfStart = keyframes.at(0).time;
	if (indexKfEnd > keyframes.back().time)
		indexKfEnd = keyframes.back().time;


	//we add keyframe for indexKfStart and indexKfEnd if necessary, if it's the case, it will be interpolated from others
	bool haveAllreadyKf_Start = false;
	bool haveAllreadyKf_End = false;
	size_t nbKf = keyframes.size();
	for (size_t i = 0; i < nbKf; i++)
	{
		if (keyframes.at(i).time == indexKfStart)
		{
			haveAllreadyKf_Start = true;
			if (haveAllreadyKf_Start && haveAllreadyKf_End)
				break;
		}

		if (keyframes.at(i).time == indexKfEnd)
		{
			haveAllreadyKf_End = true;
			if (haveAllreadyKf_Start && haveAllreadyKf_End)
				break;
		}
	}

	if (!haveAllreadyKf_Start)
	{
		float value = getInterpolatedKeyframeComponent((float)indexKfStart, values, default_value);
		size_t index = values.size();
		values.push_back(value);

		for (size_t i = 0; i < nbKf; i++)
		{
			if (keyframes.at(i).time > indexKfStart)
			{
				keyframes.insert(keyframes.begin() + (i - 1), EMAKeyframe(indexKfStart, index));
				break;
			}
		}


		nbKf = keyframes.size();
	}
	if (!haveAllreadyKf_End)
	{
		float value = getInterpolatedKeyframeComponent((float)indexKfStart, values, default_value);
		size_t index = values.size();
		values.push_back(value);


		for (size_t i = 0; i < nbKf; i++)
		{
			if (keyframes.at(i).time > indexKfEnd)
			{
				keyframes.insert(keyframes.begin() + i, EMAKeyframe(indexKfStart, index));
				break;
			}
		}

		nbKf = keyframes.size();
	}



	//So now we can keep only keyframes in range.
	vector<EMAKeyframe> keyframes_old = keyframes;

	keyframes.clear();


	for (size_t i = 0; i < nbKf; i++)
	{
		if ((keyframes_old.at(i).time >= indexKfStart) && (keyframes_old.at(i).time <= indexKfEnd))
		{
			keyframes.push_back(keyframes_old.at(i));
			if (pushTo0)
				keyframes.back().time -= indexKfStart;
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             cleanAnimations			                         |
\-------------------------------------------------------------------------------*/
void EMA::cleanAnimations()
{
	uint16_t nbAnimations = animations.size();
	if (nbAnimations > 65535)
		nbAnimations = 65535;

	for (uint16_t i = 0; i < nbAnimations; i++)
	{
		const EmaAnimation &animation = animations[i];
		int16_t nbAnimationNodes = animation.nodes.size();
		if (nbAnimationNodes > 65535)
			nbAnimationNodes = (int16_t)65535;

		for (uint16_t j = 0; j < nbAnimationNodes; j++)
		{
			animations.at(i).nodes.at(j).cleanAnimationForDuration(animation.duration, animations.at(i).values, animations.at(i).nodes.at(j).getDefaultValue(type));							//that will clean keyframes up to the limit, and also add keyframe for the first time if missed (else there will have strange effect on animation or infinite loop  (case stages .ema))
		}
	}
}

/*-------------------------------------------------------------------------------\
|                             cleanAnimationForDuration	                         |
\-------------------------------------------------------------------------------*/
void EMAAnimationNode::cleanAnimationForDuration(size_t duration, std::vector<float> &values, float default_value)
{
	if (keyframes.size() == 0)
		return;

	sort();			//order keyframes by time

	cut(0, (duration != 0 ? (duration - 1) : 0), values, default_value, false);			//clean keyframes Up to the duration, and also add keyframes is interpolation

	
	addKeyFrameAtTime(0);	//add necessary keyframe

	if(duration != 0)														//last keyframe is needed only on ema. on ean that break perfect rebuild.
		addKeyFrameAtTime(duration - 1);	//add necessary keyframe
}




/*-------------------------------------------------------------------------------\
|                             transformNameForXXXXXAnim						     |
\-------------------------------------------------------------------------------*/
string EMAAnimationNode::transformNameForObjectAnim()
{
	switch (transform)
	{
	case 0: return "Position";
	case 1: return "Rotation";
	case 2: return "Scale";
	}
	return "";
}
string EMAAnimationNode::transformNameForCameraAnim()
{
	switch (transform)
	{
	case 0: return "Position";
	case 1: return "TargetPosition";
	case 2: return "Camera";
	}
	return "";
}
string EMAAnimationNode::transformNameForLightAnim()
{
	switch (transform)
	{
	case 0: return "Position";
	case 2: return "Color";
	case 3: return "DegradeDistance";
	}
	return "";

}
string EMAAnimationNode::transformNameForMaterialAnim()
{
	switch (transform)
	{
	case 0: return "MatCol0";
	case 1: return "MatCol1";
	case 2: return "MatCol2";
	case 3: return "MatCol3";
	case 4: return "TexScrl0";
	case 5: return "TexScrl1";
	case 6: return "TexScrl2";				//logical interpolation from shader entries, but none of Dbvx2 and SSSS use it
	case 7: return "TexScrl3";				//logical interpolation from shader entries, but none of Dbvx2 and SSSS use it
	}
	return "";
}



}

