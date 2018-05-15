//#include "Szczur/Utility/Modules/Module.hpp"
#pragma once
#include "Szczur/Modules/Window/Window.hpp"
#include "Szczur/Modules/Input/Input.hpp"
#include "Szczur/Modules/Script/Script.hpp"
#include "Szczur/Modules/GUI/GUI.hpp"
#include "Szczur/Utility/Logger.hpp"
#include "Szczur/Modules/GUI/ImageWidget.hpp"

#include "Szczur/Modules/PrepScreen/ResourcesContainer.hpp"
#include "Szczur/Modules/PrepScreen/Skill/SkillCodex.hpp"
#include "GUI/SkillArea.hpp"
//#include "GUI/ColoredPPBar.hpp"
#include "GUI/GrayPPArea.hpp"
#include "GUI/ProfessionArea.hpp"
#include "GUI/ChosenSkillArea.hpp"
#include "GUI/ColorFilterArea.hpp"

#include "GUI/InfoBar.hpp"
#include "GUI/GlyphArea.hpp"

namespace rat
{
    class Widget; class ScrollAreaWidget; class ImageWidget;

    class PrepScreen : public Module <Window, Input, Script, GUI>
    {
    public:
        void init()
        {
            initGUI();
        }

        void initScript();
        void initGUI();

        PrepScreen();
        ~PrepScreen();

        SkillCodex& getSkillCodex();

    private:
        SkillCodex _codex;
        ResourcesContainer _source;

        
        
        void addSkill(Skill* skill);
        void test();
        std::vector<std::unique_ptr<Skill>> testSkills;

        Widget* _base{nullptr};

        //ColoredPPBar _coloredPPsBar;
        GrayPPArea _grayPPsBar;

        SkillArea _skillArea;
        ProfessionArea _profArea;
        ChosenSkillArea _chosenSkillArea;
        //ColorFilterArea _colorFilterArea;

        InfoBar _infoBar;

        WindowWidget* _testBar{nullptr};
        GlyphArea _testGlyphBar;

        void _initProfessionBar(GUI& gui);
        void _initSkillArea();
        void _initProfArea();
        void _initChosenSkillArea();
        void _initColorFilterArea();

        void _loadAssetsFromGUI();
    };
}