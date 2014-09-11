
#define _SAL_VERSION_CHECK(_A)

/************************************************************************
*  Introduction
*
*  specstrings.h provides a set of annotations to describe how a function uses
*  its parameters - the assumptions it makes about them, and the guarantees it
*  makes upon finishing.
* 
*  Annotations must be placed before a function parameter's type or its return
*  type. There are two basic classes of common annotations buffer annotations
*  and advanced annotations.  Buffer annotations describe how functions use
*  their pointer parameters, and advanced annotations either describe
*  complex/unusual buffer behavior, or provide additional information about a
*  parameter that is not otherwise expressible.
* 
*  Buffer Annotations
* 
*  The most important annotations in SpecStrings.h provide a consistent way to
*  annotate buffer parameters or return values for a function. Each of these
*  annotations describes a single buffer (which could be a string, a
*  fixed-length or variable-length array, or just a pointer) that the function
*  interacts with: where it is, how large it is, how much is initialized, and
*  what the function does with it.
* 
*  The appropriate macro for a given buffer can be constructed using the table
*  below.  Just pick the appropriate values from each category, and combine
*  them together with a leading underscore. Some combinations of values do not
*  make sense as buffer annotations. Only meaningful annotations can be added
*  to your code; for a list of these, see the buffer annotation definitions
*  section.
* 
*  Only a single buffer annotation should be used for each parameter.
* 
*  |------------|------------|---------|--------|----------|---------------|
*  |   Level    |   Usage    |  Size   | Output | Optional |  Parameters   |
*  |------------|------------|---------|--------|----------|---------------|
*  | <>         | <>         | <>      | <>     | <>       | <>            |
*  | _deref     | _in        | _ecount | _full  | _opt     | (size)        |
*  | _deref_opt | _out       | _bcount | _part  |          | (size,length) |
*  |            | _inout     |         |        |          |               |
*  |            |            |         |        |          |               |
*  |------------|------------|---------|--------|----------|---------------|
*
*  Note: "<>" represents the empty string.
* 
*  Level: Describes the buffer pointer's level of indirection from the
*  parameter or return value 'p'.
* 
*  <>         : p is the buffer pointer.
*  _deref     : *p is the buffer pointer. p must not be NULL.
*  _deref_opt : *p may be the buffer pointer. p may be NULL, in which case the 
*               rest of the annotation is ignored.
* 
*  Usage: Describes how the function uses the buffer.
* 
*  <> : The buffer is not accessed. If used on the return value or with
*  _deref, the function will provide the buffer, and it will be uninitialized
*  at exit.  Otherwise, the caller must provide the buffer. This should only
*  be used for alloc and free functions.
*
*  _in : The function will only read from the buffer. The caller must provide
*  the buffer and initialize it.
*
*  _out : The function will only write to the buffer. If used on the return
*  value or with _deref, the function will provide the buffer and initialize
*  it.  Otherwise, the caller must provide the buffer, and the function will
*  initialize it.
*
*  _inout : The function may freely read from and write to the buffer. The
*  caller must provide the buffer and initialize it. If used with _deref, the
*  buffer may be reallocated by the function.
*
*  Size: Describes the total size of the buffer. This may be less than the
*  space actually allocated for the buffer, in which case it describes the
*  accessible amount.
* 
*  <> : No buffer size is given. If the type specifies the buffer size (such
*  as with LPSTR and LPWSTR), that amount is used. Otherwise, the buffer is
*  one element long. Must be used with _in, _out, or _inout.
*
*  _ecount : The buffer size is an explicit element count.
*
*  _bcount : The buffer size is an explicit byte count.
* 
*  Output: Describes how much of the buffer will be initialized by the
*  function. For _inout buffers, this also describes how much is initialized
*  at entry. Omit this category for _in buffers; they must be fully
*  initialized by the caller.
* 
*  <> : The type specifies how much is initialized. For instance, a function
*  initializing an LPWSTR must NULL-terminate the string.
*
*  _full : The function initializes the entire buffer.
*
*  _part : The function initializes part of the buffer, and explicitly
*  indicates how much.
* 
*  Optional: Describes if the buffer itself is optional.
* 
*  <>   : The pointer to the buffer must not be NULL.
*
*  _opt : The pointer to the buffer might be NULL. It will be checked before
*  being dereferenced.
* 
*  Parameters: Gives explicit counts for the size and length of the buffer.
* 
*  <> : There is no explicit count. Use when neither _ecount nor _bcount is
*  used.
*
*  (size) : Only the buffer's total size is given. Use with _ecount or _bcount
*  but not _part.
*
*  (size,length) : The buffer's total size and initialized length are
*  given. Use with _ecount_part and _bcount_part.
* 
*  ----------------------------------------------------------------------------
*  Buffer Annotation Examples
* 
*  LWSTDAPI_(BOOL) StrToIntExA(
*      LPCSTR pszString,  //  No annotation required, const implies __in.
*      DWORD dwFlags,
*      __out int *piRet   // A pointer whose dereference will be filled in.
*  );
* 
*  void MyPaintingFunction(
*      __in HWND hwndControl,     //  An initialized read-only parameter.
*      __in_opt HDC hdcOptional,  //  An initialized read-only parameter that 
*                                 //  might be NULL.
*      __inout IPropertyStore *ppsStore // An initialized parameter that 
*                                       // may be freely used and modified.
*  );
* 
*  LWSTDAPI_(BOOL) PathCompactPathExA(
*      __out_ecount(cchMax) LPSTR pszOut, //  A string buffer with cch elements
*                                         //  that will be '\0' terminated 
*                                         //  on exit.
*      LPCSTR pszSrc,                     //  No annotation required, 
*                                         //  const implies __in.
*      UINT cchMax,                              
*      DWORD dwFlags
*  );
* 
*  HRESULT SHLocalAllocBytes(
*      size_t cb,
*      __deref_bcount(cb) T **ppv //  A pointer whose dereference will be set
*                                 //  to an uninitialized buffer with cb bytes.
*  );
* 
*  __inout_bcount_full(cb) : A buffer with cb elements that is fully
*  initialized at entry and exit, and may be written to by this function.
* 
*  __out_ecount_part(count, *countOut) : A buffer with count elements that
*  will be partially initialized by this function. The function indicates how
*  much it initialized by setting *countOut.
* 
************************************************************************/

