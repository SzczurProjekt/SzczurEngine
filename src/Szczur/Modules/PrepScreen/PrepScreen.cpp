#include "PrepScreen.hpp"
#include "Skill/Skill.hpp"

#include "GUI/SkillBar.hpp"

#include "ProfessionTypes.hpp"

#include "Szczur/Modules/GUI/ImageWidget.hpp"
#include "Szczur/Modules/GUI/Widget.hpp"
#include "Szczur/Modules/GUI/TextWidget.hpp"
#include "Szczur/Modules/GUI/ScrollAreaWidget.hpp"
#include "Szczur/Modules/GUI/WindowWidget.hpp"

#include "GlyphTypes.hpp"

#include "Szczur/Utility/Logger.hpp" 

namespace rat
{

    PrepScreen::PrepScreen()
    :
    //coloredPPsBar(_source),_//coloredPPsBar(_source),
    _grayPPsBar(_source),
    _skillArea(_grayPPsBar),
    _profArea(_skillArea),
    _chosenSkillArea(_skillArea)//,
    //_colorFilterArea(_skillArea)
    {
        LOG_INFO(this, " : Module PrepScreen initing...");
        init();
        LOG_INFO(this, " : Module PrepScreen inited.");
    }

    PrepScreen::~PrepScreen()
    {
        LOG_INFO(this, " : Module PrepScreen destroyed.");
    }

    void PrepScreen::initScript()
    {
    }

    SkillCodex& PrepScreen::getSkillCodex()
    {
        return _codex;
    }

    void PrepScreen::initGUI()
    {
        auto& gui = getModule<GUI>();

        _loadAssetsFromGUI();
        _codex.initAssetsViaGUI(gui);

        _base = gui.addInterface();

        test();
    }


    void PrepScreen::test()
    {
        auto& gui = getModule<GUI>();


        _grayPPsBar.setParent(_base);
        _grayPPsBar.initAssetsViaGUI(gui);
        _grayPPsBar.setPosition(1280.f/2.f, 0);

        _grayPPsBar.recalculate();

        

        _initSkillArea(); // Traitor
        _initChosenSkillArea();
        _initProfArea();
        _initColorFilterArea();

        _infoBar.setParent(_base);
        _infoBar.setPosition(600.f, 400.f);
        _infoBar.setName("Test");
        _infoBar.initAssetsViaGUI(gui);
        _infoBar.setInfo("Testowe info\nHalo halo dzieciaki efwefe wfsdvds grefdbkrfikbnj hrebf wkjbjerfkefb reji nnbi nui uyth ctr v yt  vhyt  h crcg ctr gc yctych  tyc ytc trc h t yh vg c\nAmadeusz");
        

        _testBar = new WindowWidget;
        _testBar->setTexture(gui.getAsset<sf::Texture>("Assets/Test/ChosenSkillWindow.png"), 200);
        _base->add(_testBar);
        _testBar->setScale(0.2f, 0.2f);
        _testBar->setPosition(1111.f, 350.f);
        _testBar->setPatchAmount(4, 0);

        _testGlyphBar.setPosition(300.f, 300.f);
        _testGlyphBar.setParent(_base);
        _testGlyphBar.initAssetsViaGUI(gui);
    }

    void PrepScreen::_initSkillArea()
    {
        _skillArea.initChosenSkillArea(_chosenSkillArea);
        _skillArea.setParent(_base);
        _skillArea.initViaSkillCodex(_codex); //traitor
        _skillArea.initAssetsViaGUI(getModule<GUI>());
        _skillArea.activate("Mele", {});
    }

    void PrepScreen::_initProfArea()
    {
        _profArea.setParent(_base);
        _profArea.setPosition(40.f, 120.f);
        _profArea.initAssetsViaGUI(getModule<GUI>());
    }
    
    void PrepScreen::_initChosenSkillArea()
    {
        _chosenSkillArea.setParent(_base);
        _chosenSkillArea.setPosition(200.f, 600.f);
        _chosenSkillArea.initAssetsViaGUI(getModule<GUI>());
    }
    void PrepScreen::_initColorFilterArea()
    {
        /*
        _colorFilterArea.setParent(_base);f_//coloredPPsBar(_source),
        _colorFilterArea.setPosition(500.f, 200.f);
        _colorFilterArea.initAssetsViaGUI(getModule<GUI>());
        _colorFilterArea.setWidth(250.f);   
        */
    }

    void PrepScreen::_loadAssetsFromGUI()
    {
        auto& gui = getModule<GUI>();

        std::string path = "Assets/Test/";

        gui.addAsset<sf::Texture>(path + "Bar.png");
        gui.addAsset<sf::Texture>(path + "ChosenSkillWindow.png");
        gui.addAsset<sf::Texture>(path + "Scroller.png");
        gui.addAsset<sf::Texture>(path + "ScrollerBar.png");
        gui.addAsset<sf::Texture>(path + "ScrollerBound.png");
        gui.addAsset<sf::Texture>(path + "Window.png");
        gui.addAsset<sf::Texture>(path + "Window2.png");
        gui.addAsset<sf::Texture>(path + "GlyphCircle.png");
        gui.addAsset<sf::Texture>(path + "GrayPPWindow.png");
        gui.addAsset<sf::Texture>("Assets/PrepScreen/GrayPP.png");
        

        GlyphTypes glyphTypes;
        for(auto& type : glyphTypes)
        {
            const std::string typePath = path + type + "Glyph.png";
            gui.addAsset<sf::Texture>(typePath);
        }

        gui.addAsset<sf::Font>("Assets/fonts/NotoMono.ttf");
    }
    
    
}