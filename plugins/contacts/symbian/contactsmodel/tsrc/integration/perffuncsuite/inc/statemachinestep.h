/*
* Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
* Contact: http://www.qt-project.org/legal
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/




/**
 @file 
 @publishedAll
 @released
*/
 
#ifndef __STATEMACHINE_STEP_H__
#define __STATEMACHINE_STEP_H__

#include <test/testexecutestepbase.h>
#include "performancefunctionalitybase.h"


_LIT(KStateMachineStep,"StateMachineStep");

class CStateMachineStep : public CPerformanceFunctionalityBase
	{
public:
	CStateMachineStep();
	TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();
	virtual void InitializeL();
	virtual void Cleanup();
	void PreTestL();

//testing functions
private:
    void OpeningStateRecoverTestsL();
    void OpeningStateAsyncOpenTestsL();
    void WritableStateTestsL();
    void WritableStateSpeedDialTestsL();
    void DefaultStateCompressionTestsL();
    void DefaultStateRecoveryTestsL();
    void AsynchronousIntegrityTestL();
    void TablesClosedStateTestsL();

private:
    CContactOpenOperation* 	iContactOpenOperation1;//asynchronous open 
	TRequestStatus 			iStatus1;//status of asynchronous open
    CContactOpenOperation* 	iContactOpenOperation2;//asynchronous open 2
	TRequestStatus 			iStatus2;//status of asynchronous open 2
	};
    
#endif