#if (_MSC_VER >= 1400) && !defined(__midl) && !defined(_PREFAST_) && (__SPECSTRINGS_STRICT_LEVEL > 0)
#pragma once

#define __ecount(size)                                _SAL_VERSION_CHECK(__ecount)
#define __bcount(size)                                _SAL_VERSION_CHECK(__bcount)
#define __xcount(size)                                _SAL_VERSION_CHECK(__xcount)
#define __in                                          _SAL_VERSION_CHECK(__in)
#define __in_ecount(size)                             _SAL_VERSION_CHECK(__in_ecount)
#define __in_bcount(size)                             _SAL_VERSION_CHECK(__in_bcount)
#define __in_xcount(size)                             _SAL_VERSION_CHECK(__in_xcount)
#define __in_z                                        _SAL_VERSION_CHECK(__in_z)
#define __in_ecount_z(size)                           _SAL_VERSION_CHECK(__in_ecount_z)
#define __in_bcount_z(size)                           _SAL_VERSION_CHECK(__in_bcount_z)
#define __out                                         _SAL_VERSION_CHECK(__out)
#define __out_ecount(size)                            _SAL_VERSION_CHECK(__out_ecount)
#define __out_bcount(size)                            _SAL_VERSION_CHECK(__out_bcount)
#define __out_xcount(size)                            _SAL_VERSION_CHECK(__out_xcount)
#define __out_ecount_part(size,len)                   _SAL_VERSION_CHECK(__out_ecount_part)
#define __out_bcount_part(size,len)                   _SAL_VERSION_CHECK(__out_bcount_part)
#define __out_xcount_part(size,len)                   _SAL_VERSION_CHECK(__out_xcount_part)
#define __out_ecount_full(size)                       _SAL_VERSION_CHECK(__out_ecount_full)
#define __out_bcount_full(size)                       _SAL_VERSION_CHECK(__out_bcount_full)
#define __out_xcount_full(size)                       _SAL_VERSION_CHECK(__out_xcount_full)
#define __out_z				              _SAL_VERSION_CHECK(__out_z)
#define __out_ecount_z(size)                          _SAL_VERSION_CHECK(__out_ecount_z)
#define __out_bcount_z(size)                          _SAL_VERSION_CHECK(__out_bcount_z)
#define __inout                                       _SAL_VERSION_CHECK(__inout)
#define __inout_ecount(size)                          _SAL_VERSION_CHECK(__inout_ecount)
#define __inout_bcount(size)                          _SAL_VERSION_CHECK(__inout_bcount)
#define __inout_xcount(size)                          _SAL_VERSION_CHECK(__inout_xcount)
#define __inout_ecount_part(size,len)                 _SAL_VERSION_CHECK(__inout_ecount_part)
#define __inout_bcount_part(size,len)                 _SAL_VERSION_CHECK(__inout_bcount_part)
#define __inout_xcount_part(size,len)                 _SAL_VERSION_CHECK(__inout_xcount_part)
#define __inout_ecount_full(size)                     _SAL_VERSION_CHECK(__inout_ecount_full)
#define __inout_bcount_full(size)                     _SAL_VERSION_CHECK(__inout_bcount_full)
#define __inout_xcount_full(size)                     _SAL_VERSION_CHECK(__inout_xcount_full)
#define __inout_z                                     __allowed(on_parameter)
#define __inout_ecount_z(size)                        __allowed(on_parameter)
#define __inout_bcount_z(size)                        __allowed(on_parameter)
#define __ecount_opt(size)                            __allowed(on_parameter)
#define __bcount_opt(size)                            __allowed(on_parameter)
#define __xcount_opt(size)                            __allowed(on_parameter)
#define __in_opt                                      _SAL_VERSION_CHECK(__in_opt)
#define __in_ecount_opt(size)                         _SAL_VERSION_CHECK(__in_ecount_opt)
#define __in_bcount_opt(size)                         _SAL_VERSION_CHECK(__in_bcount_opt)
#define __in_z_opt                                    __allowed(on_parameter)
#define __in_ecount_z_opt(size)                       __allowed(on_parameter)
#define __in_bcount_z_opt(size)                       __allowed(on_parameter)
#define __in_xcount_opt(size)                         __allowed(on_parameter)
#define __out_opt                                     _SAL_VERSION_CHECK(__out_opt)
#define __out_ecount_opt(size)                        _SAL_VERSION_CHECK(__out_ecount_opt)
#define __out_bcount_opt(size)                        _SAL_VERSION_CHECK(__out_bcount_opt)
#define __out_xcount_opt(size)                        __allowed(on_parameter)
#define __out_ecount_part_opt(size,len)               __allowed(on_parameter)
#define __out_bcount_part_opt(size,len)               __allowed(on_parameter)
#define __out_xcount_part_opt(size,len)               __allowed(on_parameter)
#define __out_ecount_full_opt(size)                   __allowed(on_parameter)
#define __out_bcount_full_opt(size)                   __allowed(on_parameter)
#define __out_xcount_full_opt(size)                   __allowed(on_parameter)
#define __out_ecount_z_opt(size)                      __allowed(on_parameter)
#define __out_bcount_z_opt(size)                      __allowed(on_parameter)
#define __inout_opt                                   _SAL_VERSION_CHECK(__inout_opt)
#define __inout_ecount_opt(size)                      _SAL_VERSION_CHECK(__inout_ecount_opt)
#define __inout_bcount_opt(size)                      _SAL_VERSION_CHECK(__inout_bcount_opt)
#define __inout_xcount_opt(size)                      _SAL_VERSION_CHECK(__inout_xcount_opt)
#define __inout_ecount_part_opt(size,len)             _SAL_VERSION_CHECK(__inout_ecount_part_opt)
#define __inout_bcount_part_opt(size,len)             _SAL_VERSION_CHECK(__inout_bcount_part_opt)
#define __inout_xcount_part_opt(size,len)             _SAL_VERSION_CHECK(__inout_xcount_part_opt)
#define __inout_ecount_full_opt(size)                 _SAL_VERSION_CHECK(__inout_ecount_full_opt)
#define __inout_bcount_full_opt(size)                 _SAL_VERSION_CHECK(__inout_bcount_full_opt)
#define __inout_xcount_full_opt(size)                 _SAL_VERSION_CHECK(__inout_xcount_full_opt)
#define __inout_z_opt                                 __allowed(on_parameter)
#define __inout_ecount_z_opt(size)                    __allowed(on_parameter)
#define __inout_ecount_z_opt(size)                    __allowed(on_parameter)
#define __inout_bcount_z_opt(size)                    __allowed(on_parameter)
#define __deref_ecount(size)                          __allowed(on_parameter)
#define __deref_bcount(size)                          __allowed(on_parameter)
#define __deref_xcount(size)                          __allowed(on_parameter)
#define __deref_in                                    _SAL_VERSION_CHECK(__deref_in)
#define __deref_in_ecount(size)                       _SAL_VERSION_CHECK(__deref_in_ecount)
#define __deref_in_bcount(size)                       _SAL_VERSION_CHECK(__deref_in_bcount)
#define __deref_in_xcount(size)                       _SAL_VERSION_CHECK(__deref_in_xcount)
#define __deref_out                                   _SAL_VERSION_CHECK(__deref_out)
#define __deref_out_ecount(size)                      _SAL_VERSION_CHECK(__deref_out_ecount)
#define __deref_out_bcount(size)                      _SAL_VERSION_CHECK(__deref_out_bcount)
#define __deref_out_xcount(size)                      _SAL_VERSION_CHECK(__deref_out_xcount)
#define __deref_out_ecount_part(size,len)             _SAL_VERSION_CHECK(__deref_out_ecount_part)
#define __deref_out_bcount_part(size,len)             _SAL_VERSION_CHECK(__deref_out_bcount_part)
#define __deref_out_xcount_part(size,len)             _SAL_VERSION_CHECK(__deref_out_xcount_part)
#define __deref_out_ecount_full(size)                 _SAL_VERSION_CHECK(__deref_out_ecount_full)
#define __deref_out_bcount_full(size)                 _SAL_VERSION_CHECK(__deref_out_bcount_full)
#define __deref_out_xcount_full(size)                 _SAL_VERSION_CHECK(__deref_out_xcount_full)
#define __deref_out_z                                 __allowed(on_parameter)
#define __deref_out_ecount_z(size)                    __allowed(on_parameter)
#define __deref_out_bcount_z(size)                    __allowed(on_parameter)
#define __deref_inout                                 _SAL_VERSION_CHECK(__deref_inout)
#define __deref_inout_ecount(size)                    _SAL_VERSION_CHECK(__deref_inout_ecount)
#define __deref_inout_bcount(size)                    _SAL_VERSION_CHECK(__deref_inout_bcount)
#define __deref_inout_xcount(size)                    _SAL_VERSION_CHECK(__deref_inout_xcount)
#define __deref_inout_ecount_part(size,len)           __allowed(on_parameter)
#define __deref_inout_bcount_part(size,len)           __allowed(on_parameter)
#define __deref_inout_xcount_part(size,len)           __allowed(on_parameter)
#define __deref_inout_ecount_full(size)               __allowed(on_parameter)
#define __deref_inout_bcount_full(size)               __allowed(on_parameter)
#define __deref_inout_xcount_full(size)               __allowed(on_parameter)
#define __deref_inout_z                               __allowed(on_parameter)
#define __deref_inout_ecount_z(size)                  __allowed(on_parameter)
#define __deref_inout_bcount_z(size)                  __allowed(on_parameter)
#define __deref_ecount_opt(size)                      __allowed(on_parameter)
#define __deref_bcount_opt(size)                      __allowed(on_parameter)
#define __deref_xcount_opt(size)                      __allowed(on_parameter)
#define __deref_in_opt                                __allowed(on_parameter)
#define __deref_in_opt_out                            __allowed(on_parameter)
#define __deref_in_ecount_opt(size)                   __allowed(on_parameter)
#define __deref_in_bcount_opt(size)                   __allowed(on_parameter)
#define __deref_in_xcount_opt(size)                   __allowed(on_parameter)
#define __deref_out_opt                               _SAL_VERSION_CHECK(__deref_out_opt)
#define __deref_out_ecount_opt(size)                  _SAL_VERSION_CHECK(__deref_out_ecount_opt)
#define __deref_out_bcount_opt(size)                  _SAL_VERSION_CHECK(__deref_out_bcount_opt)
#define __deref_out_xcount_opt(size)                  _SAL_VERSION_CHECK(__deref_out_xcount_opt)
#define __deref_out_ecount_part_opt(size,len)         _SAL_VERSION_CHECK(__deref_out_ecount_part_opt)
#define __deref_out_bcount_part_opt(size,len)         _SAL_VERSION_CHECK(__deref_out_bcount_part_opt)
#define __deref_out_xcount_part_opt(size,len)         _SAL_VERSION_CHECK(__deref_out_xcount_part_opt)
#define __deref_out_ecount_full_opt(size)             _SAL_VERSION_CHECK(__deref_out_ecount_full_opt)
#define __deref_out_bcount_full_opt(size)             _SAL_VERSION_CHECK(__deref_out_bcount_full_opt)
#define __deref_out_xcount_full_opt(size)             _SAL_VERSION_CHECK(__deref_out_xcount_full_opt)
#define __deref_out_z_opt                             __allowed(on_parameter)
#define __deref_out_ecount_z_opt(size)                __allowed(on_parameter)
#define __deref_out_bcount_z_opt(size)                __allowed(on_parameter)
#define __deref_inout_opt                             __allowed(on_parameter)
#define __deref_inout_ecount_opt(size)                __allowed(on_parameter)
#define __deref_inout_bcount_opt(size)                __allowed(on_parameter)
#define __deref_inout_xcount_opt(size)                __allowed(on_parameter)
#define __deref_inout_ecount_part_opt(size,len)       __allowed(on_parameter)
#define __deref_inout_bcount_part_opt(size,len)       __allowed(on_parameter)
#define __deref_inout_xcount_part_opt(size,len)       __allowed(on_parameter)
#define __deref_inout_ecount_full_opt(size)           __allowed(on_parameter)
#define __deref_inout_bcount_full_opt(size)           __allowed(on_parameter)
#define __deref_inout_xcount_full_opt(size)           __allowed(on_parameter)
#define __deref_inout_z_opt                           __allowed(on_parameter)
#define __deref_inout_ecount_z_opt(size)              __allowed(on_parameter)
#define __deref_inout_bcount_z_opt(size)              __allowed(on_parameter)
#define __deref_opt_ecount(size)                      __allowed(on_parameter)
#define __deref_opt_bcount(size)                      __allowed(on_parameter)
#define __deref_opt_xcount(size)                      __allowed(on_parameter)
#define __deref_opt_in                                __allowed(on_parameter)
#define __deref_opt_in_ecount(size)                   __allowed(on_parameter)
#define __deref_opt_in_bcount(size)                   __allowed(on_parameter)
#define __deref_opt_in_xcount(size)                   __allowed(on_parameter)
#define __deref_opt_out                               _SAL_VERSION_CHECK(__deref_opt_out)
#define __deref_opt_out_ecount(size)                  _SAL_VERSION_CHECK(__deref_opt_out_ecount)
#define __deref_opt_out_bcount(size)                  _SAL_VERSION_CHECK(__deref_opt_out_bcount)
#define __deref_opt_out_xcount(size)                  _SAL_VERSION_CHECK(__deref_opt_out_xcount)
#define __deref_opt_out_ecount_part(size,len)         __allowed(on_parameter)
#define __deref_opt_out_bcount_part(size,len)         __allowed(on_parameter)
#define __deref_opt_out_xcount_part(size,len)         __allowed(on_parameter)
#define __deref_opt_out_ecount_full(size)             __allowed(on_parameter)
#define __deref_opt_out_bcount_full(size)             __allowed(on_parameter)
#define __deref_opt_out_xcount_full(size)             __allowed(on_parameter)
#define __deref_opt_inout                             __allowed(on_parameter)
#define __deref_opt_inout_ecount(size)                __allowed(on_parameter)
#define __deref_opt_inout_bcount(size)                __allowed(on_parameter)
#define __deref_opt_inout_xcount(size)                __allowed(on_parameter)
#define __deref_opt_inout_ecount_part(size,len)       __allowed(on_parameter)
#define __deref_opt_inout_bcount_part(size,len)       __allowed(on_parameter)
#define __deref_opt_inout_xcount_part(size,len)       __allowed(on_parameter)
#define __deref_opt_inout_ecount_full(size)           __allowed(on_parameter)
#define __deref_opt_inout_bcount_full(size)           __allowed(on_parameter)
#define __deref_opt_inout_xcount_full(size)           __allowed(on_parameter)
#define __deref_opt_inout_z                           __allowed(on_parameter)
#define __deref_opt_inout_ecount_z(size)              __allowed(on_parameter)
#define __deref_opt_inout_bcount_z(size)              __allowed(on_parameter)
#define __deref_opt_ecount_opt(size)                  __allowed(on_parameter)
#define __deref_opt_bcount_opt(size)                  __allowed(on_parameter)
#define __deref_opt_xcount_opt(size)                  __allowed(on_parameter)
#define __deref_opt_in_opt                            __allowed(on_parameter)
#define __deref_opt_in_ecount_opt(size)               __allowed(on_parameter)
#define __deref_opt_in_bcount_opt(size)               __allowed(on_parameter)
#define __deref_opt_in_xcount_opt(size)               __allowed(on_parameter)
#define __deref_opt_out_opt                           __allowed(on_parameter)
#define __deref_opt_out_ecount_opt(size)              __allowed(on_parameter)
#define __deref_opt_out_bcount_opt(size)              __allowed(on_parameter)
#define __deref_opt_out_xcount_opt(size)              __allowed(on_parameter)
#define __deref_opt_out_ecount_part_opt(size,len)     __allowed(on_parameter)
#define __deref_opt_out_bcount_part_opt(size,len)     __allowed(on_parameter)
#define __deref_opt_out_xcount_part_opt(size,len)     __allowed(on_parameter)
#define __deref_opt_out_ecount_full_opt(size)         __allowed(on_parameter)
#define __deref_opt_out_bcount_full_opt(size)         __allowed(on_parameter)
#define __deref_opt_out_xcount_full_opt(size)         __allowed(on_parameter)  
#define __deref_opt_out_z_opt                         __allowed(on_parameter)
#define __deref_opt_out_ecount_z_opt(size)            __allowed(on_parameter)
#define __deref_opt_out_bcount_z_opt(size)            __allowed(on_parameter)
#define __deref_opt_inout_opt                         __allowed(on_parameter)
#define __deref_opt_inout_ecount_opt(size)            __allowed(on_parameter)
#define __deref_opt_inout_bcount_opt(size)            __allowed(on_parameter)
#define __deref_opt_inout_xcount_opt(size)            __allowed(on_parameter)
#define __deref_opt_inout_ecount_part_opt(size,len)   __allowed(on_parameter) 
#define __deref_opt_inout_bcount_part_opt(size,len)   __allowed(on_parameter)
#define __deref_opt_inout_xcount_part_opt(size,len)   __allowed(on_parameter)
#define __deref_opt_inout_ecount_full_opt(size)       __allowed(on_parameter)
#define __deref_opt_inout_bcount_full_opt(size)       __allowed(on_parameter)
#define __deref_opt_inout_xcount_full_opt(size)       __allowed(on_parameter)
#define __deref_opt_inout_z_opt                       __allowed(on_parameter)
#define __deref_opt_inout_ecount_z_opt(size)          __allowed(on_parameter)
#define __deref_opt_inout_bcount_z_opt(size)          __allowed(on_parameter)
#define __deref_in_ecount_iterator(size,incr)         __allowed(on_parameter)
#define __deref_out_ecount_iterator(size,incr)        __allowed(on_parameter)
#define __deref_inout_ecount_iterator(size,incr)      __allowed(on_parameter)
#define __deref_realloc_bcount(insize,outsize)        __allowed(on_parameter)