#pragma once

namespace pene {
  namespace pin_utils {

    template<class A, class B> struct is_same { static constexpr bool value = false; };
    template<class A> struct is_same<A, A> { static constexpr bool value = true; };
  }
}
