#ifdef LIBXENOVERSE_ZLIB_SUPPORT

#ifndef LIBXENOVERSE_EMZ_H_INCLUDED
#define LIBXENOVERSE_EMZ_H_INCLUDED

#define LIBXENOVERSE_EMZ_SIGNATURE           "#EMZ"
#define LIBXENOVERSE_EMZ_CHUNK               16384

//for Shader Program TODO 
#define LIBXENOVERSE_SDS_SIGNATURE    "#SDS"
#define LIBXENOVERSE_SDS_EXTENSION    ".sds"


namespace LibXenoverse {
	class EMZ {
	protected:
		unsigned char *data;
		unsigned int data_size;
	public:
		EMZ() {
			data = NULL;
			data_size = 0;
		}

		~EMZ() {
			delete data;
		}

		void setDataPtr(unsigned char *data_p, unsigned int data_size_p) {
			data = data_p;
			data_size = data_size_p;
		}

		bool load(string filename);
		void save(string filename);
		void saveUncompressed(string filename);

		void read(File *file);
		void write(File *file);

		string detectNewExtension();
	};
}

#endif

#endif