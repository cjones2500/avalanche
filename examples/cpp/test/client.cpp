#include <iostream>
#include <cassert>
#include <avalanche.hpp>

#include "TestObject.hh"
#include "TObject.h"
#include "TClass.h"

int main(int argc, char* argv[]) {
    avalanche::client* client = new avalanche::client;
    client->addDispatcher("tcp://localhost:5024");
    TObject* rec;

    rec = (TObject*) client->recv(true);
    assert(rec);

    assert(!strcmp(rec->Class()->GetActualClass(rec)->GetName(), "TestObject"));
    TestObject* tob = dynamic_cast<TestObject*>(rec);

    assert(tob->aBool);
    assert(tob->aChar == Char_t(0xf8));
    assert(tob->aShort == Short_t(0xf00d));
    assert(tob->aInt == Int_t(0xc0c0ac0d));
    assert(tob->aLong == Long_t(0x18c0ffee));
    assert(tob->aLong64 == Long64_t(0x8baddeafdeadbeef));
    assert(tob->aFloat == Float_t(123.456E-7));
    assert(tob->aDouble == Double_t(987.654E-321));
    assert(tob->pChar[0] == Char_t(0xf8));
    assert(tob->pShort[0] == Short_t(0xf00d));
    assert(tob->pInt[0] == Int_t(0xc0c0ac0d));
    assert(tob->pLong[0] == Long_t(0x18c0ffee));
    assert(tob->pLong64[0] == Long64_t(0x8baddeafdeadbeef));
    assert(tob->pFloat[0] == Float_t(123.456E-7));
    assert(tob->pDouble[0] == Double_t(987.654E-321));

    std::cout << "TestObject ok." << std::endl;

    delete rec;
    delete client;
    return 0;  
}

