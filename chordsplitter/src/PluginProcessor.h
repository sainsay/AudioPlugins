#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_processors/juce_audio_processors.h>


namespace ID
{
   #define PARAMETER_ID(str) constexpr const char* str { #str };

    PARAMETER_ID( inputGain )
    PARAMETER_ID( inputAlgorithm )
    PARAMETER_ID( effectDelay )
    PARAMETER_ID( effectFeedback )
    PARAMETER_ID( effectWidth )
    PARAMETER_ID( effectMix )
    PARAMETER_ID( outputGain )
}

struct NoteData{
    int note;
    int velocity;
    int sample;
    bool isOn;

    bool operator==(const NoteData& other) const noexcept
    {
        return note == other.note 
            && sample == other.sample
            && isOn == other.isOn;
    }

    bool operator!=(const NoteData& other) const noexcept
    {
        return !(*this == other);
    }

    bool operator<(const NoteData& other) const noexcept
    {
        if (note < other.note)
            return true;
        else if ( sample < other.sample )
            return true;
        else if ( isOn < other.isOn )
            return true;
        else
            return false;
    }

    bool operator>(const NoteData& other) const noexcept
    {
        if (note > other.note)
            return true;
        else if ( sample > other.sample )
            return true;
        else if ( isOn > other.isOn )
            return true;
        else
            return false;
    }

    bool operator<=(const NoteData& other) const noexcept
    {
        return !(*this > other);
    }

    bool operator>=(const NoteData& other) const noexcept
    {
        return !(*this < other);
    }

};

//===============================================================================
class Plugin  : public juce::AudioProcessor
{
public:

    //==============================================================================
    Plugin();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;

    void releaseResources() override;

    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) override;

    using AudioProcessor::processBlock;

    //==============================================================================
    bool isMidiEffect() const override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int) override;
    const juce::String getProgramName (int) override;
    void changeProgramName (int, const juce::String&) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    juce::AudioParameterInt* selectedNote;
    juce::AudioParameterBool* PlayHighestAvailableNote;

    int currentNote;
    juce::SortedSet<NoteData> notes;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Plugin)
};
