// A Bison parser, made by GNU Bison 3.3.2.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2019 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


/**
 ** \file E:\Software\manta-ray_refactor\manta-ray\dependencies\submodules\piranha\scripts\/../autogen/parser.auto.h
 ** Define the piranha::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.

#ifndef YY_YY_E_SOFTWARE_MANTA_RAY_REFACTOR_MANTA_RAY_DEPENDENCIES_SUBMODULES_PIRANHA_SCRIPTS_AUTOGEN_PARSER_AUTO_H_INCLUDED
# define YY_YY_E_SOFTWARE_MANTA_RAY_REFACTOR_MANTA_RAY_DEPENDENCIES_SUBMODULES_PIRANHA_SCRIPTS_AUTOGEN_PARSER_AUTO_H_INCLUDED
// //                    "%code requires" blocks.
#line 8 "E:\\Software\\manta-ray_refactor\\manta-ray\\dependencies\\submodules\\piranha\\scripts\\/../flex-bison/specification.y" // lalr1.cc:401

    namespace piranha {
        class IrCompilationUnit;
        class Scanner;
    }

    #include "../include/ir_compilation_unit.h"
    #include "../include/ir_node.h"
    #include "../include/ir_attribute_list.h"
    #include "../include/ir_attribute.h"
    #include "../include/ir_value.h"
    #include "../include/ir_value_constant.h"
    #include "../include/ir_binary_operator.h"
    #include "../include/ir_import_statement.h"
    #include "../include/ir_token_info.h"
    #include "../include/ir_node_definition.h"
    #include "../include/ir_attribute_definition.h"
    #include "../include/ir_attribute_definition_list.h"
    #include "../include/compilation_error.h"
    #include "../include/ir_structure_list.h"
    #include "../include/ir_visibility.h"
    #include "../include/ir_unary_operator.h"

    #include <string>

    #ifndef YY_NULLPTR
    #if defined __cplusplus && 201103L <= __cplusplus
    #define YY_NULLPTR nullptr
    #else
    #define YY_NULLPTR 0
    #endif
    #endif

    # define YYLLOC_DEFAULT(Cur, Rhs, N)                    \
    do                                                      \
        if (N) {                                            \
            (Cur).combine(&YYRHSLOC(Rhs, 1));               \
            (Cur).combine(&YYRHSLOC(Rhs, N));               \
        }                                                   \
        else {                                              \
            (Cur).combine(&YYRHSLOC(Rhs, 0));               \
        }                                                   \
    while (0)

    /* Remove annoying compiler warnings */
    #ifdef _MSC_VER
    /* warning C4065: switch statement contains 'default' but no 'case' labels */
    #pragma warning (disable: 4065)
    #endif

#line 99 "E:\\Software\\manta-ray_refactor\\manta-ray\\dependencies\\submodules\\piranha\\scripts\\/../autogen/parser.auto.h" // lalr1.cc:401

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif

#include <typeinfo>
#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
#endif


#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

#line 5 "E:\\Software\\manta-ray_refactor\\manta-ray\\dependencies\\submodules\\piranha\\scripts\\/../flex-bison/specification.y" // lalr1.cc:401
namespace piranha {
#line 215 "E:\\Software\\manta-ray_refactor\\manta-ray\\dependencies\\submodules\\piranha\\scripts\\/../autogen/parser.auto.h" // lalr1.cc:401



