#include <iostream>
#include <avalanche.hpp>

#include "TestObject.hh"
#include "PackedEvent.hh"
#include "TObject.h"
#include "TROOT.h"
#include "TH1F.h"

int main(int argc, char* argv[]) {
    avalanche::server* serv = new avalanche::server("tcp://*:5024");

    //gROOT->SetMacroPath(".:");
    //gROOT->ProcessLine(".L TestObject.hh++");

    TestObject* tob = new TestObject();
    serv->sendObject(tob);

    SNOT::MTC* mtc = new SNOT::MTC();
    mtc->Cnt10Mhz = 0x8baddeafdeadbeef;
    mtc->Cnt50Mhz = 0xfedcba9876543210;
    mtc->RunID = 0xdeadbeef;
    mtc->SubRunID = 0xcafebabe;
    mtc->NHits = 0xc0ffeef8;
    mtc->GTId = 0xc0c0ac0d;
    mtc->GTMask = 0xfaceb00b;
    mtc->AMBPeak = 0xd00d;
    mtc->AMBDiff = 0xfeed;
    mtc->AMBIntg = 0xf001;
    mtc->Error = 0x1ee7;
    mtc->MissTrg = true;
    serv->sendObject(mtc);

    SNOT::CaenHeader caen_header;
    caen_header.ChannelMask = 0x12345678;
    caen_header.Pattern = 0xabcdabcd;
    caen_header.EventCount = 0xfedcab09;
    caen_header.Clock = 0xf0e0d0c0;
    serv->sendObject(&caen_header);

    SNOT::CaenTrace caen_trace;
    caen_trace.TraceLength = 2;
    caen_trace.Waveform = new UShort_t[2];
    caen_trace.Waveform[0] = 1;
    caen_trace.Waveform[1] = 2;
    serv->sendObject(&caen_trace);

    SNOT::PMT_cccc pmt_cccc;
    pmt_cccc.nhit_cccc = 1;
    pmt_cccc.Crate = new UChar_t(1);
    pmt_cccc.Card = new UChar_t(1);
    pmt_cccc.Channel = new UChar_t(1);
    pmt_cccc.Cell = new UChar_t(1);
    pmt_cccc.CGT16 = new Bool_t(true);
    pmt_cccc.CGT24 = new Bool_t(true);
    pmt_cccc.ES16 = new Bool_t(true);
    pmt_cccc.Missed = new Bool_t(true);
    pmt_cccc.NC = new Bool_t(true);
    pmt_cccc.LGI = new Bool_t(true);
    serv->sendObject(&pmt_cccc);

    SNOT::PackedEvent packed_event;
    packed_event.mtc = *mtc;
    packed_event.pmt_cccc = pmt_cccc;
    packed_event.caen_header = caen_header;
    packed_event.caen_trace0 = caen_trace;
    packed_event.pmt_qhl.nhit_qhl = 1;
    packed_event.pmt_qhl.QHL = new UShort_t(1);
    serv->sendObject(&packed_event);

    SNOT::CAST cast;
    cast.SourceID = 1;
    cast.SourceStat = 2;
    cast.NRopes = 1;
    cast.ManipPos[0] = 4;
    cast.ManipPos[2] = 5;
    cast.ManipDest[0] = 6;
    cast.ManipDest[2] = 7;
    cast.SrcPosUncert1 = 8;
    cast.SrcPosUncert2[0] = 9;
    cast.SrcPosUncert2[2] = 10;
    cast.LBallOrient = 11;
    cast.RopeID = new UInt_t(17);
    cast.RopeLen = new Float_t(12);
    cast.RopeTargLen = new Float_t(13);
    cast.RopeVel = new Float_t(14);
    cast.RopeTens = new Float_t(15);
    cast.RopeErr = new Float_t(16);
    serv->sendObject(&cast);

    SNOT::CAAC caac;
    caac.AVPos[0] = 1;
    caac.AVPos[2] = 3;
    caac.AVRoll[0] = 4;
    caac.AVRoll[2] = 6;
    caac.AVRopeLength[0] = 7;
    caac.AVRopeLength[6] = 13;
    serv->sendObject(&caac);

    SNOT::CMOSRate cmos_rate;
    cmos_rate.Crate = UInt_t(1);
    cmos_rate.SlotMask = UInt_t(0xb001);
    cmos_rate.ChannelMask[0] = UInt_t(0xbadbadf8);
    cmos_rate.Delay = UInt_t(543);
    cmos_rate.ErrorFlags = UInt_t(0x600d);
    cmos_rate.Rate[0] = Float_t(123.4e-5);
    cmos_rate.Rate[255] = Float_t(543.2e1);
    serv->sendObject(&cmos_rate);

    SNOT::FIFOState fifo_state;
    fifo_state.Crate = UInt_t(1);
    fifo_state.Fifo[0] = Float_t(123.4e-5);
    fifo_state.Fifo[15] = Float_t(543.2e1);
    fifo_state.Xl3Ram = Float_t(111.1);
    serv->sendObject(&fifo_state);

    sleep(1);
    delete serv;
    return 0;  
}

