#include "common/Looper.hpp"

#include <cmath>

namespace sain
{
    static size_t CalcIndex(float SampleRate, float LoopLenght)
    {
        return static_cast<size_t>( std::round( std::fabs( SampleRate * LoopLenght ) ) );
    }

    void FixedBufferMonoLooper::Process()
    {
        if ( FLAG_IS_SET( Flags, LooperFlags::STATE_UNINITIALIZED ) )
        {
            size_t index = CalcIndex(SampleRate, LoopLenght);

            Playhead = Buffer.begin();
            Writehead = Buffer.from(index);

            ReverseState = false;

            CLEAR_FLAG( Flags, LooperFlags::STATE_UNINITIALIZED );

            // if uninitialized, requests have no meaning full effects 
            CLEAR_FLAG( Flags, LooperFlags::REQ_WRITEHEAD_ADJUSTMENT );
            CLEAR_FLAG( Flags, LooperFlags::REQ_FLIP_DIR );             
        }
        
        if ( FLAG_IS_SET( Flags, LooperFlags::REQ_WRITEHEAD_ADJUSTMENT ) )
        {
            if ( FLAG_IS_SET( Flags, LooperFlags::REQ_FLIP_DIR ) )
            {
                ReverseState = !ReverseState;
                CLEAR_FLAG( Flags, LooperFlags::REQ_FLIP_DIR );
            }

            const size_t index = CalcIndex(SampleRate, LoopLenght);
            const size_t current = Playhead - Buffer.begin();

            size_t nextPos = 0;

            if ( ReverseState )
            {
                if (index > current)
                {
                    nextPos = buffer_size - (index - current);
                }else
                {
                    nextPos = current - index;
                }
            } 
            else
            {   
                nextPos = current + index;
                if( nextPos > buffer_size ){
                    nextPos -= buffer_size;
                } 
            }
            
            Writehead = Buffer.from(nextPos);
            CLEAR_FLAG( Flags, LooperFlags::REQ_WRITEHEAD_ADJUSTMENT );
        }
        
        if (ReverseState)
        {
            --Playhead;
            --Writehead;
        }
        else
        {
            ++Playhead;
            ++Writehead;
        }
    }
    
}