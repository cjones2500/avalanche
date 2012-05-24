#include <iostream>
#include <avalanche.hpp>

#include "TestObject.hh"
#include "TObject.h"
#include "TROOT.h"
#include "TH1F.h"

int main(int argc, char* argv[]) {
    avalanche::server* serv = new avalanche::server("tcp://*:5024");

    //gROOT->SetMacroPath(".:");
    //gROOT->ProcessLine(".L TestObject.hh++");

    TestObject* tob = new TestObject();
    serv->sendObject(tob);

    sleep(1);
    delete serv;
    return 0;  
}

