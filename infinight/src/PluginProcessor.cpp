#include "PluginProcessor.h"
#include "PluginHelperFunc.h"

#include "common/AudioSample.hpp"

Plugin::Plugin() :
    AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
    addParameter(
        InputGain =
            new juce::AudioParameterFloat
            (
                ID::inputGain,
                "Input",
                juce::NormalisableRange<float>( -40.0f, 40.0f ),
                0.0f,
                "dB",
                juce::AudioProcessorParameter::Category::inputGain,
                valueToTextFunction,
                textToValueFunction
            )
    );

    addParameter( 
        InputAlgorithm = 
            new juce::AudioParameterChoice 
            (
                ID::inputAlgorithm,
                "Input Algorithm",
                juce::StringArray{ 
                    "L only", 
                    "R only", 
                    "L/R -> M/S Mid", 
                    "L/R -> M/S Side", 
                    "L/R split -> (1 & 2), (3 & 4)", 
                    "L/R split -> (1 & 3), (2 & 4)" 
                    },
                0 
            )  
    );

    addParameter( 
        effectDelay = 
            new juce::AudioParameterChoice 
            (
                ID::effectDelay,
                "Delay Time",
                juce::StringArray{ 
                    "~1 second", 
                    "~2 seconds", 
                    "~3 seconds", 
                    "~5 seconds", 
                    "~8 seconds", 
                    "~13 seconds" 
                    },
                0 
            )  
    );

    addParameter(
        EffectFeedback =
            new juce::AudioParameterFloat
            (
                ID::effectFeedback,
                "Feedback",
                juce::NormalisableRange<float>( 0.0f, 100.0f ),
                100.0f,
                "%",
                juce::AudioProcessorParameter::Category::genericParameter,
                valueToTextFunction,
                textToValueFunction
            )
    );

    addParameter(
        EffectWidth =
            new juce::AudioParameterFloat
            (
                ID::effectWidth,
                "Width",
                juce::NormalisableRange<float>( 0.0f, 100.0f ),
                100.0f,
                "%",
                juce::AudioProcessorParameter::Category::genericParameter,
                valueToTextFunction,
                textToValueFunction
            )
    );

    addParameter(
        EffectMix =
            new juce::AudioParameterFloat
            (
                ID::effectMix,
                "Mix",
                juce::NormalisableRange<float>( 0.0f, 100.0f ),
                50.0f,
                "%",
                juce::AudioProcessorParameter::Category::genericParameter,
                valueToTextFunction,
                textToValueFunction
            )
    );

    addParameter(
        OutputGain =
            new juce::AudioParameterFloat
            (
                ID::outputGain,
                "Output",
                juce::NormalisableRange<float>( -40.0, 40.0f ),
                0.0f,
                "dB",
                juce::AudioProcessorParameter::Category::genericParameter,
                valueToTextFunction,
                textToValueFunction
            )
    );

    InputGainProcessor.setRampDurationSeconds(0.05f);
    OutputGainProcessor.setRampDurationSeconds(0.05f);

}

//==============================================================================
void Plugin::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (auto& looper : Loopers)
    {
        looper.SetSampleRate( static_cast<float>(sampleRate) );
    }

    InputGainProcessor.prepare({sampleRate, static_cast<uint32_t>(samplesPerBlock), 2});
    OutputGainProcessor.prepare({sampleRate, static_cast<uint32_t>(samplesPerBlock), 2});
    CompressorProcessor.prepare({sampleRate, static_cast<uint32_t>(samplesPerBlock), 2});

    juce::ignoreUnused( sampleRate, samplesPerBlock );
}

void Plugin::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void Plugin::reset()
{
    InputGainProcessor.reset();
    OutputGainProcessor.reset();
    CompressorProcessor.reset();
    for (auto& looper : Loopers)
    {
        looper.Clear();
    }
}

void Plugin::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //----parameter handling-----
    InputGainProcessor.setGainDecibels( InputGain->get() );
    OutputGainProcessor.setGainDecibels( OutputGain->get() );

    float currentMix = EffectMix->get() * 0.01f;
    float currentWidth = EffectWidth->get() * 0.01f;
    float currentFeedback = EffectFeedback->get() * 0.01f;

    constexpr float currentCompRatio = 4.0f;
    constexpr float currentCompThreshold = 0.0f;
    constexpr float currentCompAttack = 1.0f;
    constexpr float currentCompRelease = 100.0f;
    constexpr float panConsts[LooperCount] = { -1.0f, -0.5f, 0.5f, 1.0f };

    PanValue panValues[LooperCount] = {
        calcPan( panConsts[0] * currentWidth ),
        calcPan( panConsts[1] * currentWidth ),
        calcPan( panConsts[2] * currentWidth ),
        calcPan( panConsts[3] * currentWidth )
    };

    CompressorProcessor.setAttack( currentCompAttack );
    CompressorProcessor.setRelease( currentCompRelease );
    CompressorProcessor.setRatio( currentCompRatio );
    CompressorProcessor.setThreshold ( currentCompThreshold );

    if( static_cast<int>( CurrentDelayTime ) != effectDelay->getIndex() ){
        CurrentDelayTime = static_cast<DelayTimes>( effectDelay->getIndex() );
        setDelayTimes( CurrentDelayTime, Loopers );
    }

    //----Processing----
    auto inoutBlock = juce::dsp::AudioBlock<float> (buffer).getSubsetChannelBlock (0, (size_t) totalNumInputChannels);
    auto context = juce::dsp::ProcessContextReplacing<float> (inoutBlock);
    
    InputGainProcessor.process(context);

    const auto& inputBlock = context.getInputBlock();
    auto& outputBlock      = context.getOutputBlock();
    const auto numChannels = outputBlock.getNumChannels();
    const auto numSamples  = outputBlock.getNumSamples();

    auto* inputSamplesL  = inputBlock .getChannelPointer (0);
    auto* inputSamplesR  = inputBlock .getChannelPointer (1);
    auto* outputSamplesL = outputBlock.getChannelPointer (0);
    auto* outputSamplesR = outputBlock.getChannelPointer (1);

    for (size_t i = 0; i < numSamples; ++i)
    {
        sain::StereoSample dry = { inputSamplesL[i], inputSamplesR[i]};

        auto In = processInputAlgorithm(dry, CurrentInputAlgorithm);
        sain::StereoSample Out[LooperCount];

        for( size_t j = 0; j < LooperCount; ++j)
        {
            Loopers[j].Process();
            float tempOut = Loopers[j].ReadSample();
            Loopers[j].WriteSample( In[j] + (tempOut * currentFeedback) );
            Out[j].L = tempOut * panValues[j].left;
            Out[j].R = tempOut * panValues[j].Right;
        }
        
        sain::StereoSample wet = {};
        for( auto& v: Out)
        {
            wet += v;
        }

        outputSamplesL[i] = crossfade(dry.L, wet.L, currentMix);
        outputSamplesR[i] = crossfade(dry.R, wet.R, currentMix);
    }

    CompressorProcessor.process(context);
    OutputGainProcessor.process(context);
}

//==============================================================================
bool Plugin::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Plugin::createEditor()
{
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void Plugin::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void Plugin::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
bool Plugin::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

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
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Plugin::getCurrentProgram()
{
    return 0;
}

void Plugin::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String Plugin::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void Plugin::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Plugin();
}
