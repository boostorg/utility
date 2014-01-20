/*
   Copyright (c) Marshall Clow 2012-2012.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org

    Based on the StringRef implementation in LLVM (http://llvm.org) and
    N3422 by Jeffrey Yasskin
        http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3442.html

*/

#ifndef BOOST_STRING_REF_HPP
#define BOOST_STRING_REF_HPP

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/utility/string_ref_fwd.hpp>
#include <boost/throw_exception.hpp>

#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <string>
#include <iosfwd>

namespace boost {

    namespace detail {
    //  A helper functor because sometimes we don't have lambdas
        template <typename charT, typename traits>
        class string_ref_traits_eq {
        public:
            string_ref_traits_eq ( charT ch ) : ch_(ch) {}
            bool operator () ( charT val ) const { return traits::eq ( ch_, val ); }
            charT ch_;
            };
        }

    template<typename charT, typename traits>
    class basic_string_ref {
        bool inline is_cleared_ () const { return !ptr_; }
    public:
        // types
        typedef traits traits_type;
        typedef charT value_type;
        typedef const charT* pointer;
        typedef const charT* const_pointer;
        typedef const charT& reference;
        typedef const charT& const_reference;
        typedef pointer const_iterator; // impl-defined
        typedef const_iterator iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef const_reverse_iterator reverse_iterator;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        static BOOST_CONSTEXPR_OR_CONST size_type npos = size_type(-1);

        // construct/copy
        BOOST_CONSTEXPR basic_string_ref () BOOST_NOEXCEPT
            : ptr_(NULL), len_(0) {}

        BOOST_CONSTEXPR basic_string_ref (const basic_string_ref &rhs) BOOST_NOEXCEPT :
            ptr_(rhs.ptr_),
            len_(rhs.len_) { }

        basic_string_ref& operator=(const basic_string_ref &rhs) BOOST_NOEXCEPT {
            ptr_ = rhs.ptr_;
            len_ = rhs.len_;
            return *this;
            }

        basic_string_ref(const charT* str)
            : ptr_(str), len_(traits::length(str)) {}

        template<typename Allocator>
        basic_string_ref(const std::basic_string<charT, traits, Allocator>& str) BOOST_NOEXCEPT
            : ptr_(str.data()), len_(str.length()) {}

        BOOST_CONSTEXPR basic_string_ref(const charT* str, size_type len)
            : ptr_(str), len_(len) {}

#ifndef BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
        template<typename Allocator>
        explicit operator std::basic_string<charT, traits, Allocator>() const {
            return std::basic_string<charT, traits, Allocator> ( ptr_, len_ );
            }
#endif

        std::basic_string<charT, traits> to_string () const {
            return std::basic_string<charT, traits> ( ptr_, len_ );
            }

        size_type copy(charT* s, size_type n, size_type pos=0) const {
            if ( pos > size() )
                BOOST_THROW_EXCEPTION( std::out_of_range ( "string_ref::copy" ) );
            size_t rlen = size() - pos;
            if ( n < rlen )
                rlen = n;
            else
                n = rlen;
            const_iterator first = begin() + pos;
            for ( ; n > 0; --n, ++first, ++s )
                *s = *first;
            return rlen;
        }

        // iterators
        BOOST_CONSTEXPR const_iterator   begin() const BOOST_NOEXCEPT { return ptr_; }
        BOOST_CONSTEXPR const_iterator  cbegin() const BOOST_NOEXCEPT { return ptr_; }
        BOOST_CONSTEXPR const_iterator     end() const BOOST_NOEXCEPT { return ptr_ + len_; }
        BOOST_CONSTEXPR const_iterator    cend() const BOOST_NOEXCEPT { return ptr_ + len_; }
                const_reverse_iterator  rbegin() const BOOST_NOEXCEPT { return const_reverse_iterator (end()); }
                const_reverse_iterator crbegin() const BOOST_NOEXCEPT { return const_reverse_iterator (end()); }
                const_reverse_iterator    rend() const BOOST_NOEXCEPT { return const_reverse_iterator (begin()); }
                const_reverse_iterator   crend() const BOOST_NOEXCEPT { return const_reverse_iterator (begin()); }

