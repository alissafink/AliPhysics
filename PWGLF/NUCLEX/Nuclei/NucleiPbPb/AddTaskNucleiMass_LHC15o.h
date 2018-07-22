#ifndef AliAnalysisTask_Alissa_cxx
#define AliAnalysisTask_Alissa_cxx


#include "AliAnalysisTaskSE.h"
#include "AliAnalysisTask.h"
#include "AliAODEvent.h"
#include "AliAODTrack.h"
#include "AliPIDResponse.h"
#include "TList.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"

class AliAnalysisTask_Alissa : public AliAnalysisTaskSE {
    
public:
    AliAnalysisTask_Alissa();
    AliAnalysisTask_Alissa(const char *name);
    virtual ~AliAnalysisTask_Alissa();
    
    //Functions
    virtual void UserCreateOutputObjects();
    virtual void UserExec  (Option_t *option);
    virtual void Terminate (Option_t *);

    Bool_t GetEvent();
	Double_t GetSharedClusters(AliAODTrack *track);
    Double_t GetDCAXY(AliAODTrack *track);
    Double_t GetDCAZ(AliAODTrack *track);
    Bool_t TrackQualityCuts(AliAODTrack *track);

	Double_t Beta(AliAODTrack *track);
	Double_t Mass(AliAODTrack *track);

    Bool_t Proton_Candidate(AliAODTrack *track);
    Bool_t Deuteron_Candidate(AliAODTrack *track);
    Bool_t He3_Candidate(AliAODTrack *track);
	Bool_t He4_Candidate(AliAODTrack *track);
    Bool_t Triton_Candidate(AliAODTrack *track);
    
private:
    AliAODEvent *fAODevent;//!
    TList       *fOutputList;//!
    AliPIDResponse* fPIDResponse;//!
    
    //Here you create your histograms
    TH1F *hist_events;//!

    TH2F *hist_TPC;//!
    TH2F *hist_TOF;//!

	TH2F *hist_DCAXY_Proton;//!
	TH2F *hist_DCAXY_AntiProton;//!
	TH2F *hist_DCAXY_Deuteron;//!
	TH2F *hist_DCAXY_AntiDeuteron;//!
    TH2F *hist_DCAXY_He3;//!
    TH2F *hist_DCAXY_AntiHe3;//!
    TH2F *hist_DCAXY_He4;//!
    TH2F *hist_DCAXY_AntiHe4;//!
    TH2F *hist_DCAXY_Triton;//!
    TH2F *hist_DCAXY_AntiTriton;//!

	TH3F *hist_mass_Proton_pos;//!
	TH3F *hist_mass_AntiProton_pos;//!
	TH3F *hist_mass_Deuteron_pos;//!
	TH3F *hist_mass_AntiDeuteron_pos;//!
	TH3F *hist_mass_He3_pos;//!
	TH3F *hist_mass_AntiHe3_pos;//!

	TH3F *hist_mass_Proton_neg;//!
	TH3F *hist_mass_AntiProton_neg;//!
	TH3F *hist_mass_Deuteron_neg;//!
	TH3F *hist_mass_AntiDeuteron_neg;//!
	TH3F *hist_mass_He3_neg;//!
	TH3F *hist_mass_AntiHe3_neg;//!

    AliAnalysisTask_Alissa(const AliAnalysisTask_Alissa&);
    AliAnalysisTask_Alissa& operator=(const AliAnalysisTask_Alissa&);
    
    ClassDef(AliAnalysisTask_Alissa, 1);
};
#endif
