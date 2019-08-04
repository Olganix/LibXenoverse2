namespace LibXenoverse
{

/*-------------------------------------------------------------------------------\
|                             EMDVertex									         |
\-------------------------------------------------------------------------------*/
EMDVertex::EMDVertex(EMDVertex* emdVertex)
{
	zero();

	if (emdVertex)
	{
		flags = emdVertex->flags;
		pos_x = emdVertex->pos_x;
		pos_y = emdVertex->pos_y;
		pos_z = emdVertex->pos_z;
		norm_x = emdVertex->norm_x;
		norm_y = emdVertex->norm_y;
		norm_z = emdVertex->norm_z;
		tang_x = emdVertex->tang_x;
		tang_y = emdVertex->tang_y;
		tang_z = emdVertex->tang_z;
		text_u = emdVertex->text_u;
		text_v = emdVertex->text_v;
		text2_u = emdVertex->text2_u;
		text2_v = emdVertex->text2_v;
		color = emdVertex->color;

		for (size_t i = 0; i < 4; i++)
		{
			blend[i] = emdVertex->blend[i];
			blend_weight[i] = emdVertex->blend_weight[i];
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             zero					                             |
\-------------------------------------------------------------------------------*/
void EMDVertex::zero(void)
{
	flags = 0;
	pos_x = pos_y = pos_z = norm_x = norm_z = tang_y = tang_z = text_u = text_v = text2_u = text2_v = blend_weight[0] = blend_weight[1] = blend_weight[2] = blend_weight[3] = 0.0f;
	norm_y = tang_x = 1.0f;
	color = 0;
	blend[0] = blend[1] = blend[2] = blend[3] = 0;
}
/*-------------------------------------------------------------------------------\
|                             opeartor ==			                             |
\-------------------------------------------------------------------------------*/
bool EMDVertex::operator == (const EMDVertex& vertex)
{
	if (this->flags != vertex.flags) return false;

	if ( (this->flags & EMD_VTX_FLAG_POS) && ((this->pos_x != vertex.pos_x) || (this->pos_y != vertex.pos_y) || (this->pos_z != vertex.pos_z) ) )
		return false;
	
	if ((this->flags & EMD_VTX_FLAG_NORM) && ((this->norm_x != vertex.norm_x) || (this->norm_y != vertex.norm_y) || (this->norm_z != vertex.norm_z)))
		return false;

	if ((this->flags & EMD_VTX_FLAG_TANGENT) && ((this->tang_x != vertex.tang_x) || (this->tang_y != vertex.tang_y) || (this->tang_z != vertex.tang_z)))
		return false;

	if ((this->flags & EMD_VTX_FLAG_TEX) && ((this->text_u != vertex.text_u) || (this->text_v != vertex.text_v)))
		return false;
	
	if ((this->flags & EMD_VTX_FLAG_TEX2) && ((this->text2_u != vertex.text2_u) || (this->text2_v != vertex.text2_v)))
		return false;

	if ((this->flags & EMD_VTX_FLAG_COLOR) && (this->color != vertex.color))
		return false;

	
	if (flags & EMD_VTX_FLAG_BLEND_WEIGHT)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (this->blend[i] != vertex.blend[i]) return false;
			if (this->blend_weight[i] != vertex.blend_weight[i]) return false;
		}
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void EMDVertex::read(File *file, uint16_t flags)
{
	this->flags = flags;
	
	if (flags & EMD_VTX_FLAG_POS)
	{
		file->readFloat32E(&pos_x);
		file->readFloat32E(&pos_y);
		file->readFloat32E(&pos_z);
	}

	if (!(flags & EMG_VTX_FLAG_COMPRESSED_FORMAT))
	{
		if (flags & EMD_VTX_FLAG_NORM)
		{
			file->readFloat32E(&norm_x);
			file->readFloat32E(&norm_y);
			file->readFloat32E(&norm_z);
		}

		if (flags & EMD_VTX_FLAG_TEX)
		{
			file->readFloat32E(&text_u);
			file->readFloat32E(&text_v);
		}

		if (flags & EMD_VTX_FLAG_TEX2)
		{
			file->readFloat32E(&text2_u);
			file->readFloat32E(&text2_v);
		}

		if (flags & EMD_VTX_FLAG_TANGENT)
		{
			file->readFloat32E(&tang_x);
			file->readFloat32E(&tang_y);
			file->readFloat32E(&tang_z);
		}

	}else {

		if (flags & EMD_VTX_FLAG_NORM)
		{
			file->readFloat16E(&norm_x);
			file->readFloat16E(&norm_y);
			file->readFloat16E(&norm_z);
			file->moveAddress(2);
		}

		
		if (flags & EMD_VTX_FLAG_TEX)
		{
			file->readFloat16E(&text_u);
			file->readFloat16E(&text_v);
		}

		if (flags & EMD_VTX_FLAG_TEX2)
		{
			file->readFloat16E(&text2_u);
			file->readFloat16E(&text2_v);
		}

		if (flags & EMD_VTX_FLAG_TANGENT)
		{
			file->readFloat16E(&tang_x);
			file->readFloat16E(&tang_y);
			file->readFloat16E(&tang_z);
			file->moveAddress(2);
		}
	}


	

	if (flags & EMD_VTX_FLAG_COLOR)
		file->readInt32E(&color);

	if (flags & EMD_VTX_FLAG_BLEND_WEIGHT)
	{
		for (size_t i = 0; i < 4; i++)
			file->readUChar(&blend[ ((!file->isBigEndian()) ? i : (3 - i)) ]);


		if (!(flags & EMG_VTX_FLAG_COMPRESSED_FORMAT))
		{
			for (size_t i = 0; i < 3; i++)				//only 3 informations, because the last is a completion.
				file->readFloat32E(&blend_weight[ i ]);
		}else {
			for (size_t i = 0; i < 4; i++)
				file->readFloat16E(&blend_weight[ i ]);
		}


		//completion to assure addition of weight always do 1.0;
		blend_weight[3] = 1.0f - (blend_weight[0] + blend_weight[1] + blend_weight[2]);

		LOG_DEBUG("%d %d %d %d - %f %f %f %f\n", blend[0], blend[1], blend[2], blend[3], blend_weight[0], blend_weight[1], blend_weight[2], blend_weight[3]);
	}
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void EMDVertex::write(File *file, uint16_t flags)
{
	unsigned short nullValue = 0x00;
	
	if (flags & EMD_VTX_FLAG_POS)
	{
		file->writeFloat32E(&pos_x);
		file->writeFloat32E(&pos_y);
		file->writeFloat32E(&pos_z);
	}

	if (!(flags & EMG_VTX_FLAG_COMPRESSED_FORMAT))
	{
		if (flags & EMD_VTX_FLAG_NORM)
		{
			file->writeFloat32E(&norm_x);
			file->writeFloat32E(&norm_y);
			file->writeFloat32E(&norm_z);
		}

		if (flags & EMD_VTX_FLAG_TEX)
		{
			file->writeFloat32E(&text_u);
			file->writeFloat32E(&text_v);
		}

		if (flags & EMD_VTX_FLAG_TEX2)
		{
			file->writeFloat32E(&text2_u);
			file->writeFloat32E(&text2_v);
		}

		if (flags & EMD_VTX_FLAG_TANGENT)
		{
			file->writeFloat32E(&tang_x);
			file->writeFloat32E(&tang_y);
			file->writeFloat32E(&tang_z);
		}

	}else {

		if (flags & EMD_VTX_FLAG_NORM)
		{
			file->writeFloat16E(&norm_x);
			file->writeFloat16E(&norm_y);
			file->writeFloat16E(&norm_z);
			file->writeInt16E(&nullValue);
		}

		if (flags & EMD_VTX_FLAG_TEX)
		{
			file->writeFloat16E(&text_u);
			file->writeFloat16E(&text_v);
		}

		if (flags & EMD_VTX_FLAG_TEX2)
		{
			file->writeFloat16E(&text2_u);
			file->writeFloat16E(&text2_v);
		}

		if (flags & EMD_VTX_FLAG_TANGENT)
		{
			file->writeFloat16E(&tang_x);
			file->writeFloat16E(&tang_y);
			file->writeFloat16E(&tang_z);
			file->writeInt16E(&nullValue);
		}
	}



	
	

	if (flags & EMD_VTX_FLAG_COLOR)
		file->writeInt32E(&color);

	if (flags & EMD_VTX_FLAG_BLEND_WEIGHT)
	{
		for (size_t i = 0; i < 4; i++)
			file->writeUChar(&blend[ ((!file->isBigEndian()) ? i : (3 - i)) ]);


		if (!(flags & EMG_VTX_FLAG_COMPRESSED_FORMAT))
		{
			for (size_t i = 0; i < 3; i++)				//only 3 informations, because the last is a completion.
				file->writeFloat32E(&blend_weight[ i ]);
		}
		else {
			for (size_t i = 0; i < 3; i++)
				file->writeFloat16E(&blend_weight[ i ]);

			file->writeInt16E(&nullValue);
		}
	}
}
/*-------------------------------------------------------------------------------\
|                             getColorRGBAFloat		                             |
\-------------------------------------------------------------------------------*/
//
//	get values into [0,1] ranges
//
void EMDVertex::getColorRGBAFloat(float &r, float &g, float &b, float &a)
{
	uint32_t color_tmp = color;
	
	r = ((float)((color_tmp >> 24) & 0xFF)) / 255.0f;
	g = ((float)((color_tmp >> 16) & 0xFF)) / 255.0f;
	b = ((float)((color_tmp >> 8) & 0xFF)) / 255.0f;
	a = ((float)(color_tmp & 0xFF)) / 255.0f;
}
/*-------------------------------------------------------------------------------\
|                             setColorFromRGBAFloat		                         |
\-------------------------------------------------------------------------------*/
//
//	set values from [0,1] ranges
//
void EMDVertex::setColorFromRGBAFloat(float r, float g, float b, float a)
{
	color = 0;
	color += (((int)floorf(max(min(r, 1.0f), 0.0f) * 255.0f)) & 0xFF) << 24;
	color += (((int)floorf(max(min(g, 1.0f), 0.0f) * 255.0f)) & 0xFF) << 16;
	color += (((int)floorf(max(min(b, 1.0f), 0.0f) * 255.0f)) & 0xFF) << 8;
	color += (((int)floorf(max(min(a, 1.0f), 0.0f) * 255.0f)) & 0xFF );
}
/*-------------------------------------------------------------------------------\
|                             getSizeFromFlags		                             |
\-------------------------------------------------------------------------------*/
size_t EMDVertex::getSizeFromFlags(uint16_t flags, bool fromFile)
{
	size_t nbOctets = 0;

	bool isCompressed = ((flags & EMD_VTX_FLAG_COMPRESSED_FORMAT) && (fromFile));

	if (flags & EMD_VTX_FLAG_POS)
		nbOctets += 3 * 4;

	if (flags & EMD_VTX_FLAG_NORM)
		nbOctets += ((isCompressed) ? (4 * 2) : (3 * 4));

	if (flags & EMD_VTX_FLAG_TANGENT)
		nbOctets += ((isCompressed) ? (4 * 2) : (3 * 4));

	if (flags & EMD_VTX_FLAG_TEX)
		nbOctets += 2 * ((isCompressed) ? 2 : 4);

	if (flags & EMD_VTX_FLAG_TEX2)
		nbOctets += 2 * ((isCompressed) ? 2 : 4);

	if (flags & EMD_VTX_FLAG_COLOR)
		nbOctets += 4;

	if (flags & EMD_VTX_FLAG_BLEND_WEIGHT)
		nbOctets += 4 + ((isCompressed) ? (4 * 2) : (3 * 4));

	return nbOctets;
}
/*-------------------------------------------------------------------------------\
|                             getFlagsVertexDefinitionInformations               |
\-------------------------------------------------------------------------------*/
std::string EMDVertex::getFlagsVertexDefinitionInformations(uint16_t flags)
{
	std::ostringstream s;
	s << "VertexDefition for flag : " << flags << std::endl;
	
	size_t nbOctets = 0;

	bool isCompressed = ((flags & EMD_VTX_FLAG_COMPRESSED_FORMAT)!=0);
	if (isCompressed)
		s << "is compressed format" << std::endl;

	if (flags & EMD_VTX_FLAG_POS)
	{
		s << "Position" << std::endl;
		nbOctets += 3 * 4;
	}

	if (flags & EMD_VTX_FLAG_NORM)
	{
		s << "Normal" << std::endl;
		nbOctets += ((isCompressed) ? (4 * 2) : (3 * 4));
	}

	if (flags & EMD_VTX_FLAG_TANGENT)
	{
		s << "Tangent" << std::endl;
		nbOctets += ((isCompressed) ? (4 * 2) : (3 * 4));
	}

	if (flags & EMD_VTX_FLAG_TEX)
	{
		s << "TextureUv_1" << std::endl;
		nbOctets += 2 * ((isCompressed) ? 2 : 4);
	}

	if (flags & EMD_VTX_FLAG_TEX2)
	{
		s << "TextureUv_2" << std::endl;
		nbOctets += 2 * ((isCompressed) ? 2 : 4);
	}

	if (flags & EMD_VTX_FLAG_COLOR)
	{
		s << "Color" << std::endl;
		nbOctets += 4;
	}

	if (flags & EMD_VTX_FLAG_BLEND_WEIGHT)
	{
		s << "Bone_Blend_Weight" << std::endl;
		nbOctets += 4 + ((isCompressed) ? (4 * 2) : (3 * 4));
	}

	s << "=> For " << nbOctets << "octets" << std::endl;
	return s.str();
}


}