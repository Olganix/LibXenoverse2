#ifndef LIBXENOVERSE_SDS_H_INCLUDED
#define LIBXENOVERSE_SDS_H_INCLUDED

#define LIBXENOVERSE_SDS_SIGNATURE  "#SDS"

namespace LibXenoverse
{


class SDSParameter
{
	public:
		enum parameterType
		{
			MtxP = 0,
			GblP,
			VsP,
			PsP,
			max
		};
		static std::vector<string> parameterTypeNames;
		
		string name;
		string  type;						//"MtxP" for matrix, "GblP" for global parameters, "PsP" Pixel shader parameters, "VsP" for vertexshader parameter.

		SDSParameter(const string &name = "", const string &type = "")
		{
			this->name = name;
			this->type = type;


			if (parameterTypeNames.size() == 0)			//todo better.
			{
				parameterTypeNames.push_back("MtxP");
				parameterTypeNames.push_back("GblP");
				parameterTypeNames.push_back("VsP");
				parameterTypeNames.push_back("PsP");
			}
		}
		

		void read(File *file);
		void write(File *file);
		void readXML(TiXmlElement *root);
		void writeXML(TiXmlElement *root);
};

class SDSShaderProgram
{
	protected:
		string name;
		string vertexShader_name;
		string pixelShader_name;
		vector<SDSParameter *> parameters;
	public:
		SDSShaderProgram()
		{
			name = vertexShader_name = pixelShader_name;
		}
		string getName() { return name; }
		string getVertexShaderName() { return vertexShader_name; }
		string getPixelShaderName() { return pixelShader_name; }
		vector<SDSParameter *> &getParameters() { return parameters; }

		void read(File *file);
		void write(File *file);
		void readXML(TiXmlElement *root);
		void writeXML(TiXmlElement *root);
};

class SDS
{
	protected:
		string name;
		vector<SDSShaderProgram *> shaderPrograms;
		
	public:
		SDS()
		{
			name = "";
		}
		string getName() { return name; }
		vector<SDSShaderProgram *> &GetshaderPrograms() { return shaderPrograms; }

		bool load(string filename);
		void save(string filename, bool big_endian=false);
		void read(File *file);
		void write(File *file);
		void readXML(TiXmlElement *root);
		void saveXML(string filename);
};


}

#endif