#ifndef LIBXENOVERSE_AUR_H_INCLUDED
#define LIBXENOVERSE_AUR_H_INCLUDED

#define LIBXENOVERSE_AUR_SIGNATURE  "#AUR"

namespace LibXenoverse
{


class AUR;

/*-------------------------------------------------------------------------------\
|                             AURCharac										     |
\-------------------------------------------------------------------------------*/
class AURCharac
{
protected:
	size_t mIdCharac;
	size_t mCostumeId;
	size_t mIdAura;
	bool mGlare;			//bool mEmanation (at the end of the flare.)

public:
	AURCharac() { mIdCharac = 0; mCostumeId = 0; mIdAura = 0; mGlare = 0; }
	~AURCharac(void) { }

	void	read(File *file);
	void	write(File *file);

	bool	readXML(TiXmlElement* xmlCurrentNode);
	TiXmlElement*	writeXml(void);
};

/*-------------------------------------------------------------------------------\
|                             AURAuraColor					                     |
\-------------------------------------------------------------------------------*/
class AURAuraEffect
{
friend class AURAura;

protected:
	size_t mIdAuraType;
	size_t mIdEffect;

public:
	AURAuraEffect(size_t idAuraType, size_t idEffect) { mIdAuraType = idAuraType; mIdEffect = idEffect; }
	~AURAuraEffect(void) { }

};

/*-------------------------------------------------------------------------------\
|                             AURAura					                         |
\-------------------------------------------------------------------------------*/
class AURAura
{
protected:
	size_t mId;
	size_t mUnknow0;
	
	std::vector<AURAuraEffect> mListColorByType;		//linked to AuraTypes.

public:
	AURAura(){ mId = 0; }
	~AURAura(void) { }

	void	read(File *file);
	void	write(File *file, size_t &startColorAddress);

	bool	readXML(TiXmlElement* xmlCurrentNode, std::vector<string> &listAuraType);
	TiXmlElement*	writeXml(std::vector<string> &listAuraType);
};



/*-------------------------------------------------------------------------------\
|                             AUR					                             |
\-------------------------------------------------------------------------------*/
class AUR
{
protected:
	std::vector<AURAura> mListAura;
	std::vector<string> mListAuraType;
	std::vector<AURCharac> mListCharacLink;
	

public:
	AUR(void) { }
	~AUR(void) { }

	bool	load(string filename);
	void	save(string filename, bool big_endian=false);
	bool	loadXml(string filename);
	void	saveXml(string filename);
	void	read(File *file);
	void	write(File *file);	
	
};


}

#endif