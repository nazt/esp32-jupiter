import { Signer } from "ethers"
import { DustBoyNFT } from "../typechain"
// import { MyOracle } from '../typechain';
const fetch = require("node-fetch")
const hre = require("hardhat")
async function main() {
    let accounts: Signer[] = await hre.ethers.getSigners()
    let contract: DustBoyNFT

    contract = await hre.ethers.getContract("DustBoyNFT")
    console.log("address", contract.address)

    const data = await fetch(
        "https://www-old.cmuccdc.org//assets/api/haze/pwa/json/allstation.json",
    )
    const stations = await data.json()

    const promisses = []
    for (const s of stations) {
        // console.log(s)
        // console.log(s.id, s.dustboy_id, s.dustboy_uri, s.pm10, s.pm25, s.dustboy_lat, s.dustboy_lon)
        try {
            const tx = await contract.mint(s.dustboy_id)
            console.log(s.dustboy_id, s.dustboy_name_th, tx.hash)
        } catch (e) {
            console.warn("duplicate", s.dustboy_id)
        }
    }

    console.log(stations.length)

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
        .catch(error => {
            console.error(error)
            process.exit(1)
        })
} else {
    module.exports = main
}
