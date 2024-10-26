//
// Created by Dan Trueman on 10/22/24.
//

#include "BKSliders.h"

/**
 * move this stuff somewhere else
 */

/*
static const int fontHeight = 15;
static const int gXSpacing = 4;

juce::Path getTickShape (float height)
{
    static const unsigned char pathData[] = { 110,109,32,210,202,64,126,183,148,64,108,39,244,247,64,245,76,124,64,108,178,131,27,65,246,76,252,64,108,175,242,4,65,246,76,252,
        64,108,236,5,68,65,0,0,160,180,108,240,150,90,65,21,136,52,63,108,48,59,16,65,0,0,32,65,108,32,210,202,64,126,183,148,64, 99,101,0,0 };

    juce::Path path;
    path.loadPathFromData (pathData, sizeof (pathData));
    path.scaleToFit (0, 0, height * 2.0f, height, true);

    return path;
}

void drawTickBox (juce::Graphics& g, juce::Component& component,
    float x, float y, float w, float h,
    const bool ticked,
    [[maybe_unused]] const bool isEnabled,
    [[maybe_unused]] const bool shouldDrawButtonAsHighlighted,
    [[maybe_unused]] const bool shouldDrawButtonAsDown)
{
    juce::Rectangle<float> tickBounds (x, y, w, h);

    g.setColour (component.findColour (juce::ToggleButton::tickDisabledColourId));
    g.drawRoundedRectangle (tickBounds, 4.0f, 1.0f);

    if (ticked)
    {
        g.setColour (component.findColour (juce::ToggleButton::tickColourId));
        auto tick = getTickShape (0.75f);
        g.fillPath (tick, tick.getTransformToScaleToFit (tickBounds.reduced (4, 5).toFloat(), false));
    }
}

void drawPointer (juce::Graphics& g, const float x, const float y, const float diameter,
    const juce::Colour& colour, const int direction) noexcept
{
    juce::Path p;
    p.startNewSubPath (x + diameter * 0.5f, y);
    p.lineTo (x + diameter, y + diameter * 0.6f);
    p.lineTo (x + diameter, y + diameter);
    p.lineTo (x, y + diameter);
    p.lineTo (x, y + diameter * 0.6f);
    p.closeSubPath();

    p.applyTransform (juce::AffineTransform::rotation ((float) direction * juce::MathConstants<float>::halfPi,
        x + diameter * 0.5f, y + diameter * 0.5f));
    g.setColour (colour);
    g.fillPath (p);
}
*/

