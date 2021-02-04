// Copyright (c) 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_HERQULES_HQ_H_
#define BASE_HERQULES_HQ_H_

#include <base/herqules_buildflags.h>

#if BUILDFLAG(USE_HERQULES)

#include <hq_wrapper>
#include <type_traits>

namespace {
// Error fallback
struct None {};

// Mapping from normal types to annotated types
template <typename T, typename U = void>
struct TypeMap {
  // using type = T;
  using type = None;
};

// Arithmetic or pointer types, and arrays thereof
template <typename T>
struct TypeMap<
    T,
    std::enable_if_t<
        std::is_scalar<T>::value ||
            (std::is_array<T>::value &&
             std::is_scalar<typename std::remove_all_extents<T>::type>::value),
        void>> {
  using type = std::hq_wrapper<T>;
};
}  // namespace

#include <memory>
#include <hq_memory>
namespace {
template <typename T>
struct TypeMap<std::unique_ptr<T>> {
  using type = std::hq_unique_ptr<typename TypeMap<T>::type>;
};
template <typename T>
struct TypeMap<std::shared_ptr<T>> {
  using type = std::hq_shared_ptr<typename TypeMap<T>::type>;
};
template <typename T>
struct TypeMap<std::weak_ptr<T>> {
  using type = std::hq_weak_ptr<typename TypeMap<T>::type>;
};
template <typename T>
struct TypeMap<std::auto_ptr<T>> {
  using type = std::hq_auto_ptr<typename TypeMap<T>::type>;
};
}  // namespace

#include <string>
#include <hq_string>
namespace {
template <typename T>
struct TypeMap<std::basic_string<T>> {
  using type = std::hq_basic_string<T>;
};
template <>
struct TypeMap<std::string> {
  using type = std::hq_string;
};
template <>
struct TypeMap<std::wstring> {
  using type = std::hq_wstring;
};
template <>
struct TypeMap<std::u16string> {
  using type = std::hq_u16string;
};
template <>
struct TypeMap<std::u32string> {
  using type = std::hq_u32string;
};
}  // namespace

#include <string_view>
#include <hq_string_view>
namespace {

template <>
struct TypeMap<std::string_view> {
  using type = std::hq_string_view;
};
template <>
struct TypeMap<std::wstring_view> {
  using type = std::hq_wstring_view;
};
template <>
struct TypeMap<std::u16string_view> {
  using type = std::hq_u16string_view;
};
template <>
struct TypeMap<std::u32string_view> {
  using type = std::hq_u32string_view;
};
}  // namespace

#include <utility>
namespace {
template <typename T1, typename T2>
struct TypeMap<std::pair<T1, T2>> {
  using type =
      std::pair<typename TypeMap<T1>::type, typename TypeMap<T2>::type>;
};
template <class... Types>
struct TypeMap<std::tuple<Types...>> {
  using type = std::tuple<typename TypeMap<Types>::type...>;
};
}  // namespace

#include <vector>
#include <hq_vector>
namespace {
template <typename T, template <typename> class A>
struct TypeMap<std::vector<T, A<T>>> {
  using type =
      std::hq_vector<typename TypeMap<T>::type, A<typename TypeMap<T>::type>>;
};
}  // namespace

// <deque>
// template <typename T, typename A>
// struct TypeMap<std::deque<T, A>> {
//   using type = std::hq_deque<typename TypeMap<T>::type, A>;
// };

// <list>
// template <typename T, typename A>
// struct TypeMap<std::list<T, A>> {
//   using type = std::hq_list<typename TypeMap<T>::type, A>;
// };

// <forward_list>
// template <typename T, typename A>
// struct TypeMap<std::forward_list<T, A>> {
//   using type = std::hq_forward_list<typename TypeMap<T>::type, A>;
// };

