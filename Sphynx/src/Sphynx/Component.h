#pragma once
#ifndef Sphynx_Component
#define Sphynx_Component
#include "Object.h"
namespace Sphynx {
	class GameObject;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
	class Component : public Object
	{
	private:
		virtual Component* CreateSelf() = 0;
		//Called When Component is Created and before being attached to the GameObject
		virtual void OnComponentCreate(GameObject* Parent) = 0;
		virtual void OnComponentAttach(GameObject* Parent) = 0;
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
	class Transform;
	//Native Components
	class Component : public Object
	{
	private:
		//Called When Component is Created and attached to the GameObject.
		virtual void OnComponentAttach(GameObject* parent) = 0;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
		virtual void OnComponentDetach() = 0;
		GameObject* Parent;
		Transform* _transform;
		size_t InstanceID = (size_t)this;
	public:
		virtual ~Component() = 0;
		virtual void Update() = 0;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
		GameObject* GetGameObject() { return Parent; };
		Transform* GetTransform() {
			return _transform; 
		};
		friend Core::Internal::ComponentFactory;
		friend GameObject;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
	};
}
#else
#endif