/***************************************************************************************************
 **
 **  Copyright (C) 2013 Ingo Berg
 **
 **  Permission is hereby granted, free of charge, to any person obtaining a copy of this
 **  software and associated documentation files (the "Software"), to deal in the Software
 **  without restriction, including without limitation the rights to use, copy, modify,
 **  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 **  permit persons to whom the Software is furnished to do so, subject to the following conditions:
 **
 **  The above copyright notice and this permission notice shall be included in all copies or
 **  substantial portions of the Software.
 **
 **  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 **  NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 **  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 **  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 **  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **
 ******************************************************************************************************/

#include "qmuparser.h"

#include <QCoreApplication>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QtGlobal>
#include <sstream>
#include <string>

#include "qmuparserdef.h"
#include "qmuparsererror.h"
#include "../vmisc/vmath.h"

/**
 * @file
 * @brief Implementation of the standard floating point QmuParser.
 */

/**
 * @brief Namespace for mathematical applications.
 */
namespace qmu
{
//---------------------------------------------------------------------------------------------------------------------
// Trigonometric function
qreal QmuParser::DegreeToRadian(qreal deg)
{
     return qDegreesToRadians(deg);
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::RadianToDegree(qreal rad)
{
     return qRadiansToDegrees(rad);
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::Sinh(qreal v)
{
    return sinh(v);
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::ASinh(qreal v)
{
    return log(v + qSqrt(v * v + 1));
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::Cosh(qreal v)
{
    return cosh(v);
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::ACosh(qreal v)
{
    return log(v + qSqrt(v * v - 1));
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::Tanh(qreal v)
{
    return tanh(v);
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::ATanh(qreal v)
{
    return (0.5 * log((1 + v) / (1 - v)));
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::SinD(qreal v)
{
    return qSin(qDegreesToRadians(v));
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::ASinD(qreal v)
{
    return qRadiansToDegrees(qAsin(v));
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::CosD(qreal v)
{
    return qCos(qDegreesToRadians(v));
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::ACosD(qreal v)
{
    return qRadiansToDegrees(qAcos(v));
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::TanD(qreal v)
{
    return qTan(qDegreesToRadians(v));
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::ATanD(qreal v)
{
    return qRadiansToDegrees(qAtan(v));
}

//---------------------------------------------------------------------------------------------------------------------
// Logarithm functions

//---------------------------------------------------------------------------------------------------------------------
// Logarithm base 2
qreal QmuParser::Log2(qreal v)
{
#ifdef MUP_MATH_EXCEPTIONS
    if (v<=0)
    {
        throw QmuParserError(ecDOMAIN_ERROR, "Log2");
    }
#endif
    return log(v)/log(2.0);
}

//---------------------------------------------------------------------------------------------------------------------
// Logarithm base 10
qreal QmuParser::Log10(qreal v)
{
#ifdef MUP_MATH_EXCEPTIONS
    if (v<=0)
    {
        throw QmuParserError(ecDOMAIN_ERROR, "Log10");
    }
#endif
    return log10(v);
}

//---------------------------------------------------------------------------------------------------------------------
//  misc
qreal QmuParser::Abs(qreal v)
{
    return qAbs(v);
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::Rint(qreal v)
{
    return qFloor(v + 0.5);
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::Sign(qreal v)
{
    return ((v<0) ? -1 : (v>0) ? 1 : 0);
}

//---------------------------------------------------------------------------------------------------------------------
qreal QmuParser::FMod(qreal number, qreal denom)
{
    return fmod(number, denom);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Callback for adding multiple values.
 * @param [in] a_afArg Vector with the function arguments
 * @param [in] a_iArgc The size of a_afArg
 */
qreal QmuParser::Sum(const qreal *a_afArg, int a_iArgc)
{
    if (a_iArgc == 0)
    {
        throw QmuParserError(QCoreApplication::translate("QmuParser", "too few arguments for function sum.",
                                                         "parser error message"));
    }
    qreal fRes=0;
    for (int i=0; i<a_iArgc; ++i)
    {
        fRes += a_afArg[i];
    }
    return fRes;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Callback for averaging multiple values.
 * @param [in] a_afArg Vector with the function arguments
 * @param [in] a_iArgc The size of a_afArg
 */
qreal QmuParser::Avg(const qreal *a_afArg, int a_iArgc)
{
    if (a_iArgc == 0)
    {
        throw QmuParserError(QCoreApplication::translate("QmuParser", "too few arguments for function sum.",
                                                         "parser error message"));
    }
    qreal fRes=0;
    for (int i=0; i<a_iArgc; ++i)
    {
        fRes += a_afArg[i];
    }
    return fRes/static_cast<qreal>(a_iArgc);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Callback for determining the minimum value out of a vector.
 * @param [in] a_afArg Vector with the function arguments
 * @param [in] a_iArgc The size of a_afArg
 */
qreal QmuParser::Min(const qreal *a_afArg, int a_iArgc)
{
    if (a_iArgc == 0)
    {
        throw QmuParserError(QCoreApplication::translate("QmuParser", "too few arguments for function min.",
                                                         "parser error message"));
    }
    qreal fRes=a_afArg[0];
    for (int i=0; i<a_iArgc; ++i)
    {
        fRes = qMin(fRes, a_afArg[i]);
    }
    return fRes;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Callback for determining the maximum value out of a vector.
 * @param [in] a_afArg Vector with the function arguments
 * @param [in] a_iArgc The size of a_afArg
 */
qreal QmuParser::Max(const qreal *a_afArg, int a_iArgc)
{
    if (a_iArgc == 0)
    {
        throw QmuParserError(QCoreApplication::translate("QmuParser", "too few arguments for function min.",
                                                         "parser error message"));
    }
    qreal fRes=a_afArg[0];
    for (int i=0; i<a_iArgc; ++i)
    {
        fRes = qMax(fRes, a_afArg[i]);
    }
    return fRes;
}

//---------------------------------------------------------------------------------------------------------------------
/**
* @brief Default value recognition callback.
* @param [in] a_szExpr Pointer to the expression
* @param [in, out] a_iPos Pointer to an index storing the current position within the expression
* @param [out] a_fVal Pointer where the value should be stored in case one is found.
* @return 1 if a value was found 0 otherwise.
*/
int QmuParser::IsVal(const QString &a_szExpr, int *a_iPos, qreal *a_fVal, const QLocale &locale, const QChar &decimal,
                     const QChar &thousand)
{
    qreal fVal(0);

    const int pos = ReadVal(a_szExpr, fVal, locale, decimal, thousand);

    if (pos == -1)
    {
        return 0;
    }

    *a_iPos += pos;
    *a_fVal = fVal;
    return 1;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Constructor.
 *
 * Call QmuParserBase class constructor and trigger Function, Operator and Constant initialization.
 */
QmuParser::QmuParser():QmuParserBase()
{
    AddValIdent(IsVal);

    InitCharSets();
    InitFun();
    InitConst();
    InitOprt();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Define the character sets.
 * @sa DefineNameChars, DefineOprtChars, DefineInfixOprtChars
 *
 * This function is used for initializing the default character sets that define
 * the characters to be useable in function and variable names and operators.
 */
void QmuParser::InitCharSets()
{
    DefineNameChars( QStringLiteral("0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") );
    DefineOprtChars( QStringLiteral("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-*^/?<>=#!$%&|~'_{}") );
    DefineInfixOprtChars( QStringLiteral("/+-*^?<>=#!$%&|~'_") );
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Initialize the default functions.
 */
void QmuParser::InitFun()
{
    // trigonometric helper functions
    DefineFun("degTorad",   DegreeToRadian);
    DefineFun("radTodeg",   RadianToDegree);

    // trigonometric functions
    DefineFun("sin",   qSin);
    DefineFun("cos",   qCos);
    DefineFun("tan",   qTan);
    DefineFun("sinD",   SinD);
    DefineFun("cosD",   CosD);
    DefineFun("tanD",   TanD);
    // inverse functions
    DefineFun("asin",  qAsin);
    DefineFun("acos",  qAcos);
    DefineFun("atan",  qAtan);
    DefineFun("atan2", qAtan2);
    DefineFun("asinD",  ASinD);
    DefineFun("acosD",  ACosD);
    DefineFun("atanD",  ATanD);
    // hyperbolic functions
    DefineFun("sinh",  Sinh);
    DefineFun("cosh",  Cosh);
    DefineFun("tanh",  Tanh);
    // inverse hyperbolic functions
    DefineFun("asinh", ASinh);
    DefineFun("acosh", ACosh);
    DefineFun("atanh", ATanh);
    // Logarithm functions
    DefineFun("log2",  Log2);
    DefineFun("log10", Log10);
    DefineFun("log",   Log10);
    DefineFun("ln",    qLn);
    // misc
    DefineFun("exp",   qExp);
    DefineFun("sqrt",  qSqrt);
    DefineFun("sign",  Sign);
    DefineFun("rint",  Rint);
    DefineFun("abs",   Abs);
    DefineFun("fmod",  FMod);
    // Functions with variable number of arguments
    DefineFun("sum",   Sum);
    DefineFun("avg",   Avg);
    DefineFun("min",   Min);
    DefineFun("max",   Max);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Initialize constants.
 *
 * By default the QmuParser recognizes two constants. Pi ("pi") and the eulerian
 * number ("_e").
 */
void QmuParser::InitConst()
{
    DefineConst("_pi", static_cast<qreal>(M_PI));
    DefineConst("_e", static_cast<qreal>(M_E));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Initialize operators.
 *
 * By default only the unary minus operator is added.
 */
void QmuParser::InitOprt()
{
    DefineInfixOprt(m_locale.negativeSign(), UnaryMinus);
}

//---------------------------------------------------------------------------------------------------------------------
void QmuParser::OnDetectVar(const QString &pExpr, int &nStart, int &nEnd)
{
    Q_UNUSED(pExpr)
    Q_UNUSED(nStart)
    Q_UNUSED(nEnd)
    // this is just sample code to illustrate modifying variable names on the fly.
    // I'm not sure anyone really needs such a feature...
    /*


    string sVar(pExpr->begin()+nStart, pExpr->begin()+nEnd);
    string sRepl = std::string("_") + sVar + "_";

    int nOrigVarEnd = nEnd;
    cout << "variable detected!\n";
    cout << "  Expr: " << *pExpr << "\n";
    cout << "  Start: " << nStart << "\n";
    cout << "  End: " << nEnd << "\n";
    cout << "  Var: \"" << sVar << "\"\n";
    cout << "  Repl: \"" << sRepl << "\"\n";
    nEnd = nStart + sRepl.length();
    cout << "  End: " << nEnd << "\n";
    pExpr->replace(pExpr->begin()+nStart, pExpr->begin()+nOrigVarEnd, sRepl);
    cout << "  New expr: " << *pExpr << "\n";
    */
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Numerically differentiate with regard to a variable.
 * @param [in] a_Var Pointer to the differentiation variable.
 * @param [in] a_fPos Position at which the differentiation should take place.
 * @param [in] a_fEpsilon Epsilon used for the numerical differentiation.
 *
 * Numerical differentiation uses a 5 point operator yielding a 4th order
 * formula. The default value for epsilon is 0.00074 which is
 * numeric_limits<double>::epsilon() ^ (1/5) as suggested in the muQmuParser
 * forum:
 *
 * http://sourceforge.net/forum/forum.php?thread_id=1994611&forum_id=462843
 */
// cppcheck-suppress unusedFunction
qreal QmuParser::Diff(qreal *a_Var, qreal  a_fPos, qreal  a_fEpsilon) const
{
    qreal fRes(0),
          fBuf(*a_Var),
          f[4] = {0, 0, 0, 0},
          fEpsilon(a_fEpsilon);

    // Backwards compatible calculation of epsilon inc case the user doesnt provide
    // his own epsilon
    if (qFuzzyIsNull(fEpsilon))
    {
        fEpsilon = qFuzzyIsNull(a_fPos) ? static_cast<qreal>(1e-10) : static_cast<qreal>(1e-7) * a_fPos;
    }

    *a_Var = a_fPos+2 * fEpsilon;  f[0] = Eval();
    *a_Var = a_fPos+1 * fEpsilon;  f[1] = Eval();
    *a_Var = a_fPos-1 * fEpsilon;  f[2] = Eval();
    *a_Var = a_fPos-2 * fEpsilon;  f[3] = Eval();
    *a_Var = fBuf; // restore variable

    fRes = (-f[0] + 8*f[1] - 8*f[2] + f[3]) / (12*fEpsilon);
    return fRes;
}
} // namespace qmu
