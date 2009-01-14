/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-    */
/* ex: set filetype=cpp softtabstop=4 shiftwidth=4 tabstop=4 cindent expandtab: */

/*
  $Id: cmnRandomSequence.h 20 2009-01-08 16:09:57Z adeguet1 $
  
  Author(s):	Ofri Sadowsky
  Created on:	2003-06-09

  (C) Copyright 2003-2007 Johns Hopkins University (JHU), All Rights
  Reserved.

--- begin cisst license - do not edit ---

This software is provided "as is" under an open source license, with
no warranty.  The complete license can be found in license.txt and
http://www.cisst.org/cisst/license.txt.

--- end cisst license ---

*/


/*!
  \file cmnRandomSequence.h
  \brief Randomization utilities for general use in CISST applications
*/

#ifndef _cmnRandomSequence_h
#define _cmnRandomSequence_h

#include <cisstCommon/cmnPortability.h>

#include <stdlib.h>

#include <cisstCommon/cmnExport.h>

/*!
   \brief Provide an interface to a reproducible random sequence.

   \ingroup cisstCommon

   Class cmnRandomSequence provides a reproducible random sequence.  A random sequence 
   is defined as a sequence of elements of type ElementaryRandomNumber, which is 
   generated by a more or less good random number generator.  The random number 
   generator is initialized with a seed, and outputs a sequence of numbers.
   
   In order to reproduce a random sequence, we need to store the seed and the position
   in the sequence.  We can then resume to any position in the sequence using the 
   method SetSequencePosition().
       
   Important note:  In the current implementation, we are using the rand() function
   of the standard C/C++ library.  However, as the rand() function does not have a 
   cross-platform standardized behavior, our random sequence is only reproducible 
   within the scope of one compiler on one machine.  In addition, as we depend on a
   global mechanism, all the random sequences use the same resource.  This means that
   in fact, there is only one random sequence in the system.  Therefore, the current
   implementation is as a Singleton.
         
   Useful background on randomization can be found, for example, in Numerical Recipes:
   http://www.nr.com . Due to copyright issues, we are currently delaying its use.
   Another possible source for randomization functions is in the Gnu Scientific
   Library (GSL): http://www.gnu.org/software/gsl/gsl.html .
*/
class CISST_EXPORT cmnRandomSequence
{
public:
    /*! type of the randomization seed
    */
    typedef unsigned int SeedType;
    
    /*! type of the elementary random number that's generated by cmnRandomSequence
    */
    typedef int ElementaryRandomNumber;
    
    /*! type of counter of the number of elements that were extracted from the sequence
    */
    typedef unsigned long SequenceCounterType;
    
    /*! The default randomization seed.
    */
    static const SeedType DefaultSeed;
    
    /*! the lower bound for a value of an elementary random number
    */
    static const ElementaryRandomNumber LowerRandomBound;
    
    /*! the upper bound for a value of an elementary random number
    */
    static const ElementaryRandomNumber UpperRandomBound;
    
protected:
    /*! 
       Default constructor -- initialize the randomization seed to the default seed
       and start at the first element of the random sequence.
    */
    cmnRandomSequence()
    {
        SetSeed(DefaultSeed);
    }
    
    /*! 
       Parametrized constructor -- initialize the randomization seed by a parameter
       and start at the specified position (zero based) of the random sequence
    */
    cmnRandomSequence(const SeedType seed, const SequenceCounterType position = 0)
    {
        SetSeed(seed);
        SetSequencePosition(position);
    }
    
    /*!
       Copy constructor -- initialize the seed and the sequence position identically
       to the other object
    */
    cmnRandomSequence(const cmnRandomSequence & other)
    {
        SetSeed(other.GetSeed());
        SetSequencePosition(other.GetSequencePosition());
    }
    
    /*!
       Assignment operator -- see copy constructor
    */
    cmnRandomSequence & operator=(const cmnRandomSequence & other)
    {
        SetSeed(other.GetSeed());
        SetSequencePosition(other.GetSequencePosition());
		return *this;
    }
    
    /*!
       The Singleton instance
    */
    static cmnRandomSequence RandomInstance;
    
public:
    /*!
       Access the Singleton instance
    */
    static cmnRandomSequence & GetInstance()
    {
        return RandomInstance;
    }
    
    /*!
       Return the randomization seed for this object
    */
    SeedType GetSeed() const
    {
        return Seed;
    }
    
    /*!
       Set the randomization seed to the given value, and move to sequence position 0.
    */
    void SetSeed(SeedType seed)
    {
        Seed = seed;
        SequencePosition = 0;
        srand(Seed);
    }
    
    /*!
       Return the number of random sequence elements extracted so far.
    */
    SequenceCounterType GetSequencePosition() const
    {
        return SequencePosition;
    }
    
