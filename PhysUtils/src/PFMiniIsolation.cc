/*******************************************************************************
*
*  Filename    : PFIsolation.cc
*  Description : MiniIsolation Algorithm
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

namespace usr {

double
PFMiniIsolation(
  const edm::Handle<pat::PackedCandidateCollection>& pfcands,
  const reco::Candidate*                             ptcl,
  const double                                       r_iso_min,
  const double                                       r_iso_max,
  const double                                       kt_scale,
  const bool                                         charged_only )
{
  if( ptcl->pt() < 5. ){ return 99999.;}

  double deadcone_nh = 0.;
  double deadcone_ch = 0.;
  double deadcone_ph = 0.;
  double deadcone_pu = 0.;

  if( ptcl->isElectron() ){
    if( fabs( ptcl->eta() ) > 1.479 ){
      deadcone_ch = 0.015;
      deadcone_pu = 0.015;
      deadcone_ph = 0.08;
    }
  } else if( ptcl->isMuon() ){
    deadcone_ch = 0.0001;
    deadcone_pu = 0.01;
    deadcone_ph = 0.01;
    deadcone_nh = 0.01;
  } else {
    // deadcone_ch = 0.0001; deadcone_pu = 0.01; deadcone_ph = 0.01;deadcone_nh = 0.01; // maybe use muon cones??
  }

  double iso_nh = 0.;
  double iso_ch = 0.;
  double iso_ph = 0.;
  double iso_pu = 0.;
  double ptThresh( 0.5 );
  if( ptcl->isElectron() ){ ptThresh = 0; }
  const double r_iso = std::max( r_iso_min, std::min( r_iso_max, kt_scale/ptcl->pt() ) );

  for( const pat::PackedCandidate& pfc : *pfcands ){
    if( abs( pfc.pdgId() ) < 7 ){ continue;}
    const double dr = reco::deltaR( pfc, *ptcl );
    if( dr > r_iso ){ continue;}
    //////////////////  NEUTRALS  /////////////////////////
    if( pfc.charge() == 0 ){
      if( pfc.pt() > ptThresh ){
        /////////// PHOTONS ////////////
        if( abs( pfc.pdgId() ) == 22 ){
          if( dr < deadcone_ph ){ continue;}
          iso_ph += pfc.pt();
          /////////// NEUTRAL HADRONS ////////////
        } else if( abs( pfc.pdgId() ) == 130 ){
          if( dr < deadcone_nh ){ continue;}
          iso_nh += pfc.pt();
        }
      }
      //////////////////  CHARGED from PV  /////////////////////////
    } else if( pfc.fromPV() > 1 ){
      if( abs( pfc.pdgId() ) == 211 ){
        if( dr < deadcone_ch ){ continue;}
        iso_ch += pfc.pt();
      }
      //////////////////  CHARGED from PU  /////////////////////////
    } else {
      if( pfc.pt() > ptThresh ){
        if( dr < deadcone_pu ){ continue;}
        iso_pu += pfc.pt();
      }
    }
  }

  double iso = 0.;
  if( charged_only ){
    iso = iso_ch;
  } else {
    iso  = iso_ph + iso_nh;
    iso -= 0.5*iso_pu;
    if( iso > 0 ){ iso += iso_ch;} else { iso = iso_ch;}
  }
  iso = iso/ptcl->pt();

  return iso;
}

}/* usr */
