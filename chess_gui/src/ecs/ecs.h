#pragma once

#include <Exception.h>
#include <queue>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <string>
#include <memory>

// Potential optimisation using int types instead of type_index.
// The hashing of int types is twice faster than type_index.
enum class ComponentType : uint32_t
{
	MESH,
	TRANSFORM,
	CHILDREN,
	MATERIAL,
	TEXTURE,
	RENDERERSTUFF
};

using ComponentTypeMap = std::unordered_map<std::type_index, ComponentType>;

struct TypeNotFoundException : public CException
{
	TypeNotFoundException(uint32_t line, const char* file)
		:
			CException(line, file, "Type Not Found", "Cannot find the type you are refercing")
	{}
};

class ComponentPtr
{
public:
    struct BaseImpl
    {
        virtual void Create() { }
        virtual void* GetPointer() { std::cout << "Creating a base pointer " << std::endl; return nullptr;}
        virtual void CheckingToSeeBase(){std::cout << "If base \n";}
        virtual BaseImpl* Clone() { return new BaseImpl(*this);} 
        virtual ~BaseImpl() {} 
    };
    template<typename T>
    struct Impl : public BaseImpl
    {
        T* data;
        Impl()
        {
            data = nullptr;
        }
        Impl(const Impl& imp)
        {
			data = nullptr;
        }
        void CheckingToSeeBase() override {std::cout << "If derived \n";}
        BaseImpl* Clone() override
        {
            return static_cast<BaseImpl*>(new Impl<T>(*this));
        }
        void Create() override
        {
            data = new T();
        }
        void* GetPointer() override
        {
			if(data == nullptr) std::cout << "Data is NULL " << std::endl;
            return data;
        }
        ~Impl()
        {
            if(data != nullptr) delete data;
        }
    };
    BaseImpl* base;
public:
    ComponentPtr()
    {
        base = nullptr;
    }

    ComponentPtr(BaseImpl* baseImpl) 
    {
        base = baseImpl->Clone();
    }
    ComponentPtr(const ComponentPtr& comptr)
    {
		if(comptr.base != nullptr)
        base = comptr.base->Clone();
		else base = nullptr;
    }
    ~ComponentPtr() 
    {
		if(base != nullptr)
        delete base;
    }

};

class Scene
{
public:
    class EntityManager
    {
        Scene* owner;
    public:
        EntityManager(Scene* scene);
        uint32_t CreateEntity() const;
        void DestroyEntity(uint32_t& entity);
    };

    struct IComponentArray
    {
		std::unordered_map<ComponentType, ComponentPtr> components;
        //std::unordered_map<std::type_index, ComponentPtr> components;
		std::vector<ComponentType> componentTypes;
        IComponentArray() = default;
        IComponentArray(std::unordered_map<std::type_index, ComponentPtr>& componentTypes, std::unordered_map<std::type_index, ComponentType>& componentTypeMap);
        template<ComponentType T>
        void* get()
        {
            auto i = components.find(T);
            if(i == components.end()) throw TypeNotFoundException(__LINE__, __FILE__);
			void* basePointer = i->second.base->GetPointer();
			if(basePointer == nullptr) std::cout << "BasePointer is null" << std::endl;
			return basePointer;
        }
    };    

    struct ComponentManager
    {
		Scene* scene;
		ComponentManager(Scene* scene);
        std::unordered_map<std::type_index, ComponentPtr> componentTypes;
        IComponentArray* CreateComponentArray();
    };
	using Entities  = std::unordered_map<uint32_t,  std::unique_ptr<IComponentArray>>;
public:
	ComponentTypeMap componentTypeMap;
    Entities entities;
    EntityManager* entityManager;
    ComponentManager* componentManager;
public:
    Scene();
    uint32_t Push();
    IComponentArray* GetEntity(uint32_t entity);
    template <typename T>
    void RegisterComponent()
    {
        componentManager->componentTypes.insert(std::make_pair
                	(std::type_index(typeid(T)), 
                	 ComponentPtr(new ComponentPtr::Impl<T>())
                	)
                );
    }
    template <typename T>
    void UnRegisterComponent()
    {
        componentManager->componentTypes.erase(std::type_index(typeid(T)));
    }

    void LoadScene(std::string filePath);
    void SaveScene(std::string filePath);
};

struct Children
{
	std::unordered_map<uint32_t, Scene::IComponentArray>* children;
};

struct System
{
    virtual void LoadScene(Scene* scene) = 0;
    virtual void update(float deltaTime) = 0;
};
