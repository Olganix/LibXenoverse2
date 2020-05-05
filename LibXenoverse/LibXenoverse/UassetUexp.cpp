
#include "UassetUexp.h"


namespace LibXenoverse
{

/*-------------------------------------------------------------------------------\
|                             load								                 |
\-------------------------------------------------------------------------------*/
bool UassetUexp::load(string filename)
{
	clean();
	isInternalLoaded = true;

	string basename = filenameNoExtension(filename);
	uasset = new Uasset();
	if (!uasset->load(basename + ".uasset"))
	{
		delete uasset;
		return false;
	}

	uexp = new Uexp();
	if (!uexp->load(basename + ".uexp"))
	{
		delete uexp;
		return false;
	}

	return true;
}
/*-------------------------------------------------------------------------------\
|                             save					                             |
\-------------------------------------------------------------------------------*/
void UassetUexp::save(string filename, bool big_endian)
{
	string basename = filenameNoExtension(filename);
	uasset->save(basename + ".uasset");
	uexp->save(basename + ".uexp");
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// wxHexEditor coloration ////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------\
|                             save_Coloration		                             |
\-------------------------------------------------------------------------------*/
void UassetUexp::save_Coloration(string filename, bool show_error)
{
	string basename = filenameNoExtension(filename);
	uasset->save_Coloration(basename + ".uasset", show_error);
	uexp->save_Coloration(basename + ".uexp", show_error);
}

}