        // capacity
        BOOST_CONSTEXPR size_type size()     const BOOST_NOEXCEPT { return len_; }
        BOOST_CONSTEXPR size_type length()   const BOOST_NOEXCEPT { return len_; }
        BOOST_CONSTEXPR size_type max_size() const BOOST_NOEXCEPT { return len_; }
        BOOST_CONSTEXPR bool empty()         const BOOST_NOEXCEPT { return len_ == 0; }

        // element access
        BOOST_CONSTEXPR const charT& operator[](size_type pos) const { return ptr_[pos]; }

        const charT& at(size_t pos) const {
            if ( pos >= len_ )
                BOOST_THROW_EXCEPTION( std::out_of_range ( "boost::string_ref::at" ) );
            return ptr_[pos];
            }

        BOOST_CONSTEXPR const charT& front() const { return ptr_[0]; }
        BOOST_CONSTEXPR const charT& back()  const { return ptr_[len_-1]; }
        BOOST_CONSTEXPR const charT* data()  const BOOST_NOEXCEPT { return ptr_ ? ptr_ : &nul_; }

        // modifiers
        void clear() BOOST_NOEXCEPT { len_ = 0; ptr_ = NULL; }
        void remove_prefix(size_type n) {
            if ( n > len_ )
                n = len_;
            ptr_ += n;
            len_ -= n;
            }

        void remove_suffix(size_type n) {
            if ( n > len_ )
                n = len_;
            len_ -= n;
            }

        void swap(basic_string_ref& s) BOOST_NOEXCEPT {
            const charT *ptr = ptr_;
            std::size_t len = len_;
            ptr_ = s.ptr_;
            s.ptr_ = ptr;
            len_ = s.len_;
            s.len_ = len;
        }


        // basic_string_ref string operations
        basic_string_ref substr(size_type pos=0, size_type n=npos) const {
            if ( pos > size())
                BOOST_THROW_EXCEPTION( std::out_of_range ( "string_ref::substr" ) );
            if ( n == npos || pos + n > size())
                n = size () - pos;
            const charT * np = ptr_ ? (ptr_ + pos) : NULL;
            return basic_string_ref ( np, n );
            }

        int compare(basic_string_ref x) const BOOST_NOEXCEPT {
            const int cmp = traits::compare ( ptr_, x.ptr_, (std::min)(len_, x.len_));
            return cmp != 0 ? cmp : ( len_ == x.len_ ? 0 : len_ < x.len_ ? -1 : 1 );
            }
        int compare(size_type pos1, size_type n1, basic_string_ref s) const {
            return substr(pos1, n1).compare(s);
            }
        int compare(size_type pos1, size_type n1, basic_string_ref s,
                    size_type pos2, size_type n2) const {
            return substr(pos1, n1).compare(s.substr(pos2, n2));
            }
        int compare(const charT* s) const {
            return compare(basic_string_ref(s));
            }
        int compare(size_type pos1, size_type n1, const charT* s) const {
            return substr(pos1, n1).compare(basic_string_ref(s));
            }
        int compare(size_type pos1, size_type n1, const charT* s, size_type n2) const {
            return substr(pos1, n1).compare(basic_string_ref(s, n2));
            }

        bool starts_with(charT c) const { return !empty() && traits::eq ( c, front()); }
        bool starts_with(basic_string_ref x) const {
            return len_ >= x.len_ && traits::compare ( ptr_, x.ptr_, x.len_ ) == 0;
            }

        bool ends_with(charT c) const { return !empty() && traits::eq ( c, back()); }
        bool ends_with(basic_string_ref x) const {
            return len_ >= x.len_ && traits::compare ( ptr_ + len_ - x.len_, x.ptr_, x.len_ ) == 0;
            }

