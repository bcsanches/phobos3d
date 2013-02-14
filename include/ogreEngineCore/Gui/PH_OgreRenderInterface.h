/*
Phobos 3d
September 2012
Copyright (c) 2005-2012 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Based on sample code provided by libRocket, original author: Peter Curry
*/

#ifndef PH_GUI_RENDER_INTERFACE_OGRE_H
#define PH_GUI_RENDER_INTERFACE_OGRE_H

#include <Rocket/Core/RenderInterface.h>

#include <OgrePrerequisites.h>
#include <OgreBlendMode.h>

namespace Phobos
{
	namespace Gui
	{

		class RenderInterfaceOgre : public Rocket::Core::RenderInterface
		{
			public:
				RenderInterfaceOgre(unsigned int window_width, unsigned int window_height);
				virtual ~RenderInterfaceOgre();

				/// Called by Rocket when it wants to render geometry that it does not wish to optimise.
				virtual void RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation);

				/// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
				virtual Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture);

				/// Called by Rocket when it wants to render application-compiled geometry.
				virtual void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation);
				/// Called by Rocket when it wants to release application-compiled geometry.
				virtual void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry);

				/// Called by Rocket when it wants to enable or disable scissoring to clip content.
				virtual void EnableScissorRegion(bool enable);
				/// Called by Rocket when it wants to change the scissor region.
				virtual void SetScissorRegion(int x, int y, int width, int height);

				/// Called by Rocket when a texture is required by the library.
				virtual bool LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source);
				/// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
				virtual bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions);
				/// Called by Rocket when a loaded texture is no longer required.
				virtual void ReleaseTexture(Rocket::Core::TextureHandle texture);

				/// Returns the native horizontal texel offset for the renderer.
				float GetHorizontalTexelOffset();
				/// Returns the native vertical texel offset for the renderer.
				float GetVerticalTexelOffset();

			private:
				Ogre::RenderSystem* render_system;

				Ogre::LayerBlendModeEx colour_blend_mode;
				Ogre::LayerBlendModeEx alpha_blend_mode;

				bool scissor_enable;
				int scissor_left;
				int scissor_top;
				int scissor_right;
				int scissor_bottom;
		};
	}
}

#endif