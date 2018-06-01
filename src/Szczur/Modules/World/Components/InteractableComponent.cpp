#include "InteractableComponent.hpp"

#include "../Entity.hpp"
#include "../ScenesManager.hpp"

#include <Szczur/Modules/Script/Script.hpp>
#include <Szczur/Modules/Input/Input.hpp>

namespace rat {
	InteractableComponent::InteractableComponent(Entity* parent) :
		Component { parent, fnv1a_64("InteractableComponent"), "InteractableComponent"}, 
		_input(detail::globalPtr<Input>->getManager()) {
	}

	void InteractableComponent::callback() {
		if(_interactionCallback.valid())
			_interactionCallback(this);
	}

	void InteractableComponent::setDistance(float distance) {
		_distance = distance;
	}
	float InteractableComponent::getDistance() const {
		return _distance;
	}

	void InteractableComponent::setHeight(float height) {
		_height = height;
	}
	float InteractableComponent::getHeight() const {
		return _height;
	}

	bool InteractableComponent::checkForInteraction(const glm::vec3& position) const {
		auto* entity = getEntity();
		auto delta = position - entity->getPosition();
		if(delta.x*delta.x + delta.z*delta.z <= _distance*_distance)
			return true;
		return false;
	}

	std::unique_ptr<Component> InteractableComponent::copy(Entity* newParent) const {
		auto ptr = std::make_unique<InteractableComponent>(*this);

		ptr->setEntity(newParent);

		return ptr;
	}

	void InteractableComponent::loadFromConfig(const Json& config) {
		Component::loadFromConfig(config);
		_distance = config["distance"];
	}

	void InteractableComponent::saveToConfig(Json& config) const {
		Component::saveToConfig(config);
		config["distance"] = _distance;
	}

	void InteractableComponent::initScript(Script& script) {
		auto object = script.newClass<InteractableComponent>("InteractableComponent", "World");
		object.set("onInteraction", &InteractableComponent::_interactionCallback);
		object.set("getEntity", sol::resolve<Entity*()>(&Component::getEntity));
		object.init();
	}

	void InteractableComponent::renderHeader(ScenesManager& scenes, Entity* object) {
		if(ImGui::CollapsingHeader("Interactable")) {

			// Set radius for circle interaction
			float distance = getDistance();
			ImGui::DragFloat("Radius", &distance);
			setDistance(distance);

			// Set height of circle interaction 
			float height = getHeight();
			ImGui::DragFloat("Height", &height);
			setHeight(height);
		}
	}

	void InteractableComponent::update(ScenesManager& scenes, float deltaTime) {
		auto* player = getEntity()->getScene()->getPlayer();
		if(player == nullptr) return;

		if(_input.isReleased(Keyboard::LShift)) {
			if(checkForInteraction(player->getPosition())) {
				callback();
			}
		}
	}
}
