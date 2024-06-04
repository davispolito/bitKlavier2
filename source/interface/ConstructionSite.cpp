//
// Created by Davis Polito on 1/17/24.
//

#include "ConstructionSite.h"
#include "FullInterface.h"
/*
==============================================================================

ConstructionSite.cpp
Created: 4 Apr 2017 5:46:38pm
Author:  Michael R Mulshine

==============================================================================
*/

#include "ConstructionSite.h"
#include "synth_gui_interface.h"
#include "Preparations/DirectPreparation.h"
#include "Preparations/NostalgicPreparation.h"
ConstructionSite::ConstructionSite( juce::ValueTree &v,  juce::UndoManager &um, OpenGlWrapper &open_gl) : SynthSection("Construction Site"),
                                                                                 tracktion::engine::ValueTreeObjectList<PreparationSection>(v),
                                                                                     state(v), undo(um), open_gl(open_gl)
{
    setWantsKeyboardFocus(true);
    addKeyListener(this);
    setSkinOverride(Skin::kConstructionSite);
    prepFactory.Register(bitklavier::BKPreparationType::PreparationTypeDirect, DirectPreparation::createDirectSection);
    prepFactory.Register(bitklavier::BKPreparationType::PreparationTypeNostalgic, NostalgicPreparation::createNostalgicSection);
}
PreparationSection* ConstructionSite::createNewObject (const juce::ValueTree& v)
{

    auto s = prepFactory.CreateObject((int)v.getProperty(IDs::type), v, open_gl);

    last_proc = s->getProcessor();
    addSubSection(s);
    Skin default_skin;
    s->setSkinValues(default_skin, false);
    s->setSizeRatio(size_ratio_);
    s->setCentrePosition(s->x, s->y);
    s->setSize(s->width, s->height);

    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    parent->getSynth()->processorInitQueue.try_enqueue([this]
                                                       {
                                                           SynthGuiInterface* _parent = findParentComponentOfClass<SynthGuiInterface>();
                                                        _parent->getSynth()->addProcessor(last_proc);

                                                        last_proc.reset();
                                                       });
    return s;
}
ConstructionSite::~ConstructionSite(void)
{
    freeObjects();
}
void ConstructionSite::paintBackground (juce::Graphics& g)
{
    paintBody(g);
    paintChildrenBackgrounds(g);
}

void ConstructionSite::resized()
{
    SynthSection::resized();
}




void ConstructionSite::redraw(void)
{
    draw();
}



bool ConstructionSite::keyPressed (const juce::KeyPress& k, juce::Component* c)
{
    int code = k.getKeyCode();

    if (code == 68) //D Direct
    {
        ValueTree t(IDs::PREPARATION);

        t.setProperty(IDs::type,bitklavier::BKPreparationType::PreparationTypeDirect, nullptr);
        t.setProperty(IDs::width, 260, nullptr);
        t.setProperty(IDs::height, 132, nullptr);
        t.setProperty(IDs::x,lastX - 260/2, nullptr);
        t.setProperty(IDs::y,lastY - 132 /2, nullptr);
       // DBG("Position" + String(lastX) + " " + String(lastY));
        state.addChild(t,-1, nullptr);
        //DBG("place" + String(lastX) + " " + String(lastY));
    } else if (code == 78) //N nostalgic
    {
        ValueTree t(IDs::PREPARATION);

        t.setProperty(IDs::type,bitklavier::BKPreparationType::PreparationTypeNostalgic, nullptr);
        t.setProperty(IDs::width, 260, nullptr);
        t.setProperty(IDs::height, 132, nullptr);
        t.setProperty(IDs::x,lastX  - (260/2), nullptr);
        t.setProperty(IDs::y,lastY - (132/2), nullptr);

        state.addChild(t,-1, nullptr);
    }
    return true;
}



void ConstructionSite::itemIsBeingDragged(BKItem* thisItem, const MouseEvent& e)
{
    repaint();
}