  /// A Bison parser.
  class Parser
  {
  public:
#ifndef YYSTYPE
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class semantic_type
  {
  public:
    /// Type of *this.
    typedef semantic_type self_type;

    /// Empty construction.
    semantic_type () YY_NOEXCEPT
      : yybuffer_ ()
      , yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    semantic_type (YY_RVREF (T) t)
      : yytypeid_ (&typeid (T))
    {
      YYASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

    /// Destruction, allowed only if empty.
    ~semantic_type () YY_NOEXCEPT
    {
      YYASSERT (!yytypeid_);
    }

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= size);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= size);
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == *that.yytypeid_);
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
    /// Prohibit blind copies.
    self_type& operator= (const self_type&);
    semantic_type (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // attribute
      char dummy1[sizeof (piranha::IrAttribute *)];

      // port_declaration
      // port_status
      // port_value
      // port_connection
      // documented_port_definition
      char dummy2[sizeof (piranha::IrAttributeDefinition *)];

      // port_definitions
      char dummy3[sizeof (piranha::IrAttributeDefinitionList *)];

      // connection_block
      // attribute_list
      char dummy4[sizeof (piranha::IrAttributeList *)];

      // import_statement
      // import_statement_visibility
      // import_statement_short_name
      char dummy5[sizeof (piranha::IrImportStatement *)];

      // node
      // inline_node
      char dummy6[sizeof (piranha::IrNode *)];

      // node_name
      // node_inline
      // node_shadow
      // node_definition
      // specific_node_definition
      // immediate_node_definition
      // node_decorator
      char dummy7[sizeof (piranha::IrNodeDefinition *)];

      // node_list
      char dummy8[sizeof (piranha::IrNodeList *)];

      // type_name_namespace
      char dummy9[sizeof (piranha::IrTokenInfoSet<std::string, 2>)];

      // BOOL
      char dummy10[sizeof (piranha::IrTokenInfo_bool)];

      // FLOAT
      char dummy11[sizeof (piranha::IrTokenInfo_float)];

      // INT
      char dummy12[sizeof (piranha::IrTokenInfo_int)];

      // CHAR
      // IMPORT
      // AS
      // NODE
      // INLINE
      // ALIAS
      // INPUT
      // OUTPUT
      // MODIFY
      // TOGGLE
      // LABEL
      // STRING
      // DECORATOR
      // PUBLIC
      // PRIVATE
      // BUILTIN_POINTER
      // NAMESPACE_POINTER
      // UNRECOGNIZED
      // OPERATOR
      // MODULE
      // AUTO
      // '='
      // '+'
      // '-'
      // '/'
      // '*'
      // '('
      // ')'
      // '{'
      // '}'
      // '['
      // ']'
      // ':'
      // ';'
      // ','
      // '.'
      // '^'
      // type_name
      // standard_operator
      // string
      char dummy13[sizeof (piranha::IrTokenInfo_string)];

      // label_value
      // value
      // constant
      // atomic_value
      // primary_exp
      // data_access
      // unary_exp
      // mul_exp
      // add_exp
      char dummy14[sizeof (piranha::IrValue *)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[size];
    } yybuffer_;

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };

#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef piranha::IrTokenInfo location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m)
        : std::runtime_error (m)
        , location (l)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
        , location (s.location)
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;

      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        END = 0,
        CHAR = 258,
        IMPORT = 259,
        AS = 260,
        NODE = 261,
        INLINE = 262,
        ALIAS = 263,
        INPUT = 264,
        OUTPUT = 265,
        MODIFY = 266,
        TOGGLE = 267,
        LABEL = 268,
        INT = 269,
        FLOAT = 270,
        BOOL = 271,
        STRING = 272,
        DECORATOR = 273,
        PUBLIC = 274,
        PRIVATE = 275,
        BUILTIN_POINTER = 276,
        NAMESPACE_POINTER = 277,
        UNRECOGNIZED = 278,
        OPERATOR = 279,
        MODULE = 280,
        AUTO = 281
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get ().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ()
        : value ()
        , location ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that);
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, location_type&& l)
        : Base (t)
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const location_type& l)
        : Base (t)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, piranha::IrAttribute *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const piranha::IrAttribute *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, piranha::IrAttributeDefinition *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const piranha::IrAttributeDefinition *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, piranha::IrAttributeDefinitionList *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const piranha::IrAttributeDefinitionList *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, piranha::IrAttributeList *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const piranha::IrAttributeList *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, piranha::IrImportStatement *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const piranha::IrImportStatement *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, piranha::IrNode *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const piranha::IrNode *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, piranha::IrNodeDefinition *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const piranha::IrNodeDefinition *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, piranha::IrNodeList *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const piranha::IrNodeList *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, piranha::IrTokenInfoSet<std::string, 2>&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const piranha::IrTokenInfoSet<std::string, 2>& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, piranha::IrTokenInfo_bool&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const piranha::IrTokenInfo_bool& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, piranha::IrTokenInfo_float&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const piranha::IrTokenInfo_float& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, piranha::IrTokenInfo_int&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const piranha::IrTokenInfo_int& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, piranha::IrTokenInfo_string&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const piranha::IrTokenInfo_string& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, piranha::IrValue *&& v, location_type&& l)
        : Base (t)
        , value (std::move (v))
        , location (std::move (l))
      {}
