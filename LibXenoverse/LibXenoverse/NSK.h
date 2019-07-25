#ifndef LIBXENOVERSE_NSK_H_INCLUDED
#define LIBXENOVERSE_NSK_H_INCLUDED

#define LIBXENOVERSE_NSK_SIGNATURE           "#ESK"


namespace LibXenoverse
{

class NSK
{
	protected:
		string mName;
		EMD* mEmd;
		ESK* mEsk;
		
	public:
		NSK(void) { mEmd = 0; mEsk = 0; }

		string	getName() { return mName; }
		EMD* getEmd() { return mEmd; };
		ESK* getEsk() { return mEsk; };

		bool	load(string filename);
		void	save(string filename, bool big_endian = false);
		void	extract(string folder, bool big_endian = false);

		bool	loadXml(string filename);
		void	saveXml(string filename);

		void	read(File *file);
		void	write(File *file);

		void	addEmdFile(const std::string filename);
		void	addEskFile(const std::string filename);
};

}

#endif