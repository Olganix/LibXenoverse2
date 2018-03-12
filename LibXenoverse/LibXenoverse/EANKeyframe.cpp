namespace LibXenoverse
{






/*-------------------------------------------------------------------------------\
|                             EANKeyframe										 |
\-------------------------------------------------------------------------------*/
EANKeyframe::EANKeyframe(unsigned int frame, float x, float y, float z, float w)
{
	this->frame = frame;
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
/*-------------------------------------------------------------------------------\
|                             EANKeyframe										 |
\-------------------------------------------------------------------------------*/
EANKeyframe::EANKeyframe(EANKeyframe *source)
{
	this->frame = source->frame;
	this->x = source->x;
	this->y = source->y;
	this->z = source->z;
	this->w = source->w;
}
/*-------------------------------------------------------------------------------\
|                             EANKeyframe										 |
\-------------------------------------------------------------------------------*/
EANKeyframe::EANKeyframe(const EANKeyframe &source)
{
	this->frame = source.frame;
	this->x = source.x;
	this->y = source.y;
	this->z = source.z;
	this->w = source.w;
}
/*-------------------------------------------------------------------------------\
|                             read												 |
\-------------------------------------------------------------------------------*/
void EANKeyframe::read(File *file, unsigned char keyframe_size)
{
	if (keyframe_size == 1)
	{
		file->readFloat16E(&x);
		file->readFloat16E(&y);
		file->readFloat16E(&z);
		file->readFloat16E(&w);
	}else{
		file->readFloat32E(&x);
		file->readFloat32E(&y);
		file->readFloat32E(&z);
		file->readFloat32E(&w);
	}

	//LOG_DEBUG("Keyframe of size %d (%d at %d): %f %f %f %f\n", keyframe_size, frame, file->getCurrentAddress() - 16, x, y, z, w);
	LOG_DEBUG(" (keyframe_size:%i = >%s) X : %f, Y : %f, Z : %f, W : %f\n", keyframe_size, ((keyframe_size == 1) ? "Float16" : "Float32"), x, y, z, w);
}
/*-------------------------------------------------------------------------------\
|                             write												 |
\-------------------------------------------------------------------------------*/
void EANKeyframe::write(File *file, unsigned char keyframe_size)
{
	if (keyframe_size == 1)
	{
		file->writeFloat16E(&x);
		file->writeFloat16E(&y);
		file->writeFloat16E(&z);
		file->writeFloat16E(&w);
	}else if (keyframe_size == 2) {
		file->writeFloat32E(&x);
		file->writeFloat32E(&y);
		file->writeFloat32E(&z);
		file->writeFloat32E(&w);
	}else {
		LOG_DEBUG("Unknown Keyframe Size: %d\n", keyframe_size);
	}
	LOG_DEBUG(" (keyframe_size:%i = >%s) X : %f, Y : %f, Z : %f, W : %f\n", keyframe_size, ((keyframe_size == 1) ? "Float16" : "Float32"), x, y, z, w);
}
/*-------------------------------------------------------------------------------\
|                             readFrame											 |
\-------------------------------------------------------------------------------*/
void EANKeyframe::readFrame(File *file, unsigned char index_size)
{
	if (index_size == 0)
	{
		unsigned char c = 0;
		file->readUChar(&c);
		frame = c;
	}else if (index_size == 1) {
		unsigned short s = 0;
		file->readInt16E(&s);
		frame = s;
	}else {
		LOG_DEBUG("Unknown Frame Index Size: %d\n", index_size);
	}

	LOG_DEBUG(" (index_size:%i=>%s) frame : %i\n", index_size, ((index_size == 0) ? "Char" : "int16"), frame);
}
/*-------------------------------------------------------------------------------\
|                             writeFrame										 |
\-------------------------------------------------------------------------------*/
void EANKeyframe::writeFrame(File *file, unsigned char index_size)
{
	if (index_size == 0)
	{
		if (frame >= 256)
		{
			LOG_DEBUG("index of a frame %d is up to a octet (256) limit. please update frameCount\n", frame);
		}

		unsigned char c = frame;
		file->writeUChar(&c);

	}else if (index_size == 1) {
		unsigned short s = frame;
		file->writeInt16E(&s);

	}else {
		LOG_DEBUG("Unknown Frame Index Size: %d\n", index_size);
	}
	LOG_DEBUG(" (index_size:%i=>%s) frame : %i\n", index_size, ((index_size == 0) ? "Char" : "int16"), frame);
}
/*-------------------------------------------------------------------------------\
|                             setXYZW											 |
\-------------------------------------------------------------------------------*/
void EANKeyframe::setXYZW(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
/*-------------------------------------------------------------------------------\
|                             operator=											 |
\-------------------------------------------------------------------------------*/
void EANKeyframe::operator=(EANKeyframe *source)
{
	this->frame = source->frame;
	this->x = source->x;
	this->y = source->y;
	this->z = source->z;
	this->w = source->w;
}



}