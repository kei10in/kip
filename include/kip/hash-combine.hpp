#ifndef KIP_HASH_COMBINE
#define KIP_HASH_COMBINE

#include <functional>


namespace kip {


template <typename T>
void hash_combine(std::size_t& seed, T const& v) {
  // see: boost::hash_combine
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}


}  // namespace kip


#endif  // KIP_HASH_COMBINE
