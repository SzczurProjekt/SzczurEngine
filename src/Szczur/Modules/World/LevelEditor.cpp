//#ifdef EDITOR

#include "LevelEditor.hpp"

#include "Szczur/Utility/SFML3D/RenderTarget.hpp"
#include "Szczur/Utility/SFML3D/RectangleShape.hpp"

namespace rat {
    LevelEditor::LevelEditor(
        CollectingHolder_t &collectingHolder,
        SpriteDisplayDataHolder_t &spriteDisplayDataHolder,
        ArmatureDisplayDataHolder_t &armatureDisplayDataHolder):
    _collectingHolder(collectingHolder),
    _spriteDisplayDataHolder(spriteDisplayDataHolder),
    _armatureDisplayDataHolder(armatureDisplayDataHolder) {
        for (auto& holder : _collectingHolder)
		{
			holder.second.reserve(100);
		}
		_spriteDisplayDataHolder.reserve(100);
		_armatureDisplayDataHolder.reserve(100);
        _camera = addEntity("single", "Camera");
		_camera->setLock(true);
		_camera->setPosition({0.f, 0.f, 1000.f});
    }

    void LevelEditor::render(sf3d::RenderTarget& target) {
        _renderBar();
        if(_ifRenderObjectsList)
            _renderObjectsList();
        if(_anySelected)
            _renderFocusedObjectsParams();
        if(_ifRenderModulesManager)
            _renderModulesManager();
        if(_ifRenderDisplayDataManager)
            _renderDisplayDataManager();
        if(_ifRenderArmatureDisplayManager)
            _renderArmatureDisplayManager();

        sf3d::RectangleShape rect({100.f, 100.f});
        rect.setColor({1.f, 1.f, 0.f, 0.2f});
        rect.setOrigin({50.f, 50.f, 0.f});

        glDisable(GL_DEPTH_TEST);
        for (auto& group : _collectingHolder) {
            for(auto object = group.second.begin(); object != group.second.end(); ++object) {
                if(object->isLocked() == false) {
                    if(_focusedObject == object && _anySelected)
                        rect.setColor({1.f, 0.f, 1.f, 0.2f});
                    else
                        rect.setColor({1.f, 1.f, 0.f, 0.2f});
                    rect.setPosition(object->getPosition());
                    target.draw(rect);
                }
            }
        }
        glEnable(GL_DEPTH_TEST);
    }

    void LevelEditor::update(InputManager& input, Camera& camera) {
		for (auto& holder : _collectingHolder)
		{
			for (auto& entity : holder.second)
			{
				//std::cout << entity.getName() << '\n';
			}
		}

		for (auto& holder : _collectingHolder) {
			for (auto& entity : holder.second) {
				if (auto ptr = entity.getComponentAs<InputControllerComponent>(); ptr != nullptr) {
					entity.move(ptr->update(input));
				}
			}
		}


		
		if(input.isKept(Keyboard::W))
			_freeCamera.move({
				_freeCameraVelocity * glm::sin(glm::radians(_freeCamera.getRotation().y)),
				0.f,
				-_freeCameraVelocity * glm::cos(glm::radians(_freeCamera.getRotation().y))
			});
		if(input.isKept(Keyboard::S))
			_freeCamera.move({
				-_freeCameraVelocity * glm::sin(glm::radians(_freeCamera.getRotation().y)),
				0.f,
				_freeCameraVelocity * glm::cos(glm::radians(_freeCamera.getRotation().y))
			});
		if(input.isKept(Keyboard::D)) {
			_freeCamera.move(glm::vec3{
				_freeCameraVelocity * glm::cos(glm::radians(_freeCamera.getRotation().y)),
				0.f,
				_freeCameraVelocity * glm::sin(glm::radians(_freeCamera.getRotation().y))
			});
		}
		if(input.isKept(Keyboard::A)) {
			_freeCamera.move(glm::vec3{
				-_freeCameraVelocity * glm::cos(glm::radians(_freeCamera.getRotation().y)),
				0.f,
				-_freeCameraVelocity * glm::sin(glm::radians(_freeCamera.getRotation().y))
			});
		}
		if(input.isKept(Keyboard::Space))
			_freeCamera.move({0.f, _freeCameraVelocity, 0.f});
		if(input.isKept(Keyboard::LControl))
			_freeCamera.move({0.f, -_freeCameraVelocity, 0.f});
		if(_rotatingCamera) {
			auto mouse = input.getMousePosition();
			_freeCamera.rotate({
				(mouse.y - _previousMouse.y)/10.f,
				(mouse.x - _previousMouse.x)/10.f, 
				0.f
			});
			_previousMouse = mouse;
		}
		if(input.isPressed(Mouse::Right)) {
			_rotatingCamera = true;
			_previousMouse = input.getMousePosition();
		}
		if(input.isReleased(Mouse::Right)) {
			_rotatingCamera = false;
		}


		sf3d::View view;
		if(_focusedObject == _camera && _anySelected) {
			view.setRotation(_camera->getRotation());
			view.setCenter(_camera->getPosition());
		}
		else {
			view.setRotation(_freeCamera.getRotation());
			view.setCenter(_freeCamera.getPosition());
		}
		camera.setView(view);
    }