#include <set>
#include <hq_set>
namespace {
template <typename K, typename C, template <typename> class A>
struct TypeMap<std::set<K, C, A<K>>> {
  using type =
      std::hq_set<typename TypeMap<K>::type, C, A<typename TypeMap<K>::type>>;
};
template <typename K, typename C, template <typename> class A>
struct TypeMap<std::multiset<K, C, A<K>>> {
  using type = std::
      hq_multiset<typename TypeMap<K>::type, C, A<typename TypeMap<K>::type>>;
};
}  // namespace

#include <map>
#include <hq_map>
namespace {
template <typename K, typename T, typename C, template <typename> class A>
struct TypeMap<std::map<K, T, C, A<std::pair<const K, T>>>> {
  using type = std::hq_map<
      typename TypeMap<K>::type,
      typename TypeMap<T>::type,
      C,
      A<std::pair<const typename TypeMap<K>::type, typename TypeMap<T>::type>>>;
};
template <typename K, typename T, typename C, template <typename> class A>
struct TypeMap<std::multimap<K, T, C, A<std::pair<const K, T>>>> {
  using type = std::hq_multimap<
      typename TypeMap<K>::type,
      typename TypeMap<T>::type,
      C,
      A<std::pair<const typename TypeMap<K>::type, typename TypeMap<T>::type>>>;
};
}  // namespace

#include <unordered_set>
#include <hq_unordered_set>
namespace {
template <typename K, typename H, typename P, template <typename> class A>
struct TypeMap<std::unordered_set<K, H, P, A<K>>> {
  using type = std::hq_unordered_set<typename TypeMap<K>::type,
                                     H,
                                     P,
                                     A<typename TypeMap<K>::type>>;
};
template <typename K, typename H, typename P, template <typename> class A>
struct TypeMap<std::unordered_multiset<K, H, P, A<K>>> {
  using type = std::hq_unordered_multiset<typename TypeMap<K>::type,
                                          H,
                                          P,
                                          A<typename TypeMap<K>::type>>;
};
}  // namespace

#include <unordered_map>
#include <hq_unordered_map>
namespace {
template <typename K,
          typename T,
          typename H,
          typename P,
          template <typename>
          class A>
struct TypeMap<std::unordered_map<K, T, H, P, A<std::pair<const K, T>>>> {
  using type = std::hq_unordered_map<
      typename TypeMap<K>::type,
      typename TypeMap<T>::type,
      H,
      P,
      A<std::pair<const typename TypeMap<K>::type, typename TypeMap<T>::type>>>;
};
template <typename K,
          typename T,
          typename H,
          typename P,
          template <typename>
          class A>
struct TypeMap<std::unordered_multimap<K, T, H, P, A<std::pair<const K, T>>>> {
  using type = std::hq_unordered_multimap<
      typename TypeMap<K>::type,
      typename TypeMap<T>::type,
      H,
      P,
      A<std::pair<const typename TypeMap<K>::type, typename TypeMap<T>::type>>>;
};
}  // namespace

// Forward-declare Chrome-internal classes instead of introducing dependencies

// #include "url/gurl.h"
class GURL;
// #include "url/hq_gurl.h"
class HQ_GURL;
namespace {
template <>
struct TypeMap<GURL> {
  using type = HQ_GURL;
};
}  // namespace

namespace base {
// #include "base/optional.h"
template <typename T>
class Optional;
// #include "base/hq_optional.h"
template <typename T>
class HQ_Optional;
}  // namespace base
namespace {
template <typename T>
struct TypeMap<base::Optional<T>> {
  using type = base::HQ_Optional<typename TypeMap<T>::type>;
};
}  // namespace

namespace blink {
// #include "third_party/blink/public/common/feature_policy/feature_policy.h"
class FeaturePolicy;
}  // namespace blink
namespace {
template <>
struct TypeMap<blink::FeaturePolicy> {
  // Pass-through
  using type = blink::FeaturePolicy;
};
}  // namespace

