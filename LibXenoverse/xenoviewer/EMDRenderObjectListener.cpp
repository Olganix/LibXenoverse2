#include "EMDRenderObjectListener.h"



EMDRenderObject::EMDRenderObject(Ogre::TexturePtr s0, Ogre::TexturePtr s1, Ogre::TexturePtr s2, Ogre::TexturePtr s3)
{
	s0_ptr = s0;
	s1_ptr = s1;
	s2_ptr = s2;
	s3_ptr = s3;
}


void EMDRenderObjectAssignVisitor::visit(Ogre::Renderable *rend, Ogre::ushort lodIndex, bool isDebug, Ogre::Any *pAny) 
{
	rend->getUserObjectBindings().setUserAny(Ogre::Any(parameter));
}