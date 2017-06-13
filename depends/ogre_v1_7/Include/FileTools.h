#ifndef __FileTools__H_
#define __FileTools__H_
#pragma once

#include "StringTools.h"

#include <direct.h>
#include <sys/stat.h>

// Some path manipulation tools
class FileSystemUtil
{
public:
	// Exact copy of what's inside OgreFileSystem.cpp
	static bool is_absolute_path(const Ogre::Char* path)
	{
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		if (isalpha(Ogre::uchar(path[0])) && path[1] == ':')
			return true;
	#endif
		return path[0] == '/' || path[0] == '\\';
	}
	inline static bool is_absolute_path(const Ogre::String& path)
	{
		return is_absolute_path(path.c_str());
	}

	// Path concatenation, based on the one that is inside OgreFileSystem.cpp
	// Modified in such a way it resolves "." and ".." navigations
	static Ogre::String concatenate_path(const Ogre::String& base, const Ogre::String& name, const Ogre::Char dirSep = '/')
	{
		if (base.empty() || base=="." || is_absolute_path(name.c_str()))
			return name;
		else
		{
			size_t bl = base.length();
			if (name[0]=='.')
			{
				size_t nl = name.length(), pl = base.find_last_of("..");
				if (pl==Ogre::String::npos || bl>pl+2) // base longer than "../"
				{
					if (name[1]=='.') // name begins with "..", so we have to remove last directory from base
					{
						size_t i = base.find_last_of(dirSep,bl - 2),cut = 2 + (nl>2);
						if (i==Ogre::String::npos)
							return name.substr(cut);
						return concatenate_path(base.substr(0,i),name.substr(cut));
					}
					if (nl>1)
						return concatenate_path(base,name.substr(2));
					return base;
				}
			}
			if (base[bl-1]==dirSep)
				return base + name;
			return base + dirSep + name;
		}
	}

	/// <summary>
	/// Simple file existence test based on how Ogre does it
	/// </summary>
	/// <param name="name">The full Path Name of the File.</param>
	/// <param name="cleanDirs">Whether it is needed to remove trailing 'Slashes' if the Name is a Directory one.</param>
	inline static bool fileExists(const Ogre::String& name, bool cleanDirs=true)
	{
		struct stat tagStat;
		if (cleanDirs && name.find_last_of("/\\")==name.size()-1)
		{
			Ogre::String local = name;
			Ogre::StringUtilExt::trim(local,false,true,"/\\");
			return stat(local.c_str(), &tagStat) == 0;
		}
		return stat(name.c_str(), &tagStat) == 0;
	}

	/// <summary>
	/// This utility tests recursively the existence of directories
	/// from within a path string
	/// and creates them if needed
	/// it assumes the path has been standardized
	/// returns false on failure
	/// </summary>
	/// <param name="outPath">The full Path.</param>
	static bool directoryCreationCheckOut(Ogre::String &outPath)
	{
		if (outPath.empty())
			return false;
		Ogre::StringUtilExt::trim(outPath,false,true,"/\\");
		if (!fileExists(outPath,false)) // Directory doesn't exist
		{
			const Ogre::Char*outPathStr = outPath.c_str();
			if (_mkdir(outPathStr)<0 && errno==2/*ENOENT*/)
			{
				Ogre::String dirName,baseName;
				Ogre::StringUtil::splitFilename(outPath,baseName,dirName);
//				StringUtilExt::trim(dirName,false,true,"/\\");
				if (!directoryCreationCheckOut(dirName) || _mkdir(outPathStr)<0)
					return false;
			}
		}
		return true;
	}
	inline static bool directoryCreationCheckOut(const Ogre::String &outPath)
	{
		Ogre::String cpOutPath = outPath;
		return directoryCreationCheckOut(cpOutPath);
	}

	/// <summary>
	/// Splits the tile name in different sub directories
	/// in order to avoid too many files inside the same directory
	/// An option in order to only create the path string.
	/// </summary>
	/// <param name="fullFileName">The FileName to be splitted.</param>
	/// <param name="dirSliceSize">The maximum Number of char a Dir Name can contain.</param>
	static Ogre::String splitIntoDirectories(const Ogre::String& fullFileName, const size_t dirSliceSize,const size_t smallestDirGran, const bool actualDirectoryCheckOut=false)
	{
		Ogre::String fileName = Ogre::StringUtil::BLANK;
		Ogre::String treatedFileName = fullFileName;

		while (treatedFileName.length() > dirSliceSize + smallestDirGran)
		{
			fileName = concatenate_path(fileName,treatedFileName.substr(0, dirSliceSize));
			if (actualDirectoryCheckOut)
				directoryCreationCheckOut(fileName);
			treatedFileName = treatedFileName.substr(dirSliceSize);
		}
		return fileName;
	}
};

#endif //__FileTools__H_