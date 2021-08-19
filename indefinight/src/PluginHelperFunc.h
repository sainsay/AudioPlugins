#pragma once

#include "PluginProcessor.h"
#include "common/AudioSample.hpp"

#include <juce_core/juce_core.h>

inline static juce::String valueToTextFunction (float x, int maxLenght) 
{ 
    juce::ignoreUnused(maxLenght);
    return juce::String (x, 2); 
}

inline static float textToValueFunction (const juce::String& str) 
{ 
    return str.getFloatValue(); 
}

inline static float crossfade(float a, float b, float p) {
    auto tempA = a * (1.0f - p);
    auto tempB = b * p;
	return tempA + tempB;
}

static auto processInputAlgorithm( sain::StereoSample sample, const Plugin::InputAlgorithms algorithm )
{
    using apap = Plugin;
    struct Result
    {
        float data[4];

        float& operator[](size_t idx)
        {
            return data[idx];
        }
    };
    Result result = {};

    switch (algorithm)
        {
        case apap::InputAlgorithms::L_ONLY :
            {
                result[0]    = sample.L;
                result[1]    = sample.L;
                result[2]    = sample.L;
                result[3]    = sample.L;
            }
            break;
        case apap::InputAlgorithms::R_ONLY :
            {
                result[0]     = sample.R;
                result[1]    = sample.R;
                result[2]    = sample.R;
                result[3]    = sample.R;
            }
            break;
        case apap::InputAlgorithms::LR_MS_M :
            {
                const float mid = ( sample.L + sample.R ) * 0.5f;
                result[0]    = mid;
                result[1]    = mid;
                result[2]    = mid;
                result[3]    = mid;
            }
            break;
        case apap::InputAlgorithms::LR_MS_S :
            {
                const float side = ( sample.L - sample.R ) * 0.5f;
                result[0]    = side;
                result[1]    = side;
                result[2]    = side;
                result[3]    = side;
            }
            break;
        case apap::InputAlgorithms::LR_SPLIT_12_34 :
            {
                result[0]    = sample.L;
                result[1]    = sample.L;
                result[2]    = sample.R;
                result[3]    = sample.R;
            }
            break;
        case apap::InputAlgorithms::LR_SPLIT_13_24 :
            {
                result[0]    = sample.L;
                result[1]    = sample.R;
                result[2]    = sample.L;
                result[3]    = sample.R;
            }
            break;
        default:
            break;
        }

    return result;
}

struct PanValue{
    float left;
    float Right;
};

// 6dB pan rule. from juce::dsp::panner
inline static auto calcPan(float pan){
    auto normalisedPan = static_cast<float> (0.5) * (pan + static_cast<float> (1.0));
    float leftValue  = static_cast<float> (std::pow (std::sin (0.5 * juce::MathConstants<double>::pi * (1.0 - normalisedPan)), 2.0));
    float rightValue = static_cast<float> (std::pow (std::sin (0.5 * juce::MathConstants<double>::pi * normalisedPan), 2.0));
    float boostValue = static_cast<float> (2.0);

    return PanValue{ leftValue* boostValue, rightValue* boostValue };
}

inline void setDelayTimes(Plugin::DelayTimes dt, std::array<sain::FixedBufferMonoLooper, LooperCount>& loopers )
{
    constexpr float timeMultiplierLooper1 = 0.904f;
    constexpr float timeMultiplierLooper2 = 1.05f;
    constexpr float timeMultiplierLooper3 = 0.961f;
    constexpr float timeMultiplierLooper4 = 1.104f; // longest delay time would be around 14.3s

    switch (dt)
    {
        case Plugin::DelayTimes::DT_1s:
        {
            loopers[0].SetLoopLength(1.0f * timeMultiplierLooper1);
            loopers[1].SetLoopLength(1.0f * timeMultiplierLooper2);
            loopers[2].SetLoopLength(1.0f * timeMultiplierLooper3);
            loopers[3].SetLoopLength(1.0f * timeMultiplierLooper4);
        }
        break;
        case Plugin::DelayTimes::DT_2s:
        {
            loopers[0].SetLoopLength(2.0f * timeMultiplierLooper1);
            loopers[1].SetLoopLength(2.0f * timeMultiplierLooper2);
            loopers[2].SetLoopLength(2.0f * timeMultiplierLooper3);
            loopers[3].SetLoopLength(2.0f * timeMultiplierLooper4);
        }
        break;
        case Plugin::DelayTimes::DT_3s:
        {
            loopers[0].SetLoopLength(3.0f * timeMultiplierLooper1);
            loopers[1].SetLoopLength(3.0f * timeMultiplierLooper2);
            loopers[2].SetLoopLength(3.0f * timeMultiplierLooper3);
            loopers[3].SetLoopLength(3.0f * timeMultiplierLooper4);
        }
        break;
        case Plugin::DelayTimes::DT_5s:
        {
            loopers[0].SetLoopLength(5.0f * timeMultiplierLooper1);
            loopers[1].SetLoopLength(5.0f * timeMultiplierLooper2);
            loopers[2].SetLoopLength(5.0f * timeMultiplierLooper3);
            loopers[3].SetLoopLength(5.0f * timeMultiplierLooper4);
        }
        break;
        case Plugin::DelayTimes::DT_8s:
        {
            loopers[0].SetLoopLength(8.0f * timeMultiplierLooper1);
            loopers[1].SetLoopLength(8.0f * timeMultiplierLooper2);
            loopers[2].SetLoopLength(8.0f * timeMultiplierLooper3);
            loopers[3].SetLoopLength(8.0f * timeMultiplierLooper4);
        }
        break;
        case Plugin::DelayTimes::DT_13s:
        {
            loopers[0].SetLoopLength(13.0f * timeMultiplierLooper1);
            loopers[1].SetLoopLength(13.0f * timeMultiplierLooper2);
            loopers[2].SetLoopLength(13.0f * timeMultiplierLooper3);
            loopers[3].SetLoopLength(13.0f * timeMultiplierLooper4);
        }
        break;
        default:
            break;
    }
}
