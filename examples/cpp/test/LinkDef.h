
#ifdef __MAKECINT__
    #pragma link C++ nestedclass;

    #pragma link C++ class TestObject+;

    #pragma link C++ class SNOT::PMT_cccc+;
    #pragma link C++ class SNOT::PMT_qhl+;
    #pragma link C++ class SNOT::PMT_qhs+;
    #pragma link C++ class SNOT::PMT_qlx+;
    #pragma link C++ class SNOT::PMT_tac+;
    #pragma link C++ class SNOT::CaenTrace+;
    #pragma link C++ class vector<SNOT::CaenTrace>;
    #pragma link C++ class SNOT::CaenHeader+;
    #pragma link C++ class vector<SNOT::CaenHeader>;
    #pragma link C++ class SNOT::MTC+;
    #pragma link C++ class vector<SNOT::MTC>;
    #pragma link C++ struct SNOT::PackedEvent;
    #pragma link C++ struct vector<SNOT::PackedEvent>;
    #pragma link C++ struct SNOT::CAAC;
    #pragma link C++ struct vector<SNOT::CAAC>;
    #pragma link C++ struct SNOT::CAST;
    #pragma link C++ struct vector<SNOT::CAST>;
#endif

