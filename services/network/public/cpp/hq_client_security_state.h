// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_CPP_BINDINGS_HQ_CLIENT_SECURITY_STATE_H_
#define MOJO_PUBLIC_CPP_BINDINGS_HQ_CLIENT_SECURITY_STATE_H_

#include <hq_wrapper>

#include "mojo/public/cpp/bindings/hq_struct_ptr.h"
#include "services/network/public/cpp/hq_cross_origin_embedder_policy.h"
#include "services/network/public/mojom/client_security_state.mojom.h"

namespace network {
namespace mojom {

// How to treat private network requests.
//
// Private network requests are any requests to a resource served by a
// non-public IP address.
//
// See the CORS-RFC1918 spec for details: https://wicg.github.io/cors-rfc1918.

class HQ_ClientSecurityState;
using HQ_ClientSecurityStatePtr = mojo::HQ_StructPtr<HQ_ClientSecurityState>;

class HQ_ClientSecurityState {
 public:
  using BaseType = ClientSecurityState;

  HQ_ClientSecurityState() = default;
  HQ_ClientSecurityState(const HQ_CrossOriginEmbedderPolicy& coep,
                         bool iwsc,
                         mojom::IPAddressSpace ias,
                         mojom::PrivateNetworkRequestPolicy pnrp)
      : cross_origin_embedder_policy(coep),
        is_web_secure_context(iwsc),
        ip_address_space(ias),
        private_network_request_policy(pnrp) {}
  ~HQ_ClientSecurityState() = default;

  template <typename... Args>
  static HQ_ClientSecurityStatePtr New(Args&&... args) {
    return HQ_ClientSecurityStatePtr(base::in_place,
                                     std::forward<Args>(args)...);
  }

  template <typename StructPtrType = ClientSecurityState>
  ClientSecurityStatePtr Clone() const {
    return ClientSecurityState::New(
        mojo::Clone(cross_origin_embedder_policy.coep()),
        mojo::Clone(is_web_secure_context.v()),
        mojo::Clone(ip_address_space.v()),
        mojo::Clone(private_network_request_policy.v()));
  }

  template <typename StructPtrType = HQ_ClientSecurityState>
  HQ_ClientSecurityStatePtr CloneHQ() const {
    return New(mojo::Clone(cross_origin_embedder_policy),
               mojo::Clone(is_web_secure_context.v()),
               mojo::Clone(ip_address_space.v()),
               mojo::Clone(private_network_request_policy.v()));
  }

  HQ_ClientSecurityState& operator=(const mojom::ClientSecurityState& other) {
    cross_origin_embedder_policy = other.cross_origin_embedder_policy;
    is_web_secure_context = other.is_web_secure_context;
    ip_address_space = other.ip_address_space;
    private_network_request_policy = other.private_network_request_policy;
    return *this;
  }

  mojom::ClientSecurityState css() const {
    mojom::ClientSecurityState css;
    css.cross_origin_embedder_policy = cross_origin_embedder_policy.coep();
    css.is_web_secure_context = is_web_secure_context;
    css.ip_address_space = ip_address_space;
    css.private_network_request_policy = private_network_request_policy;
    return css;
  }

  // See: https://html.spec.whatwg.org/multipage/origin.html#coep
  HQ_CrossOriginEmbedderPolicy cross_origin_embedder_policy;

  // Whether the initiator of the requests is in a web secure context.
  // See: https://developer.mozilla.org/en-US/docs/Web/Security/Secure_Contexts
  std::hq_wrapper<bool> is_web_secure_context;

  // The initiator's IP AddressSpace.
  std::hq_wrapper<mojom::IPAddressSpace> ip_address_space =
      mojom::IPAddressSpace::kUnknown;

  // The policy to apply to private network requests.
  std::hq_wrapper<mojom::PrivateNetworkRequestPolicy>
      private_network_request_policy =
          mojom::PrivateNetworkRequestPolicy::kAllow;
};

}  // namespace mojom
}  // namespace network

#endif
