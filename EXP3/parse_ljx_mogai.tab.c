/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parse_ljx_mogai.y"

    #include <stdio.h>
    #include <stdlib.h>
    #include "struct_of_analyser.c"

    #define YYSTYPE treeNode*

    int line_check[5000];
    extern int yylineno;
    void yyerror(char*);
    void myerror(char *msg);
    int yylex();
    int error_count = 0;
    Tree* myTree;

#line 87 "parse_ljx_mogai.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
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

#include "parse_ljx_mogai.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_FLOAT = 4,                      /* FLOAT  */
  YYSYMBOL_TYPE = 5,                       /* TYPE  */
  YYSYMBOL_LF = 6,                         /* LF  */
  YYSYMBOL_ID = 7,                         /* ID  */
  YYSYMBOL_SEMI = 8,                       /* SEMI  */
  YYSYMBOL_COMMA = 9,                      /* COMMA  */
  YYSYMBOL_DOT = 10,                       /* DOT  */
  YYSYMBOL_ASSIGNOP = 11,                  /* ASSIGNOP  */
  YYSYMBOL_RELOP = 12,                     /* RELOP  */
  YYSYMBOL_PLUS = 13,                      /* PLUS  */
  YYSYMBOL_MINUS = 14,                     /* MINUS  */
  YYSYMBOL_STAR = 15,                      /* STAR  */
  YYSYMBOL_DIV = 16,                       /* DIV  */
  YYSYMBOL_AND = 17,                       /* AND  */
  YYSYMBOL_OR = 18,                        /* OR  */
  YYSYMBOL_NOT = 19,                       /* NOT  */
  YYSYMBOL_LP = 20,                        /* LP  */
  YYSYMBOL_RP = 21,                        /* RP  */
  YYSYMBOL_LB = 22,                        /* LB  */
  YYSYMBOL_RB = 23,                        /* RB  */
  YYSYMBOL_LC = 24,                        /* LC  */
  YYSYMBOL_RC = 25,                        /* RC  */
  YYSYMBOL_STRUCT = 26,                    /* STRUCT  */
  YYSYMBOL_RETURN = 27,                    /* RETURN  */
  YYSYMBOL_IF = 28,                        /* IF  */
  YYSYMBOL_ELSE = 29,                      /* ELSE  */
  YYSYMBOL_WHILE = 30,                     /* WHILE  */
  YYSYMBOL_LOWER_THAN_ELSE = 31,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 32,                  /* $accept  */
  YYSYMBOL_Program = 33,                   /* Program  */
  YYSYMBOL_ExtDefList = 34,                /* ExtDefList  */
  YYSYMBOL_ExtDef = 35,                    /* ExtDef  */
  YYSYMBOL_ExtDecList = 36,                /* ExtDecList  */
  YYSYMBOL_Specifier = 37,                 /* Specifier  */
  YYSYMBOL_StructSpecifier = 38,           /* StructSpecifier  */
  YYSYMBOL_OptTag = 39,                    /* OptTag  */
  YYSYMBOL_Tag = 40,                       /* Tag  */
  YYSYMBOL_VarDec = 41,                    /* VarDec  */
  YYSYMBOL_FunDec = 42,                    /* FunDec  */
  YYSYMBOL_VarList = 43,                   /* VarList  */
  YYSYMBOL_ParamDec = 44,                  /* ParamDec  */
  YYSYMBOL_CompSt = 45,                    /* CompSt  */
  YYSYMBOL_StmtList = 46,                  /* StmtList  */
  YYSYMBOL_Stmt = 47,                      /* Stmt  */
  YYSYMBOL_DefList = 48,                   /* DefList  */
  YYSYMBOL_Def = 49,                       /* Def  */
  YYSYMBOL_DecList = 50,                   /* DecList  */
  YYSYMBOL_Dec = 51,                       /* Dec  */
  YYSYMBOL_Exp = 52,                       /* Exp  */
  YYSYMBOL_Args = 53                       /* Args  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   300

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  123

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   286


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    47,    47,    52,    57,    65,    71,    76,    82,    90,
      94,   112,   116,   122,   130,   137,   141,   148,   155,   159,
     168,   175,   183,   189,   195,   203,   212,   217,   223,   228,
     232,   238,   246,   256,   264,   275,   280,   286,   292,   303,
     307,   315,   319,   336,   342,   348,   354,   360,   366,   372,
     378,   384,   390,   395,   400,   407,   413,   420,   426,   429,
     432,   435,   443,   451,   459,   470,   476
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INT", "FLOAT", "TYPE",
  "LF", "ID", "SEMI", "COMMA", "DOT", "ASSIGNOP", "RELOP", "PLUS", "MINUS",
  "STAR", "DIV", "AND", "OR", "NOT", "LP", "RP", "LB", "RB", "LC", "RC",
  "STRUCT", "RETURN", "IF", "ELSE", "WHILE", "LOWER_THAN_ELSE", "$accept",
  "Program", "ExtDefList", "ExtDef", "ExtDecList", "Specifier",
  "StructSpecifier", "OptTag", "Tag", "VarDec", "FunDec", "VarList",
  "ParamDec", "CompSt", "StmtList", "Stmt", "DefList", "Def", "DecList",
  "Dec", "Exp", "Args", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-104)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-37)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       1,  -104,     7,    16,  -104,     1,    40,  -104,     2,    27,
    -104,  -104,  -104,     5,  -104,    46,    -2,    48,    12,    47,
    -104,    55,    72,  -104,    39,  -104,    55,    55,    51,    39,
    -104,    55,    56,    70,  -104,  -104,    57,    67,    -3,    74,
      76,    80,  -104,  -104,    71,  -104,     1,  -104,  -104,  -104,
      87,   152,   152,   116,   152,    93,    94,  -104,    83,    67,
      88,   152,  -104,    55,  -104,  -104,   108,    68,    35,    95,
     206,   165,   152,   152,  -104,  -104,  -104,  -104,    85,   130,
     152,   152,   152,   152,   152,   152,   152,   138,   245,  -104,
      97,  -104,   179,   100,  -104,  -104,  -104,   219,   232,  -104,
    -104,   245,   267,    68,    68,    35,    35,   278,   256,   101,
     192,  -104,   152,  -104,    67,    67,  -104,  -104,  -104,    96,
    -104,    67,  -104
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,    11,    15,     0,     2,     3,     0,    12,    17,     0,
      14,     1,     4,    18,     6,     0,     9,     0,     0,     0,
       5,     0,     0,     8,     0,     7,     0,     0,     0,     0,
      21,     0,     0,    23,    18,    10,     0,    27,    41,     0,
      39,     0,    13,    35,    24,    20,     0,    19,    59,    60,
      58,     0,     0,     0,     0,     0,     0,    29,     0,    27,
       0,     0,    38,     0,    37,    22,     0,    52,    53,     0,
       0,     0,     0,     0,    25,    26,    34,    28,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    42,    40,
       0,    55,    66,     0,    61,    51,    30,     0,     0,    57,
      64,    43,    46,    47,    48,    49,    50,    44,    45,     0,
       0,    62,     0,    54,     0,     0,    63,    56,    65,    31,
      33,     0,    32
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -104,  -104,   121,  -104,   111,     4,  -104,  -104,  -104,    18,
    -104,    92,  -104,   123,    84,  -103,    31,  -104,   -22,  -104,
     -51,    34
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     3,     4,     5,    15,    27,     7,     9,    10,    38,
      17,    32,    33,    57,    58,    59,    28,    29,    39,    40,
      60,    93
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      67,    68,    70,    71,     6,    41,     1,    21,    61,     6,
      88,   119,   120,    26,     8,    92,    11,     1,   122,    22,
      22,    97,    98,    31,    16,    19,   -16,     2,   101,   102,
     103,   104,   105,   106,   107,   108,   110,   -36,     2,    16,
      26,    89,   -36,   -36,     1,    78,   -36,    13,    14,    44,
      31,    18,     1,   -36,    20,    37,    23,    87,   -36,   -36,
      43,    92,    34,   -36,   -36,     2,   -36,   -36,    30,   -36,
      48,    49,    24,     2,    50,    36,    42,    45,    78,    46,
      47,    51,    62,    83,    84,    63,    52,    53,    64,    76,
      87,    24,    99,    22,    54,    55,    77,    56,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    66,    74,    90,
      87,    48,    49,    72,    73,    50,    94,    69,   111,    48,
      49,   113,    51,    50,   116,   121,    12,    52,    53,    91,
      51,   100,    35,    48,    49,    52,    53,    50,    65,   109,
      25,    48,    49,    75,    51,    50,   118,     0,     0,    52,
      53,     0,    51,     0,     0,    48,    49,    52,    53,    50,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       0,    52,    53,    96,     0,    78,    79,    80,    81,    82,
      83,    84,    85,    86,     0,     0,     0,    87,   112,    78,
      79,    80,    81,    82,    83,    84,    85,    86,     0,     0,
       0,    87,    78,    79,    80,    81,    82,    83,    84,    85,
      86,     0,     0,     0,    87,   117,    78,    79,    80,    81,
      82,    83,    84,    85,    86,     0,     0,    95,    87,    78,
      79,    80,    81,    82,    83,    84,    85,    86,     0,     0,
     114,    87,    78,    79,    80,    81,    82,    83,    84,    85,
      86,     0,     0,   115,    87,    78,    79,    80,    81,    82,
      83,    84,    85,    86,     0,     0,    78,    87,    80,    81,
      82,    83,    84,    85,     0,     0,     0,    78,    87,   -37,
      81,    82,    83,    84,     0,     0,     0,     0,    78,    87,
      80,    81,    82,    83,    84,     0,     0,     0,     0,     0,
      87
};

static const yytype_int8 yycheck[] =
{
      51,    52,    53,    54,     0,    27,     5,     9,    11,     5,
      61,   114,   115,     1,     7,    66,     0,     5,   121,    22,
      22,    72,    73,    19,     6,    20,    24,    26,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    25,    26,    21,
       1,    63,     3,     4,     5,    10,     7,     7,     8,    31,
      46,    24,     5,    14,     8,    24,     8,    22,    19,    20,
      29,   112,     7,    24,    25,    26,    27,    28,    21,    30,
       3,     4,    24,    26,     7,     3,    25,    21,    10,     9,
      23,    14,     8,    15,    16,     9,    19,    20,     8,     1,
      22,    24,     7,    22,    27,    28,     8,    30,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    20,    25,     1,
      22,     3,     4,    20,    20,     7,    21,     1,    21,     3,
       4,    21,    14,     7,    23,    29,     5,    19,    20,    21,
      14,     1,    21,     3,     4,    19,    20,     7,    46,     1,
      17,     3,     4,    59,    14,     7,   112,    -1,    -1,    19,
      20,    -1,    14,    -1,    -1,     3,     4,    19,    20,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,    -1,
      -1,    19,    20,     8,    -1,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    22,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    22,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    -1,    22,    23,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    -1,    -1,    21,    22,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    -1,    -1,
      21,    22,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    -1,    -1,    21,    22,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    -1,    -1,    10,    22,    12,    13,
      14,    15,    16,    17,    -1,    -1,    -1,    10,    22,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    -1,    10,    22,
      12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,    -1,
      22
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     5,    26,    33,    34,    35,    37,    38,     7,    39,
      40,     0,    34,     7,     8,    36,    41,    42,    24,    20,
       8,     9,    22,     8,    24,    45,     1,    37,    48,    49,
      21,    37,    43,    44,     7,    36,     3,    48,    41,    50,
      51,    50,    25,    48,    41,    21,     9,    23,     3,     4,
       7,    14,    19,    20,    27,    28,    30,    45,    46,    47,
      52,    11,     8,     9,     8,    43,    20,    52,    52,     1,
      52,    52,    20,    20,    25,    46,     1,     8,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    22,    52,    50,
       1,    21,    52,    53,    21,    21,     8,    52,    52,     7,
       1,    52,    52,    52,    52,    52,    52,    52,    52,     1,
      52,    21,     9,    21,    21,    21,    23,    23,    53,    47,
      47,    29,    47
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    32,    33,    34,    34,    35,    35,    35,    35,    36,
      36,    37,    37,    38,    38,    39,    39,    40,    41,    41,
      42,    42,    43,    43,    44,    45,    46,    46,    47,    47,
      47,    47,    47,    47,    47,    48,    48,    49,    49,    50,
      50,    51,    51,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    53,    53
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     3,     2,     3,     3,     1,
       3,     1,     1,     5,     2,     0,     1,     1,     1,     4,
       4,     3,     3,     1,     2,     4,     2,     0,     2,     1,
       3,     5,     7,     5,     2,     2,     0,     3,     3,     1,
       3,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     4,     3,     4,     3,     1,     1,
       1,     3,     4,     4,     3,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: ExtDefList  */