/*
void BKButtonAndMenuLAF::positionComboBoxText (juce::ComboBox& box, juce::Label& label)
{
    label.setFont (getComboBoxFont (box));

    if(comboBoxJustification == juce::Justification::centredRight)
    {
        label.setBounds (30, 1,
            box.getWidth() - 30,
            box.getHeight() - 2);

        label.setJustificationType (juce::Justification::centredRight);
    }
    else
    {
        label.setBounds (1, 1,
            box.getWidth() - 30,
            box.getHeight() - 2);

        label.setJustificationType (juce::Justification::centredLeft);
    }
}

#define HEIGHT_IOS 22
#define HEIGHT 15
#define HORIZONTAL 0.75f
#define KERNING 0.25f
#define STYLE 0
#define IOS_RATIO 1.5f

juce::Font BKButtonAndMenuLAF::getComboBoxFont (juce::ComboBox& cb)
{
    juce::Font font;

    font.setSizeAndStyle(fontHeight, STYLE, HORIZONTAL, KERNING);

    return font;
}

juce::Font BKButtonAndMenuLAF::getPopupMenuFont (void)
{
    juce::Font font;

    font.setSizeAndStyle(fontHeight, STYLE, HORIZONTAL, KERNING);

    return font;
}

juce::Font BKButtonAndMenuLAF::getTextButtonFont (juce::TextButton&, int buttonHeight)
{
    juce::Font font;

    font.setSizeAndStyle(fontHeight, STYLE, HORIZONTAL, KERNING);

    return font;
}

juce::Font BKButtonAndMenuLAF::getLabelFont (juce::Label& label)
{
    juce::Font font;

    font.setSizeAndStyle(fontHeight, STYLE, HORIZONTAL, KERNING);

    return font;
}

void BKButtonAndMenuLAF::drawTooltip(juce::Graphics& g, const juce::String& text, int width, int height)
{
    juce::Rectangle<int> bounds (width, height);
    auto cornerSize = 0.0f;

    //g.setColour (findColour (juce::TooltipWindow::backgroundColourId));
    g.setColour(juce::Colours::black);
    g.setOpacity(0.0f);
    g.fillRoundedRectangle (bounds.toFloat(), cornerSize);

    //g.setColour (findColour (juce::TooltipWindow::outlineColourId));
    g.setColour(juce::Colours::black);
    g.setOpacity(0.0f);
    g.drawRoundedRectangle (bounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 1.0f);

    const float tooltipFontSize = 13.0f;
    const int maxToolTipWidth = 400;

    juce::AttributedString s;
    s.setJustification (juce::Justification::centred);
    //s.append (text, juce::Font (tooltipFontSize, juce::Font::bold), findColour (juce::TooltipWindow::textColourId));

    juce::TextLayout tl;
    tl.createLayoutWithBalancedLineLengths (s, (float) maxToolTipWidth);
    tl.draw (g, { static_cast<float> (width), static_cast<float> (height) });
}


#define MENUBARHEIGHT_IOS 40
#define MENUBARHEIGHT  25

int BKButtonAndMenuLAF::getDefaultMenuBarHeight()
{
#if JUCE_IOS
    return MENUBARHEIGHT_IOS;
#else
    return MENUBARHEIGHT;
#endif
}

#define CORNER_SIZE 2.0f

void BKButtonAndMenuLAF::drawComboBox (juce::Graphics& g, int width, int height, bool,
    int, int, int, int, juce::ComboBox& box)
{
    //const auto cornerSize = box.findParentComponentOfClass<ChoicePropertyComponent>() != nullptr ? 0.0f : 0.0f;
    const auto cornerSize = CORNER_SIZE;
    const juce::Rectangle<int> boxBounds (0, 0, width, height);

    g.setColour (box.findColour (juce::ComboBox::backgroundColourId));
    g.fillRoundedRectangle (boxBounds.toFloat(), cornerSize);

    g.setColour (box.findColour (juce::ComboBox::outlineColourId));
    g.drawRoundedRectangle (boxBounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 0.5f);

    juce::Rectangle<int> arrowZone (width - 30, 0, 20, height);
    if(comboBoxJustification == juce::Justification::centredRight)
        arrowZone = juce::Rectangle<int> (gXSpacing, 0, 20, height);

    juce::Path path;
    path.startNewSubPath (arrowZone.getX() + 3.0f, arrowZone.getCentreY() - 2.0f);
    path.lineTo (static_cast<float> (arrowZone.getCentreX()), arrowZone.getCentreY() + 3.0f);
    path.lineTo (arrowZone.getRight() - 3.0f, arrowZone.getCentreY() - 2.0f);

    g.setColour (box.findColour (juce::ComboBox::arrowColourId).withAlpha ((box.isEnabled() ? 0.9f : 0.2f)));
    g.strokePath (path, juce::PathStrokeType (2.0f));
}

void BKButtonAndMenuLAF::drawPopupMenuItem (juce::Graphics& g, const juce::Rectangle<int>& area,
    const bool isSeparator, const bool isActive,
    const bool isHighlighted, const bool isTicked,
    const bool hasSubMenu, const juce::String& text,
    const juce::String& shortcutKeyText,
    const juce::Drawable* icon, const juce::Colour* const textColourToUse)
{
    if (isSeparator)
    {
        auto r  = area.reduced (5, 0);
        r.removeFromTop (juce::roundToInt ((r.getHeight() * 0.5f) - 0.5f));

        //g.setColour (findColour (juce::PopupMenu::textColourId).withAlpha (0.3f));
        g.fillRect (r.removeFromTop (1));
    }
    else
    {
        //auto textColour = (textColourToUse == nullptr ? findColour (juce::PopupMenu::textColourId) : *textColourToUse);
        auto textColour = (textColourToUse == nullptr ? juce::Colours::black
                                                      : *textColourToUse);
        //Colours::black

        auto r  = area.reduced (1);

        if (isHighlighted && isActive)
        {
            //g.setColour (findColour (juce::PopupMenu::highlightedBackgroundColourId));
            g.setColour(juce::Colours::green);
            g.fillRect (r);

            //g.setColour (findColour (juce::PopupMenu::highlightedTextColourId));
        }
        else
        {
            g.setColour (textColour.withMultipliedAlpha (isActive ? 1.0f : 0.5f));
        }

        r.reduce (juce::jmin (5, area.getWidth() / 20), 0);

        auto font = getPopupMenuFont();

        const auto maxFontHeight = r.getHeight() / 1.3f;

        if (font.getHeight() > maxFontHeight)
            font.setHeight (maxFontHeight);

        g.setFont (font);

        auto iconArea = r.removeFromLeft (juce::roundToInt (maxFontHeight)).toFloat();

        if (icon != nullptr)
        {
            icon->drawWithin (g, iconArea, juce::RectanglePlacement::centred | juce::RectanglePlacement::onlyReduceInSize, 1.0f);
        }
        else if (isTicked)
        {
            const auto tick = getTickShape (1.0f);
            g.fillPath (tick, tick.getTransformToScaleToFit (iconArea.reduced (iconArea.getWidth() / 5, 0).toFloat(), true));
        }

        if (hasSubMenu)
        {
            const auto arrowH = 0.6f * getPopupMenuFont().getAscent();

            const auto x = (float) r.removeFromRight ((int) arrowH).getX();
            const auto halfH = (float) r.getCentreY();

            juce::Path path;
            path.startNewSubPath (x, halfH - arrowH * 0.5f);
            path.lineTo (x + arrowH * 0.6f, halfH);
            path.lineTo (x, halfH + arrowH * 0.5f);

            g.strokePath (path, juce::PathStrokeType (2.0f));
        }

        r.removeFromRight (3);
        if(comboBoxJustification == juce::Justification::centredRight)
            g.drawFittedText (text, r, juce::Justification::centredRight, 1);
        else
            g.drawFittedText (text, r, juce::Justification::centredLeft, 1);

        if (shortcutKeyText.isNotEmpty())
        {
            auto f2 = font;
            f2.setHeight (f2.getHeight() * 0.75f);
            f2.setHorizontalScale (0.95f);
            g.setFont (f2);

            if(comboBoxJustification == juce::Justification::centredRight)
                g.drawText (shortcutKeyText, r, juce::Justification::centredRight, true);
            else
                g.drawText (shortcutKeyText, r, juce::Justification::centredLeft, true);
        }
    }
}

void BKButtonAndMenuLAF::drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
    bool isMouseOverButton, bool isButtonDown)
{
    const auto cornerSize = CORNER_SIZE;
    const auto bounds = button.getLocalBounds().toFloat().reduced (0.5f, 0.5f);

    auto baseColour = backgroundColour.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
                          .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f);

    if (isButtonDown || isMouseOverButton)
        baseColour = baseColour.contrasting (isButtonDown ? 0.2f : 0.05f);

    g.setColour (baseColour);

    if (button.isConnectedOnLeft() || button.isConnectedOnRight())
    {
        juce::Path path;
        path.addRoundedRectangle (bounds.getX(), bounds.getY(),
            bounds.getWidth(), bounds.getHeight(),
            cornerSize, cornerSize,
            ! button.isConnectedOnLeft(),
            ! button.isConnectedOnRight(),
            ! button.isConnectedOnLeft(),
            ! button.isConnectedOnRight());

        g.fillPath (path);

        g.setColour (button.findColour (juce::ComboBox::outlineColourId));
        g.strokePath (path, juce::PathStrokeType (0.5f));
    }
    else
    {
        g.fillRoundedRectangle (bounds, cornerSize);

        g.setColour (button.findColour (juce::ComboBox::outlineColourId));
        g.drawRoundedRectangle (bounds, cornerSize, 0.5f);
    }
}

void BKButtonAndMenuLAF::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button,
    bool isMouseOverButton, bool isButtonDown)
{
    if(toggleTextToRight)
    {
        const auto fontSize = juce::jmin (15.0f, button.getHeight() * 0.75f);
        const auto tickWidth = fontSize * 1.1f;

        drawTickBox (g, button, 4.0f, (button.getHeight() - tickWidth) * 0.5f,
            tickWidth, tickWidth,
            button.getToggleState(),
            button.isEnabled(),
            isMouseOverButton,
            isButtonDown);

        g.setColour (button.findColour (juce::ToggleButton::textColourId));
        g.setFont (fontSize);
        juce::Font toggleFont;
        toggleFont.setSizeAndStyle(fontHeight, STYLE, HORIZONTAL, KERNING);
        g.setFont (toggleFont);

        if (! button.isEnabled())
            g.setOpacity (0.5f);

        const auto textX = juce::roundToInt (tickWidth) + 10;

        g.drawFittedText (button.getButtonText(),
            textX, 0,
            button.getWidth() - textX - 2, button.getHeight(),
            juce::Justification::centredLeft, 10);
    }
    else
    {
        const auto fontSize = juce::jmin (15.0f, button.getHeight() * 0.75f);
        const auto tickWidth = fontSize * 1.1f;

        drawTickBox (g, button,
            (button.getWidth() - tickWidth - 2),
            (button.getHeight() - tickWidth) * 0.5f,
            tickWidth, tickWidth,
            button.getToggleState(),
            button.isEnabled(),
            isMouseOverButton,
            isButtonDown);

        g.setColour (button.findColour (juce::ToggleButton::textColourId));
        g.setFont (fontSize);
        juce::Font toggleFont;
        toggleFont.setSizeAndStyle(fontHeight, STYLE, HORIZONTAL, KERNING);
        g.setFont (toggleFont);

        if (! button.isEnabled())
            g.setOpacity (0.5f);

        const auto textX = juce::roundToInt (tickWidth) + 10;

        g.drawFittedText (button.getButtonText(),
            0,
            0,
            button.getWidth() - textX - 2,
            button.getHeight(),
            juce::Justification::centredRight, 10);

    }
}

void BKButtonAndMenuLAF::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos,
    float minSliderPos,
    float maxSliderPos,
    const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    if (slider.isBar())
    {
        g.setColour (slider.findColour (juce::Slider::trackColourId));
        g.fillRect (slider.isHorizontal() ? juce::Rectangle<float> (static_cast<float> (x), y + 0.5f, sliderPos - x, height - 1.0f)
                                          : juce::Rectangle<float> (x + 0.5f, sliderPos, width - 1.0f, y + (height - sliderPos)));
    }
    else
    {
        const auto isTwoVal   = (style == juce::Slider::SliderStyle::TwoValueVertical   || style == juce::Slider::SliderStyle::TwoValueHorizontal);
        const auto isThreeVal = (style == juce::Slider::SliderStyle::ThreeValueVertical || style == juce::Slider::SliderStyle::ThreeValueHorizontal);

        const auto trackWidth = juce::jmin (6.0f, slider.isHorizontal() ? height * 0.25f : width * 0.25f);

        const juce::Point<float> startPoint (slider.isHorizontal() ? x : x + width * 0.5f,
            slider.isHorizontal() ? y + height * 0.5f : height + y);

        const juce::Point<float> endPoint (slider.isHorizontal() ? width + x : startPoint.x,
            slider.isHorizontal() ? startPoint.y : y);

        juce::Path backgroundTrack;
        backgroundTrack.startNewSubPath (startPoint);
        backgroundTrack.lineTo (endPoint);
        g.setColour (slider.findColour (juce::Slider::backgroundColourId));
        g.strokePath (backgroundTrack, juce::PathStrokeType (trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        juce::Path valueTrack;
        juce::Point<float> minPoint, maxPoint, thumbPoint;

        if (isTwoVal || isThreeVal)
        {
            minPoint = { slider.isHorizontal() ? minSliderPos : width * 0.5f,
                slider.isHorizontal() ? height * 0.5f : minSliderPos };

            if (isThreeVal)
                thumbPoint = { slider.isHorizontal() ? sliderPos : width * 0.5f,
                    slider.isHorizontal() ? height * 0.5f : sliderPos };

            maxPoint = { slider.isHorizontal() ? maxSliderPos : width * 0.5f,
                slider.isHorizontal() ? height * 0.5f : maxSliderPos };
        }
        else
        {
            const auto kx = slider.isHorizontal() ? sliderPos : (x + width * 0.5f);
            const auto ky = slider.isHorizontal() ? (y + height * 0.5f) : sliderPos;

            minPoint = startPoint;
            maxPoint = { kx, ky };
        }

        const auto thumbWidth = trackWidth * 2.0f;

        valueTrack.startNewSubPath (minPoint);
        valueTrack.lineTo (isThreeVal ? thumbPoint : maxPoint);
        g.setColour (slider.findColour (juce::Slider::trackColourId));
        g.strokePath (valueTrack, juce::PathStrokeType (trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        if (! isTwoVal)
        {
            g.setColour (slider.findColour (juce::Slider::thumbColourId));
            g.fillEllipse (juce::Rectangle<float> (thumbWidth, thumbWidth).withCentre (isThreeVal ? thumbPoint : maxPoint));
        }

        if (isTwoVal || isThreeVal)
        {
            const auto sr = juce::jmin (trackWidth, (slider.isHorizontal() ? height : width) * 0.4f);
            const auto pointerColour = slider.findColour (juce::Slider::thumbColourId);

            if (slider.isHorizontal())
            {
                drawPointer (g, minSliderPos - sr,
                    juce::jmax (0.0f, y + height * 0.5f - trackWidth * 2.0f),
                    trackWidth * 2.0f, pointerColour, 2);

                drawPointer (g, maxSliderPos - trackWidth,
                    juce::jmin (y + height - trackWidth * 2.0f, y + height * 0.5f),
                    trackWidth * 2.0f, pointerColour, 4);
            }
            else
            {
                drawPointer (g, juce::jmax (0.0f, x + width * 0.5f - trackWidth * 2.0f),
                    minSliderPos - trackWidth,
                    trackWidth * 2.0f, pointerColour, 1);

                drawPointer (g, juce::jmin (x + width - trackWidth * 2.0f, x + width * 0.5f), maxSliderPos - sr,
                    trackWidth * 2.0f, pointerColour, 3);
            }
        }
    }
}
 */

