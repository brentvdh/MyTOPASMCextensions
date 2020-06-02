//
// ********************************************************************
// *                                                                  *
// *                                                                  *
// * This file was obtained from Topas MC Inc under the license       *
// * agreement set forth at http://www.topasmc.org/registration       *
// * Any use of this file constitutes full acceptance of              *
// * this TOPAS MC license agreement.                                 *
// *                                                                  *
// ********************************************************************
// Author: Brent van der Heyden (May 29, 2020)

#ifndef LogicalOperatorVolume_hh
#define LogicalOperatorVolume_hh

#include "TsVGeometryComponent.hh"

class LogicalOperatorVolume : public TsVGeometryComponent
{
public:
	LogicalOperatorVolume(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
				  TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name);
	~LogicalOperatorVolume();

	G4VPhysicalVolume* Construct();
};

#endif
