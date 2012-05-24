#ifndef __SNOT_PackedEvent__
#define __SNOT_PackedEvent__

#include <vector>
#include <TObject.h>
#include <cstring>

namespace SNOT {

class PMT_cccc {
    public:
        PMT_cccc(): nhit_cccc(0),
            Crate(0), Card(0), Channel(0), Cell(0), CGT16(0),
            CGT24(0), ES16(0), Missed(0), NC(0), LGI(0) {}

        virtual ~PMT_cccc() {
            if (Crate) delete [] Crate;
            if (Card) delete [] Card;
            if (Channel) delete [] Channel;
            if (Cell) delete [] Cell;
            if (CGT16) delete [] CGT16;
            if (CGT24) delete [] CGT24;
            if (ES16) delete [] ES16;
            if (Missed) delete [] Missed;
            if (NC) delete [] NC;
            if (LGI) delete [] LGI;
        }

        PMT_cccc(const PMT_cccc &src):
            nhit_cccc(src.nhit_cccc),
            Crate(0), Card(0), Channel(0), Cell(0), CGT16(0),
            CGT24(0), ES16(0), Missed(0), NC(0), LGI(0) {
            if (nhit_cccc) {
                Crate = new UChar_t[nhit_cccc];
                memcpy(Crate, src.Crate, nhit_cccc * sizeof(UChar_t));
                Card = new UChar_t[nhit_cccc];
                memcpy(Card, src.Card, nhit_cccc * sizeof(UChar_t));
                Channel = new UChar_t[nhit_cccc];
                memcpy(Channel, src.Channel, nhit_cccc * sizeof(UChar_t));
                Cell = new UChar_t[nhit_cccc];
                memcpy(Cell, src.Cell, nhit_cccc * sizeof(UChar_t));
                CGT16 = new Bool_t[nhit_cccc];
                memcpy(CGT16, src.CGT16, nhit_cccc * sizeof(Bool_t));
                CGT24 = new Bool_t[nhit_cccc];
                memcpy(CGT24, src.CGT24, nhit_cccc * sizeof(Bool_t));
                ES16 = new Bool_t[nhit_cccc];
                memcpy(ES16, src.ES16, nhit_cccc * sizeof(Bool_t));
                Missed = new Bool_t[nhit_cccc];
                memcpy(Missed, src.Missed, nhit_cccc * sizeof(Bool_t));
                NC = new Bool_t[nhit_cccc];
                memcpy(NC, src.NC, nhit_cccc * sizeof(Bool_t));
                LGI = new Bool_t[nhit_cccc];
                memcpy(LGI, src.LGI, nhit_cccc * sizeof(Bool_t));
            }
        }

        PMT_cccc &operator=(const PMT_cccc &rhs) {
            if (this == &rhs) return *this;
            if (nhit_cccc) {
                if (Crate) delete [] Crate;
                if (Card) delete [] Card;
                if (Channel) delete [] Channel;
                if (Cell) delete [] Cell;
                if (CGT16) delete [] CGT16;
                if (CGT24) delete [] CGT24;
                if (ES16) delete [] ES16;
                if (Missed) delete [] Missed;
                if (NC) delete [] NC;
                if (LGI) delete [] LGI;
            }
            nhit_cccc = rhs.nhit_cccc;
            if (nhit_cccc) {
                Crate = new UChar_t[nhit_cccc];
                memcpy(Crate, rhs.Crate, nhit_cccc * sizeof(UChar_t));
                Card = new UChar_t[nhit_cccc];
                memcpy(Card, rhs.Card, nhit_cccc * sizeof(UChar_t));
                Channel = new UChar_t[nhit_cccc];
                memcpy(Channel, rhs.Channel, nhit_cccc * sizeof(UChar_t));
                Cell = new UChar_t[nhit_cccc];
                memcpy(Cell, rhs.Cell, nhit_cccc * sizeof(UChar_t));
                CGT16 = new Bool_t[nhit_cccc];
                memcpy(CGT16, rhs.CGT16, nhit_cccc * sizeof(Bool_t));
                CGT24 = new Bool_t[nhit_cccc];
                memcpy(CGT24, rhs.CGT24, nhit_cccc * sizeof(Bool_t));
                ES16 = new Bool_t[nhit_cccc];
                memcpy(ES16, rhs.ES16, nhit_cccc * sizeof(Bool_t));
                Missed = new Bool_t[nhit_cccc];
                memcpy(Missed, rhs.Missed, nhit_cccc * sizeof(Bool_t));
                NC = new Bool_t[nhit_cccc];
                memcpy(NC, rhs.NC, nhit_cccc * sizeof(Bool_t));
                LGI = new Bool_t[nhit_cccc];
                memcpy(LGI, rhs.LGI, nhit_cccc * sizeof(Bool_t));
            }
            return *this;
        }

