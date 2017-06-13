#ifndef __ExtendedResourceManager__H_
#define __ExtendedResourceManager__H_

#pragma once

#include "FileTools.h"
#include <OgreLogManager.h>
#include <OgreArchiveManager.h>
#include <OgreConfigFile.h>
#include <OgreResourceGroupManager.h>
//#include <OgreFileSystem.h>
//#include <direct.h>

namespace Ogre {
	// Some path manipulation tools

    // Exact copy of what's inside OgreFileSystem.cpp
//    static bool is_absolute_path(const char* path)
//    {
//#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//        if (isalpha(uchar(path[0])) && path[1] == ':')
//            return true;
//#endif
//        return path[0] == '/' || path[0] == '\\';
//    }
//
//    // Path concatenation, based on the one that is inside OgreFileSystem.cpp
//	// Modified in such a way it resolves "." and ".." navigations
//    static String concatenate_path(const String& base, const String& name)
//    {
//        if (base.empty() || base=="." || is_absolute_path(name.c_str()))
//            return name;
//        else
//		{
//			size_t bl = base.length(), pl = base.find_last_of("..");
//			if (name[0]=='.')
//			{
//				size_t nl = name.length();
//				if (pl==String::npos || bl>pl+2) // base longer than "../"
//				{
//					if (name[1]=='.') // name begins with "..", so we have to remove last directory from base
//					{
//						size_t i = base.find_last_of('/',bl - 2),cut = 2 + (nl>2);
//						if (i==String::npos)
//							return name.substr(cut);
//						return concatenate_path(base.substr(0,i),name.substr(cut));
//					}
//					if (nl>1)
//						return concatenate_path(base,name.substr(2));
//					return base;
//				}
//			}
//			if (base[bl-1]=='/')
//				return base + name;
//            return base + '/' + name;
//		}
//    }
//
//	// Simple file existence test based on how Ogre does it
//	inline static bool fileExists(String name)
//	{
//		struct stat tagStat;
//		return stat(name.c_str(), &tagStat) == 0;
//	}
//
//	// This utility tests recursively the existence of directories
//	// from within a path string
//	// and creates them if needed
//	// it assumes the path has been standardized
//	static bool directoryCreationCheckOut(String outPath)
//	{
//        if (outPath[outPath.length() - 1] == '/')
//			outPath[outPath.length() - 1] = 0;
//		const char*outPathStr = outPath.c_str();
//		if (!fileExists(outPathStr)) // Directory doesn't exist
//			if (_mkdir(outPathStr)<0 && errno==2/*ENOENT*/)
//			{
//				String dirName,baseName;
//				StringUtil::splitFilename(outPath,baseName,dirName);
//				if (!directoryCreationCheckOut(dirName) || _mkdir(outPathStr)<0)
//					return false;
//			}
//		return true;
//	}

//static
class ResourceManagerExtensions //: ResourceGroupManager
{
public:
	// Verify if a Group is known from the ResourceGroupManager
	// Might be enhanced using getResourceGroup(), and added (patched) to the actual ResourceManager
	inline static bool resourceGroupExists(String groupName, ResourceGroupManager *rsm = ResourceGroupManager::getSingletonPtr())
	{
		StringVector grps = rsm->getResourceGroups();
		StringVector::iterator grpnd = grps.end(),cgp = std::find(grps.begin(),grpnd,groupName);
		return cgp!=grpnd;
	}

