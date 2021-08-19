#pragma once

#include "common/Looper.hpp"

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>

constexpr size_t LooperCount = 4;

namespace ID
{
   #define PARAMETER_ID(str) constexpr const char* str { #str };

    PARAMETER_ID( inputGain )
    PARAMETER_ID( inputAlgorithm )
    PARAMETER_ID( effectFeedback )
    PARAMETER_ID( effectDiffuse )
    PARAMETER_ID( effectWidth )
    PARAMETER_ID( effectMix )
    PARAMETER_ID( outputGain )
}

//==============================================================================
class Plugin  : public juce::AudioProcessor
{
public:
    enum class InputAlgorithms{
        L_ONLY = 0,
        R_ONLY,
        LR_MS_M,
        LR_MS_S,
        LR_SPLIT_12_34,
        LR_SPLIT_13_24
    };

    enum class DelayTimes{
        DT_1s = 0,
        DT_2s,
        DT_3s,
        DT_5s,
        DT_8s,
        DT_13s,
        DT_UNINITIALIZED
    };


private:
    std::array<sain::FixedBufferMonoLooper, LooperCount> Loopers;

    juce::dsp::Gain<float> InputGainProcessor;
    juce::dsp::Gain<float> OutputGainProcessor;
    juce::dsp::Compressor<float> CompressorProcessor;

    InputAlgorithms CurrentInputAlgorithm = InputAlgorithms::L_ONLY;
    DelayTimes CurrentDelayTime = DelayTimes::DT_UNINITIALIZED;

    // parameters are managed by parent class: AudioProcessor. Do not delete them manually
    juce::AudioParameterFloat* InputGain;
    juce::AudioParameterChoice* InputAlgorithm;
    juce::AudioParameterFloat* EffectFeedback;
    juce::AudioParameterFloat* EffectDiffuse;
    juce::AudioParameterFloat* EffectWidth;
    juce::AudioParameterFloat* EffectMix;
    juce::AudioParameterFloat* OutputGain;

public:
    //==============================================================================
    Plugin();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void reset() override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================

private:

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugin)
};
