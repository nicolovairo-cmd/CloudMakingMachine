#include "PluginEditor.h"
#include "PluginProcessor.h"

juce::AudioProcessorEditor* CloudMakingMachineAudioProcessor::createEditor()
{
    return new CloudMakingMachineAudioProcessorEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE
createPluginFilter()
{
    return new CloudMakingMachineAudioProcessor();
}