void ConstructionSite::draw(void)
{

//    int offset = 0;
//    for (auto item : processor.currentPiano->getItems())
//    {
//        addExistingItem(item);
//    }



    repaint();
}


void ConstructionSite::prepareItemDrag(BKItem* item, const MouseEvent& e, bool center)
{
//    if (center)
//    {
//        float X = item->getPosition().getX() + item->getWidth() / 2.0f;
//        float Y = item->getPosition().getY() + item->getHeight() / 2.0f;
//        juce::Point<float>pos(X,Y);
//        MouseEvent newEvent = e.withNewPosition(pos);
//
//        item->prepareDrag(newEvent);
//    }
//    else
//    {
//        item->prepareDrag(e);
//    }
}









void ConstructionSite::mouseMove (const MouseEvent& eo)
{
    MouseEvent e = eo.getEventRelativeTo(this);
    //MouseEvent a = eo.getEventRelativeTo(this);
//    if (e.x != lastEX) lastX = e.x;
//
//    if (e.y != lastEY) lastY = e.y;

//    lastEX = eo.x;
//    lastEY = eo.y;mouse = e.position;
    mouse = e.position;
    lastX = e.x;
    lastY = e.y;
    //DBG("screen" + String(lastX) + " " + String(lastY));
    //DBG("global" + String(e.getMouseDownX()) +" " + String(e.getMouseDownY()));
    //DBG("site" + String(a.getMouseDownX()) +" " + String(a.getMouseDownY()));
    if (connect)
    {
        lineEX = e.getEventRelativeTo(this).x;
        lineEY = e.getEventRelativeTo(this).y;

        repaint();
    }
}