/*
int getSliderThumbRadius (juce::Slider& slider)
{
    return juce::jmin (12, slider.isHorizontal() ? static_cast<int> ((float) slider.getHeight() * 0.5f)
                                           : static_cast<int> ((float) slider.getWidth()  * 0.5f));
}

static juce::Colour createBaseColour (juce::Colour buttonColour,
    bool hasKeyboardFocus,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown) noexcept
{
    const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
    const juce::Colour baseColour (buttonColour.withMultipliedSaturation (sat));

    if (shouldDrawButtonAsDown)        return baseColour.contrasting (0.2f);
    if (shouldDrawButtonAsHighlighted) return baseColour.contrasting (0.1f);

    return baseColour;
}

void drawGlassSphere (juce::Graphics& g, const float x, const float y,
    const float diameter, const juce::Colour& colour,
    const float outlineThickness) noexcept
{
    if (diameter <= outlineThickness)
        return;

    juce::Path p;
    p.addEllipse (x, y, diameter, diameter);

    {
        juce::ColourGradient cg (juce::Colours::white.overlaidWith (colour.withMultipliedAlpha (0.3f)), 0, y,
            juce::Colours::white.overlaidWith (colour.withMultipliedAlpha (0.3f)), 0, y + diameter, false);

        cg.addColour (0.4, juce::Colours::white.overlaidWith (colour));

        g.setGradientFill (cg);
        g.fillPath (p);
    }

    g.setGradientFill (juce::ColourGradient (juce::Colours::white, 0, y + diameter * 0.06f,
        juce::Colours::transparentWhite, 0, y + diameter * 0.3f, false));
    g.fillEllipse (x + diameter * 0.2f, y + diameter * 0.05f, diameter * 0.6f, diameter * 0.4f);

    juce::ColourGradient cg (juce::Colours::transparentBlack,
        x + diameter * 0.5f, y + diameter * 0.5f,
        juce::Colours::black.withAlpha (0.5f * outlineThickness * colour.getFloatAlpha()),
        x, y + diameter * 0.5f, true);

    cg.addColour (0.7, juce::Colours::transparentBlack);
    cg.addColour (0.8, juce::Colours::black.withAlpha (0.1f * outlineThickness));

    g.setGradientFill (cg);
    g.fillPath (p);

    g.setColour (juce::Colours::black.withAlpha (0.5f * colour.getFloatAlpha()));
    g.drawEllipse (x, y, diameter, diameter, outlineThickness);
}

//==============================================================================
void drawGlassPointer (juce::Graphics& g,
    const float x, const float y, const float diameter,
    const juce::Colour& colour, const float outlineThickness,
    const int direction) noexcept
{
    if (diameter <= outlineThickness)
        return;

    juce::Path p;
    p.startNewSubPath (x + diameter * 0.5f, y);
    p.lineTo (x + diameter, y + diameter * 0.6f);
    p.lineTo (x + diameter, y + diameter);
    p.lineTo (x, y + diameter);
    p.lineTo (x, y + diameter * 0.6f);
    p.closeSubPath();

    p.applyTransform (juce::AffineTransform::rotation ((float) direction * juce::MathConstants<float>::halfPi,
        x + diameter * 0.5f,
        y + diameter * 0.5f));

    {
        juce::ColourGradient cg (juce::Colours::white.overlaidWith (colour.withMultipliedAlpha (0.3f)), 0, y,
            juce::Colours::white.overlaidWith (colour.withMultipliedAlpha (0.3f)), 0, y + diameter, false);

        cg.addColour (0.4, juce::Colours::white.overlaidWith (colour));

        g.setGradientFill (cg);
        g.fillPath (p);
    }

    juce::ColourGradient cg (juce::Colours::transparentBlack,
        x + diameter * 0.5f, y + diameter * 0.5f,
        juce::Colours::black.withAlpha (0.5f * outlineThickness * colour.getFloatAlpha()),
        x - diameter * 0.2f, y + diameter * 0.5f, true);

    cg.addColour (0.5, juce::Colours::transparentBlack);
    cg.addColour (0.7, juce::Colours::black.withAlpha (0.07f * outlineThickness));

    g.setGradientFill (cg);
    g.fillPath (p);

    g.setColour (juce::Colours::black.withAlpha (0.5f * colour.getFloatAlpha()));
    g.strokePath (p, juce::PathStrokeType (outlineThickness));
}


void drawLinearSliderThumb (juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    auto sliderRadius = (float) (getSliderThumbRadius (slider) - 2);

    //juce::Colours::black
    auto knobColour = createBaseColour (slider.findColour (juce::Slider::thumbColourId),
        slider.hasKeyboardFocus (false) && slider.isEnabled(),
        slider.isMouseOverOrDragging() && slider.isEnabled(),
        slider.isMouseButtonDown() && slider.isEnabled());


    const float outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;

    if (style == juce::Slider::LinearHorizontal || style == juce::Slider::LinearVertical)
    {
        float kx, ky;

        if (style == juce::Slider::LinearVertical)
        {
            kx = (float) x + (float) width * 0.5f;
            ky = sliderPos;
        }
        else
        {
            kx = sliderPos;
            ky = (float) y + (float) height * 0.5f;
        }

        drawGlassSphere (g,
            kx - sliderRadius,
            ky - sliderRadius,
            sliderRadius * 2.0f,
            knobColour,
            outlineThickness);

    }
    else
    {
        if (style == juce::Slider::ThreeValueVertical)
        {
            drawGlassSphere (g, (float) x + (float) width * 0.5f - sliderRadius,
                sliderPos - sliderRadius,
                sliderRadius * 2.0f,
                knobColour, outlineThickness);
        }
        else if (style == juce::Slider::ThreeValueHorizontal)
        {
            drawGlassSphere (g,sliderPos - sliderRadius,
                (float) y + (float) height * 0.5f - sliderRadius,
                sliderRadius * 2.0f,
                knobColour, outlineThickness);
        }

        if (style == juce::Slider::TwoValueVertical || style == juce::Slider::ThreeValueVertical)
        {
            auto sr = juce::jmin (sliderRadius, (float) width * 0.4f);

            drawGlassPointer (g, juce::jmax (0.0f, (float) x + (float) width * 0.5f - sliderRadius * 2.0f),
                minSliderPos - sliderRadius,
                sliderRadius * 2.0f, knobColour, outlineThickness, 1);

            drawGlassPointer (g,
                juce::jmin ((float) x + (float) width - sliderRadius * 2.0f,
                    (float) x + (float) width * 0.5f),
                maxSliderPos - sr,
                sliderRadius * 2.0f,
                knobColour,
                outlineThickness,
                3);
        }
        else if (style == juce::Slider::TwoValueHorizontal || style == juce::Slider::ThreeValueHorizontal)
        {
            auto sr = juce::jmin (sliderRadius, (float) height * 0.4f);

            drawGlassPointer (g, minSliderPos - sr,
                juce::jmax (0.0f, (float) y + (float) height * 0.5f - sliderRadius * 2.0f),
                sliderRadius * 2.0f, knobColour, outlineThickness, 2);

            drawGlassPointer (g,
                maxSliderPos - sliderRadius,
                juce::jmin ((float) y + (float) height - sliderRadius * 2.0f,
                    (float) y + (float) height * 0.5f),
                sliderRadius * 2.0f,
                knobColour,
                outlineThickness,
                4);
        }
    }
}
*/

