//
// Created by Davis Polito on 1/17/24.
//

#include "ConstructionSite.h"
#include "FullInterface.h"
#include "ConstructionSite.h"
#include "synth_gui_interface.h"
#include "Preparations.h"
#include "sound_engine.h"
ConstructionSite::ConstructionSite( juce::ValueTree &v,  juce::UndoManager &um, OpenGlWrapper &open_gl, SynthGuiData* data) : SynthSection("Construction Site"),
                                                                                 tracktion::engine::ValueTreeObjectList<PreparationSection>(v),
                                                                                     undo(um), open_gl(open_gl),
                                                                                     cableView(*this),
                                                                                     preparationSelector(*this)
{
    setWantsKeyboardFocus(true);
    addKeyListener(this);
    setSkinOverride(Skin::kConstructionSite);
    setInterceptsMouseClicks(false,true);
//addAndMakeVisible (cableView);
    data->synth->getEngine()->processorGraph->addChangeListener(this);
//addMouseListener (&cableView, true);
    prepFactory.Register(bitklavier::BKPreparationType::PreparationTypeDirect, DirectPreparation::createDirectSection);
    prepFactory.Register(bitklavier::BKPreparationType::PreparationTypeNostalgic, NostalgicPreparation::createNostalgicSection);
    prepFactory.Register(bitklavier::BKPreparationType::PreparationTypeKeymap, KeymapPreparation::createKeymapSection);
    prepFactory.Register(bitklavier::BKPreparationType::PreparationTypeResonance, ResonancePreparation::createResonanceSection);
    prepFactory.Register(bitklavier::BKPreparationType::PreparationTypeSynchronic, SynchronicPreparation::createSynchronicSection);
    prepFactory.Register(bitklavier::BKPreparationType::PreparationTypeBlendronic, BlendronicPreparation::createBlendronicSection);
    prepFactory.Register(bitklavier::BKPreparationType::PreparationTypeTempo, TempoPreparation::createTempoSection);
    prepFactory.Register(bitklavier::BKPreparationType::PreparationTypeTuning, TuningPreparation::createTuningSection);
    cableView.toBack();
    addSubSection(&cableView);
}
void ConstructionSite::valueTreeParentChanged(juce::ValueTree &changed) {
//    SynthGuiInterface* _parent = findParentComponentOfClass<SynthGuiInterface>();
//    ///changed.copyPropertiesAndChildrenFrom(_parent->getSynth()->getValueTree(),nullptr);
//
//    parent = _parent->getSynth()->getValueTree().getChildWithName(IDs::PIANO);

}

void ConstructionSite::valueTreeRedirected (juce::ValueTree&)
{
    SynthGuiInterface* interface = findParentComponentOfClass<SynthGuiInterface>();

//    for (auto object : objects)
//    {
//        DBG("bo");
//        object->destroyOpenGlComponents(interface->getGui()->open_gl_);
//        this->removeSubSection(object);
//    }
    deleteAllObjects();
    rebuildObjects();
} // may need to add handling if this is hit

void ConstructionSite::deleteObject(PreparationSection *at)  {
    if ((OpenGLContext::getCurrentContext() == nullptr))
    {
        SynthGuiInterface* _parent = findParentComponentOfClass<SynthGuiInterface>();


        //safe to do on message thread because we have locked processing if this is called
        at->setVisible(false);
        open_gl.context.executeOnGLThread([this,&at](OpenGLContext &openGLContext) {
           this->removeSubSection(at);
        },false);
    }

}

