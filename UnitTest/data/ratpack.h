#ifndef RATPACK_API_H
#define RATPACK_API_H

#include <calc_export.h>
#include <stdint.h>

/*!
 @mainpage CalcEngine

 CalcEngine is the Calculation Engine used in Microsoft Calculator.

 It consists of two parts of API:
 - @subpage ratpack "Ratpack API"
 - @subpage calcmgr "CalcManager API" (Internal Only)

 They will be packaged into two separate NuGet packages, and only the Ratpack one will be published to public feed.

 @page ratpack Ratpack API

 Ratpack is an Arbitrary-precision **Rat** (ional) Calculation **Pack** (age), used in Microsoft Calculator.

 See \ref ratpack.h for API Documentation.
*/

/*!
 @file RatpackApi.h
 @author Hongxu Xu (hox@microsoft.com)
 @date 2020-12-17
 @brief Ratpack C API

 Arbitrary-precision rational calculation package
*/

//! Error code: No error
#define CALC_E_NO_ERROR 0

//! Error code: STL exception. Call \ref Ratpack_get_last_error for details
#define CALC_E_STL_EXCEPTION 0x8000000AU

//! Error code: The current operation would require a divide by zero to complete
#define CALC_E_DIVIDEBYZERO 0x80000000U

//! Error code: The given input is not within the domain of this function
#define CALC_E_DOMAIN 0x80000001U

//! Error code: The result of this function is undefined
#define CALC_E_INDEFINITE 0x80000002U

//! Error code: The result of this function is Positive Infinity.
#define CALC_E_POSINFINITY 0x80000003U

//! Error code: The result of this function is Negative Infinity
#define CALC_E_NEGINFINITY 0x80000004U

//! Error code: The given input is within the domain of the function but is beyond the range for which calc can successfully compute the answer
#define CALC_E_INVALIDRANGE 0x80000006U

//! Error code: There is not enough free memory to complete the requested function
#define CALC_E_OUTOFMEMORY 0x80000007U

//! Error code: The result of this operation is an overflow
#define CALC_E_OVERFLOW 0x80000008U

//! Error code: The result of this operation is undefined
#define CALC_E_NORESULT 0x80000009U