        Int_t nhit_cccc;
        UChar_t* Crate; //[nhit_cccc]
        UChar_t* Card; //[nhit_cccc]
        UChar_t* Channel; //[nhit_cccc]
        UChar_t* Cell; //[nhit_cccc]
        Bool_t* CGT16; //[nhit_cccc];
        Bool_t* CGT24; //[nhit_cccc];
        Bool_t* ES16; //[nhit_cccc];
        Bool_t* Missed; //[nhit_cccc];
        Bool_t* NC; //[nhit_cccc];
        Bool_t* LGI; //[nhit_cccc];

        ClassDef(PMT_cccc, 2)
};

class PMT_qhl {
    public:
        PMT_qhl(): nhit_qhl(0), QHL(0) {}

        virtual ~PMT_qhl() {
            if (QHL) delete [] QHL;
        }

        PMT_qhl(const PMT_qhl &src):
            nhit_qhl(src.nhit_qhl), QHL(0) {
            if (nhit_qhl) {
                QHL = new UShort_t[nhit_qhl];
                memcpy(QHL, src.QHL, nhit_qhl * sizeof(UShort_t));
            }
        }

        PMT_qhl &operator=(const PMT_qhl &rhs) {
            if (this == &rhs) return *this;
            if (nhit_qhl) delete [] QHL;
            QHL = 0;
            nhit_qhl = rhs.nhit_qhl;
            if (nhit_qhl) {
                QHL = new UShort_t[nhit_qhl];
                memcpy(QHL, rhs.QHL, nhit_qhl * sizeof(UShort_t));
            }
            return *this;
        }

        Int_t nhit_qhl;
        UShort_t* QHL; //[nhit_qhl]

        ClassDef(PMT_qhl, 2)
};

class PMT_qhs {
    public:
        PMT_qhs(): nhit_qhs(0), QHS(0) {}
        virtual ~PMT_qhs() {
            if (QHS) delete [] QHS;
        }

        PMT_qhs(const PMT_qhs &src):
            nhit_qhs(src.nhit_qhs), QHS(0) {
            if (nhit_qhs) {
                QHS = new UShort_t[nhit_qhs];
                memcpy(QHS, src.QHS, nhit_qhs * sizeof(UShort_t));
            }
        }

        PMT_qhs &operator=(const PMT_qhs &rhs) {
            if (this == &rhs) return *this;
            if (nhit_qhs) delete [] QHS;
            QHS = 0;
            nhit_qhs = rhs.nhit_qhs;
            if (nhit_qhs) {
                QHS = new UShort_t[nhit_qhs];
                memcpy(QHS, rhs.QHS, nhit_qhs * sizeof(UShort_t));
            }
            return *this;
        }

        Int_t nhit_qhs;
        UShort_t* QHS; //[nhit_qhs]

        ClassDef(PMT_qhs, 2)
};

class PMT_qlx {
    public:
        PMT_qlx(): nhit_qlx(0), QLX(0) {}

        virtual ~PMT_qlx() {
            if (QLX) delete [] QLX;
        }

        PMT_qlx(const PMT_qlx &src):
            nhit_qlx(src.nhit_qlx), QLX(0) {
            if (nhit_qlx) {
                QLX = new UShort_t[nhit_qlx];
                memcpy(QLX, src.QLX, nhit_qlx * sizeof(UShort_t));
            }
        }