    void LevelEditor::_renderBar() {
		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::BeginMenu("Files")) {
				if(ImGui::MenuItem("Load")) {
					std::cout << "LOAD\n";
				}
				if(ImGui::MenuItem("Save")) {
					std::cout << "Save\n";
				}
				if(ImGui::MenuItem("Save as")) {
					std::cout << "Save as\n";
				}
				if(ImGui::MenuItem("Exit")) {
					std::cout << "Exit\n";
				}
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Tools")) {
				ImGui::MenuItem("Objects List", nullptr, &_ifRenderObjectsList);
				ImGui::MenuItem("Display Data Manager", nullptr, &_ifRenderDisplayDataManager);
				ImGui::MenuItem("Armature Data Manager", nullptr, &_ifRenderArmatureDisplayManager);
				ImGui::MenuItem("Modules Manager", nullptr, &_ifRenderModulesManager);
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
	}
    void LevelEditor::_renderDisplayDataManager() {
		static char enteredText[255];
		if(ImGui::Begin("Display Data Manager", &_ifRenderDisplayDataManager)) {
			if(ImGui::InputText("", enteredText, 255)) {
			}
			ImGui::SameLine();
			if(ImGui::Button("Add")) {
				try{
					_spriteDisplayDataHolder.emplace_back(enteredText);
				}
				catch(const std::exception& exc) {
				}
				for(int i = 0; i<255; ++i)
					enteredText[i] = '\0';
			}
			ImGui::Separator();
			if(ImGui::BeginChild("Datas")) {
				for(auto it = _spriteDisplayDataHolder.begin(); it!=_spriteDisplayDataHolder.end(); ++it) {
					if(ImGui::SmallButton("-")) {
						//spriteDisplayData.erase(it);
						//--it;
					}
					ImGui::SameLine();
					ImGui::Text(it->getName().c_str());
				}
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}
    void LevelEditor::_renderArmatureDisplayManager() {
		static char enteredText[255];
		if(ImGui::Begin("Armature Data Manager", &_ifRenderArmatureDisplayManager)) {
			if(ImGui::InputText("", enteredText, 255)) {
			}
			ImGui::SameLine();
			if(ImGui::Button("Add")) {
				try{
					_armatureDisplayDataHolder.emplace_back(enteredText);
				}
				catch(const std::exception& exc) {
				}
				for(int i = 0; i<255; ++i)
					enteredText[i] = '\0';
			}
			ImGui::Separator();
			if(ImGui::BeginChild("Datas")) {
				for(auto it = _armatureDisplayDataHolder.begin(); it!=_armatureDisplayDataHolder.end(); ++it) {
					if(ImGui::SmallButton("-")) {
						//_armatureDisplayDataHolder.erase(it);
						//--it;
						//continue;
					}
					ImGui::SameLine();
					ImGui::Text(it->getName().c_str());
				}
			}
			ImGui::EndChild();
		}
		ImGui::End();
	}
    void LevelEditor::_renderFocusedObjectsParams() {
		if(ImGui::Begin("Object Parameters", &_anySelected)) {
			if(ImGui::CollapsingHeader("Base")) {
				glm::vec3 position = _focusedObject->getPosition();
				glm::vec3 origin = _focusedObject->getOrigin();
				origin.y = -origin.y;
				glm::vec3 rotation = _focusedObject->getRotation();
				glm::vec3 scale = _focusedObject->getScale();
				char name[255];
				std::strcpy(&name[0], _focusedObject->getName().c_str());
				ImGui::InputText("", name, 255);
				_focusedObject->setName(name);
				ImGui::DragFloat3("Position", reinterpret_cast<float*>(&position));
				ImGui::DragFloat3("Origin", reinterpret_cast<float*>(&origin));
				ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(&rotation));
				ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&scale), 0.01f);
				//ImGui::Checkbox("Locked", &(_focusedObject->locked));
				_focusedObject->setPosition(position);
				_focusedObject->setOrigin(origin);
				_focusedObject->setRotation(rotation);
				_focusedObject->setScale(scale);
			}

			if(auto* object = _focusedObject->getComponentAs<SpriteComponent>(); object != nullptr) {
				if(ImGui::CollapsingHeader("Sprite Component")) {
					if(ImGui::BeginCombo(
						"DisplayData",
						( object->getSpriteDisplayData() )?object->getSpriteDisplayData()->getName().c_str() : "None"
					)) {
						if(ImGui::MenuItem("None", nullptr, object->getSpriteDisplayData() == nullptr))
							object->setSpriteDisplayData(nullptr);
						for(auto& it : _spriteDisplayDataHolder) {
							if(ImGui::MenuItem(it.getName().c_str(), nullptr, object->getSpriteDisplayData() == &it))
								object->setSpriteDisplayData(&it);
						}

						ImGui::EndCombo();
					}
				}
			}

			if(auto* object = _focusedObject->getComponentAs<ArmatureComponent>(); object != nullptr) {
				if(ImGui::CollapsingHeader("Armature Component")) {
					if(ImGui::BeginCombo(
						"DisplayData",
						( object->getArmatureDisplayData() )?object->getArmatureDisplayData()->getName().c_str() : "None"
					)) {
						if(ImGui::MenuItem("None", nullptr, object->getArmatureDisplayData() == nullptr))
							object->setArmatureDisplayData(nullptr);
						for(auto& it : _armatureDisplayDataHolder) {
							if(ImGui::MenuItem(it.getName().c_str(), nullptr, object->getArmatureDisplayData() == &it))
								object->setArmatureDisplayData(&it);
						}
						ImGui::EndCombo();
					}
				}
			}

			if(auto* object = _focusedObject->getComponentAs<InputControllerComponent>(); object != nullptr) {
				if(ImGui::CollapsingHeader("InputControllerComponent")) {
					
				}
			}

		}
		ImGui::End();
	}
    void LevelEditor::_renderObjectsList() {
		if(ImGui::Begin("Objects", &_ifRenderObjectsList)){
			ImGui::Separator();
			if(ImGui::BeginChild("Objects")) {
				int i{0};
				for(auto& group : _collectingHolder) {
					if(ImGui::SmallButton((std::string{"+###"} + std::to_string(i++)).c_str())) {
						auto it = addEntity(group.first);
						if(it != group.second.end()) {
							_focusedObject = it;
							_anySelected = true;
						}
						else
							_anySelected = false;
						//_focusedObject->move({0.f, 0.f, -1000.f});
					}
					ImGui::SameLine();
					if(ImGui::TreeNode(group.first.c_str())) {
						for(auto object = group.second.begin(); object != group.second.end(); ++object) {
							bool temp = object == _focusedObject && _anySelected;
							if(ImGui::SmallButton("-")) {
								if(temp) {
									_anySelected = false;
								}
								group.second.erase(object);
								--object;
								continue;
							}
							ImGui::SameLine();
							if(ImGui::Selectable((object->getName()+"###"+std::to_string(i++)).c_str(), temp)) {
								if(temp) {
									_anySelected = false;
								}
								else {
									_focusedObject = object;
									_anySelected = true;
								}
							}
						}
						ImGui::TreePop();
					}
					ImGui::Separator();
				}
			}
			ImGui::EndChild();

		}
		ImGui::End();
	}
    void LevelEditor::_renderModulesManager() {
		if(ImGui::Begin("Modules Manager")) {
			if(!_anySelected) {
				ImGui::Text("Select Object First!");
			}
			else {
				bool sC = _focusedObject->hasComponent<SpriteComponent>();
				if(ImGui::Selectable("SpriteComponent", sC)) {
					if(sC)
						_focusedObject->removeComponent<SpriteComponent>();
					else
						_focusedObject->addComponent<SpriteComponent>();
				}
				sC = _focusedObject->hasComponent<InputControllerComponent>();
				if(ImGui::Selectable("InputControllerComponent", sC)) {
					if(sC)
						_focusedObject->removeComponent<InputControllerComponent>();
					else
						_focusedObject->addComponent<InputControllerComponent>();
				}
				sC = _focusedObject->hasComponent<ArmatureComponent>();
				if(ImGui::Selectable("ArmatureComponent", sC)) {
					if(sC)
						_focusedObject->removeComponent<ArmatureComponent>();
					else
						_focusedObject->addComponent<ArmatureComponent>();
				}
			}
		}
		ImGui::End();
	}

    
    LevelEditor::EntitiesHolder_t::iterator LevelEditor::addEntity(const std::string group, const std::string name) {
		auto& subGroup = _getSubHolder(group);
		if (auto it = _find(group, name); it == subGroup.end()) {
			subGroup.emplace_back(name);
			return subGroup.end()-1;
		}
		return subGroup.end();
	}

    Entity* LevelEditor::getEntity(const std::string group, const std::string name) {
		if (auto it = _find(group, name); it != _getSubHolder(group).end())
		{
			return &(*it);
		}

		return nullptr;
	}

	const Entity* LevelEditor::getEntity(const std::string group, const std::string name) const {
		if (auto it = _find(group, name); it != _getSubHolder(group).end())
		{
			return &(*it);
		}

		return nullptr;
	}

	///
	bool LevelEditor::removeEntity(const std::string group, const std::string name) {
		if (auto it = _find(group, name); it != _getSubHolder(group).end())
		{
			_getSubHolder(group).erase(it);

			return true;
		}

		return false;
	}

	///
	void LevelEditor::removeAllEntities(const std::string group){
		_getSubHolder(group).clear();
	}

	///
	void LevelEditor::removeAllEntities()	{
		for (auto& holder : _collectingHolder) {
			holder.second.clear();
		}
	}

	///
	bool LevelEditor::hasEntity(const std::string group, const std::string name) {
		return _find(group, name) != _getSubHolder(group).end();
	}


	///
	void LevelEditor::loadFromFile(const std::string& filepath){
		std::ifstream file{ filepath };
		Json config;

		file >> config;

		for (auto it = config.begin(); it != config.end(); ++it)
		{
			for (Json& entity : it.value())
			{
				addEntity(it.key(), entity["name"]);

				getEntity(it.key(), entity["name"])->loadFromConfig(entity);
			}
		}
	}

	///
	void LevelEditor::saveToFile(const std::string& filepath) const{
		std::ofstream file{ filepath };
		Json config;

		for (auto& holder : _collectingHolder) {
			Json& currGroup = config[holder.first] = Json::array();

			for (auto& entity : holder.second) {
				currGroup.push_back(Json::object());
				Json& currEntity = currGroup.back();

				entity.saveToConfig(currEntity);
			}
		}

		file << std::setw(4) << config << std::endl;
	}

    std::string LevelEditor::_getUniqueName() {
		static size_t id = 0;

		return "unnamed_" + std::to_string(id++);
	}

    LevelEditor::EntitiesHolder_t& LevelEditor::_getSubHolder(const std::string& group) {
		return _collectingHolder.at(group);
	}

	const LevelEditor::EntitiesHolder_t& LevelEditor::_getSubHolder(const std::string& group) const	{
		return _collectingHolder.at(group);
	}

	typename LevelEditor::EntitiesHolder_t::iterator LevelEditor::_find(const std::string group, const std::string& name) {
		auto& subHolder = _getSubHolder(group);

		return std::find_if(subHolder.begin(), subHolder.end(), [&](const auto& arg) {
			return arg.getName() == name;
		});
	}

	typename LevelEditor::EntitiesHolder_t::const_iterator LevelEditor::_find(const std::string group, const std::string& name) const {
		const auto& subHolder = _getSubHolder(group);

		return std::find_if(subHolder.begin(), subHolder.end(), [&](const auto& arg) {
			return arg.getName() == name;
		});
	}


}

//#endif