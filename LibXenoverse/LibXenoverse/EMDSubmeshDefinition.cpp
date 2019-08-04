namespace LibXenoverse
{

/*-------------------------------------------------------------------------------\
|                             EMDSubmesh			                             |
\-------------------------------------------------------------------------------*/
EMDSubmeshDefinition::EMDSubmeshDefinition(EMDSubmeshDefinition* emdSubmeshDefinition)
{
	zero();

	if (emdSubmeshDefinition)
	{
		flag0 = emdSubmeshDefinition->flag0;
		texIndex = emdSubmeshDefinition->texIndex;
		adressMode_v = emdSubmeshDefinition->adressMode_v;
		adressMode_u = emdSubmeshDefinition->adressMode_u;
		filtering_magnification = emdSubmeshDefinition->filtering_magnification;
		filtering_minification = emdSubmeshDefinition->filtering_minification;
		textScale_u = emdSubmeshDefinition->textScale_u;
		textScale_v = emdSubmeshDefinition->textScale_v;
	}
}
/*-------------------------------------------------------------------------------\
|                             zero					                             |
\-------------------------------------------------------------------------------*/
void EMDSubmeshDefinition::zero(void)
{
	flag0 = 0;
	texIndex = 0;
	adressMode_u = adressMode_v = EMD_TUS_ADRESSMODE_WRAP;
	filtering_magnification = filtering_minification = EMD_TUS_FILTERING_LINEAR;
	textScale_u = 1.0f;
	textScale_v = 1.0f;
}
/*-------------------------------------------------------------------------------\
|                             read					                             |
\-------------------------------------------------------------------------------*/
void EMDSubmeshDefinition::read(File *file)
{
	file->readUChar(&flag0);
	file->readUChar(&texIndex);
	file->readUChar(&adressMode_u);
	adressMode_v = (adressMode_u & 0xF0) >> 4;
	adressMode_u = adressMode_u & 0x0F;
	file->readUChar(&filtering_minification);
	filtering_magnification = (filtering_minification & 0xF0) >> 4;
	filtering_minification = filtering_minification & 0x0F;
	file->readFloat32E(&textScale_u);
	file->readFloat32E(&textScale_v);
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void EMDSubmeshDefinition::write(File *file)
{
	file->writeUChar(&flag0);
	file->writeUChar(&texIndex);

	unsigned char tmp = (adressMode_v << 4) | adressMode_u;
	file->writeUChar(&tmp);
	tmp = (filtering_magnification << 4) | filtering_minification;
	file->writeUChar(&tmp);
	file->writeFloat32E(&textScale_u);
	file->writeFloat32E(&textScale_v);
}

}