        size_type find(basic_string_ref s, size_type pos=0) const BOOST_NOEXCEPT {
            if ( pos + s.size() > size() )
                return npos;
            const_iterator iter = std::search ( this->cbegin () + pos, this->cend (),
                                                s.cbegin (), s.cend (), traits::eq );
            return iter == this->cend () ? npos : std::distance ( this->cbegin (), iter );
            }

        size_type find(charT c, size_type pos=0) const BOOST_NOEXCEPT {
            if ( pos >= size() )
                return npos;
            const_iterator iter = std::find_if ( this->cbegin () + pos, this->cend (),
                                    detail::string_ref_traits_eq<charT, traits> ( c ));
            return iter == this->cend () ? npos : std::distance ( this->cbegin (), iter );
            }

        size_type rfind(basic_string_ref s, size_type pos=npos) const BOOST_NOEXCEPT {
            if ( ( pos < npos ) && ( pos + s.size() <= size() ) )
                pos += s.size() - 1;
            const_reverse_iterator iter = std::search ( this->offset_crbegin_ (pos), this->crend (),
                                                s.crbegin (), s.crend (), traits::eq );
            return iter == this->crend () ? npos : ( std::distance( iter, this->crend() ) - s.len_ );
            }

        size_type rfind(charT c, size_type pos=npos) const BOOST_NOEXCEPT {
            const_reverse_iterator iter = std::find_if ( this->offset_crbegin_ (pos), this->crend (),
                                    detail::string_ref_traits_eq<charT, traits> ( c ));
            return iter == this->crend () ? npos : ( std::distance( iter, this->crend() ) - 1 );
            }

        size_type find_first_of(charT c, size_type pos=0) const BOOST_NOEXCEPT {
            return find (c, pos);
            }
        size_type find_last_of(charT c, size_type pos=npos) const BOOST_NOEXCEPT {
            return rfind (c, pos);
            }

        size_type find_first_of(basic_string_ref s, size_type pos=0) const BOOST_NOEXCEPT {
            if ( pos >= size() )
                return npos;
            const_iterator iter = std::find_first_of
                ( this->cbegin () + pos, this->cend (), s.cbegin (), s.cend (), traits::eq );
            return iter == this->cend () ? npos : std::distance ( this->cbegin (), iter );
            }

        size_type find_last_of(basic_string_ref s, size_type pos=npos) const BOOST_NOEXCEPT {
            const_reverse_iterator iter = std::find_first_of
                ( this->offset_crbegin_ (pos), this->crend (), s.cbegin (), s.cend (), traits::eq );
            return iter == this->crend () ? npos : ( std::distance ( iter, this->crend () ) - 1 );
            }

        size_type find_first_not_of(basic_string_ref s, size_type pos=0) const BOOST_NOEXCEPT {
            if ( pos >= size() )
                return npos;
            const_iterator iter = find_not_of ( this->cbegin () + pos, this->cend (), s );
            return iter == this->cend () ? npos : std::distance ( this->cbegin (), iter );
            }

        size_type find_first_not_of(charT c, size_type pos=0) const BOOST_NOEXCEPT {
            if ( pos >= size() )
                return npos;
            for ( const_iterator iter = this->cbegin () + pos; iter != this->cend (); ++iter )
                if ( !traits::eq ( c, *iter ))
                    return std::distance ( this->cbegin (), iter );
            return npos;
            }

        size_type find_last_not_of(basic_string_ref s, size_type pos=npos) const BOOST_NOEXCEPT {
            const_reverse_iterator iter = find_not_of ( this->offset_crbegin_ (pos), this->crend (), s );
            return iter == this->crend () ? npos : ( std::distance ( iter, this->crend () ) - 1 );
            }

