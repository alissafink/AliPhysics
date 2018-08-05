#include "AddTaskNucleiMass_LHC15o.h"
#include "AliInputEventHandler.h"
#include "AliAnalysisManager.h"
#include "AliAnalysisTaskSE.h"
#include "AliAnalysisTask.h"
#include "AliAODVertex.h"
#include "AliAODTrack.h"
#include "AliAODEvent.h"
#include "TChain.h"
#include "TList.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "AliMultSelection.h"
#include "AliPIDResponse.h"

ClassImp(AddTaskNucleiMass_LHC15o)

//_________________________________________________________________________________________________________________________________________________________________________________________________
AddTaskNucleiMass_LHC15o::AddTaskNucleiMass_LHC15o():
AliAnalysisTaskSE(),
fAODevent(0x0),
fOutputList(0x0),
fPIDResponse(0x0)
{}
//_________________________________________________________________________________________________________________________________________________________________________________________________
AddTaskNucleiMass_LHC15o::AddTaskNucleiMass_LHC15o(const char *name):
AliAnalysisTaskSE(name),
fAODevent(0x0),
fOutputList(0x0),
fPIDResponse(0x0)
{
    DefineInput(0, TChain::Class());
    DefineOutput(1, TList::Class());
}
//_________________________________________________________________________________________________________________________________________________________________________________________________
AddTaskNucleiMass_LHC15o::~AddTaskNucleiMass_LHC15o()
{
    fOutputList->Clear();
    delete fAODevent;
    delete fOutputList;
    delete fPIDResponse;

}
//_________________________________________________________________________________________________________________________________________________________________________________________________
void AddTaskNucleiMass_LHC15o::UserCreateOutputObjects()
{
    //PIDResponse
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    AliInputEventHandler *inputHandler = (AliInputEventHandler*) (mgr->GetInputEventHandler());
    fPIDResponse = inputHandler->GetPIDResponse();
    
    fOutputList = new TList();
    fOutputList -> SetOwner();
    
    //Event properties histograms
    hist_events = new TH1F ("hist_events","",5,0,5);

	fOutputList -> Add(hist_events);
    
    hist_TPC = new TH2F ("hist_TPC","",800,0,20,800,0,200);
    hist_TOF = new TH2F ("hist_TOF","",800,0,20,800,0.0,1.2);
    fOutputList -> Add(hist_TPC);
    fOutputList -> Add(hist_TOF);


	//Matter antimatter properties vs pt plots
	hist_DCAXY_Proton	    = new TH2F ("hist_DCAXY_Proton","",200,0,20,200,-2,2);
	hist_DCAXY_Deuteron     = new TH2F ("hist_DCAXY_Deuteron","",200,0,20,200,-2,2);
    hist_DCAXY_He3          = new TH2F ("hist_DCAXY_He3","",200,0,20,200,-2,2);
	hist_DCAXY_AntiProton   = new TH2F ("hist_DCAXY_AntiProton","",200,0,20,200,-2,2);
    hist_DCAXY_AntiDeuteron = new TH2F ("hist_DCAXY_AntiDeuteron","",200,0,20,200,-2,2);
    hist_DCAXY_AntiHe3      = new TH2F ("hist_DCAXY_AntiHe3","",200,0,20,200,-2,2);

    fOutputList -> Add(hist_DCAXY_Proton);
    fOutputList -> Add(hist_DCAXY_Deuteron);
    fOutputList -> Add(hist_DCAXY_He3);
    fOutputList -> Add(hist_DCAXY_AntiProton);
    fOutputList -> Add(hist_DCAXY_AntiDeuteron);
    fOutputList -> Add(hist_DCAXY_AntiHe3);
    
    
    
    
    //Mass Distributions
	hist_mass_Proton_pos       = new TH3F ("hist_mass_Proton_pos","",200,0,20,200,-1,1,1000,0,10);
	hist_mass_AntiProton_pos   = new TH3F ("hist_mass_AntiProton_pos","",200,0,20,200,-1,1,1000,0,10);
	hist_mass_Deuteron_pos     = new TH3F ("hist_mass_Deuteron_pos","",200,0,20,200,-1,1,1000,0,10);
	hist_mass_AntiDeuteron_pos = new TH3F ("hist_mass_AntiDeuteron_pos","",200,0,20,200,-1,1,1000,0,10);
	hist_mass_He3_pos          = new TH3F ("hist_mass_He3_pos","",200,0,20,200,-1,1,1000,0,10);
	hist_mass_AntiHe3_pos      = new TH3F ("hist_mass_AntiHe3_pos","",200,0,20,200,-1,1,1000,0,10);

    fOutputList -> Add(hist_mass_Proton_pos);
    fOutputList -> Add(hist_mass_AntiProton_pos);
    fOutputList -> Add(hist_mass_Deuteron_pos);
    fOutputList -> Add(hist_mass_AntiDeuteron_pos);
    fOutputList -> Add(hist_mass_He3_pos);
    fOutputList -> Add(hist_mass_AntiHe3_pos);

    //Mass Distributions
	hist_mass_Proton_neg       = new TH3F ("hist_mass_Proton_neg","",200,0,20,200,-1,1,1000,0,10);
	hist_mass_AntiProton_neg   = new TH3F ("hist_mass_AntiProton_neg","",200,0,20,200,-1,1,1000,0,10);
	hist_mass_Deuteron_neg     = new TH3F ("hist_mass_Deuteron_neg","",200,0,20,200,-1,1,1000,0,10);
	hist_mass_AntiDeuteron_neg = new TH3F ("hist_mass_AntiDeuteron_neg","",200,0,20,200,-1,1,1000,0,10);
	hist_mass_He3_neg          = new TH3F ("hist_mass_He3_neg","",200,0,20,200,-1,1,1000,0,10);
	hist_mass_AntiHe3_neg      = new TH3F ("hist_mass_AntiHe3_neg","",200,0,20,200,-1,1,1000,0,10);

    fOutputList -> Add(hist_mass_Proton_neg);
    fOutputList -> Add(hist_mass_AntiProton_neg);
    fOutputList -> Add(hist_mass_Deuteron_neg);
    fOutputList -> Add(hist_mass_AntiDeuteron_neg);
    fOutputList -> Add(hist_mass_He3_neg);
    fOutputList -> Add(hist_mass_AntiHe3_neg);

    PostData(1, fOutputList);
}
//_________________________________________________________________________________________________________________________________________________________________________________________________
void AddTaskNucleiMass_LHC15o::UserExec(Option_t *)
{
    //Get Event and make event selection
    if ( !GetEvent()) return;
	hist_events -> Fill(1.5);//number of selected events

    Double_t signB = fAODevent->GetMagneticField()/TMath::Abs(fAODevent->GetMagneticField());

    //Loop over each track
    for (Int_t i=0 ; i<fAODevent->GetNumberOfTracks() ; i++)  {

        AliAODTrack *track = (AliAODTrack*) fAODevent -> GetTrack(i);
        if ( !track ) continue;
        if ( !TrackQualityCuts(track) ) continue;

        hist_TPC->Fill(track->P(),track->GetTPCsignal());//TPC signal
        hist_TOF->Fill(track->P(),Beta(track));//TOF signal

		//Pt (to make things easier)
		Double_t pt = track->Pt();
		Double_t eta = track->Eta();
        Double_t DCAz  = TMath::Abs(GetDCAZ (track));
        Double_t DCAxy = TMath::Abs(GetDCAXY (track));

        if (DCAz<1.0)  {

            if(Proton_Candidate(track)) {
                if (track->Charge()>0) hist_DCAXY_Proton     -> Fill (pt,DCAxy);
                if (track->Charge()<0) hist_DCAXY_AntiProton -> Fill (pt,DCAxy);
            	}            

            if(Deuteron_Candidate(track)) {
                if (track->Charge()>0) hist_DCAXY_Deuteron     -> Fill (pt,DCAxy);
                if (track->Charge()<0) hist_DCAXY_AntiDeuteron -> Fill (pt,DCAxy);
            	}
    
            if(He3_Candidate(track)) {
                if (track->Charge()>0) hist_DCAXY_He3     -> Fill (pt,DCAxy);
                if (track->Charge()<0) hist_DCAXY_AntiHe3 -> Fill (pt,DCAxy);
            	}
        }
        
        
        //DCA cuts
        if (DCAz>1.0) continue;
        if (DCAxy>0.1) continue;

        if (signB>0)  {

		    if(Proton_Candidate(track)) {
		        if (track->Charge()>0) hist_mass_Proton_pos     -> Fill (pt,eta,Mass(track));
		        if (track->Charge()<0) hist_mass_AntiProton_pos -> Fill (pt,eta,Mass(track));
		    	}
		    
		    if(Deuteron_Candidate(track)) {
		        if (track->Charge()>0) hist_mass_Deuteron_pos     -> Fill (pt,eta,Mass(track));
		        if (track->Charge()<0) hist_mass_AntiDeuteron_pos -> Fill (pt,eta,Mass(track));
		    	}
		    
		    if(He3_Candidate(track)) {
		        if (track->Charge()>0) hist_mass_He3_pos     -> Fill (pt,eta,Mass(track));
		        if (track->Charge()<0) hist_mass_AntiHe3_pos -> Fill (pt,eta,Mass(track));
				}

		}

        if (signB<0)  {

		    if(Proton_Candidate(track)) {
		        if (track->Charge()>0) hist_mass_Proton_neg     -> Fill (pt,eta,Mass(track));
		        if (track->Charge()<0) hist_mass_AntiProton_neg -> Fill (pt,eta,Mass(track));
		    	}
		    
		    if(Deuteron_Candidate(track)) {
		        if (track->Charge()>0) hist_mass_Deuteron_neg     -> Fill (pt,eta,Mass(track));
		        if (track->Charge()<0) hist_mass_AntiDeuteron_neg -> Fill (pt,eta,Mass(track));
		    	}
		    
		    if(He3_Candidate(track)) {
		        if (track->Charge()>0) hist_mass_He3_neg     -> Fill (pt,eta,Mass(track));
		        if (track->Charge()<0) hist_mass_AntiHe3_neg -> Fill (pt,eta,Mass(track));
				}

		}


    }//track loop
    
    PostData(1, fOutputList);
}

