#pragma once
//#include "Core/Graphics/Window.h"
#include "Core/Module.h"
#include "Vector.h"
#define BIT(i) (1 << i)
namespace Sphynx::Core::Graphics {
	class Mesh;
	class Material;
	class Shader;
	//Information For the Rendering Engine
	typedef struct RenderObject {
		Mesh* mesh;
		Material* mat;
		Vec3 Position;
		Vec4 Rotation;
		RenderObject(Mesh* _mesh, Material* _mat, Vec3 Pos, Vec4 Rot) : mesh(_mesh), mat(_mat), Position(Pos), Rotation(Rot) {};
	}RenderObject;
	//Base Class Of Renderers.
	class IRenderer
	{
	public:
		// Inherited via Module
		virtual void Start(IWindow* app) = 0;
		virtual void Render() = 0;
		virtual void Clear() = 0;
		//Send Data,Shaders,Indexes,Textures (Make A Object that encapsulates them all).
		//They are stored until deleted
		RenderObject& Submit(Mesh* mesh, Material* mat, Vec3 Position, Vec4 Rotation) {
			auto& RO = RenderObject(mesh, mat, Position, Rotation);	
			OnSubmit(RO);
			return RO;
		}
	private:
		virtual void OnSubmit(RenderObject rend) = 0;

	};
}