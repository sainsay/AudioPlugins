#include "PluginProcessor.h"

static void processMidiIn(juce::MidiBuffer& midi, juce::SortedSet<NoteData>& notes )
{
    for (const auto metadata : midi)
    {
        const auto msg = metadata.getMessage();
        const auto note = msg.getNoteNumber();
        const auto velocity = msg.getVelocity();
        const auto sample = metadata.samplePosition;
        const auto isOn = msg.isNoteOnOrOff();

        NoteData noteData;
        noteData.note = note;
        noteData.velocity = velocity;
        noteData.sample = sample;
        noteData.isOn = isOn;

        notes.add(noteData);
    }
}

static void processMidiOut(juce::MidiBuffer& midi, juce::SortedSet<NoteData>& notes, int& currentNote, const int selectedIDX, const int sampleCount, const bool PlayHighestAvailableNote )
{
    juce::SortedSet<int> ActiveNotes;
    // TODO: move ActiveNotes creation and allocation out of hot path
    ActiveNotes.ensureStorageAllocated(32); // ensures only one allocation for most use cases

    std::for_each(
        notes.begin(), 
        notes.end(), 
        [&ActiveNotes](const NoteData& note)
        {
            if (note.sample != -1 || !note.isOn){
                return;
            }

            auto itt = std::find(ActiveNotes.begin(), ActiveNotes.end(), note.note);
            if (itt == ActiveNotes.end())
            {
                ActiveNotes.add(note.note);
            }
        }
    );

    for (int i = 0 ; i < sampleCount; ++i)
    {
        std::for_each(
            notes.begin(), 
            notes.end(), 
            [&ActiveNotes, &i](const NoteData& note)
            {
                if (note.sample != i){
                    return;
                }
                
                if (!note.isOn)
                {
                    auto itt = std::find(ActiveNotes.begin(), ActiveNotes.end(), note.note);
                    if (itt != ActiveNotes.end())
                    {
                        ActiveNotes.remove(static_cast<const int>(itt - ActiveNotes.begin()));
                    }
                }
                else
                {
                    ActiveNotes.add(note.note);
                }
            }
        );

        int selectedNote;

        if (selectedIDX < ActiveNotes.size()) {
            selectedNote = ActiveNotes.getUnchecked( selectedIDX ); 
        } 
        else
        {        
            if(PlayHighestAvailableNote && ActiveNotes.size() - 1 >= 0)
            {
                selectedNote = ActiveNotes.getUnchecked( ActiveNotes.size() - 1 );
            }
            else
            {
                selectedNote = -1;
            }
        }
        
        if (currentNote != selectedNote)
        {
            if (currentNote != -1)
            {
                juce::MidiMessage msg(juce::MidiMessage::noteOff(1, currentNote));
                midi.addEvent(msg, i);
            }

            if (selectedNote != -1)
            {
                juce::MidiMessage msg(juce::MidiMessage::noteOn(1, selectedNote, (juce::uint8) 127));
                midi.addEvent(msg, i);
            }

            currentNote =selectedNote;
        }
    }

    notes.clearQuick();
    for (const auto note : ActiveNotes)
    {
        NoteData noteData;
        noteData.note = note;
        noteData.velocity = 127;
        noteData.sample = -1;
        noteData.isOn = true;

        notes.add(noteData);
    }
}

Plugin::Plugin() :
    AudioProcessor (BusesProperties())
{
    addParameter(
        selectedNote = new juce::AudioParameterInt(
            "noteid",
            "Selected Note",
            0,
            8,
            0));
    
    addParameter(
        PlayHighestAvailableNote = new juce::AudioParameterBool(
            "playHighestAvailableNote",
            "Play Highest Available Note",
            false));
}

//==============================================================================
void Plugin::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);

    notes.clear();
    currentNote = -1;
}

void Plugin::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    notes.clear();
}

void Plugin::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    jassert(buffer.getNumChannels() == 0);

    auto numSamples = buffer.getNumSamples();
    auto NoteIDX = int(*selectedNote);
    auto PlayHighestAvailableNoteValue = bool(*PlayHighestAvailableNote);

    processMidiIn(midi, notes);    
    midi.clear();

    processMidiOut(midi, notes, currentNote, NoteIDX, numSamples, PlayHighestAvailableNoteValue);
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