//_________________________________________________________________________________________________________________________________________________________________________________________________

//Get Event
Bool_t AddTaskNucleiMass_LHC15o::GetEvent(){
    
    //Get Input Event
    fAODevent = dynamic_cast <AliAODEvent*>(InputEvent());
    if (!fAODevent) return false;
    
    hist_events -> Fill(0.5);//total number number of events
    
    //Get centrality
    AliMultSelection *multiplicitySelection = (AliMultSelection*) fAODevent->FindListObject("MultSelection");
    if( !multiplicitySelection) return false;
    Double_t centrality = multiplicitySelection->GetMultiplicityPercentile("V0M", true);

	if(centrality>90) return false;
    
    //Reject vertex contributors and make 10cm cut for vertex in z direction
    AliAODVertex *vertex = (AliAODVertex*) fAODevent->GetPrimaryVertex();
    if ( !vertex ) return false;
    if ( vertex->GetNContributors() < 1 ) return false;

    if ( TMath::Abs(vertex->GetZ() ) > 10 ) return false;

    return true;
}
//_________________________________________________________________________________________________________________________________________________________________________________________________

//Get DCAXY
Double_t AddTaskNucleiMass_LHC15o::GetDCAXY (AliAODTrack *track)  {
    
    Double_t impactParameter[2];
    Double_t covarianceMatrix[3];
    if (!track->PropagateToDCA (fAODevent->GetPrimaryVertex(),fAODevent->GetMagneticField(),10000,impactParameter,covarianceMatrix)) return -999;
    
    Double_t DCAXY = impactParameter[0];
    
    return DCAXY;
}

