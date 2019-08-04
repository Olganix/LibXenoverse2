#ifndef LIBXENOVERSE_EMM_H_INCLUDED
#define LIBXENOVERSE_EMM_H_INCLUDED

#define LIBXENOVERSE_EMM_SIGNATURE  "#EMM"

namespace LibXenoverse
{


/*-------------------------------------------------------------------------------\
|                             load					                             |
\-------------------------------------------------------------------------------*/
class EMMParameter
{
	public:
		string name;
		uint16_t type;
		uint16_t unknow_0;

		size_t uint_value;
		float float_value;

		EMMParameter(void);
		EMMParameter(string name, uint16_t type = 0x1, uint16_t unknow_0 = 0x0, size_t uint_value = 0, float float_value = 0.0f) { this->name = name; this->type = type; this->unknow_0 = unknow_0; this->uint_value = uint_value; this->float_value = float_value; }

		void	read(File *file);
		void	write(File *file);
		void	readXML(TiXmlElement* xmlCurrentNode);
		void	writeXML(TiXmlElement* xmlCurrentNode);
};



/*-------------------------------------------------------------------------------\
|                             load					                             |
\-------------------------------------------------------------------------------*/
class EMMMaterial
{
friend class EMM;

public:
	struct Sampler2D_shaderDefinition
	{
		string name;
		size_t samplerIndex;
		size_t size;

		Sampler2D_shaderDefinition(string name, size_t samplerIndex, size_t size)
		{
			this->name = name;
			this->samplerIndex = samplerIndex;
			this->size = size;
		}
	};

protected:
	string name;
	string shaderProgramName;
	vector<EMMParameter*> parameters;
	std::vector<Sampler2D_shaderDefinition> listSampler2D;				//list of defintion of sampler in shader. it will be used when 
			

public:
	EMMMaterial(string name = "") { this->name = name; shaderProgramName = ""; }
	~EMMMaterial(void);

	string	getName(void) { return string(name); }
	string	getShaderName(void) { return string(shaderProgramName); }
	vector<EMMParameter*>	&getParameters(void) { return parameters; }
	EMMParameter*	getParameter(const string &name);
	vector<Sampler2D_shaderDefinition>	&getListSampler2D(void) { return listSampler2D; }

	void	read(File *file);
	void	write(File *file);
	void	readXML(TiXmlElement* xmlCurrentNode);
	void	writeXML(TiXmlElement* xmlCurrentNode);
};



/*-------------------------------------------------------------------------------\
|                             EMM					                             |
\-------------------------------------------------------------------------------*/
class EMM
{
	friend class EMP;

protected:
	string name;
	vector<EMMMaterial*> materials;
	std::vector<size_t> listUnknowValues;

public:
	EMM(void) { name = "";  }
	~EMM(void);

	string	getName(void) { return name; }
	vector<EMMMaterial *>	&getMaterials(void) { return materials; }
	EMMMaterial*	getMaterial(string name);
	std::vector<string>	getListUniqueParameters(void);

	bool	load(string filename);
	void	save(string filename, bool big_endian=false);
	void	saveXML(string filename);
	void	read(File *file);
	void	write(File *file);	
	
};


}

#endif