PreparationSection* ConstructionSite::createNewObject (const juce::ValueTree& v)
{

    auto s = prepFactory.CreateObject((int)v.getProperty(IDs::type), v, open_gl);
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
//    last_proc = s->getProcessor();
    addSubSection(s);
    Skin default_skin;
    s->setSkinValues(default_skin, false);
    s->setDefaultColor();
    s->setSizeRatio(size_ratio_);
    s->setCentrePosition(s->x, s->y);
    s->setSize(s->width, s->height);
    s->addSoundSet(parent->sampleLoadManager.global_soundset);
    s->selectedSet = &(preparationSelector.getLassoSelection());
    preparationSelector.getLassoSelection().addChangeListener(s);
    s->addListener(&cableView);
    return s;
}
void ConstructionSite::reset()
{
    SynthGuiInterface* _parent = findParentComponentOfClass<SynthGuiInterface>();
    //safe to do on message thread because we have locked processing if this is called
    _parent->getSynth()->getEngine()->resetEngine();
    parent = _parent->getSynth()->getValueTree().getChildWithName(IDs::PIANO);

    for(auto object : objects)
    {
        newObjectAdded(object);
    }

}
void ConstructionSite::newObjectAdded (PreparationSection* object)
{
    SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
    parent->getSynth()->processorInitQueue.try_enqueue([this, object]
                                                       {
                                                           SynthGuiInterface* _parent = findParentComponentOfClass<SynthGuiInterface>();
                                                           object->setNodeAndPortInfo(_parent->getSynth()->addProcessor(std::move(object->getProcessorPtr())));
                                                           //changelistener callback is causing timing errors here.

                                                           //last_proc.reset();
                                                       });
}



ConstructionSite::~ConstructionSite(void)
{
    removeMouseListener(&cableView);
    removeChildComponent(&selectorLasso);
    freeObjects();
}
void ConstructionSite::paintBackground (juce::Graphics& g)
{
    paintBody(g);
    paintChildrenBackgrounds(g);
}



void ConstructionSite::resized()
{
    cableView.setBounds(getLocalBounds());
    cableView.updateCablePositions();
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
        parent.addChild(t,-1, nullptr);
        //DBG("place" + String(lastX) + " " + String(lastY));
    } else if (code == 78) // N nostalgic
    {
        ValueTree t(IDs::PREPARATION);

        t.setProperty(IDs::type,bitklavier::BKPreparationType::PreparationTypeNostalgic, nullptr);
        t.setProperty(IDs::width, 260, nullptr);
        t.setProperty(IDs::height, 132, nullptr);
        t.setProperty(IDs::x,lastX  - (260/2), nullptr);
        t.setProperty(IDs::y,lastY - (132/2), nullptr);

        parent.addChild(t,-1, nullptr);
    } else if (code == 75) // K Keymap
    {
        ValueTree t(IDs::PREPARATION);

        t.setProperty(IDs::type,bitklavier::BKPreparationType::PreparationTypeKeymap, nullptr);
        t.setProperty(IDs::width, 114, nullptr);
        t.setProperty(IDs::height, 76, nullptr);
        t.setProperty(IDs::x,lastX - 114/2, nullptr);
        t.setProperty(IDs::y,lastY - 76 /2, nullptr);
        // DBG("Position" + String(lastX) + " " + String(lastY));
        parent.addChild(t,-1, nullptr);
        //DBG("place" + String(lastX) + " " + String(lastY));
    } else if (code == 82) // R resonance
    {
        ValueTree t(IDs::PREPARATION);

        t.setProperty(IDs::type, bitklavier::BKPreparationType::PreparationTypeResonance, nullptr);
        t.setProperty(IDs::width, 260, nullptr);
        t.setProperty(IDs::height, 132, nullptr);
        t.setProperty(IDs::x, lastX - 260 / 2, nullptr);
        t.setProperty(IDs::y, lastY - 132 / 2, nullptr);
        parent.addChild(t, -1, nullptr);
    } else if (code == 83) // S synchronic
    {
        ValueTree t(IDs::PREPARATION);

        t.setProperty(IDs::type, bitklavier::BKPreparationType::PreparationTypeSynchronic, nullptr);
        t.setProperty(IDs::width, 260, nullptr);
        t.setProperty(IDs::height, 132, nullptr);
        t.setProperty(IDs::x, lastX - 260 / 2, nullptr);
        t.setProperty(IDs::y, lastY - 132 / 2, nullptr);
        parent.addChild(t, -1, nullptr);
    } else if (code == 66) // B blendronic
    {
        ValueTree t(IDs::PREPARATION);

        t.setProperty(IDs::type, bitklavier::BKPreparationType::PreparationTypeBlendronic, nullptr);
        t.setProperty(IDs::width, 260, nullptr);
        t.setProperty(IDs::height, 132, nullptr);
        t.setProperty(IDs::x, lastX - 260 / 2, nullptr);
        t.setProperty(IDs::y, lastY - 132 / 2, nullptr);
        parent.addChild(t, -1, nullptr);
    } else if (code == 77) // M tempo
    {
        ValueTree t(IDs::PREPARATION);

        t.setProperty(IDs::type, bitklavier::BKPreparationType::PreparationTypeTempo, nullptr);
        t.setProperty(IDs::width, 132, nullptr);
        t.setProperty(IDs::height, 260, nullptr);
        t.setProperty(IDs::x, lastX - 132 / 2, nullptr);
        t.setProperty(IDs::y, lastY -  260 / 2, nullptr);
        parent.addChild(t, -1, nullptr);
    }
    else if (code == 84) // T tuning
    {
        ValueTree t(IDs::PREPARATION);

        t.setProperty(IDs::type, bitklavier::BKPreparationType::PreparationTypeTuning, nullptr);
        t.setProperty(IDs::width, 132, nullptr);
        t.setProperty(IDs::height, 260, nullptr);
        t.setProperty(IDs::x, lastX - 132 / 2, nullptr);
        t.setProperty(IDs::y, lastY -  260 / 2 , nullptr);
        parent.addChild(t, -1, nullptr);
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


        repaint();
    }
}



