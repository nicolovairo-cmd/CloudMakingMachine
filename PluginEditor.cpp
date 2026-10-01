#include "PluginEditor.h"
#include <BinaryData.h>

class CloudMakingMachineAudioProcessorEditor::LookAndFeel final : public juce::LookAndFeel_V4
{
public:
    LookAndFeel()
    {
        setColour(juce::Slider::thumbColourId, juce::Colours::white);
        setColour(juce::Slider::trackColourId, juce::Colours::transparentBlack);
        setColour(juce::Slider::backgroundColourId, juce::Colours::transparentBlack);
    }

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float, float, juce::Slider::SliderStyle style,
                          juce::Slider&) override
    {
        const bool vertical = style == juce::Slider::LinearVertical;
        const auto cyan = juce::Colour::fromRGB(20, 220, 226);
        const auto cyanBright = juce::Colour::fromRGB(38, 238, 244);
        const auto dark = juce::Colour::fromRGB(12, 48, 52);
        const auto shadow = juce::Colour::fromRGB(4, 30, 33);
        const auto white = juce::Colour::fromRGB(225, 247, 246);

        g.setImageResamplingQuality(juce::Graphics::lowResamplingQuality);

        if (vertical)
        {
            const int cx = x + width / 2;
            const int top = y + 6;
            const int bottom = y + height - 6;

            g.setColour(shadow);
            g.fillRect(cx - 8, top + 2, 16, bottom - top);
            g.setColour(dark);
            g.fillRect(cx - 6, top, 12, bottom - top);

            const int segments = 18;
            const int gap = 2;
            const float usable = static_cast<float>(bottom - top - 8);
            const float segmentH = (usable - gap * (segments - 1)) / segments;
            const float normalized = juce::jmap(sliderPos, static_cast<float>(bottom),
                                                static_cast<float>(top));

            for (int i = 0; i < segments; ++i)
            {
                const float yy = bottom - 4.0f - (i + 1) * segmentH - i * gap;
                g.setColour(((segments - i) / static_cast<float>(segments)) <= normalized
                                ? cyanBright : juce::Colour::fromRGB(20, 70, 73));
                g.fillRect(juce::Rectangle<float>(static_cast<float>(cx - 5), yy,
                                                   10.0f, segmentH));
            }

            const int thumbY = juce::jlimit(top + 10, bottom - 10, juce::roundToInt(sliderPos));
            g.setColour(shadow);
            g.fillRect(cx - 12, thumbY - 12, 24, 24);
            g.setColour(white);
            g.fillRect(cx - 9, thumbY - 9, 18, 18);
            g.setColour(cyan);
            g.fillRect(cx - 6, thumbY - 6, 12, 12);
        }
        else
        {
            const int cy = y + height / 2;
            const int left = x + 4;
            const int right = x + width - 4;

            g.setColour(shadow);
            g.fillRoundedRectangle((float) left + 2.0f, (float) cy - 4.0f,
                                   (float) (right - left), 10.0f, 3.0f);
            g.setColour(dark);
            g.fillRoundedRectangle((float) left, (float) cy - 6.0f,
                                   (float) (right - left), 10.0f, 3.0f);

            const int fillRight = juce::roundToInt(juce::jmap(sliderPos,
                                                              (float) left,
                                                              (float) right));
            g.setColour(cyanBright);
            if (fillRight > left + 2)
                g.fillRoundedRectangle((float) left + 2.0f, (float) cy - 4.0f,
                                       (float) (fillRight - left - 2), 6.0f, 2.0f);

            g.setColour(shadow);
            g.fillRect(fillRight - 8, cy - 11, 16, 22);
            g.setColour(white);
            g.fillRect(fillRight - 6, cy - 9, 12, 18);
            g.setColour(cyan);
            g.fillRect(fillRight - 3, cy - 6, 6, 12);
        }
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider&) override
    {
        const int cx = x + width / 2;
        const int cy = y + height / 2;
        const int r = juce::jmin(width, height) / 2 - 5;

        const auto cyan = juce::Colour::fromRGB(20, 220, 226);
        const auto cyanBright = juce::Colour::fromRGB(38, 238, 244);
        const auto dark = juce::Colour::fromRGB(5, 35, 39);
        const auto shadow = juce::Colour::fromRGB(3, 23, 26);
        const auto white = juce::Colour::fromRGB(225, 247, 246);

        g.setColour(shadow);
        g.fillEllipse((float) (cx - r - 4), (float) (cy - r - 4),
                      (float) ((r + 4) * 2), (float) ((r + 4) * 2));
        g.setColour(cyan);
        g.fillEllipse((float) (cx - r), (float) (cy - r),
                      (float) (r * 2), (float) (r * 2));
        g.setColour(dark);
        g.fillEllipse((float) (cx - r + 6), (float) (cy - r + 6),
                      (float) ((r - 6) * 2), (float) ((r - 6) * 2));

        const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        juce::Path arc;
        arc.addArc((float) (cx - r + 2), (float) (cy - r + 2),
                   (float) ((r - 2) * 2), (float) ((r - 2) * 2),
                   rotaryStartAngle, angle, true);
        g.setColour(cyanBright);
        g.strokePath(arc, juce::PathStrokeType(4.0f, juce::PathStrokeType::curved,
                                                juce::PathStrokeType::rounded));

        const float ix = cx + std::cos(angle) * (r - 13);
        const float iy = cy + std::sin(angle) * (r - 13);
        g.setColour(white);
        g.fillRect(juce::roundToInt(ix) - 3, juce::roundToInt(iy) - 3, 6, 6);
    }
};