/*
void BKMultiSliderLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    g.fillAll (slider.findColour (juce::Slider::backgroundColourId));

    if (style == juce::Slider::LinearBar || style == juce::Slider::LinearBarVertical)
    {
        const float fx = (float) x, fy = (float) y, fw = (float) width, fh = (float) height;

        juce::Path p;

        if (style == juce::Slider::LinearBarVertical)
            p.addRectangle (fx, sliderPos - 2, fw, 4); //band instead of bar, width of 4
        else
            //p.addRectangle (fx, fy, sliderPos - fx, fh);
            p.addRectangle (sliderPos - 2, fy, 4 , fh);

        juce::Colour baseColour (slider.findColour (juce::Slider::thumbColourId)
                               .withMultipliedSaturation (slider.isEnabled() ? 1.0f : 0.5f)
                               .withMultipliedAlpha (0.8f));

        g.setGradientFill (juce::ColourGradient (baseColour.brighter (0.08f), 0.0f, 0.0f,
            baseColour.darker (0.08f), 0.0f, (float) height, false));
        g.fillPath (p);

        g.setColour (baseColour.darker (0.2f));


        if (style == juce::Slider::LinearBarVertical)
            g.fillRect (fx, sliderPos, fw, 1.0f);
        else
            g.fillRect (sliderPos, fy, 1.0f, fh);

    }
    else
    {
        drawLinearSliderBackground (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        drawLinearSliderThumb (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    }
}
*/