#line 47 "parse_ljx_mogai.y"
                    {
    yyval = upConstruct(yyvsp[0], "Program", (yylsp[0]).first_line, N_PROGRAM);
    myTree = yyval;
}
#line 1359 "parse_ljx_mogai.tab.c"
    break;

  case 3: /* ExtDefList: %empty  */
#line 52 "parse_ljx_mogai.y"
                   {
        //修改$$ = insertNode(NULL, "ExtDefList", yylineno, NONTERMINAL);
        yyval = upConstruct(NULL, "ExtDefList", yylineno, N_EXT_DEF_L);
        myTree = yyval;
    }
#line 1369 "parse_ljx_mogai.tab.c"
    break;

  case 4: /* ExtDefList: ExtDef ExtDefList  */
#line 57 "parse_ljx_mogai.y"
                      {
        yyval = upConstruct(yyvsp[-1], "ExtDefList", (yylsp[-1]).first_line, N_EXT_DEF_L);
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1379 "parse_ljx_mogai.tab.c"
    break;

  case 5: /* ExtDef: Specifier ExtDecList SEMI  */
#line 65 "parse_ljx_mogai.y"
                                  {
        yyval = upConstruct(yyvsp[-2], "ExtDef", (yylsp[-2]).first_line, N_EXT_DEF);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1390 "parse_ljx_mogai.tab.c"
    break;

  case 6: /* ExtDef: Specifier SEMI  */
#line 71 "parse_ljx_mogai.y"
                    {
        yyval = upConstruct(yyvsp[-1], "ExtDef", (yylsp[-1]).first_line, N_EXT_DEF);
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1400 "parse_ljx_mogai.tab.c"
    break;

  case 7: /* ExtDef: Specifier FunDec CompSt  */
#line 76 "parse_ljx_mogai.y"
                             {
        yyval = upConstruct(yyvsp[-2], "ExtDef", (yylsp[-2]).first_line, N_EXT_DEF);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1411 "parse_ljx_mogai.tab.c"
    break;

  case 8: /* ExtDef: Specifier FunDec SEMI  */
#line 82 "parse_ljx_mogai.y"
                           {
        yyval = upConstruct(yyvsp[-2], "ExtDef", (yylsp[-2]).first_line, N_EXT_DEF);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1422 "parse_ljx_mogai.tab.c"
    break;

  case 9: /* ExtDecList: VarDec  */
#line 90 "parse_ljx_mogai.y"
                   {
        yyval = upConstruct(yyvsp[0], "ExtDecList", (yylsp[0]).first_line, N_EXT_DEC_L);
        myTree = yyval;
    }
#line 1431 "parse_ljx_mogai.tab.c"
    break;

  case 10: /* ExtDecList: VarDec COMMA ExtDecList  */
#line 94 "parse_ljx_mogai.y"
                             {
        yyval = upConstruct(yyvsp[-2], "ExtDecList", (yylsp[-2]).first_line, N_EXT_DEC_L);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1442 "parse_ljx_mogai.tab.c"
    break;

  case 11: /* Specifier: TYPE  */
#line 112 "parse_ljx_mogai.y"
                {
        yyval = upConstruct(yyvsp[0], "Specifier", (yylsp[0]).first_line, N_SPECI);
        myTree = yyval;
    }
#line 1451 "parse_ljx_mogai.tab.c"
    break;

  case 12: /* Specifier: StructSpecifier  */
#line 116 "parse_ljx_mogai.y"
                     {
        yyval = upConstruct(yyvsp[0], "Specifier", (yylsp[0]).first_line, N_SPECI);
        myTree = yyval;
    }
#line 1460 "parse_ljx_mogai.tab.c"
    break;

  case 13: /* StructSpecifier: STRUCT OptTag LC DefList RC  */
#line 122 "parse_ljx_mogai.y"
                                             {
        yyval = upConstruct(yyvsp[-4], "StructSpecifier", (yylsp[-4]).first_line, N_STRUCT_SPECI);
        yyvsp[-4]->sibling = yyvsp[-3];
        yyvsp[-3]->sibling = yyvsp[-2];
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1473 "parse_ljx_mogai.tab.c"
    break;

  case 14: /* StructSpecifier: STRUCT Tag  */
#line 130 "parse_ljx_mogai.y"
                {
        yyval = upConstruct(yyvsp[-1], "StructSpecifier", (yylsp[-1]).first_line, N_STRUCT_SPECI);
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1483 "parse_ljx_mogai.tab.c"
    break;

  case 15: /* OptTag: %empty  */
#line 137 "parse_ljx_mogai.y"
                {
        yyval = upConstruct(NULL, "OptTag", yylineno, N_OPT_TAG);
        myTree = yyval;
    }
#line 1492 "parse_ljx_mogai.tab.c"
    break;

  case 16: /* OptTag: ID  */
#line 141 "parse_ljx_mogai.y"
       {
        yyval = upConstruct(yyvsp[0], "OptTag", (yylsp[0]).first_line, N_OPT_TAG);
        myTree = yyval;
    }
#line 1501 "parse_ljx_mogai.tab.c"
    break;

  case 17: /* Tag: ID  */
#line 148 "parse_ljx_mogai.y"
        {
        yyval = upConstruct(yyvsp[0], "Tag", (yylsp[0]).first_line, N_TAG);
        myTree = yyval;
}
#line 1510 "parse_ljx_mogai.tab.c"
    break;

  case 18: /* VarDec: ID  */
#line 155 "parse_ljx_mogai.y"
           {
        yyval = upConstruct(yyvsp[0], "VarDec", (yylsp[0]).first_line, N_VAR_DEC);
        myTree = yyval;
    }
#line 1519 "parse_ljx_mogai.tab.c"
    break;

  case 19: /* VarDec: VarDec LB INT RB  */
#line 159 "parse_ljx_mogai.y"
                      {
        yyval = upConstruct(yyvsp[-3], "VarDec", (yylsp[-3]).first_line, N_VAR_DEC);
        yyvsp[-3]->sibling = yyvsp[-2];
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1531 "parse_ljx_mogai.tab.c"
    break;

  case 20: /* FunDec: ID LP VarList RP  */
#line 168 "parse_ljx_mogai.y"
                         {
        yyval = upConstruct(yyvsp[-3], "FunDec", (yylsp[-3]).first_line, N_FUN_DEC);
        yyvsp[-3]->sibling = yyvsp[-2];
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1543 "parse_ljx_mogai.tab.c"
    break;

  case 21: /* FunDec: ID LP RP  */
#line 175 "parse_ljx_mogai.y"
              {
        yyval = upConstruct(yyvsp[-2], "FunDec", (yylsp[-2]).first_line, N_FUN_DEC);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1554 "parse_ljx_mogai.tab.c"
    break;

  case 22: /* VarList: ParamDec COMMA VarList  */
#line 183 "parse_ljx_mogai.y"
                                {
        yyval = upConstruct(yyvsp[-2], "VarList", (yylsp[-2]).first_line, N_VAR_L);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1565 "parse_ljx_mogai.tab.c"
    break;

  case 23: /* VarList: ParamDec  */
#line 189 "parse_ljx_mogai.y"
              {
        yyval = upConstruct(yyvsp[0], "VarList", (yylsp[0]).first_line, N_VAR_L);
        myTree = yyval;
    }
#line 1574 "parse_ljx_mogai.tab.c"
    break;

  case 24: /* ParamDec: Specifier VarDec  */
#line 195 "parse_ljx_mogai.y"
                           {
        yyval = upConstruct(yyvsp[-1], "ParamDec", (yylsp[-1]).first_line, N_PARAM_DEC);
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1584 "parse_ljx_mogai.tab.c"
    break;

  case 25: /* CompSt: LC DefList StmtList RC  */
#line 203 "parse_ljx_mogai.y"
                               {
        yyval = upConstruct(yyvsp[-3], "CompSt", (yylsp[-3]).first_line, N_COMPST);
        yyvsp[-3]->sibling = yyvsp[-2];
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
   }
#line 1596 "parse_ljx_mogai.tab.c"
    break;

  case 26: /* StmtList: Stmt StmtList  */
#line 212 "parse_ljx_mogai.y"
                        {
        yyval = upConstruct(yyvsp[-1], "StmtList", (yylsp[-1]).first_line, N_STMT_L);
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1606 "parse_ljx_mogai.tab.c"
    break;

  case 27: /* StmtList: %empty  */
#line 217 "parse_ljx_mogai.y"
            {
        yyval = upConstruct(NULL, "StmtList", yylineno, N_STMT_L);
        myTree = yyval;
    }
#line 1615 "parse_ljx_mogai.tab.c"
    break;

  case 28: /* Stmt: Exp SEMI  */
#line 223 "parse_ljx_mogai.y"
               {
        yyval = upConstruct(yyvsp[-1], "Stmt", (yylsp[-1]).first_line, N_STMT);
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1625 "parse_ljx_mogai.tab.c"
    break;

  case 29: /* Stmt: CompSt  */
#line 228 "parse_ljx_mogai.y"
            {
        yyval = upConstruct(yyvsp[0], "Stmt", (yylsp[0]).first_line, N_STMT);
        myTree = yyval;
    }
#line 1634 "parse_ljx_mogai.tab.c"
    break;

  case 30: /* Stmt: RETURN Exp SEMI  */
#line 232 "parse_ljx_mogai.y"
                     {
        yyval = upConstruct(yyvsp[-2], "Stmt", (yylsp[-2]).first_line, N_STMT);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1645 "parse_ljx_mogai.tab.c"
    break;

  case 31: /* Stmt: IF LP Exp RP Stmt  */
#line 238 "parse_ljx_mogai.y"
                       {
        yyval = upConstruct(yyvsp[-4], "Stmt", (yylsp[-4]).first_line, N_STMT);
        yyvsp[-4]->sibling = yyvsp[-3];
        yyvsp[-3]->sibling = yyvsp[-2];
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1658 "parse_ljx_mogai.tab.c"
    break;

  case 32: /* Stmt: IF LP Exp RP Stmt ELSE Stmt  */
#line 246 "parse_ljx_mogai.y"
                                 {
        yyval = upConstruct(yyvsp[-6], "Stmt", (yylsp[-6]).first_line, N_STMT);
        yyvsp[-6]->sibling = yyvsp[-5];
        yyvsp[-5]->sibling = yyvsp[-4];
        yyvsp[-4]->sibling = yyvsp[-3];
        yyvsp[-3]->sibling = yyvsp[-2];
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1673 "parse_ljx_mogai.tab.c"
    break;

  case 33: /* Stmt: WHILE LP Exp RP Stmt  */
#line 256 "parse_ljx_mogai.y"
                          {
        yyval = upConstruct(yyvsp[-4], "Stmt", (yylsp[-4]).first_line, N_STMT);
        yyvsp[-4]->sibling = yyvsp[-3];
        yyvsp[-3]->sibling = yyvsp[-2];
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1686 "parse_ljx_mogai.tab.c"
    break;

  case 34: /* Stmt: Exp error  */
#line 264 "parse_ljx_mogai.y"
               {
        if(line_check[(yylsp[0]).first_line-1]==0){
        line_check[(yylsp[0]).first_line-1]=1;
   }
        char msg[100];
        sprintf( msg, "Stmt Missing \";\"");
        myerror( msg );
    }
#line 1699 "parse_ljx_mogai.tab.c"
    break;

  case 35: /* DefList: Def DefList  */
#line 275 "parse_ljx_mogai.y"
                     {
        yyval = upConstruct(yyvsp[-1], "DefList", (yylsp[-1]).first_line, N_DEF_L);
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1709 "parse_ljx_mogai.tab.c"
    break;

  case 36: /* DefList: %empty  */
#line 280 "parse_ljx_mogai.y"
            {
        yyval = upConstruct(NULL, "DefList", yylineno, N_DEF_L);
        myTree = yyval;
    }
#line 1718 "parse_ljx_mogai.tab.c"
    break;

  case 37: /* Def: Specifier DecList SEMI  */
#line 286 "parse_ljx_mogai.y"
                            {
        yyval = upConstruct(yyvsp[-2], "Def", (yylsp[-2]).first_line, N_DEF);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
}
#line 1729 "parse_ljx_mogai.tab.c"
    break;

  case 38: /* Def: error DecList SEMI  */
#line 292 "parse_ljx_mogai.y"
                       {
        if(line_check[(yylsp[-2]).first_line-1]==0){
            line_check[(yylsp[-2]).first_line-1]=1;
        }
        char msg[100];
        sprintf( msg, "Def syntax error");
        myerror( msg );
}
#line 1742 "parse_ljx_mogai.tab.c"
    break;

  case 39: /* DecList: Dec  */
#line 303 "parse_ljx_mogai.y"
             {
        yyval = upConstruct(yyvsp[0], "DecList", (yylsp[0]).first_line, N_DEC_L);
        myTree = yyval;
    }
#line 1751 "parse_ljx_mogai.tab.c"
    break;

  case 40: /* DecList: Dec COMMA DecList  */
#line 307 "parse_ljx_mogai.y"
                       {
        yyval = upConstruct(yyvsp[-2], "DecList", (yylsp[-2]).first_line, N_DEC_L);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1762 "parse_ljx_mogai.tab.c"
    break;

  case 41: /* Dec: VarDec  */
#line 315 "parse_ljx_mogai.y"
            {
        yyval = upConstruct(yyvsp[0], "Dec", (yylsp[0]).first_line, N_DEC);
        myTree = yyval;
    }
#line 1771 "parse_ljx_mogai.tab.c"
    break;

  case 42: /* Dec: VarDec ASSIGNOP Exp  */
#line 319 "parse_ljx_mogai.y"
                         {
        yyval = upConstruct(yyvsp[-2], "Dec", (yylsp[-2]).first_line, N_DEC);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1782 "parse_ljx_mogai.tab.c"
    break;

  case 43: /* Exp: Exp ASSIGNOP Exp  */
#line 336 "parse_ljx_mogai.y"
                      {
        yyval = upConstruct(yyvsp[-2], "Exp", (yylsp[-2]).first_line, N_EXP);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1793 "parse_ljx_mogai.tab.c"
    break;

  case 44: /* Exp: Exp AND Exp  */
#line 342 "parse_ljx_mogai.y"
                 {
        yyval = upConstruct(yyvsp[-2], "Exp", (yylsp[-2]).first_line, N_EXP);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1804 "parse_ljx_mogai.tab.c"
    break;

  case 45: /* Exp: Exp OR Exp  */
#line 348 "parse_ljx_mogai.y"
                {
        yyval = upConstruct(yyvsp[-2], "Exp", (yylsp[-2]).first_line, N_EXP);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1815 "parse_ljx_mogai.tab.c"
    break;

  case 46: /* Exp: Exp RELOP Exp  */
#line 354 "parse_ljx_mogai.y"
                   {
        yyval = upConstruct(yyvsp[-2], "Exp", (yylsp[-2]).first_line, N_EXP);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1826 "parse_ljx_mogai.tab.c"
    break;

  case 47: /* Exp: Exp PLUS Exp  */
#line 360 "parse_ljx_mogai.y"
                  {
        yyval = upConstruct(yyvsp[-2], "Exp", (yylsp[-2]).first_line, N_EXP);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1837 "parse_ljx_mogai.tab.c"
    break;

  case 48: /* Exp: Exp MINUS Exp  */
#line 366 "parse_ljx_mogai.y"
                   {
        yyval = upConstruct(yyvsp[-2], "Exp", (yylsp[-2]).first_line, N_EXP);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1848 "parse_ljx_mogai.tab.c"
    break;

  case 49: /* Exp: Exp STAR Exp  */
#line 372 "parse_ljx_mogai.y"
                  {
        yyval = upConstruct(yyvsp[-2], "Exp", (yylsp[-2]).first_line, N_EXP);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1859 "parse_ljx_mogai.tab.c"
    break;

  case 50: /* Exp: Exp DIV Exp  */
#line 378 "parse_ljx_mogai.y"
                 {
        yyval = upConstruct(yyvsp[-2], "Exp", (yylsp[-2]).first_line, N_EXP);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1870 "parse_ljx_mogai.tab.c"
    break;

  case 51: /* Exp: LP Exp RP  */
#line 384 "parse_ljx_mogai.y"
               {
        yyval = upConstruct(yyvsp[-2], "Exp", (yylsp[-2]).first_line, N_EXP);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1881 "parse_ljx_mogai.tab.c"
    break;

  case 52: /* Exp: MINUS Exp  */
#line 390 "parse_ljx_mogai.y"
               {
        yyval = upConstruct(yyvsp[-1], "Exp", (yylsp[-1]).first_line, N_EXP);
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1891 "parse_ljx_mogai.tab.c"
    break;

  case 53: /* Exp: NOT Exp  */
#line 395 "parse_ljx_mogai.y"
             {
        yyval = upConstruct(yyvsp[-1], "Exp", (yylsp[-1]).first_line, N_EXP);
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1901 "parse_ljx_mogai.tab.c"
    break;

  case 54: /* Exp: ID LP Args RP  */
#line 400 "parse_ljx_mogai.y"
                   {
        yyval = upConstruct(yyvsp[-3], "Exp", (yylsp[-3]).first_line, N_EXP);
        yyvsp[-3]->sibling = yyvsp[-2];
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1913 "parse_ljx_mogai.tab.c"
    break;

  case 55: /* Exp: ID LP RP  */
#line 407 "parse_ljx_mogai.y"
              {
        yyval = upConstruct(yyvsp[-2], "Exp", (yylsp[-2]).first_line, N_EXP);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1924 "parse_ljx_mogai.tab.c"
    break;

  case 56: /* Exp: Exp LB Exp RB  */
#line 413 "parse_ljx_mogai.y"
                   {
        yyval = upConstruct(yyvsp[-3], "Exp", (yylsp[-3]).first_line, N_EXP);
        yyvsp[-3]->sibling = yyvsp[-2];
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1936 "parse_ljx_mogai.tab.c"
    break;

  case 57: /* Exp: Exp DOT ID  */
#line 420 "parse_ljx_mogai.y"
                {
        yyval = upConstruct(yyvsp[-2], "Exp", (yylsp[-2]).first_line, N_EXP);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 1947 "parse_ljx_mogai.tab.c"
    break;

  case 58: /* Exp: ID  */
#line 426 "parse_ljx_mogai.y"
        {
        yyval = upConstruct(yyvsp[0], "Exp", (yylsp[0]).first_line, N_EXP);
    }
#line 1955 "parse_ljx_mogai.tab.c"
    break;

  case 59: /* Exp: INT  */
#line 429 "parse_ljx_mogai.y"
         {
        yyval = upConstruct(yyvsp[0], "Exp", (yylsp[0]).first_line, N_EXP);
    }
#line 1963 "parse_ljx_mogai.tab.c"
    break;

  case 60: /* Exp: FLOAT  */
#line 432 "parse_ljx_mogai.y"
           {
        yyval = upConstruct(yyvsp[0], "Exp", (yylsp[0]).first_line, N_EXP);
    }
#line 1971 "parse_ljx_mogai.tab.c"
    break;

  case 61: /* Exp: LP error RP  */
#line 435 "parse_ljx_mogai.y"
                  {
        if(line_check[(yylsp[-1]).first_line-1]==0){
        line_check[(yylsp[-1]).first_line-1]=1;
   }
        char msg[100];
        sprintf( msg, "Exp Syntax error");
        myerror( msg );
    }
#line 1984 "parse_ljx_mogai.tab.c"
    break;

  case 62: /* Exp: ID LP error RP  */
#line 443 "parse_ljx_mogai.y"
                     {
        if(line_check[(yylsp[-2]).first_line-1]==0){
        line_check[(yylsp[-2]).first_line-1]=1;
   }
        char msg[100];
        sprintf( msg, "Exp Syntax error");
        myerror( msg );
    }
#line 1997 "parse_ljx_mogai.tab.c"
    break;

  case 63: /* Exp: Exp LB error RB  */
#line 451 "parse_ljx_mogai.y"
                      {
        if(line_check[(yylsp[-2]).first_line-1]==0){
        line_check[(yylsp[-2]).first_line-1]=1;
   }
        char msg[100];
        sprintf( msg, "Exp Missing \"]\"");
        myerror( msg );
    }
#line 2010 "parse_ljx_mogai.tab.c"
    break;

  case 64: /* Exp: Exp ASSIGNOP error  */
#line 459 "parse_ljx_mogai.y"
                        {
       if(line_check[(yylsp[-1]).first_line-1]==0){
        line_check[(yylsp[-1]).first_line-1]=1;
   }
        char msg[100];
        sprintf( msg, "Exp syntax error");
        myerror( msg );
    }
#line 2023 "parse_ljx_mogai.tab.c"
    break;

  case 65: /* Args: Exp COMMA Args  */
#line 470 "parse_ljx_mogai.y"
                     {
        yyval = upConstruct(yyvsp[-2], "Args", (yylsp[-2]).first_line, N_ARGS);
        yyvsp[-2]->sibling = yyvsp[-1];
        yyvsp[-1]->sibling = yyvsp[0];
        myTree = yyval;
    }
#line 2034 "parse_ljx_mogai.tab.c"
    break;

  case 66: /* Args: Exp  */
#line 476 "parse_ljx_mogai.y"
         {
        yyval = upConstruct(yyvsp[0], "Exp", (yylsp[0]).first_line, N_ARGS);
    }
#line 2042 "parse_ljx_mogai.tab.c"
    break;


#line 2046 "parse_ljx_mogai.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 481 "parse_ljx_mogai.y"


#include "lex.yy.c"

int main(int argc, char** argv)
{
	// if(argc <= 1) return 1;
	FILE* f = fopen("test1.cmm", "r");
	// if(!f){
		// perror(argv[1]);
		// return 1;
	// }
    /*yylineno=1??*/
	yyrestart(f);
	yyparse();
    printf("Built the tree successfully\n");
    
    if(error_count == 0){
        tree_analys(myTree);
    }
        
        
	return 0;
}

void yyerror(char *msg){
    //
}

void myerror(char *msg){

    fprintf(stderr, "Error type B at Line %d: %s \n", yylineno,  msg);
    error_count++;
}
