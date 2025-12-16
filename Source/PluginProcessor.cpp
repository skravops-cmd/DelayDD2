#include "PluginProcessor.h"
#include "PluginEditor.h"

Dd2AudioProcessor::Dd2AudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
        .withOutput ("Output", juce::AudioChannelSet::mono(), true)),
      apvts (*this, nullptr, "PARAMS",
      {
          std::make_unique<juce::AudioParameterFloat>("TIME", "Delay Time",
              juce::NormalisableRange<float>(1.0f, 800.0f, 1.0f), 300.0f),
          std::make_unique<juce::AudioParameterFloat>("FB", "Feedback",
              juce::NormalisableRange<float>(0.0f, 0.95f, 0.001f), 0.35f),
          std::make_unique<juce::AudioParameterFloat>("MIX", "Mix",
              juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.4f)
      })
{
    delayTimeMs = apvts.getRawParameterValue("TIME");
    feedback    = apvts.getRawParameterValue("FB");
    mix         = apvts.getRawParameterValue("MIX");
}

Dd2AudioProcessor::~Dd2AudioProcessor() {}

const juce::String Dd2AudioProcessor::getName() const
{
    return "DD2_Mono";
}

bool Dd2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainInputChannelSet() == juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono();
}

void Dd2AudioProcessor::prepareToPlay (double sampleRate, int)
{
    sr = sampleRate;

    const int maxDelaySamples = (int)(sr * 2.0); // 2 seconds max
    delayBuffer.setSize(1, maxDelaySamples);
    delayBuffer.clear();

    writePosition = 0;
}

void Dd2AudioProcessor::releaseResources() {}

void Dd2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                      juce::MidiBuffer&)
{
    const int numSamples = buffer.getNumSamples();
    const int delaySamples =
        (int)(*delayTimeMs * 0.001 * sr);

    auto* channelData = buffer.getWritePointer(0);
    auto* delayData   = delayBuffer.getWritePointer(0);

    for (int i = 0; i < numSamples; ++i)
    {
        const int readPos =
            (writePosition - delaySamples + delayBuffer.getNumSamples())
            % delayBuffer.getNumSamples();

        const float delayedSample = delayData[readPos];
        const float inputSample = channelData[i];

        delayData[writePosition] =
            inputSample + delayedSample * (*feedback);

        channelData[i] =
            inputSample * (1.0f - *mix)
            + delayedSample * (*mix);

        writePosition++;
        if (writePosition >= delayBuffer.getNumSamples())
            writePosition = 0;
    }
}

bool Dd2AudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* Dd2AudioProcessor::createEditor()
{
    return new Dd2AudioProcessorEditor (*this, apvts);
}

// JUCE plugin factory function — must be global and outside any namespace/class
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Dd2AudioProcessor();
}