juce::String BKfloatArrayToString(juce::Array<float> arr)
{
    juce::String s = "";
    for (auto key : arr)
    {
        s.append(juce::String(key), 6);
        s.append(" ", 1);
    }
    return s;
}

juce::Array<float> BKstringToFloatArray(juce::String s)
{
    juce::Array<float> arr = juce::Array<float>();

    juce::String temp = "";
    bool inNumber = false;

    juce::String::CharPointerType c = s.getCharPointer();

    juce::juce_wchar prd = '.';
    juce::juce_wchar dash = '-';
    juce::juce_wchar slash = '/'; // blank: put a zero in

    int prdCnt = 0;

    // DEBUG
    for (int i = 0; i < (s.length()+1); i++)
    {
        juce::juce_wchar c1 = c.getAndAdvance();

        bool isPrd = !juce::CharacterFunctions::compare(c1, prd);
        bool isDash = !juce::CharacterFunctions::compare(c1, dash);
        bool isSlash = !juce::CharacterFunctions::compare(c1, slash);

        if (isPrd) prdCnt += 1;

        bool isNumChar = juce::CharacterFunctions::isDigit(c1) || isPrd || isDash;

        if (!isNumChar)
        {
            if (inNumber)
            {
                arr.add(temp.getFloatValue());
                temp = "";
            }

            // slash indicates a zero slot
            if (isSlash) {
                arr.add(0.);
                temp = "";
            }

            inNumber = false;
            continue;
        }
        else
        {
            inNumber = true;

            temp += c1;
        }
    }

    return arr;
}


