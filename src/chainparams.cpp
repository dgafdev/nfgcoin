// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 **/
   CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1536590975, nBits=1d00ffff, nNonce=2083236893, vtx=1)
     CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
       CTxIn(COutPoint(000000, -1), coinbase 041ed50505871f09bc46809aa43d8d23c5a8cf5fd5f3ecb504a610b7818f11f3cbbd9db3e259b888802244ab4e2361bb4c33464ade9f1d0b6a0a1cd08c998f049c)
      CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
     vMerkleTree: 4a5e1e
 
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const CScript genesisOutputScript = CScript() << ParseHex("041ed50505871f09bc46809aa43d8d23c5a8cf5fd5f3ecb504a610b7818f11f3cbbd9db3e259b888802244ab4e2361bb4c33464ade9f1d0b6a0a1cd08c998f049c") << OP_CHECKSIG;
    const char* pszTimestamp = "The Onion 10/Sep/2018 Whoa, Classmate Got Totally Hideous Over Summer Vacation";
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 1225000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("b5e2664e1ae77de8c8548cf2f17b67379a57044266456045a0ad558e2fde8d9d");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 10.5 * 60; // 1.1 days
        consensus.nPowTargetSpacing = 1.5 * 60; // 1.5 minutes
        consensus.nPowTargetTimespanDigisheld = 1.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 7560; // 75% of 10080
        consensus.nMinerConfirmationWindow = 10080; // 3.5 days / nPowTargetSpacing * 4 * 0.75
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1488931200; // March 8, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1520467200; // March 8, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1488931200; // March 8, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1520467200; // March 8, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000000000100010001");

        // By default assume that the signatures in ancestors of this block are valid.
        //consensus.defaultAssumeValid = uint256S("0xd1c1bdae051d11b7c3bb1e26bbc473eeecc6dcc2464005f2c9838cda47ebfb9d"); // 302536

        // Hard premine (no difficulty retargeting) 445,000 blocks * 50 = 22,250,000 NFGCoins
        // Soft premine (normal difficulty retargeting) 5,000 blocks * 50 = 250,000 NFGCoins
        consensus.premineBlocks = 400000;

        // Hardfork params
        nSwitchKGWblock = 0;
        nSwitchDIGIblock = 0;
        nSwitchLyra2REv2_DGW = 0;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xB8;
        pchMessageStart[1] = 0x67;
        pchMessageStart[2] = 0x1B;
        pchMessageStart[3] = 0x81;
        nDefaultPort = 9543;
        nPruneAfterHeight = 100000;
        vAlertPubKey = ParseHex("04c193104567734547d76c6fef6b2b48385d610e00a47efb21a7e52923afa5b5def904d50b797e53c4a4836f4c504856cf1639f130c0f5c2f758db9ba73d6a269e");

        genesis = CreateGenesisBlock(1536590975, 263152, 0x1e0ffff0, 1, 5000 * COIN);


		
	/*	hashGenesisBlock = uint256("0x01");
		
		if (true && genesis.GetHash() != hashGenesisBlock)
			{
				Logprintf("recalculating params for mainnet.\n");
				Logprintf("old mainnet genesis nonce: %s\n", genesis.nNonce.ToString().c_str());
				Logprintf("old mainnet genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
				// deliberately empty for loop finds nonce value.
				for(genesis.nNonce == 0; genesis.GetHash() > bnProofOfWorkLimit; genesis.nNonce++){ } 
				Logprintf("new mainnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
				Logprintf("new mainnet genesis nonce: %s\n", genesis.nNonce.ToString().c_str());
				Logprintf("new mainnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
			}

	*/

        consensus.hashGenesisBlock = genesis.GetHash();

        //assert(consensus.hashGenesisBlock == uint256S("0xc699ca19d51c98fd39a188570afc6e73237682a57407ea6c7f2d22bdb6ae35e6"));
        //assert(genesis.hashMerkleRoot == uint256S("0x6cecc2669293484fcb9ac76d1e937b7aee80e49046b67e43bc1d8e403b0498da"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        // vSeeds.emplace_back("dnsseed.nfgcoin.com", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 112);  // n
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,55);  // P
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,176);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        base58Prefixes[OLD_SECRET_KEY] = std::vector<unsigned char>(1,178);

        vFixedSeeds.clear();

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {};

        chainTxData = ChainTxData{
            // Data as of block b8f1e3efdbaec9b2adf722a9ad3667c7368e222e044123bfc00dc2054e373bea (height 447457).
            1536590975, // * UNIX timestamp of last known number of transactions
            0,          // * total number of transactions between genesis and that timestamp
            //   (the tx=... number in the SetBestChain debug.log lines)
            0.025 // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v4)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 99225000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("4df73b297d37d41e66f50780a1ce671c6cafb319fb6676aa5b2af0ea09fd1d0b");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 10.5 * 60; // 1.1 days
        consensus.nPowTargetSpacing = 1.5 * 60; // 1.5 minutes
        consensus.nPowTargetTimespanDigisheld = 1.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 75; // 75% for testchains
        consensus.nMinerConfirmationWindow = 100; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1488931200; // March 8, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1520467200; // March 8, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1488931200; // March 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1520467200; // March 8, 2018
        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0");

        // By default assume that the signatures in ancestors of this block are valid.
        //consensus.defaultAssumeValid = uint256S("0x4df73b297d37d41e66f50780a1ce671c6cafb319fb6676aa5b2af0ea09fd1d0b");

        // No premine in testnet
        consensus.premineBlocks = 0;

        pchMessageStart[0] = 0x44;
        pchMessageStart[1] = 0x3E;
        pchMessageStart[2] = 0xB8;
        pchMessageStart[3] = 0xE3;

        // Hardfork params
        nSwitchKGWblock = 0;
        nSwitchDIGIblock = 0;
        nSwitchLyra2REv2_DGW = 0;

        nDefaultPort = 19543;
        nPruneAfterHeight = 1000;
        vAlertPubKey = ParseHex("047a94d1a9a5186d831852a1055c6283fc15411af97ad22b4f7b75bbf2c1051b79c33b3fc1dedaec8239974eabb353b3911671934a93af231646bbb88bb6452dce");

        genesis = CreateGenesisBlock(1524130420, 62288, 0x1e0ffff0, 1, 5000 * COIN);


/*
		hashGenesisBlock = uint256("0x01");
if (true && genesis.GetHash() != hashGenesisBlock)
        {
            Logprintf("recalculating params for mainnet.\n");
            Logprintf("old testnet genesis nonce: %s\n", genesis.nNonce.ToString().c_str());
            Logprintf("old testnet genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
            // deliberately empty for loop finds nonce value.
            for(genesis.nNonce == 0; genesis.GetHash() > bnProofOfWorkLimit; genesis.nNonce++){ } 
            Logprintf("new testnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
            Logprintf("new testnet genesis nonce: %s\n", genesis.nNonce.ToString().c_str());
            Logprintf("new testnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
        }
*/



        consensus.hashGenesisBlock = genesis.GetHash();

        //assert(consensus.hashGenesisBlock == uint256S("0x4df73b297d37d41e66f50780a1ce671c6cafb319fb6676aa5b2af0ea09fd1d0b"));
        //assert(genesis.hashMerkleRoot == uint256S("0x6cecc2669293484fcb9ac76d1e937b7aee80e49046b67e43bc1d8e403b0498da"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        // vSeeds.emplace_back("testnet-dnsseed.nfgcoin.com", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,112);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,117);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        base58Prefixes[OLD_SECRET_KEY] = std::vector<unsigned char>(1,239);

        vFixedSeeds.clear();

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = {};

        chainTxData = ChainTxData{
            1536590975,
            0,
            250};
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = -1; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 33000; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 33000; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 10.5 * 60; // 1.5 days
        consensus.nPowTargetSpacing = 1.5 * 60; // 1.5 minutes
        consensus.nPowTargetTimespanDigisheld = 1.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        // No premine in regtest
        consensus.premineBlocks = 0;

        // Hardfork params
        nSwitchKGWblock = 0;
        nSwitchDIGIblock = 0;
        nSwitchLyra2REv2_DGW = 0;

        pchMessageStart[0] = 0xB9;
        pchMessageStart[1] = 0xCC;
        pchMessageStart[2] = 0xC3;
        pchMessageStart[3] = 0x2A;
        nDefaultPort = 9543;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1296688602, 3, 0x207fffff, 1, 5000 * COIN);