        PMT_qlx &operator=(const PMT_qlx &rhs) {
            if (this == &rhs) return *this;
            if (nhit_qlx) delete [] QLX;
            QLX = 0;
            nhit_qlx = rhs.nhit_qlx;
            if (nhit_qlx) {
                QLX = new UShort_t[nhit_qlx];
                memcpy(QLX, rhs.QLX, nhit_qlx * sizeof(UShort_t));
            }
            return *this;
        }

        Int_t nhit_qlx;
        UShort_t* QLX; //[nhit_qlx]

        ClassDef(PMT_qlx, 2)
};

class PMT_tac {
    public:
        PMT_tac(): nhit_tac(0), TAC(0) {}

        virtual ~PMT_tac() {
            if (TAC) delete [] TAC;
        }

        PMT_tac(const PMT_tac &src):
            nhit_tac(src.nhit_tac), TAC(0) {
            if (nhit_tac) {
                TAC = new UShort_t[nhit_tac];
                memcpy(TAC, src.TAC, nhit_tac * sizeof(UShort_t));
            }
        }

        PMT_tac &operator=(const PMT_tac &rhs) {
            if (this == &rhs) return *this;
            if (nhit_tac) delete [] TAC;
            TAC = 0;
            nhit_tac = rhs.nhit_tac;
            if (nhit_tac) {
                TAC = new UShort_t[nhit_tac];
                memcpy(TAC, rhs.TAC, nhit_tac * sizeof(UShort_t));
            }
            return *this;
        }

        Int_t nhit_tac;
        UShort_t* TAC; //[nhit_tac]

        ClassDef(PMT_tac, 2)
};

class CaenTrace: public TObject {
    public:
        CaenTrace(): TObject(),
            TraceLength(0), Waveform(0) {}

        virtual ~CaenTrace() {
            if (Waveform) delete [] Waveform;
        }

        CaenTrace(const CaenTrace &src): TObject(),
            TraceLength(src.TraceLength), Waveform(0)  {
            if (TraceLength) {
                Waveform = new UShort_t[TraceLength];
                memcpy(Waveform, src.Waveform, TraceLength * sizeof(UShort_t));
            }
        }

        CaenTrace &operator=(const CaenTrace &rhs) {
            if (this == &rhs) {
                return *this;
            }
            if (Waveform) delete [] Waveform;
            Waveform = 0;
            TraceLength = rhs.TraceLength;
            if (TraceLength) {
                Waveform = new UShort_t[TraceLength];
                memcpy(Waveform, rhs.Waveform, TraceLength * sizeof(UShort_t));
            }
            return *this;
        }

        Int_t TraceLength;
        UShort_t* Waveform; //[TraceLength]
        //baseline, risetime, ...

        ClassDef(CaenTrace, 2)
};

class CaenHeader: public TObject {
    public:
        CaenHeader(): TObject(),
            ChannelMask(0), Pattern(0), EventCount(0), Clock(0) {}
        virtual ~CaenHeader() {}

        UInt_t ChannelMask;
        UInt_t Pattern;
        UInt_t EventCount;
        UInt_t Clock;

        ClassDef(CaenHeader, 2)

        //default copy constructor is ok
        //default copy assigment operator is ok
};

class MTC : public TObject { 
    public:
        MTC(): TObject(),
            Cnt10Mhz(0), Cnt50Mhz(0), RunID(0), SubRunID(0), NHits(0),
            GTId(0), GTMask(0), AMBPeak(0), AMBDiff(0), AMBIntg(0),
            Error(0), MissTrg(false) {}
        virtual ~MTC() {}

        ULong64_t Cnt10Mhz;
        ULong64_t Cnt50Mhz;
        UInt_t RunID;
        UInt_t SubRunID;
        UInt_t NHits;
        UInt_t GTId;
        UInt_t GTMask;
        UShort_t AMBPeak;
        UShort_t AMBDiff;
        UShort_t AMBIntg;
        UShort_t Error;
        Bool_t MissTrg;
 
