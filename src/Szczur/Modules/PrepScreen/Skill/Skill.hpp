#pragma once

#include <initializer_list>
#include <set>

#include <SFML/Graphics/Texture.hpp>

#include "SkillCost.hpp"

namespace rat
{
    class ResourcesContainer;
    class Skill
    {
        using amount_t = size_t;
        using power_t = size_t;

        public:
        Skill(const std::string& name);

        const std::string& getName() const;
        const SkillCost& getCostInfo() const;
        void unlock();
        bool isUnlocked() const;
        void lock();
        bool isBought() const;
        void buyFrom(ResourcesContainer& source);

        void returnCostsTo(ResourcesContainer& source);

        bool canBeBoughtFrom(ResourcesContainer& source);
        void setPPCost(amount_t cost);
        void addRequirement(GlyphID glyph, power_t power = 1);

        void setRequirements(std::initializer_list<std::pair<GlyphID, power_t>> requirements);

        void setProfession(const std::string& profession);
        const std::string getProfession() const;

        const std::set<GlyphID> getGlyphs() const;

        void setTexturePath(const std::string& texturePath);
        const std::string& getTexturePath() const;
        void setTexture(sf::Texture* texture);
        sf::Texture* getTexture() const;

        bool hasGlyphs(const std::set<GlyphID>& colors);

    private:
        std::string _name;
        std::string _profession{"Mele"};
        SkillCost _cost;
        
        bool _unlocked{true};
        bool _isBought{false};

        std::string _texturePath{""};
        sf::Texture* _iconTexture{nullptr};
    };
}