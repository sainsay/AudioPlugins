#include <inttypes.h>
#include <array>

#include <iostream>

namespace sain
{
	template <typename _Ty, size_t count>
	class RingBuffer
	{
    public:
		typedef _Ty value_type;
		typedef _Ty* pointer;
		typedef const _Ty* const_pointer;
		typedef _Ty& reference;
		typedef const _Ty& const_reference;
    
		std::array<value_type, count> data;

    private:

        typedef typename std::array<_Ty, count>::iterator DataIterator;
        typedef typename std::array<value_type, count>::reverse_iterator DataReverseIterator;

		class RingBufferIterator
		{
            RingBuffer* myRingBuffer;
            DataIterator it;
		public:

            RingBufferIterator() :
                myRingBuffer(nullptr),
                it(nullptr)
            {
            }

            RingBufferIterator(RingBuffer* rb, DataIterator start) :
                myRingBuffer(rb),
                it(start)
            {
            }

            RingBufferIterator(const RingBufferIterator& rhs)
            {
                this->it = rhs.it;
                this->myRingBuffer = rhs.myRingBuffer;
            }

            RingBufferIterator& operator=(const RingBufferIterator& rhs)
            {
                this->it = rhs.it;
                this->myRingBuffer = rhs.myRingBuffer;
                return *this;
            }

            RingBufferIterator(RingBufferIterator&& rhs)
            {
                this->it = rhs.it;
                this->myRingBuffer = rhs.myRingBuffer;

                rhs.myRingBuffer = nullptr;
                rhs.it = std::_Array_iterator<_Ty, count>( nullptr );
            }

            RingBufferIterator& operator=(RingBufferIterator&& rhs)
            {
                this->it = rhs.it;
                this->myRingBuffer = rhs.myRingBuffer;

                rhs.myRingBuffer = nullptr;
                rhs.it = std::_Array_iterator<_Ty, count>( nullptr );
                return *this;
            }

            RingBufferIterator& operator++()
            {
                ++it;
                if (it == myRingBuffer->data.end())
                {
                    it = myRingBuffer->data.begin();
                }
                return *this;
            }

            RingBufferIterator& operator--(){
                if(it == myRingBuffer->data.begin())
                {
                    it = myRingBuffer->data.end();
                    --it;
                    return *this;
                }
                --it;
                return *this;
            }

            size_t operator-(const RingBufferIterator& rhs) const
            {
                return this->it - rhs.it;
            }

            reference operator*() const
            {
                return *it;
            }
		};

	public:
		using iterator = RingBufferIterator;

		RingBuffer( )
		{
            std::fill(data.begin(), data.end(), value_type());
		}

		~RingBuffer( ){}

        iterator from(size_t idx)
        {
            return iterator(this, data.begin() + idx);
        }

        iterator begin()
        {
            return iterator(this, data.begin());
        }

        iterator end()
        {
            return iterator(this, data.end());
        }

        void Clear(){
            std::fill(data.begin(), data.end(), value_type());
        }

	};

}	 // namespace sain