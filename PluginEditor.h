#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class CloudMakingMachineAudioProcessorEditor : public juce::AudioProcessorEditor,
                                                       private juce::Timer
{
public:
    explicit CloudMakingMachineAudioProcessorEditor(CloudMakingMachineAudioProcessor&);
    ~CloudMakingMachineAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    class LookAndFeel;

    CloudMakingMachineAudioProcessor& processor;
    std::unique_ptr<LookAndFeel> lookAndFeel;

    juce::Slider delayTime;
    juce::Slider delayFeedback;
    juce::Slider delayChaos;
    juce::Slider delayMix;

    juce::Slider lowPass;
    juce::Slider highPass;
    juce::Slider distTone;
    juce::Slider distAmount;
    juce::Slider compInput;
    juce::Slider compPeak;

    juce::Slider dryWet;
    juce::Slider output;

    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attachment> delayTimeAttachment;
    std::unique_ptr<Attachment> delayFeedbackAttachment;
    std::unique_ptr<Attachment> delayChaosAttachment;
    std::unique_ptr<Attachment> delayMixAttachment;
    std::unique_ptr<Attachment> lowPassAttachment;
    std::unique_ptr<Attachment> highPassAttachment;
    std::unique_ptr<Attachment> distToneAttachment;
    std::unique_ptr<Attachment> distAmountAttachment;
    std::unique_ptr<Attachment> compInputAttachment;
    std::unique_ptr<Attachment> compPeakAttachment;
    std::unique_ptr<Attachment> dryWetAttachment;
    std::unique_ptr<Attachment> outputAttachment;

    void configureSlider(juce::Slider&, bool rotary);
    void repaintAllSliders();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CloudMakingMachineAudioProcessorEditor)
};
