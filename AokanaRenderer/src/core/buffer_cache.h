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
			// ��� cache ���Ѿ����ں� buf ����ͬ������, ��ֱ�ӷ���ָ�����ݵ�ָ��
			Buffer lookup_buffer(buf.data, buf.size());
			int shard_index = uint32_t(lookup_buffer.hash) >> (32 - log_shards);
			mutex[shard_index].lock_shared();
			if (auto iter = cache[shard_index].find(lookup_buffer); iter != cache[shard_index].end()) {
				const T* ptr = iter->ptr;
				mutex[shard_index].unlock_shared();
				return ptr;
			}
			// ����� buf �����ݼ��� cache, Ȼ�󷵻� cache �е�ָ��
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
		static constexpr int n_shards = 1 << log_shards;	// cache ����Ϊ 2^6 = 64 �� shard
		std::shared_mutex mutex[n_shards];					// ÿ�� shard ���Լ��� mutex, ����߳̿��Բ�������
		std::unordered_set<Buffer, BufferHasher> cache[n_shards];
	};


	extern BufferCache<int>* g_int_buffer_cache;
	extern BufferCache<Point3>* g_point3_buffer_cache;
}