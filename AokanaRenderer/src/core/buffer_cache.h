#pragma once
#include "hash.h"
#include "vec.h"
#include <vector>
#include <span>
#include <mutex>
#include <shared_mutex>
#include <unordered_set>

namespace Aokana {
	
	template<typename T>
	class BufferCache {
	public:
		struct Buffer {
			Buffer(const T* ptr, size_t size) : ptr(ptr), size(size) {
				hash = HashBuffer(ptr, size);
			}

			bool operator==(const Buffer& b) const {
				return size == b.size && hash == b.hash && std::memcmp(ptr, b.ptr, size * sizeof(T)) == 0;
			}

			const T* ptr{ nullptr };
			size_t size{ 0 };
			size_t hash;
		};
		
		struct BufferHasher {
			size_t operator()(const Buffer& b) const {
				return b.hash;
			}
		};

		const T* LookupOrAdd(std::span<const T> buf) {
			// 如果 cache 中已经存在和 buf 中相同的数据, 就直接返回指向数据的指针
			Buffer lookup_buffer(buf.data, buf.size());
			int shard_index = uint32_t(lookup_buffer.hash) >> (32 - log_shards);
			mutex[shard_index].lock_shared();
			if (auto iter = cache[shard_index].find(lookup_buffer); iter != cache[shard_index].end()) {
				const T* ptr = iter->ptr;
				mutex[shard_index].unlock_shared();
				return ptr;
			}
			// 否则把 buf 的数据加入 cache, 然后返回 cache 中的指针
			mutex[shard_index].unlock_shared();
			T* ptr = new T[buf.size()];
			std::copy(buf.begin(), buf.end(), ptr);
			mutex[shard_index].lock();
			cache[shard_index].insert(Buffer(ptr, buf.size()));
			mutex[shard_index].unlock();
			return ptr;
		}


	private:
		static constexpr int log_shards = 6;
		static constexpr int n_shards = 1 << log_shards;	// cache 被分为 2^6 = 64 个 shard
		std::shared_mutex mutex[n_shards];					// 每个 shard 有自己的 mutex, 多个线程可以并发访问
		std::unordered_set<Buffer, BufferHasher> cache[n_shards];
	};


	extern BufferCache<int>* g_int_buffer_cache;
	extern BufferCache<Point3>* g_point3_buffer_cache;
}