#include "PluginProcessor.h"

Plugin::Plugin() :
    AudioProcessor (BusesProperties())
{
    addParameter(
        speed = new juce::AudioParameterFloat(
            "speed",
            "Arpeggiator Speed",
            0.0,
            1.0,
            0.5));


}

//==============================================================================
void Plugin::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(samplesPerBlock);

    notes.clear();
    currentNote = -1;
}

void Plugin::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void Plugin::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    static juce::uint32 bufferID = 0;
    ++bufferID; 

    // the audio buffer in a midi effect will have zero channels!
    jassert(buffer.getNumChannels() == 0);

    // however we use the buffer to get timing information
    auto numSamples = buffer.getNumSamples();

    for (const auto metadata : midi)
    {
        const auto msg = metadata.getMessage();
        const auto note = msg.getNoteNumber();
        const auto velocity = msg.getVelocity();
        const auto sample = metadata.samplePosition;

        // if (msg.isNoteOn())
        //     notes.add(NoteData{note, velocity, bufferID, sample});
        // else if (msg.isNoteOff())
        //     notes.removeValue(msg.getNoteNumber());
    }

    midi.clear();

    auto NoteIDX = int(*selectedNote);

    //if ( notes[NoteIDX] != currentNote)
    {
        /* code */
    }



    // if ((time + numSamples) >= noteDuration)
    // {
    //     auto offset = juce::jmax(0, juce::jmin((int)(noteDuration - time), numSamples - 1));

    //     if (lastNoteValue > 0)
    //     {
    //         midi.addEvent(juce::MidiMessage::noteOff(1, lastNoteValue), offset);
    //         lastNoteValue = -1;
    //     }

    //     if (notes.size() > 0)
    //     {
    //         currentNote = (currentNote + 1) % notes.size();
    //         lastNoteValue = notes[currentNote];
    //         midi.addEvent(juce::MidiMessage::noteOn(1, lastNoteValue, (juce::uint8)127), offset);
    //     }
    // }

    // time = (time + numSamples) % noteDuration;
}

//==============================================================================
bool Plugin::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Plugin::createEditor()
{
    return nullptr; //new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void Plugin::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void Plugin::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================

const juce::String Plugin::getName() const
{
    return JucePlugin_Name;
}

bool Plugin::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool Plugin::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool Plugin::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double Plugin::getTailLengthSeconds() const
{
    return 0.0;
}

int Plugin::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int Plugin::getCurrentProgram()
{
    return 0;
}

void Plugin::setCurrentProgram (int index)
{
    juce::ignoreUnused(index);
}

const juce::String Plugin::getProgramName (int index)
{
    juce::ignoreUnused(index);
    return {};
}

void Plugin::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugin();
}
