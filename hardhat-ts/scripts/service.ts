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
    // console.log('address', contract.address)

    // contract.on(contract.filters.AmbientChanged(), (ambient) => {
    //     console.log(ambient)
    // })
    // console.log(contract)

    const measureStartBlock = 15033763;
    const ambientEvents = (await contract.queryFilter(contract.filters['AmbientChanged(uint256,uint256,uint256)'](), measureStartBlock, await hre.ethers.provider.getBlockNumber())).sort((a, b) => b.blockNumber - a.blockNumber);

    // console.log('events', ambientEvents.length)
    const allEvents = ambientEvents.map(it => [it.blockNumber, it.args[0].toString(), it.args[1].toNumber()/100])
    const last10 = allEvents.slice(0, 10).reverse();

    const lastTxTS = await hre.ethers.provider.getBlock(ambientEvents[0].blockNumber).then( (it:any) => it.timestamp)


    console.log(last10, `total ${allEvents.length}`,contract.address, ambientEvents[0].blockNumber, 'lastTxTS', new Date(lastTxTS * 1000).toLocaleString())
    let lines = ''
    for (const tx of allEvents.reverse()) {
        lines += tx.join(',') + '\n'
    }

    console.log(lines)

    // ambientEvents[0]

}

async function run() {
    // while (true) {
        await main();
    // }
}

if (require.main === module) {
    run()
        .then(() => process.exit(0))
        .catch((error) => {
            console.error(error);
            process.exit(1);
        });
} else {
    module.exports = main;
}
