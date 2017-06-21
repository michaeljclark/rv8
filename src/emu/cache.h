//
//  cache.h
//

#ifndef rv_cache_h
#define rv_cache_h

namespace riscv {

	/*
	 * cache_state
	 */

	enum cache_state {
		cache_state_modified  = 0b011,                   /* only copy, modified */
		cache_state_owned     = 0b110,                   /* several copies, modify permission */
		cache_state_exclusive = 0b010,                   /* only copy, unmodified */
		cache_state_shared    = 0b100,                   /* several copies, no modify permission */
		cache_state_invalid   = 0b000,                   /* not valid, must be fetched */
		cache_state_mask      = 0b111,
	};


	/*
	 * tagged_cache_entry
	 *
	 * protection domain, address space and physically tagged virtually indexed cache entry with page attributes
	 *
	 * cache[PDID:ASID:VA] = STATE:PPN:DATA
	 */

	template <typename PARAM, const size_t cache_line_size>
	struct tagged_cache_entry
	{

		typedef typename PARAM::UX UX;

		enum : UX {
			cache_line_shift =    ctz_pow2(cache_line_size),
			va_bits =             (sizeof(UX) << 3) - cache_line_shift,
			state_bits =          cache_line_shift,
			asid_bits =           PARAM::asid_bits,
			ppn_bits =            PARAM::ppn_bits,
			vcln_limit =          (1ULL<<va_bits)-1,
			ppn_limit =           (1ULL<<ppn_bits)-1,
			asid_limit =          (1ULL<<asid_bits)-1
		};

		static_assert(asid_bits + ppn_bits == 32 || asid_bits + ppn_bits == 64 ||
			asid_bits + ppn_bits == 128, "asid_bits + ppn_bits == (32, 64, 128)");

		/* Cache entry attributes */

		UX      vcln  : va_bits;       /* Virtual Cache Line Number */
		UX      state : state_bits;    /* Cache State */
		UX      ppn   : ppn_bits;      /* Physical Page Number */
		UX      asid  : asid_bits;     /* Address Space Identifier */
		pdid_t  pdid;                  /* Protection Domain Identifer */
		u8*     data;                  /* Cache Data */

		tagged_cache_entry() :
			 vcln(vcln_limit),
			 state(cache_state_invalid),
			 ppn(ppn_limit),
			 asid(asid_limit),
			 pdid(-1),
			 data(nullptr) {}

		tagged_cache_entry(UX vcln, UX asid, UX ppn) :
			vcln(vcln),
			state(cache_state_invalid),
			ppn(ppn),
			asid(asid),
			pdid(0),
			data(nullptr) {}
	};


	/*
	 * tagged_cache
	 *
	 * protection domain, address space and physically tagged, virtually indexed set associative cache
	 *
	 * cache[PDID:ASID:VA] = STATE:PPN:DATA
	 */

	template <typename PARAM, const size_t cache_size, const size_t cache_ways, const size_t cache_line_size,
		typename MEMORY = user_memory<typename PARAM::UX>>
	struct tagged_cache
	{
		static_assert(ispow2(cache_size), "cache_size must be a power of 2");
		static_assert(ispow2(cache_ways), "cache_ways must be a power of 2");
		static_assert(ispow2(cache_line_size), "cache_line_size must be a power of 2");

		typedef typename PARAM::UX UX;
		typedef MEMORY memory_type;
		typedef tagged_cache_entry<PARAM,cache_line_size> cache_entry_t;

		enum : UX {
			size =                cache_size,
			line_size =           cache_line_size,
			num_ways =            cache_ways,
			num_entries =         size / (num_ways * line_size),
			key_size =            sizeof(cache_entry_t),
			total_size =          cache_size + sizeof(cache_entry_t) * num_entries * num_ways,

			num_entries_shift =   ctz_pow2(num_entries),
			cache_line_shift =    ctz_pow2(cache_line_size),
			num_ways_shift =      ctz_pow2(num_ways),

			cache_line_mask =   ~((UX(1) << cache_line_shift) - 1),
			num_entries_mask =    (UX(1) << num_entries_shift) - 1,

			asid_bits =           PARAM::asid_bits,
			ppn_bits =            PARAM::ppn_bits
		};

		// TODO - map cache index and data into the machine address space with user_memory::add_segment

		cache_entry_t cache_key[num_entries * num_ways];
		u8 cache_data[cache_size];

		tagged_cache() : cache_key()
		{
			for (size_t i = 0; i < num_entries * num_ways; i++) {
				cache_key[i].data = cache_data + i * cache_line_size;
			}
		}

		void flush(memory_type &mem)
		{
			for (size_t i = 0; i < num_entries * num_ways; i++) {
				// TODO - flush this line to memory
				cache_key[i] = cache_entry_t();
			}
		}

		void flush(memory_type &mem, UX pdid)
		{
			for (size_t i = 0; i < num_entries * num_ways; i++) {
				if (cache_key[i].pdid != pdid) continue;
				// TODO - flush this line to memory
				cache_key[i] = cache_entry_t();
			}
		}

		void flush(memory_type &mem, UX pdid, UX asid)
		{
			for (size_t i = 0; i < num_entries * num_ways; i++) {
				if (cache_key[i].pdid != pdid && cache_key[i].asid != asid) continue;
				// TODO - flush this line to memory
				cache_key[i] = cache_entry_t();
			}
		}

		// cache line is virtually indexed but physically tagged.
		// caller has to check that the ppn of the cache line matches the TLB ppn.
		cache_entry_t* lookup_cache_line(memory_type &mem, UX pdid, UX asid, UX va)
		{
			UX vcln = va >> cache_line_shift;
			UX entry = vcln & num_entries_mask;
			cache_entry_t *ent = cache_key + (entry << num_ways_shift);
			for (size_t i = 0; i < num_ways; i++) {
				if (ent->vcln == vcln && ent->pdid == pdid && ent->asid == asid) {
					return ent;
				}
				ent++;
			}
			return nullptr;
		}

		// caller got a cache miss or invalid ppn from TLB and wants to allocate
		cache_entry_t* alloc_cache_line(memory_type &mem, UX pdid, UX asid, UX va)
		{
			// TODO - choose a way, if there is no free way, flush and allocate
		}

		// caller got a cache line with an invalid ppn and wants to invalidate
		void invalidate_cache_line(memory_type &mem, cache_entry_t *ent)
		{
			// TODO - flush to memory and invalidate cache line
		}

	};

	template <const size_t cache_size, const size_t cache_ways, const size_t cache_line_size>
	using tagged_cache_rv32 = tagged_cache<param_rv32,cache_size,cache_ways,cache_line_size>;

	template <const size_t cache_size, const size_t cache_ways, const size_t cache_line_size>
	using tagged_cache_rv64 = tagged_cache<param_rv64,cache_size,cache_ways,cache_line_size>;
}

#endif
