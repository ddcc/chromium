// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_CPP_BINDINGS_HQ_STRUCT_PTR_H_
#define MOJO_PUBLIC_CPP_BINDINGS_HQ_STRUCT_PTR_H_

#include <hq_memory>
#include <hq_wrapper>

#include "mojo/public/cpp/bindings/struct_ptr.h"

namespace mojo {
namespace internal {

template <typename Struct>
class HQ_StructPtrWTFHelper;

}  // namespace internal

// Smart pointer wrapping a mojom structure with move-only semantics.
template <typename S>
class HQ_StructPtr {
 public:
  using Struct = S;

  // Exposing HQ_StructPtr<S>::element_type allows gmock's Pointee matcher to
  // dereference HQ_StructPtr's.
  using element_type = S;

  HQ_StructPtr() = default;
  HQ_StructPtr(std::nullptr_t) {}

  ~HQ_StructPtr() = default;

  HQ_StructPtr& operator=(std::nullptr_t) {
    reset();
    return *this;
  }

  HQ_StructPtr(StructPtr<S>&& other) { Take(&other); }
  HQ_StructPtr& operator=(StructPtr<S>&& other) {
    Take(&other);
    return *this;
  }

  HQ_StructPtr(HQ_StructPtr&& other) { Take(&other); }
  HQ_StructPtr& operator=(HQ_StructPtr&& other) {
    Take(&other);
    return *this;
  }

  template <typename... Args>
  HQ_StructPtr(base::in_place_t, Args&&... args)
      : ptr_(new Struct(std::forward<Args>(args)...)) {}

  template <typename U>
  U To() const& {
    return TypeConverter<U, HQ_StructPtr>::Convert(*this);
  }

  template <typename U>
  U To() && {
    return TypeConverter<U, HQ_StructPtr>::Convert(std::move(*this));
  }

  void reset() { ptr_.reset(); }

  bool is_null() const { return !ptr_; }

  Struct& operator*() const {
    DCHECK(ptr_);
    return *ptr_;
  }
  Struct* operator->() const {
    DCHECK(ptr_);
    return ptr_.get();
  }
  Struct* get() const { return ptr_.get(); }

  void Swap(HQ_StructPtr* other) { std::swap(ptr_, other->ptr_); }
  void Swap(StructPtr<S>* other) {
    const auto* p = other->ptr_.release();
    other = ptr_.release();
    ptr_ = p;
  }

  // Please note that calling this method will fail compilation if the value
  // type |Struct| doesn't have a Clone() method defined (which usually means
  // that it contains Mojo handles).
  StructPtr<typename S::BaseType> Clone() const {
    return is_null() ? StructPtr<typename S::BaseType>() : ptr_->Clone();
  }

  HQ_StructPtr CloneHQ() const {
    return is_null() ? HQ_StructPtr() : ptr_->CloneHQ();
  }

  // Compares the pointees (which might both be null).
  // TODO(crbug.com/735302): Get rid of Equals in favor of the operator. Same
  // for Hash.
  bool Equals(const HQ_StructPtr& other) const {
    if (is_null() || other.is_null())
      return is_null() && other.is_null();
    return ptr_->Equals(*other.ptr_);
  }

  // Hashes based on the pointee (which might be null).
  size_t Hash(size_t seed) const {
    if (is_null())
      return internal::HashCombine(seed, 0);
    return ptr_->Hash(seed);
  }

  explicit operator bool() const { return !is_null(); }

 private:
  friend class internal::HQ_StructPtrWTFHelper<Struct>;
  void Take(HQ_StructPtr* other) {
    reset();
    Swap(other);
  }

  void Take(StructPtr<S>* other) {
    reset();
    Swap(other);
  }

  std::hq_unique_ptr<Struct> ptr_;

  DISALLOW_COPY_AND_ASSIGN(HQ_StructPtr);
};

namespace internal {

template <typename Struct>
class HQ_StructPtrWTFHelper {
 public:
  static bool IsHashTableDeletedValue(const HQ_StructPtr<Struct>& value) {
    return value.ptr_.get() == reinterpret_cast<Struct*>(1u);
  }

  static void ConstructDeletedValue(mojo::HQ_StructPtr<Struct>& slot) {
    // |slot| refers to a previous, real value that got deleted and had its
    // destructor run, so this is the first time the "deleted value" has its
    // constructor called.
    //
    // Dirty trick: implant an invalid pointer in |ptr_|. Destructor isn't
    // called for deleted buckets, so this is okay.
    new (&slot) HQ_StructPtr<Struct>();
    slot.ptr_.reset(reinterpret_cast<Struct*>(1u));
  }
};

// Convenience type trait so that we can get away with defining the comparison
// operators only once.
template <typename S>
struct IsStructPtrImpl<HQ_StructPtr<S>> : std::true_type {};

}  // namespace internal

}  // namespace mojo

namespace std {

template <typename T>
struct hash<mojo::HQ_StructPtr<T>> {
  size_t operator()(const mojo::HQ_StructPtr<T>& value) const {
    return value.Hash(mojo::internal::kHashSeed);
  }
};

}  // namespace std

#endif  // MOJO_PUBLIC_CPP_BINDINGS_HQ_STRUCT_PTR_H_