	// Adds a given resource location based on the Resource Management in place
	// It looks from within available achives to see if the given location actually exists
	// eventually as a sub path of a previous one, resolves the path in that case and adds the new location to the given group.
	// The ResourceGroupManager pointer to the singleton may be passed as argument, avoiding to call it more than once.
	static void addResourceLocation(const String& name, const String& locType, 
							 const String& resGroup = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
							 bool recursive = false,
							 ResourceGroupManager *rsm = ResourceGroupManager::getSingletonPtr())
	{
		if (rsm->resourceLocationExists(name, resGroup))
		{
			LogManager::getSingleton().logMessage("NOT ADDED: Resource Location " + name + " already exists in " + resGroup);
			return;
		}

		// Not Optimal
		// should only walk through group->locationlist ?
		ArchiveManager* am = ArchiveManager::getSingletonPtr();
		ArchiveManager::ArchiveMapIterator ai = am->getArchiveIterator();

		bool added =false;
		while (ai.hasMoreElements())
		{
			Archive* ii = ai.getNext();
			if (ii->exists(name))
			{
				// Not Optimal
				// Should centralise common path functions
				String rebuildPath = FileSystemUtil::concatenate_path(ii->getName(),name);
				if (!rsm->resourceLocationExists(rebuildPath, resGroup))
				{
					rsm->addResourceLocation(rebuildPath,locType,resGroup,recursive);
					if (!added)
						added = true;
				}
			}
		}
		if (!added) // à voir si on y vient jamais !!
			if (FileSystemUtil::fileExists(name.c_str())) // Attention confus '!'
				rsm->addResourceLocation(name,locType,resGroup,recursive);
	}

	// Loading of specific "Resource" files, similar to the ones given as example
	// adds every MultiMap section (as 'FileSystem','Path' pair) as ResourceLocation
	// it also recursively applies to any other "Resource" file founded meanwhile
	// it keeps track of already added Resource and "Resopurce" file
	// in order not to treat them more than once, using the 'currentFileInfoList'
	static void recursiveAddResourceLocation(String resourceFileName, String resourceGroupName, FileInfoListPtr currentFileInfoList, ResourceGroupManager* rsm = ResourceGroupManager::getSingletonPtr())
	{
		bool lookForExisting = !currentFileInfoList.isNull();
		FileInfoListPtr rdl = rsm->findResourceFileInfo(resourceGroupName,resourceFileName+".cfg");

		for (FileInfoList::iterator fi = rdl->begin();fi!=rdl->end();++fi)
		{
			// TODO find a better way to compare vectors
			if (lookForExisting)
			{
			bool treated = false;
//			std::find(treatedFileInfoList->begin(),treatedFileInfoList->end(),&(*fi));
				for (FileInfoList::iterator tfi=currentFileInfoList->begin();tfi!=currentFileInfoList->end();++tfi)
				{
					if ((*tfi).archive==(*fi).archive)
					{
						treated = true;
						break;
					}
				}
				if (treated)
					continue;
			}
			// end TODO find a better way to compare vectors

			ConfigFile cf;
			cf.load((*fi).archive->open((*fi).filename));

			ConfigFile::SectionIterator seci = cf.getSectionIterator();
			currentFileInfoList->insert(currentFileInfoList->end(),*fi);

			while (seci.hasMoreElements())
			{
				String secName = seci.peekNextKey();
				ConfigFile::SettingsMultiMap *settings = seci.getNext();
				if (!secName.empty())
				{
					for (ConfigFile::SettingsMultiMap::iterator i = settings->begin(); i != settings->end(); ++i)
						addResourceLocation(i->second, i->first, secName);
					if (resourceGroupExists(secName,rsm))
						recursiveAddResourceLocation(resourceFileName,secName,secName==resourceGroupName?rdl:currentFileInfoList,rsm);
				}
			}
		}		
	}

	// Loading of all specific "Resource" files, similar to the ones given as example
	// Loads all that can be found for all groups
	// TODO: be more dynamic, that is look into newly added groups if any
	// the 'for' loop is not relevant in that case
	static void addAllResourceLocation(String resourceFileName)
	{
		ResourceGroupManager* rsm = ResourceGroupManager::getSingletonPtr();

		StringVector rg = rsm->getResourceGroups();
		FileInfoListPtr treatedFileInfoList(OGRE_NEW_T(FileInfoList, MEMCATEGORY_GENERAL)(), SPFM_DELETE_T);
		for (StringVector::iterator i = rg.begin(); i!=rg.end();++i)
		{
			String grp = i->c_str();
			recursiveAddResourceLocation(resourceFileName,grp,treatedFileInfoList,rsm);
//			StringVector rg2 = rsm->getResourceGroups();
		}
	}
};
}
#endif //__ExtendedResourceManager__H_