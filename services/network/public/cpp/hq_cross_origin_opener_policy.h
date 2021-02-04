// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SERVICES_NETWORK_PUBLIC_CPP_HQ_CROSS_ORIGIN_OPENER_POLICY_H_
#define SERVICES_NETWORK_PUBLIC_CPP_HQ_CROSS_ORIGIN_OPENER_POLICY_H_

#include <hq_string>
#include <hq_wrapper>

#include "base/hq_optional.h"
#include "services/network/public/cpp/cross_origin_opener_policy.h"

namespace network {

// This corresponds to network::mojom::HQ_CrossOriginOpenerPolicy.
// See the comments there.
struct COMPONENT_EXPORT(NETWORK_CPP_BASE) HQ_CrossOriginOpenerPolicy final {
  HQ_CrossOriginOpenerPolicy() = default;
  ~HQ_CrossOriginOpenerPolicy() = default;
  HQ_CrossOriginOpenerPolicy(const HQ_CrossOriginOpenerPolicy&) = default;
  HQ_CrossOriginOpenerPolicy(HQ_CrossOriginOpenerPolicy&&) = default;
  HQ_CrossOriginOpenerPolicy& operator=(const HQ_CrossOriginOpenerPolicy&) =
      default;
  HQ_CrossOriginOpenerPolicy& operator=(HQ_CrossOriginOpenerPolicy&&) = default;

  bool operator==(const HQ_CrossOriginOpenerPolicy& other) const {
    return value == other.value &&
           reporting_endpoint == other.reporting_endpoint &&
           report_only_value == other.report_only_value &&
           report_only_reporting_endpoint ==
               other.report_only_reporting_endpoint;
  }

  HQ_CrossOriginOpenerPolicy& operator=(const CrossOriginOpenerPolicy& other) {
    value = other.value;
    if (other.reporting_endpoint)
      reporting_endpoint = *other.reporting_endpoint;
    report_only_value = other.report_only_value;
    if (other.report_only_reporting_endpoint)
      report_only_reporting_endpoint = *other.report_only_reporting_endpoint;
    return *this;
  }

  CrossOriginOpenerPolicy coop() const {
    CrossOriginOpenerPolicy policy;
    policy.value = value;
    if (reporting_endpoint)
      policy.reporting_endpoint = reporting_endpoint->str();
    policy.report_only_value = report_only_value;
    if (report_only_reporting_endpoint)
      policy.report_only_reporting_endpoint =
          report_only_reporting_endpoint->str();
    return policy;
  }

  operator CrossOriginOpenerPolicy() const {
    return coop();
  }

  std::hq_wrapper<mojom::CrossOriginOpenerPolicyValue> value =
      mojom::CrossOriginOpenerPolicyValue::kUnsafeNone;
  base::HQ_Optional<std::hq_string> reporting_endpoint;
  std::hq_wrapper<mojom::CrossOriginOpenerPolicyValue> report_only_value =
      mojom::CrossOriginOpenerPolicyValue::kUnsafeNone;
  base::HQ_Optional<std::hq_string> report_only_reporting_endpoint;
};

COMPONENT_EXPORT(NETWORK_CPP_BASE)
bool IsAccessFromCoopPage(mojom::CoopAccessReportType);

COMPONENT_EXPORT(NETWORK_CPP_BASE)
const char* CoopAccessReportTypeToString(mojom::CoopAccessReportType type);

}  // namespace network

#endif  // SERVICES_NETWORK_PUBLIC_CPP_HQ_CROSS_ORIGIN_OPENER_POLICY_H_