        size_type find_last_not_of(charT c, size_type pos=npos) const BOOST_NOEXCEPT {
            for ( const_reverse_iterator iter = this->offset_crbegin_ (pos); iter != this->crend (); ++iter )
                if ( !traits::eq ( c, *iter ))
                    return std::distance ( iter, this->crend () ) - 1;
            return npos;
            }

    private:
        template <typename Iterator>
        Iterator find_not_of ( Iterator first, Iterator last, basic_string_ref s ) const {
            for ( ; first != last ; ++first )
                if ( 0 == traits::find ( s.ptr_, s.len_, *first ))
                    return first;
            return last;
            }

        const_reverse_iterator offset_crbegin_ ( size_type pos ) const {
            const_reverse_iterator first = this->crbegin();
            if ( pos < size() )
                first += (size() - 1 - pos);
            return first;
            }

        const charT *ptr_;
        union {
            std::size_t len_;
            charT nul_;
            };
        };


//  Comparison operators
//  Equality
    template<typename charT, typename traits>
    inline bool operator==(basic_string_ref<charT, traits> x, basic_string_ref<charT, traits> y) {
        if ( x.size () != y.size ()) return false;
        return x.compare(y) == 0;
        }

    template<typename charT, typename traits, typename Allocator>
    inline bool operator==(basic_string_ref<charT, traits> x, const std::basic_string<charT, traits, Allocator> & y) {
        return x == basic_string_ref<charT, traits>(y);
        }

    template<typename charT, typename traits, typename Allocator>
    inline bool operator==(const std::basic_string<charT, traits, Allocator> & x, basic_string_ref<charT, traits> y) {
        return basic_string_ref<charT, traits>(x) == y;
        }

    template<typename charT, typename traits>
    inline bool operator==(basic_string_ref<charT, traits> x, const charT * y) {
        return x == basic_string_ref<charT, traits>(y);
        }

    template<typename charT, typename traits>
    inline bool operator==(const charT * x, basic_string_ref<charT, traits> y) {
        return basic_string_ref<charT, traits>(x) == y;
        }

//  Inequality
    template<typename charT, typename traits>
    inline bool operator!=(basic_string_ref<charT, traits> x, basic_string_ref<charT, traits> y) {
        if ( x.size () != y.size ()) return true;
        return x.compare(y) != 0;
        }

    template<typename charT, typename traits, typename Allocator>
    inline bool operator!=(basic_string_ref<charT, traits> x, const std::basic_string<charT, traits, Allocator> & y) {
        return x != basic_string_ref<charT, traits>(y);
        }

    template<typename charT, typename traits, typename Allocator>
    inline bool operator!=(const std::basic_string<charT, traits, Allocator> & x, basic_string_ref<charT, traits> y) {
        return basic_string_ref<charT, traits>(x) != y;
        }

    template<typename charT, typename traits>
    inline bool operator!=(basic_string_ref<charT, traits> x, const charT * y) {
        return x != basic_string_ref<charT, traits>(y);
        }

    template<typename charT, typename traits>
    inline bool operator!=(const charT * x, basic_string_ref<charT, traits> y) {
        return basic_string_ref<charT, traits>(x) != y;
        }

//  Less than
    template<typename charT, typename traits>
    inline bool operator<(basic_string_ref<charT, traits> x, basic_string_ref<charT, traits> y) {
        return x.compare(y) < 0;
        }

    template<typename charT, typename traits, typename Allocator>
    inline bool operator<(basic_string_ref<charT, traits> x, const std::basic_string<charT, traits, Allocator> & y) {
        return x < basic_string_ref<charT, traits>(y);
        }

    template<typename charT, typename traits, typename Allocator>
    inline bool operator<(const std::basic_string<charT, traits, Allocator> & x, basic_string_ref<charT, traits> y) {
        return basic_string_ref<charT, traits>(x) < y;
        }

    template<typename charT, typename traits>
    inline bool operator<(basic_string_ref<charT, traits> x, const charT * y) {
        return x < basic_string_ref<charT, traits>(y);
        }

