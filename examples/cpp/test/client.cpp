#include <iostream>
#include <cassert>
#include <avalanche.hpp>

#include "TestObject.hh"
#include "PackedEvent.hh"
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

    rec = (TObject*) client->recv(true);
    assert(rec);
    assert(!strcmp(rec->Class()->GetActualClass(rec)->GetName(), "SNOT::MTC"));
    SNOT::MTC* mtc = dynamic_cast<SNOT::MTC*>(rec);

    assert(mtc->Cnt10Mhz == ULong64_t(0x8baddeafdeadbeef));
    assert(mtc->Cnt50Mhz == ULong64_t(0xfedcba9876543210));
    assert(mtc->RunID == UInt_t(0xdeadbeef));
    assert(mtc->SubRunID == UInt_t(0xcafebabe));
    assert(mtc->NHits = UInt_t(0xc0ffeef8));
    assert(mtc->GTId = UInt_t(0xc0c0ac0d));
    assert(mtc->GTMask = UInt_t(0xfaceb00b));
    assert(mtc->AMBPeak = UShort_t(0xd00d));
    assert(mtc->AMBDiff = UShort_t(0xfeed));
    assert(mtc->AMBIntg = UShort_t(0xf001));
    assert(mtc->Error = UShort_t(0x1ee7));
    assert(mtc->MissTrg = Bool_t(true));

    std::cout << "MTC ok." << std::endl;
    delete rec;

    rec = (TObject*) client->recv(true);
    assert(rec);
    assert(!strcmp(rec->Class()->GetActualClass(rec)->GetName(), "SNOT::CaenHeader"));
    SNOT::CaenHeader* caen_header = dynamic_cast<SNOT::CaenHeader*>(rec);

    assert(caen_header->ChannelMask == UInt_t(0x12345678));
    assert(caen_header->Pattern == UInt_t(0xabcdabcd));
    assert(caen_header->EventCount = UInt_t(0xfedcab09));
    assert(caen_header->Clock == UInt_t(0xf0e0d0c0));

    std::cout << "CaenHeader ok." << std::endl;
    delete rec;

    rec = (TObject*) client->recv(true);
    assert(rec);
    assert(!strcmp(rec->Class()->GetActualClass(rec)->GetName(), "SNOT::CaenTrace"));
    SNOT::CaenTrace* caen_trace = dynamic_cast<SNOT::CaenTrace*>(rec);

    assert(caen_trace->TraceLength == Int_t(2));
    assert(caen_trace->Waveform[0] == UShort_t(1));

    std::cout << "CaenTrace ok." << std::endl;
    delete rec;

    rec = (TObject*) client->recv(true);
    assert(rec);
    assert(!strcmp(rec->Class()->GetActualClass(rec)->GetName(), "SNOT::PMT_cccc"));
    SNOT::PMT_cccc* pmt_cccc = dynamic_cast<SNOT::PMT_cccc*>(rec);

    assert(pmt_cccc->nhit_cccc == Int_t(1));
    assert(pmt_cccc->Crate[0] == UChar_t(1));
    assert(pmt_cccc->Card[0] == UChar_t(1));
    assert(pmt_cccc->Channel[0] == UChar_t(1));
    assert(pmt_cccc->Cell[0] == UChar_t(1));
    assert(pmt_cccc->CGT16[0] == Bool_t(true));
    assert(pmt_cccc->CGT24[0] == Bool_t(true));
    assert(pmt_cccc->ES16[0] == Bool_t(true));
    assert(pmt_cccc->Missed[0] == Bool_t(true));
    assert(pmt_cccc->NC[0] == Bool_t(true));
    assert(pmt_cccc->LGI[0] == Bool_t(true));
 
    std::cout << "PMT_cccc ok." << std::endl;
    delete rec;

    rec = (TObject*) client->recv(true);
    assert(rec);
    assert(!strcmp(rec->Class()->GetActualClass(rec)->GetName(), "SNOT::PackedEvent"));
    SNOT::PackedEvent* packed_event = dynamic_cast<SNOT::PackedEvent*>(rec);

    assert(packed_event->mtc.Cnt10Mhz == ULong64_t(0x8baddeafdeadbeef));
    assert(packed_event->pmt_cccc.Crate[0] == UChar_t(1));
    assert(packed_event->caen_header.ChannelMask == UInt_t(0x12345678));
    assert(packed_event->caen_trace0.Waveform[0] == UShort_t(1));
    assert(packed_event->caen_trace1.TraceLength == Int_t(0));
    assert(packed_event->pmt_qhl.QHL[0] == UShort_t(1));
    assert(packed_event->pmt_tac.nhit_tac == Int_t(0));
    assert(packed_event->genericRec == 0);
 
    std::cout << "PackedEvent ok." << std::endl;
    delete rec;

    rec = (TObject*) client->recv(true);
    assert(rec);
    assert(!strcmp(rec->Class()->GetActualClass(rec)->GetName(), "SNOT::CAST"));
    SNOT::CAST* cast = dynamic_cast<SNOT::CAST*>(rec);

    assert(cast->SourceID == UShort_t(1));
    assert(cast->SourceStat == UShort_t(2));
    assert(cast->NRopes == Int_t(1));
    assert(cast->ManipPos[0] == Float_t(4));
    assert(cast->ManipPos[2] == Float_t(5));
    assert(cast->ManipDest[0] == Float_t(6));
    assert(cast->ManipDest[2] == Float_t(7));
    assert(cast->SrcPosUncert1 == Float_t(8));
    assert(cast->SrcPosUncert2[0] == Float_t(9));
    assert(cast->SrcPosUncert2[2] == Float_t(10));
    assert(cast->LBallOrient == Float_t(11));
    assert(cast->RopeID[0] == UInt_t(17));
    assert(cast->RopeLen[0] == Float_t(12));
    assert(cast->RopeTargLen[0] == Float_t(13));
    assert(cast->RopeVel[0] == Float_t(14));
    assert(cast->RopeTens[0] == Float_t(15));
    assert(cast->RopeErr[0] == Float_t(16));

    std::cout << "CAST ok." << std::endl;
    delete rec;

    rec = (TObject*) client->recv(true);
    assert(rec);
    assert(!strcmp(rec->Class()->GetActualClass(rec)->GetName(), "SNOT::CAAC"));
    SNOT::CAAC* caac = dynamic_cast<SNOT::CAAC*>(rec);

    assert(caac->AVPos[0] == UInt_t(1));
    assert(caac->AVPos[2] == Float_t(3));
    assert(caac->AVRoll[0] == Float_t(4));
    assert(caac->AVRoll[2] == Float_t(6));
    assert(caac->AVRopeLength[0] == Float_t(7));
    assert(caac->AVRopeLength[6] == Float_t(13));
 
    std::cout << "CAAC ok." << std::endl;
    delete rec;

    rec = (TObject*) client->recv(true);
    assert(rec);
    assert(!strcmp(rec->Class()->GetActualClass(rec)->GetName(), "SNOT::CMOSRate"));
    SNOT::CMOSRate* cmos_rate = dynamic_cast<SNOT::CMOSRate*>(rec);

    assert(cmos_rate->Crate == UInt_t(1));
    assert(cmos_rate->SlotMask == UInt_t(0xb001));
    assert(cmos_rate->ChannelMask[0] == UInt_t(0xbadbadf8));
    assert(cmos_rate->Delay == UInt_t(543));
    assert(cmos_rate->ErrorFlags == UInt_t(0x600d));
    assert(cmos_rate->Rate[0] == Float_t(123.4e-5));
    assert(cmos_rate->Rate[255] == Float_t(543.2e1));

    std::cout << "CMOSRate ok." << std::endl;
    delete rec;

    rec = (TObject*) client->recv(true);
    assert(rec);
    assert(!strcmp(rec->Class()->GetActualClass(rec)->GetName(), "SNOT::FIFOState"));
    SNOT::FIFOState* fifo_state = dynamic_cast<SNOT::FIFOState*>(rec);

    assert(fifo_state->Crate == UInt_t(1));
    assert(fifo_state->Fifo[0] == Float_t(123.4e-5));
    assert(fifo_state->Fifo[15] == Float_t(543.2e1));
    assert(fifo_state->Xl3Ram == Float_t(111.1));

    std::cout << "FIFOState ok." << std::endl;
    delete rec;
 
    delete client;
    return 0;  
}

