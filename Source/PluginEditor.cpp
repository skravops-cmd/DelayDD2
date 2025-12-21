#include "PluginEditor.h"

Dd2AudioProcessorEditor::Dd2AudioProcessorEditor
(
    Dd2AudioProcessor& processor,
    juce::AudioProcessorValueTreeState& apvts
)
    : AudioProcessorEditor (&processor), // MUST NOT be nullptr
      timeAttach (apvts, "TIME", timeSlider),
      fbAttach   (apvts, "FB", feedbackSlider),
      mixAttach  (apvts, "MIX", mixSlider)
{
    setSize (360, 140);

    for (auto* s : { &timeSlider, &feedbackSlider, &mixSlider })
    {
        s->setSliderStyle(juce::Slider::Rotary);
        s->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
        addAndMakeVisible(*s);
    }
}

Dd2AudioProcessorEditor::~Dd2AudioProcessorEditor() {}

void Dd2AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawText("DD-2 Digital Delay (Mono)",
               getLocalBounds().removeFromTop(30),
               juce::Justification::centred);
}

void Dd2AudioProcessorEditor::resized()
{
    const int knobSize = 90;
    timeSlider.setBounds(20, 40, knobSize, knobSize);
    feedbackSlider.setBounds(135, 40, knobSize, knobSize);
    mixSlider.setBounds(250, 40, knobSize, knobSize);
}
