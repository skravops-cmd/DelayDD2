#pragma once

#include <JuceHeader.h>

class Dd2AudioProcessor  : public juce::AudioProcessor
{
public:
    Dd2AudioProcessor();
    ~Dd2AudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    double getTailLengthSeconds() const override { return 2.0; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock&) override {}
    void setStateInformation (const void*, int) override {}

    // Parameters
    std::atomic<float>* delayTimeMs;
    std::atomic<float>* feedback;
    std::atomic<float>* mix;

private:
    juce::AudioProcessorValueTreeState apvts;

    juce::AudioBuffer<float> delayBuffer;
    int writePosition = 0;
    double sr = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Dd2AudioProcessor)
};