BKStackedSlider::BKStackedSlider(
    juce::String sliderName,
    double min,
    double max,
    double defmin,
    double defmax,
    double def,
    double increment):
           sliderName(sliderName),
           sliderMin(min),
           sliderMax(max),
           sliderMinDefault(defmin),
           sliderMaxDefault(defmax),
           sliderDefault(def),
           sliderIncrement(increment)
{

    showName.setText(sliderName, juce::dontSendNotification);
    showName.setInterceptsMouseClicks(false, true);
    addAndMakeVisible(showName);

    editValsTextField = std::make_unique<juce::TextEditor>();
    editValsTextField->setMultiLine(true);
    editValsTextField->setName("PARAMTXTEDIT");
    editValsTextField->addListener(this);
    editValsTextField->setColour(juce::TextEditor::highlightColourId, juce::Colours::darkgrey);
#if JUCE_IOS
    editValsTextField->setReadOnly(true);
    editValsTextField->setCaretVisible(true);
#endif
    addAndMakeVisible(*editValsTextField);
    editValsTextField->setVisible(false);

    numSliders = 12;
    numActiveSliders = 1;

    for(int i=0; i<numSliders; i++)
    {
        dataSliders.add(new juce::Slider);
        juce::Slider* newSlider = dataSliders.getLast();

        newSlider->setSliderStyle(juce::Slider::LinearBar);
        newSlider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        newSlider->setRange(sliderMin, sliderMax, sliderIncrement);
        newSlider->setValue(sliderDefault, juce::dontSendNotification);
        //newSlider->setLookAndFeel(&stackedSliderLookAndFeel);
        //newSlider->addListener(this);
        addAndMakeVisible(newSlider);
        if(i>0) {
            newSlider->setVisible(false);
            activeSliders.insert(i, false);
        }
        else activeSliders.insert(0, true);
    }

    topSlider = std::make_unique<juce::Slider>();
    topSlider->setSliderStyle(juce::Slider::LinearBar);
    topSlider->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxLeft, true, 0,0);
    //topSlider->setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, true, 50,50);
    topSlider->setRange(sliderMin, sliderMax, sliderIncrement);
    topSlider->setValue(sliderDefault, juce::dontSendNotification);
    topSlider->addListener(this);
    topSlider->addMouseListener(this, true);
    //topSlider->setLookAndFeel(&topSliderLookAndFeel);
    topSlider->setAlpha(0.);
    addAndMakeVisible(*topSlider);

    //topSliderLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colour::greyLevel (0.8f).contrasting().withAlpha (0.0f));
    //stackedSliderLookAndFeel.setColour(juce::Slider::thumbColourId, juce::Colours::goldenrod.withMultipliedAlpha(0.95));

}

void BKStackedSlider::setDim(float alphaVal)
{
    showName.setAlpha(alphaVal);
    //topSlider->setAlpha(alphaVal);

    for(int i=0; i<numSliders; i++)
    {
        juce::Slider* newSlider = dataSliders.operator[](i);
        if(newSlider != nullptr)
        {
            if(activeSliders.getUnchecked(i))
            {
                newSlider->setAlpha(alphaVal);
            }
        }
    }
}

void BKStackedSlider::setBright()
{
    showName.setAlpha(1.);
    //topSlider->setAlpha(1.);

    for(int i=0; i<numSliders; i++)
    {
        juce::Slider* newSlider = dataSliders.operator[](i);
        if(newSlider != nullptr)
        {
            if(activeSliders.getUnchecked(i))
            {
                newSlider->setAlpha(1.);
            }
        }
    }
}

void BKStackedSlider::sliderValueChanged (juce::Slider *slider)
{

}