#ifdef __cplusplus
extern "C"
{
#endif

    //! @brief Number formats
    enum ratpack_number_format_type
    {
        //! Floating point, or exponential if number is too big
        RATPACK_FLOAT,

        //! Scientific notation
        RATPACK_SCIENTIFIC,

        //! Engineering notation such that exponent is a multiple of 3
        RATPACK_ENGINEER,
    };

    //! @brief Angle types
    enum ratpack_angle_type
    {
        //! Calculate trig using 360 degrees per revolution
        RATPACK_DEGREES,

        //! Calculate trig using 2 pi radians per revolution
        RATPACK_RADIANS,

        //! Calculate trig using 400 gradians per revolution
        RATPACK_GRADIANS,
    };

#pragma warning(push)
#pragma warning(disable : 4200) // nonstandard extension used : zero-sized array in struct/union
    //! @brief Arbitrary-precision Number
    typedef struct ratpack_number
    {
        //! The sign of the mantissa, +1, or -1
        int32_t sign;
        //! The number of digits, or what passes for digits in the radix being used
        int32_t cdigit;
        //! The offset of digits from the radix point (decimal point in radix 10)
        int32_t exp;
        //! The mantissa (digits). This is actually allocated as a continuation of the NUMBER structure
        uint32_t mant[];
    } ratpack_number_t;
#pragma warning(pop)

    //! @brief Arbitrary-precision Rational
    //!
    //! Represented by pp/pq
    typedef struct ratpack_rational
    {
        //! The numerator
        ratpack_number_t* pp;
        //! The denominator
        ratpack_number_t* pq;
    } ratpack_rational_t;

    typedef struct ratpack_string ratpack_string_t;

    //! @brief Error code type (a uint32_t)
    //!
    //! Defines the error codes thrown by ratpak and caught by Calculator
    //! Ratpak errors are 32 bit values laid out as follows:
    //!
    //!       3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
    //!       1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
    //!      +-+-------+---------------------+-------------------------------+
    //!      |S|   R   |    Facility         |               Code            |
    //!      +-+-------+---------------------+-------------------------------+
    //!
    //!  where
    //!
    //!      S - Severity - indicates success/fail
    //!
    //!          0 - Success
    //!          1 - Fail
    //!
    //!      R - Reserved - not currently used for anything
    //!
    //!      r - reserved portion of the facility code. Reserved for internal
    //!              use. Used to indicate int32_t values that are not status
    //!              values, but are instead message ids for display strings.
    //!
    //!      Facility - is the facility code
    //!
    //!      Code - is the actual error code
    //!
    //! This format is based loosely on an OLE HRESULT and is compatible with the
    //! SUCCEEDED and FAILED macros as well as the HRESULT_CODE macro
    typedef uint32_t ratpack_error_code_t;

    //! @brief Get last error
    //! @return last error
    CALC_EXPORT const char* ratpack_get_last_error();

    //! @brief Set decimal separator used in ratpack_number_to_string
    //! @param decimalSeparator Decimal separator. '.' by default
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_set_decimal_separator(wchar_t decimalSeparator);

    //! @brief Change a mess of global constants based on new radix and new precision
    //! @param radix New radix
    //! @param precision New precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_change_constants(uint32_t radix, int32_t precision);

    //! @brief Check if two numbers are equivalent
    //!
    //! Assumes a and b have same radix.
    //! @param[in] pa Number a
    //! @param[in] pb Number b
    //! @param[out] ret true if equivalent, otherwise false
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_equals(const ratpack_number_t* pa, const ratpack_number_t* pb, bool* ret);

    //! @brief Check if unsigned number a is less than unsigned number b
    //!
    //! Assumes a and b have same radix and does unsigned comparison
    //! @param[in] pa Number a
    //! @param[in] pb Number b
    //! @param[out] ret true if abs(a) < abs(b), otherwise false
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_less(const ratpack_number_t* pa, const ratpack_number_t* pb, bool* ret);

    //! @brief Check if the number is zero
    //!
    //! Check if the number equals its negative number
    //! @param[in] pa Number a
    //! @param[out] ret true if a == -a, otherwise false
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_is_zero(const ratpack_number_t* pa, bool* ret);

    //! @brief Check if the rational is zero
    //! @param[in] pa Rational a
    //! @param[out] ret true if a == 0, otherwise false
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_is_zero(const ratpack_rational_t* pa, bool* ret);

    //! @brief Convert number to string
    //! @param[in,out] ppnum The number
    //! @param format Number format
    //! @param radix
    //! @param precision
    //! @param[out] ret A pointer to string
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t
    ratpack_number_to_string(ratpack_number_t** ppnum, ratpack_number_format_type format, uint32_t radix, int32_t precision, ratpack_string_t** ret);

    //! @brief Convert rational to string
    //!
    //! NOTE: It may be that doing a GCD() could shorten the rational form
    //! And it may eventually be worthwhile to keep the result. That is
    //! why a pointer to the rational is passed in.
    //! @param[in,out] pprat The rational
    //! @param format Number format
    //! @param radix
    //! @param precision
    //! @param[out] ret A pointer to string
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t
    ratpack_rat_to_string(ratpack_rational_t** pprat, ratpack_number_format_type format, uint32_t radix, int32_t precision, ratpack_string_t** ret);

    //! @brief Read wide char array from the pointer to string
    //! @param[in] s The pointer to string
    //! @return Wide char array represented by the input string
    CALC_EXPORT const wchar_t* ratpack_string_get_wcstr(const ratpack_string_t* s);

    //! @brief Destroy string
    //! @param str The pointer to string
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_string_destroy(ratpack_string_t* str);

    //! @brief Convert rational to number
    //! @param prat The rational
    //! @param radix
    //! @param precision
    //! @param[out] ret The number
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_to_num(const ratpack_rational_t* prat, uint32_t radix, int32_t precision, ratpack_number_t** ret);

    //! @brief  Flatten/simplify the rational
    //!
    //! Convert rational to number and back to a rational
    //! @param pprat The rational
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_flat(ratpack_rational_t** pprat, uint32_t radix, int32_t precision);

    //! @brief Convert number to int32.
    //! @param[in] pnum The number
    //! @param radix
    //! @param[out] ret The converted int32
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_to_i32(const ratpack_number_t* pnum, uint32_t radix, int32_t* ret);

    //! @brief Convert rational to int32
    //!
    //! Assumes that the number is in the internal radix
    //! @param[in] prat The rational
    //! @param radix
    //! @param precision
    //! @param[out] ret The converted int32
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_to_i32(const ratpack_rational_t* prat, uint32_t radix, int32_t precision, int32_t* ret);

    //! @brief Convert rational to uint64
    //!
    //! Assumes that the number is in the internal
    //! radix. Can throw exception if the number exceeds 2^64
    //! Implementation by getting the HI & LO 32 bit words and concatenating them, as the
    //! internal radix chosen happens to be 2^32, this is easier
    //! @param[in] prat The rational
    //! @param radix
    //! @param precision
    //! @param[out] ret The converted uint64
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_to_u64(const ratpack_rational_t* prat, uint32_t radix, int32_t precision, uint64_t* ret);

    //! @brief Allocate and zero-init a number
    //! @param size Digit size
    //! @param[out] ret Pointer to the created number
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_create(uint32_t size, ratpack_number_t** ret);

    //! @brief Do radix conversion on a number from internal to requested radix
    //!
    //! Assumes number being passed in is really in internal radix form
    //! @param[in] pa
    //! @param radix
    //! @param precision
    //! @param[out] ret Number representation in radix requested
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_nRadixx_to_num(const ratpack_number_t* pa, uint32_t radix, int32_t precision, ratpack_number_t** ret);

    //! @brief Use remainders to find the greatest common divisor
    //!
    //! Before it was found that the TRIM macro actually kept the
    //! size down cheaper than GCD, this routine was used extensively.
    //! now it is not used but might be later
    //! @param[in] pa Number a
    //! @param[in] pb Number b
    //! @param[out] ret GCD(a, b)
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_gcd(const ratpack_number_t* pa, const ratpack_number_t* pb, ratpack_number_t** ret);

    //! @brief Parse string to number
    //! @param[in] numberString the string
    //! @param strLen The length of numberString
    //! @param radix
    //! @param precision
    //! @param[out] ret The parsed number
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t
    ratpack_string_to_num(const wchar_t* numberString, uint32_t strLen, uint32_t radix, int32_t precision, ratpack_number_t** ret);

    //! @brief Parse string to rational
    //! @param mantissaIsNegative Whether mantissa is less than zero
    //! @param[in] mantissa The string representation of the digits of number
    //! @param mantissaLen The length of mantissa string
    //! @param exponentIsNegative Whether exponent is less than zero
    //! @param[in] exponent The string representation of the exponent of number
    //! @param exponentLen The length of exponent string
    //! @param radix
    //! @param precision
    //! @param[out] ret The parsed rational
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_string_to_rat(
        bool mantissaIsNegative,
        const wchar_t* mantissa,
        uint32_t mantissaLen,
        bool exponentIsNegative,
        const wchar_t* exponent,
        uint32_t exponentLen,
        uint32_t radix,
        int32_t precision,
        ratpack_rational_t** ret);

    //! @brief Factorialize integer
    //! @param i32 Integer to factorialize
    //! @param radix
    //! @param[out] ret Factorial of input in number
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_factorial(int32_t i32, uint32_t radix, ratpack_number_t** ret);

    //! @brief Multiply integers in a range
    //! @param start Start integer of the range (inclusive)
    //! @param stop Stop integer of the range (inclusive)
    //! @param radix
    //! @param[out] ret Product of the mulitplication
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_product(int32_t start, int32_t stop, uint32_t radix, ratpack_number_t** ret);

    //! @brief Convert int32 to number
    //! @param i32 The 32-bit integer
    //! @param radix
    //! @param[out] ret The converted number
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_i32_to_num(int32_t i32, uint32_t radix, ratpack_number_t** ret);

    //! @brief Convert uint32 to number
    //! @param u32 The 32-bit unsigned integer
    //! @param radix
    //! @param[out] ret The converted number
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_u32_to_num(uint32_t u32, uint32_t radix, ratpack_number_t** ret);

    //! @brief Convert the radix of number to requested radix
    //! @param[in] pa The number
    //! @param radix Requested radix
    //! @param[out] ret The converted number
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_to_nRadixx(const ratpack_number_t* pa, uint32_t radix, ratpack_number_t** ret);

    //! @brief Allocate a rational with p/q
    //!
    //! The allocated rational will own the 2 numbers.
    //! i.e. the lifetime of numerator and denominator numbers will be managed by the rational.
    //! @param[in] p Numerator
    //! @param[in] q Denominator
    //! @param[out] ret The allocated rational
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_create(ratpack_number_t* p, ratpack_number_t* q, ratpack_rational_t** ret);

    //! @brief Calculate acos (inverse cosine) in specified \ref ratpack_angle_type
    //!
    //! Use Taylor series:
    //! \f{eqnarray*}{
    //!     \sum_{j=0}^{n} thisterm_j; \\
    //!     \text{where } thisterm_{j+1} &=& thisterm_{j} \times \frac{(2j+1)^2 x^2}{(2j+2)(2j+3)}, \\
    //!     thisterm_0 &=& 1. \\
    //!     \text{stop when } thisterm_n &<& \text{precision used}
    //! \f}
    //!
    //! In this case, \f$\frac{\pi}{2} - asin(x)\f$ is used.
    //!
    //! @param[in,out] ppx The rational x
    //! @param angletype Angle type
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_acos_angle(ratpack_rational_t** ppx, ratpack_angle_type angletype, uint32_t radix, int32_t precision);

    //! @brief Calculate acosh (inverse hyperbolic cosine)
    //! @param[in,out] ppx The rational x
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_acosh(ratpack_rational_t** ppx, uint32_t radix, int32_t precision);

    //! @brief Calculate acos (inverse cosine)
    //!
    //! Use Taylor series:
    //! \f{eqnarray*}{
    //!     \sum_{j=0}^{n} thisterm_j; \\
    //!     \text{where } thisterm_{j+1} &=& thisterm_{j} \times \frac{(2j+1)^2 x^2}{(2j+2)(2j+3)}, \\
    //!     thisterm_0 &=& 1. \\
    //!     \text{stop when } thisterm_n &<& \text{precision used}
    //! \f}
    //!
    //! In this case, \f$\frac{\pi}{2} - asin(x)\f$ is used.
    //!
    //! @param[in,out] ppx The rational x
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_acos(ratpack_rational_t** ppx, uint32_t radix, int32_t precision);

    //! @brief Calculate asin (inverse sine) in specified \ref ratpack_angle_type
    //!
    //! Use Taylor series:
    //! \f{eqnarray*}{
    //!     \sum_{j=0}^{n} thisterm_j; \\
    //!     \text{where } thisterm_{j+1} &=& thisterm_{j} \times \frac{(2j+1)^2 x^2}{(2j+2)(2j+3)}, \\
    //!     thisterm_0 &=& 1. \\
    //!     \text{stop when } thisterm_n &<& \text{precision used}
    //! \f}
    //!
    //! If abs(x) > 0.85, then an alternate form is used:
    //! \f[
    //!     \pi/2 - sgn(x) \times asin(sqrt(1-x^2))
    //! \f]
    //!
    //! @param[in,out] ppx The rational x
    //! @param angletype Angle type
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_asin_angle(ratpack_rational_t** ppx, ratpack_angle_type angletype, uint32_t radix, int32_t precision);

    //! @brief Calculate asinh (inverse hyperbolic sine)
    //! @param[in,out] ppx The rational x
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_asinh(ratpack_rational_t** ppx, uint32_t radix, int32_t precision);

    //! @brief Calculate asin (inverse sine)
    //!
    //! Use Taylor series:
    //! \f{eqnarray*}{
    //!     \sum_{j=0}^{n} thisterm_j; \\
    //!     \text{where } thisterm_{j+1} &=& thisterm_{j} \times \frac{(2j+1)^2 x^2}{(2j+2)(2j+3)}, \\
    //!     thisterm_0 &=& 1. \\
    //!     \text{stop when } thisterm_n &<& \text{precision used}
    //! \f}
    //!
    //! If abs(x) > 0.85, then an alternate form is used:
    //! \f[
    //!     \pi/2 - sgn(x) \times asin(sqrt(1-x^2))
    //! \f]
    //!
    //! @param[in,out] ppx The rational x
    //! @param angletype Angle type
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_asin(ratpack_rational_t** ppx, uint32_t radix, int32_t precision);

    //! @brief Calculate atan (inverse tangent) in specified \ref ratpack_angle_type
    //!
    //! Use Taylor series:
    //! \f{eqnarray*}{
    //!     \sum_{j=0}^{n} thisterm_j; \\
    //!     \text{where } thisterm_{j+1} &=& thisterm_{j} \times \frac{2j \cdot x^2 (-1^j)}{2j+2}, \\
    //!     thisterm_0 &=& x. \\
    //!     \text{stop when } thisterm_n &<& \text{precision used}
    //! \f}
    //!
    //! If abs(x) > 0.85, then an alternate form is used:
    //! \f[
    //!     asin(\frac{x}{sqrt(q+x^2)})
    //! \f]
    //!
    //! If abs(x) > 2.0, then this form is used:
    //! \f[
    //!     \frac{\pi}{2} - atan(\frac{1}{x})
    //! \f]
    //!
    //! @param[in,out] ppx The rational x
    //! @param angletype Angle type
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_atan_angle(ratpack_rational_t** ppx, ratpack_angle_type angletype, uint32_t radix, int32_t precision);

    //! @brief Calculate atanh (inverse hyperbolic tangent)
    //!
    //! Use \f$atanh(x) = \frac{1}{2} ln(\frac{x+1}{x-1}) \f$
    //! @param[in,out] ppx The rational x
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_atanh(ratpack_rational_t** ppx, int32_t precision);

    //! @brief Calculate atan (inverse tangent)
    //!
    //! Use Taylor series:
    //! \f{eqnarray*}{
    //!     \sum_{j=0}^{n} thisterm_j; \\
    //!     \text{where } thisterm_{j+1} &=& thisterm_{j} \times \frac{2j \cdot x^2 (-1^j)}{2j+2}, \\
    //!     thisterm_0 &=& x. \\
    //!     \text{stop when } thisterm_n &<& \text{precision used}
    //! \f}
    //!
    //! If abs(x) > 0.85, then an alternate form is used:
    //! \f[
    //!     asin(\frac{x}{sqrt(q+x^2)})
    //! \f]
    //!
    //! If abs(x) > 2.0, then this form is used:
    //! \f[
    //!     \frac{\pi}{2} - atan(\frac{1}{x})
    //! \f]
    //!
    //! @param[in,out] ppx The rational x
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_atan(ratpack_rational_t** ppx, uint32_t radix, int32_t precision);

    //! @brief Calculate cosh (cosine hyperbolic)
    //!
    //! Use Taylor series:
    //! \f$\sum_{j=0}^{n} \frac{x^{2j}}{(2j)!}\f$
    //! or,
    //! \f{eqnarray*}{
    //!     \sum_{j=0}^{n} thisterm_j; \\
    //!     \text{where } thisterm_{j+1} &=& thisterm_{j} \times \frac{x^2}{2j \cdot (2j+1)}, \\
    //!     thisterm_0 &=& 1. \\
    //!     \text{stop when } thisterm_n &<& \text{precision used}
    //! \f}
    //!
    //! If x > 1.0, \f$\frac{e^x + e^{-x}}{2}\f$ is used.
    //!
    //! @param[in,out] ppx The rational x
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_cosh(ratpack_rational_t** ppx, uint32_t radix, int32_t precision);

    //! @brief Calculate cos (cosine)
    //!
    //! Use Taylor series:
    //! \f$\sum_{j=0}^{n} \frac{(-1)^j x^{2j}}{(2j)!}\f$
    //! or,
    //! \f{eqnarray*}{
    //!     \sum_{j=0}^{n} thisterm_j; \\
    //!     \text{where } thisterm_{j+1} &=& thisterm_{j} \times \frac{-x^2}{2j \cdot (2j+1)}, \\
    //!     thisterm_0 &=& 1. \\
    //!     \text{stop when } thisterm_n &<& \text{precision used}
    //! \f}
    //!
    //! @param[in,out] ppx The rational x
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_cos(ratpack_rational_t** ppx, uint32_t radix, int32_t precision);

    //! @brief Calculate cos (cosine) in specified \ref ratpack_angle_type
    //!
    //! Use Taylor series:
    //! \f$\sum_{j=0}^{n} \frac{(-1)^j x^{2j}}{(2j)!}\f$
    //! or,
    //! \f{eqnarray*}{
    //!     \sum_{j=0}^{n} thisterm_j; \\
    //!     \text{where } thisterm_{j+1} &=& thisterm_{j} \times \frac{-x^2}{2j \cdot (2j+1)}, \\
    //!     thisterm_0 &=& 1. \\
    //!     \text{stop when } thisterm_n &<& \text{precision used}
    //! \f}
    //!
    //! @param[in,out] ppx The rational x
    //! @param angletype Angle type
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_cos_angle(ratpack_rational_t** ppx, ratpack_angle_type angletype, uint32_t radix, int32_t precision);

    //! @brief Calculate exp (exponentiation)
    //!
    //! Use Taylor series:
    //! \f{eqnarray*}{
    //!     \sum_{j=0}^{n} thisterm_j; \\
    //!     \text{where } thisterm_{j+1} &=& thisterm_{j} \times \frac{x}{j+1}, \\
    //!     thisterm_0 &=& x. \\
    //!     \text{stop when } thisterm_n &<& \text{precision used}
    //! \f}
    //!
    //! @param[in,out] ppx The rational x
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_exp(ratpack_rational_t** ppx, uint32_t radix, int32_t precision);

    //! @brief Calculate log10 (log with base 10)
    //! @param[in,out] ppx The rational x
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_log10(ratpack_rational_t** ppx, int32_t precision);

    //! @brief Calculate log/ln (log with base e)
    //! @param[in,out] ppx The rational x
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_log(ratpack_rational_t** ppx, int32_t precision);

    //! @brief Convert int32 to rational
    //!
    //! p is the int32, and q is 1.
    //! @param i32 The 32-bit integer
    //! @param[out] ret The converted rational
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_i32_to_rat(int32_t i32, ratpack_rational_t** ret);

    //! @brief Convert uint32 to rational
    //!
    //! p is the uint32, and q is 1.
    //! @param u32 The 32-bit unsigned integer
    //! @param[out] ret The converted rational
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_u32_to_rat(uint32_t u32, ratpack_rational_t** ret);

    //! @brief Convert number to rational
    //! @param[in] pnum The number
    //! @param radix
    //! @param[out] ret The converted rational
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_to_rat(const ratpack_number_t* pnum, uint32_t radix, ratpack_rational_t** ret);

    //! @brief Calculate sinh (sine hyperbolic)
    //!
    //! Use Taylor series:
    //! \f$\sum_{j=0}^{n} \frac{x^{2j+1}}{(2j+1)!}\f$
    //! or,
    //! \f{eqnarray*}{
    //!     \sum_{j=0}^{n} thisterm_j; \\
    //!     \text{where } thisterm_{j+1} &=& thisterm_{j} \times \frac{x^2}{2j \cdot (2j+1)}, \\
    //!     thisterm_0 &=& x. \\
    //!     \text{stop when } thisterm_n &<& \text{precision used}
    //! \f}
    //!
    //! @param[in,out] ppx The rational x
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_sinh(ratpack_rational_t** ppx, uint32_t radix, int32_t precision);

    //! @brief Calculate sin (sine)
    //!
    //! Use Taylor series:
    //! \f$\sum_{j=0}^{n} \frac{x^{2j+1}}{(2j+1)!}\f$
    //! or,
    //! \f{eqnarray*}{
    //!     \sum_{j=0}^{n} thisterm_j; \\
    //!     \text{where } thisterm_{j+1} &=& thisterm_{j} \times \frac{x^2}{2j \cdot (2j+1)}, \\
    //!     thisterm_0 &=& x. \\
    //!     \text{stop when } thisterm_n &<& \text{precision used}
    //! \f}
    //!
    //! @param[in,out] ppx The rational x
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_sin(ratpack_rational_t** ppx, uint32_t radix, int32_t precision);

    //! @brief Calculate sin (sine) in specified \ref ratpack_angle_type
    //!
    //! Use Taylor series:
    //! \f$\sum_{j=0}^{n} \frac{x^{2j+1}}{(2j+1)!}\f$
    //! or,
    //! \f{eqnarray*}{
    //!     \sum_{j=0}^{n} thisterm_j; \\
    //!     \text{where } thisterm_{j+1} &=& thisterm_{j} \times \frac{x^2}{2j \cdot (2j+1)}, \\
    //!     thisterm_0 &=& x. \\
    //!     \text{stop when } thisterm_n &<& \text{precision used}
    //! \f}
    //!
    //! @param[in,out] ppx The rational x
    //! @param angletype Angle type
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_sin_angle(ratpack_rational_t** ppx, ratpack_angle_type angletype, uint32_t radix, int32_t precision);

    //! @brief Calculate tanh (tangent hyperbolic)
    //!
    //! Use sinh and cosh.
    //! @param[in,out] ppx The rational x
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_tanh(ratpack_rational_t** ppx, uint32_t radix, int32_t precision);

    //! @brief Calculate tan (tangent)
    //!
    //! Use sin and cos.
    //! @param[in,out] ppx The rational x
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_tan(ratpack_rational_t** ppx, uint32_t radix, int32_t precision);

    //! @brief Calculate tan (tangent) in specified \ref ratpack_angle_type
    //!
    //! Use sin and cos.
    //! @param[in,out] ppx The rational x
    //! @param angletype Angle type
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_tan_angle(ratpack_rational_t** ppx, ratpack_angle_type angletype, uint32_t radix, int32_t precision);

    //! @brief Copy src number to dest number
    //! @param[in,out] dest Number to copy to
    //! @param[in] src Number to copy from
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_copy(ratpack_number_t* dest, const ratpack_number_t* const src);

    //! @brief Duplicate the number
    //! @param[in] src Number to duplicate
    //! @param[out] ret A new number that equals to src number
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_dup(const ratpack_number_t* const src, ratpack_number_t** ret);

    //! @brief Duplicate the rational
    //! @param[in] src Rational to duplicate
    //! @param[out] ret A new rational that equals to src rational
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_dup(const ratpack_rational_t* const src, ratpack_rational_t** ret);

    //! @brief ??? Log number by radix
    //!
    //! \f$ (cdigit + exp) \cdot ratio \f$
    //!
    //! ratio is the ratio of internal 'digits' to output 'digits', calculated by \f$ \frac{log(2^{BASEXPWR})}{log(radix)} \f$.
    //! @param[in] pnum The number
    //! @param[out] ret log radix
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_log_radix(const ratpack_number_t* pnum, int32_t* ret);

    //! @brief ??? Log rational by radix
    //!
    //! \f$ num_log_radix(p) - num_log_radix(q) \f$
    //!
    //! @param[in] prat The rational
    //! @param[out] ret log radix
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_log_radix(const ratpack_rational_t* prat, int32_t* ret);

    //! @brief ???
    //!
    //! \f$ cdigit + exp \f$
    //! @param[in] pnum The number
    //! @param[out] ret
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_lognum2(const ratpack_number_t* pnum, int32_t* ret);

    //! @brief ???
    //!
    //! \f$ lognum2(p) - lognum2(q) \f$
    //! @param[in] prat
    //! @param[out] ret
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_lograt2(const ratpack_rational_t* prat, int32_t* ret);

    //! @brief Sign of rational
    //!
    //! \f$ p.sign \times q.sign \f$
    //! @param[in] prat The rational
    //! @param[out] ret The sign. +1 means positive and -1 means negative.
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_sign(const ratpack_rational_t* prat, int32_t* ret);

    //! @brief Destroy a number
    //! @param pnum The number
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_destroy(ratpack_number_t* pnum);

    //! @brief Destroy a rational
    //! @param prat The rational
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_destroy(ratpack_rational_t* prat);

    //! @brief Add two numbers and save result to the 1st number
    //!
    //! It's just a += b.
    //! Assumes radix is the base of both numbers.
    //! @param[in,out] ppa The number a
    //! @param[in] pb The number b
    //! @param radix
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_add(ratpack_number_t** ppa, const ratpack_number_t* pb, uint32_t radix);

    //! @brief Add two rationals and save result to the 1st rational
    //!
    //! It's just a += b.
    //! The internal representation of b may be changed to optimize the calculation, but its real value won't be changed.
    //! Assumes base is internal throughout.
    //! @param[in,out] ppa The rational a
    //! @param[in,out] pb The rational b
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_add(ratpack_rational_t** ppa, ratpack_rational_t* pb, int32_t precision);

    //! @brief Bitwise AND two rationals and save result to the 1st rational
    //!
    //! It's just a &= b.
    //! @param[in,out] ppa The rational a
    //! @param[in] pb The rational b
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_and(ratpack_rational_t** ppa, const ratpack_rational_t* pb, uint32_t radix, int32_t precision);

    //! @brief Divide numbers and save result to the 1st number
    //!
    //! It's just a /= b.
    //! Assumes radix is the radix of both numbers.
    //! @param[in,out] ppa The number a
    //! @param[in] pb The number b
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_div(ratpack_number_t** ppa, const ratpack_number_t* pb, uint32_t radix, int32_t precision);

    //! @brief Divide numbers and save result to the 1st number
    //!
    //! It's just a /= b.
    //! Assumes radix is the internal radix representation.
    //! @param[in,out] ppa The number a
    //! @param[in] pb The number b
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_divx(ratpack_number_t** ppa, const ratpack_number_t* pb, int32_t precision);

    //! @brief Divide rationals and save result to the 1st rational
    //!
    //! It's just a /= b.
    //! Assumes radix is the radix of both rationals.
    //! @param[in,out] ppa The rational a
    //! @param[in] pb The rational b
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_div(ratpack_rational_t** ppa, const ratpack_rational_t* pb, int32_t precision);

    //! @brief ???
    //!
    //! Does the rational equivalent of frac(*ppa)
    //! @param[in,out] ppa The rational a
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_frac(ratpack_rational_t** ppa, uint32_t radix, int32_t precision);

    //! @brief Caculate factorial of the rational and save result inplace
    //!
    //! Use Taylor series:
    //! \f{eqnarray*}{
    //!     A^n \sum_{j=0}^{n} \frac{A^{2j}}{(2j)!} [\frac{1}{n+2j} - \frac{A}{(n+2j+1)(2j+1)}]
    //! \f}
    //! 
    //! This was derived from:
    //! \f{eqnarray*}{
    //!     \int_0^{\infty} x^{n-1} e^{-x} dx = \Gamma_n
    //! \f}
    //! ( = (n-1)! for +integers )
    //! 
    //! It can be shown that the above series is within precision if A is chosen big enough.
    //!
    //! Based on the relation \f$ n e^A = A^n 10^{precision} \f$ A was chosen as 
    //!
    //! \f$ A = ln(\frac{Base^{precision}}{n})+1 \f$
    //!
    //! \f$ A += n ln(A) \f$ This is close enough for precision > base and n < 1.5
    //! 
    //! @param[in,out] ppa The rational a
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_fact(ratpack_rational_t** ppa, uint32_t radix, int32_t precision);

    //! @brief Divide rationals and save the remainder to the 1st rational
    //!
    //! It's just a %= b.
    //! It produces a result that is either zero or has the same sign as the dividend.
    //! @param[in,out] ppa The rational a
    //! @param[in] pb The rational b
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_rem(ratpack_rational_t** ppa, const ratpack_rational_t* pb);

    //! @brief Divide rationals and save the modulo to the 1st rational
    //!
    //! It's like a %= b, but not exatly the same.
    //! It produces a result that is either zero or has the same sign as the divisor.
    //! When a or b is negative, the result won't be the same as a%b.
    //! Use \ref ratpack_rat_rem for the same behavior of a%b.
    //! @param[in,out] ppa The rational a
    //! @param[in] pb The rational b
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_mod(ratpack_rational_t** ppa, const ratpack_rational_t* pb);

    //! @brief Divide p and q of rational by their GCD (greatest common divisor)
    //!
    //! It was hoped this would speed up some calculations, and until the above trimming was done it did, but after trimming gcdratting, only slows things down.
    //! @param ppa The rational
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_gcd(ratpack_rational_t** ppa, int32_t precision);

    //! @brief Truncate the rational to integral rational
    //! @param[in,out] ppx The rational
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_int(ratpack_rational_t** ppx, uint32_t radix, int32_t precision);

    //! @brief Multiply numbers and save result to the 1st number
    //!
    //! It's just a *= b.
    //! Assumes radix is the radix of both numbers.
    //! The algorithm is the same one you learned in grade school.
    //! @param[in,out] ppa The number a
    //! @param[in] pb The number b
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_mul(ratpack_number_t** ppa, const ratpack_number_t* pb, uint32_t radix);

    //! @brief Multiply numbers and save result to the 1st number
    //!
    //! It's just a /= b.
    //! Assumes radix is the internal radix representation.
    //! @param[in,out] ppa The number a
    //! @param[in] pb The number b
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_mulx(ratpack_number_t** ppa, const ratpack_number_t* pb);

    //! @brief Multiply rationals and save result to the 1st rational
    //!
    //! It's just a *= b.
    //! Assumes radix is the radix of both rationals.
    //! @param[in,out] ppa The rational a
    //! @param[in] pb The rational b
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_mul(ratpack_rational_t** ppa, const ratpack_rational_t* pb, int32_t precision);

    //! @brief Power the number for times specified by int32
    //!
    //! The result will be saved in the number inplace.
    //! Assumes radix is the radix of root.
    //! @param[in,out] proot The number
    //! @param power The power int32
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_pow_i32(ratpack_number_t** proot, int32_t power, uint32_t radix, int32_t precision);

    //! @brief Power the number for times specified by int32
    //!
    //! The result will be saved in the number inplace.
    //! Assumes base BASEX decomposes the exponent into its sums of powers of 2, so on average
    //! it will take \f$ n + \frac{n}{2} \f$ multiplications where n is the highest on bit.
    //! @param[in,out] proot The number
    //! @param power The power int32
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_pow_i32x(ratpack_number_t** proot, int32_t power);

    //! @brief Bitwise OR two rationals and save result to the 1st rational
    //!
    //! It's just a |= b.
    //! @param[in,out] ppa The rational a
    //! @param[in] pb The rational b
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_or(ratpack_rational_t** ppa, const ratpack_rational_t* pb, uint32_t radix, int32_t precision);

    //! @brief Power the rational for rational times
    //!
    //! The result will be saved in the 1st rational inplace.
    //! \f$ a = a^b \f$.
    //!
    //! It will handle special cases where a is a perfect root.
    //! Assumes all checking has been done on validity of numbers.
    //! @param[in,out] ppa The root rational a
    //! @param[in] pb The power rational b
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_pow(ratpack_rational_t** ppa, const ratpack_rational_t* pb, uint32_t radix, int32_t precision);

    //! @brief Power the rational for rational times (using numerator/denominator)
    //!
    //! The result will be saved in the 1st rational inplace.
    //! \f$ a = a^b \f$.
    //!
    //! Use \f$ a^{\frac{b_p}{b_q}} = a^{b_p^{\frac{1}{b_q}}} \f$.
    //!
    //! 1. For \f$ a^{b_p} \f$, we call \ref ratpack_rat_pow_comp directly which will call \ref ratpack_rat_pow_i32 and store the result in \f$
    //! aPow_p \f$.
    //! 2. For \f$ aPow_p^{\frac{1}{b_q}} \f$, we call \ref ratpack_rat_pow_comp.
    //! 3. Validate the result of 2 by adding/subtracting 0.5, flooring and call \ref ratpack_rat_pow_comp with \f$ b_q \f$ on the floored result.
    //!
    //! > the subscript \f$ p \f$ in \f$ x_p \f$ means numerator of \f$ x \f$, and \f$ q \f$ in \f$ x_q \f$ means denominator of \f$ x \f$.
    //!
    //! @param[in,out] ppa The root rational a
    //! @param[in] pb The power rational b
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t
    ratpack_rat_pow_numerator_denominator(ratpack_rational_t** ppa, const ratpack_rational_t* pb, uint32_t radix, int32_t precision);

    //! @brief Power the rational for rational times (less acurate method of passing in the original power)
    //!
    //! The result will be saved in the 1st rational inplace.
    //! \f$ a = a^b \f$.
    //!
    //! Use \f$ a^b = e^{b \cdot ln(a)} \f$,
    //! or a more exact calculation where b is an integer.
    //!
    //! Assumes all checking has been done on validity of numbers.
    //! @param[in,out] ppa The root rational a
    //! @param[in] pb The power rational b
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_pow_comp(ratpack_rational_t** ppa, const ratpack_rational_t* pb, uint32_t radix, int32_t precision);

    //! @brief Power the rational for times specified by int32
    //!
    //! The result will be saved in the rational inplace.
    //! @param[in,out] proot The rational
    //! @param power The power int32
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_pow_i32(ratpack_rational_t** proot, int32_t power, int32_t precision);

    //! @brief Divide the numbers and save the remainder result to the 1st number
    //!
    //! It's just a %= b.
    //! Repeatedly subtracts off powers of 2 of b until a < b.
    //! @param[in,out] ppa The rational a
    //! @param[in] pb The rational b
    //! @param radix
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_num_rem(ratpack_number_t** ppa, const ratpack_number_t* pb, uint32_t radix);

    //! @brief Calculate bth root of the rational a
    //!
    //! The result will be saved in the rational a inplace.
    //! This is now a stub function to \ref ratpack_rat_pow.
    //! @param[in,out] ppa The rational a
    //! @param[in] pb The rational b
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_root(ratpack_rational_t** ppa, const ratpack_rational_t* pb, uint32_t radix, int32_t precision);

    //! @brief Smash the rational with a scaled number in the range of 0..\f$ 2 \pi \f$
    //! @param[in,out] ppx The rational
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_scale2pi(ratpack_rational_t** ppx, uint32_t radix, int32_t precision);

    //! @brief Smash the rational x with a scaled number in the range of the rational scalefact
    //! @param[in,out] ppx The rational x
    //! @param[in] scalefact The scale factor rational
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_scale(ratpack_rational_t** ppx, const ratpack_rational_t* scalefact, uint32_t radix, int32_t precision);

    //! @brief Subtract rationals and save result to the 1st rational
    //!
    //! It's just a -= b.
    //! The internal representation of b may be changed to optimize the calculation, but its real value won't be changed.
    //! Assumes base is internal throughout.
    //! @param[in,out] ppa The rational a
    //! @param[in,out] pb The rational b
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_sub(ratpack_rational_t** ppa, ratpack_rational_t* pb, int32_t precision);

    //! @brief Bitwise XOR two rationals and save result to the 1st rational
    //!
    //! It's just a ^= b.
    //! @param[in,out] ppa The rational a
    //! @param[in] pb The rational b
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_xor(ratpack_rational_t** ppa, ratpack_rational_t* pb, uint32_t radix, int32_t precision);

    //! @brief Left shift the rational
    //!
    //! It's just a <<= b.
    //! @param[in,out] ppa The rational a
    //! @param[in] pb The rational b
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_lsh(ratpack_rational_t** ppa, const ratpack_rational_t* pb, uint32_t radix, int32_t precision);

    //! @brief Right shift the rational
    //!
    //! It's just a >>= b.
    //! @param[in,out] ppa The rational a
    //! @param[in] pb The rational b
    //! @param radix
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_rsh(ratpack_rational_t** ppa, const ratpack_rational_t* pb, uint32_t radix, int32_t precision);

    //! @brief Equal
    //! @param[in] pa The rational a
    //! @param[in] pb The rational b
    //! @param precision
    //! @param[out] ret true if equal, false otherwise
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_equ(ratpack_rational_t* pa, ratpack_rational_t* pb, int32_t precision, bool* ret);

    //! @brief Not equal
    //! @param[in] pa The rational a
    //! @param[in] pb The rational b
    //! @param precision
    //! @param[out] ret true if not equal, false otherwise
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_neq(ratpack_rational_t* pa, ratpack_rational_t* pb, int32_t precision, bool* ret);

    //! @brief Greater than
    //! @param[in] pa The rational a
    //! @param[in] pb The rational b
    //! @param precision
    //! @param[out] ret true if a > b, false otherwise
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_gt(ratpack_rational_t* pa, ratpack_rational_t* pb, int32_t precision, bool* ret);

    //! @brief Greater than or equal
    //! @param[in] pa The rational a
    //! @param[in] pb The rational b
    //! @param precision
    //! @param[out] ret true if a >= b, false otherwise
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_ge(ratpack_rational_t* pa, ratpack_rational_t* pb, int32_t precision, bool* ret);

    //! @brief Less than
    //! @param[in] pa The rational a
    //! @param[in] pb The rational b
    //! @param precision
    //! @param[out] ret true if a < b, false otherwise
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_lt(ratpack_rational_t* pa, ratpack_rational_t* pb, int32_t precision, bool* ret);

    //! @brief Less than or equal
    //! @param[in] pa The rational a
    //! @param[in] pb The rational b
    //! @param precision
    //! @param[out] ret true if a <= b, false otherwise
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_le(ratpack_rational_t* pa, ratpack_rational_t* pb, int32_t precision, bool* ret);

    //! @brief Truncate the rational within a range
    //!
    //! Change the rational x to the boundary value -/+range if x is out of the range (outside -range..+range).
    //! @param[in,out] ppx The rational x
    //! @param[in] range The range boundary rational
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_inbetween(ratpack_rational_t** ppx, ratpack_rational_t* range, int32_t precision);

    //! @brief Chop off digits in the rational to optimize the calculations
    //!
    //! Chops off digits from rational numbers to avoid time
    //! explosions in calculations of functions using series.
    //! It can be shown that it is enough to only keep the first n digits
    //! of the largest of p or q in the rational p over q form, and of course
    //! scale the smaller by the same number of digits.  This will give you
    //! n-1 digits of accuracy.  This dramatically speeds up calculations
    //! involving hundreds of digits or more.
    //! The last part of this trim dealing with exponents never affects accuracy
    //! @param[in,out] ppx The rational
    //! @param precision
    //! @return Error code
    CALC_EXPORT ratpack_error_code_t ratpack_rat_trim(ratpack_rational_t** ppx, int32_t precision);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // RATPACK_API_H
