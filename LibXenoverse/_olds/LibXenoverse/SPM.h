#ifndef LIBXENOVERSE_SPM_H_INCLUDED
#define LIBXENOVERSE_SPM_H_INCLUDED

#define LIBXENOVERSE_SPM_SIGNATURE  "#SPM"

namespace LibXenoverse {
	class SPMParameter {
		public:
			char name[32];
			unsigned int type;
			unsigned int value;

			SPMParameter() {
			}

			void read(File *file);
			void readXML(TiXmlElement *root);
			void write(File *file);
			void writeXML(TiXmlElement *root);

			float valueFloat() {
				return *(float *)(&value);
			}
	};

	class SPMMaterial
	{
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
			char name[32];
			char shader_name[32];
			vector<SPMParameter *> parameters;
			std::vector<Sampler2D_shaderDefinition> listSampler2D;				//list of defintino of sampler in shader. it will be used when 
			

		public:
			SPMMaterial() { }

			string getName() { return string(name); }
			string getShaderName() { return string(shader_name); }
			vector<SPMParameter *> &getParameters() { return parameters; }
			SPMParameter *getParameter(const string &name);
			vector<Sampler2D_shaderDefinition> &getListSampler2D() { return listSampler2D; }

			void read(File *file);
			void readXML(TiXmlElement *root);
			void write(File *file);
			void writeXML(TiXmlElement *root);
	};

	class SPM {
		protected:
			vector<SPMMaterial *> materials;
			string name;
		public:
			SPM() {
			}

			vector<SPMMaterial *> &getMaterials() {
				return materials;
			}

			SPMMaterial* getMaterial(string name)
			{
				size_t nbMaterial = materials.size();
				for (size_t i = 0; i < nbMaterial; i++)
					if (materials.at(i)->getName() == name)
						return materials.at(i);
				return nullptr;
			}

			bool load(string filename);
			void save(string filename, bool big_endian=false);
			void saveXML(string filename);

			void read(File *file);
			void readXML(TiXmlElement *root);
			void write(File *file);

			string getName() {
				return name;
			}
	};
}

#endif