void BKStackedSlider::addSlider(juce::NotificationType newnotify)
{
    juce::Array<float> sliderVals = getAllActiveValues();
    //sliderVals.add(sliderDefault);
    sliderVals.add(topSlider->proportionOfLengthToValue((double)clickedPosition / getWidth()));
    setTo(sliderVals, newnotify);
    topSlider->setValue(topSlider->proportionOfLengthToValue((double)clickedPosition / getWidth()), juce::dontSendNotification);

    listeners.call(&BKStackedSlider::Listener::BKStackedSliderValueChanged,
        getName(),
        getAllActiveValues());
}

void BKStackedSlider::setTo(juce::Array<float> newvals, juce::NotificationType newnotify)
{

    int slidersToActivate = newvals.size();
    if(slidersToActivate > numSliders) slidersToActivate = numSliders;
    if(slidersToActivate <= 0)
    {
        slidersToActivate = 1;
        newvals.add(sliderDefault);
    }

    //activate sliders
    for(int i=0; i<slidersToActivate; i++)
    {

        juce::Slider* newSlider = dataSliders.operator[](i);
        if(newSlider != nullptr)
        {
            if(newvals.getUnchecked(i) > sliderMax)
                newSlider->setRange(sliderMin, newvals.getUnchecked(i), sliderIncrement);

            if(newvals.getUnchecked(i) < sliderMin)
                newSlider->setRange(newvals.getUnchecked(i), sliderMax, sliderIncrement);

            newSlider->setValue(newvals.getUnchecked(i));
            newSlider->setVisible(true);
        }

        activeSliders.set(i, true);
    }

    //deactivate unused sliders
    for(int i=slidersToActivate; i<numSliders; i++)
    {

        juce::Slider* newSlider = dataSliders.operator[](i);
        if(newSlider != nullptr)
        {
            newSlider->setValue(sliderDefault);
            newSlider->setVisible(false);
        }

        activeSliders.set(i, false);
    }

    //make sure there is one!
    if(slidersToActivate <= 0)
    {
        dataSliders.getFirst()->setValue(sliderDefault);
        activeSliders.set(0, true);
    }

    resetRanges();

    topSlider->setValue(dataSliders.getFirst()->getValue(), juce::dontSendNotification);
}


void BKStackedSlider::mouseDown (const juce::MouseEvent &event)
{
    if(event.mouseWasClicked())
    {
        clickedSlider = whichSlider();
        clickedPosition = event.x;

        mouseJustDown = true;

        if(event.mods.isCtrlDown())
        {
            showModifyPopupMenu();
        }
    }
}


void BKStackedSlider::mouseDrag(const juce::MouseEvent& e)
{
    //DBG("BKStacked Slider: mouseDrag");
    if(!mouseJustDown)
    {
        juce::Slider* currentSlider = dataSliders.operator[](clickedSlider);
        if(currentSlider != nullptr)
        {
            if(e.mods.isShiftDown())
            {
                currentSlider->setValue(round(topSlider->getValue()));
                topSlider->setValue(round(topSlider->getValue()));
            }
            else {
                currentSlider->setValue(topSlider->getValue(), juce::sendNotification);
                DBG("slider val = " + juce::String(topSlider->getValue()));
            }
        }
    }
    else mouseJustDown = false;
}

void BKStackedSlider::mouseUp(const juce::MouseEvent& e)
{
    DBG("BKStackedSlider::mouseUp");

    listeners.call(&BKStackedSlider::Listener::BKStackedSliderValueChanged,
        getName(),
        getAllActiveValues());

}

void BKStackedSlider::mouseMove(const juce::MouseEvent& e)
{
    //topSlider->setValue(topSlider->proportionOfLengthToValue((double)e.x / getWidth()), dontSendNotification);
    topSlider->setValue(dataSliders.getUnchecked(whichSlider(e))->getValue());

    for(int i=0; i<dataSliders.size(); i++)
    {
        if(dataSliders.getUnchecked(whichSlider(e)) == dataSliders.getUnchecked(i))
            dataSliders.getUnchecked(i)->setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 50);
        else
            dataSliders.getUnchecked(i)->setTextBoxStyle(juce::Slider::NoTextBox, false, 50, 50);
    }
}


void BKStackedSlider::mouseDoubleClick (const juce::MouseEvent &e)
{
    //highlight number for current slider

#if JUCE_IOS
    hasBigOne = true;
    editValsTextField->setText(floatArrayToString(getAllActiveValues()), dontSendNotification);
    WantsBigOne::listeners.call(&WantsBigOne::Listener::iWantTheBigOne, editValsTextField.get(), sliderName);
#else
    juce::StringArray tokens;
    tokens.addTokens(BKfloatArrayToString(getAllActiveValues()), false); //arrayFloatArrayToString
    int startPoint = 0;
    int endPoint;

    int which = whichSlider();

    for(int i=0; i < which; i++) {
        startPoint += tokens[i].length() + 1;
    }
    endPoint = startPoint + tokens[which].length();

    editValsTextField->setVisible(true);
    editValsTextField->toFront(true);
    editValsTextField->grabKeyboardFocus();
    editValsTextField->setText(BKfloatArrayToString(getAllActiveValues()), juce::dontSendNotification); //arrayFloatArrayToString

    juce::Range<int> highlightRange(startPoint, endPoint);
    editValsTextField->setHighlightedRegion(highlightRange);

    focusLostByEscapeKey = false;
#endif

}

