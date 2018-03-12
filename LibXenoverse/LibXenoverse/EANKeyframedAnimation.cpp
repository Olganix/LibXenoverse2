namespace LibXenoverse
{







/*-------------------------------------------------------------------------------\
|                             EANKeyframedAnimation								 |
\-------------------------------------------------------------------------------*/
EANKeyframedAnimation::EANKeyframedAnimation(EANKeyframedAnimation *source)
{
	this->flag = source->flag;
	this->keyframes = source->keyframes;
}
/*-------------------------------------------------------------------------------\
|                             read												 |
\-------------------------------------------------------------------------------*/
void EANKeyframedAnimation::read(File *file, unsigned char index_size, unsigned char keyframe_size)
{
	unsigned int base_keyframed_animation_address = file->getCurrentAddress();
	unsigned int keyframes_count = 0;
	unsigned int indices_offset = 0;
	unsigned int keyframes_offset = 0;

	file->readInt32E(&flag);
	file->readInt32E(&keyframes_count);
	file->readInt32E(&indices_offset);
	file->readInt32E(&keyframes_offset);

	LOG_DEBUG("[%i] flag : %i, keyframes_count : %i, indices_offset : [%i], keyframes_offset : [%i]\n", base_keyframed_animation_address, flag, keyframes_count, indices_offset, keyframes_offset);


	keyframes.resize(keyframes_count);
	//LOG_DEBUG("Reading Keyframed Animation at %d\n", base_keyframed_animation_address);
	//LOG_DEBUG("  Keyframed Animation Flag: %d\n", flag);

	file->goToAddress(base_keyframed_animation_address + indices_offset);
	for (size_t i = 0; i < keyframes_count; i++)
	{
		LOG_DEBUG("KF %i : [%i] : ", i, file->getCurrentAddress());
		keyframes[i].readFrame(file, index_size);
	}

	//LOG_DEBUG("  Keyframes (%d):\n", keyframes_count);
	file->goToAddress(base_keyframed_animation_address + keyframes_offset);
	for (size_t i = 0; i < keyframes_count; i++)
	{
		LOG_DEBUG("KF %i : [%i] : ", i, file->getCurrentAddress());
		keyframes[i].read(file, keyframe_size);
	}
	//LOG_DEBUG("\n");
}
/*-------------------------------------------------------------------------------\
|                             write												 |
\-------------------------------------------------------------------------------*/
size_t EANKeyframedAnimation::write(File *file, unsigned char index_size, unsigned char keyframe_size)
{
	unsigned int base_keyframed_animation_address = file->getCurrentAddress();
	unsigned int keyframes_count = keyframes.size();
	unsigned int indices_offset = 16;			//this header size
	unsigned int keyframes_offset = (int)ceil((base_keyframed_animation_address + indices_offset + keyframes_count * ((index_size == 0) ? 1 : 2)) / 16.0) * 16 - base_keyframed_animation_address;		//We cast on the next 16octets line.

	file->writeInt32E(&flag);
	file->writeInt32E(&keyframes_count);
	file->writeInt32E(&indices_offset);
	file->writeInt32E(&keyframes_offset);

	LOG_DEBUG("[%i] flag : %i, keyframes_count : %i, indices_offset : [%i], keyframes_offset : [%i]\n", base_keyframed_animation_address, flag, keyframes_count, indices_offset, keyframes_offset);

	file->goToAddress(base_keyframed_animation_address + indices_offset);
	for (size_t i = 0; i < keyframes_count; i++)
	{
		LOG_DEBUG("KF %i : [%i] : ", i, file->getCurrentAddress());
		keyframes[i].writeFrame(file, index_size);
	}

	file->goToAddress(base_keyframed_animation_address + keyframes_offset);
	for (size_t i = 0; i < keyframes_count; i++)
	{
		LOG_DEBUG("KF %i : [%i] : ", i, file->getCurrentAddress());
		keyframes[i].write(file, keyframe_size);
	}

	return keyframes_offset + keyframes_count * 4 * ((keyframe_size == 1) ? 2 : 4);
}
/*-------------------------------------------------------------------------------\
|                             getFrame											 |
\-------------------------------------------------------------------------------*/
void EANKeyframedAnimation::getFrame(EANKeyframe *keyframe, float &x, float &y, float &z, float &w) {
	x = keyframe->x;
	y = keyframe->y;
	z = keyframe->z;
	w = keyframe->w;
}
/*-------------------------------------------------------------------------------\
|                             getInterpolatedFrame								 |
\-------------------------------------------------------------------------------*/
void EANKeyframedAnimation::getInterpolatedFrame(unsigned int frame, float &x, float &y, float &z, float &w) {
	EANKeyframe *lower_keyframe = NULL;
	EANKeyframe *upper_keyframe = NULL;

	unsigned int key_frame;
	for (size_t i = 0; i < keyframes.size(); i++)
	{
		key_frame = keyframes[i].getFrame();
		if ((key_frame <= frame) && (!lower_keyframe || (lower_keyframe->getFrame() <= key_frame)))
			lower_keyframe = &keyframes.at(i);

		if ((key_frame >= frame) && (!upper_keyframe || (upper_keyframe->getFrame() >= key_frame)))
			upper_keyframe = &keyframes.at(i);
	}

	// Interpolate XYZW with the frames
	if ((!lower_keyframe) && (!upper_keyframe))
	{
		x = y = z = 0.0f;
		w = 1.0f;
	}else if (lower_keyframe == upper_keyframe) {
		getFrame(lower_keyframe, x, y, z, w);

	}else if (lower_keyframe && upper_keyframe){

		float lower_frame = (float) lower_keyframe->getFrame();
		float upper_frame = (float) upper_keyframe->getFrame();
		float factor = ((float)frame - lower_frame) / (upper_frame - lower_frame);

		x = lerp(lower_keyframe->x, upper_keyframe->x, factor);
		y = lerp(lower_keyframe->y, upper_keyframe->y, factor);
		z = lerp(lower_keyframe->z, upper_keyframe->z, factor);
		w = lerp(lower_keyframe->w, upper_keyframe->w, factor);

	}else if (lower_keyframe) {
		getFrame(lower_keyframe, x, y, z, w);

	}else if (upper_keyframe) {
		getFrame(upper_keyframe, x, y, z, w);
	}
}
/*-------------------------------------------------------------------------------\
|                             operator=											 |
\-------------------------------------------------------------------------------*/
void EANKeyframedAnimation::operator=(EANKeyframedAnimation &source)
{
	this->flag = source.flag;
	this->keyframes = source.keyframes;
}
/*-------------------------------------------------------------------------------\
|                             append					                         |
\-------------------------------------------------------------------------------*/
void EANKeyframedAnimation::append(EANKeyframedAnimation &source, size_t delayStartFrame)
{
	size_t nbKf_scr = source.keyframes.size();
	for (size_t i = 0; i < nbKf_scr; i++)
	{
		this->keyframes.push_back(source.keyframes.at(i));
		this->keyframes.back().frame += delayStartFrame;
	}
}
/*-------------------------------------------------------------------------------\
|                             delayTimeFrame			                         |
\-------------------------------------------------------------------------------*/
void EANKeyframedAnimation::delayTimeFrame(size_t delayStartFrame)
{
	size_t nbKf = keyframes.size();
	for (size_t i = 0; i < nbKf; i++)
		this->keyframes.at(i).frame += delayStartFrame;
}
/*-------------------------------------------------------------------------------\
|                             cut						                         |
\-------------------------------------------------------------------------------*/
void EANKeyframedAnimation::cut(size_t indexKfStart, size_t indexKfEnd, bool pushTo0)
{
	if ((keyframes.size() == 0) || (indexKfEnd < keyframes.at(0).frame) || (indexKfStart > keyframes.back().frame))		//exclusion
	{
		keyframes.clear();
		return;
	}

	if (indexKfStart < keyframes.at(0).frame)				//to avoid add strange values
		indexKfStart = keyframes.at(0).frame;
	if (indexKfEnd > keyframes.back().frame)
		indexKfEnd = keyframes.back().frame;


	//we add keyframe for indexKfStart and indexKfEnd if necessary, if it's the case, it will be interpolated from others
	bool haveAllreadyKf_Start = false;
	bool haveAllreadyKf_End = false;
	size_t nbKf = keyframes.size();
	for (size_t i = 0; i < nbKf; i++)
	{
		if (keyframes.at(i).frame == indexKfStart)
		{
			haveAllreadyKf_Start = true;
			if (haveAllreadyKf_Start && haveAllreadyKf_End)
				break;
		}

		if (keyframes.at(i).frame == indexKfEnd)
		{
			haveAllreadyKf_End = true;
			if (haveAllreadyKf_Start && haveAllreadyKf_End)
				break;
		}
	}
	
	if (!haveAllreadyKf_Start)
	{
		float x, y, z, w;
		getInterpolatedFrame(indexKfStart, x, y, z, w);
		
		for (size_t i = 0; i < nbKf; i++)
		{
			if (keyframes.at(i).frame > indexKfStart)
			{
				keyframes.insert(keyframes.begin() + (i - 1), EANKeyframe(indexKfStart, x, y, z, w));
				break;
			}
		}
		

		nbKf = keyframes.size();
	}
	if (!haveAllreadyKf_End)
	{
		float x, y, z, w;
		getInterpolatedFrame(indexKfEnd, x, y, z, w);
		

		for (size_t i = 0; i < nbKf; i++)
		{
			if (keyframes.at(i).frame > indexKfEnd)
			{
				keyframes.insert(keyframes.begin() + (i - 1), EANKeyframe(indexKfEnd, x, y, z, w));
				break;
			}
		}

		nbKf = keyframes.size();
	}

	
	
	//So now we can keep only keyframes in range.
	vector<EANKeyframe> keyframes_old = keyframes;

	keyframes.clear();


	for (size_t i = 0; i < nbKf; i++)
	{
		if ((keyframes_old.at(i).frame >= indexKfStart) && (keyframes_old.at(i).frame <= indexKfEnd))
		{
			keyframes.push_back(keyframes_old.at(i));
			if(pushTo0)
				keyframes.back().frame -= indexKfStart;
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             sort						                         |
\-------------------------------------------------------------------------------*/
void EANKeyframedAnimation::sort()
{
	std::sort(keyframes.begin(), keyframes.end(), &EANKeyframedAnimation::timeOrder);
}
/*-------------------------------------------------------------------------------\
|                             addKeyFrameAtTime			                         |
\-------------------------------------------------------------------------------*/
void EANKeyframedAnimation::addKeyFrameAtTime(size_t frame)
{
	size_t nbKf = keyframes.size();
	if (nbKf == 0)										//can't add, there is not values to duplicate.
		return;

	
	if (frame < keyframes.at(0).frame)
	{
		keyframes.insert(keyframes.begin(), keyframes.at(0));
		keyframes.at(0).frame = frame;
		return;
	}
	if (frame > keyframes.back().frame)
	{
		keyframes.push_back(keyframes.back());
		keyframes.back().frame = frame;
		return;
	}


	size_t isFoundForInsert = (size_t)-1;
	for (size_t i = 0; i < nbKf; i++)
	{
		if (keyframes.at(i).frame == frame)
		{
			return;
		}else if (keyframes.at(i).frame > frame) {
			isFoundForInsert = i - 1;
			break;
		}
	}

	if (isFoundForInsert != (size_t)-1)
	{
		keyframes.insert(keyframes.begin() + isFoundForInsert, keyframes.at(isFoundForInsert + 1));
		keyframes.at(isFoundForInsert).frame = frame;
	}	//normaly else is not possible.
}



#ifdef LIBXENOVERSE_FBX_SUPPORT

/*-------------------------------------------------------------------------------\
|                             importFBXPositionAnimCurve						 |
\-------------------------------------------------------------------------------*/
bool EANKeyframedAnimation::importFBXPositionAnimCurve(FbxNode *fbx_node, size_t nbframes, FbxAnimCurve* fbx_animCurve_translation_x, FbxAnimCurve* fbx_animCurve_translation_y, FbxAnimCurve* fbx_animCurve_translation_z)
{
	this->flag = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_POSITION;
	float fps = 60.0f;			//TODO configure.
	float intervale_ms = 1000.0f / fps;

	//we have to fill gaps. so first thing is to search the real number of frames with merge of this animations (note : normaly, with exporter/importer behaviour, all animations must be with the same frequenty, but we need to test if is not)
	size_t nbFrames_X = (fbx_animCurve_translation_x) ? fbx_animCurve_translation_x->KeyGetCount() : 0;
	size_t nbFrames_Y = (fbx_animCurve_translation_y) ? fbx_animCurve_translation_y->KeyGetCount() : 0;
	size_t nbFrames_Z = (fbx_animCurve_translation_z) ? fbx_animCurve_translation_z->KeyGetCount() : 0;
		
	FbxLongLong time_X_tmp = (FbxLongLong)((fbx_animCurve_translation_x) ? fbx_animCurve_translation_x->KeyGetTime(fbx_animCurve_translation_x->KeyGetCount() - 1).GetMilliSeconds() : 0.0);
	FbxLongLong time_Y_tmp = (FbxLongLong)((fbx_animCurve_translation_y) ? fbx_animCurve_translation_y->KeyGetTime(fbx_animCurve_translation_y->KeyGetCount() - 1).GetMilliSeconds() : 0.0);
	FbxLongLong time_Z_tmp = (FbxLongLong)((fbx_animCurve_translation_z) ? fbx_animCurve_translation_z->KeyGetTime(fbx_animCurve_translation_z->KeyGetCount() - 1).GetMilliSeconds() : 0.0);

	if (fbx_animCurve_translation_x)
	{
		FbxTime time_tmp = fbx_animCurve_translation_x->KeyGetTime(fbx_animCurve_translation_x->KeyGetCount() - 1);
		FbxTime::EMode emod = time_tmp.GetGlobalTimeMode();
		double framerate = time_tmp.GetFrameRate(emod);
	}


	std::vector<FbxTime> list_fbxtime;
	FbxTime fbxtime;
	size_t nbMerged = 0;


	//get all Key_time merged
	this->mergeListTime(fbx_animCurve_translation_x, list_fbxtime);
	this->mergeListTime(fbx_animCurve_translation_y, list_fbxtime);
	this->mergeListTime(fbx_animCurve_translation_z, list_fbxtime);
	
	//now, we could add Keys
	float px = 0, py = 0, pz = 0, pw = 1.0f;
	float px_last = 0, py_last = 0, pz_last = 0, pw_last = 0;
	float px_next = 0, py_next = 0, pz_next = 0, pw_next = 0;
		
	size_t inc_x = 0;
	size_t inc_y = 0;
	size_t inc_z = 0;
	size_t inc_x_next, inc_y_next, inc_z_next;
	FbxTime fbxtime_next;
	size_t numFrame = 0;

	bool haveNoNeutralPosition = false;

	nbMerged = list_fbxtime.size();
	for (size_t i = 0; i < nbMerged; i++)
	{
		fbxtime = list_fbxtime.at(i);
		fbxtime_next = (i + 1< nbMerged) ? list_fbxtime.at(i+1) : list_fbxtime.at(i);

		numFrame = (size_t)(round(fbxtime.GetMilliSeconds() / intervale_ms));
			
		px = getValueForTime(fbx_animCurve_translation_x, fbxtime, inc_x, nbFrames_X, px_last);
		py = getValueForTime(fbx_animCurve_translation_y, fbxtime, inc_y, nbFrames_Y, py_last);
		pz = getValueForTime(fbx_animCurve_translation_z, fbxtime, inc_z, nbFrames_Z, pz_last);
		

		// test to not have too much keyframe, we will remove duplicate ones
		inc_x_next = inc_x;
		inc_y_next = inc_y;
		inc_z_next = inc_z;
		px_next = getValueForTime(fbx_animCurve_translation_x, fbxtime_next, inc_x_next, nbFrames_X, px);
		py_next = getValueForTime(fbx_animCurve_translation_y, fbxtime_next, inc_x_next, nbFrames_Y, py);
		pz_next = getValueForTime(fbx_animCurve_translation_z, fbxtime_next, inc_x_next, nbFrames_Z, pz);

		if ((fbxtime != fbxtime_next) &&				//if is not the last keyframe
			(px == px_last) && (px == px_next) &&		//if the intermediaire value is not unique
			(py == py_last) && (py == py_next) &&
			(pz == pz_last) && (pz == pz_next)
			)
		{
			continue;									//we could remove this.
		}


		px_last = px;
		py_last = py;
		pz_last = pz;
		
		if ((!haveNoNeutralPosition) && ((abs(px) > 0.00001) || (abs(py) > 0.00001) || (abs(pz) > 0.00001)) )
			haveNoNeutralPosition = true;

		EANKeyframe keyframe(numFrame, px, py, pz, pw);
		keyframes.push_back(keyframe);
	}

	return haveNoNeutralPosition;
}
/*-------------------------------------------------------------------------------\
|                             importFBXRotationAnimCurve						 |
\-------------------------------------------------------------------------------*/
bool EANKeyframedAnimation::importFBXRotationAnimCurve(FbxNode *fbx_node, size_t nbframes, FbxAnimCurve* fbx_animCurve_rotation_x, FbxAnimCurve* fbx_animCurve_rotation_y, FbxAnimCurve* fbx_animCurve_rotation_z)
{
	this->flag = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION;
		
	float fps = 60.0f;
	float intervale_ms = 1000.0f / fps;

	//we have to fill gaps. so first thing is to search the real number of frames with merge of this animations (note : normaly, with exporter/importer behaviour, all animations must be with the same frequenty, but we need to test if is not)
	size_t nbFrames_X = (fbx_animCurve_rotation_x) ? fbx_animCurve_rotation_x->KeyGetCount() : 0;
	size_t nbFrames_Y = (fbx_animCurve_rotation_y) ? fbx_animCurve_rotation_y->KeyGetCount() : 0;
	size_t nbFrames_Z = (fbx_animCurve_rotation_z) ? fbx_animCurve_rotation_z->KeyGetCount() : 0;

	std::vector<FbxTime> list_fbxtime;
	FbxTime fbxtime;
	size_t nbMerged = 0;


	//get all Key_time merged
	this->mergeListTime(fbx_animCurve_rotation_x, list_fbxtime);
	this->mergeListTime(fbx_animCurve_rotation_y, list_fbxtime);
	this->mergeListTime(fbx_animCurve_rotation_z, list_fbxtime);

	//now, we could add Keys
	float rx = 0, ry = 0, rz = 0, rw = 1.0f;
	float rx_last = 0, ry_last = 0, rz_last = 0;
	float rx_next = 0, ry_next = 0, rz_next = 0;

	size_t inc_x = 0;
	size_t inc_y = 0;
	size_t inc_z = 0;
	size_t inc_x_next, inc_y_next, inc_z_next;
	FbxTime fbxtime_next;
	size_t numFrame = 0;

	
	bool haveNoNeutralPosition = false;

	nbMerged = list_fbxtime.size();
	for (size_t i = 0; i < nbMerged; i++)
	{
		fbxtime = list_fbxtime.at(i);
		fbxtime_next = (i + 1< nbMerged) ? list_fbxtime.at(i + 1) : list_fbxtime.at(i);
		numFrame = (size_t)(round(fbxtime.GetMilliSeconds() / intervale_ms));

		rx = getValueForTime(fbx_animCurve_rotation_x, fbxtime, inc_x, nbFrames_X, rx_last);
		ry = getValueForTime(fbx_animCurve_rotation_y, fbxtime, inc_y, nbFrames_Y, ry_last);
		rz = getValueForTime(fbx_animCurve_rotation_z, fbxtime, inc_z, nbFrames_Z, rz_last);
		


		// test to not have too much keyframe, we will remove duplicate ones
		inc_x_next = inc_x;
		inc_y_next = inc_y;
		inc_z_next = inc_z;
		rx_next = getValueForTime(fbx_animCurve_rotation_x, fbxtime_next, inc_x_next, nbFrames_X, rx);
		ry_next = getValueForTime(fbx_animCurve_rotation_y, fbxtime_next, inc_x_next, nbFrames_Y, ry);
		rz_next = getValueForTime(fbx_animCurve_rotation_z, fbxtime_next, inc_x_next, nbFrames_Z, rz);

		if ((fbxtime != fbxtime_next) &&				//if is not the last keyframe
			(rx == rx_last) && (rx == rx_next) &&		//if the intermediaire value is not unique
			(ry == ry_last) && (ry == ry_next) &&
			(rz == rz_last) && (rz == rz_next)
			)
		{
			continue;									//we could remove this. Todo remettre
		}


		
		


		rx_last = rx;
		ry_last = ry;
		rz_last = rz;



		//we check the convention on Eulers rotations, it's about orders.
		FbxEuler::EOrder order;
		fbx_node->GetRotationOrder(FbxNode::EPivotSet::eSourcePivot, order);

		FbxQuaternion rotation;

		switch (order)
		{
		case FbxEuler::EOrder::eOrderXZY:								//the version of XenoVerse
		{
			//yaw sur Y //pitch sur Z //roll sur X
			rotation = FbxQuaternion(FbxVector4(0, 1, 0, 1), ry) * FbxQuaternion(FbxVector4(0, 0, 1, 1), rz) * FbxQuaternion(FbxVector4(1, 0, 0, 1), rx);	//lacet sur Y //tangage sur Z //roulis sur X
		}
		break;

		case FbxEuler::EOrder::eOrderXYZ: { rotation = FbxQuaternion(FbxVector4(0, 0, 1, 1), rz) * FbxQuaternion(FbxVector4(0, 1, 0, 1), ry) * FbxQuaternion(FbxVector4(1, 0, 0, 1), rx); } break;
		case FbxEuler::EOrder::eOrderYZX: { rotation = FbxQuaternion(FbxVector4(1, 0, 0, 1), rx) * FbxQuaternion(FbxVector4(0, 0, 1, 1), rz) * FbxQuaternion(FbxVector4(0, 1, 0, 1), ry); } break;
		case FbxEuler::EOrder::eOrderYXZ: { rotation = FbxQuaternion(FbxVector4(0, 0, 1, 1), rz) * FbxQuaternion(FbxVector4(1, 0, 0, 1), rx) * FbxQuaternion(FbxVector4(0, 1, 0, 1), ry); } break;
		case FbxEuler::EOrder::eOrderZXY: { rotation = FbxQuaternion(FbxVector4(0, 1, 0, 1), ry) * FbxQuaternion(FbxVector4(1, 0, 0, 1), rx) * FbxQuaternion(FbxVector4(0, 0, 1, 1), rz); } break;
		case FbxEuler::EOrder::eOrderZYX: { rotation = FbxQuaternion(FbxVector4(1, 0, 0, 1), rx) * FbxQuaternion(FbxVector4(0, 1, 0, 1), ry) * FbxQuaternion(FbxVector4(0, 0, 1, 1), rz); } break;

		case FbxEuler::EOrder::eOrderSphericXYZ:
		{
			//todo put a warning :  we will use eOrderXYZ instead of the spherique version, because I don't find information on that case.
			rotation = FbxQuaternion(FbxVector4(0, 0, 1, 1), rz) * FbxQuaternion(FbxVector4(0, 1, 0, 1), ry) * FbxQuaternion(FbxVector4(1, 0, 0, 1), rx);
		}
		break;
		}


		
		rx = (float)(rotation[0]);
		ry = (float)(rotation[1]);
		rz = (float)(rotation[2]);
		rw = (float)(rotation[3]);
			

		if ((!haveNoNeutralPosition) && ((abs(rx) > 0.00001) || (abs(ry) > 0.00001) || (abs(rz) > 0.00001) || (abs(rw-1.0) > 0.00001)))
			haveNoNeutralPosition = true;

		EANKeyframe keyframe(numFrame, rx, ry, rz, rw);
		keyframes.push_back(keyframe);
	}

	return haveNoNeutralPosition;
}
/*-------------------------------------------------------------------------------\
|                             importFBXScalingAnimCurve							 |
\-------------------------------------------------------------------------------*/
bool EANKeyframedAnimation::importFBXScalingAnimCurve(FbxNode *fbx_node, size_t nbframes, FbxAnimCurve* fbx_animCurve_scale_x, FbxAnimCurve* fbx_animCurve_scale_y, FbxAnimCurve* fbx_animCurve_scale_z)
{
	this->flag = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_SCALE;
		

	float fps = 60.0f;
	float intervale_ms = 1000.0f / fps;

	//we have to fill gaps. so first thing is to search the real number of frames with merge of this animations (note : normaly, with exporter/importer behaviour, all animations must be with the same frequenty, but we need to test if is not)
	size_t nbFrames_X = (fbx_animCurve_scale_x) ? fbx_animCurve_scale_x->KeyGetCount() : 0;
	size_t nbFrames_Y = (fbx_animCurve_scale_y) ? fbx_animCurve_scale_y->KeyGetCount() : 0;
	size_t nbFrames_Z = (fbx_animCurve_scale_z) ? fbx_animCurve_scale_z->KeyGetCount() : 0;

	std::vector<FbxTime> list_fbxtime;
	FbxTime fbxtime;
	size_t nbMerged = 0;


	//get all Key_time merged
	this->mergeListTime(fbx_animCurve_scale_x, list_fbxtime);
	this->mergeListTime(fbx_animCurve_scale_y, list_fbxtime);
	this->mergeListTime(fbx_animCurve_scale_z, list_fbxtime);

	//now, we could add Keys
	float sx = 1.0f, sy = 1.0f, sz = 1.0f, sw = 1.0f;
	float sx_last = 1.0, sy_last = 1.0, sz_last = 1.0;
	float sx_next = 1.0, sy_next = 1.0, sz_next = 1.0;

	size_t inc_x = 0;
	size_t inc_y = 0;
	size_t inc_z = 0;
	size_t inc_x_next, inc_y_next, inc_z_next;
	FbxTime fbxtime_next;
	size_t numFrame = 0;

	bool haveNoNeutralPosition = false;

	nbMerged = list_fbxtime.size();
	for (size_t i = 0; i < nbMerged; i++)
	{
		fbxtime = list_fbxtime.at(i);
		fbxtime_next = (i + 1< nbMerged) ? list_fbxtime.at(i + 1) : list_fbxtime.at(i);
		numFrame = (size_t)(round(fbxtime.GetMilliSeconds() / intervale_ms));

		sx = getValueForTime(fbx_animCurve_scale_x, fbxtime, inc_x, nbFrames_X, sx_last);
		sy = getValueForTime(fbx_animCurve_scale_y, fbxtime, inc_y, nbFrames_Y, sy_last);
		sz = getValueForTime(fbx_animCurve_scale_z, fbxtime, inc_z, nbFrames_Z, sz_last);
		

		// test to not have too much keyframe, we will remove duplicate ones
		inc_x_next = inc_x;
		inc_y_next = inc_y;
		inc_z_next = inc_z;
		sx_next = getValueForTime(fbx_animCurve_scale_x, fbxtime_next, inc_x_next, nbFrames_X, sx);
		sy_next = getValueForTime(fbx_animCurve_scale_y, fbxtime_next, inc_x_next, nbFrames_Y, sy);
		sz_next = getValueForTime(fbx_animCurve_scale_z, fbxtime_next, inc_x_next, nbFrames_Z, sz);

		if ((fbxtime != fbxtime_next) &&				//if is not the last keyframe
			(sx == sx_last) && (sx == sx_next) &&		//if the intermediaire value is not unique
			(sy == sy_last) && (sy == sy_next) &&
			(sz == sz_last) && (sz == sz_next)
			)
		{
			continue;									//we could remove this.
		}


		sx_last = sx;
		sy_last = sy;
		sz_last = sz;

		if ((!haveNoNeutralPosition) && ((abs(sx-1.0) > 0.00001) || (abs(sy - 1.0) > 0.00001) || (abs(sz - 1.0) > 0.00001)))
			haveNoNeutralPosition = true;

		EANKeyframe keyframe(numFrame, sx, sy, sz, sw);
		keyframes.push_back(keyframe);
	}

	return haveNoNeutralPosition;
}
/*-------------------------------------------------------------------------------\
|                             importFBXTargetCameraPositionAnimCurve			 |
\-------------------------------------------------------------------------------*/
bool EANKeyframedAnimation::importFBXTargetCameraPositionAnimCurve(FbxNode *fbx_node, size_t nbframes, FbxAnimCurve* fbx_animCurve_translation_x, FbxAnimCurve* fbx_animCurve_translation_y, FbxAnimCurve* fbx_animCurve_translation_z)
{
	this->flag = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_ROTATION;
	float fps = 60.0f;			//TODO configure.
	float intervale_ms = 1000.0f / fps;

	//we have to fill gaps. so first thing is to search the real number of frames with merge of this animations (note : normaly, with exporter/importer behaviour, all animations must be with the same frequenty, but we need to test if is not)
	size_t nbFrames_X = (fbx_animCurve_translation_x) ? fbx_animCurve_translation_x->KeyGetCount() : 0;
	size_t nbFrames_Y = (fbx_animCurve_translation_y) ? fbx_animCurve_translation_y->KeyGetCount() : 0;
	size_t nbFrames_Z = (fbx_animCurve_translation_z) ? fbx_animCurve_translation_z->KeyGetCount() : 0;

	FbxLongLong time_X_tmp = (FbxLongLong)((fbx_animCurve_translation_x) ? fbx_animCurve_translation_x->KeyGetTime(fbx_animCurve_translation_x->KeyGetCount() - 1).GetMilliSeconds() : 0.0);
	FbxLongLong time_Y_tmp = (FbxLongLong)((fbx_animCurve_translation_y) ? fbx_animCurve_translation_y->KeyGetTime(fbx_animCurve_translation_y->KeyGetCount() - 1).GetMilliSeconds() : 0.0);
	FbxLongLong time_Z_tmp = (FbxLongLong)((fbx_animCurve_translation_z) ? fbx_animCurve_translation_z->KeyGetTime(fbx_animCurve_translation_z->KeyGetCount() - 1).GetMilliSeconds() : 0.0);

	if (fbx_animCurve_translation_x)
	{
		FbxTime time_tmp = fbx_animCurve_translation_x->KeyGetTime(fbx_animCurve_translation_x->KeyGetCount() - 1);
		FbxTime::EMode emod = time_tmp.GetGlobalTimeMode();
		double framerate = time_tmp.GetFrameRate(emod);
	}


	std::vector<FbxTime> list_fbxtime;
	FbxTime fbxtime;
	size_t nbMerged = 0;


	//get all Key_time merged
	this->mergeListTime(fbx_animCurve_translation_x, list_fbxtime);
	this->mergeListTime(fbx_animCurve_translation_y, list_fbxtime);
	this->mergeListTime(fbx_animCurve_translation_z, list_fbxtime);

	//now, we could add Keys
	float px = 0, py = 0, pz = 0, pw = 1.0f;
	float px_last = 0, py_last = 0, pz_last = 0, pw_last = 0;
	float px_next = 0, py_next = 0, pz_next = 0, pw_next = 0;

	size_t inc_x = 0;
	size_t inc_y = 0;
	size_t inc_z = 0;
	size_t inc_x_next, inc_y_next, inc_z_next;
	FbxTime fbxtime_next;
	size_t numFrame = 0;

	bool haveNoNeutralPosition = false;

	nbMerged = list_fbxtime.size();
	for (size_t i = 0; i < nbMerged; i++)
	{
		fbxtime = list_fbxtime.at(i);
		fbxtime_next = (i + 1< nbMerged) ? list_fbxtime.at(i + 1) : list_fbxtime.at(i);

		numFrame = (size_t)(round(fbxtime.GetMilliSeconds() / intervale_ms));

		px = getValueForTime(fbx_animCurve_translation_x, fbxtime, inc_x, nbFrames_X, px_last);
		py = getValueForTime(fbx_animCurve_translation_y, fbxtime, inc_y, nbFrames_Y, py_last);
		pz = getValueForTime(fbx_animCurve_translation_z, fbxtime, inc_z, nbFrames_Z, pz_last);


		// test to not have too much keyframe, we will remove duplicate ones
		inc_x_next = inc_x;
		inc_y_next = inc_y;
		inc_z_next = inc_z;
		px_next = getValueForTime(fbx_animCurve_translation_x, fbxtime_next, inc_x_next, nbFrames_X, px);
		py_next = getValueForTime(fbx_animCurve_translation_y, fbxtime_next, inc_x_next, nbFrames_Y, py);
		pz_next = getValueForTime(fbx_animCurve_translation_z, fbxtime_next, inc_x_next, nbFrames_Z, pz);

		if ((fbxtime != fbxtime_next) &&				//if is not the last keyframe
			(px == px_last) && (px == px_next) &&		//if the intermediaire value is not unique
			(py == py_last) && (py == py_next) &&
			(pz == pz_last) && (pz == pz_next)
			)
		{
			continue;									//we could remove this.
		}


		px_last = px;
		py_last = py;
		pz_last = pz;

		if ((!haveNoNeutralPosition) && ((abs(px) > 0.00001) || (abs(py) > 0.00001) || (abs(pz) > 0.00001)))
			haveNoNeutralPosition = true;

		EANKeyframe keyframe(numFrame, px, py, pz, pw);
		keyframes.push_back(keyframe);
	}

	return haveNoNeutralPosition;
}
/*-------------------------------------------------------------------------------\
|                             importFBXCameraAnimCurve							 |
\-------------------------------------------------------------------------------*/
bool EANKeyframedAnimation::importFBXCameraAnimCurve(FbxNode *fbx_node, size_t nbframes, FbxAnimCurve* fbx_animCurve_roll, FbxAnimCurve* fbx_animCurve_focale)
{
	this->flag = LIBXENOVERSE_EAN_KEYFRAMED_ANIMATION_FLAG_CAMERA;

	float fps = 60.0f;
	float intervale_ms = 1000.0f / fps;

	//we have to fill gaps. so first thing is to search the real number of frames with merge of this animations (note : normaly, with exporter/importer behaviour, all animations must be with the same frequenty, but we need to test if is not)
	size_t nbFrames_X = (fbx_animCurve_roll) ? fbx_animCurve_roll->KeyGetCount() : 0;
	size_t nbFrames_Y = (fbx_animCurve_focale) ? fbx_animCurve_focale->KeyGetCount() : 0;

	std::vector<FbxTime> list_fbxtime;
	FbxTime fbxtime;
	size_t nbMerged = 0;


	//get all Key_time merged
	this->mergeListTime(fbx_animCurve_roll, list_fbxtime);
	this->mergeListTime(fbx_animCurve_focale, list_fbxtime);

	//now, we could add Keys
	float rx = 0, ry = 0, rz = 0, rw = 0.0f;
	float rx_last = 0, ry_last = 0;
	float rx_next = 0, ry_next = 0;

	size_t inc_x = 0;
	size_t inc_y = 0;
	size_t inc_x_next, inc_y_next;
	FbxTime fbxtime_next;
	size_t numFrame = 0;


	bool haveNoNeutralPosition = false;

	nbMerged = list_fbxtime.size();
	for (size_t i = 0; i < nbMerged; i++)
	{
		fbxtime = list_fbxtime.at(i);
		fbxtime_next = (i + 1< nbMerged) ? list_fbxtime.at(i + 1) : list_fbxtime.at(i);
		numFrame = (size_t)(round(fbxtime.GetMilliSeconds() / intervale_ms));

		rx = getValueForTime(fbx_animCurve_roll, fbxtime, inc_x, nbFrames_X, rx_last);
		ry = getValueForTime(fbx_animCurve_focale, fbxtime, inc_y, nbFrames_Y, ry_last);



		// test to not have too much keyframe, we will remove duplicate ones
		inc_x_next = inc_x;
		inc_y_next = inc_y;
		rx_next = getValueForTime(fbx_animCurve_roll, fbxtime_next, inc_x_next, nbFrames_X, rx);
		ry_next = getValueForTime(fbx_animCurve_focale, fbxtime_next, inc_x_next, nbFrames_Y, ry);

		if ((fbxtime != fbxtime_next) &&				//if is not the last keyframe
			(rx == rx_last) && (rx == rx_next) &&		//if the intermediaire value is not unique
			(ry == ry_last) && (ry == ry_next)
			)
		{
			continue;									//we could remove this. Todo remettre
		}

		rx_last = rx;
		ry_last = ry;

		if ((!haveNoNeutralPosition) && ((abs(rx) > 0.00001) || (abs(ry) > 0.00001) || (abs(rz) > 0.00001) || (abs(rw - 1.0) > 0.00001)))
			haveNoNeutralPosition = true;

		EANKeyframe keyframe(numFrame, rx * 3.14159265358979f / 180.0f, ry * 3.14159265358979f / 180.0f, rz, rw);
		keyframes.push_back(keyframe);
	}

	return haveNoNeutralPosition;
}
/*-------------------------------------------------------------------------------\
|                             mergeListTime										 |
\-------------------------------------------------------------------------------*/
void EANKeyframedAnimation::mergeListTime(FbxAnimCurve* fbx_animCurve, std::vector<FbxTime> &list_fbxtime)
{
	size_t nbFrames = (fbx_animCurve) ? fbx_animCurve->KeyGetCount() : 0;
	FbxTime fbxtime;
	float fps = 60.0f;
	float intervale_ms = 1000.0f / fps;

	size_t nbMerged = 0;
	for (size_t i = 0; i < nbFrames; i++)
	{
		fbxtime = fbx_animCurve->KeyGetTime(i);
		size_t numFrame = (size_t)(round(fbxtime.GetMilliSeconds() / intervale_ms));

		nbMerged = list_fbxtime.size();
		if ((i < nbMerged) && (round(list_fbxtime.at(i).GetMilliSeconds() / intervale_ms) == numFrame))		//to speedup in good case, if they is the same Keys between curves
			continue;												//we could pass to another key.

		if ((list_fbxtime.size() == 0) || (numFrame > round(list_fbxtime.back().GetMilliSeconds() / intervale_ms)))
		{
			list_fbxtime.push_back(fbxtime);
			continue;
		}

		//else, we must inset new key
		bool haveToAdd = true;
		for (size_t j = 0; j < nbMerged; j++)
		{
			size_t curr_numFrame = (size_t)(round(list_fbxtime.at(j).GetMilliSeconds() / intervale_ms));

			if (curr_numFrame == numFrame)
			{
				haveToAdd = false;
				break;
			}

			if (numFrame <= curr_numFrame)
			{
				list_fbxtime.insert(list_fbxtime.begin() + j, fbxtime);
				haveToAdd = false;
				break;
			}
		}
		if (haveToAdd)				//case fbxtime > of the last
			list_fbxtime.push_back(fbxtime);
	}
}
/*-------------------------------------------------------------------------------\
|                             getValueForTime									 |
\-------------------------------------------------------------------------------*/
float EANKeyframedAnimation::getValueForTime(FbxAnimCurve* fbx_animCurve, FbxTime fbxtime, size_t &currentIndex, size_t nbFrames, float defaultValue)
{
	float value = defaultValue;
	if (currentIndex < nbFrames)
	{
		FbxTime fbxtime_next = fbx_animCurve->KeyGetTime(currentIndex);

		if (fbxtime_next == fbxtime)
		{
			value = fbx_animCurve->KeyGetValue(currentIndex);
			currentIndex++;

		}else{		//gap

			if (currentIndex > 1)		//have previous , we could interpolate (linear)
			{
				FbxTime fbxtime_prev = fbx_animCurve->KeyGetTime(currentIndex - 1);
				float value_prev = fbx_animCurve->KeyGetValue(currentIndex - 1);

				value = (fbx_animCurve->KeyGetValue(currentIndex) - value_prev) * ((fbxtime.GetMilliSeconds() - fbxtime_prev.GetMilliSeconds()) / (fbxtime_next.GetMilliSeconds() - fbxtime_prev.GetMilliSeconds())) + value_prev;

			}else{				//no previous, will use next value
				value = fbx_animCurve->KeyGetValue(currentIndex);
			}
		}
	}

	return value;
}

#endif
}