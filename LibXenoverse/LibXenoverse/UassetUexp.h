
#ifndef LIBXENOVERSE_UASSET_UEXP_H_INCLUDED
#define LIBXENOVERSE_UASSET_UEXP_H_INCLUDED

#include "Uasset.h"
#include "Uexp.h"

namespace LibXenoverse
{

/***************************** class UassetUexp ***************************************/
class UassetUexp
{
	friend class EMA;

protected:
	Uasset* uasset;
	Uexp* uexp;
	bool isInternalLoaded;							//to know if it's about to clean. no clea n if the Uasset and Uexp files come from external

public:
	UassetUexp(void) { uasset = 0; uexp = 0; isInternalLoaded = false; }
	virtual ~UassetUexp(void) { clean(); }

	void	clean() { if (isInternalLoaded) { if (uasset) { delete uasset; }   if (uexp) { delete uexp; } } uexp = 0; uasset = 0; isInternalLoaded = false; }
	bool	load(string filename);
	void	save(string filename, bool big_endian = false);
	
	Uasset*	getUasset(void) { return uasset; }
	void	setUasset(Uasset* uasset) { this->uasset = uasset; isInternalLoaded = false; }
	Uexp*	getUexp(void) { return uexp; }
	void	setUexp(Uexp* uasset) { this->uexp = uexp; isInternalLoaded = false; }

	/*
	//todo
	bool	loadXml(string filename);
	void	saveXml(string filename);
	*/

	//Debug/work: create a file for wxHexEditor, for add tag and color on section of the file. also detect if a file still have empty part (but you better have to ouput in a log file)
	void save_Coloration(string filename, bool show_error = false);
};

}

#endif