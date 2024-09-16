#include "ParametersView.h"
#include "synth_section.h"
#include "synth_slider.h"
#include "open_gl_background.h"
namespace bitklavier {
    namespace parameters_view_detail {
        //==============================================================================
        class BooleanParameterComponent : public juce::Component {
        public:
            BooleanParameterComponent(chowdsp::BoolParameter &param, chowdsp::PluginState &pluginState)
                    : attachment(param, pluginState, button) {
                addAndMakeVisible(button);
            }

            void resized() override {
                auto area = getLocalBounds();
                area.removeFromLeft(8);
                button.setBounds(area.reduced(0, 10));
            }

        private:
            juce::ToggleButton button;
            chowdsp::ButtonAttachment attachment;

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BooleanParameterComponent)
        };

        class ChoiceParameterComponent : public juce::Component {
        public:
            ChoiceParameterComponent(chowdsp::ChoiceParameter &param, chowdsp::PluginState &pluginState)
                    : attachment(param, pluginState, box) {
                addAndMakeVisible(box);
            }

            void resized() override {
                auto area = getLocalBounds();
                area.removeFromLeft(8);
                box.setBounds(area.reduced(0, 10));
            }

        private:
            juce::ComboBox box;
            chowdsp::ComboBoxAttachment attachment;

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChoiceParameterComponent)
        };

        class SliderParameterComponent : public juce::Component {
        public:
            SliderParameterComponent(chowdsp::FloatParameter &param, chowdsp::PluginState &pluginState, SynthSection &parent)
                    : slider("slider", param, pluginState), attachment(param, pluginState, slider) {
                slider.setScrollWheelEnabled(false);
                addAndMakeVisible(slider);
                parent.addSlider(&slider, false);
                slider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);

            }

            void resized() override {
                auto area = getLocalBounds().reduced(3, 10);
                slider.setBounds(area);
                slider.redoImage();
            }

        private:
            SynthSlider slider;
            //juce::Slider slider { juce::Slider::LinearHorizontal, juce::Slider::TextEntryBoxPosition::TextBoxRight };
            chowdsp::SliderAttachment attachment;

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderParameterComponent)
        };

        //==============================================================================
        class ParameterDisplayComponent : public juce::Component {
        public:
            ParameterDisplayComponent(juce::RangedAudioParameter &param, chowdsp::PluginState &pluginState, SynthSection &parent)
                    : parameter(param), parent(parent) {
                parameterName.setText(parameter.getName(128), juce::dontSendNotification);
                parameterName.setJustificationType(juce::Justification::centredRight);
                parameterName.setInterceptsMouseClicks(false, false);
                addAndMakeVisible(parameterName);

                parameterLabel.setText(parameter.getLabel(), juce::dontSendNotification);
                parameterLabel.setInterceptsMouseClicks(false, false);
                addAndMakeVisible(parameterLabel);

                addAndMakeVisible(*(parameterComp = createParameterComp(pluginState)));

                setSize(400, 40);
            }

            void resized() override {
                auto area = getLocalBounds();

                parameterName.setBounds(area.removeFromLeft(100));
                parameterLabel.setBounds(area.removeFromRight(50));
                parameterComp->setBounds(area);
            }

        private:
            juce::RangedAudioParameter &parameter;
            juce::Label parameterName, parameterLabel;
            std::unique_ptr<juce::Component> parameterComp;
            SynthSection& parent;
            std::unique_ptr<juce::Component> createParameterComp(chowdsp::PluginState &pluginState) const {
                if (auto *boolParam = dynamic_cast<chowdsp::BoolParameter *> (&parameter))
                    return std::make_unique<BooleanParameterComponent>(*boolParam, pluginState);

                if (auto *choiceParam = dynamic_cast<chowdsp::ChoiceParameter *> (&parameter))
                    return std::make_unique<ChoiceParameterComponent>(*choiceParam, pluginState);

                if (auto *sliderParam = dynamic_cast<chowdsp::FloatParameter *> (&parameter))
                    return std::make_unique<SliderParameterComponent>(*sliderParam, pluginState, parent);

                return {};
            }

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterDisplayComponent)
        };

        //==============================================================================
        struct ParamControlItem : public juce::TreeViewItem {
            ParamControlItem(juce::RangedAudioParameter &paramIn, chowdsp::PluginState &pluginState, SynthSection &parent)
                    : param(paramIn), state(pluginState), parent(parent) {}

            bool mightContainSubItems() override { return false; }

            std::unique_ptr<juce::Component> createItemComponent() override {
                return std::make_unique<ParameterDisplayComponent>(param, state,parent);
            }

            [[nodiscard]] int getItemHeight() const override { return 40; }

            juce::RangedAudioParameter &param;
            SynthSection &parent;
            chowdsp::PluginState &state;
        };

        struct ParameterGroupItem : public juce::TreeViewItem {
            ParameterGroupItem(chowdsp::ParamHolder &params, chowdsp::PluginState &pluginState, SynthSection &parent)
                    : name(params.getName()), parent(parent) {
                params.doForAllParameterContainers(
                        [this, &pluginState](auto &paramVec) {
                            for (auto &param: paramVec)
                                addSubItem(std::make_unique<ParamControlItem>(param, pluginState, this->parent).release());
                        },
                        [this, &pluginState](auto &paramHolder) {
                            addSubItem(std::make_unique<ParameterGroupItem>(paramHolder, pluginState, this->parent).release());
                        });

            }

            bool mightContainSubItems() override { return getNumSubItems() > 0; }

            std::unique_ptr<juce::Component> createItemComponent() override {
                return std::make_unique<juce::Label>(name, name);
            }
            SynthSection &parent;
            juce::String name;
        };
    } // namespace parameters_view_detail