/*
		hashGenesisBlock = uint256("0x01");
if (true && genesis.GetHash() != hashGenesisBlock)
        {
            Logprintf("recalculating params for regtest.\n");
            Logprintf("old regtest genesis nonce: %s\n", genesis.nNonce.ToString().c_str());
            Logprintf("old regtest genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
            // deliberately empty for loop finds nonce value.
            for(genesis.nNonce == 0; genesis.GetHash() > bnProofOfWorkLimit; genesis.nNonce++){ } 
            Logprintf("new regtest genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
            Logprintf("new regtest genesis nonce: %s\n", genesis.nNonce.ToString().c_str());
            Logprintf("new regtest genesis hash: %s\n", genesis.GetHash().ToString().c_str());
        }

*/


        consensus.hashGenesisBlock = genesis.GetHash();

        //assert(consensus.hashGenesisBlock == uint256S("0x719548bfb318084da500e0e4b6fea5d00a79300f2e33ed54a10bc4f477e91710"));
        //assert(genesis.hashMerkleRoot == uint256S("0x6cecc2669293484fcb9ac76d1e937b7aee80e49046b67e43bc1d8e403b0498da"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {};

        chainTxData = ChainTxData{
            1536590975,
            0,
            0};

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,112);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,117);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        base58Prefixes[OLD_SECRET_KEY] = std::vector<unsigned char>(1,239);
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