        ClassDef(MTC, 2)

        //default copy constructor is ok
        //default copy assignment operator is ok
};

class PackedEvent : public TObject {
    public:
        PackedEvent(): TObject(),
            mtc(MTC()), pmt_cccc(PMT_cccc()), pmt_qhl(PMT_qhl()), pmt_qhs(PMT_qhs()),
            pmt_qlx(PMT_qlx()), pmt_tac(PMT_tac()), caen_header(CaenHeader()),
            caen_trace0(CaenTrace()), caen_trace1(CaenTrace()), caen_trace2(CaenTrace()),
            caen_trace3(CaenTrace()), caen_trace4(CaenTrace()), caen_trace5(CaenTrace()),
            caen_trace6(CaenTrace()), caen_trace7(CaenTrace()), genericRec(0) {}

        virtual ~PackedEvent() {
            if (genericRec) {
                delete genericRec;
            }
        }

        PackedEvent(const PackedEvent &src): TObject(),
            mtc(MTC()), pmt_cccc(PMT_cccc()), pmt_qhl(PMT_qhl()), pmt_qhs(PMT_qhs()),
            pmt_qlx(PMT_qlx()), pmt_tac(PMT_tac()), caen_header(CaenHeader()),
            caen_trace0(CaenTrace()), caen_trace1(CaenTrace()), caen_trace2(CaenTrace()),
            caen_trace3(CaenTrace()), caen_trace4(CaenTrace()), caen_trace5(CaenTrace()),
            caen_trace6(CaenTrace()), caen_trace7(CaenTrace()), genericRec(0) {

            if (src.genericRec) {
                genericRec = src.genericRec->Clone(); //this uses Streamer (dictionary and gDirectory)
            }
        }
            
        PackedEvent &operator=(const PackedEvent &rhs) {
            if (this == &rhs) return *this;
            mtc = rhs.mtc;
            pmt_cccc = rhs.pmt_cccc;
            pmt_qhl = rhs.pmt_qhl;
            pmt_qhs = rhs.pmt_qhs;
            pmt_qlx = rhs.pmt_qlx;
            pmt_tac = rhs.pmt_tac;
            caen_header = rhs.caen_header;
            caen_trace0 = rhs.caen_trace0;
            caen_trace1 = rhs.caen_trace1;
            caen_trace2 = rhs.caen_trace2;
            caen_trace3 = rhs.caen_trace3;
            caen_trace4 = rhs.caen_trace4;
            caen_trace5 = rhs.caen_trace5;
            caen_trace6 = rhs.caen_trace6;
            caen_trace7 = rhs.caen_trace7;
            if (genericRec) delete genericRec;
            genericRec = 0;
            if (rhs.genericRec) {
                genericRec = rhs.genericRec->Clone(); //this uses Streamer (dictionary and gDirectory)
            }
            //do NOT call TObject::operator=(rhs);
            return *this;
        }

        MTC mtc;
        PMT_cccc pmt_cccc;
        PMT_qhl pmt_qhl;
        PMT_qhs pmt_qhs;
        PMT_qlx pmt_qlx;
        PMT_tac pmt_tac;
        CaenHeader caen_header;
        CaenTrace caen_trace0;
        CaenTrace caen_trace1;
        CaenTrace caen_trace2;
        CaenTrace caen_trace3;
        CaenTrace caen_trace4;
        CaenTrace caen_trace5;
        CaenTrace caen_trace6;
        CaenTrace caen_trace7;
        TObject* genericRec;

        ClassDef(PackedEvent, 2)
};

class CAST : public TObject {
public:
  CAST() : TObject() { Init(); };
  CAST(const CAST &rhs) : TObject() { Init(); CopyObj(rhs); };
  virtual ~CAST() {};
  virtual CAST &operator=(const CAST &rhs) { CopyObj(rhs); return *this; };

