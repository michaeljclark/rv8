//
//  queue.h
//

#ifndef rv_queue_h
#define rv_queue_h

/*
 * queue_atomic
 *
 * Multiple producer multiple consumer queue template using C++11 atomics.
 *
 * Completely lockless in the single producer single consumer case.
 *
 *   - uses 4 atomic variables: counter_back, version_back, counter_front and version_front
 *
 *   - push_back reads 3 atomics: counter_back, version_back and version_front
 *               writes 2 atomics: counter_back and version_back
 *
 *   - pop_front reads 3 atomics: counter_front, version_back and version_front
 *               writes 2 atomics: counter_front and version_front
 *
 *   - back version and front version are packed into version_back and version_front
 *
 *   - version is used for conflict detection during ordered writes
 *
 */

namespace riscv {

	#if defined(_MSC_VER)
	#define ALIGNED(x) __declspec(align(x))
	#elif defined(__GNUC__)
	#define ALIGNED(x) __attribute__((aligned(x)))
	#else
	#define ALIGNED(x)
	#endif

	template <typename T,
			  const int debug_contention = false,
			  typename ATOMIC_UINT = uint64_t,
			  const int OFFSET_BITS = 48,
			  const int VERSION_BITS = 16,
			  std::memory_order relaxed_memory_order = std::memory_order_relaxed,
			  std::memory_order acquire_memory_order = std::memory_order_acquire,
			  std::memory_order release_memory_order = std::memory_order_release>
	struct queue_atomic
	{
		/* queue atomic type */
		
		typedef ATOMIC_UINT                         atomic_uint_t;
		typedef std::atomic<T>                      atomic_item_t;
		
		
		/* queue constants */
		
		static const int tight_spin_limit =         8;
		static const int spin_limit =               1 << 24;
		static const int atomic_bits =              sizeof(atomic_uint_t) << 3;
		static const int offset_bits =              OFFSET_BITS;
		static const int version_bits =             VERSION_BITS;
		static const int offset_shift =             0;
		static const int version_shift =            offset_bits;
		static const atomic_uint_t size_max =       (1ULL << (offset_bits - 1));
		static const atomic_uint_t offset_limit =   (1ULL << offset_bits);
		static const atomic_uint_t version_limit =  (1ULL << version_bits);
		static const atomic_uint_t offset_mask =    (1ULL << offset_bits) - 1;
		static const atomic_uint_t version_mask =   (1ULL << version_bits) - 1;
		
		
		/* queue storage */
		
		ALIGNED(64) atomic_item_t *vec;
		const atomic_uint_t size_limit;
		ALIGNED(64) std::atomic<atomic_uint_t> counter_back;
		std::atomic<atomic_uint_t> version_back;
		ALIGNED(64) std::atomic<atomic_uint_t> counter_front;
		std::atomic<atomic_uint_t> version_front;
		
		
		/* queue helpers */
		
		static inline bool ispow2(size_t val) { return val && !(val & (val-1)); }
		
		/*
		 * pack a version number and an offset into an unsigned atomic integer
		 */
		static inline const atomic_uint_t pack_offset(const atomic_uint_t version, const atomic_uint_t offset)
		{
			assert(version < version_limit);
			assert(offset < offset_limit);
			return (version << version_shift) | (offset << offset_shift);
		}
		
		/*
		 * unpack a version number and offset and compare the version to a counter value
		 * returns true if the version in the counter matches the version packed in the offset
		 */
		static inline bool unpack_offsets(const atomic_uint_t counter, const atomic_uint_t pack,
										  atomic_uint_t &offset)
		{
			if (((pack >> version_shift) & version_mask) == (counter & version_mask)) {
				offset = (pack >> offset_shift) & offset_mask;
				return true;
			}
			return false;
		}
		
		/* queue implementation */
		
		atomic_uint_t _back_version()   { return (version_back >> version_shift) & version_mask; }
		atomic_uint_t _front_version()  { return (version_front >> version_shift) & version_mask; }
		atomic_uint_t _back()           { return (version_back >> offset_shift) & offset_mask; }
		atomic_uint_t _front()          { return (version_front >> offset_shift) & offset_mask; }
		size_t capacity()               { return size_limit; }
		
		
		queue_atomic(size_t size_limit) :
			size_limit(size_limit),
			counter_back(0),
			version_back(pack_offset(0, 0)),
			counter_front(0),
			version_front(pack_offset(0, size_limit))
		{
			static_assert(version_bits + offset_bits <= atomic_bits,
						  "version_bits + offset_bits must fit into atomic integer type");
			assert(size_limit > 0);
			assert(size_limit <= size_max);
			assert(ispow2(size_limit));
			vec = new atomic_item_t[size_limit]();
			assert(vec != nullptr);
		}
		
		virtual ~queue_atomic()
		{
			delete [] vec;
		}
		
		bool empty()
		{
			atomic_uint_t back = (version_back >> offset_shift) & offset_mask;
			atomic_uint_t front = (version_front >> offset_shift) & offset_mask;
			
			/* return true if queue is empty */
			return (front - back == size_limit);
		}
		
