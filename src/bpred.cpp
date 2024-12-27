// --------------------------------------------------------------------- //
// For part B, you will need to modify this file.                        //
// You may add any code you need, as long as you correctly implement the //
// three required BPred methods already listed in this file.             //
// --------------------------------------------------------------------- //

// bpred.cpp
// Implements the branch predictor class.

#include "bpred.h"
#include <iostream>
/**
 * Construct a branch predictor with the given policy.
 * 
 * In part B of the lab, you must implement this constructor.
 * 
 * @param policy the policy this branch predictor should use
 */
BPred::BPred(BPredPolicy policy)
{
    // TODO: Initialize member variables here.

    // As a reminder, you can declare any additional member variables you need
    // in the BPred class in bpred.h and initialize them here.

    stat_num_branches = 0;
    stat_num_mispred = 0;
    gshare.ghr = 0;

    for (unsigned int i = 0; i < 4096; i++)
    {
        pht[i].state = 2;
    }

    if (policy == BPRED_ALWAYS_TAKEN)
    {
        //printf("policy is ALWAYS TAKEN, %d\n", policy);
        this->policy = policy;
    }
    else if (policy == BPRED_GSHARE)
    {
        //printf("policy is GSHARE\n");
        this->policy = policy;
    }
}

/**
 * Get a prediction for the branch with the given address.
 * 
 * In part B of the lab, you must implement this method.
 * 
 * @param pc the address (program counter) of the branch to predict
 * @return the prediction for whether the branch is taken or not taken
 */
BranchDirection BPred::predict(uint64_t pc)
{
    // TODO: Return a prediction for whether the branch at address pc will be
    // TAKEN or NOT_TAKEN according to this branch predictor's policy.

    // Note that you do not have to handle the BPRED_PERFECT policy here; this
    // function will not be called for that policy.

    if (this->policy == BPRED_GSHARE)
    {
        gshare.pc = pc % 4096;
        gshare.index = gshare.ghr ^ gshare.pc;

        if (pht[gshare.index].state == 0 || pht[gshare.index].state == 1)
            return NOT_TAKEN;
        else
            return TAKEN;
    }
    else    /* always taken */
        return TAKEN; 
}

/**
 * Update the branch predictor statistics (stat_num_branches and
 * stat_num_mispred), as well as any other internal state you may need to
 * update in the branch predictor.
 * 
 * In part B of the lab, you must implement this method.
 * 
 * @param pc the address (program counter) of the branch
 * @param prediction the prediction made by the branch predictor
 * @param resolution the actual outcome of the branch
 */
void BPred::update(uint64_t pc, BranchDirection prediction,
                   BranchDirection resolution)
{
    // TODO: Update the stat_num_branches and stat_num_mispred member variables
    // according to the prediction and resolution of the branch.

    // Note that you do not have to handle the BPRED_PERFECT policy here; this
    // function will not be called for that policy.

    stat_num_branches++;

    if (prediction != resolution)
        stat_num_mispred++;

    // TODO: Update any other internal state you may need to keep track of.
    if (resolution == NOT_TAKEN)
    {
        if (pht[gshare.index].state > 0)
            pht[gshare.index].state -= 1;
        gshare.ghr = gshare.ghr << 1;
    }
    else if (resolution == TAKEN)
    {
        if (pht[gshare.index].state < 3)
            pht[gshare.index].state += 1;
        gshare.ghr = gshare.ghr << 1;
        gshare.ghr += 1;
    }
}
