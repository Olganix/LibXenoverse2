#ifndef LIBXENOVERSE_IGGYTEX_H_INCLUDED
#define LIBXENOVERSE_IGGYTEX_H_INCLUDED

#define LIBXENOVERSE_IGGYTEX_SIGNATURE    "oggytex"
#define LIBXENOVERSE_IGGYTEX_EXTENSION    ".iggytext"

namespace LibXenoverse
{
	class IGGYTEXFile
	{
		protected:
			char *data;
			size_t data_size;
			size_t data_id;
			string name;
			size_t index;
		public:
			IGGYTEXFile(string filename);

			IGGYTEXFile(size_t data_id_p, char *data_p, size_t data_size_p)
			{
				data_id = data_id_p;
				data = data_p;
				data_size = data_size_p;

				string id_str = ToString(data_id);
				while (id_str.size() < 6)
					id_str = "0" + id_str;
				name = "DATA" + id_str + ".dds";
				index = 0;
			}

			void setName(string name_p) { name = name_p; }
			string getName() { return name; }
			size_t getId() { return data_id; }
			void setIndex(size_t v) { index = v; }
			size_t getIndex() { return index; }

			char *getData() { return data; }
			size_t getSize(){ return data_size; }

			void write(File *file);
			void save(string filename);
	};


	class IGGYTEX
	{
		protected:
			vector<IGGYTEXFile *> files;
			string name;
		public:
			IGGYTEX()
			{
			}

			string getName() { return name; }
			vector<IGGYTEXFile *> &getFiles() { return files; }

			void addFile(string filename);
			void addFolder(string folder);
			
			bool load(string filename);
			void save(string filename, bool enable_filenames = true, bool big_endian = false);
			bool detectFilenameMode(); 
			void extract(string folder);
	};
}

#endif