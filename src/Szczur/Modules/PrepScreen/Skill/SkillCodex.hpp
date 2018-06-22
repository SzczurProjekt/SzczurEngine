#pragma once
#include <unordered_map>
#include <memory>
#include <vector>

#include "Skill.hpp"


namespace rat 
{
    class GUI;

    class SkillCodex
    {

    public:
        SkillCodex();

        Skill* getSkill(const std::string& name);
        Skill* addSkill(std::unique_ptr<Skill> skill);
        void renameSkill(const std::string& oldName, const std::string& newName);
        void initAssetsViaGUI(GUI& gui);

        auto begin()
        {
            return _skills.begin();
        }
        auto end()
        {
            return _skills.end();
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<Skill>> _skills;

        void _initSkills();
    };
}