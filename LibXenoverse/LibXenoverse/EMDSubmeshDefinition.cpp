namespace LibXenoverse
{


/*-------------------------------------------------------------------------------\
|                             zero					                             |
\-------------------------------------------------------------------------------*/
void EMDSubmeshDefinition::zero(void)
{
	flag0 = 0;
	texIndex = 0;
	flag1 = 0;
	flag2 = 0x22;
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
	file->readUChar(&flag1);
	file->readUChar(&flag2);
	file->readFloat32E(&textScale_u);
	file->readFloat32E(&textScale_v);

	LOG_DEBUG("Submesh Definition: %d, textIndex: %d, %d, %d, textScale_u: %f, textScale_v: %f\n", flag0, texIndex, flag1, flag2, textScale_u, textScale_v);
}
/*-------------------------------------------------------------------------------\
|                             write					                             |
\-------------------------------------------------------------------------------*/
void EMDSubmeshDefinition::write(File *file)
{
	file->writeUChar(&flag0);
	file->writeUChar(&texIndex);
	file->writeUChar(&flag1);
	file->writeUChar(&flag2);
	file->writeFloat32E(&textScale_u);
	file->writeFloat32E(&textScale_v);
}

}