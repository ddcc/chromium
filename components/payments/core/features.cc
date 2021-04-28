// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "components/payments/core/features.h"

#include "build/build_config.h"

namespace payments {
namespace features {

const base::Feature __attribute__((no_destroy)) kWebPaymentsExperimentalFeatures{
    "WebPaymentsExperimentalFeatures", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kReturnGooglePayInBasicCard{
    "ReturnGooglePayInBasicCard", base::FEATURE_DISABLED_BY_DEFAULT};

#if defined(OS_IOS)
const base::Feature __attribute__((no_destroy)) kWebPaymentsNativeApps{"WebPaymentsNativeApps",
                                           base::FEATURE_DISABLED_BY_DEFAULT};
#endif

// TODO(rouslan): Remove this.
const base::Feature __attribute__((no_destroy)) kWebPaymentsMethodSectionOrderV2{
    "WebPaymentsMethodSectionOrderV2", base::FEATURE_DISABLED_BY_DEFAULT};

// TODO(rouslan): Remove this.
const base::Feature __attribute__((no_destroy)) kWebPaymentsModifiers{"WebPaymentsModifiers",
                                          base::FEATURE_ENABLED_BY_DEFAULT};

// TODO(rouslan): Remove this.
const base::Feature __attribute__((no_destroy)) kWebPaymentsSingleAppUiSkip{
    "WebPaymentsSingleAppUiSkip", base::FEATURE_ENABLED_BY_DEFAULT};

// TODO(rouslan): Remove this.
const base::Feature __attribute__((no_destroy)) kWebPaymentsJustInTimePaymentApp{
    "WebPaymentsJustInTimePaymentApp", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAlwaysAllowJustInTimePaymentApp{
    "AlwaysAllowJustInTimePaymentApp", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kWebPaymentsRedactShippingAddress{
    "WebPaymentsRedactShippingAddress", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAppStoreBilling {
  "AppStoreBilling",
#if defined(OS_ANDROID)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif  // OS_ANDROID
};

const base::Feature __attribute__((no_destroy)) kAppStoreBillingDebug{"AppStoreBillingDebug",
                                          base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kStrictHasEnrolledAutofillInstrument{
    "StrictHasEnrolledAutofillInstrument", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kPaymentRequestSkipToGPay{
    "PaymentRequestSkipToGPay", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kPaymentRequestSkipToGPayIfNoCard{
    "PaymentRequestSkipToGPayIfNoCard", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kDownRankJustInTimePaymentApp{
    "DownRankJustInTimePaymentApp", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kPaymentHandlerPopUpSizeWindow{
    "PaymentHandlerPopUpSizeWindow", base::FEATURE_ENABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kAllowJITInstallationWhenAppIconIsMissing{
    "AllowJITInstallationWhenAppIconIsMissing",
    base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kPaymentHandlerSecurityIcon{
    "PaymentHandlerSecurityIcon", base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kEnforceFullDelegation{"EnforceFullDelegation",
                                           base::FEATURE_DISABLED_BY_DEFAULT};

const base::Feature __attribute__((no_destroy)) kSecurePaymentConfirmation {
  "SecurePaymentConfirmation",
#if defined(OS_MAC)
      base::FEATURE_ENABLED_BY_DEFAULT
#else
      base::FEATURE_DISABLED_BY_DEFAULT
#endif  // OS_MAC
};

}  // namespace features
}  // namespace payments