//Get DCAZ
Double_t AddTaskNucleiMass_LHC15o::GetDCAZ (AliAODTrack *track)  {
    
    Double_t impactParameter[2];
    Double_t covarianceMatrix[3];
    if (!track->PropagateToDCA (fAODevent->GetPrimaryVertex(),fAODevent->GetMagneticField(),10000,impactParameter,covarianceMatrix) ) return -999;
    
    Double_t DCAZ = impactParameter[1];
    
    return DCAZ;
}

//Get shared clusters in ITS
Double_t AddTaskNucleiMass_LHC15o::GetSharedClusters (AliAODTrack *track) {
    
    Double_t nSharedCls(0);
    Double_t fSharedCls(0);
    
    for ( Int_t i=0 ; i<6 ; i++ )
        if ( track->HasPointOnITSLayer(i) && track->HasSharedPointOnITSLayer(i) ) nSharedCls++;
    
    fSharedCls = nSharedCls/(Double_t)track->GetNcls(0);
    
    return fSharedCls;
}


//Apply Cuts + fill track properties histograms
//_________________________________________________________________________________________________________________________________________________________________________________________________
Bool_t AddTaskNucleiMass_LHC15o::TrackQualityCuts(AliAODTrack *track){

	if(!track->TestFilterMask(AliAODTrack::kTrkGlobalNoDCA)) return false;


	//Eta
    if(TMath::Abs(track->Eta())>0.8) return false;

	//# clusters in TPC
	Double_t cluster_TPC = track->GetTPCNcls();
    if(cluster_TPC<70) return false;

	//# crossed rows/findable clusters
    Double_t crossed_rows = track->GetTPCNCrossedRows();
    Double_t findable_cluster = track->GetTPCNclsF();
    if (findable_cluster>0) {
        findable_cluster = crossed_rows/findable_cluster;
    }
    if(findable_cluster<0.8) return false;
    if(track->GetITSNcls()<2) return false;

	//# clusters used for de/dx
	Double_t TPCsignalN = track->GetTPCsignalN();
    if(TPCsignalN<50) return false;
    
	//# clusters in ITS
    Bool_t SPD1 = track->HasPointOnITSLayer(0);
    Bool_t SPD2 = track->HasPointOnITSLayer(1);
    
    if(SPD1==false&&SPD2==false) return false;

    return true;
}