CloudMakingMachineAudioProcessorEditor::CloudMakingMachineAudioProcessorEditor(
    CloudMakingMachineAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p),
      background(juce::ImageCache::getFromMemory(BinaryData::background_png,
                                                 BinaryData::background_pngSize)),
      lookAndFeel(std::make_unique<LookAndFeel>())
{
    setSize(600, 400);
    setResizable(false, false);

    configureSlider(delayTime, false);
    configureSlider(delayFeedback, false);
    configureSlider(delayChaos, false);
    configureSlider(delayMix, false);

    configureSlider(lowPass, false);
    configureSlider(highPass, false);
    configureSlider(distTone, false);
    configureSlider(distAmount, false);
    configureSlider(compInput, false);
    configureSlider(compPeak, false);

    configureSlider(dryWet, true);
    configureSlider(output, true);

    auto& state = processor.parameters;
    delayTimeAttachment = std::make_unique<Attachment>(state, "delayTime", delayTime);
    delayFeedbackAttachment = std::make_unique<Attachment>(state, "delayFeedback", delayFeedback);
    delayChaosAttachment = std::make_unique<Attachment>(state, "delayChaos", delayChaos);
    delayMixAttachment = std::make_unique<Attachment>(state, "delayMix", delayMix);
    lowPassAttachment = std::make_unique<Attachment>(state, "lpCutoff", lowPass);
    highPassAttachment = std::make_unique<Attachment>(state, "hpCutoff", highPass);
    distToneAttachment = std::make_unique<Attachment>(state, "distTone", distTone);
    distAmountAttachment = std::make_unique<Attachment>(state, "distAmount", distAmount);
    compInputAttachment = std::make_unique<Attachment>(state, "compInput", compInput);
    compPeakAttachment = std::make_unique<Attachment>(state, "compPeak", compPeak);
    dryWetAttachment = std::make_unique<Attachment>(state, "globalDryWet", dryWet);
    outputAttachment = std::make_unique<Attachment>(state, "outputGain", output);
}

void CloudMakingMachineAudioProcessorEditor::configureSlider(juce::Slider& slider, bool rotary)
{
    addAndMakeVisible(slider);
    slider.setLookAndFeel(lookAndFeel.get());
    slider.setSliderStyle(rotary ? juce::Slider::RotaryHorizontalVerticalDrag
                                 : juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setRange(0.0, 1.0, 0.0);
    slider.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    slider.setPopupDisplayEnabled(false, false, this);
    slider.setDoubleClickReturnValue(false, 0.0);

    if (rotary)
        slider.setRotaryParameters(juce::MathConstants<float>::pi * 1.25f,
                                   juce::MathConstants<float>::pi * 3.75f, true);
}

void CloudMakingMachineAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(42, 128, 133));

    if (background.isValid())
        g.drawImageAt(background, 0, 0);

    // The original background contains placeholder Dry/Wet / Output text.
    // Mask only those small areas and draw the final larger labels.
    g.setColour(juce::Colour::fromRGB(42, 128, 133));
    g.fillRect(188, 349, 126, 24);
    g.fillRect(432, 349, 104, 24);

    g.setColour(juce::Colour::fromRGB(0, 235, 240));
    g.setFont(juce::Font(15.0f, juce::Font::bold));
    g.drawText("DRY/WET", 220, 352, 92, 24, juce::Justification::centredLeft, false);
    g.drawText("OUTPUT", 442, 352, 82, 24, juce::Justification::centredLeft, false);
}

void CloudMakingMachineAudioProcessorEditor::resized()
{
    // Layout follows the approved 600x400 preview.
    delayTime.setBounds(28, 86, 192, 35);
    delayFeedback.setBounds(28, 143, 192, 35);
    delayChaos.setBounds(28, 200, 192, 35);
    delayMix.setBounds(28, 257, 192, 35);

    // Vertical controls are intentionally compact enough to stay clear of the knobs.
    lowPass.setSliderStyle(juce::Slider::LinearVertical);
    highPass.setSliderStyle(juce::Slider::LinearVertical);
    distTone.setSliderStyle(juce::Slider::LinearVertical);
    distAmount.setSliderStyle(juce::Slider::LinearVertical);
    compInput.setSliderStyle(juce::Slider::LinearVertical);
    compPeak.setSliderStyle(juce::Slider::LinearVertical);

    lowPass.setBounds(248, 88, 40, 224);
    highPass.setBounds(292, 88, 40, 224);
    distTone.setBounds(358, 88, 40, 224);
    distAmount.setBounds(402, 88, 40, 224);
    compInput.setBounds(468, 88, 40, 224);
    compPeak.setBounds(522, 88, 40, 224);

    dryWet.setBounds(303, 340, 50, 50);
    output.setBounds(510, 340, 50, 50);
}
