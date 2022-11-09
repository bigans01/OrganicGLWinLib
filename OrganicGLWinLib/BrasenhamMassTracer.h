#pragma once

#ifndef BRASENHAMMASSTRACER_H
#define BRASENHAMMASSTRACER_H

#include "MassGridArray.h"
#include "BrasenhamMassTracerDim.h"
#include "BrasenhamLineData.h"

class BrasenhamMassTracer
{
	public:
		BrasenhamMassTracer() {};
		BrasenhamMassTracer(MassGridArray* in_massGridArrayRef, 
							EnclaveKeyDef::Enclave2DKey in_twoDKeyBegin,
							EnclaveKeyDef::Enclave2DKey in_twoDKeyEnd,
							BrasenhamMassTracerDim in_massTracingDim, 
							int in_brashenhamtilesPerDim,
							int in_massTracingSelectedDimLocation,
							short in_brasenhamMaterialID) :
			massGridArrayRef(in_massGridArrayRef),
			massTracingDim(in_massTracingDim),
			twoDKeyBegin(in_twoDKeyBegin),
			twoDKeyEnd(in_twoDKeyEnd),
			brasenhamTilesPerDim(in_brashenhamtilesPerDim),
			massTracingSelectedDimLocation(in_massTracingSelectedDimLocation),
			brasenhamMaterialID(in_brasenhamMaterialID)
		{
			TileLocation tileLocationA(twoDKeyBegin.a, twoDKeyBegin.b);
			TileLocation tileLocationB(twoDKeyEnd.a, twoDKeyEnd.b);
			BrasenhamLineData generatedLineData(tileLocationA, tileLocationB);
			tracingLine = generatedLineData;
		};

		void setDownfillCrustBit(short in_downfillCrustBitValue);
		void setUpfillCrustBit(short in_upfillCrustBitValue);
		void executeRun();

	private:
		void plotLineLow();
		void plotLineHigh();
		void updateMassGridCell(EnclaveKeyDef::EnclaveKey in_key);
		EnclaveKeyDef::EnclaveKey getCellLocation(TileLocation in_tileLocation);


		short downfillCrustBitValue = 0;
		short upfillCrustBitValue = 0;
		MassGridArray* massGridArrayRef = nullptr;
		EnclaveKeyDef::Enclave2DKey twoDKeyBegin;
		EnclaveKeyDef::Enclave2DKey twoDKeyEnd;
		BrasenhamMassTracerDim massTracingDim = BrasenhamMassTracerDim::NOVAL;
		int brasenhamTilesPerDim = 0;
		int massTracingSelectedDimLocation = 0;
		short brasenhamMaterialID = 0;

		BrasenhamLineData tracingLine;
		
};

#endif
