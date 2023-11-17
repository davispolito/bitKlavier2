//
// Created by Davis Polito on 11/8/23.
//

#include "bitKlavierGuiInterface.h"
#include <memory>
#include "FullInterface.h"
BitKlavierGuiInterface::BitKlavierGuiInterface (bool use_gui)
{
    if (use_gui) {
        gui_ = std::make_unique<FullInterface>();
    }
}

BitKlavierGuiInterface::~BitKlavierGuiInterface() {}

void BitKlavierGuiInterface::updateFullGui() {
    if (gui_ == nullptr)
        return;

//    gui_->setAllValues(synth_->getControls());
//    gui_->reset();
}