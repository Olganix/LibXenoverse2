
#ifndef LIBXENOVERSE_EMA_MATERIAL_H_INCLUDED
#define LIBXENOVERSE_EMA_MATERIAL_H_INCLUDED

namespace LibXenoverse
{

class EMA_Material;
class EMA_Material_Animation;

/***************************** class EMA_Material_KeyFrame ***************************************/
class EMA_Material_KeyFrame
{
public:
	size_t frameIndex;
	float x;
	float y;
	float z;
	float w;

	EMA_Material_KeyFrame(size_t frameIndex = 0, float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f) { this->frameIndex = frameIndex; this->x = x; this->y = y; this->z = z; this->w = w; }
	virtual ~EMA_Material_KeyFrame(void) { }

	bool	importXml(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	exportXml(size_t emaParamType);
};


/***************************** class EMA_Material_MaterialParameter ***************************************/
class EMA_Material_MaterialParameter
{
	friend class EMA_Material_Animation;

public:
	enum EMA_MaterialParameter_Type
	{
		EMPT_Float = 0,
		EMPT_Vector2,
		EMPT_Vector3,
		EMPT_Vector4,
	};


protected:
	string name;
	vector<EMA_Material_KeyFrame> keyframes;
	EMA_MaterialParameter_Type emaParamType;


public:
	EMA_Material_MaterialParameter(string name = "unknow", EMA_MaterialParameter_Type emaParamType = EMPT_Float) { this->name = name;  this->emaParamType = emaParamType; }
	virtual ~EMA_Material_MaterialParameter(void) { keyframes.clear(); }

	vector<EMA_Material_KeyFrame> &getKeyframe() { return keyframes; }

	bool	importXml(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	exportXml(void);
};



/***************************** class EMA_Material_Material ***************************************/
class EMA_Material_Material
{
	friend class EMA_Material_Animation;
	friend class EMA_Material;

protected:
	string name;
	vector<EMA_Material_MaterialParameter> materialParamters;

public:
	EMA_Material_Material(string name = "unknow") { this->name = name;}
	virtual ~EMA_Material_Material(void) { materialParamters.clear(); }

	vector<EMA_Material_MaterialParameter> &getMaterialParamters(){ return materialParamters; }

	void	readEmaEmm(EmaCommand* ema, std::vector<float>* values, EMM* emm);
	void	writeEma(EmaCommand* ema, std::vector<float>* values, EMM* emm);

	bool	importXml(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	exportXml(void);
};





/***************************** class EMA_Material_Animation ***************************************/
class EMA_Material_Animation
{
	friend class EMA_Material;

protected:
	string name;
	size_t frame_count;
	vector<EMA_Material_Material> materials;

	uint16_t type;
	uint16_t frame_float_size;

public:
	EMA_Material_Animation(string name = "unknow") { this->name = name; frame_count = 0; type = 0; frame_float_size = 0; }
	virtual ~EMA_Material_Animation(void) { materials.clear(); }
	
	string	getName(void) { return name; }
	void	setName(string newName) { name = newName; }
	size_t	getFrameCount(void) { return frame_count; }
	void	setFrameCount(size_t value) { frame_count = value; }

	vector<EMA_Material_Material>	&getMaterials(void) { return materials; }

	void	readEmaEmm(EmaAnimation* ema, std::vector<EMO_Bone>* bones, EMM* emm);
	void	writeEma(EmaAnimation* ema, EMO_Skeleton* emoSkeleton, EMM* emm);

	bool	importXml(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	exportXml(void);
};






/***************************** class EMA_Material ***************************************/
class EMA_Material
{
protected:
	string name;
	vector<EMA_Material_Animation> animations;

	uint16_t unk_08;				//from EMA
	uint16_t unk_12;				//from EMA

public:
	EMA_Material(void) { name = ""; unk_08 = 0; unk_12 = 0; }
	virtual ~EMA_Material(void) { animations.clear(); }

	bool	load(string filename);
	void	save(string filename, bool big_endian = false);
	void	readEmaEmm(EMA* ema, EMM* emm);
	void	writeEma(EMA* ema, EMM* emm);

	string	getName(void) { return name; }
	vector<EMA_Material_Animation>	&getAnimations(void) { return animations; }

	bool	loadXml(string filename);
	void	saveXml(string filename);
	bool	importXml(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	exportXml(void);
};

}

#endif