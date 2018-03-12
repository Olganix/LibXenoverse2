#ifndef LIBXENOVERSE_EMB_H_INCLUDED
#define LIBXENOVERSE_EMB_H_INCLUDED

#define LIBXENOVERSE_EMB_SIGNATURE    "#EMB"
#define LIBXENOVERSE_EMB_EXTENSION    ".emb"

namespace LibXenoverse
{

	class EMBFile
	{
		protected:
			char *data;
			size_t data_size;
			string name;
			size_t index;

		public:
			EMBFile(string filename);

			EMBFile(char *data_p, size_t data_size_p) {
				data = data_p;
				data_size = data_size_p;
				name = "";
				index = 0;
			}

			char *getData() { return data; }
			size_t getSize() { return data_size; }
			void setName(string name_p) { name = name_p; }
			string getName() { return name; }
			void setIndex(size_t v) { index = v; }
			size_t getIndex() { return index; }

			void write(File *file);
			void save(string filename);

			EMBFile* clone(void);
	};


	class EMB
	{
		protected:
			vector<EMBFile*> files;
			string name;
		public:
			EMB() { }

			string getName() { return name; }
			vector<EMBFile *> &getFiles() { return files; }

			bool load(string filename);
			void extract(string folder);
			void save(string filename, bool enable_filenames = true, bool big_endian = false);

			void addFile(string filename);
			void addFolder(string folder);

			bool detectFilenameMode();

			EMB* clone(void);

			#ifdef LIBXENOVERSE_FBX_SUPPORT
			void exportShadersToFx(EMB *shader_pack_vs,  EMB *shader_pack_ps);
			#endif
	};
}

#endif