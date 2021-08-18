#pragma once
#include "Buffer.hpp"
#include "Bitflag.hpp"

namespace sain
{
	enum class LooperFlags : int8_t
	{
		STATE_OK            = 0b0,
		STATE_UNINITIALIZED = 1 << 0,
		REQ_FLIP_DIR        = 1 << 1,
		REQ_WRITEHEAD_ADJUSTMENT = 1 << 2
	};

	DEF_FLAG_OR_OP( LooperFlags, int8_t )
	DEF_FLAG_AND_OP( LooperFlags, int8_t )
    DEF_FLAG_ASSIGN_OR_OP( LooperFlags, int8_t)
    DEF_FLAG_ASSIGN_AND_OP( LooperFlags, int8_t)
    DEF_FLAG_NOT_OP( LooperFlags, int8_t)

	class FixedBufferMonoLooper
	{
    public:
		// 15 seconds on 192kHz
		static const size_t buffer_size = 15 * 192000; 

    private:
		RingBuffer<float, buffer_size> Buffer;

		typedef typename RingBuffer<float, buffer_size>::iterator iterator;
		iterator Playhead;
		iterator Writehead;

		LooperFlags Flags = LooperFlags::STATE_UNINITIALIZED;

        float LoopLenght = 1.0f;
		float SampleRate = 44100.0f;
		bool ReverseState = false;

	 public:
		FixedBufferMonoLooper( )
        {}

        FixedBufferMonoLooper( const float lenght, const float sampleRate ) :
            LoopLenght(lenght),
			SampleRate(sampleRate)
        {}

		void Process( );

		inline void WriteSample( float value )
		{
        	*Writehead = value;
    	}

		inline float ReadSample( ) const
		{
			return *Playhead;
		}

		inline void Reverse()
		{
			SET_FLAG(Flags, LooperFlags::REQ_FLIP_DIR);
		}

		inline void SetSampleRate( const float value )
		{
			SampleRate = value;
			SET_FLAG(Flags, LooperFlags::REQ_WRITEHEAD_ADJUSTMENT);
		}

		inline void SetLoopLength( const float value )
		{
			if( value > MaxLoopLength())
			{
				return;
			}

			LoopLenght = value;
			SET_FLAG(Flags, LooperFlags::REQ_WRITEHEAD_ADJUSTMENT);
		}
		
		inline const float MaxLoopLength() const
		{
			return std::floor( buffer_size / SampleRate );
		}

		inline void Clear()
		{
			Buffer.Clear();
			SET_FLAG(Flags, LooperFlags::STATE_UNINITIALIZED);
		}
	};

}	 // namespace sain