  // Members
  UShort_t SourceID;
  UShort_t SourceStat;
  UShort_t NRopes;
  float ManipPos[3];
  float ManipDest[3];
  float SrcPosUncert1;
  float SrcPosUncert2[3];
  float LBallOrient;
  std::vector<int> RopeID;
  std::vector<float> RopeLen;
  std::vector<float> RopeTargLen;
  std::vector<float> RopeVel;
  std::vector<float> RopeTens;
  std::vector<float> RopeErr;

  // ROOT junk
  ClassDef(CAST,2)

protected:
  virtual void Init() {
    for(int i=0;i<3;++i){
      ManipPos[i] = 0.;
      ManipDest[i] = 0.;
      SrcPosUncert2[i] = 0.;
    }
    SourceID = 0;
    SourceStat = 0;
    NRopes = 0;
    SrcPosUncert1 = 0.;
    LBallOrient = 0.;
    RopeID.resize(0);
    RopeLen.resize(0);
    RopeTargLen.resize(0);
    RopeVel.resize(0);
    RopeTens.resize(0);
    RopeErr.resize(0);
  }
  
  virtual void CopyObj(const CAST &rhs) {
    for(int i=0;i<3;++i){
      ManipPos[i] = rhs.ManipPos[i];
      ManipDest[i] = rhs.ManipDest[i];
      SrcPosUncert2[i] = rhs.SrcPosUncert2[i];
    }
    SourceID = rhs.SourceID;
    SourceStat = rhs.SourceStat;
    NRopes = rhs.NRopes;
    SrcPosUncert1 = rhs.SrcPosUncert1;
    LBallOrient = rhs.LBallOrient;
    RopeID = rhs.RopeID;
    RopeLen = rhs.RopeLen;
    RopeTargLen = rhs.RopeTargLen;
    RopeVel = rhs.RopeVel;
    RopeTens = rhs.RopeTens;
    RopeErr = rhs.RopeErr;
  }
};

class CAAC : public TObject {
public:
  CAAC() : TObject() { Init(); };
  CAAC(const CAAC &rhs) : TObject() { Init(); CopyObj(rhs); };
  virtual ~CAAC() {};
  virtual CAAC &operator=(const CAAC &rhs) { CopyObj(rhs); return *this; };

  // Members
  float AVPos[3];
  float AVRoll[3];  // roll, pitch and yaw
  float AVRopeLength[7];

  // ROOT junk
  ClassDef(CAAC,2)

protected:
  virtual void Init() {
    for(int i=0;i<3;++i){
      AVPos[i] = 0.;
      AVRoll[i] = 0.;
    }
    for(int i=0;i<7;++i){
      AVRopeLength[i] = 0.;
    }
  }

  virtual void CopyObj(const CAAC &rhs) {
    for(int i=0;i<3;++i){
      AVPos[i] = rhs.AVPos[i];
      AVRoll[i] = rhs.AVRoll[i];
    }
    for(int i=0;i<7;++i){
      AVRopeLength[i] = rhs.AVRopeLength[i];
    }
  }
};

} // namespace SNOT
#endif

#ifdef __MAKECINT__
    #pragma link C++ nestedclass;
    #pragma link C++ class SNOT::PMT_cccc+;
    #pragma link C++ class SNOT::PMT_qhl+;
    #pragma link C++ class SNOT::PMT_qhs+;
    #pragma link C++ class SNOT::PMT_qlx+;
    #pragma link C++ class SNOT::PMT_tac+;
    #pragma link C++ class SNOT::CaenTrace+;
    #pragma link C++ class std::vector<SNOT::CaenTrace>;
    #pragma link C++ class SNOT::CaenHeader+;
    #pragma link C++ class std::vector<SNOT::CaenHeader>;
    #pragma link C++ class SNOT::MTC+;
    #pragma link C++ class std::vector<SNOT::MTC>;
    #pragma link C++ class SNOT::PackedEvent;
    #pragma link C++ class std::vector<SNOT::PackedEvent>;
    #pragma link C++ class SNOT::CAAC;
    #pragma link C++ class std::vector<SNOT::CAAC>;
    #pragma link C++ class SNOT::CAST;
    #pragma link C++ class std::vector<SNOT::CAST>;
#endif //__MAKECINT__

