#include "ScenesManager.hpp"

#include <experimental/filesystem>

#include "Szczur/Modules/FileSystem/FileDialog.hpp"
#include "Szczur/Modules/FileSystem/DirectoryDialog.hpp"

#include <algorithm>

#include "Components/CameraComponent.hpp"
#include "Components/BaseComponent.hpp"
#include "Components/ScriptableComponent.hpp"

#include <Szczur/Modules/World/World.hpp>

namespace rat
{

ScenesManager::ScenesManager() {
	_armatureDisplayDataHolder.reserve(100);
}

Scene* ScenesManager::addScene()
{
	Scene* scene = _holder.emplace_back(std::make_unique<Scene>(this)).get();

	// Add default player
	Entity* player = scene->addEntity("single");
	player->setName("Player");
	scene->setPlayerID(player->getID());
	
	// Add default camera
	Entity* camera = scene->addEntity("single");
	camera->addComponent<CameraComponent>();
	camera->setName("Camera");
	camera->setPosition({ 0.f, 1160.f, 3085.f });
	camera->setRotation({ 15.f, 0.f, 0.f });

	return scene;
}

bool ScenesManager::removeScene(size_t id)
{
	if (auto it = _find(id); it != _holder.end())
	{
		_holder.erase(it);

		if (_currentSceneID == id)
		{
			if(_holder.size())
				_currentSceneID = (*_holder.begin())->getID();
			else
				_currentSceneID = addScene()->getID();
		}

		return true;
	}

	return false;
}

void ScenesManager::removeAllScenes()
{
	_holder.clear();

	_currentSceneID = 0u;

	#ifdef EDITOR
	detail::globalPtr<World>->getLevelEditor().getObjectsList().unselect();
	#endif //EDITOR
}

Scene* ScenesManager::getScene(size_t id) const
{
	if (auto it = _find(id); it != _holder.end())
	{
		return it->get();
	}

	return nullptr;
}

Scene* ScenesManager::getScene(const std::string& name) {

	for(auto& scene : _holder) {
		if(scene->getName() == name) {
			return scene.get();
		}
	}
	return nullptr;
}

ScenesManager::ScenesHolder_t& ScenesManager::getScenes()
{
	return _holder;
}

const ScenesManager::ScenesHolder_t& ScenesManager::getScenes() const
{
	return _holder;
}

bool ScenesManager::hasScene(size_t id) const
{
	return _find(id) != _holder.end();
}

bool ScenesManager::setCurrentScene(size_t id)
{
	if (hasScene(id))
	{
		#ifdef EDITOR
		detail::globalPtr<World>->getLevelEditor().getObjectsList().unselect();
		#endif //EDITOR
		
		_currentSceneID = id;

		if(isGameRunning()) {
			auto* scene = getCurrentScene();		
		
			scene->forEach([](const std::string& group, Entity& entity) {
					if(auto* comp = entity.getComponentAs<ScriptableComponent>()) comp->sceneChanged();
				}
			);
		}

		detail::globalPtr<World>->getLevelEditor().updateCurrentCamera();

		return true;
	}
	return false;
}

Scene* ScenesManager::getCurrentScene() const
{
	return getScene(_currentSceneID);
}

size_t ScenesManager::getCurrentSceneID() const
{
	return _currentSceneID;
}

bool ScenesManager::isCurrentSceneValid() const
{
	return _currentSceneID != 0u;
}

void ScenesManager::loadFromConfig(Json& config) {

	removeAllScenes();

	_currentSceneID = config["currentSceneID"];

	Json& scenes = config["scenes"];

	for (auto& current : scenes)
	{
		auto* scene = addScene();
		scene->removeAllEntities();
		scene->loadFromConfig(current);

		bool foundPlayer = false;
		bool foundCamera = false;
		for(auto& ent : scene->getEntities("single")) {
			if(ent->getName() == "Player") {
				foundPlayer = true;
				scene->setPlayerID(ent->getID());
			}
			else if(ent->getName() == "Camera") {
				foundCamera = true;
			}
		}
		if(!foundPlayer) {
			Entity* player = scene->addEntity("single");
			player->setName("Player");
			scene->setPlayerID(player->getID());
		}
		if(!foundCamera) {			
			Entity* camera = scene->addEntity("single");
			camera->addComponent<CameraComponent>();
			camera->setName("Camera");
			camera->setPosition({ 0.f, 1160.f, 3085.f });
			camera->setRotation({ 15.f, 0.f, 0.f });
		}

	}
}

void ScenesManager::saveToConfig(Json& config) {

	config["currentSceneID"] = getCurrentSceneID();
	Json& scenes = config["scenes"] = Json::array();

	for (auto& scene : _holder)
	{
		scenes.push_back(Json::object());
		Json& current = scenes.back();

		scene->saveToConfig(current);
	}
}

void ScenesManager::saveEntityToConfig(Entity& entity, Json& config) {
	// config["currentSceneID"] = getCurrentSceneID();
	// Json& scenes = config["scenes"];

	

	// for (auto& scene : _holder)
	// {
	// 	scenes.push_back(Json::object());
	// 	Json& current = scenes.back();

	// 	scene->saveToConfig(current);
	// }
}

void ScenesManager::loadFromFile(const std::string& filepath)
{
	std::ifstream file{ filepath };
	Json config;

	file >> config;

	loadFromConfig(config);
}

void ScenesManager::saveToFile(const std::string& filepath)
{
	std::ofstream file{ filepath };
	Json config;

	saveToConfig(config);
	file << std::setw(4) << config << std::endl;
}

ScenesManager::ArmatureDisplayDataHolder_t& ScenesManager::getArmatureDisplayDataHolder()
{
	return _armatureDisplayDataHolder;
}

const ScenesManager::ArmatureDisplayDataHolder_t& ScenesManager::getArmatureDisplayDataHolder() const
{
	return _armatureDisplayDataHolder;
}

void ScenesManager::loadScenesFromFile(const std::string& filepath)
{
	std::ifstream file{ filepath };
	Json config;

	file >> config;

	Json& scenes = config["scenes"];

	for (auto& current : scenes)
	{
		auto* scene = addScene();
		scene->removeAllEntities();
		scene->loadFromConfig(current);
	}
}

void ScenesManager::addPlayer()
{
	auto scene = getCurrentScene();

	Entity* player = scene->addEntity("single");
	player->setName("Player");
	scene->setPlayerID(player->getID());
}

void ScenesManager::addCamera()
{
	auto scene = getCurrentScene();

	Entity* camera = scene->addEntity("single");
	camera->addComponent<CameraComponent>();
	camera->setName("Camera");
}

bool ScenesManager::isGameRunning() {
	return _gameIsRunning;
}

void ScenesManager::runGame() {
	if(!_gameIsRunning) {
		_gameIsRunning = true;
		saveToConfig(_configBeforeRun);

		for(auto& scene : _holder) {
			scene->forEach([](const std::string& group, Entity& entity) {
				if(auto* comp = entity.getComponentAs<ScriptableComponent>()) comp->runScript();
			});
		}
	}
}

void ScenesManager::stopGame() {
	if(_gameIsRunning) {

		#ifdef EDITOR
		detail::globalPtr<World>->getLevelEditor().getObjectsList().unselect();
		#endif //EDITOR

		_gameIsRunning = false;
		loadFromConfig(_configBeforeRun);
	}
}

Json& ScenesManager::getRunConfig() {
	return _configBeforeRun;
}

TextureDataHolder& ScenesManager::getTextureDataHolder() {
	return _textureDataHolder;
}

typename ScenesManager::ScenesHolder_t::iterator ScenesManager::_find(size_t id)
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		return arg->getID() == id;
	});
}