namespace content {
// #include "content/browser/child_process_security_policy_impl.h"
// class ChildProcessSecurityPolicyImpl::SecurityState;
// #include "content/browser/site_instance_impl.h"
class SiteInfo;
}  // namespace content
namespace {
// template <>
// struct TypeMap<content::ChildProcessSecurityPolicyImpl::SecurityState> {
//   // Pass-through
//   using type = content::ChildProcessSecurityPolicyImpl::SecurityState;
// };
template <>
struct TypeMap<content::SiteInfo> {
  // Pass-through
  using type = content::SiteInfo;
};
}  // namespace

// #include "mojo/public/cpp/bindings/hq_struct_ptr.h"
// #include "mojo/public/cpp/bindings/struct_ptr.h"
namespace mojo {
template <typename S>
class StructPtr;
template <typename S>
class HQ_StructPtr;
}  // namespace mojo
namespace {
template <typename T>
struct TypeMap<mojo::StructPtr<T>> {
  using type = mojo::HQ_StructPtr<typename TypeMap<T>::type>;
};
}  // namespace

namespace net {
// #include "net/base/isolation_info.h"
class IsolationInfo;
}  // namespace net
namespace {
template <>
struct TypeMap<net::IsolationInfo> {
  // Pass-through
  using type = net::IsolationInfo;
};
}  // namespace

namespace network {
// #include "services/network/public/cpp/cross_origin_embedder_policy.h"
struct CrossOriginEmbedderPolicy;
// #include "services/network/public/cpp/hq_cross_origin_embedder_policy.h"
struct HQ_CrossOriginEmbedderPolicy;
// #include "services/network/public/cpp/cross_origin_opener_policy.h"
struct CrossOriginOpenerPolicy;
// #include "services/network/public/cpp/hq_cross_origin_opener_policy.h"
struct HQ_CrossOriginOpenerPolicy;

namespace mojom {
// #include "services/network/public/mojom/client_security_state.mojom.h"
class ClientSecurityState;
// #include "services/network/public/cpp/hq_client_security_state.h"
class HQ_ClientSecurityState;
}  // namespace mojom
}  // namespace network
namespace {
template <>
struct TypeMap<network::CrossOriginEmbedderPolicy> {
  using type = network::HQ_CrossOriginEmbedderPolicy;
};
template <>
struct TypeMap<network::CrossOriginOpenerPolicy> {
  using type = network::HQ_CrossOriginOpenerPolicy;
};
template <>
struct TypeMap<network::mojom::ClientSecurityState> {
  using type = network::mojom::HQ_ClientSecurityState;
};
}  // namespace

namespace url {
// #include "url/third_party/mozilla/url_parse.h"
struct Component;
struct Parsed;
// #include "url/third_party/mozilla/hq_url_parse.h"
struct HQ_Component;
struct HQ_Parsed;
// #include "url/origin.h"
class Origin;
// #include "url/scheme_host_port.h"
class SchemeHostPort;
}  // namespace url
namespace {
template <>
struct TypeMap<url::Component> {
  using type = url::HQ_Component;
};
template <>
struct TypeMap<url::Parsed> {
  using type = url::HQ_Parsed;
};
template <>
struct TypeMap<url::Origin> {
  // Pass-through
  using type = url::Origin;
};
template <>
struct TypeMap<url::SchemeHostPort> {
  // Pass-through
  using type = url::SchemeHostPort;
};
}  // namespace

namespace util {
// #include "base/util/type_safety/id_type.h"
template <typename T, typename W, W>
class IdType;
}  // namespace util
namespace {
template <typename T, typename W, W kInvalidValue>
struct TypeMap<util::IdType<T, W, kInvalidValue>> {
  // Pass-through
  using type = util::IdType<T, W, kInvalidValue>;
};
}  // namespace

namespace base {
namespace HerQules {
template <typename T>
using HQ = typename TypeMap<T>::type;
}  // namespace HerQules
}  // namespace base

#else

namespace base {
namespace HerQules {
template <typename T>
using HQ = T;
}
}  // namespace base

#endif

#endif