    /*!
       Move to the specified position (zero based) in the random sequence starting
       with the current seed.
    */
    void SetSequencePosition(SequenceCounterType position)
    {
        SetSeed(GetSeed());
        while ( GetSequencePosition() < position )
            ExtractRandomElement();
    }
    
    
    /*! Extract a single random element from the sequence. This is the basic operation
       on the random sequence.
    */
    ElementaryRandomNumber ExtractRandomElement()
    {
        ++SequencePosition;
        return rand();
    }

    //@{
    /*! Extract a random value for a templated type.  These methods are specialized
      by the type argument so that they can be used generically, e.g., in sequence
      testing. */
    template<typename _valueType>
    void ExtractRandomValue(_valueType & result);

    template<typename _valueType>
    inline void ExtractRandomValue(const _valueType min, const _valueType max, _valueType & result);

    template<typename _valueType>
    inline void ExtractRandomValueArray(const _valueType min, const _valueType max, _valueType * array,
        const unsigned int arraySize);
    //@}

    /*! Return a random floating point number in the range (0..1) */
    float ExtractRandomFloat()
    {
      ElementaryRandomNumber randomNumber = ExtractRandomElement();
      float myPosition = static_cast<float>(randomNumber) - static_cast<float>(LowerRandomBound);
      float randomFloat = myPosition / (static_cast<float>(UpperRandomBound) - static_cast<float>(LowerRandomBound) + 1);
      return randomFloat;
    }

    /*! Return a random floating point number in the range [min..max] */
    float ExtractRandomFloat(const float min, const float max)
    {
        const float randomFloat = ExtractRandomFloat();
        return (randomFloat * (max - min)) + min;
    }

    void ExtractRandomFloatArray(const float min, const float max, float *array, 
        const unsigned int arraySize)
    {
        for (unsigned int i = 0; i < arraySize; ++i, ++array)
            *array = ExtractRandomFloat(min, max);
    }

    /*! Return a random double-precision floating point number in the range (0..1) */
    double ExtractRandomDouble()
    {
		return (static_cast<double>(ExtractRandomElement()) - static_cast<double>(LowerRandomBound)) / (static_cast<double>(UpperRandomBound) - static_cast<double>(LowerRandomBound) + 1);
    }

    /*! Return a random double-precision floating point number in the range [min..max] */
    double ExtractRandomDouble(const double min, const double max)
    {
        const double randomDouble = ExtractRandomDouble();
        return (randomDouble * (max - min)) + min;
    }

    void ExtractRandomDoubleArray(const double min, const double max, double *array, 
        const unsigned int arraySize)
    {
        for (unsigned int i = 0; i < arraySize; ++i, ++array)
            *array = ExtractRandomDouble(min, max);
    }

    int ExtractRandomInt()
    {
        return ExtractRandomElement();
    }

    int ExtractRandomInt(const int min, const int max)
    {
        const float randomFloat = ExtractRandomFloat();
        return static_cast<int>((randomFloat * (max - min)) + min);
    }

    void ExtractRandomIntArray(const int min, const int max, int *array, 
                               const unsigned int arraySize)
    {
        for (unsigned int i = 0; i < arraySize; ++i, ++array)
            *array = ExtractRandomInt(min, max);
    }

    unsigned int ExtractRandomUnsignedInt()
    {
        int randomInt = ExtractRandomElement();
        return static_cast<unsigned int>(randomInt);
    }

    unsigned int ExtractRandomUnsignedInt(const unsigned int min, const unsigned int max)
    {
        const float randomFloat = ExtractRandomFloat();
        return static_cast<unsigned int>((randomFloat * (max - min)) + min);
    }

    void ExtractRandomUnsignedIntArray(const unsigned int min, const unsigned int max, unsigned int *array, 
                                        const unsigned int arraySize)
    {
        for (unsigned int i = 0; i < arraySize; ++i, ++array)
            *array = ExtractRandomUnsignedInt(min, max);
    }

    short ExtractRandomShort()
    {
        return static_cast<short>(ExtractRandomElement()&0xffff);
    }

    short ExtractRandomShort(const short min, const short max)
    {
        const float randomFloat = ExtractRandomFloat();
        return static_cast<short>((randomFloat * (max - min)) + min);
    }

    void ExtractRandomShortArray(const short min, const short max, short *array, 
                                 const unsigned int arraySize)
    {
        for (unsigned int i = 0; i < arraySize; ++i, ++array)
            *array = ExtractRandomShort(min, max);
    }

    unsigned short ExtractRandomUnsignedShort()
    {
        int randomInt = ExtractRandomElement();
        return static_cast<unsigned short>((0xff) & randomInt);
    }

