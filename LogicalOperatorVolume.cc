// Component for LogicalOperatorVolume
//
// ********************************************************************
// *                                                                  *
// * This file was obtained from Topas MC Inc under the license       *
// * agreement set forth at http://www.topasmc.org/registration       *
// * Any use of this file constitutes full acceptance of              *
// * this TOPAS MC license agreement.                                 *
// *                                                                  *
// ********************************************************************
// Author: Brent van der Heyden (May 29, 2020)                        *
// ********************************************************************

#include "LogicalOperatorVolume.hh"
#include "TsParameterManager.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"

#include "G4VSolid.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4RotationMatrix.hh"

#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"

#include "G4SystemOfUnits.hh"

LogicalOperatorVolume::LogicalOperatorVolume(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
			 TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name) :
TsVGeometryComponent(pM, eM, mM, gM, parentComponent, parentVolume, name)
{
}


LogicalOperatorVolume::~LogicalOperatorVolume()
{
}


G4VPhysicalVolume* LogicalOperatorVolume::Construct()
{
	BeginConstruction();

	// Specify type of locial operation
	G4String logicalOperation = fPm->GetStringParameter(GetFullParmName("Operation")); // Subtraction, Union
	if (logicalOperation!="Union" || logicalOperation!="Subtraction" || logicalOperation!="Intersection") {
		G4cerr << "Logical operation of unknown type." << G4endl;
	}

  // Specify mother volume type and perform user check
  G4String motherType   = fPm->GetStringParameter(GetFullParmName("Type_mother"));
	if (motherType!="TsBox" || motherType!="TsCylinder" || motherType != "TsSphere") {
		G4cerr << "Mother volume of unknown type." << G4endl;
	}

  // Specify daughter volume type and perform user check
	G4String daughterType = fPm->GetStringParameter(GetFullParmName("Type_daughter"));
	if (daughterType!="TsBox" || daughterType!="TsCylinder" || daughterType != "TsSphere") {
		G4cerr << "Daughter volume of unknown type." << G4endl;
	}

	// Specify translation and rotation properties relative to each other
  G4double relRotX = fPm->GetDoubleParameter(GetFullParmName("RelRotX"), "Angle");
	G4double relRotY = fPm->GetDoubleParameter(GetFullParmName("RelRotY"), "Angle");
	G4double relRotZ = fPm->GetDoubleParameter(GetFullParmName("RelRotZ"), "Angle");

	G4double relTransX = fPm->GetDoubleParameter(GetFullParmName("RelTransX"), "Length");
	G4double relTransY = fPm->GetDoubleParameter(GetFullParmName("RelTransY"), "Length");
	G4double relTransZ = fPm->GetDoubleParameter(GetFullParmName("RelTransZ"), "Length");

  // Read parameters of mother volume
	G4VSolid* motherVolume;
	G4double  motherHLX, motherHLY, motherHLZ; // BOX
	G4double  motherRMin,motherRMax, motherSPhi, motherDPhi, motherHL, motherSTheta, motherDTheta; // Cylinder & Sphere

	if (motherType=="TsBox"){
		// Read parameters for mother box
	  motherHLX = fPm->GetDoubleParameter(GetFullParmName("HLX_mother"), "Length");
		motherHLY = fPm->GetDoubleParameter(GetFullParmName("HLY_mother"), "Length");
		motherHLZ = fPm->GetDoubleParameter(GetFullParmName("HLZ_mother"), "Length");

    motherVolume = new G4Box("MotherVolume", motherHLX, motherHLY, motherHLZ);

	} else if (motherType=="TsCylinder"){
		// Read parameters for mother cylinder
    motherRMin = 0.0*cm; //otherwise, tricky situation
		motherRMax = fPm->GetDoubleParameter(GetFullParmName("RMax_mother"), "Length");
		motherSPhi = fPm->GetDoubleParameter(GetFullParmName("SPhi_mother"), "Angle");
		motherDPhi = fPm->GetDoubleParameter(GetFullParmName("DPhi_mother"), "Angle");
		motherHL   = fPm->GetDoubleParameter(GetFullParmName("HL_mother")  , "Length");

		motherVolume = new G4Tubs("MotherVolume", motherRMin, motherRMax, motherHL, motherSPhi, motherDPhi);

	} else if (motherType=="TsSphere") {
		// Read parameters for mother sphere
    motherRMin   = 0.0*cm; //otherwise, tricky situation
		motherRMax   = fPm->GetDoubleParameter(GetFullParmName("RMax_mother")  , "Length");
		motherSPhi   = fPm->GetDoubleParameter(GetFullParmName("SPhi_mother")  , "Angle");
		motherDPhi   = fPm->GetDoubleParameter(GetFullParmName("DPhi_mother")  , "Angle");
		motherSTheta = fPm->GetDoubleParameter(GetFullParmName("STheta_mother"), "Angle");
		motherDTheta = fPm->GetDoubleParameter(GetFullParmName("DTheta_mother"), "Angle");

		motherVolume = new G4Sphere("MonterVolume", motherRMin, motherRMax, motherSPhi, motherDPhi, motherSTheta, motherDTheta);

	}

 // Read parameters of daughter volume
 G4VSolid* daughterVolume;
 G4double  daughterHLX, daughterHLY, daughterHLZ; // BOX
 G4double  daughterRMin,daughterRMax, daughterSPhi, daughterDPhi, daughterHL, daughterSTheta, daughterDTheta; // Cylinder & Sphere
 if (daughterType=="TsBox"){
	  // Read parameters for daughter box
		daughterHLX = fPm->GetDoubleParameter(GetFullParmName("HLX_daughter"), "Length");
		daughterHLY = fPm->GetDoubleParameter(GetFullParmName("HLY_daughter"), "Length");
		daughterHLZ = fPm->GetDoubleParameter(GetFullParmName("HLZ_daughter"), "Length");

		daughterVolume = new G4Box("DaugterVolume", daughterHLX, daughterHLY, daughterHLZ);

 } else if (daughterType=="TsCylinder") {
	  // Read parameters for daughter cylinder
	  daughterRMin = 0.0*cm; //otherwise, tricky situation
 	  daughterRMax = fPm->GetDoubleParameter(GetFullParmName("RMax_daughter"), "Length");
	  daughterSPhi = fPm->GetDoubleParameter(GetFullParmName("SPhi_daughter"), "Angle");
	  daughterDPhi = fPm->GetDoubleParameter(GetFullParmName("DPhi_daughter"), "Angle");
	  daughterHL   = fPm->GetDoubleParameter(GetFullParmName("HL_daughter")  , "Length");

		daughterVolume = new G4Tubs("DaughterVolume", daughterRMin, daughterRMax, daughterHL, daughterSPhi, daughterDPhi);

 } else if (daughterType=="TsSphere") {
	 // Read parameters for daughter sphere
	 daughterRMin   = 0.0*cm; //otherwise, tricky situation
	 daughterRMax   = fPm->GetDoubleParameter(GetFullParmName("RMax_daughter")  , "Length");
	 daughterSPhi   = fPm->GetDoubleParameter(GetFullParmName("SPhi_daughter")  , "Angle");
	 daughterDPhi   = fPm->GetDoubleParameter(GetFullParmName("DPhi_daughter")  , "Angle");
	 daughterSTheta = fPm->GetDoubleParameter(GetFullParmName("STheta_daughter"), "Angle");
	 daughterDTheta = fPm->GetDoubleParameter(GetFullParmName("DTheta_daughter"), "Angle");

	 daughterVolume = new G4Sphere("DaughterVolume", daughterRMin, daughterRMax, daughterSPhi, daughterDPhi, daughterSTheta, daughterDTheta);

 }

  G4RotationMatrix* rotMatrix   = new G4RotationMatrix();
	rotMatrix->rotateX(relRotX);
	rotMatrix->rotateY(relRotY);
	rotMatrix->rotateZ(relRotZ);

	const G4ThreeVector transMatrix(relTransX,relTransY,relTransZ);

  G4VSolid* fEnvelopeSolid;
  if (logicalOperation=="Subtraction") {
		fEnvelopeSolid = new G4SubtractionSolid(fName,motherVolume,daughterVolume,rotMatrix,transMatrix);
	}

	if (logicalOperation=="Union") {
		fEnvelopeSolid = new G4UnionSolid(fName,motherVolume,daughterVolume,rotMatrix,transMatrix);
	}

	if (logicalOperation=="Intersection") {
		fEnvelopeSolid = new G4IntersectionSolid(fName,motherVolume,daughterVolume,rotMatrix,transMatrix);
	}


	fEnvelopeLog  = CreateLogicalVolume(fEnvelopeSolid);
	fEnvelopePhys = CreatePhysicalVolume(fEnvelopeLog);

	InstantiateChildren(fEnvelopePhys);

	return fEnvelopePhys;
}
