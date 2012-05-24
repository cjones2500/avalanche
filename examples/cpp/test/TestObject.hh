#ifndef __TestObject__
#define __TestObject__

#include "TObject.h"

//see io/io/src/TStreamerInfoWriteBuffer.cxx

class TestObject: public TObject
{
    public:
        TestObject(): TObject(),
            aBool(true),
            aChar(0xf8),
            aShort(0xf00d),
            aInt(0xc0c0ac0d),
            aLong(0x18c0ffee),
            aLong64(0x8baddeafdeadbeef),
            aFloat(123.456E-7),
            aDouble(987.654E-321),
            pChar({Char_t(0xf8)}),
            pShort({Short_t(0xf00d)}),
            pInt({Int_t(0xc0c0ac0d)}),
            pLong({Long_t(0x18c0ffee)}),
            pLong64({Long64_t(0x8baddeafdeadbeef)}),
            pFloat({Float_t(123.456E-7)}),
            pDouble({Double_t(987.654E-321)})
        {}
        /**
         feeding the ROOT hydra
         1. virtual destructor
         2. copy constructor
         3. assignment operator
         4. ClassDef(.., n) where n > 1
         5. LinkDef.h file (both makecint and cint)
         6. append makecint for ".L ..++"
         make 3 virtual if the destructor can not be virtual
         add any virtual method if neither 1 nor 3 can be virtual
         */
        virtual ~TestObject() {}
        //default copy constructor is ok
        //default copy assignment operator is ok

    public:
        Bool_t aBool;
        Char_t aChar;
        Short_t aShort;
        Int_t aInt;
        Long_t aLong;
        Long64_t aLong64;
        Float_t aFloat;
        Double_t aDouble;
        Char_t pChar[1];
        Short_t pShort[1];
        Int_t pInt[1];
        Long_t pLong[1];
        Long64_t pLong64[1];
        Float_t pFloat[1];
        Double_t pDouble[1];
        
    ClassDef(TestObject, 2);
};
#endif

#ifdef __MAKECINT__
        #pragma link C++ nestedclass;
        #pragma link C++ class TestObject+;
#endif

