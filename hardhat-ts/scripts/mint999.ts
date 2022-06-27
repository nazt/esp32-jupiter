import { BigNumber, ContractTransaction, Signer } from 'ethers';
import { getNamedAccounts } from 'hardhat';
import { OracleNFT } from '../typechain';
// import { MyOracle } from '../typechain';
const fetch = require('node-fetch');
const hre = require('hardhat');
async function main() {
    let accounts: Signer[] = await hre.ethers.getSigners();
    let contract: OracleNFT;

    contract = await hre.ethers.getContract('OracleNFT');
    console.log('address', contract.address)
    const tx = await contract.mint(0xdeadbeef)
    console.log(tx, 'minted');

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
