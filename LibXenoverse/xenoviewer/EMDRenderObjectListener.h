#ifndef EMD_RENDER_OBJECT_LISTENER_H_INCLUDED
#define EMD_RENDER_OBJECT_LISTENER_H_INCLUDED



class EMDRenderObject {
public:
	Ogre::TexturePtr s0_ptr;
	Ogre::TexturePtr s1_ptr;
	Ogre::TexturePtr s2_ptr;
	Ogre::TexturePtr s3_ptr;

	EMDRenderObject(Ogre::TexturePtr s0, Ogre::TexturePtr s1, Ogre::TexturePtr s2, Ogre::TexturePtr s3);
};

class EMDRenderObjectAssignVisitor : public Ogre::Renderable::Visitor {
protected:
	EMDRenderObject *parameter;
public:
	EMDRenderObjectAssignVisitor(EMDRenderObject *p)
	{
		parameter = p;
	}

	void visit(Ogre::Renderable *rend, Ogre::ushort lodIndex, bool isDebug, Ogre::Any *pAny = 0);
};

#endif