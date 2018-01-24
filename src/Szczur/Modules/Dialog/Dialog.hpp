#pragma once

#include <iostream>

#include "Szczur/Utility/Module.hpp"
#include "Szczur/Json.hpp"

#include "Szczur/Modules/GUI/GUI.hpp"

#include "DialogManager.hpp"

namespace rat {
    class Dialog : public Module<GUI> {
    public:
        template<typename Tuple>
        Dialog(Tuple&& tuple, const std::string& file);

    private:
        Interface* _interface;
        TextAreaWidget* _area;
        DialogManager _dialogManager;
    };
}

#include "Dialog.tpp"