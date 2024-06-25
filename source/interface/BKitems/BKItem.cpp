//
// Created by Davis Polito on 4/22/24.
//

#include "BKItem.h"
#include "paths.h"
namespace
{
    Array<Path> getPathForPreparation (bitklavier::BKPreparationType type)
    {
        if (type == bitklavier::BKPreparationType::PreparationTypeDirect)
            return Paths::directPaths();
        if (type == bitklavier::BKPreparationType::PreparationTypeNostalgic)
            return Paths::nostalgicPaths();
        if (type == bitklavier::BKPreparationType::PreparationTypeKeymap)
            return Paths::keymapPaths();
    }
}

BKItem::BKItem (bitklavier::BKPreparationType type) : Button("bkitem")
{


    image_component_.setComponent(this);


    Array<Path> paths;
    paths = getPathForPreparation(type);
    layer_1_ = paths.getUnchecked(0);
    layer_2_ = paths.getUnchecked(1);
    layer_3_ = paths.getUnchecked(2);
    layer_4_ = paths.getUnchecked(3);

}


void BKItem::mouseDown(const MouseEvent& e)
{
    getParentComponent()->mouseDown(e);
}

void BKItem::mouseDrag(const MouseEvent& e)
{
    getParentComponent()->mouseDrag(e);
}

void BKItem::mouseDoubleClick(const MouseEvent& e)
{

}