#else
      basic_symbol (typename Base::kind_type t, const piranha::IrValue *& v, const location_type& l)
        : Base (t)
        , value (v)
        , location (l)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }

      /// Destroy contents, and record that is empty.
      void clear ()
      {
        // User destructor.
        symbol_number_type yytype = this->type_get ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yytype)
        {
       default:
          break;
        }

        // Type destructor.
switch (yytype)
    {
      case 74: // attribute
        value.template destroy< piranha::IrAttribute * > ();
        break;

      case 66: // port_declaration
      case 67: // port_status
      case 68: // port_value
      case 69: // port_connection
      case 70: // documented_port_definition
        value.template destroy< piranha::IrAttributeDefinition * > ();
        break;

      case 65: // port_definitions
        value.template destroy< piranha::IrAttributeDefinitionList * > ();
        break;

      case 72: // connection_block
      case 73: // attribute_list
        value.template destroy< piranha::IrAttributeList * > ();
        break;

      case 50: // import_statement
      case 51: // import_statement_visibility
      case 52: // import_statement_short_name
        value.template destroy< piranha::IrImportStatement * > ();
        break;

      case 55: // node
      case 71: // inline_node
        value.template destroy< piranha::IrNode * > ();
        break;

      case 58: // node_name
      case 59: // node_inline
      case 60: // node_shadow
      case 61: // node_definition
      case 62: // specific_node_definition
      case 63: // immediate_node_definition
      case 64: // node_decorator
        value.template destroy< piranha::IrNodeDefinition * > ();
        break;

      case 56: // node_list
        value.template destroy< piranha::IrNodeList * > ();
        break;

      case 54: // type_name_namespace
        value.template destroy< piranha::IrTokenInfoSet<std::string, 2> > ();
        break;

      case 16: // BOOL
        value.template destroy< piranha::IrTokenInfo_bool > ();
        break;

      case 15: // FLOAT
        value.template destroy< piranha::IrTokenInfo_float > ();
        break;

      case 14: // INT
        value.template destroy< piranha::IrTokenInfo_int > ();
        break;

      case 3: // CHAR
      case 4: // IMPORT
      case 5: // AS
      case 6: // NODE
      case 7: // INLINE
      case 8: // ALIAS
      case 9: // INPUT
      case 10: // OUTPUT
      case 11: // MODIFY
      case 12: // TOGGLE
      case 13: // LABEL
      case 17: // STRING
      case 18: // DECORATOR
      case 19: // PUBLIC
      case 20: // PRIVATE
      case 21: // BUILTIN_POINTER
      case 22: // NAMESPACE_POINTER
      case 23: // UNRECOGNIZED
      case 24: // OPERATOR
      case 25: // MODULE
      case 26: // AUTO
      case 27: // '='
      case 28: // '+'
      case 29: // '-'
      case 30: // '/'
      case 31: // '*'
      case 32: // '('
      case 33: // ')'
      case 34: // '{'
      case 35: // '}'
      case 36: // '['
      case 37: // ']'
      case 38: // ':'
      case 39: // ';'
      case 40: // ','
      case 41: // '.'
      case 42: // '^'
      case 53: // type_name
      case 57: // standard_operator
      case 77: // string
        value.template destroy< piranha::IrTokenInfo_string > ();
        break;

      case 75: // label_value
      case 76: // value
      case 78: // constant
      case 79: // atomic_value
      case 80: // primary_exp
      case 81: // data_access
      case 82: // unary_exp
      case 83: // mul_exp
      case 84: // add_exp
        value.template destroy< piranha::IrValue * > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_type (by_type&& that);
#endif

      /// Copy constructor.
      by_type (const by_type& that);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const YY_NOEXCEPT;

      /// The token.
      token_type token () const YY_NOEXCEPT;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_type>
    {
      /// Superclass.
      typedef basic_symbol<by_type> super_type;

      /// Empty symbol.
      symbol_type () {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, location_type l)
        : super_type(token_type (tok), std::move (l))
      {
        YYASSERT (tok == token::END || tok == 33);
      }
#else
      symbol_type (int tok, const location_type& l)
        : super_type(token_type (tok), l)
      {
        YYASSERT (tok == token::END || tok == 33);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, piranha::IrTokenInfo_bool v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YYASSERT (tok == token::BOOL);
      }
#else
      symbol_type (int tok, const piranha::IrTokenInfo_bool& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YYASSERT (tok == token::BOOL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, piranha::IrTokenInfo_float v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YYASSERT (tok == token::FLOAT);
      }
#else
      symbol_type (int tok, const piranha::IrTokenInfo_float& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YYASSERT (tok == token::FLOAT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, piranha::IrTokenInfo_int v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YYASSERT (tok == token::INT);
      }
#else
      symbol_type (int tok, const piranha::IrTokenInfo_int& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YYASSERT (tok == token::INT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, piranha::IrTokenInfo_string v, location_type l)
        : super_type(token_type (tok), std::move (v), std::move (l))
      {
        YYASSERT (tok == token::CHAR || tok == token::IMPORT || tok == token::AS || tok == token::NODE || tok == token::INLINE || tok == token::ALIAS || tok == token::INPUT || tok == token::OUTPUT || tok == token::MODIFY || tok == token::TOGGLE || tok == token::LABEL || tok == token::STRING || tok == token::DECORATOR || tok == token::PUBLIC || tok == token::PRIVATE || tok == token::BUILTIN_POINTER || tok == token::NAMESPACE_POINTER || tok == token::UNRECOGNIZED || tok == token::OPERATOR || tok == token::MODULE || tok == token::AUTO || tok == 61 || tok == 43 || tok == 45 || tok == 47 || tok == 42 || tok == 40 || tok == 41 || tok == 123 || tok == 125 || tok == 91 || tok == 93 || tok == 58 || tok == 59 || tok == 44 || tok == 46 || tok == 94);
      }
#else
      symbol_type (int tok, const piranha::IrTokenInfo_string& v, const location_type& l)
        : super_type(token_type (tok), v, l)
      {
        YYASSERT (tok == token::CHAR || tok == token::IMPORT || tok == token::AS || tok == token::NODE || tok == token::INLINE || tok == token::ALIAS || tok == token::INPUT || tok == token::OUTPUT || tok == token::MODIFY || tok == token::TOGGLE || tok == token::LABEL || tok == token::STRING || tok == token::DECORATOR || tok == token::PUBLIC || tok == token::PRIVATE || tok == token::BUILTIN_POINTER || tok == token::NAMESPACE_POINTER || tok == token::UNRECOGNIZED || tok == token::OPERATOR || tok == token::MODULE || tok == token::AUTO || tok == 61 || tok == 43 || tok == 45 || tok == 47 || tok == 42 || tok == 40 || tok == 41 || tok == 123 || tok == 125 || tok == 91 || tok == 93 || tok == 58 || tok == 59 || tok == 44 || tok == 46 || tok == 94);
      }
#endif
    };

    /// Build a parser object.
    Parser (Scanner &scanner_yyarg, IrCompilationUnit &driver_yyarg);
    virtual ~Parser ();

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    // Implementation of make_symbol for each symbol type.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_END (location_type l)
      {
        return symbol_type (token::END, std::move (l));
      }
#else
      static
      symbol_type
      make_END (const location_type& l)
      {
        return symbol_type (token::END, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CHAR (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::CHAR, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_CHAR (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::CHAR, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IMPORT (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::IMPORT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_IMPORT (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::IMPORT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AS (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::AS, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_AS (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::AS, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NODE (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::NODE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_NODE (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::NODE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INLINE (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::INLINE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INLINE (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::INLINE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ALIAS (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::ALIAS, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_ALIAS (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::ALIAS, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INPUT (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::INPUT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INPUT (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::INPUT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OUTPUT (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::OUTPUT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_OUTPUT (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::OUTPUT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MODIFY (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::MODIFY, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_MODIFY (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::MODIFY, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOGGLE (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::TOGGLE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_TOGGLE (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::TOGGLE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LABEL (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::LABEL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_LABEL (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::LABEL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INT (piranha::IrTokenInfo_int v, location_type l)
      {
        return symbol_type (token::INT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_INT (const piranha::IrTokenInfo_int& v, const location_type& l)
      {
        return symbol_type (token::INT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FLOAT (piranha::IrTokenInfo_float v, location_type l)
      {
        return symbol_type (token::FLOAT, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_FLOAT (const piranha::IrTokenInfo_float& v, const location_type& l)
      {
        return symbol_type (token::FLOAT, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BOOL (piranha::IrTokenInfo_bool v, location_type l)
      {
        return symbol_type (token::BOOL, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_BOOL (const piranha::IrTokenInfo_bool& v, const location_type& l)
      {
        return symbol_type (token::BOOL, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STRING (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::STRING, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_STRING (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::STRING, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DECORATOR (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::DECORATOR, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_DECORATOR (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::DECORATOR, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PUBLIC (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::PUBLIC, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_PUBLIC (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::PUBLIC, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PRIVATE (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::PRIVATE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_PRIVATE (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::PRIVATE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BUILTIN_POINTER (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::BUILTIN_POINTER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_BUILTIN_POINTER (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::BUILTIN_POINTER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NAMESPACE_POINTER (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::NAMESPACE_POINTER, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_NAMESPACE_POINTER (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::NAMESPACE_POINTER, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UNRECOGNIZED (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::UNRECOGNIZED, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_UNRECOGNIZED (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::UNRECOGNIZED, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OPERATOR (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::OPERATOR, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_OPERATOR (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::OPERATOR, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MODULE (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::MODULE, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_MODULE (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::MODULE, v, l);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AUTO (piranha::IrTokenInfo_string v, location_type l)
      {
        return symbol_type (token::AUTO, std::move (v), std::move (l));
      }
#else
      static
      symbol_type
      make_AUTO (const piranha::IrTokenInfo_string& v, const location_type& l)
      {
        return symbol_type (token::AUTO, v, l);
      }
#endif


  private:
    /// This class is not copyable.
    Parser (const Parser&);
    Parser& operator= (const Parser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (int t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const short yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const signed char yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const short yytable_[];

  static const short yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


#if YYDEBUG
    /// For a symbol, its name in clear.
    static const char* const yytname_[];

  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      enum { empty_state = -1 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::reverse_iterator iterator;
      typedef typename S::const_reverse_iterator const_iterator;
      typedef typename S::size_type size_type;

      stack (size_type n = 200)
        : seq_ (n)
      {}

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (size_type i)
      {
        return seq_[size () - 1 - i];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (int i)
      {
        return operator[] (size_type (i));
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (size_type i) const
      {
        return seq_[size () - 1 - i];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (int i) const
      {
        return operator[] (size_type (i));
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (int n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      size_type
      size () const YY_NOEXCEPT
      {
        return seq_.size ();
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.rbegin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.rend ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, int range)
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (int i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        int range_;
      };

    private:
      stack (const stack&);
      stack& operator= (const stack&);
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1);

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 307,     ///< Last index in yytable_.
      yynnts_ = 41,  ///< Number of nonterminal symbols.
      yyfinal_ = 57, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 44  ///< Number of tokens.
    };


    // User arguments.
    Scanner &scanner;
    IrCompilationUnit &driver;
  };


#line 5 "E:\\Software\\manta-ray_refactor\\manta-ray\\dependencies\\submodules\\piranha\\scripts\\/../flex-bison/specification.y" // lalr1.cc:401
} // piranha
#line 1839 "E:\\Software\\manta-ray_refactor\\manta-ray\\dependencies\\submodules\\piranha\\scripts\\/../autogen/parser.auto.h" // lalr1.cc:401




#endif // !YY_YY_E_SOFTWARE_MANTA_RAY_REFACTOR_MANTA_RAY_DEPENDENCIES_SUBMODULES_PIRANHA_SCRIPTS_AUTOGEN_PARSER_AUTO_H_INCLUDED