    unsigned short ExtractRandomUnsignedShort(const unsigned short min, const unsigned short max)
    {
        const float randomFloat = ExtractRandomFloat();
        return static_cast<unsigned short>((randomFloat * (max - min)) + min);
    }

    void ExtractRandomUnsignedShortArray(const unsigned short min, const unsigned short max, unsigned short *array, 
                                         const unsigned int arraySize)
    {
        for (unsigned int i = 0; i < arraySize; ++i, ++array)
            *array = ExtractRandomUnsignedShort(min, max);
    }

    long ExtractRandomLong()
    {
        return static_cast<long>(ExtractRandomElement());
    }

    long ExtractRandomLong(const long min, const long max)
    {
        const float randomFloat = ExtractRandomFloat();
        return static_cast<long>((randomFloat * (max - min)) + min);
    }

    void ExtractRandomLongArray(const long min, const long max, long *array, 
                               const unsigned int arraySize)
    {
        for (unsigned int i = 0; i < arraySize; ++i, ++array)
            *array = ExtractRandomLong(min, max);
    }

    char ExtractRandomChar()
    {
        int randomInt = ExtractRandomElement();
        return static_cast<char>((0xff) & randomInt);
    }

    char ExtractRandomChar(const char min, const char max)
    {
        const float randomFloat = ExtractRandomFloat();
        return static_cast<char>((randomFloat * (max - min)) + min);
    }

    void ExtractRandomCharArray(const char min, const char max, char *array, 
                                const unsigned int arraySize)
    {
        for (unsigned int i = 0; i < arraySize; ++i, ++array)
            *array = ExtractRandomChar(min, max);
    }

    unsigned char ExtractRandomUnsignedChar()
    {
        int randomInt = ExtractRandomElement();
        return static_cast<unsigned char>((0xff) & randomInt);
    }

    unsigned char ExtractRandomUnsignedChar(const unsigned char min, const unsigned char max)
    {
        const float randomFloat = ExtractRandomFloat();
        return static_cast<unsigned char>((randomFloat * (max - min)) + min);
    }

    void ExtractRandomUnsignedCharArray(const unsigned char min, const unsigned char max, unsigned char *array, 
                                        const unsigned int arraySize)
    {
        for (unsigned int i = 0; i < arraySize; ++i, ++array)
            *array = ExtractRandomUnsignedChar(min, max);
    }

    /*! Fill the given array with a random permutation of the numbers 0..length */
    void ExtractRandomPermutation(const unsigned int length, unsigned int *array)
    {
        unsigned int i;
        for (i = 0; i < length; ++i)
            array[i] = ExtractRandomInt(0, length);

        unsigned int next;
        unsigned int tmp;
        for (i = 0; i < length; ++i) {
            next = ExtractRandomInt(i, length);
            tmp = array[i];
            array[i] = array[next];
            array[next] = tmp;
        }
    }


private:
    /*! The randomization seed to start the random sequence
    */
    SeedType Seed;
    
    /*! The number of elementary random numbers extracted from the sequence so far
    */
    SequenceCounterType SequencePosition;
    
};


