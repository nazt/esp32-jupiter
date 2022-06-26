import { BigNumber, ContractTransaction, Signer } from 'ethers';
import { getNamedAccounts } from 'hardhat';
import { DustBoyNFT } from '../typechain';
// import { MyOracle } from '../typechain';
const fetch = require('node-fetch');
const hre = require('hardhat');
async function main() {
    let accounts: Signer[] = await hre.ethers.getSigners();
    let contract: DustBoyNFT;

    contract = await hre.ethers.getContract('DustBoyNFT');
    console.log('address', contract.address)
    // const { deployer, tokenOwner } = await getNamedAccounts();

    // await contract.deployed();
    // console.log('...', contract.address);

    const data = await fetch('https://www-old.cmuccdc.org/assets/api/haze/pwa/json/stations.json')
    const stations = await data.json();


    // const promisses = []
    for (const s of stations) {
        console.log(s.id, s.dustboy_id, s.dustboy_uri, s.pm10, s.pm25, s.dustboy_lat, s.dustboy_lon);
        // promisses.push(contract.mint(s.id))
        const tx = await contract.mint(s.id)
        console.log(`tx: ${tx.hash}`, tx)
        // await contract.addLocation(s.dustboy_lat, s.dustboy_lon, s.dustboy_id);
    }

    // await Promise.allSettled(promisses).then(results => {
    //     console.log(results)
    // })

    // console.log(await contract.getAllLocations());

    // const results2: BigNumber[] = await contract.getArr();
    // console.log('results', results2.map(x => x.toString()))
    // console.log('tx', tx)

    // console.log(await contract.locations)
    // const c = await contract.locations.call()
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
