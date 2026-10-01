#include "PluginEditor.h"
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
                          float sliderPos, float /*minSliderPos*/, float /*maxSliderPos*/,
                          juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
        const bool vertical = (style == juce::Slider::LinearVertical);

        g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);

        if (vertical)
        {
            const float cx = x + width * 0.5f;

            // Fixed white vertical guide line.
            const float lineW = juce::jmin(36.0f, width * 0.54f);
            g.setColour(juce::Colour::fromRGB(225, 247, 246));
            g.fillRoundedRectangle(
                cx - lineW * 0.5f, static_cast<float>(y),
                lineW, static_cast<float>(height), lineW * 0.5f);

            // The dark rectangular thumb is the part that moves vertically.
            const float thumbW = juce::jmin(84.0f, width * 1.44f);
            const float thumbH = juce::jmin(36.0f, height * 0.16f);

            g.setColour(juce::Colour::fromRGB(20, 45, 48));
            g.fillRoundedRectangle(
                cx - thumbW * 0.5f,
                sliderPos - thumbH * 0.5f,
                thumbW, thumbH, thumbH * 0.5f);
        }
        else
        {
            // Horizontal slider is intentionally left as the working diagnostic
            // implementation. Do not change its behaviour.
            const float cy = y + height * 0.5f;
            const float trackH = juce::jmin(10.0f, height * 0.30f);

            g.setColour(juce::Colour::fromRGB(20, 45, 48));
            g.fillRoundedRectangle(
                static_cast<float>(x), cy - trackH * 0.5f,
                static_cast<float>(width), trackH, trackH * 0.5f);

            const float thumbX = sliderPos;
            const float thumbW = juce::jmin(18.0f, width * 0.08f);
            const float thumbH = juce::jmin(42.0f, height * 0.72f);

            g.setColour(juce::Colour::fromRGB(225, 247, 246));
            g.fillRoundedRectangle(
                thumbX - thumbW * 0.5f, cy - thumbH * 0.5f,
                thumbW, thumbH, thumbW * 0.5f);
        }

        juce::ignoreUnused(slider);
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

    // Force regular GUI repaints so custom slider graphics always follow
    // parameter/attachment changes, including automation.
    startTimerHz(30);
}

CloudMakingMachineAudioProcessorEditor::~CloudMakingMachineAudioProcessorEditor()
{
    stopTimer();
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

    slider.onValueChange = [&slider]()
    {
        slider.repaint();
    };

    if (rotary)
        slider.setRotaryParameters(juce::MathConstants<float>::pi * 1.25f,
                                   juce::MathConstants<float>::pi * 3.75f, true);
}

void CloudMakingMachineAudioProcessorEditor::timerCallback()
{
    repaintAllSliders();
}

void CloudMakingMachineAudioProcessorEditor::repaintAllSliders()
{
    delayTime.repaint();
    delayFeedback.repaint();
    delayChaos.repaint();
    delayMix.repaint();

    lowPass.repaint();
    highPass.repaint();
    distTone.repaint();
    distAmount.repaint();
    compInput.repaint();
    compPeak.repaint();

    dryWet.repaint();
    output.repaint();
}

void CloudMakingMachineAudioProcessorEditor::paint(juce::Graphics& g)
{
    // Entire interface is drawn procedurally; no external assets are required.
    const auto bg = juce::Colour::fromRGB(13, 77, 82);
    const auto panel = juce::Colour::fromRGB(15, 91, 96);
    const auto cyan = juce::Colour::fromRGB(38, 238, 244);
    const auto cyanDim = juce::Colour::fromRGB(20, 154, 160);

    g.fillAll(bg);

    g.setColour(cyan);
    g.setFont(juce::Font(21.0f, juce::Font::bold));
    g.drawText("CLOUD MAKING MACHINE", 16, 10, 350, 28,
               juce::Justification::left, false);

    g.setColour(cyanDim);
    g.fillRect(16, 43, 568, 2);

    auto drawPanel = [&g, &cyan, &cyanDim, &panel](int x, int y, int w, int h, const char* title)
    {
        g.setColour(panel);
        g.fillRoundedRectangle((float)x, (float)y, (float)w, (float)h, 4.0f);
        g.setColour(cyanDim);
        g.drawRoundedRectangle((float)x, (float)y, (float)w, (float)h, 4.0f, 2.0f);
        g.setColour(cyan);
        g.setFont(juce::Font(13.0f, juce::Font::bold));
        g.drawText(title, x + 8, y + 5, w - 16, 22, juce::Justification::left, false);
    };

    drawPanel(16, 56, 218, 274, "DELAY");
    drawPanel(244, 56, 102, 274, "FILTERS");
    drawPanel(354, 56, 102, 274, "DISTORTION");
    drawPanel(464, 56, 120, 274, "COMPRESSION");

    // Output labels: kept to the left of the knobs, with no overlap.
    g.setColour(cyan);
    g.setFont(juce::Font(15.0f, juce::Font::bold));
    g.drawText("DRY/WET", 220, 352, 80, 24, juce::Justification::left, false);
    g.drawText("OUTPUT", 442, 352, 64, 24, juce::Justification::left, false);
};

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

    lowPass.repaint();
    highPass.repaint();
    distTone.repaint();
    distAmount.repaint();
    compInput.repaint();
    compPeak.repaint();

    lowPass.setBounds(248, 88, 40, 224);
    highPass.setBounds(292, 88, 40, 224);
    distTone.setBounds(358, 88, 40, 224);
    distAmount.setBounds(402, 88, 40, 224);
    compInput.setBounds(468, 88, 40, 224);
    compPeak.setBounds(522, 88, 40, 224);

    dryWet.setBounds(303, 340, 50, 50);
    output.setBounds(510, 340, 50, 50);
}
