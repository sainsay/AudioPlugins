#pragma once

namespace sain{



    template<typename T, size_t size>
    struct Sample
    {
        T Data[size];

        T& operator[](const size_t idx)
        {
            return Data[idx];
        }
    };

    using StereoSample = Sample<float, 2>;
    template<>
    struct Sample<float , 2>
    {
        float L;
        float R;

        float& operator[](const size_t idx)
        {
            jassert(idx < 2);
            float* f = reinterpret_cast<float*>(this);
            return *(f+idx);
            // potentially dangerous code. 

        }

        StereoSample& operator*( float rhs )
        {
            L *= rhs;
            R *= rhs;
            return *this;
        }

        StereoSample& operator+=( StereoSample& rhs )
        {
            L += rhs.L;
            R += rhs.R;
            return *this;
        }
        
    };


    template<>
    struct Sample<float , 6>
    {
        float L;
        float R;
        float C;
        float Sub;
        float SurL;
        float SurR;

        float& operator[](const size_t idx)
        {
            jassert(idx < 6);

            float* f = reinterpret_cast<float*>(this);
            return *(f+idx);
            // potentially dangerous code. 
        }
    };

    using FiveOneSample = Sample<float, 6>;

}