void ConstructionSite::mouseDown (const MouseEvent& eo)
{
     DBG("mousedown");
    MouseEvent e = eo.getEventRelativeTo(this);


    itemToSelect = dynamic_cast<BKItem*> (e.originalComponent->getParentComponent());

    held = false;

    lastX = eo.x; lastY = eo.y;


    mouse = e.position;

    // This must happen before the right-click menu or the menu will close
   grabKeyboardFocus();

    // Clicking on an item
    if (itemToSelect != nullptr )
    {


//        processor.gallery->setGalleryDirty(true);

        // Right mouse click (must be first so right click overrides other mods)
        if (e.mods.isRightButtonDown())
        {
//            if (!itemToSelect->getSelected())
//            {
//                graph->deselectAll();
//                graph->select(itemToSelect);
//            }
//            if (processor.wrapperType == juce::AudioPluginInstance::wrapperType_Standalone)
//            {
//                getEditMenuStandalone(&buttonsAndMenusLAF, graph->getSelectedItems().size(), false, true).showMenuAsync
//                    (PopupMenu::Options(), ModalCallbackFunction::forComponent (editMenuCallback, this) );
//            }
//            else
//            {
//                getEditMenu(&buttonsAndMenusLAF, graph->getSelectedItems().size(), false, true).showMenuAsync
//                    (PopupMenu::Options(), ModalCallbackFunction::forComponent (editMenuCallback, this) );
//            }
        }
        // Control click (same as right click on Mac)
#if JUCE_MAC
        else if (e.mods.isCtrlDown())
        {
//            if (!itemToSelect->getSelected())
//            {
//                graph->deselectAll();
//                graph->select(itemToSelect);
//            }
//            if (processor.wrapperType == juce::AudioPluginInstance::wrapperType_Standalone)
//            {
//                getEditMenuStandalone(&buttonsAndMenusLAF, graph->getSelectedItems().size(), false, true).showMenuAsync
//                    (PopupMenu::Options(), ModalCallbackFunction::forComponent (editMenuCallback, this) );
//            }
//            else
//            {
//                getEditMenu(&buttonsAndMenusLAF, graph->getSelectedItems().size(), false, true).showMenuAsync
//                    (PopupMenu::Options(), ModalCallbackFunction::forComponent (editMenuCallback, this) );
//            }
        }
#endif
        else if (e.mods.isShiftDown())
        {
//            // also select this item
//            if (itemToSelect != nullptr)
//            {
//                if (!itemToSelect->getSelected())   graph->select(itemToSelect);
//                else                                graph->deselect(itemToSelect);
//            }
//
//            for (auto item : graph->getSelectedItems())
//            {
//                prepareItemDrag(item, e, true);
//            }

        }
        else if (e.mods.isAltDown())
        {
//            // make a connection and look to make another
//            if (connect) makeConnection(e.x, e.y, true);
//
//            // Copy and drag
//            if (!itemToSelect->getSelected())
//            {
//                graph->deselectAll();
//                graph->select(itemToSelect);
//            }
        }
        // Command click
        else if (e.mods.isCommandDown())
        {
//            if (!itemToSelect->getSelected())
//            {
//                graph->deselectAll();
//                graph->select(itemToSelect);
//            }
//            if (connect) makeConnection(e.x, e.y);
//            else startConnection(e.x, e.y);
        }
        // Unmodified left mouse click
        else
        {
//            if (connect) makeConnection(e.x, e.y);
//
//            if (!itemToSelect->getSelected())
//            {
//                graph->deselectAll();
//                graph->select(itemToSelect);
//            }
//
//            for (auto item : graph->getSelectedItems())
//            {
//                prepareItemDrag(item, e, true);
//            }
        }
    }
    // Clicking on blank graph space
    else
    {
        if (e.mods.isRightButtonDown())
        {
//            if (processor.wrapperType == juce::AudioPluginInstance::wrapperType_Standalone)
//            {
//                getEditMenuStandalone(&buttonsAndMenusLAF, graph->getSelectedItems().size(), true, true).showMenuAsync
//                    (PopupMenu::Options(), ModalCallbackFunction::forComponent (editMenuCallback, this) );
//            }
//            else
//            {
//                getEditMenu(&buttonsAndMenusLAF, graph->getSelectedItems().size(), true, true).showMenuAsync
//                    (PopupMenu::Options(), ModalCallbackFunction::forComponent (editMenuCallback, this) );
//            }
        }
#if JUCE_MAC
        else if (e.mods.isCtrlDown())
        {
//            if (processor.wrapperType == juce::AudioPluginInstance::wrapperType_Standalone)
//            {
//                getEditMenuStandalone(&buttonsAndMenusLAF, graph->getSelectedItems().size(), true, true).showMenuAsync
//                    (PopupMenu::Options(), ModalCallbackFunction::forComponent (editMenuCallback, this) );
//            }
//            else
//            {
//                getEditMenu(&buttonsAndMenusLAF, graph->getSelectedItems().size(), true, true).showMenuAsync
//                    (PopupMenu::Options(), ModalCallbackFunction::forComponent (editMenuCallback, this) );
//            }
        }
#endif
        else
        {
//            if (!e.mods.isShiftDown())
//            {
//                graph->deselectAll();
//            }
//            lassoSelection.deselectAll();
//
//            lasso = std::make_unique<LassoComponent<BKItem*>>();
//            addAndMakeVisible(*lasso);
//
//            lasso->setAlpha(0.5);
//            lasso->setColour(LassoComponent<BKItem*>::ColourIds::lassoFillColourId, Colours::lightgrey);
//            lasso->setColour(LassoComponent<BKItem*>::ColourIds::lassoOutlineColourId, Colours::antiquewhite);
//
//            lasso->beginLasso(eo, this);
//            inLasso = true;
        }
        // Stop trying to make a connection on blank space click
        connect = false;
    }
}