//==============================================================================
    struct ParametersView::Pimpl {
        Pimpl(chowdsp::ParamHolder &params, chowdsp::PluginState &pluginState, SynthSection& parent)
                :   groupItem(params, pluginState, parent){
            const auto numIndents = getNumIndents(groupItem);
            const auto width = 400 + view.getIndentSize() * numIndents;

            view.setSize(width, 400);
            view.setDefaultOpenness(true);
            view.setRootItemVisible(false);

        }

        static int getNumIndents(const juce::TreeViewItem &item) {
            int maxInner = 0;

            for (auto i = 0; i < item.getNumSubItems(); ++i)
                maxInner = juce::jmax(maxInner, 1 + getNumIndents(*item.getSubItem(i)));

            return maxInner;
        }
        // Public function definitions for the class, which override the base class methods for
        // initializing, rendering, resizing, and painting OpenGl components
//        void initOpenGlComponents(OpenGlWrapper &open_gl) override
//        {
//
//        }
//        void renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) override
//        {
//
//        }
        parameters_view_detail::ParameterGroupItem groupItem;
        juce::TreeView view;
    };

//==============================================================================
    ParametersView::ParametersView(chowdsp::PluginState &pluginState, chowdsp::ParamHolder &params, OpenGlWrapper *open_gl)
            : SynthSection("params", open_gl) , pimpl(std::make_unique<Pimpl>(params, pluginState, *this)) {
        auto *viewport = pimpl->view.getViewport();
        setLookAndFeel(DefaultLookAndFeel::instance());
        setOpaque(true);
        addAndMakeVisible(pimpl->view);

        setSize(viewport->getViewedComponent()->getWidth() + viewport->getVerticalScrollBar().getWidth(),
                juce::jlimit(125, 400, viewport->getViewedComponent()->getHeight()));
    }

    ParametersView::~ParametersView() = default;

    void ParametersView::paint(juce::Graphics &g) {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

    void ParametersView::resized() {
        pimpl->view.setBounds(getLocalBounds());
        SynthSection::resized();
    }

    void ParametersView::initOpenGlComponents(OpenGlWrapper &open_gl)
    {
        open_gl.context.executeOnGLThread([this,&open_gl](OpenGLContext &openGLContext)
        {
            for (auto& open_gl_component : open_gl_components_)
                open_gl_component->init(open_gl);

            for (auto& sub_section : sub_sections_)
                sub_section->initOpenGlComponents(open_gl);

            for (auto& open_gl_component : open_gl_components_)
                open_gl_component->setVisible(true);
            if(background_)
                background_->init(open_gl);
        }, false);

    }

    void ParametersView::renderOpenGlComponents(OpenGlWrapper& open_gl, bool animate) {
        for (auto& sub_section : sub_sections_) {
            if (sub_section->isVisible() && !sub_section->isAlwaysOnTop())
                sub_section->renderOpenGlComponents(open_gl, animate);
        }

        for (auto& open_gl_component : open_gl_components_) {
            if (open_gl_component->isVisible() && !open_gl_component->isAlwaysOnTop()) {
                open_gl_component->render(open_gl, animate);
                GLenum gl =  juce::gl::glGetError();
                //DBG(String(gl));
                BITKLAVIER_ASSERT(gl == juce::gl::GL_NO_ERROR);
            }
        }

        for (auto& sub_section : sub_sections_) {
            if (sub_section->isVisible() && sub_section->isAlwaysOnTop())
                sub_section->renderOpenGlComponents(open_gl, animate);
        }

        for (auto& open_gl_component : open_gl_components_) {
            if (open_gl_component->isVisible() && open_gl_component->isAlwaysOnTop()) {
                open_gl_component->render(open_gl, animate);
                BITKLAVIER_ASSERT(juce::gl::glGetError() == juce::gl::GL_NO_ERROR);
            }
        }
        if(background_)
            background_->render(open_gl);

    }
    void ParametersView::init_()
    {
        pimpl->view.setRootItem(&pimpl->groupItem);
    }
}//naemspace bitlkavier