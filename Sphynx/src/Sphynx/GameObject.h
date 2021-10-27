#pragma once
#include "Object.h"
#include "Component.h"
#include "Core/Factories/ComponentFactory.h"
#include "Pointer.h"
#include "Core/Transform.h"

namespace Sphynx {
<<<<<<< Updated upstream
=======
	class Transform;
	enum class Primitives {
		Sphere, Capsule, Cube, Plane, Triangle
	};
	//Native GameObject
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
	class GameObject :
		public Object
	{
	private:
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
		std::list<Component*> Components;
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
		std::list<Component*> Components = std::list<Component*>();
		size_t InstanceID = 0;
		bool IsAlive = false;
	protected:
		Transform* transform = new Transform();
	public:
		static const GameObject PlaceHolder;
		GameObject();
		GameObject(const GameObject& obj)noexcept;
		GameObject(GameObject&& obj)noexcept;
		GameObject& operator=(GameObject&& obj)noexcept;
		GameObject& operator=(const GameObject& obj)noexcept;
		bool IsActive() { return IsAlive; };
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
		void Update() {
			for (auto comp : Components) {
				comp->Update();
			}
		}
		bool IsAlive;
	public:
		//No Double
		template<class component>
		void AddComponent() {
			//C++17
			if (std::is_base_of_v<Component, component>) {
				if (Core::Internal::ComponentFactory::ComponentHelper::IsComponentInGameObject<component>(this)) {
					//No Parameters allowed. They will be provided to the factory.
					//Components Should not have consturctors and they will be ignored by the factory.
					Components.push_back(Core::Internal::ComponentFactory::CreateComponent<component>(this));
				}
			}
		}
		template<class component>
		component* GetComponent() {
			if (std::is_base_of_v<Component, component>)
			{
				for (auto comp : Components) {
					if (Core::Internal::ComponentFactory::ComponentHelper::CompareTypeToComponant_typeid<component>(comp)) {
						return static_cast<component*>(comp);
					}
				}
			}
			return nullptr;
		}
<<<<<<< Updated upstream
=======
		template<class component>
		void RemoveComponent() {
			if (!std::is_base_of_v<Component, component>) return;
			else {
				for (auto comp : Components) {
					if (Core::Internal::ComponentFactory::ComponentHelper::CompareTypeToComponant_typeid<component>(comp)) {
						Components.remove(comp);
						return;
					}
				}
			}
		}
		static GameObject CreatePrimitive(Primitives primitive);
		Transform* GetTransform() { 
			return transform;
		};
		uintptr_t GetID() { return InstanceID; };
		bool operator==(const GameObject& other) {
			if (IsAlive == false && other.IsAlive == IsAlive)return true;
			return InstanceID == other.InstanceID && Components == other.Components;
		}
		bool operator!=(const GameObject& other) {
			return !(*this == other);
		}
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
	};
}