    template<typename charT, typename traits>
    inline bool operator<(const charT * x, basic_string_ref<charT, traits> y) {
        return basic_string_ref<charT, traits>(x) < y;
        }

//  Greater than
    template<typename charT, typename traits>
    inline bool operator>(basic_string_ref<charT, traits> x, basic_string_ref<charT, traits> y) {
        return x.compare(y) > 0;
        }

    template<typename charT, typename traits, typename Allocator>
    inline bool operator>(basic_string_ref<charT, traits> x, const std::basic_string<charT, traits, Allocator> & y) {
        return x > basic_string_ref<charT, traits>(y);
        }

    template<typename charT, typename traits, typename Allocator>
    inline bool operator>(const std::basic_string<charT, traits, Allocator> & x, basic_string_ref<charT, traits> y) {
        return basic_string_ref<charT, traits>(x) > y;
        }

    template<typename charT, typename traits>
    inline bool operator>(basic_string_ref<charT, traits> x, const charT * y) {
        return x > basic_string_ref<charT, traits>(y);
        }

    template<typename charT, typename traits>
    inline bool operator>(const charT * x, basic_string_ref<charT, traits> y) {
        return basic_string_ref<charT, traits>(x) > y;
        }

//  Less than or equal to
    template<typename charT, typename traits>
    inline bool operator<=(basic_string_ref<charT, traits> x, basic_string_ref<charT, traits> y) {
        return x.compare(y) <= 0;
        }

    template<typename charT, typename traits, typename Allocator>
    inline bool operator<=(basic_string_ref<charT, traits> x, const std::basic_string<charT, traits, Allocator> & y) {
        return x <= basic_string_ref<charT, traits>(y);
        }

    template<typename charT, typename traits, typename Allocator>
    inline bool operator<=(const std::basic_string<charT, traits, Allocator> & x, basic_string_ref<charT, traits> y) {
        return basic_string_ref<charT, traits>(x) <= y;
        }

    template<typename charT, typename traits>
    inline bool operator<=(basic_string_ref<charT, traits> x, const charT * y) {
        return x <= basic_string_ref<charT, traits>(y);
        }

    template<typename charT, typename traits>
    inline bool operator<=(const charT * x, basic_string_ref<charT, traits> y) {
        return basic_string_ref<charT, traits>(x) <= y;
        }

//  Greater than or equal to
    template<typename charT, typename traits>
    inline bool operator>=(basic_string_ref<charT, traits> x, basic_string_ref<charT, traits> y) {
        return x.compare(y) >= 0;
        }

    template<typename charT, typename traits, typename Allocator>
    inline bool operator>=(basic_string_ref<charT, traits> x, const std::basic_string<charT, traits, Allocator> & y) {
        return x >= basic_string_ref<charT, traits>(y);
        }

    template<typename charT, typename traits, typename Allocator>
    inline bool operator>=(const std::basic_string<charT, traits, Allocator> & x, basic_string_ref<charT, traits> y) {
        return basic_string_ref<charT, traits>(x) >= y;
        }

    template<typename charT, typename traits>
    inline bool operator>=(basic_string_ref<charT, traits> x, const charT * y) {
        return x >= basic_string_ref<charT, traits>(y);
        }

    template<typename charT, typename traits>
    inline bool operator>=(const charT * x, basic_string_ref<charT, traits> y) {
        return basic_string_ref<charT, traits>(x) >= y;
        }

    namespace detail {

        template<class charT, class traits>
        inline void insert_fill_chars(std::basic_ostream<charT, traits>& os, std::size_t n) {
            enum { chunk_size = 8 };
            charT fill_chars[chunk_size];
            std::fill_n(fill_chars, static_cast< std::size_t >(chunk_size), os.fill());
            for (; n >= chunk_size && os.good(); n -= chunk_size)
                os.write(fill_chars, static_cast< std::size_t >(chunk_size));
            if (n > 0 && os.good())
                os.write(fill_chars, n);
            }

