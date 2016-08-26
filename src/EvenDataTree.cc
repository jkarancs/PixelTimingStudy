#include "../interface/EventDataTree.h"

////////////////////
// Tree branching //
////////////////////

void EventDataTree::defineEventTreeBranches(TTree*& eventTree, EventData& eventField)
{
	eventTree -> Branch("event", &eventField, eventField.list.c_str());
}

////////////////////////////////////
// Tree branch address assignment //
////////////////////////////////////

void EventDataTree::setEventTreeDataFields(TTree*& eventTree, EventData& eventField)
{
	eventTree -> SetBranchAddress("event", &eventField);	
}