#include "Entity.hpp"

#include <algorithm>

#include "ScenesManager.hpp"

namespace rat
{

Entity::Entity(Scene* parent, const std::string& group)
	: _id { getUniqueID<Entity>() }
	, _group { group }
	, _name { "unnamed_" + std::to_string(_id) }
	, _parent { parent }
{

}

Entity::Entity(const Entity& rhs)
	: sf3d::Transformable { static_cast<const sf3d::Transformable&>(rhs) }
	, _id { getUniqueID<Entity>() }
	, _group { rhs.getGroup() }
	, _name { rhs.getName() + "_copy_" + std::to_string(_id) }
	, _parent { rhs._parent }
{
	for (const auto& ptr : rhs._holder)
	{
		_holder.emplace_back(ptr->copy(this));
	}
	_scriptData = rhs._scriptData;
}

Entity& Entity::operator = (const Entity& rhs)
{
	if (this != &rhs)
	{
		static_cast<sf3d::Transformable&>(*this) = static_cast<const sf3d::Transformable&>(rhs);
		_id = getUniqueID<Entity>();
		_group = rhs.getGroup();
		_name = rhs.getName() + "_copy_" + std::to_string(_id);
		_parent = rhs._parent;

		removeAllComponents();

		for (const auto& ptr : rhs._holder)
		{
			_holder.emplace_back(ptr->copy(this));
		}
	}

	return *this;
}

void Entity::update(ScenesManager& scenes, float deltaTime)
{
	if (_isActive) {
		if(auto* comp = getComponentAs<InteractableComponent>()) comp->update(scenes, deltaTime);
		if(auto* comp = getComponentAs<TraceComponent>()) comp->update(scenes, deltaTime);
		if(auto* comp = getComponentAs<CameraComponent>()) comp->update(scenes, deltaTime);
		if(auto* comp = getComponentAs<TriggerComponent>()) comp->update(scenes, deltaTime);
		if(auto* comp = getComponentAs<ArmatureComponent>()) comp->update(scenes, deltaTime);
		if(auto* comp = getComponentAs<AnimatedSpriteComponent>()) comp->update(scenes, deltaTime);
		if(auto* comp = getComponentAs<SpriteComponent>()) comp->update(scenes, deltaTime);

		if(auto* comp = getComponentAs<ScriptableComponent>()) {  
			if(getScene()->getScenes()->isGameRunning()) { 
				comp->update(scenes, deltaTime); 
			} 
		} 
	}
}

void Entity::draw(sf3d::RenderTarget& target, sf3d::RenderStates states) const
{
	if (_isVisible) {
		if (auto* ptr = getFeature<sf3d::Drawable>()) {
			target.draw(*ptr, states);
		}
	
		if (auto* comp = getComponentAs<TraceComponent>()) {
			comp->render(target);
		}
	}
}

void Entity::destroy()
{
	_exists = false;
}

void Entity::setVisible(bool visible)
{
	_isVisible = visible;
}

bool Entity::isVisible() const
{
	return _isVisible;
}

void Entity::setActive(bool active)
{
	_isActive = active;
}

bool Entity::isActive()
{
	return _isActive;
}

size_t Entity::getID() const
{
	return _id;
}

void Entity::setGroup(const std::string& group)
{
	_group = group;
}

const std::string& Entity::getGroup() const
{
	return _group;
}

void Entity::setName(const std::string& name)
{
	_name = name;
}

const std::string& Entity::getName() const
{
	return _name;
}

Scene* Entity::getScene()
{
	return _parent;
}

const Scene* Entity::getScene() const
{
	return _parent;
}

Component* Entity::addComponent(std::unique_ptr<Component> component)
{
	if (auto ptr = getComponent(component->getComponentID()); ptr != nullptr)
	{
		LOG_WARNING("Entity ( ", getID(), " ) ", std::quoted(getName()), " already has ", ComponentTraits::getNameFromIdentifier(ptr->getComponentID()), " component, existing one returned");

		return ptr;
	}

	return _holder.emplace_back(std::move(component)).get();
}

Component* Entity::addComponent(Hash64_t componentID)
{
	return addComponent(ComponentTraits::createFromComponentID(this, componentID));
}

bool Entity::removeComponent(Hash64_t componentID)
{
	if (auto it = _findByComponentID(componentID); it != _holder.end())
	{
		_holder.erase(it);

		return true;
	}

	return false;
}

void Entity::removeAllComponents()
{
	_holder.clear();
}

Component* Entity::getComponent(Hash64_t componentID) const
{
	if (auto it = _findByComponentID(componentID); it != _holder.end())
	{
		return it->get();
	}

	return nullptr;
}

bool Entity::hasComponent(Hash64_t componentID) const
{
	return _findByComponentID(componentID) != _holder.end();
}

Entity::ComponentsHolder_t& Entity::getComponents()
{
	return _holder;
}

const Entity::ComponentsHolder_t& Entity::getComponents() const
{
	return _holder;
}

void Entity::loadFromConfig(Json& config, bool withNewID)
{
	_id = withNewID ? getUniqueID<Entity>() : config["id"].get<size_t>();
	_name = config["name"].get<std::string>();

	setPosition({
		config["position"]["x"].get<float>(),
		config["position"]["y"].get<float>(),
		config["position"]["z"].get<float>()
	});
	setRotation({
		config["rotation"]["x"].get<float>(),
		config["rotation"]["y"].get<float>(),
		config["rotation"]["z"].get<float>()
	});
	setOrigin({
		config["origin"]["x"].get<float>(),
		-config["origin"]["y"].get<float>(),
		config["origin"]["z"].get<float>()
	});
	setScale({
		config["scale"]["x"].get<float>(),
		config["scale"]["y"].get<float>(),
		config["scale"]["z"].get<float>()
	});

	Json& components = config["components"];

	bool base = false;
	for (Json& component : components)
	{
		if (component["name"] == "BaseComponent") base = true;
		addComponent(static_cast<Hash64_t>(component["id"]))->loadFromConfig(component);
	}

	if (base == false) {         
        addComponent<BaseComponent>(); 
    } 

	trySettingInitialUniqueID<Entity>(_id);
}

void Entity::saveToConfig(Json& config) const
{
	config["id"] = getID();
	config["name"] = getName();
	config["components"] = Json::array();

	config["position"]["x"] = getPosition().x;
	config["position"]["y"] = getPosition().y;
	config["position"]["z"] = getPosition().z;

	config["rotation"]["x"] = getRotation().x;
	config["rotation"]["y"] = getRotation().y;
	config["rotation"]["z"] = getRotation().z;

	config["origin"]["x"] = getOrigin().x;
	config["origin"]["y"] = getOrigin().y;
	config["origin"]["z"] = getOrigin().z;

	config["scale"]["x"] = getScale().x;
	config["scale"]["y"] = getScale().y;
	config["scale"]["z"] = getScale().z;

	for (const auto& component : _holder)
	{
		config["components"].push_back(Json::object());
		Json& comp = config["components"].back();

		component->saveToConfig(comp);
	}
}

void Entity::initScript(Script& script)
{
	auto object = script.newClass<Entity>("Entity", "World");
	ComponentTraits::initScript(object, script);
	object.set("getName", &Entity::getName);
	object.set("setName", &Entity::setName);
	object.set(sol::meta_function::index, &Entity::_getScriptDataObject);
	object.set(sol::meta_function::new_index, &Entity::_setScriptDataObject);
	object.set("getScene", sol::resolve<Scene*()>(&Entity::getScene)); 
	object.set("destroy", &Entity::destroy);

	object.init();


	// object.set("getScriptable", &Entity::getComponentAs<ScriptableComponent>);
	// object.set("getInteractable", &Entity::getComponentAs<InteractableComponent>);
	// object.set("getArmature", &Entity::getComponentAs<ArmatureComponent>);
	// object.set("getTrace", &Entity::getComponentAs<TraceComponent>);
}

void Entity::updateIDs()
{
	_id = getUniqueID<Entity>();

	for (const auto& component : _holder)
	{
		component->updateIDs();
	}
}

typename Entity::ComponentsHolder_t::iterator Entity::_findByComponentID(size_t id)
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		return arg->getComponentID() == id;
	});
}

typename Entity::ComponentsHolder_t::const_iterator Entity::_findByComponentID(size_t id) const
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		return arg->getComponentID() == id;
	});
}

typename Entity::ComponentsHolder_t::iterator Entity::_findByFeature(Component::Feature_e feature)
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		return arg->getFeatures() & feature;
	});
}

typename Entity::ComponentsHolder_t::const_iterator Entity::_findByFeature(Component::Feature_e feature) const
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		return arg->getFeatures() & feature;
	});
}

void Entity::_setScriptDataObject(std::string key, sol::stack_object value) {
	auto obj = _scriptData.find(key);

	if (obj == _scriptData.cend()) {
		_scriptData.insert(obj, { std::move(key), std::move(value) });
	}
	else {
		obj->second = sol::object(std::move(value));
	}
}

sol::object Entity::_getScriptDataObject(const std::string& key) {
	auto obj = _scriptData.find(key);
	return (obj == _scriptData.cend() ? sol::lua_nil : obj->second);
}

}