        template<class charT, class traits>
        void insert_aligned(std::basic_ostream<charT, traits>& os, const basic_string_ref<charT,traits>& str) {
            const std::size_t size = str.size();
            const std::size_t alignment_size = static_cast< std::size_t >(os.width()) - size;
            const bool align_left = (os.flags() & std::basic_ostream<charT, traits>::adjustfield) == std::basic_ostream<charT, traits>::left;
            if (!align_left) {
                detail::insert_fill_chars(os, alignment_size);
                if (os.good())
                    os.write(str.data(), size);
                }
            else {
                os.write(str.data(), size);
                if (os.good())
                    detail::insert_fill_chars(os, alignment_size);
                }
            }

        } // namespace detail

    // Inserter
    template<class charT, class traits>
    inline std::basic_ostream<charT, traits>&
    operator<<(std::basic_ostream<charT, traits>& os, const basic_string_ref<charT,traits>& str) {
        if (os.good()) {
            const std::size_t size = str.size();
            const std::size_t w = static_cast< std::size_t >(os.width());
            if (w <= size)
                os.write(str.data(), size);
            else
                detail::insert_aligned(os, str);
            os.width(0);
            }
        return os;
        }

#if 0
    // numeric conversions
    //
    //  These are short-term implementations.
    //  In a production environment, I would rather avoid the copying.
    //
    inline int stoi (string_ref str, size_t* idx=0, int base=10) {
        return std::stoi ( std::string(str), idx, base );
        }

    inline long stol (string_ref str, size_t* idx=0, int base=10) {
        return std::stol ( std::string(str), idx, base );
        }

    inline unsigned long stoul (string_ref str, size_t* idx=0, int base=10) {
        return std::stoul ( std::string(str), idx, base );
        }

    inline long long stoll (string_ref str, size_t* idx=0, int base=10) {
        return std::stoll ( std::string(str), idx, base );
        }

    inline unsigned long long stoull (string_ref str, size_t* idx=0, int base=10) {
        return std::stoull ( std::string(str), idx, base );
        }

    inline float stof (string_ref str, size_t* idx=0) {
        return std::stof ( std::string(str), idx );
        }

    inline double stod (string_ref str, size_t* idx=0) {
        return std::stod ( std::string(str), idx );
        }

    inline long double stold (string_ref str, size_t* idx=0)  {
        return std::stold ( std::string(str), idx );
        }

    inline int  stoi (wstring_ref str, size_t* idx=0, int base=10) {
        return std::stoi ( std::wstring(str), idx, base );
        }

    inline long stol (wstring_ref str, size_t* idx=0, int base=10) {
        return std::stol ( std::wstring(str), idx, base );
        }

    inline unsigned long stoul (wstring_ref str, size_t* idx=0, int base=10) {
        return std::stoul ( std::wstring(str), idx, base );
        }

    inline long long stoll (wstring_ref str, size_t* idx=0, int base=10) {
        return std::stoll ( std::wstring(str), idx, base );
        }

    inline unsigned long long stoull (wstring_ref str, size_t* idx=0, int base=10) {
        return std::stoull ( std::wstring(str), idx, base );
        }

    inline float  stof (wstring_ref str, size_t* idx=0) {
        return std::stof ( std::wstring(str), idx );
        }

    inline double stod (wstring_ref str, size_t* idx=0) {
        return std::stod ( std::wstring(str), idx );
        }

    inline long double stold (wstring_ref str, size_t* idx=0) {
        return std::stold ( std::wstring(str), idx );
        }
#endif

}

#if 0
namespace std {
    // Hashing
    template<> struct hash<boost::string_ref>;
    template<> struct hash<boost::u16string_ref>;
    template<> struct hash<boost::u32string_ref>;
    template<> struct hash<boost::wstring_ref>;
}
#endif

#endif