void ConstructionSite::mouseDown (const MouseEvent& eo)
{


    MouseEvent e = eo.getEventRelativeTo(this);
    auto itemToSelect = dynamic_cast<PreparationSection*> (e.originalComponent->getParentComponent());
    if(itemToSelect == nullptr)
    {
        preparationSelector.getLassoSelection().deselectAll();
        DBG("mousedown empty space");
    } else
    {
        DBG("mousedown on object");
    }
    addChildComponent(selectorLasso);
    selectorLasso.beginLasso(e, &preparationSelector);
    //////Fake drag so the lasso will select anything we click and drag////////
    auto thisPoint = e.getPosition();
    thisPoint.addXY(10,10);
    selectorLasso.dragLasso(e.withNewPosition(thisPoint));


    held = false;

    lastX = eo.x; lastY = eo.y;


    mouse = e.position;

    // This must happen before the right-click menu or the menu will close
   grabKeyboardFocus();



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

    // Clicking on blank graph space

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

void ConstructionSite::mouseUp (const MouseEvent& eo)
{
    //inLasso = false;
DBG("mouseupconst");
    selectorLasso.endLasso();
    removeChildComponent(&selectorLasso);
    if (edittingComment) return;

    MouseEvent e = eo.getEventRelativeTo(this);
    cableView.mouseUp(e);
    // Do nothing on right click mouse up
    if (e.mods.isRightButtonDown()) return;
#if JUCE_MAC
    // Do nothing on ctrl click mouse up
    if (e.mods.isCtrlDown()) return;
#endif




//    DBG(e.x);
//    DBG(e.y);
//    DBG(eo.x);
//    DBG(eo.y);
    //getParentComponent()->grabKeyboardFocus();

    grabKeyboardFocus();


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

    cableView.mouseDrag(e);
    // Do nothing on right click drag
    if (e.mods.isRightButtonDown()) return;




    if(e.mods.isShiftDown()) {
        selectorLasso.toFront(false);
        selectorLasso.dragLasso(e);
    }


    repaint();

}

void ConstructionSite::updateComponents()
    {
        SynthGuiInterface* parent = findParentComponentOfClass<SynthGuiInterface>();
        for (int i = objects.size(); --i >= 0;) {
//            if (parent->getSynth()->getNodeForId(objects.getUnchecked(i)->pluginID) == nullptr) {
//                parent.removeChild(objects.getUnchecked(i)->parent, nullptr);
//            }
        }




        for (auto* fc : objects)
            fc->update();

        cableView.updateComponents();
//        for (auto* f : graph.graph.getNodes())
//        {
//            if (getComponentForPlugin (f->nodeID) == nullptr)
//            {
//                auto* comp = nodes.add (new PluginComponent (*this, f->nodeID));
//                addAndMakeVisible (comp);
//                comp->update();
//            }
//        }



    }