		bool full()
		{
			atomic_uint_t back = (version_back >> offset_shift) & offset_mask;
			atomic_uint_t front = (version_front >> offset_shift) & offset_mask;

			/* return true if queue is full */
			return (front == back);
		}
		
		size_t size()
		{
			atomic_uint_t back = (version_back >> offset_shift) & offset_mask;
			atomic_uint_t front = (version_front >> offset_shift) & offset_mask;

			/* return queue size */
			return (front < back) ? back - front : size_limit - front + back;
		}
		
		bool push_back(T elem)
		{
			atomic_uint_t back;
			atomic_uint_t front = (version_front >> offset_shift) & offset_mask;

			int spin_count = 0;
			do {
				/*
				 * if packed version equals counter_back then attempt push back
				 *
				 * this is where we detect if another thread is in the push back
				 * critical section and we only proceed if the versions are consistent:
				 *
				 *     i.e. counter_back == version_back >> version_shift & version_mask
				 */
				atomic_uint_t _counter_back = counter_back.load(relaxed_memory_order);
				atomic_uint_t _version_back = version_back.load(relaxed_memory_order);
				if (unpack_offsets(_counter_back, _version_back, back))
				{
					/* if (full) return false; */
					if (front == back) return false;
					
					/* create new back version */
					atomic_uint_t new_back_version = (_counter_back + 1) & version_mask;
					
					/* calculate store offset and update back */
					size_t offset = back++ & (size_limit - 1);
					
					/* pack new back version and back offset */
					atomic_uint_t pack = pack_offset(new_back_version, back & (offset_limit - 1));
					
					/*
					 * compare_exchange_weak and attempt to update the counter with the new version
					 *
					 * this is where we enter the critical section:
					 *
					 *     i.e. counter_back != version_back >> version_shift & version_mask
					 *     for a brief number of instructions until we write the new version_back
					 *
					 * if successful other threads will spin until new version_back is visible
					 * if successful we write the value followed by writing a new version_back
					 * to leave the critical section
					 */
					if (counter_back.compare_exchange_weak(_counter_back, new_back_version, std::memory_order_acq_rel))
					{
						vec[offset].store(elem, release_memory_order);

						/*
						 * exit the critical section and reveal the new back offset to other threads
						 *
						 *    i.e. counter_front == version_front >> version_shift & version_mask
						 */
						version_back.store(pack, release_memory_order);
						return true;
						
					}
				}

				/*
				 * if we reach here then we detected an inconsistent version in phase 1 prepare
				 * or failed to update the counter to enter the critical section in phase 2
				 */

				/* yield the thread before retrying */
				if (spin_limit > tight_spin_limit) {
					std::this_thread::yield();
				}
				
			} while (++spin_count < spin_limit);
			
			return false;
		}
		
		T pop_front()
		{
			atomic_uint_t back = (version_back >> offset_shift) & offset_mask;
			atomic_uint_t front;
			
			int spin_count = 0;
			do {
				/*
				 * if packed version equals counter_front then attempt pop front
				 *
				 * this is where we detect if another thread is in the pop front
				 * critical section and we only proceed if the versions are consistent:
				 *
				 *     i.e. counter_front == version_front >> version_shift & version_mask
				 */
				atomic_uint_t _counter_front = counter_front.load(relaxed_memory_order);
				atomic_uint_t _version_front = version_front.load(relaxed_memory_order);
				if (unpack_offsets(_counter_front, _version_front, front))
				{
					/* if (empty) return nullptr; */
					if (front - back == size_limit) return T(0);
					
					/* create new front version */
					atomic_uint_t new_front_version = (_counter_front + 1) & version_mask;
					
					/* calculate offset and update front */
					size_t offset = front++ & (size_limit - 1);
					
					/* pack new front version and front offset */
					atomic_uint_t pack = pack_offset(new_front_version, front & (offset_limit - 1));
					
					/*
					 * compare_exchange_weak and attempt to update the counter with the new version
					 *
					 * this is where we enter the critical section:
					 *
					 *     i.e. counter_front != version_front >> version_shift & version_mask
					 *     for a brief number of instructions until we write the new version_front
					 *
					 * if successful other threads will spin until new version_front is visible
					 * if successful we read the value followed by writing a new version_front
					 * to leave the critical section
					 */
					if (counter_front.compare_exchange_weak(_counter_front, new_front_version, std::memory_order_acq_rel))
					{
						T val = vec[offset].load(acquire_memory_order);
						
						/*
						 * exit the critical section and reveal the new front offset to other threads
						 *
						 *    i.e. counter_front == version_front >> version_shift & version_mask
						 */
						version_front.store(pack, release_memory_order);
						return val;
						
					}
				}
				
				/*
				 * if we reach here then we detected an inconsistent version in phase 1 prepare
				 * or failed to update the counter to enter the critical section in phase 2
				 */
				
				/* yield the thread before retrying */
				if (spin_limit > tight_spin_limit) {
					std::this_thread::yield();
				}
				
			} while (++spin_count < spin_limit);
			
			return T(0);
		}
	};

}

#endif
