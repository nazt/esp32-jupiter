import { Signer } from "ethers"
import { DustBoyNFT, Redemption } from "../../typechain"
const hre = require("hardhat")
const { network, ethers } = require("hardhat")
async function main() {
    let accounts: Signer[] = await hre.ethers.getSigners()
    const { deployer } = await hre.getNamedAccounts()
    console.log("deployer: ", deployer)
    // const nftContract: DustBoyNFT = await hre.ethers.getContract("DustBoyNFT")
    // console.log('nftContract', nftContract)

    const nftContract: DustBoyNFT = await hre.ethers.getContract("DustBoyNFT")
    // const tx = await nftContract.mint()
    // console.log('tx: ', tx)
    // console.log(await tx.wait())
    // const tx = await nftContract.mint(10);
    // console.log(await tx.wait())
    // console.log(nftContract)

    const redemptionContract: Redemption = await ethers.getContract("Redemption")
    console.log(redemptionContract)

    console.log("getItems", deployer)
    console.log(await redemptionContract.getItems(deployer))
    console.log(await redemptionContract.getUrl(deployer))

    // console.log(await nftContract.sensors[])
    // await nftContract.sensors[0]
    // nftContract["setSensor(uint256,(uint256,uint256,uint256))"](0, [0, 0, 0])
    // nftContract["setSensor(uint256,(uint256,uint256,uint256))"](0, [0, 0, 0])
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