typename ScenesManager::ScenesHolder_t::const_iterator ScenesManager::_find(size_t id) const
{
	return std::find_if(_holder.begin(), _holder.end(), [=](const auto& arg) {
		return arg->getID() == id;
	});
}

#ifdef EDITOR
	void ScenesManager::menuSave() {
		if(currentFilePath == "") {
			std::string relative = getRelativePathFromExplorer("Save world", ".\\Editor\\Saves", "Worlds (*.world)|*.world", true);
			// std::cout<<"--s-"<<relative<<std::endl;
			if(relative != "") {
				try {
					saveToFile(relative);
					currentFilePath = relative;
				}
				catch (const std::exception& exc)
				{
					LOG_EXCEPTION(exc);
				}
			}
		}
		else {
			try {
				saveToFile(currentFilePath);
			}
			catch (const std::exception& exc)
			{
				LOG_EXCEPTION(exc);
			}
		}
	}

	std::string ScenesManager::getRelativePathFromExplorer(const std::string& title, const std::string& directory, const std::string& filter, bool saveButton) {
		namespace filesystem = std::experimental::filesystem;

		std::string file;
		if(saveButton) 
            file = FileDialog::getSaveFileName(title, directory, filter);
		else 
            file = FileDialog::getOpenFileName(title, directory, filter);

		if(file == "") 
            return "";

		std::string current = filesystem::current_path().string();

		if(current == file.substr(0, current.size()))
			return file.substr(current.size()+1);

		return "";
	}
#endif

}