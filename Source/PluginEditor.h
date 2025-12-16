#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class Dd2AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Dd2AudioProcessorEditor (Dd2AudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~Dd2AudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider timeSlider, feedbackSlider, mixSlider;

    juce::AudioProcessorValueTreeState::SliderAttachment timeAttach;
    juce::AudioProcessorValueTreeState::SliderAttachment fbAttach;
    juce::AudioProcessorValueTreeState::SliderAttachment mixAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Dd2AudioProcessorEditor)
};
