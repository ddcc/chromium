// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_NETWORK_PUBLIC_CPP_HQ_CROSS_ORIGIN_EMBEDDER_POLICY_H_
#define SERVICES_NETWORK_PUBLIC_CPP_HQ_CROSS_ORIGIN_EMBEDDER_POLICY_H_

#include <hq_string>
#include <hq_wrapper>

#include "base/hq_optional.h"
#include "services/network/public/cpp/cross_origin_embedder_policy.h"

namespace network {

// This corresponds to network::mojom::HQ_CrossOriginEmbedderPolicy.
// See the comments there.
struct COMPONENT_EXPORT(NETWORK_CPP_BASE) HQ_CrossOriginEmbedderPolicy final {
  HQ_CrossOriginEmbedderPolicy() = default;
  ~HQ_CrossOriginEmbedderPolicy() = default;
  HQ_CrossOriginEmbedderPolicy(const HQ_CrossOriginEmbedderPolicy&) = default;
  HQ_CrossOriginEmbedderPolicy(HQ_CrossOriginEmbedderPolicy&&) = default;
  HQ_CrossOriginEmbedderPolicy& operator=(const HQ_CrossOriginEmbedderPolicy&) =
      default;
  HQ_CrossOriginEmbedderPolicy& operator=(HQ_CrossOriginEmbedderPolicy&&) =
      default;

  bool operator==(const HQ_CrossOriginEmbedderPolicy& other) const {
    return value == other.value &&
           reporting_endpoint == other.reporting_endpoint &&
           report_only_value == other.report_only_value &&
           report_only_reporting_endpoint ==
               other.report_only_reporting_endpoint;
  }

  HQ_CrossOriginEmbedderPolicy& operator=(
      const CrossOriginEmbedderPolicy& other) {
    value = other.value;
    if (other.reporting_endpoint)
      reporting_endpoint = *other.reporting_endpoint;
    report_only_value = other.report_only_value;
    if (other.report_only_reporting_endpoint)
      report_only_reporting_endpoint = *other.report_only_reporting_endpoint;
    return *this;
  }

  CrossOriginEmbedderPolicy coep() const {
    CrossOriginEmbedderPolicy policy;
    policy.value = value;
    if (reporting_endpoint)
      policy.reporting_endpoint = reporting_endpoint->str();
    policy.report_only_value = report_only_value;
    if (report_only_reporting_endpoint)
      policy.report_only_reporting_endpoint =
          report_only_reporting_endpoint->str();
    return policy;
  }

  operator CrossOriginEmbedderPolicy() const {
    return coep();
  }

  std::hq_wrapper<mojom::CrossOriginEmbedderPolicyValue> value =
      mojom::CrossOriginEmbedderPolicyValue::kNone;
  base::HQ_Optional<std::hq_string> reporting_endpoint;
  std::hq_wrapper<mojom::CrossOriginEmbedderPolicyValue> report_only_value =
      mojom::CrossOriginEmbedderPolicyValue::kNone;
  base::HQ_Optional<std::hq_string> report_only_reporting_endpoint;
};

}  // namespace network

#endif  // SERVICES_NETWORK_PUBLIC_CPP_HQ_CROSS_ORIGIN_EMBEDDER_POLICY_H_
