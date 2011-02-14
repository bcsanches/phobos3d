#include "PH_ModelRendererComponent.h"

#include <OgreEntity.h>

#include <PH_Dictionary.h>
#include <PH_Render.h>

#include "PH_DictionaryUtils.h"
#include "PH_EntityComponentFactory.h"
#include "PH_EntityKeys.h"

namespace Phobos
{
	PH_FULL_ENTITY_COMPONENT_CREATOR("ModelRendererComponent", ModelRendererComponent_c);	
		
	ModelRendererComponent_c::ModelRendererComponent_c(const String_c &name):
		EntityComponent_c(name),
		pclSceneNode(NULL),
		pclMeshEntity(NULL)
	{		
		//empty
	}

	ModelRendererComponent_c::~ModelRendererComponent_c()
	{
		RenderPtr_t render = Render_c::GetInstance();

		render->DestroyEntity(pclMeshEntity);
		render->DestroySceneNode(pclSceneNode);
	}

	void ModelRendererComponent_c::OnLoad(const Dictionary_c &dictionary)
	{
		const String_c &meshName = dictionary.GetValue(PH_ENTITY_KEY_MESH_FILE);

		RenderPtr_t render = Render_c::GetInstance();

		pclSceneNode = render->CreateSceneNode();
		pclMeshEntity = render->CreateEntity(meshName);

		pclSceneNode->attachObject(pclMeshEntity);

		Float_t values[4];
		dictionary.Get4Float(values, "orientation");

		pclSceneNode->setOrientation(Ogre::Quaternion(values));

		pclSceneNode->setPosition(DictionaryGetVector3(dictionary, PH_ENTITY_KEY_POSITION));
		pclSceneNode->setOrientation(DictionaryGetQuaternion(dictionary, PH_ENTITY_KEY_ORIENTATION));
	}
}