///
/// BSD 3-Clause License
///
/// Copyright (c) 2020, Alexandre Arsenault
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
/// * Redistributions of source code must retain the above copyright notice, this
///   list of conditions and the following disclaimer.
///
/// * Redistributions in binary form must reproduce the above copyright notice,
///   this list of conditions and the following disclaimer in the documentation
///   and/or other materials provided with the distribution.
///
/// * Neither the name of the copyright holder nor the names of its
///   contributors may be used to endorse or promote products derived from
///   this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
///

#pragma once

#ifdef __APPLE__
#include <CoreGraphics/CoreGraphics.h>
namespace fst::ui {
using native_image = CGImageRef;
// using const_native_path = CGPathRef;
// using native_path_value_type = CGFloat;
} // namespace fst::ui.
#endif // __APPLE__

namespace fst::ui {
class image {
public:
  image() = default;

  image(const char* filename) {
    CGDataProviderRef data_provider = CGDataProviderCreateWithFilename(filename);
    _image = CGImageCreateWithPNGDataProvider(data_provider, nullptr, true, kCGRenderingIntentDefault);
    CGDataProviderRelease(data_provider);
  }

  inline image(const image& img) { _image = CGImageCreateCopy(img._image); }

  inline image(image&& img) {
    _image = img._image;
    img._image = nullptr;
  }

  ~image() {
    if (_image) {
      CGImageRelease(_image);
    }
  }

  inline image& operator=(const image& img) {
    if (_image) {
      CGImageRelease(_image);
      _image = nullptr;
    }
    _image = CGImageCreateCopy(img._image);

    return *this;
  }

  inline image& operator=(image&& img) {
    if (_image) {
      CGImageRelease(_image);
      _image = nullptr;
    }
    _image = img._image;
    img._image = nullptr;
    return *this;
  }

  inline native_image get_native_image() { return _image; }

  inline native_image get_native_image() const { return _image; }

private:
  native_image _image = nullptr;
};
#ifdef __APPLE__
#endif // __APPLE__.
} // namespace fst::ui.