//_________________________________________________________________________________________________________________________________________________________________________________________________
Double_t AddTaskNucleiMass_LHC15o::Beta(AliAODTrack *track){

	Double_t length = track->GetIntegratedLength();
	Double_t beta = 0;
      
        if (length > 350.) {
            Double_t time = track->GetTOFsignal() - fPIDResponse->GetTOFResponse().GetStartTime(track->P());
            if (time > 0) {
               beta = length / (2.99792457999999984e-02 * time);
            }
        }

	return beta;
}
//__________________________________________________________________________________________________________________________________________________________________
Double_t AddTaskNucleiMass_LHC15o::Mass(AliAODTrack *track){

	Double_t beta = Beta(track);
	Double_t gamma = 1/TMath::Sqrt(1 - beta*beta);
	Double_t mass = track->P()/TMath::Sqrt(gamma*gamma - 1);

	return mass*mass;
}
//___________________________________________________________________________________________________________________________________________________________________
Bool_t AddTaskNucleiMass_LHC15o::Proton_Candidate(AliAODTrack *track){
    
    Double_t signal_d = TMath::Abs(fPIDResponse->NumberOfSigmasTPC(track,AliPID::kProton));
    if(signal_d>3) return false;
    
    return true;
}
//__________________________________________________________________________________________________________________________________________________________________
Bool_t AddTaskNucleiMass_LHC15o::Deuteron_Candidate(AliAODTrack *track){
    
    Double_t signal_d = TMath::Abs(fPIDResponse->NumberOfSigmasTPC(track,AliPID::kDeuteron));
    if(signal_d>3) return false;
    
    return true;
}
//__________________________________________________________________________________________________________________________________________________________________
Bool_t AddTaskNucleiMass_LHC15o::He3_Candidate(AliAODTrack *track){
    
    Double_t signal_He3 = TMath::Abs(fPIDResponse->NumberOfSigmasTPC(track,AliPID::kHe3));
    if(signal_He3>3) return false;
    
    return true;
}
//__________________________________________________________________________________________________________________________________________________________________

//__________________________________________________________________________________________________________________________________________________________________

//_________________________________________________________________________________________________________________________________________________________________________________________________
void AddTaskNucleiMass_LHC15o::Terminate(Option_t *)  {
    
    fOutputList = dynamic_cast<TList*> (GetOutputData(1));
    if (!fOutputList) return;
}
//__________________________________________________________________________________________________________________________________________________________________