void BKStackedSlider::textEditorReturnKeyPressed(juce::TextEditor& textEditor)
{
    if(textEditor.getName() == editValsTextField->getName())
    {
        editValsTextField->setVisible(false);
        editValsTextField->toBack();

        setTo(BKstringToFloatArray(textEditor.getText()), juce::sendNotification);
        clickedSlider = 0;
        resized();

        listeners.call(&BKStackedSlider::Listener::BKStackedSliderValueChanged,
            getName(),
            getAllActiveValues());

    }
}

void BKStackedSlider::textEditorFocusLost(juce::TextEditor& textEditor)
{
#if !JUCE_IOS
    if(!focusLostByEscapeKey) {
        textEditorReturnKeyPressed(textEditor);
    }
#endif
}

void BKStackedSlider::textEditorEscapeKeyPressed (juce::TextEditor& textEditor)
{
    if(textEditor.getName() == editValsTextField->getName())
    {
        focusLostByEscapeKey = true;
        editValsTextField->setVisible(false);
        editValsTextField->toBack();
        unfocusAllComponents();
    }
}

void BKStackedSlider::textEditorTextChanged(juce::TextEditor& textEditor)
{
#if JUCE_IOS
    if (hasBigOne)
    {
        hasBigOne = false;
        textEditorReturnKeyPressed(textEditor);
    }
#endif
}

void BKStackedSlider::resetRanges()
{

    double sliderMinTemp = sliderMinDefault;
    double sliderMaxTemp = sliderMaxDefault;

    for(int i = 0; i<dataSliders.size(); i++)
    {
        juce::Slider* currentSlider = dataSliders.operator[](i);
        if(currentSlider != nullptr)
        {
            if(currentSlider->getValue() > sliderMaxTemp) sliderMaxTemp = currentSlider->getValue();
            if(currentSlider->getValue() < sliderMinTemp) sliderMinTemp = currentSlider->getValue();
        }
    }

    if( (sliderMax != sliderMaxTemp) || sliderMin != sliderMinTemp)
    {
        sliderMax = sliderMaxTemp;
        sliderMin = sliderMinTemp;

        for(int i = 0; i<dataSliders.size(); i++)
        {
            juce::Slider* currentSlider = dataSliders.operator[](i);
            if(currentSlider != nullptr)
            {
                currentSlider->setRange(sliderMin, sliderMax, sliderIncrement);
            }
        }

        topSlider->setRange(sliderMin, sliderMax, sliderIncrement);
    }
}


juce::Array<float> BKStackedSlider::getAllActiveValues()
{
    juce::Array<float> currentVals;

    for(int i=0; i<dataSliders.size(); i++)
    {
        juce::Slider* currentSlider = dataSliders.operator[](i);
        if(currentSlider != nullptr)
        {
            if(activeSliders.getUnchecked(i))
                currentVals.add(currentSlider->getValue());
        }

    }

    return currentVals;
}

int BKStackedSlider::whichSlider()
{
    float refDistance;
    int whichSub = 0;

    juce::Slider* refSlider = dataSliders.getFirst();
    refDistance = fabs(refSlider->getValue() - topSlider->getValue());

    for(int i=1; i<dataSliders.size(); i++)
    {
        if(activeSliders.getUnchecked(i))
        {
            juce::Slider* currentSlider = dataSliders.operator[](i);
            if(currentSlider != nullptr) {
                float tempDistance = fabs(currentSlider->getValue() - topSlider->getValue());
                if(tempDistance < refDistance)
                {
                    whichSub = i;
                    refDistance = tempDistance;
                }
            }
        }
    }
    //DBG("whichSlider = " + String(whichSub));

    return whichSub;
}

int BKStackedSlider::whichSlider(const juce::MouseEvent& e)
{
    float refDistance;
    float topSliderVal = topSlider->proportionOfLengthToValue((double)e.x / getWidth());

    int whichSub = 0;

    juce::Slider* refSlider = dataSliders.getFirst();
    refDistance = fabs(refSlider->getValue() - topSliderVal);

    for(int i=1; i<dataSliders.size(); i++)
    {
        if(activeSliders.getUnchecked(i))
        {
            juce::Slider* currentSlider = dataSliders.operator[](i);
            if(currentSlider != nullptr) {
                float tempDistance = fabs(currentSlider->getValue() - topSliderVal);
                if(tempDistance < refDistance)
                {
                    whichSub = i;
                    refDistance = tempDistance;
                }
            }
        }
    }
    //DBG("whichSlider = " + String(whichSub));

    return whichSub;
}

void BKStackedSlider::showModifyPopupMenu()
{
    juce::PopupMenu m;
    m.addItem (1, juce::translate ("add transposition"), true, false);
    m.addSeparator();

    m.showMenuAsync (juce::PopupMenu::Options(),
        juce::ModalCallbackFunction::forComponent (sliderModifyMenuCallback, this));
}

void BKStackedSlider::sliderModifyMenuCallback (const int result, BKStackedSlider* ss)
{
    if (ss == nullptr)
    {
        juce::PopupMenu::dismissAllActiveMenus();
        return;
    }

    switch (result)
    {
        case 1:   ss->addSlider(juce::sendNotification); break;

        default:  break;
    }
}


void BKStackedSlider::resized ()
{
    juce::Rectangle<int> area (getLocalBounds());

    showName.setBounds(area.toNearestInt());
    showName.setJustificationType(juce::Justification::topRight);
    showName.toFront(false);

    topSlider->setBounds(area);

    editValsTextField->setBounds(area);
    editValsTextField->setVisible(false);

    for(int i=0; i<numSliders; i++)
    {
        juce::Slider* newSlider = dataSliders.getUnchecked(i);
        newSlider->setBounds(area);
    }

}