/* Specializations of cmnRandomSequence::ExtractRandomValue */
#ifndef DOXYGEN
/* --- float --- */
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<float>(float & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<float>(const float min, const float max, float & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValueArray<float>(const float min, const float max, float * array,
                                                       const unsigned int arraySize);

/* --- double --- */
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<double>(double & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<double>(const double min, const double max, double & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValueArray<double>(const double min, const double max, double * array,
                                                        const unsigned int arraySize);

/* --- int --- */
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<int>(int & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<int>(const int min, const int max, int & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValueArray<int>(const int min, const int max, int * array,
                                                     const unsigned int arraySize);
/* --- unsigned int --- */
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<unsigned int>(unsigned int & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<unsigned int>(const unsigned int min, const unsigned int max, unsigned int & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValueArray<unsigned int>(const unsigned int min, const unsigned int max, unsigned int * array,
                                                              const unsigned int arraySize);

/* --- short --- */
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<short>(short & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<short>(const short min, const short max, short & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValueArray<short>(const short min, const short max, short * array,
                                                       const unsigned int arraySize);
/* --- unsigned short --- */
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<unsigned short>(unsigned short & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<unsigned short>(const unsigned short min, const unsigned short max, unsigned short & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValueArray<unsigned short>(const unsigned short min, const unsigned short max, unsigned short * array,
                                                                const unsigned int arraySize);

/* --- long --- */
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<long>(long & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<long>(const long min, const long max, long & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValueArray<long>(const long min, const long max, long * array,
                                                     const unsigned int arraySize);

/* --- char --- */
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<char>(char & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<char>(const char min, const char max, char & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValueArray<char>(const char min, const char max, char * array,
                                                      const unsigned int arraySize);

/* --- unsigned char --- */
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<unsigned char>(unsigned char & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<unsigned char>(const unsigned char min, const unsigned char max, unsigned char & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValueArray<unsigned char>(const unsigned char min, const unsigned char max, unsigned char * array,
                                                               const unsigned int arraySize);

/* --- bool --- */
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<bool>(bool & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValue<bool>(const bool min, const bool max, bool & result);
CISST_DECLARE_TEMPLATE_FUNCTION_SPECIALIZATION
void cmnRandomSequence::ExtractRandomValueArray<bool>(const bool min, const bool max, bool * array,
                                                               const unsigned int arraySize);


/* --- float ---*/
CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(float & result)
{
    result = ExtractRandomFloat();
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(const float min, const float max, float & result)
{
    result = ExtractRandomFloat(min, max);
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValueArray(const float min, const float max, float * array,
                                                       const unsigned int arraySize)
{
    ExtractRandomFloatArray(min, max, array, arraySize);
}


/* --- double ---*/
CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(double & result)
{
    result = ExtractRandomDouble();
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(const double min, const double max, double & result)
{
    result = ExtractRandomDouble(min, max);
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValueArray(const double min, const double max, double * array,
                                                       const unsigned int arraySize)
{
    ExtractRandomDoubleArray(min, max, array, arraySize);
}


/* --- int ---*/
CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(int & result)
{
    result = ExtractRandomInt();
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(const int min, const int max, int & result)
{
    result = ExtractRandomInt(min, max);
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValueArray(const int min, const int max, int * array,
                                                       const unsigned int arraySize)
{
    ExtractRandomIntArray(min, max, array, arraySize);
}


/* --- unsigned int ---*/
CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(unsigned int & result)
{
    result = ExtractRandomUnsignedInt();
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(const unsigned int min, const unsigned int max, unsigned int & result)
{
    result = ExtractRandomUnsignedInt(min, max);
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValueArray(const unsigned int min, const unsigned int max, unsigned int * array,
                                                       const unsigned int arraySize)
{
    ExtractRandomUnsignedIntArray(min, max, array, arraySize);
}


/* --- long ---*/
CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(long & result)
{
    result = ExtractRandomLong();
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(const long min, const long max, long & result)
{
    result = ExtractRandomLong(min, max);
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValueArray(const long min, const long max, long * array,
                                                       const unsigned int arraySize)
{
    ExtractRandomLongArray(min, max, array, arraySize);
}

/* --- short ---*/
CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(short & result)
{
    result = ExtractRandomShort();
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(const short min, const short max, short & result)
{
    result = ExtractRandomShort(min, max);
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValueArray(const short min, const short max, short * array,
                                                       const unsigned int arraySize)
{
    ExtractRandomShortArray(min, max, array, arraySize);
}

/* --- unsigned short ---*/
CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(unsigned short & result)
{
    result = ExtractRandomUnsignedShort();
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(const unsigned short min, const unsigned short max, unsigned short & result)
{
    result = ExtractRandomUnsignedShort(min, max);
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValueArray(const unsigned short min, const unsigned short max, unsigned short * array,
                                                       const unsigned int arraySize)
{
    ExtractRandomUnsignedShortArray(min, max, array, arraySize);
}

/* --- char --- */
CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(char & result)
{
    result = ExtractRandomChar();
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(const char min, const char max, char & result)
{
    result = ExtractRandomChar(min, max);
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValueArray(const char min, const char max, char * array,
                                                       const unsigned int arraySize)
{
    ExtractRandomCharArray(min, max, array, arraySize);
}


/* --- unsigned char --- */
CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(unsigned char & result)
{
    result = ExtractRandomUnsignedChar();
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(const unsigned char min, const unsigned char max, unsigned char & result)
{
    result = ExtractRandomUnsignedChar(min, max);
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValueArray(const unsigned char min, const unsigned char max, unsigned char * array,
                                                       const unsigned int arraySize)
{
    ExtractRandomUnsignedCharArray(min, max, array, arraySize);
}


/* --- bool --- */
CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(bool & result)
{
    int value = ExtractRandomInt(0, 1024);
    result = value & 1;
}

CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValue(const bool min, const bool max, bool & result)
{
    if (min == max) {
        result = min;
    } else {
        ExtractRandomValue(result);
    }
}


CISST_DEFINE_TEMPLATE_FUNCTION_SPECIALIZATION
inline void cmnRandomSequence::ExtractRandomValueArray(const bool min, const bool max, bool * array,
                                                       const unsigned int arraySize)
{
    for (unsigned int i = 0; i < arraySize; ++i, ++array) {
        ExtractRandomValue(min, max, *array);
    }
}


#endif // DOXYGEN


#endif  // _cmnRandomSequence_h