void ConstructionSite::mouseUp (const MouseEvent& eo)
{
    inLasso = false;
    if (edittingComment) return;

    MouseEvent e = eo.getEventRelativeTo(this);

    // Do nothing on right click mouse up
    if (e.mods.isRightButtonDown()) return;
#if JUCE_MAC
    // Do nothing on ctrl click mouse up
    if (e.mods.isCtrlDown()) return;
#endif

    touches.removeObject (getTouchEvent(e.source));


//    DBG(e.x);
//    DBG(e.y);
//    DBG(eo.x);
//    DBG(eo.y);
    //getParentComponent()->grabKeyboardFocus();

    grabKeyboardFocus();
    if (itemToSelect == nullptr)
    {
        //lasso->endLasso();
    }

//    if (lassoSelection.getNumSelected())
//    {
//        for (auto item : lassoSelection)
//        {
//            graph->select(item);
//        }
//
//        lassoSelection.deselectAll();
//
//        return;
//    }

    // Uncomment for press-hold-release connection behavior when using CMD shortcut
    // Otherwise make connections with CMD+click and click on target
    //  if (connect) makeConnection(e.x, e.y, e.mods.isAltDown());

//    bool shouldStore = false;
//    for (auto item : graph->getSelectedItems())
//    {
//        if (item->isDragging)
//        {
//            item->isDragging = false;
//            shouldStore = true;
//        }
//    }
//    if (shouldStore && e.getDistanceFromDragStart() > 0) processor.saveGalleryToHistory("Move");

    redraw();
}

void ConstructionSite::mouseDrag (const MouseEvent& e)
{
    if (edittingComment) return;

    // Do nothing on right click drag
    if (e.mods.isRightButtonDown()) return;

#if JUCE_IOS
    MouseEvent eo = (e.eventComponent != this) ? e.getEventRelativeTo(this) : e;

    TouchEvent* t = getTouchEvent(eo.source);

    if (t == nullptr)
    {
        t = new TouchEvent(eo.source);
        t->path.startNewSubPath(eo.position);
        touches.add(t);
    }

    t->pushPoint(eo.position, eo.mods);

    repaint();
#endif



    if (itemToSelect == nullptr) //lasso->dragLasso(e);

//    if (!connect && !e.mods.isShiftDown() && !inLasso)
//    {
//        for (auto item : graph->getSelectedItems())
//        {
//            item->performDrag(e);
//        }
//    }

    lineEX = e.getEventRelativeTo(this).x;
    lineEY = e.getEventRelativeTo(this).y;

    repaint();

}



BKItem* ConstructionSite::getItemAtPoint(const int X, const int Y)
{
    BKItem* theItem = nullptr;

    if (itemSource != nullptr)
    {
        int which = 0;

//        for (auto item : graph->getItems())
//        {
//            int left = item->getX(); int right = left + item->getWidth();
//            int top = item->getY(); int bottom = top + item->getHeight();
//
//            if (X >= left && X <= right && Y >= top && Y <= bottom) // is in item
//            {
//                theItem = dynamic_cast<BKItem*> (item);
//                break;
//            }
//
//            which++;
//        }
    }

    return theItem;
}

void ConstructionSite::findLassoItemsInArea (Array <BKItem*>& itemsFound,
    const Rectangle<int>& area)
{
    int areaX = area.getX(); int areaY = area.getY(); int areaWidth = area.getWidth(); int areaHeight = area.getHeight();

    // psuedocode determine if collide: if (x1 + w1) - x2 >= 0 and (x2 + w2) - x1 >= 0

//    for (auto item : graph->getItems())
//    {
//        int itemX = item->getX(); int itemWidth = item->getWidth();
//        int itemY = item->getY(); int itemHeight = item->getHeight();
//
//        if (((itemX + itemWidth - areaX) >= 0) && ((areaX + areaWidth - itemX) >= 0) &&
//            ((itemY + itemHeight - areaY) >= 0) && ((areaY + areaHeight - itemY) >= 0))
//            itemsFound.add(item);
//    }

    /*
    for (int x = area.getX(); x < area.getRight(); x++)
    {
        for (int y = area.getY(); y < area.getBottom(); y++)
        {
            BKItem* item = getItemAtPoint(x,y);

            if (item != nullptr) itemsFound.add(item);
        }
    }
     */
}

SelectedItemSet<BKItem*>& ConstructionSite::getLassoSelection(void)
{
    return lassoSelection;
}
