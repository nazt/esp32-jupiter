import { BigNumber, ContractTransaction, Signer } from 'ethers';
import { getNamedAccounts } from 'hardhat';
import { OracleNFT, OracleNFT__factory } from '../typechain';
// import { MyOracle } from '../typechain';
const fetch = require('node-fetch');
const hre = require('hardhat');
async function main() {
    let accounts: Signer[] = await hre.ethers.getSigners();
    let contract: OracleNFT;
    contract = await hre.ethers.getContract('OracleNFT');
    console.log('address', contract.address)

    // contract.on(contract.filters.AmbientChanged(), (ambient) => {
    //     console.log(ambient)
    // })
    // console.log(contract)

    const transferEvents = await contract.queryFilter(contract.filters['AmbientChanged(uint256,uint256,uint256)'](), 15024268, await hre.ethers.provider.getBlockNumber())
    console.log(transferEvents)
}

if (require.main === module) {
    main()
        .then(() => process.exit(0))
        .catch((error) => {
            console.error(error);
            process.exit(1);
        });
} else {
    module.exports = main;
}
