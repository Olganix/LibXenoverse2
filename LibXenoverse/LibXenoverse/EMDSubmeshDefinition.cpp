namespace LibXenoverse
{

/*-------------------------------------------------------------------------------\
|                             EMDSubmesh			                             |
\-------------------------------------------------------------------------------*/
EMDTextureUnitState::EMDTextureUnitState(EMDTextureUnitState* emdTextureUnitState)
{
	zero();

	if (emdTextureUnitState)
	{
		flag0 = emdTextureUnitState->flag0;
		texIndex = emdTextureUnitState->texIndex;
		adressMode_v = emdTextureUnitState->adressMode_v;
		adressMode_u = emdTextureUnitState->adressMode_u;
		filtering_magnification = emdTextureUnitState->filtering_magnification;
		filtering_minification = emdTextureUnitState->filtering_minification;
		textScale_u = emdTextureUnitState->textScale_u;
		textScale_v = emdTextureUnitState->textScale_v;
	}
}
/*-------------------------------------------------------------------------------\
|                             zero					                             |
\-------------------------------------------------------------------------------*/
void EMDTextureUnitState::zero(void)
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
void EMDTextureUnitState::read(File *file)
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
void EMDTextureUnitState::write(File *file)
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